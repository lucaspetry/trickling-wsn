CC = g++
CFLAGS = -std=c++11 -Wno-deprecated-register -std=c++11 -g
LINEAR = linear
MLP = mlp

SRC_FILES += $(wildcard *.hpp)

SRC_LINEAR += main_linear.cpp
SRC_LINEAR += ${SRC_FILES}

SRC_MLP += main_mlp.cpp
SRC_MLP += ${SRC_FILES}

all:
	${CC} ${CFLAGS} -o ${LINEAR} ${SRC_LINEAR}
	${CC} ${CFLAGS} -o ${MLP} ${SRC_MLP}

linear: FORCE
	${CC} ${CFLAGS} -o ${LINEAR} ${SRC_LINEAR}

mlp: FORCE
	${CC} ${CFLAGS} -o ${MLP} ${SRC_MLP}

clean:
	-@rm ${LINEAR}
	-@rm ${MLP}

FORCE:
	