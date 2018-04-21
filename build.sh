
docker build -t proxy-lynx-analog-v0.0.1 -f Dockerfile.armhf .
docker save  proxy-lynx-analog-v0.0.1 > proxy-lynx-analog-v0.0.1.tar
