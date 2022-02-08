CPP       = c++
LINK      = c++

OBJSERVER = RPCTesterServer.o \
			\
			../lib/RpcThread.o \
			../lib/RpcRunnerThread.o \
			../lib/RpcRunnerThreads.o

OBJCLIENT = RPCTesterClient.o

LIBS      = -L/usr/local/lib \
			-lds \
			-lfcgi \
			-lpq \
			`wx-config --libs`

INCS      = -I"/usr/local/include"

BINSERVER = RPCTesterServer
BINCLIENT = RPCTesterClient

CXXFLAGS  = $(INCS) `wx-config --cppflags`

all: $(BINSERVER) $(BINCLIENT)

clean:
	rm -f $(OBJSERVER) $(BINSERVER) $(OBJCLIENT) $(BINCLIENT)

$(BINSERVER): $(OBJSERVER)
	$(LINK) $(OBJSERVER) -o $(BINSERVER) $(LIBS)

$(BINCLIENT): $(OBJCLIENT)
	$(LINK) $(OBJCLIENT) -o $(BINCLIENT) $(LIBS)

.cpp.o:
	$(CPP) $(CXXFLAGS) -g -c $*.cpp -o $*.o
