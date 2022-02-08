CC        = cc
CPP       = c++
LINK      = c++

WEBROOT   = /server/web/gug-it/www/modules/contact

OBJ       = DynSoftContact.o \
			\
			../lib/ContactEntity.o \
			../lib/ContactController.o \
			../lib/BugEntity.o \
			../lib/BugController.o

LIBS      = -L"/usr/local/pgsql/lib" \
			-L"/usr/local/lib" \
			-lpq \
			-lds \
			-lfcgi \
			`wx-config --libs`

INCS      = -I"/usr/include" \
			-I"/usr/local/include" \
			-I"/usr/local/pgsql/include" \
			-I"../lib"

BIN       = DynSoftContact.ds

CXXFLAGS  = $(INCS) \
			-O3 \
			`wx-config --cppflags`

all: $(BIN)

clean:
	rm -f $(OBJ) $(BIN)

install: all
	service apache24 onestop
	cp -f $(BIN) $(WEBROOT)/index.ds
	chown -R www:www $(WEBROOT)
	service apache24 onestart

$(BIN): $(OBJ)
	$(LINK) $(OBJ) -o $(BIN) $(LIBS)

.cpp.o:
	$(CPP) $(CXXFLAGS) -g -c $*.cpp -o $*.o
