cmake_minimum_required(VERSION 3.31)
project(jnikot_cpp VERSION 0.1.0 LANGUAGES CXX)

if (NOT JNIKOT_OUTPUT_PROGRAM_NAME)
    set(JNIKOT_OUTPUT_PROGRAM_NAME kotlin_cpp_test)
endif ()

set(CMAKE_CXX_STANDARD 23)

set(JNI_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/vendor/jdk/lib")

file(GLOB_RECURSE MODULES "src/*.cppm" "src/*.ixx")
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_library(${PROJECT_NAME} ${MODULES} ${SOURCES})

target_sources(${PROJECT_NAME}
        PRIVATE FILE_SET cxx_modules
        TYPE CXX_MODULES
        FILES
        ${MODULES}
)

file(GLOB_RECURSE TEST_SOURCES "test/*.cpp" "test/*.h")
file(GLOB_RECURSE TEST_MODULES "test/*.cppm" "test/*.ixx")

add_executable(kotlin_cpp_test ${MODULES} ${SOURCES} ${TEST_SOURCES} ${TEST_MODULES})

target_sources(kotlin_cpp_test
        PRIVATE FILE_SET cxx_modules
        TYPE CXX_MODULES
        FILES
        ${MODULES}
        ${TEST_MODULES}
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/jdk/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/jdk/include/win32>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(${PROJECT_NAME}
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_include_directories(kotlin_cpp_test
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/jdk/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/jdk/include/win32>
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/jdk/lib/jvm.lib
)

target_link_libraries(kotlin_cpp_test
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/jdk/lib/jvm.lib
)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/run.bat
        "@echo off\n"
        "set OLD_PATH=%PATH%\n"
        "set PATH=%~dp0jdk/bin/server;%PATH%\n"
        "${JNIKOT_OUTPUT_PROGRAM_NAME}.exe\n"
        "set PATH=%OLD_PATH%\n"
)
# all files
file(COPY vendor/jdk
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
        FILES_MATCHING PATTERN "*")

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

#install(TARGETS ${PROJECT_NAME}
#        EXPORT ${PROJECT_NAME}Targets
#        FILE_SET cxx_modules
#        DESTINATION ${CMAKE_INSTALL_LIBDIR}
#)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

#install(FILES
#        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
#        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
#        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
#)
#
#install(EXPORT ${PROJECT_NAME}Targets
#        FILE ${PROJECT_NAME}Targets.cmake
#        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
#)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/jni-bind>
)

add_subdirectory(vendor/jni-bind)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        jni_bind
)

target_link_libraries(kotlin_cpp_test
        PUBLIC
        jni_bind
)