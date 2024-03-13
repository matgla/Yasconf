/**
 * config_tests.cpp
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

#include "yasconf/config.hpp"

#include <experimental/array>
#include <utility>

namespace yasconf
{

class ConfigShould : public ::testing::Test
{
};

TEST_F(ConfigShould, OpenFile)
{
  Config<256> config("./data/simple_config.txt");
  EXPECT_TRUE(config.is_open());
}

TEST_F(ConfigShould, ReportFalseWhenOpeningFailed)
{
  Config<256> config("./data/nonexistent.txt");
  EXPECT_FALSE(config.is_open());
}

const auto simple_config_expect = std::experimental::make_array(
  std::make_pair("parameter_a", "1.234"),
  std::make_pair("par_without_space", "12345678"),
  std::make_pair("some_string", "Hello World!"),
  std::make_pair("other_string_convention", "other string"),
  std::make_pair("or_even_this", "to be or not to be"),
  std::make_pair("but_in_the_middle", "this is 'quote' string"),
  std::make_pair("integer", "1028398"), std::make_pair("comment_after_code", "1234"),
  std::make_pair("empty_value", ""), std::make_pair("comment_value", ""),
  std::make_pair("partial_quote_begin", "'test"),
  std::make_pair("partial_quote_end", "end'"),
  std::make_pair("\"wrong key?\"", "wrong value"));

TEST_F(ConfigShould, ReadLines)
{
  Config<128> config("./data/simple_config.txt");
  auto entry = config.first();

  for (const auto &e : simple_config_expect)
  {
    EXPECT_FALSE(entry.eof());
    EXPECT_THAT(entry.key(), ::testing::StrEq(e.first));
    EXPECT_THAT(entry.value(), ::testing::StrEq(e.second));
    entry.next();
  }
  EXPECT_TRUE(entry.eof());
}

TEST_F(ConfigShould, ReadLinesWithWithAccessOperator)
{
  Config<128> config("./data/simple_config.txt");
  auto entry = config.first();

  for (const auto &e : simple_config_expect)
  {
    EXPECT_FALSE(entry.eof());
    EXPECT_THAT(entry.key(), ::testing::StrEq(e.first));
    EXPECT_THAT(*entry, ::testing::StrEq(e.second));
    entry.next();
  }
  EXPECT_TRUE(entry.eof());
}

TEST_F(ConfigShould, ReturnCorrectKeys)
{
  const auto simple_config_expect = std::experimental::make_array(
    std::make_pair("parameter_a", "1.234"),
    std::make_pair("or_even_this", "to be or not to be"),
    std::make_pair("partial_quote_begin", "'test"),
    std::make_pair("but_in_the_middle", "this is 'quote' string"),
    std::make_pair("integer", "1028398"),
    std::make_pair("comment_after_code", "1234"), std::make_pair("empty_value", ""),
    std::make_pair("comment_value", ""),
    std::make_pair("other_string_convention", "other string"),
    std::make_pair("partial_quote_end", "end'"),
    std::make_pair("par_without_space", "12345678"),
    std::make_pair("some_string", "Hello World!"),
    std::make_pair("\"wrong key?\"", "wrong value"));

  Config<128> config("./data/simple_config.txt");

  for (const auto &e : simple_config_expect)
  {
    EXPECT_THAT(*config[e.first], e.second);
  }

  EXPECT_TRUE(config["no such key"].eof());
}

TEST_F(ConfigShould, ProvideRangeLoopIterators)
{
  Config<128> config("./data/simple_config.txt");

  int index = 0;
  for (const auto &entry : config)
  {
    EXPECT_THAT(entry.key(), ::testing::StrEq(simple_config_expect[index].first));
    EXPECT_THAT(entry.value(), ::testing::StrEq(simple_config_expect[index].second));
    ++index;
  }

  index = 0;
  for (const auto &entry : config)
  {
    EXPECT_THAT(entry.key(), ::testing::StrEq(simple_config_expect[index].first));
    EXPECT_THAT(entry.value(), ::testing::StrEq(simple_config_expect[index].second));
    ++index;
  }
}

} // namespace yasconf
