# mcidle-cpp

This is a port of `mcidle-python` to C++.

# Running using docker

Make sure to set the environment variables `MCIDLE_USER` and `MCIDLE_PASS`.

```
# Build the image and tag it
docker build -t mcidle-docker -f Dockerfile . 
# Run the image
docker run -it -e MCIDLE_USER -e MCIDLE_PASS mcidle-docker
```

