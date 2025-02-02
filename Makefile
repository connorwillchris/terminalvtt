all: server
	gcc -o build/client \
		client/client.c \
		common/lib.c \
		common/ansi.c \
		-llua -lenet
	gcc -o build/server \
		server/server.c \
		common/lib.c \
		common/ansi.c \
		-llua -lenet
