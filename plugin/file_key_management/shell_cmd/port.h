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

#include <dlfcn.h> // dladdr
#include <sys/stat.h>
#include <sys/time.h>
#include <algorithm> // std::accumulate

void ExecShellCommand(const std::string &cmd, std::string *message);

bool PathExists(const std::string &path);

// TODO(yuyang18): If the functions below are needed by other files, move them
// to paddle::filesystem namespace.

bool FileExists(const std::string &filepath);

std::string DirName(const std::string &filepath);

void MkDir(const char *path);

void MkDirRecursively(const char *fullpath);