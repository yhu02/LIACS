RMDIR   := rm -rf
CC      := gcc													#change depending on compiler
MKDIR   := mkdir
OBJ     := ./obj
INCLUDE := ../include
EXT     := .c													#change depending on file extension
SRC     := .
SRCS    := $(wildcard $(SRC)/*$(EXT))
OBJS    := $(patsubst $(SRC)/%$(EXT),$(OBJ)/%.o,$(SRCS))
EXE     := ./main
CFLAGS  :=  -I$(INCLUDE) -g 
LDLIBS  := -lm

.PHONY: all run clean

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(OBJ)/%.o: $(SRC)/%$(EXT) | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ):
	$(MKDIR) $@

clean:
	$(RMDIR) $(OBJ) $(EXE)