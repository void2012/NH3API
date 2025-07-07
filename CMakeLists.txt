cmake_minimum_required(VERSION 3.10)

project(NH3API VERSION 1.0.0)

option(NH3API_CMAKE_TARGET_WINDOWS_XP "Target Windows XP. This flag lowers down the Win32 SDK level." OFF)
option(NH3API_CMAKE_NO_CPP_EXCEPTIONS "Disable C++ exceptions globally." OFF)
option(NH3API_CMAKE_LIB_NO_CPP_EXCEPTIONS "Disable C++ exceptions for NH3API" OFF)
include(CMakeDependentOption)
cmake_dependent_option(NH3API_CMAKE_NOTHROW_NEW "NH3APIs operators new will not throw" ON "NH3API_CMAKE_NO_CPP_EXCEPTIONS" OFF)
option(NH3API_CMAKE_INLINE_HEADERS "Inline mode. See nh3api_std.hpp for details. Requires C++17" OFF)
option(NH3API_CMAKE_USE_ERA "Build ERA support module" OFF)

function(nh3api_add_files)
	set(base_path BASE_PATH OUT)
	set(files FILES)
	cmake_parse_arguments(PARSE_ARGV 0 arg "" "${base_path}" "${files}")

	if(NOT arg_BASE_PATH)
		message(FATAL_ERROR "You must provide a BASE_PATH parameter")
	endif()

	if(NOT arg_OUT)
		message(FATAL_ERROR "You must provide a OUT parameter")
	endif()

	if(NOT arg_FILES)
		message(FATAL_ERROR "You must provide a FILES parameter")
	endif()

	set(local_FILES ${${arg_OUT}})
	foreach(header ${arg_FILES})
		list(APPEND local_FILES "${arg_BASE_PATH}/${header}")
	endforeach()

	set(${arg_OUT} ${local_FILES} PARENT_SCOPE)
endfunction()

function(add_core_headers VAR)

endfunction()

add_library(nh3api STATIC)
add_library(nh3api::nh3api ALIAS nh3api)
add_library(nh3api-header-only INTERFACE)
add_library(nh3api::nh3api-header-only ALIAS nh3api-header-only)

if(MSVC)
	target_compile_definitions(nh3api PUBLIC _X86_)
else()
	add_compile_options(nh3api PUBLIC -m32)
endif()

if(NH3API_CMAKE_TARGET_WINDOWS_XP)
	add_compile_definitions(nh3api PUBLIC WINVER=0x501)
	add_compile_definitions(nh3api PUBLIC _WIN32_WINNT=0x501)
	if(MSVC)
		if(MSVC_TOOLSET_VERSION GREATER_EQUAL 140)
			# Visual Studio 2015 and greater:
			# Remove TLS static variables initialization optimization
			# Which uses Windows Vista syscalls
			# https://learn.microsoft.com/en-us/cpp/build/reference/zc-threadsafeinit-thread-safe-local-static-initialization
			add_compile_options(nh3api PUBLIC /Zc:threadSafeInit-)
			add_compile_options(nh3api PUBLIC /arch:SSE2)
		endif()
		if(MSVC_TOOLSET_VERSION GREATER 141)
			message(FATAL_ERROR "Visual Studio dropped support of Windows XP since Visual Studio 2019. If you want to target Windows XP, use v141_xp toolset.")
		endif()
	else()
		add_compile_options(nh3api PUBLIC -march=pentium4)
		add_compile_options(nh3api PUBLIC -masm=intel)
	endif() 
endif() # Target Windows XP

if(NH3API_CMAKE_LIB_NO_CPP_EXCEPTIONS)
	add_compile_definitions(nh3api PUBLIC NH3API_FLAG_NO_CPP_EXCEPTIONS)
endif()

if(NH3API_CMAKE_NO_CPP_EXCEPTIONS)
	if(MSVC) 
	# MSVC sadly has no -fno-exceptions
		add_compile_definitions(nh3api PUBLIC _HAS_EXCEPTIONS=0)
	else()
		add_compile_options(nh3api PUBLIC -fno-exceptions)
	endif()
endif()

if(NH3API_CMAKE_INLINE_HEADERS)
# Inline mode requires C++17 support 
set(CMAKE_CXX_STANDARD 17)  
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_definitions(nh3api PUBLIC NH3API_FLAG_INLINE_HEADERS)
# Do not add source files in inline mode
else()
nh3api_add_files(
	BASE_PATH
		nh3api/core
	OUT
		NH3API_SOURCES
	FILES
		adventure.cpp
		artifact.cpp
		base_manager.cpp
		combat.cpp
		creatures.cpp
		events.cpp
		global.cpp
		hero.cpp
		input_manager.cpp
		map.cpp
		mouse.cpp
		pathfinding.cpp
		player.cpp
		skills.cpp
		terrain.cpp
		town.cpp
)

nh3api_add_files(
	BASE_PATH
		nh3api/core/interface
	OUT
		NH3API_SOURCES
	FILES
		chat_manager.cpp
		window_manager.cpp
		windows.cpp
)

nh3api_add_files(
	BASE_PATH
		nh3api/core/resources
	OUT
		NH3API_SOURCES
	FILES
		resources.cpp
		sound.cpp
)

nh3api_add_files(
	BASE_PATH
		nh3api
	OUT
		NH3API_SOURCES
	FILES
		turn.cpp
)
endif()

nh3api_add_files(
	BASE_PATH 
		nh3api/core
	OUT 
		NH3API_HEADERS
	FILES
		adventure.hpp
		adventure_AI.hpp
		army.hpp
		artifact.hpp
		base_manager.hpp
		campaign.hpp
		CAutoArray.hpp
		combat.hpp
		creatures.hpp
		events.hpp
		game_setup.hpp
		global.hpp
		hero.hpp
		hero_enums.hpp
		hexcell.hpp
		input_manager.hpp
		lossvictory.hpp
		map.hpp
		map_header.hpp
		mouse.hpp
		multiplayer.hpp
		objects.hpp
		pathfinding.hpp
		player.hpp
		player_enums.hpp
		quests.hpp
		random.hpp
		saved_game.hpp
		skills.hpp
		spells.hpp
		terrain.hpp
		town.hpp
		utils.hpp
		vftables.hpp
)

nh3api_add_files(
	BASE_PATH 
		nh3api
	OUT 
		NH3API_HEADERS
	FILES
		cdplay.hpp
		cheats.hpp
		core.hpp
		hd_mod.hpp
		legacy.hpp
		turn.hpp
)

if(NH3API_CMAKE_USE_ERA)
if(MSVC)
  find_library(ERA_LIBRARY NAMES era HINTS ${CMAKE_CURRENT_LIST_DIR}/nh3api/era)
else()
  find_library(ERA_LIBRARY NAMES libera HINTS ${CMAKE_CURRENT_LIST_DIR}/nh3api/era)
endif()
  add_library(nh3api-era-module UNKNOWN IMPORTED)
  set_target_properties(nh3api-era-module PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/nh3api")
  set_property(TARGET nh3api-era-module APPEND PROPERTY IMPORTED_LOCATION ${ERA_LIBRARY})
  list(APPEND NH3API_SOURCES nh3api/era/era.cpp)
  target_link_libraries(nh3api INTERFACE nh3api-era-module)
endif()

set_target_properties(nh3api PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}")
set_target_properties(nh3api-header-only PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}")
if(NH3API_CMAKE_INLINE_HEADERS)
else()
target_sources(nh3api PRIVATE ${NH3API_SOURCES})
endif()
