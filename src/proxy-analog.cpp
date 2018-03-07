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

float Analog::decode(const std::string &data) noexcept {
    std::cout << "Got data:" << data << std::endl;
    float temp = std::stof(data);
    return temp;
}

Analog::Analog()
    : m_conversionConst(1)
    , m_debug(1)
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
    for (std::pair<uint16_t, float> const& pair : reading) {
      std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
      cluon::data::TimeStamp sampleTime = cluon::time::convert(tp);

      int16_t senderStamp = (int16_t) pair.first;
      opendlv::proxy::VoltageReading msg;
      msg.torque(pair.second);
      od4.send(msg, sampleTime, senderStamp);
    }
    if(m_debug) {
      std::cout << "[PROXY_ANALOG] ";
      for (std::pair<uint16_t, float> const& pair : reading) {
        std::cout << "Pin " << pair.first << ": " << pair.second << " ";
      }
      std::cout << std::endl;
    }
}



void Analog::setUp() 
{
  std::vector<std::string> pinsVecString = {"0", "1", "2", "3", "4", "5", "6"};

  for(std::string const& str : pinsVecString) {
    m_pins.push_back(std::stoi(str));
  }
}

void Analog::tearDown() 
{
}

std::vector<std::pair<uint16_t, float>> Analog::getReadings() {
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
  return reading;
}
