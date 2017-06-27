# compiler
CC = g++

# compiler flags
CFLAG = -g -Wall

# define libraries to link into executable
LIB = -lglut -lGL -lGLEW

# define library paths
#LFLAG = -L../lib

# directories containing headers other than /usr/include
#INCLUDE = -I../include

# search for prereqs in these directories
VPATH = src

# define the source files
SRC = main.cpp \
	render.cpp

# the build target executable
TARGET = out

.PHONY: depend clean

$(TARGET): $(SRC)
	$(CC) -o $@ $^ $(CFLAG) $(LIB)

clean:
	$(RM) *.o $(TARGET)

