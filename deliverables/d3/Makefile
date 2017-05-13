CC = g++
CFLAGS = -std=c++11 -fcxx-exceptions -Wno-deprecated-register -Wall
EXECUTABLE = test

SRC_FILES += $(wildcard *.cpp)
SRC_FILES += $(wildcard *.hpp)

all:
	${CC} -o ${EXECUTABLE} ${SRC_FILES}

clean:
	-@rm ${EXECUTABLE}
