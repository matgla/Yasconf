/**
 * element.cpp
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

#include "yasconf/section.hpp"

#include <cstdio>
#include <cstring>

#include "yasconf/yasconf_error_codes.hpp"

namespace yasconf
{

Section::Section(FileReader &file, int start_line, int end_line)
    : file_(file), start_line_(start_line), end_line_(end_line)
{
  eul::error::error_code ec;
  file_.set_line(start_line, ec);
  std::string_view line = file_.next_line(ec);
  while (!line.starts_with('[') && !ec)
  {
    line = file_.next_line(ec);
  }

  std::size_t last_dot = line.find_last_of('.');
  if (last_dot != std::string_view::npos)
  {
    line = line.substr(last_dot, line.size() - last_dot);
  }
  line.remove_suffix(1);
  line.remove_prefix(1);

  std::memcpy(name_, line.data(), line.size());
  name_[line.size()] = '\0';
}

Section &Section::operator=(const Section &section)
{
  file_ = section.file_;
  start_line_ = section.start_line_;
  end_line_ = section.end_line_;
  std::memcpy(name_, section.name_, std::strlen(section.name_));
  return *this;
}

std::string_view Section::operator[](const std::string_view &key)
{
  int line = 0;
  while (line < end_line_)
  {
    eul::error::error_code ec;
    file_.set_line(line, ec);
    if (ec)
    {
      return {};
    }

    std::string_view line_view = file_.next_line(ec);
    if (ec)
    {
      return {};
    }

    std::string_view element_key = line_view.substr(0, line_view.find('='));
    if (element_key == key)
    {
      return line_view.substr(line_view.find('=') + 1);
    }
    ++line;
  }
  return {};
}

std::string_view Section::name() const
{
  return name_;
}

} // namespace yasconf
