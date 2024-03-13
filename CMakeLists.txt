#
# CMakeLists.txt
#
# Copyright (C) 2024 Mateusz Stadnik <matgla@live.com>
#
# This program is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General
# Public License along with this program. If not, see
# <https://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.22)

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

add_library(yasconf INTERFACE)
target_sources(yasconf 
  INTERFACE 
    include/yasconf/config.hpp
    include/yasconf/config_entry_iterator.hpp
)
target_include_directories(yasconf INTERFACE include)

if (NOT DISABLE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif ()
