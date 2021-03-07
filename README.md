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
chmod +x run.sh && ./run.sh
```

then you can connect on `localhost:1337` to connect to mcidle.

To kill the process, run `docker ps`, find the container ID and then run `docker kill <containerID>`.
