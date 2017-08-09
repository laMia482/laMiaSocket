EXE = socket
EXE_SRC = socket.cc

C_COMPILER = gcc
CC_COMPILER = g++

INCLUDE_PATH = 	-I./

CFLAGS = -g -std=c++11 -DLinux

LD_LIBRARY_PATH = -L./

LD_FLAGS = 	

OBJ = laMiaSocket.o
OBJS_DIR = ./
OBJS = $(addprefix $(OBJS_DIR), $(OBJ))



all: $(EXE)
	@echo '---------------- DONE FOR ALL ---------------'

$(EXE): $(OBJS)
	$(CC_COMPILER) $(EXE_SRC) -o $(EXE) $(INCLUDE_PATH) $(LD_LIBRARY_PATH) $(CFLAGS) $(LD_FLAGS) $(OBJS)
	rm -f $(OBJS)

$(OBJS_DIR)%.o: %.cc
	$(CC_COMPILER) $(INCLUDE_PATH) $(LD_LIBRARY_PATH) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f $(OBJS) $(EXE)
