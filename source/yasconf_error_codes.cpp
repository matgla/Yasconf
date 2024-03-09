/**
 * yasconf_error_codes.cpp
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

#include "yasconf/yasconf_error_codes.hpp"

namespace yasconf
{

[[nodiscard]] std::string_view YasconfErrorCategory::name() const noexcept
{
  return "yasconf";
}

[[nodiscard]] std::string_view YasconfErrorCategory::message(int ev) const
{
  switch (static_cast<YasconfErrors>(ev))
  {
  case YasconfErrors::BufferTooSmall:
    return "Buffer too small";
  case YasconfErrors::FileOperationFailed:
    return "File operation failed";
  case YasconfErrors::FileNotOpen:
    return "File not open";
  case YasconfErrors::EndOfFile:
    return "End of file";
  case YasconfErrors::SectionNotFound:
    return "Section not found";
  default:
    return "Unknown error";
  }
}

} // namespace yasconf

namespace eul::error
{
template <> error_code make_error_code<yasconf::YasconfErrors>(yasconf::YasconfErrors e)
{
  static const yasconf::YasconfErrorCategory yasconf_error_category{};
  return {static_cast<int>(e), yasconf_error_category};
}

} // namespace eul::error
