SRCS:=$(wildcard *.c)
OBJECTS:=$(patsubst %.c, %.o, $(SRCS))
CC:=gcc
LIBS:=-lcrypt
SERVER:=client

$(SERVER):$(OBJECTS)
	$(CC) $^ -o $@ $(LIBS) -g

%.o:%.c
	$(CC) -c $^ -o $@ $(LIBS) -g 
print:
	echo $(OBJECTS)
clean:
	rm -rf $(OBJECTS) $(SERVER)

