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

#include "cluon-complete.hpp"
#include "proxy-analog.hpp"

// #include <cmath>
#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
// #include <array>
// #include <sstream>
#include <string>
#include <ctime>
#include <chrono>

Analog::Analog(bool verbose, uint32_t id)
    : m_conversionConst(1)
    , m_debug(verbose)
    , m_bbbId(id)
    , m_senderStampOffsetAnalog(id*1000+200)
    , m_pins()

{
	Analog::setUp();
}

Analog::~Analog() 
{
}

void Analog::body(cluon::OD4Session &od4)
{
    std::vector<std::pair<uint16_t, float>> reading = getReadings();
    if(m_debug) {
      std::cout << "[PROXY_ANALOG_RAW] ";
      for (std::pair<uint16_t, float> const& pair : reading) {
        std::cout << "Pin " << pair.first << ": " << pair.second << " ";
      }
      std::cout << std::endl;
    }

    if(m_debug) {
          std::cout << "[PROXY_ANALOG-PARSED] ";
    }
    try {
      for (std::pair<uint16_t, float> const& pair : reading) {
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
        cluon::data::TimeStamp sampleTime = cluon::time::convert(tp);
        int16_t senderStamp = (int16_t) pair.first + m_senderStampOffsetAnalog;

        float value;

        if(pair.first == m_analogPinSteerPosition){
          value = pair.second/((float) m_analogConvSteerPosition)-((float) m_analogOffsetSteerPosition);
        }else if(pair.first == m_analogPinEbsLine){
          value = pair.second/((float) m_analogConvEbsLine)-((float) m_analogOffsetEbsLine);
        }else if(pair.first == m_analogPinServiceTank){
          value = pair.second/((float) m_analogConvServiceTank)-((float) m_analogOffsetServiceTank);
        }else if(pair.first == m_analogPinEbsActuator){
          value = pair.second/((float) m_analogConvEbsActuator)-((float) m_analogOffsetEbsActuator);
        }else if(pair.first == m_analogPinPressureReg){
          value = pair.second/((float) m_analogConvPressureReg)-((float) m_analogOffsetPressureReg);
        }else if(pair.first == m_analogPinSteerPositionRack){
          value = pair.second/((float) m_analogConvSteerPositionRack)-((float) m_analogOffsetSteerPositionRack);
        }

        if(pair.first == m_analogPinSteerPosition || pair.first == m_analogPinSteerPositionRack){
          opendlv::proxy::GroundSteeringReading msgSteer;
          msgSteer.groundSteering(value);
          od4.send(msgSteer, sampleTime, senderStamp);
        }else if(pair.first == m_analogPinEbsLine || pair.first == m_analogPinServiceTank || pair.first == m_analogPinEbsActuator || pair.first == m_analogPinPressureReg){
          opendlv::proxy::PressureReading msgPressure;
          msgPressure.pressure(value);
          od4.send(msgPressure, sampleTime, senderStamp);
        }else{
          opendlv::proxy::VoltageReading msg;
          msg.torque(pair.second);
          od4.send(msg, sampleTime, senderStamp);
        }

        if(m_debug) {
          std::cout << "Pin " << pair.first << ": " << value << " ";
        }

      }
    } catch (...) {
      std::cerr << "[PROXY_ANALOG_EXCEPT] Exception in parsing" << std::endl;
    }

    if(m_debug) {
      std::cout << std::endl;
    }
}



void Analog::setUp() 
{
  std::vector<std::string> pinsVecString = {"0", "1", "2", "3", "5", "6"};

  for(std::string const& str : pinsVecString) {
    m_pins.push_back(std::stoi(str));
  }
}

void Analog::tearDown() 
{
}

std::vector<std::pair<uint16_t, float>> Analog::getReadings() {
  try{
    std::vector<std::pair<uint16_t, float>> reading;
    for(uint16_t const pin : m_pins) {
      std::string filename = "/sys/bus/iio/devices/iio:device0/in_voltage" 
          + std::to_string(pin) + "_raw";
      std::ifstream file(filename, std::ifstream::in);
      std::string line;
      if(file.is_open()){
        std::getline(file, line);
        uint16_t rawReading = std::stoi(line);
        reading.push_back(std::make_pair(pin, rawReading*m_conversionConst));
      } else {
        std::cerr << "[PROXY_ANALOG] Could not read from analog input. (pin: " << pin 
            << ", filename: " << filename << ")" << std::endl;
        reading.push_back(std::make_pair(pin, 0));
      }
      file.close();
    }
  } catch (...) {
    std::cerr << "[PROXY_ANALOG_EXCEPT] Exception in getReadings" << std::endl;
  }
  return reading;
}
