//
//  Utils.cpp
//  VulkanTemplate
//
//  Created by 刘致畅 on 2021/9/16.
//

#include "Utils.h"

#include <iostream>

void Utils::readFile(const char *filename, std::vector<char> &code) {

    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = file.tellg();
    code.resize(fileSize);
    file.seekg(0);

    file.read(code.data(), fileSize);

}
