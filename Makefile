
MODE ?= release        

ifeq ($(MODE),release)
  CFLAGS := -O2 -Wall -Wextra
endif

ifeq ($(MODE),debug)
  CFLAGS := -O0 -g -Wall -Wextra
endif

ifeq ($(MODE),asan)
  CFLAGS := -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -Wall -Wextra
  LDFLAGS := -fsanitize=address,undefined
endif

CORE_INC := -Icore/headers
SOCK_INC := $(CORE_INC) -ISocketServer/headers
MHD_INC  := $(CORE_INC) -IMHDServer/headers


CORE_SRC := $(wildcard core/*.c)
CORE_OBJ := $(CORE_SRC:.c=.o)

SOCK_SRC := $(shell find SocketServer -name '*.c')
SOCK_OBJ := $(SOCK_SRC:.c=.o)

MHD_SRC  := $(shell find MHDServer/src -name '*.c')
MHD_OBJ  := $(MHD_SRC:.c=.o)


socket: server_socket
mhd   : server_mhd
all   : socket mhd

server_socket: $(CORE_OBJ) $(SOCK_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ -lpng -lm -lpthread $(LDFLAGS)

server_mhd: $(CORE_OBJ) $(MHD_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ -lpng -lm -lpthread -lmicrohttpd $(LDFLAGS)

core/%.o: core/%.c
	$(CC) $(CFLAGS) $(CORE_INC) -c $< -o $@

SocketServer/%.o: SocketServer/%.c
	$(CC) $(CFLAGS) $(SOCK_INC) -c $< -o $@

MHDServer/%.o: MHDServer/%.c
	$(CC) $(CFLAGS) $(MHD_INC) -c $< -o $@

run: socket
	./server_socket --port 7000

clean:
	rm -f $(CORE_OBJ) $(SOCK_OBJ) $(MHD_OBJ) server_socket server_mhd

.PHONY: all socket mhd run clean
