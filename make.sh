#!/usr/bin/sh
if [ ! -d "build" ];
then
    echo "No build directory found, Making one..."
    mkdir -p build
fi

echo "Making the client..."
gcc -o build/client \
    client/client.c \
    common/lib.c \
    -lenet -lncurses -llua -lpthread

echo "Making the server..."
gcc -o build/server \
    server/server.c \
    common/lib.c \
    -lenet -lncurses -llua -lpthread
