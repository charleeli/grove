BASEPATH    := $(ROOTPATH)/base
SERVER_NAME  = ${TARGET}
LIB       	+= 
#-------------------------------------------------------------------------------
CC           = gcc
CXX          = g++
CNOWARN    	:= -Wno-write-strings \
               -Wno-char-subscripts \
               -Wno-deprecated \
               -Wno-unused-variable \
               -Wall
CFLAGS     	+= -g \
               -fPIC \
               -std=gnu++11 $(CNOWARN) \
               -pipe \
               -fno-ident \
               -fno-strict-aliasing \
               -MMD \
               -D_GNU_SOURCE \
               -D_REENTRANT \
               -DSERVER_NAME=\"$(SERVER_NAME)\" 
LIB_32		:= 
LIB_64  	:= 
#-------------------------------------------------------------------------------
INCLUDE     += 	-I. \
                -I${BASEPATH} \
				-I${BASEPATH}/codec \
				-I${BASEPATH}/libstore \
				-I${BASEPATH}/pattern \
				-I${BASEPATH}/pbjson \
				-I${BASEPATH}/rapidjson\
				-I${ROOTPATH}/fast-cgi/dao\
				-I${ROOTPATH}/fast-cgi/proto

LIB  		:= 	-lprotobuf \
				-lpthread \
				-lfcgi \
				-lhiredis\
				$(LIB)
#-------------------------------------------------------------------------------
PLATFORM := $(strip $(shell echo `uname -m`))
ifneq ($(MFLAGS),64)
    ifneq ($(MFLAGS),32)
        ifeq ($(PLATFORM),x86_64)
	        MFLAGS := 64
        else
            MFLAGS := 32
        endif
    endif
endif

ifeq ($(MFLAGS),64)
    ifneq ($(PLATFORM),x86_64) 
        MFLAGS := 32
    endif
endif

ifeq ($(MFLAGS),64)
	LIB += ${LIB_64}
else
	LIB += ${LIB_32}
endif
#-------------------------------------------------------------------------------
PRO2CPP     := ${BASEPATH}/bin/protoc

PRO_REMOTE_SRC += 
PRO_REMOTE_H   := $(foreach src, $(PRO_REMOTE_SRC), $(basename $(notdir $(src))).pb.h)
PRO_REMOTE_CPP := $(foreach src, $(PRO_REMOTE_SRC), $(basename $(notdir $(src))).pb.cc)
PRO_REMOTE_OBJ := $(foreach src, $(PRO_REMOTE_SRC), $(basename $(notdir $(src))).pb.o)

LOCAL_SRC += $(sort $(wildcard *.cpp *.c) $(PRO_REMOTE_CPP))
LOCAL_OBJ += $(patsubst %.cpp,%.o, $(patsubst %.cc,%.o, $(patsubst %.c,%.o, $(LOCAL_SRC))))
DEP_FILE  := $(foreach obj, $(LOCAL_OBJ), $(dir $(obj))$(basename $(notdir $(obj))).d)
#-------------------------------------------------------------------------------
all : $(PRO_REMOTE_H) $(PRO_REMOTE_CPP) $(LOCAL_OBJ) $(TARGET)

$(TARGET) : $(LOCAL_OBJ) $(PRO_REMOTE_OBJ)
	$(CXX) -m$(MFLAGS) $(CFLAGS) -shared -o $@ $^ $(INCLUDE) $(LIB)

ifneq ($(PRO_REMOTE_SRC),)
$(PRO_REMOTE_H) $(PRO_REMOTE_CPP) : $(PRO_REMOTE_SRC)
	@echo "protoc ${P2CPP_FLAG} "
	rm -vf $(PRO_REMOTE_H) $(PRO_REMOTE_CPP) $(PRO_REMOTE_OBJ)
	$(PRO2CPP) ${P2CPP_FLAG} --proto_path=$(dir $(PRO_REMOTE_SRC)) --cpp_out=./ $(PRO_REMOTE_SRC)
endif

clean:
	rm -f $(LOCAL_OBJ) $(TARGET) *~ .pb.* *.pb.* *.64.*

cleanall:
	rm -f $(LOCAL_OBJ) $(TARGET) $(PRO_REMOTE_H) $(PRO_REMOTE_CPP) *.d *.o

ifneq ($(DEP_FILE),)
    -include $(DEP_FILE)
endif

%.o: %.cpp
	$(CXX) -m$(MFLAGS) $(CFLAGS) $(INCLUDE) -o $@ -c $<

%.o: %.cc
	$(CXX) -m$(MFLAGS) $(CFLAGS) $(INCLUDE) -o $@ -c $<

%.o: %.c
	$(CC) -m$(MFLAGS) $(CFLAGS) $(INCLUDE) -o $@ -c $<
#-------------------------------------------------------------------------------
