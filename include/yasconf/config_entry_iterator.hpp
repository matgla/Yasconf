/**
 * entry_iterator.hpp
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
class ConfigEntryIterator
{
public:
  using iterator_category = std::input_iterator_tag;
  using difference_type = int;
  using value_type = ConfigEntry<BufferSize>;
  using pointer = ConfigEntry<BufferSize> *;
  using reference = ConfigEntry<BufferSize> &;

  using self = ConfigEntryIterator<BufferSize>;
  bool operator==(const self &other)
  {
    if (other.eof() == value_.eof())
    {
      return true;
    }
    return value_.key() == other.value_.key() &&
           value_.value() == other.value_.value();
  }

  bool operator!=(const self &other)
  {
    return !this->operator==(other);
  }

  reference operator*()
  {
    return value_;
  }

  pointer operator->()
  {
    return &value_;
  }

private:
  value_type value_;
};

} // namespace yasconf
