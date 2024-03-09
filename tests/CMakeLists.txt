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

include(yaspem)

setup_yaspem(
  YASPEM_SOURCE ${yaspem_SOURCE_DIR} OUTPUT_DIRECTORY
  ${CMAKE_CURRENT_BINARY_DIR}/packages PACKAGE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/packages.json)

add_executable(yasconf_tests)

target_sources(yasconf_tests PRIVATE parser_tests.cpp file_reader_tests.cpp)

find_package(googletest REQUIRED)

target_link_libraries(yasconf_tests PRIVATE GTest::gtest_main GTest::gmock
                                            yasconf yasconf_flags)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/simple_config.txt
               ${PROJECT_BINARY_DIR}/data/simple_config.txt COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/empty_file.txt
               ${PROJECT_BINARY_DIR}/data/empty_file.txt COPYONLY)
