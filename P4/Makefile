# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -g 

# Target executable
TARGET = compile

# For deleting the target
TARGET_DEL = compile.exe

# Automatically find all .cpp files and corresponding .o files
# Source files
SRCS = $(wildcard *.cpp)
# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule to build the executable
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
	/bin/rm -f $(TARGET) $(OBJS) *.o *.asm


