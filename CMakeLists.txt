#
# CMakeLists.txt
#
# Copyright (C) 2024 Mateusz Stadnik <matgla@live.com>
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this program. If not, see <https://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.10)

include(FetchContent)

FetchContent_Declare(
  yaspem
  GIT_REPOSITORY https://github.com/matgla/Yaspem.git
  GIT_TAG v1.2)

FetchContent_MakeAvailable(yaspem)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    ${yaspem_SOURCE_DIR}/cmake
    CACHE INTERNAL "")

project(yasconf CXX)

include(yaspem)
setup_yaspem(
  YASPEM_SOURCE ${yaspem_SOURCE_DIR} OUTPUT_DIRECTORY
  ${CMAKE_CURRENT_BINARY_DIR}/packages PACKAGE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/packages.json)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(yasconf_flags INTERFACE)

target_compile_options(
  yasconf_flags
  INTERFACE -Wall
            -Wextra
            -Werror
            -Wshadow
            -Wdouble-promotion
            -Wno-format
            -Wformat-truncation
            -Wundef
            -Wconversion
            -ffunction-sections
            -fdata-sections
            -Wno-unused-parameter
            $<$<CONFIG:Debug>:-Og
            -g
            -ggdb3>
            $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions
            -std=c++23
            -fno-rtti>)

add_subdirectory(source)

if(NOT DISABLE_YASCONF_TESTS)
  add_subdirectory(tests)
endif()
