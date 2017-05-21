CC = g++
CFLAGS = -std=c++11 -fcxx-exceptions -Wno-deprecated-register -g -Wall
LINEAR = linear
ELMAN = elman

SRC_FILES += $(wildcard *.hpp)

SRC_LINEAR += main_linear.cpp
SRC_LINEAR += ${SRC_FILES}

SRC_ELMAN += main_elman.cpp
SRC_ELMAN += ${SRC_FILES}

all:
	${CC} -o ${LINEAR} ${SRC_LINEAR}
	${CC} -o ${ELMAN} ${SRC_ELMAN}

linear:
	${CC} -o ${LINEAR} ${SRC_LINEAR}

elman:
	${CC} -o ${ELMAN} ${SRC_ELMAN}

clean:
	-@rm ${LINEAR}
	-@rm ${ELMAN}
