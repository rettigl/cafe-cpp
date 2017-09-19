INCLUDES = -I./include -I./ -I/opt/gfa/cafe/boost/boost -I/usr/local/epics/base/include -I/usr/local/epics/base/include/os/Linux -I/usr/include
CC=g++
COMPILE.cpp =$(CC) -c
CPPFLAGS = -fPIC
OUTPUT_OPTION = -o $@
LIB_LOCAL = -L/usr/local/epics/base/lib/SL6-x86_64  -L/afs/psi.ch/project/zeromq/devl/mate/deps/boost/lib \
 -Wl,-rpath,/afs/psi.ch/project/zeromq/devl/mate/deps/boost/lib \
 -L/usr/lib64
LIBS = -lca -lCom -lQtCore -lQtXml -lboost_thread-mt  -lboost_system  
#LIB_LOCAL = -L/usr/local/epics/base/lib/SL6-x86_64  -L/usr/lib64
#LIBS = -lca -lCom -lQtCore -lQtXml 


libcafe.so: src/cafeCache.o src/cafeGroup.o src/cafe.o src/cafeVectors.o \
			 src/cafeXML.o src/callbackHandlerCreate.o src/callbackHandlerMonitor.o src/conduitGroup.o src/conduit.o \
			 src/connectCallbacks.o src/connectGroup.o \
			 src/connect.o src/exceptionsHelper.o src/granules.o src/handleHelper.o src/helper.o \
			 src/loadCollectionXMLParser.o src/loadGroupXMLParser.o src/methodCallbacks.o src/policyHelper.o \
			 src/restorePVGroupXMLParser.o src/transpose.o 
			 $(CC) $(CPPFLAGS) -shared src/cafeCache.o src/cafeGroup.o src/cafe.o src/cafeVectors.o \
			 src/cafeXML.o src/callbackHandlerCreate.o src/callbackHandlerMonitor.o src/conduitGroup.o src/conduit.o \
			 src/connectCallbacks.o src/connectGroup.o \
			 src/connect.o src/exceptionsHelper.o src/granules.o src/handleHelper.o src/helper.o \
			 src/loadCollectionXMLParser.o src/loadGroupXMLParser.o src/methodCallbacks.o src/policyHelper.o \
			 src/restorePVGroupXMLParser.o src/transpose.o $(LIB_LOCAL) $(LIBS) -o libcafe.so
%.o: %.cpp
			 $(COMPILE.cpp) $(CPPFLAGS) $(INCLUDES) $(OUTPUT_OPTION) $<



