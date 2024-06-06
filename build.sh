
# 清理
# docker run --rm -it -v .:/mnt/ ortools/alpine/dev /bin/sh -c "rm -rf /mnt/bin /mnt/lib"


# 編譯 shared library
docker run --rm -it -v .:/mnt/ ortools/alpine/dev /bin/sh -c "cd /mnt/ && make build_all"

# python 造假資料
mkdir test_data/input/ test_data/output/
python3 make_test_data.py

# 複製 shared library 到容器中 執行測試
docker run --rm -it -v .:/mnt/ ortools/alpine/dev /bin/sh -c "cp /mnt/lib/* /usr/local/lib && cd /mnt && ./bin/test"