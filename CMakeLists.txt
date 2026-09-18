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

#=========================================================
#Metal
#=========================================================
set(METAL_AIR
        ${CMAKE_CURRENT_BINARY_DIR}/shaders.air
)

set(METALLIB
        ${CMAKE_CURRENT_BINARY_DIR}/shaders.metallib
)

add_custom_command(
        OUTPUT ${METALLIB}

        COMMAND xcrun -sdk macosx metal
        -c
        ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/shaders.metal
        -o ${METAL_AIR}

        COMMAND xcrun -sdk macosx metallib
        ${METAL_AIR}
        -o ${METALLIB}

        DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/shaders.metal

        COMMENT "Compiling Metal shaders"
)

target_sources(BluetoothSelector PRIVATE
        ${METALLIB}
)

set_source_files_properties(
        ${METALLIB}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
        GENERATED TRUE
)

# We compile Metal ourselves above, but still want the .metal
# file visible/editable inside Xcode.
set_source_files_properties(
        Shaders/shaders.metal
        PROPERTIES
        HEADER_FILE_ONLY TRUE
)

