/**
 * config_entry_const_iterator.hpp
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

#include <iterator>

#include "yasconf/config_entry.hpp"

namespace yasconf
{

template <std::size_t BufferSize>
class ConfigEntryConstIterator
{
public:
  using iterator_category = std::input_iterator_tag;
  using difference_type = int;
  using value_type = ConfigEntry<BufferSize>;
  using pointer = const ConfigEntry<BufferSize> *;
  using reference = const ConfigEntry<BufferSize> &;

  using self = ConfigEntryConstIterator<BufferSize>;
 
  ConfigEntryConstIterator(int fd) 
    : value_(fd) 
  {
    value_.next();
  }

  ConfigEntryConstIterator(bool eof)
    : value_(eof)
  {
  }

  bool operator==(const self &other) const
  {
    if (other.value_.eof() == value_.eof())
    {
      return true;
    }
    return value_.key() == other.value_.key() &&
           value_.value() == other.value_.value();
  }

  bool operator!=(const self &other) const
  {
    return !this->operator==(other);
  }

  reference operator*() const
  {
    return value_;
  }

  pointer operator->() const
  {
    return &value_;
  }

  self& operator++() 
  {
    value_.next(); 
    return *this;
  }

  self operator++(int)
  {
    self copy = *this;
    value_.next();
    return copy;
  }

private:
  value_type value_;
};

} // namespace yasconf
