//
// Created by dongbo on 2022/7/5.
//
// Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.
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

#pragma once

#include <iostream>
#include <time.h>

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>

#include <dlfcn.h>    // dladdr
#include <sys/stat.h>
#include <sys/time.h>
#include <algorithm>    // std::accumulate

void ExecShellCommand(const std::string &cmd, std::string *message) {
  char buffer[128];

  std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);

  if (!pipe) {
    std::cout << "error running command: " << cmd;
    return;
  }
  while (!feof(pipe.get())) {
    if (fgets(buffer, 128, pipe.get()) != nullptr) {
      *message += buffer;
    }
  }
}

bool PathExists(const std::string &path) {

  struct stat statbuf;
  if (stat(path.c_str(), &statbuf) != -1) {
    if (S_ISDIR(statbuf.st_mode)) {
      return true;
    }
  }

  return false;
}

// TODO(yuyang18): If the functions below are needed by other files, move them
// to paddle::filesystem namespace.

constexpr char kSEP = '/';

bool FileExists(const std::string &filepath) {
  struct stat buffer;
  return (stat(filepath.c_str(), &buffer) == 0);
}

std::string DirName(const std::string &filepath) {
  auto pos = filepath.rfind(kSEP);
  if (pos == std::string::npos) {
    return "";
  }
  return filepath.substr(0, pos);
}

 void MkDir(const char *path) {
  std::string path_error(path);
  path_error += " mkdir failed!";

  if (mkdir(path, 0755)) {
    if (errno != EEXIST) {
      throw std::runtime_error(path_error);
    }
  }
}

 void MkDirRecursively(const char *fullpath) {
  if (*fullpath == '\0')
    return;    // empty string
  if (FileExists(fullpath))
    return;

  MkDirRecursively(DirName(fullpath).c_str());
  MkDir(fullpath);
}
