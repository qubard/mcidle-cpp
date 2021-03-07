# mcidle-cpp
[![Build Status](https://travis-ci.com/qubard/mcidle-cpp.svg?branch=master)](https://travis-ci.com/qubard/mcidle-cpp)

This is a port of `mcidle-python` to C++.

# Configuration

Before running, edit the `conf` file with your credentials.

```
MC_USER=test
MC_PASS=testpw
MC_IP=connect.2b2t.org
MC_PORT=25565
```

# Running with Docker

Download [Docker Desktop](https://www.docker.com/products/docker-desktop), launch it and then run the following in your terminal of choice in the project directory

```
# Build the image and tag it
docker build -t mcidle-docker -f Dockerfile . 
docker run -it --env-file conf -p 1337:1337 mcidle-docker
```

then you can connect on `localhost:1337` to connect to mcidle.
