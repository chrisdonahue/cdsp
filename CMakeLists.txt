cmake_minimum_required(VERSION 2.8)

project(cdsp C)

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

#option(CDSP_WITH_SSL "Include SSL support (default OpenSSL, CyaSSL if CDSP_USE_CYASSL is set)" ON)
#option(CDSP_SSL_CLIENT_USE_OS_CA_CERTS "SSL support should make use of OS installed CA root certs" ON)
#option(CDSP_USE_EXTERNAL_ZLIB "Search the system for ZLib instead of using the included one (on Windows)" OFF)
#option(CDSP_USE_CYASSL "Use CyaSSL replacement for OpenSSL. When settings this, you also need to specify CDSP_CYASSL_LIB and CDSP_CYASSL_INCLUDE_DIRS" OFF)
#option(CDSP_WITHOUT_BUILTIN_GETIFADDRS "Don't use BSD getifaddrs implementation from libcdsp if it is missing (this will result in a compilation error) ... Default is your libc provides it. On some systems such as uclibc it doesn't exist." OFF)
#option(CDSP_WITHOUT_CLIENT "Don't build the client part of the library" OFF)
#option(CDSP_WITHOUT_SERVER "Don't build the server part of the library" OFF)
#option(CDSP_LINK_TESTAPPS_DYNAMIC "Link the test apps to the shared version of the library. Default is to link statically" OFF)
option(CDSP_WITHOUT_TESTAPPS "Don't build the cdsp-test-apps" OFF)

# Allow the user to override installation directories.
set(CDSP_INSTALL_LIB_DIR       lib CACHE PATH "Installation directory for libraries")
set(CDSP_INSTALL_BIN_DIR       bin CACHE PATH "Installation directory for executables")
set(CDSP_INSTALL_INCLUDE_DIR   include CACHE PATH "Installation directory for header files")
set(CDSP_INSTALL_EXAMPLES_DIR  bin CACHE PATH "Installation directory for example files")

#if (CDSP_WITHOUT_CLIENT AND CDSP_WITHOUT_SERVER)
#	message(FATAL_ERROR "Makes no sense to compile without both client or server.")
#endif()

#if (CDSP_WITHOUT_DEBUG)
	#set(_DEBUG 0)
#else()
	#set(_DEBUG 1)
#endif()

if (MINGW)
	set(CDSP_MINGW_SUPPORT 1)
endif()

include_directories("${PROJECT_BINARY_DIR}")

include(CheckCSourceCompiles)

# Check for different inline keyword versions.
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

# Put the libaries and binaries that get built into directories at the
# top of the build tree rather than in hard-to-find leaf directories. 
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

# Put absolute path of dynamic libraries into the object code. Some
# architectures, notably Mac OS X, need this.
SET(CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CDSP_INSTALL_LIB_DIR}${LIB_SUFFIX}")

# So we can include the CMake generated config file only when
# building with CMAKE.
add_definitions(-DCMAKE_BUILD)

include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckTypeSize)

CHECK_FUNCTION_EXISTS(bzero HAVE_BZERO)
CHECK_FUNCTION_EXISTS(fork HAVE_FORK)
CHECK_FUNCTION_EXISTS(getenv HAVE_GETENV)
CHECK_FUNCTION_EXISTS(malloc HAVE_MALLOC)
CHECK_FUNCTION_EXISTS(memset HAVE_MEMSET)
CHECK_FUNCTION_EXISTS(realloc HAVE_REALLOC)
CHECK_FUNCTION_EXISTS(socket HAVE_SOCKET)
CHECK_FUNCTION_EXISTS(strerror HAVE_STRERROR)
CHECK_FUNCTION_EXISTS(vfork HAVE_VFORK)
CHECK_FUNCTION_EXISTS(getifaddrs HAVE_GETIFADDRS)

if (NOT HAVE_GETIFADDRS)
	if (CDSP_WITHOUT_BUILTIN_GETIFADDRS)
		message(FATAL_ERROR "No getifaddrs was found on the system. Turn off the CDSP_WITHOUT_BUILTIN_GETIFADDRS compile option to use the supplied BSD version.")
	endif()

	set(CDSP_BUILTIN_GETIFADDRS 1)
endif()

CHECK_INCLUDE_FILE(dlfcn.h HAVE_DLFCN_H)
CHECK_INCLUDE_FILE(fcntl.h HAVE_FCNTL_H)
CHECK_INCLUDE_FILE(in6addr.h HAVE_IN6ADDR_H)
CHECK_INCLUDE_FILE(inttypes.h HAVE_INTTYPES_H)
CHECK_INCLUDE_FILE(memory.h HAVE_MEMORY_H)
CHECK_INCLUDE_FILE(netinet/in.h HAVE_NETINET_IN_H)
CHECK_INCLUDE_FILE(stdint.h HAVE_STDINT_H)
CHECK_INCLUDE_FILE(stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILE(strings.h HAVE_STRINGS_H)
CHECK_INCLUDE_FILE(string.h HAVE_STRING_H)
CHECK_INCLUDE_FILE(sys/prctl.h HAVE_SYS_PRCTL_H)
CHECK_INCLUDE_FILE(sys/socket.h HAVE_SYS_SOCKET_H)
CHECK_INCLUDE_FILE(sys/stat.h HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE(sys/types.h HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE(unistd.h HAVE_UNISTD_H)
CHECK_INCLUDE_FILE(vfork.h HAVE_VFORK_H)
CHECK_INCLUDE_FILE(zlib.h HAVE_ZLIB_H)

# TODO: These can be tested if they actually work also...
set(HAVE_WORKING_FORK HAVE_FORK)
set(HAVE_WORKING_VFORK HAVE_VFORK)

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
       "${PROJECT_BINARY_DIR}/CDSP_config.h")

if (MSVC)
	# Turn off stupid microsoft security warnings.
	#add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
endif(MSVC)

include_directories("${PROJECT_SOURCE_DIR}/lib")

# Group headers and sources.
# Some IDEs use this for nicer file structure.
#set(HDR_PRIVATE
	#lib/private-libcdsp.h
	#)

#set(HDR_PUBLIC	
	#"${PROJECT_SOURCE_DIR}/lib/libcdsp.h"
	#"${PROJECT_BINARY_DIR}/CDSP_config.h"
	#)

set(SOURCES
	src_flat/add.c
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
			#${HDR_PRIVATE}
			#${HDR_PUBLIC}
			${SOURCES})
add_library(cdsp SHARED
			#${HDR_PRIVATE}
			#${HDR_PUBLIC}
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
if (NOT CDSP_WITHOUT_TESTAPPS)
	#
	# Helper function for adding a test app.
	#
	macro(create_test_app TEST_NAME MAIN_SRC)
		
		set(TEST_SRCS ${MAIN_SRC})
		set(TEST_HDR)

		if (WIN32)
			list(APPEND TEST_SRCS 
				${WIN32_HELPERS_PATH}/getopt.c
				${WIN32_HELPERS_PATH}/getopt_long.c
				${WIN32_HELPERS_PATH}/gettimeofday.c
			)

			list(APPEND TEST_HDR 
				${WIN32_HELPERS_PATH}/getopt.h
				${WIN32_HELPERS_PATH}/gettimeofday.h
			)
		endif(WIN32)

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
	create_test_app(test-server "test/test_table_oscillator.cpp")

	# Data files for running the test server.
	#set(TEST_SERVER_DATA
	#	"${PROJECT_SOURCE_DIR}/test-server/favicon.ico"
	#	"${PROJECT_SOURCE_DIR}/test-server/leaf.jpg"
	#	"${PROJECT_SOURCE_DIR}/test-server/libcdsp.org-logo.png"
	#	"${PROJECT_SOURCE_DIR}/test-server/test.html")
endif(NOT CDSP_WITHOUT_TESTAPPS)

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

set(CDSP_INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

# Export targets (This is used for other CMake projects to easily find the libraries and include files).
export(TARGETS cdsp cdsp_shared
        FILE "${PROJECT_BINARY_DIR}/LibcdspTargets.cmake")
export(PACKAGE libcdsp)

# Generate the config file for the build-tree.
set(CDSP__INCLUDE_DIRS 
    "${PROJECT_SOURCE_DIR}/lib"
    "${PROJECT_BINARY_DIR}")
set(LIBcdsp_INCLUDE_DIRS ${CDSP__INCLUDE_DIRS} CACHE PATH "Libcdsp include directories")
configure_file(${PROJECT_SOURCE_DIR}/cmake/LibcdspConfig.cmake.in
                ${PROJECT_BINARY_DIR}/LibcdspConfig.cmake 
                @ONLY)

# Generate the config file for the installation tree.
get_filename_component(CDSP_ABSOLUTE_INSTALL_CMAKE_DIR ${CDSP_INSTALL_CMAKE_DIR} ABSOLUTE)
get_filename_component(CDSP_ABSOLUTE_INSTALL_INCLUDE_DIR ${CDSP_INSTALL_INCLUDE_DIR} ABSOLUTE)
file(RELATIVE_PATH 
    REL_INCLUDE_DIR 
    "${CDSP_ABSOLUTE_INSTALL_CMAKE_DIR}"
    "${CDSP_ABSOLUTE_INSTALL_INCLUDE_DIR}") # Calculate the relative directory from the cmake dir.

# Note the EVENT_CMAKE_DIR is defined in JanssonConfig.cmake.in, 
# we escape it here so it's evaluated when it is included instead
# so that the include dirs are given relative to where the 
# config file is located.
set(CDSP__INCLUDE_DIRS 
    "\${CDSP_CMAKE_DIR}/${REL_INCLUDE_DIR}") 
configure_file(${PROJECT_SOURCE_DIR}/cmake/LibcdspConfig.cmake.in
                ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/LibcdspConfig.cmake 
                @ONLY)

# Generate version info for both build-tree and install-tree.
configure_file(${PROJECT_SOURCE_DIR}/cmake/LibcdspConfigVersion.cmake.in
                ${PROJECT_BINARY_DIR}/LibcdspConfigVersion.cmake 
                @ONLY)

set_target_properties(cdsp cdsp_shared 
					PROPERTIES PUBLIC_HEADER "${HDR_PUBLIC}")

# Install exports for the install-tree.
install(EXPORT LibcdspTargets
		DESTINATION "${CDSP_INSTALL_CMAKE_DIR}" COMPONENT dev)

# build subdir is not part of sources
set(CPACK_SOURCE_IGNORE_FILES $(CPACK_SOURCE_IGNORE_FILES) ".git" "build" "tgz" "tar.gz")

# Most people are more used to "make dist" compared to "make package_source"
add_custom_target(dist COMMAND "${CMAKE_MAKE_PROGRAM}" package_source)

include(UseRPMTools)
if (RPMTools_FOUND)
	RPMTools_ADD_RPM_TARGETS(libcdsp libcdsp.spec)
endif()

message("---------------------------------------------------------------------")
message("  Settings:  (For more help do cmake -LH <srcpath>")
message("---------------------------------------------------------------------")
message(" CDSP_WITH_SSL = ${CDSP_WITH_SSL}  (SSL Support)")
message(" CDSP_SSL_CLIENT_USE_OS_CA_CERTS = ${CDSP_SSL_CLIENT_USE_OS_CA_CERTS}")
message(" CDSP_USE_CYASSL = ${CDSP_USE_CYASSL} (CyaSSL replacement for OpenSSL)")
if (CDSP_USE_CYASSL)
	message("   CDSP_CYASSL_LIB = ${CDSP_CYASSL_LIB}")
	message("   CDSP_CYASSL_INCLUDE_DIRS = ${CDSP_CYASSL_INCLUDE_DIRS}")
endif()
message(" CDSP_WITHOUT_BUILTIN_GETIFADDRS = ${CDSP_WITHOUT_BUILTIN_GETIFADDRS}")
message(" CDSP_WITHOUT_CLIENT = ${CDSP_WITHOUT_CLIENT}")
message(" CDSP_WITHOUT_SERVER = ${CDSP_WITHOUT_SERVER}")
message(" CDSP_LINK_TESTAPPS_DYNAMIC = ${CDSP_LINK_TESTAPPS_DYNAMIC}")
message(" CDSP_WITHOUT_TESTAPPS = ${CDSP_WITHOUT_TESTAPPS}")
message(" CDSP_WITHOUT_TEST_SERVER = ${CDSP_WITHOUT_TEST_SERVER}")
message(" CDSP_WITHOUT_TEST_SERVER_EXTPOLL = ${CDSP_WITHOUT_TEST_SERVER_EXTPOLL}")
message(" CDSP_WITHOUT_TEST_PING = ${CDSP_WITHOUT_TEST_PING}")
message(" CDSP_WITHOUT_TEST_CLIENT = ${CDSP_WITHOUT_TEST_CLIENT}")
message(" CDSP_WITHOUT_TEST_FRAGGLE = ${CDSP_WITHOUT_TEST_FRAGGLE}")
message(" CDSP_WITHOUT_DEBUG = ${CDSP_WITHOUT_DEBUG}")
message(" CDSP_WITHOUT_EXTENSIONS = ${CDSP_WITHOUT_EXTENSIONS}")
message(" CDSP_WITH_LATENCY = ${CDSP_WITH_LATENCY}")
message(" CDSP_WITHOUT_DAEMONIZE = ${CDSP_WITHOUT_DAEMONIZE}")
message(" CDSP_USE_LIBEV = ${CDSP_USE_LIBEV}")
message(" CDSP_IPV6 = ${CDSP_IPV6}")
message(" CDSP_WITH_HTTP2 = ${CDSP_WITH_HTTP2}")
message("---------------------------------------------------------------------")

# These will be available to parent projects including libcdsp using add_subdirectory()
set(LIBcdsp_LIBRARIES cdsp cdsp_shared CACHE STRING "cdsp libraries")
set(LIBcdsp_LIBRARIES_STATIC cdsp CACHE STRING "cdsp static library")
set(LIBcdsp_LIBRARIES_SHARED cdsp_shared CACHE STRING "cdsp shared library")

# This must always be last!
include(CPack)
