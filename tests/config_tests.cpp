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

#include <gtest/gtest.h>

#include "yasconf/config.hpp"

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

} // namespace yasconf
