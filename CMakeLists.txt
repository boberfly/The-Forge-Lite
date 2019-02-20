cmake_minimum_required( VERSION 3.0 )

project( The-Forge-Lite )

list(APPEND CMAKE_MODULE_PATH
    "${PROJECT_SOURCE_DIR}/cmake/modules"
)

option( BUILD_ANDROID "Build Android Platform" OFF )
option( BUILD_IOS "Build iOS Platform" OFF )
option( BUILD_LINUX "Build Linux Platform" OFF )
option( BUILD_MACOS "Build macOS Platform" OFF )
option( BUILD_WINDOWS "Build Windows Platform" OFF )

option( BUILD_DIRECT3D11 "Build Direct3D 11" OFF )
option( BUILD_DIRECT3D12 "Build Direct3D 12" OFF )
option( BUILD_METAL "Build Metal" OFF )
option( BUILD_VULKAN "Build Vulkan" OFF )

option( USE_LOGGING "Use Logging" ON )
option( USE_MEMORY_TRACKING "Use Memory Tracking" OFF )

option( BUILD_EXAMPLES "Build Examples" OFF )

# Dependencies
option( BUILD_SPIRV_CROSS "Build SPIRV-Cross, needed for Vulkan reflection" ON )

find_package( Threads REQUIRED )

if( BUILD_LINUX )
    find_package( X11 REQUIRED )
    include_directories( ${X11_INCLUDE_DIR} )
endif()

if( BUILD_DIRECT3D11 )
    find_package( D3D11 REQUIRED )
    include_directories( ${D3D11_INCLUDE_DIRS} )
endif()

if( BUILD_DIRECT3D12 )
    find_package( D3D12 REQUIRED )
    include_directories( ${D3D12_INCLUDE_DIRS} )
endif()

if( BUILD_VULKAN )
    find_package( Vulkan REQUIRED )
    include_directories( ${Vulkan_INCLUDE_DIR} )
endif()

include_directories(
    include
    include/Renderer
    src
    src/Examples
    src/OS
    src/Tools
    third_party
)

# Build Spirv-Cross from CMake
if( BUILD_SPIRV_CROSS )
    option( SPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS "Instead of throwing exceptions assert" ON )
    add_subdirectory( third_party/SPIRV_Cross EXCLUDE_FROM_ALL )
    set( THEFORGE_SPIRV_CROSS_LIBRARY spirv-cross-core )
endif()

# Public headers
set( THEFORGE_PUBLIC_H_FILES
    include/Renderer/GpuProfiler.h
    include/Renderer/IRay.h
    include/Renderer/IRenderer.h
    include/Renderer/IShaderReflection.h
    include/Renderer/ResourceLoader.h
    )

set( THEFORGE_INTERFACES_FILES 
    include/Renderer/Interfaces/IApp.h
    include/Renderer/Interfaces/IFileSystem.h
    include/Renderer/Interfaces/ILogManager.h
    include/Renderer/Interfaces/IMemoryManager.h
    include/Renderer/Interfaces/IMiddleware.h
    include/Renderer/Interfaces/IOperatingSystem.h
    include/Renderer/Interfaces/IPlatformEvents.h
    include/Renderer/Interfaces/IThread.h
    include/Renderer/Interfaces/ITimeManager.h
)

set( THEFORGE_IMAGE_H_FILES
    include/Renderer/Image/Image.h
    include/Renderer/Image/ImageEnums.h
)

# Core
set( THEFORGE_CORE_FILES
    src/OS/Core/Atomics.h
    src/OS/Core/Compiler.h
    src/OS/Core/DLL.h
    src/OS/Core/FileSystem.cpp
    src/OS/Core/GPUConfig.h
    src/OS/Core/PlatformEvents.cpp
    src/OS/Core/RingBuffer.h
    src/OS/Core/ThreadSystem.cpp
    src/OS/Core/Timer.cpp
    )

# Image
set( THEFORGE_IMAGE_FILES
    src/OS/Image/Image.cpp
    include/Renderer/Image/Image.h
    third_party/TinyEXR/tinyexr.cpp
    )

# Logging
if( USE_LOGGING )
    add_definitions( -DUSE_LOGGING )
    set( THEFORGE_LOGGING_FILES
        src/OS/Logging/LogManager.cpp
        src/OS/Logging/LogManager.h
        )
    install( FILES src/OS/Logging/LogManager.h
            DESTINATION include/Renderer/Logging )
endif()

# Math
set( THEFORGE_MATH_FILES
    src/OS/Math/MathTypes.h
    )

# Memory Tracking
set( THEFORGE_MEMORYTRACKING_FILES
    src/OS/MemoryTracking/MemoryTrackingManager.cpp
    )

if( USE_MEMORY_TRACKING )
    add_definitions( -DUSE_MEMORY_TRACKING )
endif()

# Common
set( THEFORGE_COMMON_FILES
    src/Renderer/CommonShaderReflection.cpp
    src/Renderer/GpuProfiler.cpp
    src/Renderer/ResourceLoader.cpp
    )

# Platform interfaces
set( THEFORGE_ANDROID_FILES 
    src/OS/Android/AndroidBase.cpp
    src/OS/Android/AndroidFileSystem.cpp
    src/OS/Android/AndroidLogManager.cpp
    src/OS/Android/AndroidThreadManager.cpp
    )
set( THEFORGE_IOS_FILES 
    src/OS/iOS/AppDelegate.h
    src/OS/iOS/AppDelegate.m
    src/OS/iOS/iOSBase.cpp
    src/OS/iOS/iOSBase.mm
    src/OS/iOS/iOSFileSystem.mm
    src/OS/iOS/iOSLogManager.cpp
    src/OS/iOS/iOSThreadManager.cpp
    )
set( THEFORGE_LINUX_FILES 
    src/OS/Linux/LinuxBase.cpp
    src/OS/Linux/LinuxFileSystem.cpp
    src/OS/Linux/LinuxLogManager.cpp
    src/OS/Linux/LinuxThreadManager.cpp
    )
set( THEFORGE_MACOS_FILES 
    src/OS/macOS/AppDelegate.h
    src/OS/macOS/AppDelegate.m
    src/OS/macOS/macOSBase.cpp
    src/OS/macOS/iOSBase.mm
    src/OS/macOS/macOSFileSystem.mm
    src/OS/macOS/macOSLogManager.cpp
    src/OS/macOS/macOSThreadManager.cpp
    )
set( THEFORGE_WINDOWS_FILES 
    src/OS/Windows/WindowsBase.cpp
    src/OS/Windows/WindowsFileSystem.cpp
    src/OS/Windows/WindowsLogManager.cpp
    src/OS/Windows/WindowsThreadManager.cpp
    )

# Direct3D 11
set( THEFORGE_DIRECT3D11_FILES
    src/Renderer/Direct3D11/Direct3D11.cpp
    src/Renderer/Direct3D11/Direct3D11Commands.h
    src/Renderer/Direct3D11/Direct3D11Raytracing.cpp
    src/Renderer/Direct3D11/Direct3D11ShaderReflection.cpp
    )

# Direct3D 12
set( THEFORGE_DIRECT3D12_FILES
    src/Renderer/Direct3D12/Direct3D12.cpp
    src/Renderer/Direct3D12/Direct3D12Hooks.h
    src/Renderer/Direct3D12/Direct3D12MemoryAllocator.cpp
    src/Renderer/Direct3D12/Direct3D12MemoryAllocator.h
    src/Renderer/Direct3D12/Direct3D12Raytracing.cpp
    src/Renderer/Direct3D12/Direct3D12ShaderReflection.cpp
    )

# Metal
set( THEFORGE_METAL_FILES
    src/Renderer/Metal/MetalMemoryAllocator.h
    src/Renderer/Metal/MetalRaytracing.mm
    src/Renderer/Metal/MetalRenderer.mm
    src/Renderer/Metal/MetalShaderReflection.mm
    )

# Vulkan
set( THEFORGE_VULKAN_FILES
    src/Renderer/Vulkan/Vulkan.cpp
    src/Renderer/Vulkan/VulkanRaytracing.cpp
    src/Renderer/Vulkan/VulkanShaderReflection.cpp
    )
set( THEFORGE_VULKAN_DEP_FILES
    src/Tools/SpirvTools/SpirvTools.cpp
    src/Tools/SpirvTools/SpirvTools.h
    third_party/volk/volk.c
    third_party/volk/volk.h
    )

# Image library
add_library( TFImage STATIC 
    ${THEFORGE_IMAGE_FILES}
    )
install( TARGETS TFImage DESTINATION lib )

# Android library
if( BUILD_ANDROID )
    add_library( TFAndroid STATIC
        ${THEFORGE_ANDROID_FILES}
        ${THEFORGE_MATH_FILES}
        ${THEFORGE_MEMORYTRACKING_FILES}
    )
    target_compile_definitions( TFAndroid PRIVATE NO_GAINPUT )
    install( TARGETS TFAndroid DESTINATION lib )
endif()

# iOS library
if( BUILD_IOS )
    add_library( TFiOS STATIC
        ${THEFORGE_IOS_FILES}
        ${THEFORGE_MATH_FILES}
        ${THEFORGE_MEMORYTRACKING_FILES}
    )
    target_compile_definitions( TFiOS PRIVATE NO_GAINPUT )
    target_link_libraries( TFiOS "-framework AppKit" )
    install( TARGETS TFiOS DESTINATION lib )
endif()

# Linux library
if( BUILD_LINUX )
    add_library( TFLinux STATIC
        ${THEFORGE_LINUX_FILES}
        ${THEFORGE_MATH_FILES}
        ${THEFORGE_MEMORYTRACKING_FILES}
        ${X11_LIBRARIES}
    )
    target_compile_definitions( TFLinux PRIVATE NO_GAINPUT )
    target_link_libraries( TFLinux ${CMAKE_THREAD_LIBS_INIT} )
    install( TARGETS TFLinux DESTINATION lib )
endif()

# macOS library
if( BUILD_MACOS )
    add_library( TFmacOS STATIC
        ${THEFORGE_MACOS_FILES}
        ${THEFORGE_MATH_FILES}
        ${THEFORGE_MEMORYTRACKING_FILES}
    )
    target_compile_definitions( TFmacOS PRIVATE NO_GAINPUT )
    target_link_libraries( TFmacOS "-framework AppKit" )
    install( TARGETS TFmacOS DESTINATION lib )
endif()

# Windows library
if( BUILD_WINDOWS )
    add_library( TFWindows STATIC
        ${THEFORGE_WINDOWS_FILES}
        ${THEFORGE_MATH_FILES}
        ${THEFORGE_MEMORYTRACKING_FILES}
    )
    target_compile_definitions( TFWindows PRIVATE NO_GAINPUT )
    install( TARGETS TFWindows DESTINATION lib )
endif()

# Direct3D11 library
if( BUILD_DIRECT3D11 )
    add_library( TFDirect3D11 STATIC
        ${THEFORGE_PUBLIC_H_FILES}
        ${THEFORGE_CORE_FILES}
        ${THEFORGE_COMMON_FILES}
        ${THEFORGE_DIRECT3D11_FILES}
        ${THEFORGE_DIRECT3D11_DEP_FILES}
        ${THEFORGE_LOGGING_FILES}
        ${THEFORGE_MEMORYTRACKING_FILES}
        )
    target_compile_definitions( TFDirect3D11 PRIVATE DIRECT3D11 )
    target_link_libraries( TFDirect3D12 ${D3D11_LIBRARIES} )
    install( TARGETS TFDirect3D11 DESTINATION lib )
endif()

# Direct3D12 library
if( BUILD_DIRECT3D12 )
    add_library( TFDirect3D12 STATIC
        ${THEFORGE_PUBLIC_H_FILES}
        ${THEFORGE_CORE_FILES}
        ${THEFORGE_COMMON_FILES}
        ${THEFORGE_DIRECT3D12_FILES}
        ${THEFORGE_DIRECT3D12_DEP_FILES}
        ${THEFORGE_LOGGING_FILES}
        ${THEFORGE_MEMORYTRACKING_FILES}
        )
    target_compile_definitions( TFDirect3D12 PRIVATE DIRECT3D12 )
    target_link_libraries( TFDirect3D12 ${D3D12_LIBRARIES} )
    install( TARGETS TFDirect3D12 DESTINATION lib )
endif()

# Metal library
if( BUILD_METAL )
    add_library( TFMetal STATIC
        ${THEFORGE_PUBLIC_H_FILES}
        ${THEFORGE_CORE_FILES}
        ${THEFORGE_COMMON_FILES}
        ${THEFORGE_METAL_FILES}
        ${THEFORGE_METAL_DEP_FILES}
        ${THEFORGE_LOGGING_FILES}
        ${THEFORGE_MEMORYTRACKING_FILES}
        )
    target_compile_definitions( TFMetal PRIVATE METAL )
    target_link_libraries( TFMetal "-framework Metal" )
    install( TARGETS TFMetal DESTINATION lib )
endif()

# Vulkan library
if( BUILD_VULKAN )
    add_library( TFVulkan STATIC
        ${THEFORGE_PUBLIC_H_FILES}
        ${THEFORGE_CORE_FILES}
        ${THEFORGE_COMMON_FILES}
        ${THEFORGE_VULKAN_FILES}
        ${THEFORGE_VULKAN_DEP_FILES}
        ${THEFORGE_LOGGING_FILES}
        ${THEFORGE_MEMORYTRACKING_FILES}
        )
    target_compile_definitions( TFVulkan PRIVATE VULKAN )
    target_link_libraries( TFVulkan ${Vulkan_LIBRARIES} ${THEFORGE_SPIRV_CROSS_LIBRARY} )
    if( BUILD_SPIRV_CROSS )
        add_dependencies( TFVulkan ${THEFORGE_SPIRV_CROSS_LIBRARY} )
    endif()
    install( TARGETS TFVulkan DESTINATION lib )
endif()

# Examples
macro( add_theforge_example THEFORGE_EXAMPLES_NAME THEFORGE_TARGET THEFORGE_BACKEND )
    add_executable( ${THEFORGE_EXAMPLES_NAME}${THEFORGE_TARGET}${THEFORGE_BACKEND} src/Examples/${THEFORGE_EXAMPLES_NAME}/${THEFORGE_EXAMPLES_NAME}.cpp )
    target_compile_definitions( ${THEFORGE_EXAMPLES_NAME}${THEFORGE_TARGET}${THEFORGE_BACKEND} PRIVATE ${THEFORGE_EXAMPLES_DEFINES} )
    target_link_libraries( ${THEFORGE_EXAMPLES_NAME}${THEFORGE_TARGET}${THEFORGE_BACKEND} ${THEFORGE_EXAMPLES_DEPS} )
    set_target_properties( ${THEFORGE_EXAMPLES_NAME}${THEFORGE_TARGET}${THEFORGE_BACKEND} PROPERTIES OUTPUT_NAME ${THEFORGE_EXAMPLES_NAME} )
    add_dependencies( TF${THEFORGE_TARGET} TF${THEFORGE_BACKEND} TFCamera TFInput )
    install( TARGETS ${THEFORGE_EXAMPLES_NAME}${THEFORGE_TARGET}${THEFORGE_BACKEND} DESTINATION bin/Examples/${THEFORGE_EXAMPLES_NAME} )
    install( DIRECTORY src/Examples/${THEFORGE_EXAMPLES_NAME}/Shaders/${THEFORGE_BACKEND}
            DESTINATION bin/Examples/${THEFORGE_EXAMPLES_NAME}/Shaders )
endmacro()

if( BUILD_EXAMPLES )
    set( THEFORGE_EXAMPLES_NAMES 01_Transformations )
    install( DIRECTORY src/Examples/UnitTestResources
            DESTINATION bin/Examples )
    set( THEFORGE_CAMERA_FILES
        src/Examples/Camera/FpsCameraController.cpp
        src/Examples/Camera/GuiCameraController.cpp
        src/Examples/Camera/Interfaces/ICameraController.h
    )
    add_library( TFCamera STATIC ${THEFORGE_CAMERA_FILES} )

    option( GAINPUT_BUILD_SHARED "BUILD_SHARED" OFF )
    add_subdirectory( third_party/gainput EXCLUDE_FROM_ALL )
    set( THEFORGE_INPUT_FILES
        src/Examples/Input/InputSystem.cpp
        src/Examples/Input/InputSystem.h
        src/Examples/Input/InputMappings.h
    )
    add_library( TFInput STATIC ${THEFORGE_INPUT_FILES} )
    target_link_libraries( TFInput gainputstatic )
    add_dependencies( TFInput gainputstatic )

    foreach( THEFORGE_EXAMPLES_NAME ${THEFORGE_EXAMPLES_NAMES} )
        install( DIRECTORY src/Examples/${THEFORGE_EXAMPLES_NAME}/GPUCfg
                DESTINATION bin/Examples/${THEFORGE_EXAMPLES_NAME}/ )

        if( BUILD_ANDROID AND BUILD_VULKAN )
            set( THEFORGE_EXAMPLES_DEPS
                TFVulkan
                TFImage
                TFAndroid
                TFCamera
                TFInput
                ${Vulkan_LIBRARIES}
                ${THEFORGE_SPIRV_CROSS_LIBRARY}
            )
            set( THEFORGE_EXAMPLES_DEFINES
                VULKAN
            )
            add_theforge_example( ${THEFORGE_EXAMPLES_NAME} Android Vulkan )
        endif()
        if( BUILD_IOS AND BUILD_METAL )
            set( THEFORGE_EXAMPLES_DEPS
                TFMetal
                TFiOS
                TFCamera
                TFInput
                "-framework Metal"
            )
            set( THEFORGE_EXAMPLES_DEFINES
                METAL
            )
            add_theforge_example( ${THEFORGE_EXAMPLES_NAME} iOS Metal )
        endif()
        if( BUILD_LINUX AND BUILD_VULKAN )
            set( THEFORGE_EXAMPLES_DEPS
                TFVulkan
                TFImage
                TFLinux
                TFCamera
                TFInput
                ${Vulkan_LIBRARIES}
                ${THEFORGE_SPIRV_CROSS_LIBRARY}
                ${X11_LIBRARIES}
                ${CMAKE_DL_LIBS}
            )
            set( THEFORGE_EXAMPLES_DEFINES
                VULKAN
            )
            add_theforge_example( ${THEFORGE_EXAMPLES_NAME} Linux Vulkan )
        endif()
        if( BUILD_MACOS AND BUILD_METAL )
            set( THEFORGE_EXAMPLES_DEPS
                TFMetal
                TFmacOS 
                TFCamera
                TFInput
                "-framework Metal"
            )
            set( THEFORGE_EXAMPLES_DEFINES
                METAL
            )
            add_theforge_example( ${THEFORGE_EXAMPLES_NAME} macOS Metal )
        endif()
        if( BUILD_WINDOWS AND BUILD_DIRECT3D11 )
            set( THEFORGE_EXAMPLES_DEPS
                TFDirect3D11
                TFWindows 
                TFCamera
                TFInput
                ${D3D11_LIBRARIES}
            )
            set( THEFORGE_EXAMPLES_DEFINES
                DIRECT3D11
            )
            add_theforge_example( ${THEFORGE_EXAMPLES_NAME} Windows Direct3D11 )
        endif()
        if( BUILD_WINDOWS AND BUILD_DIRECT3D12 )
            set( THEFORGE_EXAMPLES_DEPS
                TFDirect3D12
                TFWindows
                TFCamera
                TFInput
                ${D3D12_LIBRARIES}
            )
            set( THEFORGE_EXAMPLES_DEFINES
                DIRECT3D12
            )
            add_theforge_example( ${THEFORGE_EXAMPLES_NAME} Windows Direct3D12 )
        endif()
        if( BUILD_WINDOWS AND BUILD_VULKAN )
            set( THEFORGE_EXAMPLES_DEPS
                TFVulkan
                TFWindows
                TFCamera
                TFInput
                ${Vulkan_LIBRARIES} 
                ${THEFORGE_SPIRV_CROSS_LIBRARY}
            )
            set( THEFORGE_EXAMPLES_DEFINES
                VULKAN
            )
            add_theforge_example( ${THEFORGE_EXAMPLES_NAME} Windows Vulkan )
        endif()
    endforeach()
endif()

install( FILES ${THEFORGE_PUBLIC_H_FILES}
        DESTINATION include/Renderer )

install( FILES ${THEFORGE_INTERFACES_FILES}
        DESTINATION include/Renderer/Interfaces )

install( FILES ${THEFORGE_IMAGE_H_FILES}
        DESTINATION include/Renderer/Image )
