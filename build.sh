# build dir
if [ ! -d "test_data/" ] && ! mkdir "test_data/"; then
    exit 1
fi
if [ ! -d "test_data/input/" ] && ! mkdir "test_data/input/"; then
    exit 1
fi
if [ ! -d "test_data/output/" ] && ! mkdir "test_data/output/"; then
    exit 1
fi


# 編譯 shared library
docker run --rm -it -v .:/mnt/ alpine/dev/flask /bin/sh -c "cd /mnt/ && make build_all"


# run flask http server in docker container
# docker run --rm -it -p 5000:5000 -v .:/mnt  alpine/dev/flask /bin/sh -c "cd /mnt/ && python3 app.py & ngrok http http://localhost:5000"