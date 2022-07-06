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

#include <iostream>
#include <fstream>
#include <array>
#include "fs.h"

int main() {
    std::string r;
    int err_no;
    auto fp = paddle::framework::fs_open_read(
            "hdfs://192.168.10.19:9000/temp/upload/9296762ae3814ab5af1b34075ea3a154.csv", &err_no, "");

    std::array<char, 1000> content;
    fread(const_cast<char *>(content.data()), sizeof(char), content.size(), fp.get());
    std::cout << content.begin() << std::endl;
    return 0;
}
