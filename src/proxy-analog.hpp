/*
 * Copyright (C) 2018  Christian Berger
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ANALOG
#define ANALOG

#include "opendlv-standard-message-set.hpp"

#include <memory>
#include <string>
#include <vector>
#include <utility>

class Analog {
   private:
    //Analog(const Analog &) = delete;
    //Analog(Analog &&)      = delete;
    //Analog &operator=(const Analog &) = delete;
    //Analog &operator=(Analog &&) = delete;

   public:
    Analog(bool verbose, uint32_t id);
    ~Analog();

   public:
    float decode(const std::string &data) noexcept;
    void body(cluon::OD4Session &od4);

   private:
    void setUp();
    void tearDown();
   
    std::vector<std::pair<uint16_t, float>> getReadings();
    
    float m_conversionConst;
    bool m_debug;
    uint32_t m_bbbId;
    uint32_t m_senderStampOffsetAnalog;
    std::vector<uint16_t> m_pins;



};

#endif

