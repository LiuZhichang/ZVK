cmake_minimum_required(VERSION 3.20)
project(zvkExample)

set(CMAKE_CXX_STANDARD 20)

SET(HEADERS_PATH ./headers/)
SET(FRAMEWORK_PATH ./framework/)

SET(LINK_Vulkan libvulkan.1.2.189.dylib)
SET(LINK_Glfw libglfw.dylib)
SET(LINK_SDL libSDL2.a)

include_directories(${HEADERS_PATH})
link_directories(${FRAMEWORK_PATH})

link_libraries(${LINK_Vulkan} ${LINK_Glfw} ${LINK_SDL})

if (APPLE)
    SET(FRAMEWORKS_Cocoa /System/Library/Frameworks/Cocoa.framework)
    SET(FRAMEWORKS_OpenGL /System/Library/Frameworks/OpenGL.framework)
    SET(FRAMEWORKS_AppKit /System/Library/Frameworks/AppKit.framework)
    SET(FRAMEWORKS_IOSurface /System/Library/Frameworks/IOSurface.framework)
    SET(FRAMEWORKS_CoreVideo /System/Library/Frameworks/CoreVideo.framework)
    SET(FRAMEWORKS_IOKit /System/Library/Frameworks/IOKit.framework)
    SET(FRAMEWORKS_MetalKit /System/Library/Frameworks/MetalKit.framework)
    SET(FRAMEWORKS_Metal /System/Library/Frameworks/Metal.framework)
    SET(FRAMEWORKS_CoreFoundation /System/Library/Frameworks/CoreFoundation.framework)
    SET(FRAMEWORKS_CoreGraphics /System/Library/Frameworks/CoreGraphics.framework)
    SET(FRAMEWORKS_Foundation /System/Library/Frameworks/Foundation.framework)

    link_libraries(${FRAMEWORKS_Cocoa} ${FRAMEWORKS_OpenGL} ${FRAMEWORKS_AppKit} ${FRAMEWORKS_IOSurface} ${FRAMEWORKS_CoreVideo} ${FRAMEWORKS_IOKit} ${FRAMEWORKS_MetalKit} ${FRAMEWORKS_Metal} ${FRAMEWORKS_CoreFoundation} ${FRAMEWORKS_CoreGraphics} ${FRAMEWORKS_Foundation})
endif ()

add_executable(zvkExample main.cpp app/Application.cpp app/Application.h zvk/base/common.h zvk/ZVKManager.cpp zvk/ZVKManager.h zvk/log/log.cpp zvk/base/surfacetype.h zvk/ZVKDestructor.h zvk/ZVKDestructor.cpp zvk/ZVKPlatform.h zvk/ZVKAuxiliary.h zvk/ZVKAuxiliary.cpp zvk/ZVK.h zvk/ZVKContext.h zvk/ZVKInternal.h zvk/ZVKNoncopyable.h zvk/ZVKScopeguard.h zvk/ZVKSingleton.h zvk/platform/platform_osx.mm zvk/platform/platform_osx.h zvk/platform/platform_win32.cpp zvk/platform/platform_win32.h zvk/platform/ext_glfw.cpp zvk/platform/ext_glfw.h zvk/platform/ext_sdl.cpp zvk/platform/ext_sdl.h zvk/base/internal.h zvk/platform/Platform.h zvk/ZVKContext.cpp zvk/ZVKConstructor.cpp zvk/ZVKConstructor.h)
