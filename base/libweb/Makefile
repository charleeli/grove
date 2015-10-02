CXX ?= g++
AR = ar
CFLAGS = -g -Wall
ALLINC = -I.
FASTCGINC = 

TARGETS=libweb.a

webobj=webparam.o webapp.o webpage.o webgeneral.o

all:$(TARGETS)
	
$(webobj):%.o:%.cpp
	$(CXX) $(CFLAGS) -D WEBLIB_WITH_FASTCGI $(ALLINC) $(FASTCGINC) -c $< -o$@
	
libweb.a:$(webobj)
	ar r $@ $^
	ranlib $@

clean mostlyclean:
	-rm -f *.o *.tmp* *.so *.so.* $(TARGETS) *~ *.o *.a

./PHONY:clean
