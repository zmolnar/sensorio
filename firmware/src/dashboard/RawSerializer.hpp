//
//  This file is part of Sensorio.
//
//  Sensorio is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Sensorio is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Sensorio.  If not, see <https://www.gnu.org/licenses/>.
//

#ifndef RAWSERIALIZER_HPP
#define RAWSERIALIZER_HPP

#include <dashboard/Config.hpp>

namespace Config {
  class RawSerializer : public ASerializer {
    static const char *tag;
  public:
    bool serialize(etl::vector_ext<uint8_t> &blob) override final;
    bool deserialize(const etl::vector_ext<uint8_t> &blob) override final;
  };
};

#endif
