CC=g++

#CFLAGS= -Wall -g3 -fprofile-arcs -ftest-coverage -pg
CFLAGS= -Wall -g3 
#CFLAGS= -Wall -O2 
LINK_FLAGS= -lpthread -ldl -lOLFO 
INCLUDE= -I$(ORACLE_HOME)/rdbms/public -I$(ORACLE_HOME)/rdbms/demo -I/opt/work/src/lib
LIB_PATH= -L./ -L/opt/work/bin/lib
ORAOPTION=-L$(ORACLE_HOME)/lib -lclntsh \
        `cat $(ORACLE_HOME)/lib/ldflags`   \
        `cat $(ORACLE_HOME)/lib/sysliblist`
COMMON_OBJ=OraTest.o SimpleServer.o TerminalService.o CommonDao.o AdStatsBody.o \
	AdStatsDao.o BlackListDao.o Exception.o \
	HiringDao.o TCPClient.o TerminalParameterDao.o WhiteListDao.o WtdpPacket.o 

all: happytest
	happytest philip philip carsmart
happytest: ${COMMON_OBJ}
	${CC} ${CFLAGS} ${LIB_PATH} ${ORAOPTION} ${LINK_FLAGS} -o  $@  $^
OraTest.o: OraTest.cpp 
	${CC} ${CFLAGS} ${INCLUDE} -c $<
SimpleServer.o: SimpleServer.cpp SimpleServer.h
	${CC} ${CFLAGS} ${INCLUDE} -c $<
TerminalService.o: TerminalService.cpp TerminalService.h
	${CC} ${CFLAGS} ${INCLUDE} -c $<
AdStatsBody.o: AdStatsBody.cpp AdStatsBody.h
	${CC} ${CFLAGS} ${INCLUDE} -c $<
AdStatsDao.o: AdStatsDao.cpp AdStatsDao.h
	${CC} ${CFLAGS} ${INCLUDE} -c $<
BlackListDao.o: BlackListDao.cpp BlackListDao.h
	${CC} ${CFLAGS} ${INCLUDE} -c $<
CommonDao.o: CommonDao.cpp CommonDao.h
	${CC} ${CFLAGS} ${INCLUDE} -c $<
Exception.o: Exception.cpp Exception.h
	${CC} ${CFLAGS} ${INCLUDE} -c $<
HiringDao.o: HiringDao.cpp HiringDao.h
	${CC} ${CFLAGS} ${INCLUDE} -c $<
TCPClient.o: TCPClient.cpp TCPClient.h
	${CC} ${CFLAGS} ${INCLUDE} -c $<
TerminalParameterDao.o: TerminalParameterDao.cpp TerminalParameterDao.h
	${CC} ${CFLAGS} ${INCLUDE} -c $<
WhiteListDao.o: WhiteListDao.cpp WhiteListDao.h
	${CC} ${CFLAGS} ${INCLUDE} -c $<
WtdpPacket.o: WtdpPacket.cpp WtdpPacket.h
	${CC} ${CFLAGS} ${INCLUDE} -c $<

clean:
	rm *.o
