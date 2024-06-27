## Description
taxi-sharing.

## Deploy

### 1. Build docker image.

```shell=
docker build . -t="alpine/dev/flask"
```

### 2. Build Algorithm
```shell=
bash build.sh
```

### 3. Make mock data
```shell=
docker run --rm -it -v .:/mnt  alpine/dev/flask /bin/sh -c "cd /mnt/ && python3 make_test_data.py"
```

### 3. Run Flask http Server and DB in Container
```shell=
docker compose up --build
```