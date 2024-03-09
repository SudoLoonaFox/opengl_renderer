# I apologize how hard this might be to read
CC=gcc
CFLAGS=-g -Wall -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW
SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
BIN=game

all:$(BIN) # all requires game

$(BIN): $(OBJS) # link all object files into single binary
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.c # create object files
	$(CC) $(CFLAGS) -c $< -o $@

# I do not use rm if I can help it. trash-put is far safer
clean:
	trash-put $(BIN) $(OBJ)/* 
