/**
 * config.hpp
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

#pragma once

#include <span>
#include <string_view>

#include "yasconf/file_reader.hpp"
#include "yasconf/section.hpp"
#include "yasconf/section_iterator.hpp"

namespace yasconf
{

class ConfigParser
{
public:
  ConfigParser(const std::string_view &file, std::span<char> buffer) noexcept;
  ~ConfigParser() noexcept;

  bool is_open() const;

  SectionIterator sections() const;

  Section operator[](const std::string_view &section);

private:
  FileReader file_;
  int last_line_;
};

} // namespace yasconf
