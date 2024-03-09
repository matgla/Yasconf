/**
 * parser_tests.cpp
 *
 * Copyright (C) 2024 Mateusz Stadnik <matgla@live.com>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version
 * 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General
 * Public License along with this program. If not, see
 * <https://www.gnu.org/licenses/>.
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <array>
#include <filesystem>

#include "yasconf/config_parser.hpp"

class ParserShould : public ::testing::Test
{
};

TEST_F(ParserShould, OpenFileCorrectly)
{
  auto test_file = std::filesystem::current_path() / "data/simple_config.txt";
  std::array<char, 128> buffer;
  yasconf::ConfigParser sut(std::string_view(test_file.c_str()), buffer);
  EXPECT_TRUE(sut.is_open());
}

TEST_F(ParserShould, PropagateOpeningFailure)
{
  auto test_file = std::filesystem::current_path() / "data/non_existing_simple_config.txt";

  std::array<char, 128> buffer;
  yasconf::ConfigParser sut(std::string_view(test_file.c_str()), buffer);
  EXPECT_FALSE(sut.is_open());
}

TEST_F(ParserShould, GetSectionName)
{
  auto test_file = std::filesystem::current_path() / "data/simple_config.txt";
  std::array<char, 128> buffer;
  yasconf::ConfigParser sut(std::string_view(test_file.c_str()), buffer);
  EXPECT_TRUE(sut.is_open());

  yasconf::Section section = sut["section_a"];
  EXPECT_THAT(section.name(), testing::StrEq("section_a"));

  section = sut["section_b"];
  EXPECT_THAT(section.name(), testing::StrEq("section_b"));
}

// TEST_F(ParserShould, AccessSections)
// {
//   auto test_file = std::filesystem::current_path() / "data/simple_config.txt";
//   std::array<char, 128> buffer;
//   yasconf::ConfigParser sut(std::string_view(test_file.c_str()), buffer);
//   EXPECT_TRUE(sut.is_open());

//   EXPECT_THAT(sut["section_a"]["name"], testing::StrEq("This is testing name"));
//   EXPECT_THAT(sut["section_a"]["other_name"], testing::StrEq("but also this is string"));
//   EXPECT_THAT(sut["section_a"]["some_number"], testing::StrEq("some_number = 12345"));

//   EXPECT_THAT(sut["section_b"]["name"], testing::StrEq("This is section b"));
//   EXPECT_THAT(sut["section_b"]["value"], testing::StrEq("-129"));
// };

// TEST_F(ParserShould, AccessSubsections)
// {
//   auto test_file = std::filesystem::current_path() / "data/simple_config.txt";
//   std::array<char, 128> buffer;
//   yasconf::ConfigParser sut(std::string_view(test_file.c_str()), buffer);
//   EXPECT_TRUE(sut.is_open());

//   EXPECT_THAT(sut["section_a.subsection"]["subsection_value"],
//               testing::StrEq("value in subsection"));
//   EXPECT_THAT(sut["section_a"]["subsection"]["subsection_value"],
//               testing::StrEq("value in subsection"));
// }
