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

add_library(yasconf)

target_sources(
  yasconf
  PUBLIC ${PROJECT_SOURCE_DIR}/include/yasconf/config_parser.hpp
         ${PROJECT_SOURCE_DIR}/include/yasconf/file_reader.hpp
         ${PROJECT_SOURCE_DIR}/include/yasconf/yasconf_error_codes.hpp
         ${PROJECT_SOURCE_DIR}/include/yasconf/section.hpp
  PRIVATE config_parser.cpp file_reader.cpp yasconf_error_codes.cpp section.cpp)

find_package(eul REQUIRED)

target_link_libraries(
  yasconf
  PUBLIC eul::error
  PRIVATE yasconf_flags)

target_include_directories(yasconf PUBLIC ${PROJECT_SOURCE_DIR}/include)
