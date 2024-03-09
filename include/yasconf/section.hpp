/**
 * section.hpp
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

#include "yasconf/file_reader.hpp"

namespace yasconf
{

class Section
{
public:
  constexpr static int max_section_name_length = 64;
  Section(FileReader &file, int start_line, int end_line);

  Section &operator=(const Section &);

  std::string_view operator[](const std::string_view &key);

  std::string_view name() const;

private:
  FileReader &file_;
  int start_line_;
  int end_line_;
  char name_[max_section_name_length];
};

} // namespace yasconf
