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
MC_ONLINE=true
```

# Running with Docker

Download [Docker Desktop](https://www.docker.com/products/docker-desktop), launch it and then run the following in your terminal of choice in the project directory

```
chmod +x run-and-build.sh && ./run-and-build.sh
```

then you can connect on `localhost:1337` to connect to mcidle.

To kill the process, run `docker ps`, find the container ID and then run `docker kill <containerID>`.

# Resolving IP addresses

Sometimes the domain you use to connect to a Minecraft server is hides the actual IP address behind an SRV record that needs to get resolved, so run `python get-ip.py domain.com` where `domain.com` is the domain you want to find the IP for to resolve its ip address.
