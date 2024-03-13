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

#include <cstddef>
#include <string_view>

#include <fcntl.h>
#include <unistd.h>

#include "yasconf/config_entry.hpp"

namespace yasconf
{

template <std::size_t BufferSize>
class Config
{
public:
  Config(const std::string_view &path)
  {
    id_ = open(path.data(), O_RDONLY);
  }

  bool is_open() const
  {
    return id_ != -1;
  }

  void close()
  {
    if (id_ != -1)
    {
      close(id_);
      id_ = -1;
    }
  }

  ConfigEntry<BufferSize> first() const
  {
    lseek(id_, 0, SEEK_SET);
    ConfigEntry<BufferSize> n(id_);
    n.next();
    return n;
  }

  ConfigEntry<BufferSize> operator[](const std::string_view &key)
  {
    for (ConfigEntry<BufferSize> n = first(); !n.eof(); n.next())
    {
      if (n.key() == key)
      {
        return n;
      }
    }
    return ConfigEntry<BufferSize>(true);
  }

private:
  int id_;
};

} // namespace yasconf
