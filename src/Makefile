
CC=gcc
AR=ar
SRCS=sothoth.c cJSON.c
OBJS=$(SRCS:.c=.o)
EXEC=../release/libsothoth.a

build: $(OBJS)
	$(AR) -crv $(EXEC) $(OBJS)

.c.o:
	$(CC) -fPIC -Wall -I../include -o $@ -c $<

clean:
	rm -rf $(EXEC) $(OBJS)