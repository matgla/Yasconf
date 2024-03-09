/**
 * config_parser.cpp
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

#include "yasconf/config_parser.hpp"

#include <span>

#include <fcntl.h>
#include <unistd.h>

namespace yasconf
{

ConfigParser::ConfigParser(const std::string_view &file, std::span<char> buffer) noexcept
    : file_(file, buffer)
{
  last_line_ = 0;
  eul::error::error_code ec;
  while (!ec)
  {
    file_.next_line(ec);
    if (!ec)
    {
      ++last_line_;
    }
  }
}

ConfigParser::~ConfigParser() noexcept
{
}

bool ConfigParser::is_open() const
{
  return file_.is_open();
}

Section ConfigParser::operator[](const std::string_view &section)
{
  int line = 0;
  eul::error::error_code ec;
  file_.set_line(line, ec);
  while (line < last_line_)
  {
    std::string_view line_data = file_.next_line(ec);
    ++line;
    if (line_data.starts_with('['))
    {
      line_data.remove_prefix(1);
      line_data.remove_suffix(1);
      if (line_data == section)
      {
        return Section(file_, line - 1, last_line_);
      }
    }
  }
  return Section(file_, 0, 0);
}

} // namespace yasconf
