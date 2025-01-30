# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -g 

# Target executable
TARGET = P0

# For deleting the target
TARGET_DEL = P0.exe

# Source files
SRCS = P0.cpp Binary_Search_Tree.h Binary_Tree.h BTNode.h P0.h

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule to build and run the executable
all: $(TARGET) make

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to run the excutable
make: $(TARGET)
#	./$(TARGET)

# Clean rule to remove generated files
.PHONY: clean
clean:
	/bin/rm -f $(TARGET_DEL) *.inorder *.preorder *.postorder *.o


