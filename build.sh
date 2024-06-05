
# 編譯
docker run --rm -it -v .:/mnt/ ortools/alpine/dev /bin/sh -c "cd /mnt/ && make build_all"

# 清理
# docker run --rm -it -v .:/mnt/ ortools/alpine/dev /bin/sh -c "rm -rf /mnt/bin /mnt/lib"

# 執行
# docker run --rm -it -v .:/mnt/ ortools/alpine/runtime /bin/sh -c "cp /mnt/lib/* /usr/local/lib  && cd /mnt/bin/ && ./test"