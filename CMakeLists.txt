cmake_minimum_required(VERSION 3.14)

project(NH3API VERSION 1.2.0)

option(NH3API_CMAKE_TARGET_WINDOWS_XP "Target Windows XP. This flag lowers down the Win32 SDK level." OFF)
option(NH3API_CMAKE_NO_CPP_EXCEPTIONS "Disable C++ exceptions globally." OFF)
option(NH3API_CMAKE_LIB_NO_CPP_EXCEPTIONS "Disable C++ exceptions for NH3API" OFF)
option(NH3API_CMAKE_NOTHROW_NEW "Deprecated. Unused." OFF)
option(NH3API_CMAKE_INLINE_HEADERS "Deprecated. Unused. Inline mode is the only mode available for NH3API since v1.2" ON)
option(NH3API_CMAKE_USE_ERA "Build ERA support module" OFF)

add_library(nh3api INTERFACE)
add_library(nh3api::nh3api ALIAS nh3api)

macro(nh3api_compile_definition definition)
    if(NH3API_CMAKE_INLINE_HEADERS)
        set_property(TARGET nh3api APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS ${definition})
    else()
        add_compile_definitions(nh3api PUBLIC ${definition})
    endif()
endmacro()

macro(nh3api_compile_option arg_option)
    if(NH3API_CMAKE_INLINE_HEADERS)
        set_property(TARGET nh3api APPEND PROPERTY INTERFACE_COMPILE_OPTIONS ${arg_option})
    else()
        add_compile_options(nh3api PUBLIC ${arg_option})
    endif()
endmacro()

if(MSVC)
	add_definitions(-D_USE_STD_VECTOR_ALGORITHMS=0)
    add_definitions(-D_X86_)
else()
    nh3api_compile_option(-m32)
    nh3api_compile_option(-masm=intel)
endif()

if(NH3API_CMAKE_TARGET_WINDOWS_XP)
    nh3api_compile_definition(WINVER=0x501)
    nh3api_compile_definition(_WIN32_WINNT=0x501)
    if(MSVC)
        if(MSVC_TOOLSET_VERSION GREATER_EQUAL 140)
            # Visual Studio 2015 and greater:
            # Remove TLS static variables initialization optimization
            # Which uses Windows Vista syscalls
            # https://learn.microsoft.com/en-us/cpp/build/reference/zc-threadsafeinit-thread-safe-local-static-initialization
            nh3api_compile_option(/Zc:threadSafeInit-)
            nh3api_compile_option(/arch:SSE2)
        endif()
        if(MSVC_TOOLSET_VERSION GREATER 141)
            message(FATAL_ERROR "Visual Studio dropped support of Windows XP since Visual Studio 2019. If you want to target Windows XP, use v141_xp toolset.")
        endif()
    else()
        nh3api_compile_option(-march=pentium4)
    endif()
endif() # Target Windows XP

if(NH3API_CMAKE_LIB_NO_CPP_EXCEPTIONS)
    nh3api_compile_definition(NH3API_FLAG_NO_CPP_EXCEPTIONS)
endif()

if(NH3API_CMAKE_NO_CPP_EXCEPTIONS)
    if(MSVC)
    # MSVC sadly has no -fno-exceptions
        nh3api_compile_definition(_HAS_EXCEPTIONS=0)
    else()
        nh3api_compile_option(-fno-exceptions)
    endif()
endif()


# NH3API requires at least C++17 to work since v1.2, please use v1.1 for C++98 support(unmaintained)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_definitions(nh3api INTERFACE NH3API_FLAG_INLINE_HEADERS)

if(NH3API_CMAKE_USE_ERA)
  find_library(ERA_LIBRARY NAMES era HINTS ${CMAKE_CURRENT_LIST_DIR}/nh3api/era)
  add_library(nh3api-era-module UNKNOWN IMPORTED)
  set_target_properties(nh3api-era-module PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/nh3api")
  set_property(TARGET nh3api-era-module APPEND PROPERTY IMPORTED_LOCATION ${ERA_LIBRARY})
  target_link_libraries(nh3api INTERFACE nh3api-era-module)
endif()

set_target_properties(nh3api PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}")
