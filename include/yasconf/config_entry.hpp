/**
 * config_entry.hpp
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

#include <array>
#include <cstddef>
#include <string_view>

namespace yasconf
{

template <std::size_t LineSize>
class ConfigEntry
{
public:
  ConfigEntry(int fd) noexcept
    : eof_{false}
    , fd_{fd}
    , line_{}
    , key_{}
    , value_{}
  {
  }

  ConfigEntry(bool eof) noexcept
    : eof_{eof}
    , fd_{-1}
    , line_{}
    , key_{}
    , value_{}
  {
  }

  std::string_view key() const
  {
    return key_;
  }

  std::string_view value() const
  {
    return value_;
  }

  bool eof() const
  {
    return eof_;
  }

  bool next()
  {
    do
    {
      if (!read_line())
      {
        return false;
      }
    } while (key_.empty());
    return true;
  }

  std::string_view operator*() const
  {
    return value_;
  }

private:
  std::string_view remove_comments(std::string_view str)
  {
    std::string_view::size_type comment = str.find_first_of('#');
    if (comment != std::string_view::npos)
    {
      return str.substr(0, comment);
    }
    return str;
  }

  bool read_line()
  {
    if (fd_ < 0)
    {
      return false;
    }

    if (eof_)
    {
      return false;
    }

    int i = read_line_from_file();
    if (i == 0)
    {
      key_ = {};
      value_ = {};
      return true;
    }

    key_ = std::string_view(&line_[0], i);
    key_ = remove_comments(key_);
    std::string_view::size_type delimiter = key_.find_first_of('=');
    if (delimiter == std::string_view::npos)
    {
      key_ = {};
      value_ = {};

      // valid but empty line
      return true;
    }

    value_ = key_;
    value_ = value_.substr(delimiter + 1);
    key_ = key_.substr(0, delimiter);

    const char *whitespaces = " \n\t\r\f\v";
    key_.remove_prefix(key_.find_first_not_of(whitespaces));
    key_.remove_suffix(key_.size() - key_.find_last_not_of(whitespaces) - 1);
    const auto first_char = value_.find_first_not_of(whitespaces);
    if (first_char == std::string_view::npos)
    {
      value_ = {};
      return true;
    }
    value_.remove_prefix(first_char);
    value_.remove_suffix(value_.size() - value_.find_last_not_of(whitespaces) - 1);
    if ((value_[0] == '\"' || value_[0] == '\'') &&
        (value_[value_.size() - 1] == '\"' || value_[value_.size() - 1] == '\''))
    {
      value_.remove_suffix(1);
      value_.remove_prefix(1);
    }
    return true;
  }

  int read_line_from_file()
  {
    std::size_t i = 0;
    for (i = 0; i < LineSize; ++i)
    {
      if (read(fd_, &line_[i], 1) != 1)
      {
        line_[i] = '\0';
        eof_ = true;
        // no more data
        break;
      }

      if (line_[i] == '\n')
      {
        line_[i] = '\0';
        break;
      }
    }
    return i;
  }

  bool eof_;
  int fd_;
  std::array<char, LineSize> line_;
  std::string_view key_;
  std::string_view value_;
};

} // namespace yasconf
