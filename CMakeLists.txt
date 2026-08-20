cmake_minimum_required(VERSION 3.20...4.3 FATAL_ERROR)

project(pengupop VERSION 2.2.6 LANGUAGES C
	DESCRIPTION "Bust a Move/Puzzle Bobble clone"
	HOMEPAGE_URL "https://github.com/carstene1ns/pengupop")

set(CMAKE_C_STANDARD 99)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
message(STATUS "Build type is set to ${CMAKE_BUILD_TYPE}.")

# tools
add_executable(bin2c EXCLUDE_FROM_ALL tools/bin2c.c)

# data building
macro(databuilder IN OUT SYM)
add_custom_command(OUTPUT ${OUT}
	COMMAND bin2c ${CMAKE_CURRENT_SOURCE_DIR}/${IN} ${CMAKE_CURRENT_BINARY_DIR}/${OUT} ${SYM}
	DEPENDS bin2c ${IN}
	VERBATIM)
endmacro()

databuilder(data/images.bin images.h images)
databuilder(data/font_16.dat font_16.h font_16)
databuilder(data/font_32.dat font_32.h font_32)
databuilder(snd/destroy_group.wav destroy_group.h destroy_group)
databuilder(snd/launch.wav launch.h launch)
databuilder(snd/rebound.wav rebound.h rebound)
databuilder(snd/stick.wav stick.h stick)

# executable
add_executable(pengupop WIN32
	src/common.h
	src/error.c src/error.h
	src/font.c
	src/gameid.c
	src/image.c
	src/packet.h
	src/singleplayer.c # includes src/singleplayer_levels.c
	src/sound.c src/sound.h
	src/main.c
	${CMAKE_CURRENT_BINARY_DIR}/images.h
	${CMAKE_CURRENT_BINARY_DIR}/font_16.h
	${CMAKE_CURRENT_BINARY_DIR}/font_32.h
	${CMAKE_CURRENT_BINARY_DIR}/destroy_group.h
	${CMAKE_CURRENT_BINARY_DIR}/launch.h
	${CMAKE_CURRENT_BINARY_DIR}/rebound.h
	${CMAKE_CURRENT_BINARY_DIR}/stick.h)
target_compile_definitions(pengupop PRIVATE VERSION="v${PROJECT_VERSION}")
target_compile_options(pengupop PRIVATE -Wall -Wextra)
target_include_directories(pengupop PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# external libs
find_package(SDL REQUIRED)
find_package(ZLIB REQUIRED)
target_link_libraries(pengupop PRIVATE SDL::SDL ZLIB::ZLIB)
if(UNIX)
	target_link_libraries(pengupop PRIVATE m)
endif()

# installation
if(WIN32)
	# put everything in a folder
	install(TARGETS pengupop RUNTIME DESTINATION dist)
	install(CODE [[
		cmake_policy(SET CMP0207 OLD) # slash matching (since 4.3)
		file(GET_RUNTIME_DEPENDENCIES
			EXECUTABLES
				$<TARGET_FILE:pengupop>
			RESOLVED_DEPENDENCIES_VAR _r_deps
			UNRESOLVED_DEPENDENCIES_VAR _u_deps
			DIRECTORIES
				$<TARGET_FILE_DIR:SDL::SDL>
				$<TARGET_FILE_DIR:ZLIB::ZLIB>
		)
		foreach(_file ${_r_deps})
			string(TOLOWER ${_file} _file_lower)
			if(NOT ${_file_lower} MATCHES "c:[\\/]windows[\\/]system32.*")
				file(INSTALL
					DESTINATION dist
					TYPE SHARED_LIBRARY
					FOLLOW_SYMLINK_CHAIN
					FILES "${_file}"
				)
			endif()
		endforeach()
		#message("UNRESOLVED_DEPENDENCIES_VAR: ${_u_deps}")
		]])
elseif(UNIX)
	include(GNUInstallDirs)
	install(TARGETS pengupop RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
	install(FILES res/pengupop.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
	install(FILES res/pengupop.png DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/48x48/apps)
endif()

# source code formatting
if(NOT ASTYLE_FOUND)
	find_program(ASTYLE_EXECUTABLE astyle)
	if(ASTYLE_EXECUTABLE)
		message(STATUS "Found source code formatter: astyle")
		set(ASTYLE_FOUND 1 CACHE INTERNAL "Astyle has been found")
	endif()
endif()
if(ASTYLE_EXECUTABLE)
	separate_arguments(ASTYLE_OPTIONS UNIX_COMMAND
		"--options=none --project=none --suffix=none --style=attach
		--indent=spaces=2 --indent-preproc-block --indent-switches
		--min-conditional-indent=0 --squeeze-lines=1 --max-code-length=100
		--pad-oper --pad-comma --pad-header --unpad-paren --align-pointer=name
		--ignore-exclude-errors --exclude=data.h --exclude=levels.h --exclude=tinfl.c")
	add_custom_target(source-format
		COMMAND ${ASTYLE_EXECUTABLE} ${ASTYLE_OPTIONS}
			"${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
			"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
			"${CMAKE_CURRENT_SOURCE_DIR}/tools/*.c"
		COMMENT "Applying source code format..." VERBATIM)
else()
	add_custom_target(source-format
		COMMAND ${CMAKE_COMMAND} -E false
		COMMENT "No source code formatter enabled!" VERBATIM)
endif()
