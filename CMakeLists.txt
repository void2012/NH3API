cmake_minimum_required(VERSION 3.8)

project(NH3API VERSION 1.0.0)

option(NH3API_TARGET_WINDOWS_XP "Target Windows XP. This flag lowers down the Win32 SDK level." ON)
option(NH3API_NO_CPP_EXCEPTIONS "Disable C++ exceptions globally." OFF)
option(NH3API_LIB_NO_CPP_EXCEPTIONS "Disable C++ exceptions for NH3API" OFF)
option(NH3API_INSTALLTARGET "Generate install target" ON)
include(CMakeDependentOption)
cmake_dependent_option(NH3API_CMAKE_NOTHROW_NEW "NH3APIs operators new will not throw" ON "NH3API_CMAKE_NO_CPP_EXCEPTIONS" OFF)
option(NH3API_CMAKE_INLINE_HEADERS "Inline mode. See nh3api_std.hpp for details. Requires C++17" OFF)

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

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	if(MSVC_TOOLSET_VERSION GREATER 140)
		target_compile_options(nh3api PUBLIC /Zc:threadSafeInit-)
	endif()
	if(MSVC_TOOLSET_VERSION GREATER 141)
		target_compile_options(nh3api PUBLIC /Zc:__cplusplus)
	endif()
	target_compile_definitions(nh3api PUBLIC _X86_)
endif()


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
		events.cpp
		global.cpp
		hero.cpp
		map.cpp
		pathfinding.cpp
		player.cpp
		skills.cpp
		terrain.cpp
		town.cpp
)

nh3api_add_files(
	BASE_PATH
		nh3api
	OUT
		NH3API_SOURCES
	FILES
		turn.cpp
)

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

target_sources(nh3api PRIVATE ${NH3API_SOURCES})