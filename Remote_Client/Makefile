# _*_ Makefile _*_

CC =gcc

OBJ = client.o

all: client

client: $(OBJ)
	$(CC)  -o client $(OBJ) -lpthread -g -lrt 

client.o: client.c
	$(CC) -c client.c $(CFLAGS)

clean:
	rm *.o client
