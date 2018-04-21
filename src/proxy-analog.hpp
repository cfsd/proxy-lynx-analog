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

    const uint16_t m_analogPinSteerPosition = 0;
    const uint16_t m_analogPinEbsLine = 1;
    const uint16_t m_analogPinServiceTank = 2;
    const uint16_t m_analogPinEbsActuator = 3;
    const uint16_t m_analogPinPressureReg = 5;
    const uint16_t m_analogPinSteerPositionRack = 6;


    const double m_analogConvSteerPosition = 80.38;
    const double m_analogConvEbsLine = 1;
    const double m_analogConvServiceTank = 1;
    const double m_analogConvEbsActuator = 1;
    const double m_analogConvPressureReg = 1;
    const double m_analogConvSteerPositionRack = 80.86;

    const double m_analogOffsetSteerPosition = 27.74;
    const double m_analogOffsetEbsLine = 0;
    const double m_analogOffsetServiceTank = 0;
    const double m_analogOffsetEbsActuator = 0;
    const double m_analogOffsetPressureReg = 0;
    const double m_analogOffsetSteerPositionRack = 28.06;



};

#endif

