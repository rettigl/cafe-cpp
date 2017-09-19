INCLUDES = -IC:\CAFE\cafe-1.0.0-beta-3\cpp\include -IC:\local\boost_1_63_0\boost  \
-IC:\local\boost_1_63_0 -IC:\EPICS\base-3.14.12.5\include -IC:\EPICS\base-3.14.12.5\include\os\WIN32 \
-IC:\Qt\qt-4.8.6-x64-msvc2010\include -Ic:\Qt\qt-4.8.6-x64-msvc2010\include -Ic:\Qt\qt-4.8.6-x64-msvc2010\include\QtCore \
-IC:\Qt\qt-4.8.6-msvc2010\include\QtXml 
#-IC:\Users\chrin\AppData\Local\Continuum\Anaconda3\envs\py345\include

LIB_DEST=lib\cafe

CXX=cl
CXXFLAGS = /W4 /EHsc /c /MT /DHAVE_LIBQTXML
OUTPUT_OPTION = /o "$@"
LIB_LOCAL = C:\epics\base-3.14.12.5\lib\windows-x64\Com.lib C:\epics\base-3.14.12.5\lib\windows-x64\ca.lib \
C:\Qt\qt-4.8.6-x64-msvc2010\lib\QtCore4.lib C:\Qt\qt-4.8.6-x64-msvc2010\lib\QtXml4.lib \
C:\local\boost_1_63_0\lib64-msvc-10.0\libboost_thread-vc100-mt-s-1_63.lib \
C:\local\boost_1_63_0\lib64-msvc-10.0\libboost_system-vc100-mt-s-1_63.lib 
#C:\Users\chrin\AppData\Local\Continuum\Anaconda3\envs\py345\libs\python34.lib \


OBJS= cafeCache.obj cafeGroup.obj cafe.obj cafeVectors.obj \
			 callbackHandlerCreate.obj callbackHandlerMonitor.obj conduitGroup.obj conduit.obj \
			 connectCallbacks.obj connectGroup.obj \
			 connect.obj exceptionsHelper.obj granules.obj handleHelper.obj helper.obj \
			 methodCallbacks.obj policyHelper.obj transpose.obj  \
			 cafeXML.obj loadCollectionXMLParser.obj loadGroupXMLParser.obj \
			 restorePVGroupXMLParser.obj $(LIB_LOCAL)



$(LIB_DEST)\cafe.lib: $(OBJS)
	LIB $(OBJS) /out:$(LIB_DEST)\cafe.lib


%.obj: src\%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OUTPUT_OPTION) $< 
	
	
clean:
	del *.obj 


