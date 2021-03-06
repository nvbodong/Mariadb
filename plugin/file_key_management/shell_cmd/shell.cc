// Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#define GLOG_NO_ABBREVIATED_SEVERITIES    // msvc conflict logging with windows.h

#include "shell.h"
#include "printf.h"
#include <cassert>
#include <iostream>
#include <dlfcn.h>     // dladdr
#include <unistd.h>    // sleep, usleep
#include "timer.h"

namespace paddle {
    namespace framework {

        std::shared_ptr<FILE> shell_fopen(const std::string &path, const std::string &mode) {
#if defined(_WIN32) || defined(__APPLE__) || defined(PADDLE_ARM)
            return nullptr;
#else
            if (shell_verbose()) {
                std::cout << "Opening file[" << path << "] with mode[" << mode << "]";
            }
            FILE *fp;
            if (!(fp = fopen(path.c_str(), mode.c_str()))) {
                std::cout << (string::Sprintf("Failed to open file, path[%s], mode[%s].", path, mode));
            }
            return {fp, [path](FILE *fp) {
                if (shell_verbose()) {
                    std::cout << "Closing file[" << path << "]";
                }
                if (0 != fclose(fp)) {
                    std::cout << (string::Sprintf("Failed to close file, path[%s].", path));
                }
            }};
#endif
        }

        // Close all open file descriptors
        // The implementation is async signal safe
        // Mostly copy from CPython code
        static int close_open_fds_internal() {
#if defined(_WIN32) || defined(__APPLE__) || defined(PADDLE_ARM)
            return 0;
#else
            struct linux_dirent {
                long d_ino = 0;    // NOLINT
                off_t d_off;
                unsigned short d_reclen = 0;    // NOLINT
                char d_name[256];
            };

            int dir_fd = -1;
            if ((dir_fd = open("/proc/self/fd", O_RDONLY)) < 0) {
                std::cout << ("Failed to open proc/self/fd.");
                return -1;
            }
            char buffer[sizeof(linux_dirent)];

            for (;;) {
                int bytes = 0;
                if ((bytes = syscall(SYS_getdents, dir_fd, reinterpret_cast<linux_dirent *>(buffer),
                                     sizeof(buffer)))
                    < 0) {
                    std::cout << ("System call failed via syscall function.");
                    return -1;
                }

                if (bytes == 0) {
                    break;
                }

                linux_dirent *entry = NULL;

                for (int offset = 0; offset < bytes; offset += entry->d_reclen) {
                    entry = reinterpret_cast<linux_dirent *>(buffer + offset);
                    int fd = 0;
                    const char *s = entry->d_name;

                    while (*s >= '0' && *s <= '9') {
                        fd = fd * 10 + (*s - '0');
                        s++;
                    }

                    if (s != entry->d_name && fd != dir_fd && fd >= 3) {
                        close(fd);
                    }
                }
            }

            close(dir_fd);
            return 0;
#endif
        }

        static int shell_popen_fork_internal(const char *real_cmd, bool do_read, int parent_end,
                                             int child_end, bool redirect_stderr = false) {
#if defined(_WIN32) || defined(__APPLE__) || defined(PADDLE_ARM)
            return 0;
#else
            int child_pid = -1;
            // Too frequent calls to fork() makes openmpi very slow. Use vfork() instead.
            // But vfork() is very dangerous. Be careful.
            if ((child_pid = vfork()) < 0) {
                return -1;
            }

            // The following code is async signal safe (No memory allocation, no access to
            // global data, etc.)
            if (child_pid != 0) {
                return child_pid;
            }

            int child_std_end = do_read ? 1 : 0;
            close(parent_end);

            if (child_end != child_std_end) {
                dup2(child_end, child_std_end) == child_std_end;
//                std::cout << (dup2(child_end, child_std_end) == child_std_end);
                if (redirect_stderr && do_read) {
                    std::cout << (dup2(child_end, 2) == 2);
                }
                close(child_end);
            }

            close_open_fds_internal();

#if defined(PADDLE_WITH_MUSL)
            std::cout << (execl("/bin/sh", "sh", "-c", real_cmd, NULL) >= 0);
#else
           // std::cout << (
                    execl("/bin/bash", "bash", "-c", real_cmd, NULL);
                    //>= 0);
#endif
            // Note: just for compilation. the child don't run this line.
            _exit(0);
#endif
        }

        static int read_from_pipe(FILE *fp, std::string *output) {
            char buf[4096];
            while (1) {
                int n = fread(buf, 1, 4096, fp);
                if (n <= 0) {
                    break;
                }

                output->append(buf, n);
            }

            if (!feof(fp)) {
                return -1;
            }

            return 0;
        }

        std::shared_ptr<FILE> shell_popen(const std::string &cmd, const std::string &mode, int *err_no,
                                          int *status, bool redirect_stderr) {
#if defined(_WIN32) || defined(__APPLE__) || defined(PADDLE_ARM)
            return nullptr;
#else
            bool do_read = mode == "r";
            bool do_write = mode == "w";
            if (!(do_read || do_write)) {
                *err_no = -1;
                return NULL;
            }

//        std::cout << "Opening pipe[" << cmd << "] with mode[" << mode << "]";

            std::string real_cmd = "set -o pipefail; " + cmd;

            int pipe_fds[2];
            if (pipe(pipe_fds) != 0) {
                *err_no = -1;
                return NULL;
            }
            int parent_end = 0;
            int child_end = 0;

            if (do_read) {
                parent_end = pipe_fds[0];
                child_end = pipe_fds[1];
            } else if (do_write) {
                parent_end = pipe_fds[1];
                child_end = pipe_fds[0];
            }

            sighandler_t old_handler;
            old_handler = signal(SIGCHLD, SIG_DFL);

            fcntl(parent_end, F_SETFD, FD_CLOEXEC);

            int child_pid = shell_popen_fork_internal(real_cmd.c_str(), do_read, parent_end, child_end,
                                                      redirect_stderr);

            close(child_end);

            FILE *fp = NULL;
            if ((fp = fdopen(parent_end, mode.c_str())) == NULL) {
                *err_no = -1;
                signal(SIGCHLD, old_handler);
                return NULL;
            }

            return {fp, [cmd, child_pid, old_handler, err_no, status](FILE *fp) {
//                std::cout << "Closing pipe[" << cmd << "]";
                if (fclose(fp)) {
                    *err_no = -1;
                }

                int wstatus = -1;
                // don't do this before parent read data from child pipe
                // or when get the large data, it will hang!
                waitpid(child_pid, &wstatus, 0);

                if (status) {
                    *status = wstatus;
                }

                if (WIFEXITED(wstatus) || wstatus == (128 + SIGPIPE) * 256) {
                } else {
                    std::cout << ("Must not be ECHILD errno here!");
                    *err_no = -1;
                }

                signal(SIGCHLD, old_handler);
            }};
#endif
        }

        static int shell_p2open_fork_internal(const char *real_cmd, int pipein_fds[2],
                                              int pipeout_fds[2]) {
#if defined(_WIN32) || defined(__APPLE__) || defined(PADDLE_ARM)
            return 0;
#else
            int child_pid = -1;
            if ((child_pid = fork()) < 0) {
                return -1;
            }

            if (child_pid != 0) {
                return child_pid;
            }

            close(pipein_fds[0]);
            close(pipeout_fds[1]);

            if (pipein_fds[1] != 1) {
                if (dup2(pipein_fds[1], 1) != 1) {
                    return -1;
                }
                close(pipein_fds[1]);
            }

            if (pipeout_fds[0] != 0) {
                if (dup2(pipeout_fds[0], 0) != 0) {
                    return -1;
                }
                close(pipeout_fds[0]);
            }

            close_open_fds_internal();
            if (execl("/bin/sh", "sh", "-c", real_cmd, NULL) < 0) {
                return -1;
            }
            exit(127);
#endif
        }

        std::pair<std::shared_ptr<FILE>, std::shared_ptr<FILE>> shell_p2open(const std::string &cmd) {
#if defined(_WIN32) || defined(__APPLE__) || defined(PADDLE_ARM)
            return {};
#else
            if (shell_verbose()) {
                std::cout << "Opening bidirectional pipe[" << cmd << "]";
            }

            std::string real_cmd = "set -o pipefail; " + cmd;

            int pipein_fds[2];
            int pipeout_fds[2];
            if (pipe(pipein_fds) != 0) {
                return {NULL, NULL};
            }
            if (pipe(pipeout_fds) != 0) {
                return {NULL, NULL};
            }

            int child_pid = shell_p2open_fork_internal(real_cmd.c_str(), pipein_fds, pipeout_fds);

            close(pipein_fds[1]);
            close(pipeout_fds[0]);
            fcntl(pipein_fds[0], F_SETFD, FD_CLOEXEC);
            fcntl(pipeout_fds[1], F_SETFD, FD_CLOEXEC);

            std::shared_ptr<int> child_life = {
                    NULL,
                    [child_pid, cmd](void *) {
                        if (shell_verbose()) {
                            std::cout << "Closing bidirectional pipe[" << cmd << "]";
                        }

                        int wstatus, ret;

                        do {
                            std::cout << ((ret = waitpid(child_pid, &wstatus, 0)) >= 0
                                          || (ret == -1 && errno == EINTR));
                        } while (ret == -1 && errno == EINTR);

                        std::cout << (wstatus == 0 || wstatus == (128 + SIGPIPE) * 256
                                      || (wstatus == -1 && errno == ECHILD))
                                  << "status[" << wstatus << "], cmd[" << cmd << "]";

                        if (wstatus == -1 && errno == ECHILD) {
                            // temporarily remove this warning
                            // std::cout << "errno is ECHILD";
                        }
                    }
            };

            FILE *in_fp;
            std::cout << ((in_fp = fdopen(pipein_fds[0], "r")) != NULL);
            FILE *out_fp;
            std::cout << ((out_fp = fdopen(pipeout_fds[1], "w")) != NULL);
            return {{in_fp,  [child_life](FILE *fp) { std::cout << (fclose(fp) == 0); }},
                    {out_fp, [child_life](FILE *fp) { std::cout << (fclose(fp) == 0); }}};
#endif
        }

#if defined(_WIN32) || defined(__APPLE__) || defined(PADDLE_ARM)
#else

        static int _get_err_no(int err_no, int status) {
            if (err_no == 0) {
                if (WIFEXITED(status)) {
                    return WEXITSTATUS(status);
                }
                return -1;
            }

            return err_no;
        }

#endif

        static int _shell_execute_cmd(const std::string &cmd, std::string *output, int time_out,
                                      int sleep_inter, bool redirect_stderr = false) {

            int err_no = 0;
            int status = 0;
            int cmd_status = 0;
            platform::Timer timer;
            do {
//            std::cout << "exec cmd:[" << cmd << "]";

                err_no = 0;
                status = 0;
                *output = "";
                auto pipe = shell_popen(cmd, "r", &err_no, &status, redirect_stderr);

                if (err_no == 0) {
                    // read file
                    err_no = read_from_pipe(&*pipe, output);
                    if (err_no) {
                        std::cout << "status[" << status << "], cmd[" << cmd << "]"
                                  << ", err_no[" << err_no << "]";
                    }
                }

                // close file and etc.
                pipe = nullptr;
                if (err_no) {
                    std::cout << "status[" << status << "], cmd[" << cmd << "]"
                              << ", err_no[" << err_no << "]";
                }

                cmd_status = _get_err_no(err_no, status);
                // cmd run ok!
                if (cmd_status == 0) {
                    return cmd_status;
                }

                // time out
                timer.Pause();
                if ((time_out > 0 && timer.ElapsedMS() >= time_out) || time_out == 0) {
                    break;
                }
                timer.Resume();

                if (sleep_inter > 0) {
                    usleep(sleep_inter * 1000);
                }
            } while (cmd_status);

            // log when check timeout!
            if (time_out != 0) {
//            *output += string::Sprintf(
//                " _shell_execute_cmd execute cmd:%s ElapsedMS:%d, err_no:%d status:%d", cmd,
//                timer.ElapsedMS(), err_no, cmd_status);
//            std::cout << *output;
            }

            return cmd_status;
        }

        std::string shell_get_command_output(const std::string &cmd, int time_out, int sleep_inter) {
            std::string output;
            _shell_execute_cmd(cmd, &output, time_out, sleep_inter);
            return output;
        }

        std::vector<std::string> shell_execute_cmd(const std::string &cmd, int time_out,
                                                   int sleep_inter, bool redirect_stderr) {
            std::string output;
            int ret = _shell_execute_cmd(cmd, &output, time_out, sleep_inter, redirect_stderr);
            return std::vector<std::string>({string::Sprintf("%d", ret), output});
        }

    }    // end namespace framework
}    // end namespace paddle
