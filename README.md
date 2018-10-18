# Analog

Collect the data from the Beaglebone then boardcast them.

|   Data collect    |  PIN  | conversion | Offset |
|-------------------|-------|------------|--------|
| SteerPosition     |   0   |    80.38   | 27.74  |
| EbsLine           |   1   |    378.5   | 0.11   |
| ServiceTank       |   2   |    377.6   | 0.11   |
| EbsActuator       |   3   |    377.9   | 0.11   |
| PressureReg       |   5   |    378.7   | 0      |
| SteerPositionRack |   6   |    80.86   | 28.06  |



The media of connection with hardware interface is the file :

```cpp
/sys/bus/iio/devices/iio:device0/in_voltage" + std::to_string(pin) + "_raw"
```

## usage

```cpp
    cluon::OD4Session od4Dan{static_cast<uint16_t>(std::stoi(commandlineArguments["cidDan"]))};
    
    od4Dan.dataTrigger(opendlv::proxy::GroundSteeringReading::ID(),rackEnvelope);
```

## OpenDLV Microservice for Beaglebone

This repository provides source code for beaglebones for the OpenDLV.io software ecosystem.

[![Build Status](https://travis-ci.org/chalmers-revere/opendlv.io.svg?branch=master)](https://travis-ci.org/se-research/opendlv.sensors.oxts) [![License: GPLv3](https://img.shields.io/badge/license-GPL--3-blue.svg
)](https://www.gnu.org/licenses/gpl-3.0.txt)


## Table of Contents
* [Dependencies](#dependencies)
* [Usage](#usage)
* [Build from sources on the example of Ubuntu 16.04 LTS](#build-from-sources-on-the-example-of-ubuntu-1604-lts)
* [License](#license)


## Dependencies
No dependencies! You just need a C++14-compliant compiler to compile this project as it ships the following dependencies as part of the source distribution:

* [libcluon](https://github.com/chrberger/libcluon) - [![License: GPLv3](https://img.shields.io/badge/license-GPL--3-blue.svg
)](https://www.gnu.org/licenses/gpl-3.0.txt)
* [Unit Test Framework Catch2](https://github.com/catchorg/Catch2/releases/tag/v2.1.1) - [![License: Boost Software License v1.0](https://img.shields.io/badge/License-Boost%20v1-blue.svg)](http://www.boost.org/LICENSE_1_0.txt)


## Usage
This microservice is created automatically on changes to this repository via Docker's public registry for:
* [x86_64](https://hub.docker.com/r/chalmersrevere/opendlv.io-amd64/tags/)
* [armhf](https://hub.docker.com/r/chalmersrevere/opendlv.io-armhf/tags/)
* [aarch64](https://hub.docker.com/r/chalmersrevere/opendlv.io-aarch64/tags/)

To run this microservice using our pre-built Docker multi-arch images to connect to an OXTS GPS/INSS unit broadcasting data to `195.0.0.33:3000` and to publish the messages according to OpenDLV Standard Message Set into session 111 in Google Protobuf format, simply start it as follows:
```
docker run --rm --net=host chalmersrevere/opendlv.io-multi:proxy-beaglebone-v0.0.1 beaglebone --cid=111 --verbose
```

## Build from sources on the example of Ubuntu 16.04 LTS
To build this software, you need cmake, C++14 or newer, and make. Having these
preconditions, just run `cmake` and `make` as follows:

```
mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=Release ..
make && make test && make install
```

## Build it with docker
Make sure you have the latest docker version. 1.17

AMD64:
Run
```
docker build -t chalmersrevere/opendlv.io-multi:proxy-beaglebone-v0.0.1 -f Dockerfile.amd64 .
```

ARMHF:
Run
```
docker build -t chalmersrevere/opendlv.io-multi-armhf:proxy-beaglebone-v0.0.1 -f Dockerfile.armhf .
```

## Execute with Docker-compose
Make sure you have the latest docker-compose verison.

AMD64:
Run
```
cd usecase
docker-compose up
```

ARMHF:
Run
```
cd usecase
docker-compose -f beaglebone.yml up
```


## License

* This project is released under the terms of the GNU GPLv3 License

