cmake_minimum_required (VERSION 3.5)
project(SnetDevice)

file(STRINGS "${PROJECT_SOURCE_DIR}/libversion" SHARED_LIBRARY_VERSION)
# Read the version information from the VERSION file
file(STRINGS "${PROJECT_SOURCE_DIR}/VERSION" PACKAGE_VERSION)
string(REGEX REPLACE "([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" CPACK_PACKAGE_VERSION_MAJOR ${PACKAGE_VERSION})
string(REGEX REPLACE "[0-9]+\\.([0-9]+)\\.[0-9]+.*" "\\1" CPACK_PACKAGE_VERSION_MINOR ${PACKAGE_VERSION})
string(REGEX REPLACE "[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" CPACK_PACKAGE_VERSION_PATCH ${PACKAGE_VERSION})

set(PROJECT_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(RELEASE_NAME "Unstable-trunk")

# Put the libaries and binaries that get built into directories at the
# top of the build tree rather than in hard-to-find leaf
# directories. This simplifies manual testing and the use of the build
# tree rather than installed Boost libraries.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# Windows DLLs are "runtime" for CMake. Output them to "bin" like the Visual Studio projects do.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Append our module directory to CMake
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# this project is only shared libs, because libUsb lincens is LGPL
set(BUILD_SHARED_LIBS ON)

# Allow enabling and disabling components
include(DefinePlatformSpecifc)

# Collect the built libraries and include dirs, the will be used to create the SnetDeviceConfig.cmake file

include(SnetDeviceMacros)
# Sources
file(GLOB SRCS_G "src/*.cpp" 
				)
				
SNETDEVICE_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
SNETDEVICE_HEADERS_AUTO(SRCS ${HDRS_G})

add_library(${PROJECT_NAME} ${SRCS})
add_library(Emotion::SnetDevice ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME}
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME EmotionSnetDevice
	DEFINE_SYMBOL SnetDevice_EXPORTS
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(${PROJECT_NAME}
	PUBLIC
		$<$<CONFIG:Debug>:_DEBUG>
		$<$<BOOL:${DISABLE_CPP14}>:ELBATS_DISABLE_CPP14>
		$<$<NOT:$<BOOL:${DISABLE_CPP14}>>:ELBATS_ENABLE_CPP14>
		$<$<BOOL:${DISABLE_CPP11}>:ELBATS_DISABLE_CPP11>
		$<$<NOT:$<BOOL:${DISABLE_CPP11}>>:ELBATS_ENABLE_CPP11>
)

target_compile_features(${PROJECT_NAME}
	PUBLIC
		$<$<NOT:$<BOOL:${DISABLE_CPP11}>>:cxx_defaulted_move_initializers>
)

if(NOT BUILD_SHARED_LIBS)
	target_compile_definitions(${PROJECT_NAME}
		PUBLIC
			SNETDEVICE_STATIC
	)
endif()

if(NOT DISABLE_CPP14 AND CMAKE_VERSION VERSION_GREATER "3.8")
	target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
endif()

if(WIN32)
	target_compile_definitions(${PROJECT_NAME} PUBLIC SNETDEVICE_NO_AUTOMATIC_LIBS UNICODE _UNICODE)
	target_link_libraries(${PROJECT_NAME} PUBLIC iphlpapi)
endif()

if(true)
	if(NOT BUILD_SHARED_LIBS)
		set_property(TARGET Common PROPERTY POSITION_INDEPENDENT_CODE ON) # This is needed to build TestLibrary.so as shared.
	endif()
	add_subdirectory(samples)
endif()

#############################################################
# Uninstall stuff see: http://www.vtk.org/Wiki/CMake_FAQ
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

add_custom_target(uninstall
	"${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
)

#############################################################
# Enable packaging

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME} Libraries")
set(CPACK_PACKAGE_VENDOR "E-Motion Inc.")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
if(WIN32)
	set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
else()
	set(CPACK_PACKAGE_INSTALL_DIRECTORY "/usr/local")
endif()

include(CPack)

#############################################################
# cmake config files

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

# Set config script install location in a location that find_package() will
# look for, which is different on MS Windows than for UNIX
# Note: also set in SNETDEViCE_GENERATE_PACKAGE macro in cmake/SnetDeviceMacros.cmake
if(WIN32)
	set(SnetDeviceConfigPackageLocation "cmake")
else()
	set(SnetDeviceConfigPackageLocation "lib${LIB_SUFFIX}/cmake/${PROJECT_NAME}")
endif()

configure_file(cmake/${PROJECT_NAME}Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake" @ONLY)
install(
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake
	DESTINATION
		"${SnetDeviceConfigPackageLocation}"
	COMPONENT
		Devel
)

message(STATUS "CMake ${CMAKE_VERSION} successfully configured ${PROJECT_NAME} using ${CMAKE_GENERATOR} generator")
message(STATUS "${PROJECT_NAME} package version: ${PROJECT_VERSION}")
if(BUILD_SHARED_LIBS)
	message(STATUS "Building dynamic libraries")
else()
	message(STATUS "Building static libraries")
endif()
message(STATUS "[cmake] Installation target path: ${CMAKE_INSTALL_PREFIX}")
if(CMAKE_TOOLCHAIN_FILE)
	message(STATUS "[cmake] Use toolchain file:		${CMAKE_TOOLCHAIN_FILE}")
endif()
message(STATUS "[cmake] Bulid for OS type:		${CMAKE_SYSTEM_NAME}")
message(STATUS "[cmake] Build for OS version:	${CMAKE_SYSTEM_VERSION}")
message(STATUS "[cmake] Build for CPU type:		${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "[cmake] Build type:				${CMAKE_BUILD_TYPE}")
string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE)
message(STATUS "[cmake] Build with cxx flags:	${CMAKE_CXX_FLAGS_${BUILD_TYPE}} ${CMAKE_CXX_FLAGS}")
message(STATUS "[cmake] Build with c flags:		${CMAKE_C_FLAGS_${BUILD_TYPE}} ${CMAKE_C_FLAGS}")