/**
 * yasconf_error_category.hpp
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

#include <eul/error/error_category.hpp>
#include <eul/error/error_code.hpp>

namespace yasconf
{

enum class YasconfErrors
{
  BufferTooSmall = 1,
  FileOperationFailed,
  FileNotOpen,
  EndOfFile,
  SectionNotFound
};

class YasconfErrorCategory : public eul::error::error_category
{
public:
  [[nodiscard]] std::string_view name() const noexcept override;
  [[nodiscard]] std::string_view message(int ev) const override;
};

} // namespace yasconf

namespace eul::error
{

template <> error_code make_error_code<yasconf::YasconfErrors>(yasconf::YasconfErrors e);

} // namespace eul::error
