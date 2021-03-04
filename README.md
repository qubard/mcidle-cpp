# mcidle-cpp

This is a port of `mcidle-python` to C++.

# Running using docker

Make sure to set the environment variables `MC_USER` and `MC_PASS` for your username and password. `MC_IP`
is the IP address of the server you're trying to connect to. `MC_PORT` is optional if the server's default port is not
25565.

```
# Build the image and tag it
docker build -t mcidle-docker -f Dockerfile . 
# Run the image
docker run -it -e MC_PORT -e MC_PASS -e MC_IP -e MC_PORT mcidle-docker
```

