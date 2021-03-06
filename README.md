# mcidle-cpp
[![Build Status](https://travis-ci.com/qubard/mcidle-cpp.svg?branch=master)](https://travis-ci.com/qubard/mcidle-cpp)

This is a port of `mcidle-python` to C++.

# Running using docker

Make sure to set the environment variables `MC_USER` and `MC_PASS` for your username and password. `MC_IP`
is the IP address of the server you're trying to connect to. `MC_PORT` is optional if the server's default port is not
25565.

```
# Build the image and tag it
docker build -t mcidle-docker -f Dockerfile . 
# Run the image and map port 1337 outside the container to 1337 inside the container
docker run -it -p 1337:1337 \ 
    -e MC_USER=username \
    -e MC_PASS=pass \
    -e MC_IP=ip \
    -e MC_PORT=25565 \ 
    mcidle-docker
```
