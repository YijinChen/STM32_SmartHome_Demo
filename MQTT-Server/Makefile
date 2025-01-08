Target=main
Object=main.o epoll.o mqtt.o link.o database.o

CFLAGS =-lsqlite3 -ljson-c

$(Target):$(Object)
	gcc $(Object) -o $(Target) -Wall $(CFLAGS)


clean:
	rm *.o $(Target)