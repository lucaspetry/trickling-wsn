CC = g++
CFLAGS = -std=c++11 -Wno-deprecated-register -std=c++11 -g
LINEAR = linear
ELMAN = elman

SRC_FILES += $(wildcard *.hpp)

SRC_LINEAR += main_linear.cpp
SRC_LINEAR += ${SRC_FILES}

SRC_ELMAN += main_elman.cpp
SRC_ELMAN += ${SRC_FILES}

all:
	${CC} ${CFLAGS} -o ${LINEAR} ${SRC_LINEAR}
	${CC} ${CFLAGS} -o ${ELMAN} ${SRC_ELMAN}

linear: FORCE
	${CC} ${CFLAGS} -o ${LINEAR} ${SRC_LINEAR}

elman: FORCE
	${CC} ${CFLAGS} -o ${ELMAN} ${SRC_ELMAN}

clean:
	-@rm ${LINEAR}
	-@rm ${ELMAN}

FORCE:
	