#
# Jan Chrin
# June 2016
#
# Makefile to build a simple C++ cafe client 
#


#CAFE version to link to
CAFE_VERSION=cafe-1.12.5-gcc-7.3.0

#PYTHON_INCLUDE=/opt/gfa/python-3.5/latest/include/python3.5m
#PYTHON_LIB=/opt/gfa/python-3.5/latest/lib

EPICS_BASE=${EPICS}/base

#CAFE project base
CAFE_BASE=/opt/gfa/cafe
CAFE_CPP_BASE=${CAFE_BASE}/cpp

CAFE_BASE=/afs/psi.ch/project/cafe/gitlab/CAFE/
CAFE_CPP_BASE=${CAFE_BASE}
CAFE_VERSION=cpp


BOOST_BASE=${CAFE_BASE}/boost/boost_1_61_0/include

INCLUDE_PATH += -I$(CAFE_CPP_BASE)/$(CAFE_VERSION)/include \
 -I$(EPICS_BASE)/include -I$(EPICS_BASE)/include/os/Linux \
 -I$(BOOST_BASE) 
 #-I$(BOOST_BASE)/boost

LIB_DIR += -L$(EPICS_BASE)/lib/${EPICS_HOST_ARCH} \
 -Wl,-rpath,$(EPICS_BASE)/lib/${EPICS_HOST_ARCH} \
 -L$(CAFE_CPP_BASE)/$(CAFE_VERSION)/lib \
 -Wl,-rpath,$(CAFE_CPP_BASE)/$(CAFE_VERSION)/lib

LIBS += -lcafe -lca -lCom 

cafeTest: cafeTest.cc callbacks.h
	g++ cafeTest.cc -o cafeTest \
	$(INCLUDE_PATH) $(LIB_DIR) $(LIBS)

clean:
	rm -f cafeTest.o cafeTest

