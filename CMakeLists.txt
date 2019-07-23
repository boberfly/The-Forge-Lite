cmake_minimum_required( VERSION 3.0 )

project( The-Forge-Lite )

set(CMAKE_CXX_STANDARD 14)

function(set_prefix var prefix)
  string(REGEX REPLACE "(^|;)([^;]+)" "\\1${prefix}\\2" tmp "${ARGN}")
  set(${var} "${tmp}" PARENT_SCOPE)
endfunction()

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

option( USE_MEMORY_TRACKING "Use Memory Tracking" OFF )
option( USE_PROFILER "Use Profiler" OFF )

option( BUILD_MIDDLEWARE_UI "Build Middleware UI" OFF )
option( BUILD_MIDDLEWARE_TEXT "Build Middleware Text" OFF )

option( BUILD_EXAMPLES "Build Examples" OFF )

option( INSTALL_EASTL "Install EASTL" ON )

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

# Spirv-Cross
set_prefix( THEFORGE_SPIRV_CROSS_FILES third_party/SPIRV_Cross/
    GLSL.std.450.h
    spirv_cfg.hpp
    spirv_common.hpp
    spirv_cpp.hpp
    spirv_cross_containers.hpp
    spirv_cross_error_handling.hpp
    spirv_cross_parsed_ir.hpp
    spirv_cross_util.hpp
    spirv_cross.hpp
    spirv_glsl.hpp
    spirv_hlsl.hpp
    spirv_msl.hpp
    spirv_parser.hpp
    spirv_reflect.hpp
    spirv.h
    spirv.hpp
    spirv_cfg.cpp
    spirv_cpp.cpp
    spirv_cross_parsed_ir.cpp
    spirv_cross_util.cpp
    spirv_cross.cpp
    spirv_glsl.cpp
    spirv_hlsl.cpp
    spirv_msl.cpp
    spirv_parser.cpp
    spirv_reflect.cpp
)

# Public headers
set_prefix( THEFORGE_PUBLIC_H_FILES include/Renderer/
    GpuProfiler.h
    IRay.h
    IRenderer.h
    IShaderReflection.h
    ResourceLoader.h
    )

set_prefix( THEFORGE_INTERFACES_FILES include/Renderer/Interfaces/
    IApp.h
    IFileSystem.h
    ILog.h
    IMemory.h
    IMiddleware.h
    IOperatingSystem.h
    IPlatformEvents.h
    IProfiler.h
    IThread.h
    ITime.h
)

set_prefix( THEFORGE_IMAGE_H_FILES include/Renderer/Image/
    Image.h
    ImageEnums.h
)

# EASTL
set_prefix( THEFORGE_EASTL_FILES third_party/EASTL/
    EAStdC/EASprintf.cpp
    EAStdC/EAMemory.cpp
    thread_support.cpp
    string.cpp
    red_black_tree.cpp
    numeric_limits.cpp
    intrusive_list.cpp
    hashtable.cpp
    fixed_pool.cpp
    assert.cpp
    allocator_forge.cpp
    allocator_eastl.cpp
    )
add_library( EASTL STATIC ${THEFORGE_EASTL_FILES} )

# Core
set_prefix( THEFORGE_CORE_FILES src/OS/Core/
    Atomics.h
    Compiler.h
    DLL.h
    ThreadSystem.h
    FileSystem.cpp
    GPUConfig.h
    PlatformEvents.cpp
    RingBuffer.h
    ThreadSystem.cpp
    Timer.cpp
    )

# Image
set( THEFORGE_IMAGE_FILES
    src/OS/Image/Image.cpp
    include/Renderer/Image/Image.h
    third_party/TinyEXR/tinyexr.cpp
    )

# Logging
set_prefix( THEFORGE_LOGGING_FILES src/OS/Logging/
    Log.cpp
    Log.h
    )
install( FILES src/OS/Logging/Log.h
        DESTINATION include/Renderer/Logging )

# Math
set( THEFORGE_MATH_FILES
    src/OS/Math/MathTypes.h
    )

# Memory Tracking
set_prefix( THEFORGE_MEMORYTRACKING_FILES src/OS/MemoryTracking/
    NoMemoryDefines.h
    MemoryTracking.cpp
    )

if( USE_MEMORY_TRACKING )
    add_definitions( -DUSE_MEMORY_TRACKING )
endif()

# Common
set_prefix( THEFORGE_COMMON_FILES src/Renderer/
    CommonShaderReflection.cpp
    GpuProfiler.cpp
    ResourceLoader.cpp
    )

# Platform interfaces
set_prefix( THEFORGE_ANDROID_FILES src/OS/Android/
    AndroidBase.cpp
    AndroidFileSystem.cpp
    AndroidLog.cpp
    AndroidThread.cpp
    AndroidTime.cpp
    )
set_prefix( THEFORGE_IOS_FILES src/OS/iOS/
    AppDelegate.h
    AppDelegate.m
    iOSBase.cpp
    iOSBase.mm
    iOSFileSystem.mm
    iOSLog.cpp
    iOSThread.cpp
    )
set_prefix( THEFORGE_LINUX_FILES src/OS/Linux/
    LinuxBase.cpp
    LinuxFileSystem.cpp
    LinuxLog.cpp
    LinuxThread.cpp
    LinuxTime.cpp
    )
set_prefix( THEFORGE_MACOS_FILES src/OS/macOS/
    AppDelegate.h
    AppDelegate.m
    macOSBase.mm
    macOSFileSystem.mm
    macOSLog.cpp
    macOSThread.cpp
    )
set_prefix( THEFORGE_WINDOWS_FILES src/OS/Windows/
    WindowsBase.cpp
    WindowsFileSystem.cpp
    WindowsLog.cpp
    WindowsThread.cpp
    WindowsTime.cpp
    )

# Direct3D 11
set_prefix( THEFORGE_DIRECT3D11_FILES src/Renderer/Direct3D11/
    Direct3D11.cpp
    Direct3D11Commands.h
    Direct3D11Raytracing.cpp
    Direct3D11ShaderReflection.cpp
    )

# Direct3D 12
set_prefix( THEFORGE_DIRECT3D12_FILES src/Renderer/Direct3D12/
    Direct3D12.cpp
    Direct3D12Hooks.h
    Direct3D12MemoryAllocator.cpp
    Direct3D12MemoryAllocator.h
    Direct3D12Raytracing.cpp
    Direct3D12ShaderReflection.cpp
    )

# Metal
set_prefix( THEFORGE_METAL_FILES src/Renderer/Metal/
    MetalMemoryAllocator.h
    MetalRaytracing.mm
    MetalRenderer.mm
    MetalShaderReflection.mm
    )

# Vulkan
set_prefix( THEFORGE_VULKAN_FILES src/Renderer/Vulkan/
    Vulkan.cpp
    VulkanRaytracing.cpp
    VulkanShaderReflection.cpp
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

if( BUILD_EXAMPLES )
    set( BUILD_MIDDLEWARE_UI ON )
endif()

# Profiler
if( USE_PROFILER )
    add_definitions( -DPROFILE_ENABLED=1 )
    set( BUILD_MIDDLEWARE_UI ON )
else()
    add_definitions( -DPROFILE_ENABLED=0 )
endif()

if( BUILD_MIDDLEWARE_UI )
    set( BUILD_MIDDLEWARE_TEXT ON )
endif()

# Android library
if( BUILD_ANDROID )
    add_library( TFAndroid STATIC
        ${THEFORGE_ANDROID_FILES}
        ${THEFORGE_MATH_FILES}
        ${THEFORGE_MEMORYTRACKING_FILES}
    )
    #target_compile_definitions( TFAndroid PRIVATE NO_GAINPUT )
    target_link_libraries( TFAndroid EASTL )
    install( TARGETS TFAndroid DESTINATION lib )
endif()

# iOS library
if( BUILD_IOS )
    add_library( TFiOS STATIC
        ${THEFORGE_IOS_FILES}
        ${THEFORGE_MATH_FILES}
        ${THEFORGE_MEMORYTRACKING_FILES}
    )
    #target_compile_definitions( TFiOS PRIVATE NO_GAINPUT )
    target_link_libraries( TFiOS "-framework AppKit" EASTL )
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
    #target_compile_definitions( TFLinux PRIVATE NO_GAINPUT )
    target_link_libraries( TFLinux ${CMAKE_THREAD_LIBS_INIT} EASTL )
    install( TARGETS TFLinux DESTINATION lib )
endif()

# macOS library
if( BUILD_MACOS )
    add_library( TFmacOS STATIC
        ${THEFORGE_MACOS_FILES}
        ${THEFORGE_MATH_FILES}
        ${THEFORGE_MEMORYTRACKING_FILES}
    )
    #target_compile_definitions( TFmacOS PRIVATE NO_GAINPUT )
    target_link_libraries( TFmacOS "-framework AppKit" EASTL )
    install( TARGETS TFmacOS DESTINATION lib )
endif()

# Windows library
if( BUILD_WINDOWS )
    add_library( TFWindows STATIC
        ${THEFORGE_WINDOWS_FILES}
        ${THEFORGE_MATH_FILES}
        ${THEFORGE_MEMORYTRACKING_FILES}
    )
    #target_compile_definitions( TFWindows PRIVATE NO_GAINPUT )
    target_link_libraries( TFWindows EASTL )
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
        ${THEFORGE_SPIRV_CROSS_FILES}
        )
    target_compile_definitions( TFVulkan PRIVATE VULKAN )
    target_link_libraries( TFVulkan ${Vulkan_LIBRARIES} )
    install( TARGETS TFVulkan DESTINATION lib )
endif()

# Middleware
set( THEFORGE_MIDDLEWARE_Text_FILES
    src/Middleware/Text/Fontstash.h
    src/Middleware/Text/Fontstash.cpp
    )

set( THEFORGE_MIDDLEWARE_UI_FILES
    include/Renderer/Interfaces/IProfiler.h
    src/Middleware/UI/AppUI.h
    src/Middleware/UI/imgui_user.h
    src/Middleware/UI/AppUI.cpp
    src/Middleware/UI/imgui_user.cpp
    src/Middleware/UI/ImguiGUIDriver.cpp
    third_party/imgui/imconfig.h
    third_party/imgui/imgui_internal.h
    third_party/imgui/imgui.h
    third_party/imgui/imgui_demo.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/imgui.cpp
    )

macro( add_theforge_middleware THEFORGE_MIDDLEWARE_NAME THEFORGE_BACKEND )
    string(TOUPPER ${THEFORGE_BACKEND} THEFORGE_BACKEND_DEFINE)
    add_library( TF${THEFORGE_MIDDLEWARE_NAME}${THEFORGE_BACKEND} STATIC
        ${THEFORGE_MIDDLEWARE_${THEFORGE_MIDDLEWARE_NAME}_FILES}
        )
    target_compile_definitions( TF${THEFORGE_MIDDLEWARE_NAME}${THEFORGE_BACKEND} PRIVATE ${THEFORGE_BACKEND_DEFINE} )
    target_link_libraries( TF${THEFORGE_MIDDLEWARE_NAME}${THEFORGE_BACKEND} TF${THEFORGE_BACKEND} )
    set( THEFORGE_MIDDLEWARE_${THEFORGE_BACKEND}_LIBRARIES
        ${THEFORGE_MIDDLEWARE_${THEFORGE_BACKEND}_LIBRARIES}
        TF${THEFORGE_MIDDLEWARE_NAME}${THEFORGE_BACKEND}
    )
endmacro()

if( BUILD_DIRECT3D11 )
    if( BUILD_MIDDLEWARE_TEXT )
        add_theforge_middleware( Text Direct3D11 )
    endif()
    if( BUILD_MIDDLEWARE_UI )
        add_theforge_middleware( UI Direct3D11 DIRECT3D11 )
        target_link_libraries( TFUIDirect3D11 TFTextDirect3D11 )
    endif()
endif()
if( BUILD_DIRECT3D12 )
    if( BUILD_MIDDLEWARE_TEXT )
        add_theforge_middleware( Text Direct3D12 )
    endif()
    if( BUILD_MIDDLEWARE_UI )
        add_theforge_middleware( UI Direct3D12 )
        target_link_libraries( TFUIDirect3D12 TFTextDirect3D12 )
    endif()
endif()
if( BUILD_METAL )
    if( BUILD_MIDDLEWARE_TEXT )
        add_theforge_middleware( Text Metal )
    endif()
    if( BUILD_MIDDLEWARE_UI )
        add_theforge_middleware( UI Metal )
        target_link_libraries( TFUIMetal TFTextMetal )
    endif()
endif()
if( BUILD_VULKAN )
    if( BUILD_MIDDLEWARE_TEXT )
        add_theforge_middleware( Text Vulkan )
    endif()
    if( BUILD_MIDDLEWARE_UI )
        add_theforge_middleware( UI Vulkan )
        target_link_libraries( TFUIVulkan TFTextVulkan )
    endif()
endif()

# Profiler
set_prefix( THEFORGE_MICROPROFILE_FILES third_party/MicroProfile/
    ProfilerBase.h
    ProfilerHTML.h
    ProfilerUI.h
    Profiler.cpp
    ProfilerBase.cpp
    ProfilerDraw.cpp
    ProfilerInput.cpp
    ProfilerUI.cpp
    )

if( BUILD_DIRECT3D11 )
    add_library( TFProfilerDirect3D11 STATIC
        ${THEFORGE_MICROPROFILE_FILES}
        )
    target_compile_definitions( TFProfilerDirect3D11 PRIVATE DIRECT3D11 )
    target_link_libraries( TFProfilerDirect3D11 TFDirect3D11 TFTextDirect3D11 TFUIDirect3D11 )
endif()
if( BUILD_DIRECT3D12 )
    add_library( TFProfilerDirect3D12 STATIC
        ${THEFORGE_MICROPROFILE_FILES}
        )
    target_compile_definitions( TFProfilerDirect3D12 PRIVATE DIRECT3D12 )
    target_link_libraries( TFProfilerDirect3D12 TFDirect3D12 TFTextDirect3D12 TFUIDirect3D12 )
endif()
if( BUILD_METAL )
    add_library( TFProfilerMetal STATIC
        ${THEFORGE_MICROPROFILE_FILES}
        )
    target_compile_definitions( TFProfilerMetal PRIVATE METAL )
    target_link_libraries( TFProfilerMetal TFMetal TFTextMetal TFUIMetal )
endif()
if( BUILD_VULKAN )
    add_library( TFProfilerVulkan STATIC
        ${THEFORGE_MICROPROFILE_FILES}
        )
    target_compile_definitions( TFProfilerVulkan PRIVATE VULKAN )
    target_link_libraries( TFProfilerVulkan TFVulkan TFTextVulkan TFUIVulkan )
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
    install( DIRECTORY src/Middleware/Text/Shaders
                DESTINATION bin/Examples/Middleware/Text )
    install( DIRECTORY src/Middleware/UI/Shaders
                DESTINATION bin/Examples/Middleware/UI )
    set_prefix( THEFORGE_CAMERA_FILES src/Examples/Camera/
        FpsCameraController.cpp
        GuiCameraController.cpp
        Interfaces/ICameraController.h
    )
    add_library( TFCamera STATIC ${THEFORGE_CAMERA_FILES} )

    option( GAINPUT_BUILD_SHARED "BUILD_SHARED" OFF )
    add_subdirectory( third_party/gainput EXCLUDE_FROM_ALL )
    set_prefix( THEFORGE_INPUT_FILES src/Examples/Input/
        InputSystem.cpp
        InputSystem.h
        InputMappings.h
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
                TFProfilerVulkan
                ${THEFORGE_MIDDLEWARE_VULKAN_LIBRARIES}
                ${Vulkan_LIBRARIES}
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
                TFProfilerMetal
                ${THEFORGE_MIDDLEWARE_METAL_LIBRARIES}
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
                TFProfilerVulkan
                ${THEFORGE_MIDDLEWARE_VULKAN_LIBRARIES}
                ${Vulkan_LIBRARIES}
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
                TFProfilerMetal
                ${THEFORGE_MIDDLEWARE_METAL_LIBRARIES}
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
                TFProfilerDirect3D11
                ${THEFORGE_MIDDLEWARE_DIRECT3D11_LIBRARIES}
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
                TFProfilerDirect3D12
                ${THEFORGE_MIDDLEWARE_DIRECT3D12_LIBRARIES}
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
                TFProfilerVulkan
                ${THEFORGE_MIDDLEWARE_VULKAN_LIBRARIES}
                ${Vulkan_LIBRARIES}
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

# Thirdparty
if( BUILD_VULKAN )
    install( FILES third_party/volk/volk.h
             DESTINATION include/volk )
endif()

if( INSTALL_EASTL )
    install( DIRECTORY third_party/EASTL
             DESTINATION include )
endif()
