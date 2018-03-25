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
#include "opendlv-standard-message-set.hpp"

#include "proxy-analog.hpp"

#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <cmath>
#include <ctime>
#include <chrono>

void callOnReceive(cluon::data::Envelope data){
    if (data.dataType() == static_cast<int32_t>(opendlv::proxy::VoltageReading::ID())) {
       // opendlv::proxy::VoltageReading t = cluon::extractMessage<opendlv::proxy::VoltageReading>(std::move(data));
       // std::cout << "Recieved Msg Pin " << data.senderStamp() << ": " << t.torque() << " " << std::endl;
    }
}

int32_t main(int32_t argc, char **argv) {
    int32_t retCode{0};
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if ( (0 == commandlineArguments.count("freq")) || (0 == commandlineArguments.count("cid")) ) {
        std::cerr << argv[0] << " testing unit and publishes it to a running OpenDaVINCI session using the OpenDLV Standard Message Set." << std::endl;
        std::cerr << "Usage:   " << argv[0] << " --cid=<OpenDaVINCI session> [--id=<Identifier in case of multiple beaglebone units>] [--verbose]" << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=111 --id=1 --verbose=1 --freq=30" << std::endl;
        retCode = 1;
    } else {
        const uint32_t ID{(commandlineArguments["id"].size() != 0) ? static_cast<uint32_t>(std::stoi(commandlineArguments["id"])) : 0};
        const bool VERBOSE{commandlineArguments.count("verbose") != 0};
        const double FREQ{static_cast<double>(std::stof(commandlineArguments["freq"]))};
        std::cout << "Micro-Service ID:" << ID << std::endl;

        // Interface to a running OpenDaVINCI session.
        Analog analog(VERBOSE, ID);

        cluon::data::Envelope data;
        cluon::OD4Session od4{static_cast<uint16_t>(std::stoi(commandlineArguments["cid"])),
            [&data](cluon::data::Envelope &&envelope){
                callOnReceive(envelope);
                // IMPORTANT INTRODUCE A MUTEX
                data = envelope;
            }
        };
    
        using namespace std::literals::chrono_literals;
        std::chrono::system_clock::time_point threadTime = std::chrono::system_clock::now();
        while (od4.isRunning()) {
            std::this_thread::sleep_until(std::chrono::duration<double>(1/FREQ)+threadTime);
            threadTime = std::chrono::system_clock::now();

            analog.body(od4);
        }
    }
    return retCode;
}

