//
//  Utils.hpp
//  VulkanTemplate
//
//  Created by 刘致畅 on 2021/9/16.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <cstdio>
#include <fstream>
#include <vector>

class Utils {

public:
    static void readFile(const char *filename, std::vector<char> &code);

};

#endif /* Utils_hpp */
