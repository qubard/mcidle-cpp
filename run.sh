docker build -t mcidle-docker -f Dockerfile . 
docker run -it --env-file conf -p 1337:1337 mcidle-docker
