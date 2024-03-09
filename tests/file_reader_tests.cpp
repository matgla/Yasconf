/**
 * file_reader_tests.cpp
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

#include <filesystem>

#include "yasconf/file_reader.hpp"
#include "yasconf/yasconf_error_codes.hpp"

namespace yasconf
{

class FileReaderShould : public ::testing::Test
{
};

TEST_F(FileReaderShould, ReadLines)
{
  auto test_file = std::filesystem::current_path() / "data/simple_config.txt";
  printf("Test file: %s\n", test_file.c_str());
  std::array<char, 128> buffer;
  yasconf::FileReader sut(test_file.c_str(), std::span<char>(buffer.begin(), buffer.end()));
  ASSERT_TRUE(sut.is_open());
  std::vector<std::string> lines;

  eul::error::error_code ec;
  while (ec != YasconfErrors::EndOfFile && ec != YasconfErrors::FileNotOpen &&
         ec != YasconfErrors::FileOperationFailed)
  {
    lines.push_back(std::string(sut.next_line(ec)));
  }

  std::vector<std::string> expected_lines = {"[section_a]",
                                             "name = \"This is testing name\"",
                                             "other_name = but also this is string",
                                             "some_number = 12345",
                                             "",
                                             "[section_b]",
                                             "subsection_value = value in subsection",
                                             "",
                                             "[section_c]",
                                             "name = This is section c",
                                             "value = -129",
                                             ""};

  EXPECT_THAT(lines, ::testing::ContainerEq(expected_lines));
}

TEST_F(FileReaderShould, ReturnEmptyStringWhenFileNotOpen)
{
  std::array<char, 128> buffer;
  yasconf::FileReader sut("non_existing_file.txt", std::span<char>(buffer.begin(), buffer.end()));
  ASSERT_FALSE(sut.is_open());
  eul::error::error_code ec;
  auto line = sut.next_line(ec);
  EXPECT_TRUE(line.empty());
  EXPECT_EQ(ec, YasconfErrors::FileNotOpen);
}

TEST_F(FileReaderShould, ReturnEmptyStringWhenFileOperationFailed)
{
  auto test_file = std::filesystem::current_path() / "data";
  std::array<char, 128> buffer;
  yasconf::FileReader sut(test_file.c_str(), std::span<char>(buffer.begin(), buffer.end()));
  ASSERT_TRUE(sut.is_open());
  eul::error::error_code ec;
  auto line = sut.next_line(ec);
  EXPECT_TRUE(line.empty());
  EXPECT_EQ(ec, YasconfErrors::FileOperationFailed);
}

TEST_F(FileReaderShould, ReturnEmptyStringWhenEndOfFile)
{
  auto test_file = std::filesystem::current_path() / "data/empty_file.txt";
  std::array<char, 128> buffer;
  yasconf::FileReader sut(test_file.c_str(), std::span<char>(buffer.begin(), buffer.end()));
  ASSERT_TRUE(sut.is_open());
  eul::error::error_code ec;
  auto line = sut.next_line(ec);
  EXPECT_TRUE(line.empty());
  EXPECT_EQ(ec, YasconfErrors::EndOfFile);
}

TEST_F(FileReaderShould, ReturnPartialStringWithErrorWhenBufferTooSmall)
{
  auto test_file = std::filesystem::current_path() / "data/simple_config.txt";
  std::array<char, 2> buffer;
  yasconf::FileReader sut(test_file.c_str(), std::span<char>(buffer.begin(), buffer.end()));
  ASSERT_TRUE(sut.is_open());
  eul::error::error_code ec;
  auto line = sut.next_line(ec);
  EXPECT_THAT(line, ::testing::StrEq("["));
  EXPECT_EQ(ec, YasconfErrors::BufferTooSmall);
}

TEST_F(FileReaderShould, SetLine)
{
  auto test_file = std::filesystem::current_path() / "data/simple_config.txt";
  std::array<char, 128> buffer;
  yasconf::FileReader sut(test_file.c_str(), std::span<char>(buffer.begin(), buffer.end()));
  ASSERT_TRUE(sut.is_open());
  eul::error::error_code ec;

  sut.set_line(3, ec);
  EXPECT_FALSE(ec);
  auto line = sut.next_line(ec);
  EXPECT_THAT(line, ::testing::StrEq("some_number = 12345"));

  sut.set_line(5, ec);
  EXPECT_FALSE(ec);
  line = sut.next_line(ec);
  EXPECT_THAT(line, ::testing::StrEq("[section_b]"));
}

TEST_F(FileReaderShould, SetErrorWhenSetLineIsOutOfFile)
{
  auto test_file = std::filesystem::current_path() / "data/simple_config.txt";
  std::array<char, 128> buffer;
  yasconf::FileReader sut(test_file.c_str(), std::span<char>(buffer.begin(), buffer.end()));
  ASSERT_TRUE(sut.is_open());
  eul::error::error_code ec;

  sut.set_line(9999, ec);
  EXPECT_EQ(ec, YasconfErrors::EndOfFile);
  auto line = sut.next_line(ec);
  EXPECT_THAT(line, ::testing::StrEq(""));
}

} // namespace yasconf
