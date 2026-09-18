cmake_minimum_required(VERSION 4.1)

project(BluetoothSelector
        LANGUAGES
        C
        CXX
        OBJC
        OBJCXX
)

set(CMAKE_C_STANDARD 11)

add_executable(BluetoothSelector MACOSX_BUNDLE
        main.mm
        mainWindow.h
        AppLifecycle/Renderer.mm
        AppLifecycle/Renderer.h)

#======================================================
#Automatically manage memory
#======================================================
target_compile_options(BluetoothSelector PRIVATE
        -fobjc-arc)
#======================================================
#Plist for metadata settings
#======================================================
set_target_properties(BluetoothSelector PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST
        ${CMAKE_CURRENT_SOURCE_DIR}/AppLifecycle/Info.plist)

target_link_libraries(BluetoothSelector
        PRIVATE
        "-framework AppKit"
        "-framework Metal"
        "-framework MetalKit"
        "-framework QuartzCore"
        "-framework AVFoundation"
        "-framework CoreVideo"
        "-framework CoreMedia")

