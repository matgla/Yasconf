/**
 * file_reader.cpp
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

#include "yasconf/file_reader.hpp"

#include <cerrno>
#include <cstdio>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>

#include "yasconf/yasconf_error_codes.hpp"

namespace yasconf
{

FileReader::FileReader(const std::string_view &file, std::span<char> buffer) noexcept
    : fd_(-1), position_(0), buffer_(buffer)
{
  fd_ = open(file.data(), O_RDONLY);
}

FileReader::~FileReader() noexcept
{
  if (fd_ >= 0)
  {
    close(fd_);
  }
}

bool FileReader::is_open() const
{
  return fd_ >= 0;
}

std::string_view FileReader::next_line(eul::error::error_code &ec)
{
  if (buffer_.size() < 2)
  {
    ec = YasconfErrors::BufferTooSmall;
    return {};
  }
  if (fd_ < 0)
  {
    ec = YasconfErrors::FileNotOpen;
    return {};
  }
  char byte = '\0';
  std::size_t buffer_position = 0;
  while (byte != '\n')
  {
    ssize_t readed = read(fd_, &byte, 1);
    if (readed < 0)
    {
      ec = YasconfErrors::FileOperationFailed;
      return {};
    }

    else if (readed == 0)
    {
      ec = YasconfErrors::EndOfFile;
      return {};
    }

    if (buffer_position < buffer_.size() - 1)
    {
      buffer_[buffer_position] = byte;
      ++buffer_position;
    }
    else
    {
      ec = YasconfErrors::BufferTooSmall;
      break;
    }
    ++position_;

    if (byte == '\n')
    {
      break;
    }
  }

  buffer_[buffer_position] = '\0';
  std::string_view str = {buffer_.data(), static_cast<size_t>(buffer_position)};
  const char *whitespaces = " \t\n\r\f\v\0";
  std::size_t last = str.find_last_not_of(whitespaces) + 1;
  std::size_t first = str.find_first_not_of(whitespaces);
  if (first == std::string_view::npos)
  {
    return {};
  }
  buffer_[last] = '\0';
  return str.substr(first, last);
}

void FileReader::set_line(int line, eul::error::error_code &ec)
{
  position_ = 0;
  if (lseek(fd_, 0, SEEK_SET) < 0)
  {
    ec = YasconfErrors::FileOperationFailed;
  }

  for (int i = 0; i < line; ++i)
  {
    next_line(ec);
    if (ec)
    {
      return;
    }
  }
}

} // namespace yasconf
