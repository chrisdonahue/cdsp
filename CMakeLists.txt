cmake_minimum_required(VERSION 2.8)

project(cdsp)

set(PACKAGE "cdsp")
set(CPACK_PACKAGE_NAME "${PACKAGE}")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
set(CPACK_PACKAGE_VENDOR "christopherdonahue@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PACKAGE} ${PACKAGE_VERSION}")
set(SOVERSION "5")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(VERSION "${CPACK_PACKAGE_VERSION}")

set(CDSP_LIBRARY_VERSION ${CPACK_PACKAGE_VERSION})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

message(STATUS "CMAKE_TOOLCHAIN_FILE='${CMAKE_TOOLCHAIN_FILE}'")

# Try to find the current Git hash.
find_package(Git)
if(GIT_EXECUTABLE)
	execute_process(
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMAND "${GIT_EXECUTABLE}" log -n 1 --pretty=%h
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(CDSP_BUILD_HASH ${GIT_HASH})
    message("Git commit hash: ${CDSP_BUILD_HASH}")
endif()

# Options
option(CDSP_BUILD_SHARED "Build shared library" ON)
option(CDSP_DEBUG_INTERNAL "Don't build the cdsp test apps" ON)
option(CDSP_BUILD_TESTS "Don't build the cdsp test apps" ON)

# Include directories
include_directories("${PROJECT_BINARY_DIR}")

# Fix inline
include(CheckCSourceCompiles)
foreach(KEYWORD "inline" "__inline__" "__inline")
	set(CMAKE_REQUIRED_DEFINITIONS "-DKEYWORD=${KEYWORD}")
	CHECK_C_SOURCE_COMPILES(
		"
		#include <stdio.h>
		KEYWORD void a() {}
		int main(int argc, char **argv) { a(); return 0; }
		" HAVE_${KEYWORD})
endforeach()

if (NOT HAVE_inline)
	if (HAVE___inline__)
		set(inline __inline__)
	elseif(HAVE___inline)
		set(inline __inline)
	endif()
endif()

# So we can include the CMake generated config file only when
# building with CMAKE.
add_definitions(-DCMAKE_BUILD)

# Check system compatibilities
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckTypeSize)

CHECK_FUNCTION_EXISTS(malloc HAVE_MALLOC)
CHECK_FUNCTION_EXISTS(memset HAVE_MEMSET)
CHECK_FUNCTION_EXISTS(realloc HAVE_REALLOC)

CHECK_INCLUDE_FILE(stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILE(string.h HAVE_STRING_H)

CHECK_INCLUDE_FILES("stdlib.h;stdarg.h;string.h;float.h" STDC_HEADERS)

CHECK_TYPE_SIZE(size_t SIZE_T_SIZE)

if (NOT SIZE_T_SIZE)
	set(size_t "unsigned int")
endif()

if (NOT HAVE_MALLOC)
	set(malloc rpl_malloc)
endif()

if (NOT HAVE_REALLOC)
	set(realloc rpl_realloc)
endif()

# Generate the config.h that includes all the compilation settings.
configure_file(
       "${PROJECT_SOURCE_DIR}/config.h.cmake"
       "${PROJECT_BINARY_DIR}/cdsp_config.h")

if (MSVC)
	# Turn off stupid microsoft security warnings.
	#add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
endif(MSVC)

include_directories("${PROJECT_SOURCE_DIR}/src")

# Group headers and sources.
# Some IDEs use this for nicer file structure.
#set(HDR_PRIVATE
	#lib/private-libcdsp.h
	#)

#set(HDR_PUBLIC	
	#"${PROJECT_SOURCE_DIR}/lib/libcdsp.h"
	#"${PROJECT_BINARY_DIR}/CDSP_config.h"
	#)

set(HEADERS
	src/base.hpp
	src/cdsp.hpp
	src/define.hpp
	src/exception.hpp
	src/helper.hpp
	src/parameter.hpp
	src/sample_buffer.hpp
	src/types.hpp
	src/values.hpp
	src/primitive/primitive_base.hpp
	src/primitive/operator/operator_base.hpp
	src/primitive/operator/add.hpp
	src/primitive/operator/multiply.hpp
	src/primitive/envelope/schedule_linear.hpp
	src/primitive/table/table_abstract.hpp
	src/primitive/table/table_oscillator.hpp
)
	
set(SOURCES
	src/base.cpp
	src/exception.cpp
	src/helper.cpp
	src/parameter.cpp
	src/sample_buffer.cpp
	src/primitive/primitive_base.cpp
	src/primitive/operator/operator_base.cpp
	src/primitive/operator/add.cpp
	src/primitive/operator/multiply.cpp
	src/primitive/envelope/schedule_linear.cpp
	src/primitive/table/table_abstract.cpp
	src/primitive/table/table_oscillator.cpp
)

#if (NOT CDSP_WITHOUT_CLIENT)
	#list(APPEND SOURCES
	#	lib/client.c
	#	)
#endif()

#if (WIN32)
#	list(APPEND SOURCES
#		lib/CDSP-plat-win.c
#		)
#else()
	#list(APPEND SOURCES
	#	lib/CDSP-plat-unix.c
	#	)
#endif()


if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    include (CheckCCompilerFlag)
    CHECK_C_COMPILER_FLAG(-fvisibility=hidden HAVE_VISIBILITY)
    if (HAVE_VISIBILITY)
                set(VISIBILITY_FLAG -fvisibility=hidden)
    endif()
    if (UNIX)
		set( CMAKE_C_FLAGS "-Wall -Werror -O3 ${VISIBILITY_FLAG} ${CMAKE_C_FLAGS}" )
    else(UNIX)
		set( CMAKE_C_FLAGS "-Wall -O3 ${VISIBILITY_FLAG} ${CMAKE_C_FLAGS}" )
    endif(UNIX)
endif ()

#source_group("Headers Private"  FILES ${HDR_PRIVATE})
#source_group("Headers Public"   FILES ${HDR_PUBLIC})
source_group("Sources"          FILES ${SOURCES})

#
# Create the lib.
#
add_library(cdsp STATIC
			${HEADERS}
			${SOURCES})
			
add_library(cdsp_shared SHARED
			${HEADERS}
			${SOURCES})

if (WIN32)
	# On Windows libs have the same file ending (.lib)
	# for both static and shared libraries, so we
	# need a unique name for the static one.
	set_target_properties(cdsp 
		PROPERTIES
		OUTPUT_NAME cdsp_static)

	# Compile as DLL (export function declarations)
	set_property(
		TARGET cdsp_shared
		PROPERTY COMPILE_DEFINITIONS 
		CDSP_DLL
		CDSP_INTERNAL
		)
endif(WIN32)

# We want the shared lib to be named "libcdsp"
# not "cdsp_shared".
set_target_properties(cdsp_shared
		PROPERTIES 
		OUTPUT_NAME cdsp)

# Set the so version of the lib.
# Equivalent to LDFLAGS=-version-info x:x:x
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	foreach(lib cdsp cdsp_shared)
		set_target_properties(${lib} 
			PROPERTIES
			SOVERSION ${SOVERSION})
	endforeach()
endif()

#
# Test applications
#
set(TEST_APP_LIST)
if (CDSP_BUILD_TESTS)
	#
	# Helper function for adding a test app.
	#
	macro(create_test_app TEST_NAME MAIN_SRC)
		
		set(TEST_SRCS ${MAIN_SRC})
		set(TEST_HDR)

		#if (WIN32)
			#list(APPEND TEST_SRCS 
			#	${WIN32_HELPERS_PATH}/getopt.c
			#	${WIN32_HELPERS_PATH}/getopt_long.c
			#	${WIN32_HELPERS_PATH}/gettimeofday.c
			#)

			#list(APPEND TEST_HDR 
			#	${WIN32_HELPERS_PATH}/getopt.h
			#	${WIN32_HELPERS_PATH}/gettimeofday.h
			#)
		#endif(WIN32)

		source_group("Headers Private"   FILES ${TEST_HDR})
		source_group("Sources"   FILES ${TEST_SRCS})
		add_executable(${TEST_NAME} ${TEST_SRCS} ${TEST_HDR})
		
		if (CDSP_LINK_TESTAPPS_DYNAMIC)
			target_link_libraries(${TEST_NAME} cdsp_shared)
			add_dependencies(${TEST_NAME} cdsp_shared)
		else(CDSP_LINK_TESTAPPS_DYNAMIC)
			target_link_libraries(${TEST_NAME} cdsp)
			add_dependencies(${TEST_NAME} cdsp)
		endif(CDSP_LINK_TESTAPPS_DYNAMIC)

		# Set test app specific defines.
		set_property(TARGET ${TEST_NAME}
					PROPERTY COMPILE_DEFINITIONS 
						INSTALL_DATADIR="${CMAKE_INSTALL_PREFIX}/share"
					)

		# Prefix the binary names with libcdsp.
		set_target_properties(${TEST_NAME} 
			PROPERTIES
			OUTPUT_NAME libcdsp-${TEST_NAME})

		# Add to the list of tests.
		list(APPEND TEST_APP_LIST ${TEST_NAME})
	endmacro()

	# test apps
	create_test_app(test_table_oscillator "test/test_table_oscillator.cpp")

	# Data files for running the test server.
	#set(TEST_SERVER_DATA
	#	"${PROJECT_SOURCE_DIR}/test-server/favicon.ico"
	#	"${PROJECT_SOURCE_DIR}/test-server/leaf.jpg"
	#	"${PROJECT_SOURCE_DIR}/test-server/libcdsp.org-logo.png"
	#	"${PROJECT_SOURCE_DIR}/test-server/test.html")
endif(CDSP_BUILD_TESTS)

if (UNIX)
# Generate and install pkgconfig.
# (This is not indented, because the tabs will be part of the output)
file(WRITE "${PROJECT_BINARY_DIR}/libcdsp.pc"
"prefix=\"${CMAKE_INSTALL_PREFIX}\"
exec_prefix=\${prefix}
libdir=\${exec_prefix}/lib${LIB_SUFFIX}
includedir=\${prefix}/include

Name: libcdsp
Description: digital audio processing in C++
Version: ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}

Libs: -L\${libdir} -lcdsp
Cflags: -I\${includedir}"
)

	install(FILES "${PROJECT_BINARY_DIR}/libcdsp.pc"
		DESTINATION lib${LIB_SUFFIX}/pkgconfig)
endif(UNIX)

#
# Installation preparations.
#

if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR cmake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/cmake/libcdsp)
endif()

message("---------------------------------------------------------------------")
message("  Settings:  (For more help do cmake -LH <srcpath>")
message("---------------------------------------------------------------------")
message(" CDSP_BUILD_SHARED = ${CDSP_BUILD_SHARED}")
message(" CDSP_DEBUG_INTERNAL = ${CDSP_DEBUG_INTERNAL}")
message(" CDSP_BUILD_TESTS = ${CDSP_BUILD_TESTS}")
message("---------------------------------------------------------------------")

# These will be available to parent projects including libcdsp using add_subdirectory()
set(LIBCDSP_LIBRARIES cdsp cdsp_shared CACHE STRING "cdsp libraries")
set(LIBCDSP_LIBRARIES_STATIC cdsp CACHE STRING "cdsp static library")
set(LIBCDSP_LIBRARIES_SHARED cdsp_shared CACHE STRING "cdsp shared library")

# This must always be last!
include(CPack)
