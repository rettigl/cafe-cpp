INCLUDES = -Ic:\CAFE\CAFE\cpp\include -Ic:\CAFE\CAFE\cpp -Ic:\local\boost_1_62_0\boost  \
-Ic:\local\boost_1_62_0 -Ic:\epics\base-3.14.12.5\include -Ic:\epics\base-3.14.12.5\include\os\WIN32 \
-Ic:\Qt\4.8.4\include
CXX=cl
CXXFLAGS = /W4 /EHsc /c
OUTPUT_OPTION = /o $@
LIB_LOCAL = C:\epics\base-3.14.12.5\lib\windows-x64\Com.lib C:\epics\base-3.14.12.5\lib\windows-x64\ca.lib \
C:\local\boost_1_62_0\lib64-msvc-10.0\libboost_thread-vc100-mt-s-1_62.lib \
C:\local\boost_1_62_0\lib64-msvc-10.0\libboost_system-vc100-mt-s-1_62.lib \
C:\Qt\4.8.4\lib\QtCore4.lib C:\Qt\4.8.4\lib\QtXml4.lib
LIBS = -lca -lCom 

OBJS= src\cafeCache.obj src\cafeGroup.obj src\cafe.obj src\cafeVectors.obj \
			 src\cafeXML.obj src\callbackHandlerCreate.obj src\callbackHandlerMonitor.obj src\conduitGroup.obj src\conduit.obj \
			 src\connectCallbacks.obj src\connectGroup.obj \
			 src\connect.obj src\exceptionsHelper.obj src\granules.obj src\handleHelper.obj src\helper.obj \
			 src\loadCollectionXMLParser.obj src\loadGroupXMLParser.obj src\methodCallbacks.obj src\policyHelper.obj \
			 src\restorePVGroupXMLParser.obj src\transpose.obj $(LIB_LOCAL)

cafe.lib: $(OBJS)
	LIB $(OBJS) /out:cafe.lib

%.obj: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $<



