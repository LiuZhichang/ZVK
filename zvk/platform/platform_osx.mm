//
// Created by 刘致畅 on 2021/10/13.
//

#include "platform_osx.h"
#include "../ZVKInternal.h"

#include "../base/common.h"

#import <Cocoa/Cocoa.h>

#if defined(ZVK_PLATFORM_APPLE)

ZVK::ZVK_Bool ZVK::Platform_OSX::CreateWindow(ZVK::Window window) {

}

ZVK::ZVK_Bool ZVK::Platform_OSX::CreateSurface(ZVK::Instance inst, ZVK::Window window, ZVK::Surface surface,
                                               ZVK::Allocator allocator) {

}

void ZVK::Platform_OSX::DestroyWindow(ZVK::Window window) {

}


#endif