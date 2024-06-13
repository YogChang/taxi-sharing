## Description
taxi-sharing.

## Deploy

### 1. Build docker image.

```shell=
docker build . -t="alpine/dev/flask"
```


### 2. Build Algorithm and Test
```shell=
bash build.sh
```

### 3. Run Flask http Server in Container
```shell=
docker run --rm -it -p 5000:5000 -v .:/mnt  alpine/dev/flask /bin/sh -c "cd /mnt/ && python3 app.py"
```