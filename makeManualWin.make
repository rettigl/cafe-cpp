INCLUDES = -IC:\CAFE\CAFE\cpp\include -IC:\CAFE\CAFE\cpp -IC:\local\boost_1_62_0\boost  \
-IC:\local\boost_1_62_0 -IC:\epics\base-3.14.12.5\include -IC:\epics\base-3.14.12.5\include\os\WIN32 \
-IC:\Qt\qt-4.8.6-x64-msvc2010\include -Ic:\Qt\qt-4.8.6-x64-msvc2010\include -Ic:\Qt\qt-4.8.6-x64-msvc2010\include\QtCore \
-IC:\Qt\qt-4.8.6-msvc2010\include\QtXml \
-IC:\Users\chrin\AppData\Local\Continuum\Anaconda3\envs\py345\include
CXX=cl
CXXFLAGS = /W4 /EHsc /c /MT
OUTPUT_OPTION = /o "$@"
LIB_LOCAL = C:\epics\base-3.14.12.5\lib\windows-x64\Com.lib C:\epics\base-3.14.12.5\lib\windows-x64\ca.lib \
C:\Qt\qt-4.8.6-x64-msvc2010\lib\QtCore4.lib C:\Qt\qt-4.8.6-x64-msvc2010\lib\QtXml4.lib \
C:\Users\chrin\AppData\Local\Continuum\Anaconda3\envs\py345\libs\python34.lib \
#C:\local\boost_1_62_0\lib64-msvc-10.0\libboost_thread-vc100-mt-s-1_62.lib \
#C:\local\boost_1_62_0\lib64-msvc-10.0\libboost_system-vc100-mt-s-1_62.lib \

LIBS = -lca -lCom -lQtCore4 -lQtXml4

OBJS= src\cafeCache.obj src\cafeGroup.obj src\cafe.obj src\cafeVectors.obj \
			 src\callbackHandlerCreate.obj src\callbackHandlerMonitor.obj src\conduitGroup.obj src\conduit.obj \
			 src\connectCallbacks.obj src\connectGroup.obj \
			 src\connect.obj src\exceptionsHelper.obj src\granules.obj src\handleHelper.obj src\helper.obj \
			 src\methodCallbacks.obj src\policyHelper.obj src\transpose.obj  \
			 src\cafeXML.obj src\loadCollectionXMLParser.obj src\loadGroupXMLParser.obj \
			 src\restorePVGroupXMLParser.obj $(LIB_LOCAL)

cafe.lib: $(OBJS)
	LIB $(OBJS) /out:cafe.lib

%.obj: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OUTPUT_OPTION) $< 
	
	
clean:
	del src\*.obj 


