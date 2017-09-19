//
//  A simple example
//
#include <cafe.h>

#include "callbacks.h"

#define PV_X1      "ARIDI-BPM-01LE:X-AVG"
#define PV_Y1      "ARIDI-BPM-01LE:Y-AVG"
#define PV_WF      "ARIDI-BPM-01LE:WF-INT-1"   //256 elements
//define PV_WF 		 "SINSB04-ROPT-LOG:MON-MSG"
#define PV_MBBI    "ARIDI-BPM-01LE:GET-ENABLE" //mmbi
#define PV_X1_DESC "ARIDI-BPM-01LE:X-AVG.DESC" //dbr_string_t

#define PV_JOKE    "PV:JOKE"



#define NHANDLES 6 // should match number of pvs above that we are to open.


template<typename A, typename B>
std::pair<B,A> flip_pair(const std::pair<A,B> &p)
{
	return std::pair<B,A>(p.second, p.first);
}


template <typename A, typename B>
std::multimap<B,A> flip_map(const std::map<A,B> & src) {
	
	std::multimap<B,A> dst;
	
  std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()),
									flip_pair<A,B>);  
	
	return dst;	
}



int main( int argc, char *argv[] )
{
    using namespace std;
		
		// Test with 6 PVS
		// PV_X1,PV_Y1 are scalar numerics
		// PV_WF is a vecror numeric
		// PV_MBBI is anmbbi record witha number of enum vakues
		// PV_X!_DESC is of data type dbr_string_t 
		// PV_JOKE is a non-existant channel (used to show CAFE response is such cases)
		
    string pvArray[NHANDLES]={PV_X1,PV_Y1,PV_WF,PV_MBBI,PV_X1_DESC,PV_JOKE};
		unsigned int hArray[NHANDLES];
		
		//------------------------------------------------------------
    //(1) Preliminaries
    //------------------------------------------------------------
		
		cout << "------------------------------------------------------------" << endl;
		cout << "START: (1) Preliminaries " << endl;
		cout << "------------------------------------------------------------" << endl;
		
	  //Instantaite CAFE
		
		ChannelRequestPolicy channelRequestPolicyPutNew;
		channelRequestPolicyPutNew.setPolicy(CAFENUM::FLUSH_AFTER_EACH_MESSAGE,
									  CAFENUM::NO_WAIT, CAFENUM::WITH_CALLBACK_DEFAULT); //W
										
		ChannelRequestPolicy channelRequestPolicyGetNew;
		channelRequestPolicyGetNew.setPolicy(CAFENUM::FLUSH_AFTER_EACH_MESSAGE,
									  CAFENUM::WAIT, CAFENUM::WITHOUT_CALLBACK);								
		
		ChannelGetActionWhenMonitorPolicy cgawmp; 
		cgawmp.setActionKind(CAFENUM::GET_FROM_CACHE);
									 			
				
    CAFE  * cafe = new CAFE(); //channelRequestPolicyPutNew);
   
	 
	  // cafe->channelRequestPolicyMasterPut.setPolicy(CAFENUM::FLUSH_AFTER_EACH_MESSAGE,
		//							  CAFENUM::NO_WAIT,  CAFENUM::WITH_CALLBACK_DEFAULT); //WITHOUT_CALLBACK)
	 
	  cout << "---1------------------------------------------------------ " << endl;
	  ChannelRequestPolicy * channelRequestPolicySet = new ChannelRequestPolicy();
	  cout << "---2------------------------------------------------------ " << endl;
	  ChannelRequestPolicy channelRequestPolicySet2;
	  cout << "---3------------------------------------------------------ " << endl;
	  unsigned int handle;
    int status;
		//Class with methods to report on the value and meaning of status codes
    CAFEStatus cstat;
		//Classes defining value/name pairs of alarm status and and severity
		CAFEGlobalAlarmSeverity  alarmSev;
		CAFEGlobalAlarmCondition alarmStat;
		
		HandleHelper handleHelper;
	

		// This is optional
    //Initialize ca (otherwise first cafe method call will initialize ca)
	   cout << "---4------------------------------------------------------ " << endl;
    try {
      status=cafe->init( ); // ca_disable_preemptive_callback
    }
    catch (CAFEException_init &e) {
      cout << e.what() << endl;
      exit(1);
    }
		
				
		// This too is optional
		// The first Cafe method invoked on this channel will otherwise enable the virtual circuit
		// An open operation will pend the ioc for a default amount of time fiven by:
		// cafe->channelOpenPolicy.getDefaultTimeout() 
		// else otherwise specified.
		cout << "---5------------------------------------------------------ " << endl;
		cafe->channelOpenPolicy.setTimeout(0.1); //pend 0.1 seconds
		cout << "---6------------------------------------------------------ " << endl;
		// It is not an error if the channel is not connected!	 
		cout << "open " << endl;
		try {       		
			cafe->open(pvArray[0].c_str(), handle);			
		}
		catch(CAFEException_open & e) {
		  cout << e.what() << endl;
			exit(1);
		}
					
		cout << "------------------------------------------------------------" << endl;
		cout << "  END: (1) Preliminaries " << endl;
		cout << "------------------------------------------------------------" << endl;
		sleep(1);
		
		cout << "when /" << channelRequestPolicySet->getWhenToFlushSendBuffer() << endl;
		cout << "wait /" << channelRequestPolicySet->getWaitKind() << endl;
		cout << "method /" << channelRequestPolicySet->getMethodKind() << endl;	
				
		cout <<  cafe->getPolicyHelper().getChannelRequestPolicyPut(1, *channelRequestPolicySet ) << endl;
		
		cout << "when /" << channelRequestPolicySet->getWhenToFlushSendBuffer() << endl;
		cout << "wait /" << channelRequestPolicySet->getWaitKind() << endl;
		cout << "method /" << channelRequestPolicySet->getMethodKind() << endl;	
		
					
		cout <<  cafe->getPolicyHelper().getChannelRequestPolicyGet(1, *channelRequestPolicySet ) << endl;
		
		cout << "when /" << channelRequestPolicySet->getWhenToFlushSendBuffer() << endl;
		cout << "wait /" << channelRequestPolicySet->getWaitKind() << endl;
		cout << "method /" << channelRequestPolicySet->getMethodKind() << endl;	
		
		
		ChannelGetActionWhenMonitorPolicy cm;		
		cout <<  cafe->getPolicyHelper().getChannelGetActionWhenMonitorPolicy(1, cm ) << endl;
		cout << cm.getActionKind() << endl;
		
		
		
		cout << "when /" << channelRequestPolicySet2.getWhenToFlushSendBuffer() << endl;
		cout << "wait /" << channelRequestPolicySet2.getWaitKind() << endl;
		cout << "method /" << channelRequestPolicySet2.getMethodKind() << endl;	
				
		cout <<  cafe->getPolicyHelper().getChannelRequestPolicyPut(1, channelRequestPolicySet2 ) << endl;
		
		cout << "when /" << channelRequestPolicySet2.getWhenToFlushSendBuffer() << endl;
		cout << "wait /" << channelRequestPolicySet2.getWaitKind() << endl;
		cout << "method /" << channelRequestPolicySet2.getMethodKind() << endl;	
		
		
		
		
		cout << "------------------------------------------------------------" << endl;
		cout << "START: (1b) Opening and Closing Channels " << endl;
		cout << "------------------------------------------------------------" << endl;
		
		unsigned int hDesc;
				
		vector<unsigned int> hV; 
		vector<string> pvV;
		vector<unsigned short> stateV;
		
		
		try{ 
		 
		  //For multiple invocations of cafe->open it is more efficient and less of a
			//time sink to first prepare the messages before flushing the message buffer 
			//once at the end.
			//This can be done by configuring the channelopenPolicy class accordingly.
			//See policies.h
		  //Set open policy so as to create virtual circuits in one step
      //cafe->channelOpenPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
      //cafe->channelOpenPolicy.setFlushSendBufferKind(WITH_PEND_EVENT);
			//cafe->channelOpenPolicy.setTimeout(1.0);
			
			//cafe->open(...)
			
			//cafe->openNow() // Now send messages to IOC
		 
		  
		  //However, for convenience open policy has been conveniently packaged so that 
			//only the openPrepare method need be called a priori 
			
		  cafe->openPrepare();		 		 
			cafe->open(pvArray,  hArray, NHANDLES);					
			cafe->open(PV_X1_DESC, hDesc); 

      cafe->openNow() ; // and wait for cafe->channelOpenPolicy.getTimeout()
			//or
      cafe->openNowAndWait(0.5);  //wait for specified time, here 3.0 seconds
			
			
			//Is the given channel connected? Returns true/false
			bool ifYes= cafe->isChannelConnected(hDesc); 
			
			//Are all channels connected? Returns true/false
			ifYes= cafe->allChannelsConnected();
			
			
			if (!ifYes) {
      	cafe->printDisconnectedHandles();
			}
	
			
			status=handleHelper.getDisconnectedHandles(hV, pvV);
			status=handleHelper.getHandles(hV, pvV);
			status=handleHelper.getHandleStates(hV, pvV,stateV);
				
						
 		  //cafe->channelOpenPolicy.flushSendBufferNow(); //Now send open message		
			//cafe->channelOpenPolicy.setTimeoutToDefault(); //Reset timeout to default value
	
		}
		catch(CAFEException_open & e) {
		  cout << e.what() << endl;
		}
		
		
		// To close channels within a given ca_client_context
		
		// To close a single Channel
		//cafe->close(hDesc);
		
		// To close many channels
		//cafe->closeChannels(hArray, NHANDLES);
	
	  // To close alls channesl
		//cafe->closeChannels();
			
		// If you wish to close channels that are in other threads 
		// (i.e., with other ca_client_context)
		// use instead the following methods:	
		// To close a single Channel irrespective of ca_client_context
		// I.e. these will also close handle s in other threads
		//cafe->closeHandle(hDesc);
		
		// To close many handles
		//cafe->closeHandles(hArray, NHANDLES);
	
	  // To close all handles
		//cafe->closeHandles();	
	
	/*
		cafe->loadCollectionsFromXML("cNodes.xml");
		cafe->loadGroupsFromXML("gDBPM.xml");
		//cafe->loadCollectionsFromXML("cSF_Aramis_bunch1.xml");
		//cafe->loadCollectionsFromXML("testVSUP.xml");
		//cafe->loadGroupsFromXML("bpmDisplaySFEnergyBunch1.xml");
		
		vector<string> _glist;
		cafe->groupMemberList("gDBPM", _glist);
		vector<unsigned int> _hVA;
		
		cafe->openPrepare();		 		 
		cafe->open(_glist,  _hVA);					
		
    cafe->openNowAndWait(2.0);  //wait for specified time, here 3.0 seconds
			
		*/
		
		vector<string> _glist;
			
		vector<unsigned int> _hVA;
				
		typedef std::map<float, std::string> floatStringMap2;
		floatStringMap2 posDev2;
		floatStringMap2::iterator pos2;
		
		/*
		cafe->devicePositionMap("cDBPM", posDev2);
		
		for (pos2 =posDev2.begin(); pos2 != posDev2.end(); ++pos2) {
			cout << "position= " << pos2->first << " Dev " << pos2->second << endl;		
			//++pos;
		}		
	*/	
		//cafe->readyDBPM(_hVA);
		
		
	//	cafe->prepareDBPM(_glist, _hVA, posDev2);
		
		
		//cafe->devicePositionMap("cDBPM", posDev2);
		
	//	for (pos2 =posDev2.begin(); pos2 != posDev2.end(); ++pos2) {
	//		cout << "position= " << pos2->first << " Dev " << pos2->second << endl;		
		
	//	}		
		
		
		//DBPMKeeper dbpm(_glist,_hVA,posDev2);
	
	
	  vector<float> posV; vector<string> devS;
		 cout << " HERE prepareDBPM" << endl;
		
		
	 	cafe->prepareDBPM(_glist, _hVA, devS, posV);		
		
    cout << " HERE " << endl;
		
	  DBPMKeeper dbpm(_glist,_hVA, devS, posV);
		
		cafe->readDBPMOffsets(dbpm);
		vector <double> ox    = dbpm.getOffsetX();
		vector <double> oy    = dbpm.getOffsetY();
		vector <string> devis = dbpm.getDevice();
		
		cout << "size x " << ox.size() << endl;
		cout << "size y " << oy.size() << endl;
		cout << "devis  " << devis.size() << endl;
			
		for (int i=0; i<devis.size(); ++i) {
		
			cout << i << " " << devis[i] << " xoffset= " << ox[i] << " " << oy[i] << endl;
		}
	
		//for (int i=0; i<devS.size(); ++i) {		
		//	cout << _glist[i] << " " << _hVA[i] << " " << devS[i] << " " << posV[i] << endl; 
		//}
	
	  cafe->terminate();
		exit(0);
	
		////dbpm.setBS(true);
		
		////vector<double> x,y,q,E;
		////vector<bool> vx,vy,vq;
		vector<DBPMData> ddx, ddy, ddq, dde;
	  ChannelRequestStatus cre;
		vector<string> devices;
		
	  cout << "STATUS GETCTRL = " << cafe->getHandleHelper().getChannelRequestStatusGetCtrl(8, cre) << endl;	 
	  cout << "Value 1 is PENDING, while 2 is COMPLETE: " << cre.getCallbackProgressKind()  << endl;
	 
		
		system("date");
		for (int i=0; i<200; ++i ) {
		  if (i==0) dbpm.setBS(true);
		  status=cafe->getDBPM(dbpm);
			
			if (status > ICAFE_LINUX_ERROR) continue;
			
			 //cout << i << " STATUS GETCTRL = " << cafe->getHandleHelper().getChannelRequestStatusGetCtrl(8, cre) << endl;
	 
	     //cout << i << " Value 1 is PENIDING, while 2 is COMPLETE: " << cre.getCallbackProgressKind()  << endl;
	   cout << "STATUS " << status << " " << dbpm.status << endl; 
		
			ddx=dbpm.getX();
			ddy=dbpm.getY();
			ddq=dbpm.getQ();
			dde=dbpm.getEnergy();
				
			devices = dbpm.getDevice();
				
			cout << " ddx.size " << ddx.size() << endl;	
			cout << " ddy.size " << ddy.size() << endl;	
			cout << " ddq.size " << ddq.size() << endl;	
			cout << " dde.size " << dde.size() << endl;	
			for (int j=0; j<ddx.size(); ++j) {
			   cout << i << " " << devices[j] << " " << devS[j] << " value="  << ddx[j].getValue() << " status=" <<  ddx[j].getStatus() << endl;
			
			}
						
		  //cout << i << " " << ddx[0].getValue() << " "   << ddy[0].getValue() << " " 
			//<< ddq[0].getValue() << " " << dde[0].getValue() <<  endl;
							
		  //cout << i << " " << ddx[0].getEpicsTimeStamp().secPastEpoch << " "   << ddx[0].getEpicsTimeStamp().nsec << " " 
			//<< ddx[0].getStatus() << " "  <<  endl;
			
			
		  //cout << dbpm.getY()[0] << " " << dbpm.getY()[6] << " " << dbpm.getY()[7] << endl;
		  //cout << dbpm.getQ()[0] << " " << dbpm.getQ()[6] << " " << dbpm.getQ()[7] << endl;
		
		  //for (int i=0; i<_glist.size(); ++ i) {
		  //	cout << _glist[i] << " " << " [" << "] "  ;
		  //}
			//usleep(1000);
		  //cout << endl;
			
			//sleep(1);
			
		}
		system("date");
		cafe->terminate();
		exit(0);	
			
			
			
		
		PVDataHolder * pdh = new PVDataHolder[_glist.size()];
		system("date");
		for (int i=0; i<1; ++i) {	
			status=cafe->get(_hVA, pdh);
		}	
			system("date");
		cafe->closeHandles(_hVA);
		
		unsigned int _gh;
		
		cafe->groupOpen("gDBPM", _gh);
		PVGroup _pvg;
		system("date");
		status=cafe->groupGet(_gh, _pvg);
		system("date");
		cout <<"status + " << status << endl;
			
		cafe->terminate();
		exit(0);
		
		
		typedef std::map<float, std::string> floatStringMap;
		floatStringMap posDev;
		floatStringMap::iterator pos;
		
		cafe->devicePositionMap("cDBPM", posDev);
	
		
		for (pos =posDev.begin(); pos != posDev.end(); ++pos) {
			cout << "device= " << pos->first << " Pos " << pos->second << endl;		
			//++pos;
		}
		
		
		cafe->devicePositionMap("cDBPM", posDev);
			
		for (pos =posDev.begin(); pos != posDev.end(); ++pos) {
			cout << "device= " << pos->first << " Pos " << pos->second << endl;		
			//++pos;
		}
			
		posDev.clear();
		cafe->devicePositionMap("cVSUP", posDev);
			
		for (pos =posDev.begin(); pos != posDev.end(); ++pos) {
			cout << "device= " << pos->first << " Pos " << pos->second << endl;		
			//++pos;
		}	
		
		vector<const char *> deviceListV;
		deviceListV.push_back("AB-CD");
		deviceListV.push_back("AB-EF");	
		cafe->collectionDefine("cTest", deviceListV);
		
		posDev.clear();
		cafe->devicePositionMap("cTest", posDev);
			
		for (pos =posDev.begin(); pos != posDev.end(); ++pos) {
			cout << "device= " << pos->first << " Pos " << pos->second << endl;		
			//++pos;
		}	
		
		double dValArray[12];
		int sAy[12];	
		cafe->getCache(hArray, NHANDLES, dValArray, sAy);	
		
			
		for (int i=0; i<NHANDLES; ++i) {
			cout << "val from Cache " << dValArray[i] << " status " << sAy[i]<< endl;	
		}	
		
		
		
		
		//cafe->terminate();
		//exit(1);
			
 		vector<string> gl;
 		cafe->collectionList(gl);
		for (int i=0; i < gl.size(); ++i) {
			cout << gl[i] << endl;
		}
		PVGroup pvgrp;
		unsigned int ghan; 
		cafe->groupOpen("gDBPM", ghan);
		cafe->groupGet(ghan, pvgrp);
		//pvgrp.show();
		
		
		
		cout << "------------------------------------------------------------" << endl;
		cout << "  END: (1b) Opening and Closing Channels " << endl;
		cout << "------------------------------------------------------------" << endl;
		
		sleep(3);
		
		//------------------------------------------------------------
    //(2) Basic Single Channel Operations
    //------------------------------------------------------------
		
		cout << "------------------------------------------------------------" << endl;
		cout << "START: (2) Basic Single Channel Operations " << endl;
		cout << "------------------------------------------------------------" << endl;
		
		//------------------------------------------------------------
    //Data retrieval methods returning a scalar value
		//------------------------------------------------------------
		
    double d;
		//get by handle
		status=cafe->get(handle, d);
			
		if (status != ICAFE_NORMAL) {
		  cout << "Status indicates an error at line no." << __LINE__ << endl;
		 	cstat.report(status);	
			//or
			cafe->printStatus(handle,status);	
		}
		else {
			cout << "Value d=" << d << endl; 
		}
			
		float f; short alarmStatus, alarmSeverity; epicsTimeStamp ets;
		status=cafe->get(handle, f, alarmStatus, alarmSeverity, ets);
			
		if (status != ICAFE_NORMAL) {
		  cout << "Status indicates an error at line no." << __LINE__ << endl;
			cafe->printStatus(handle,status);	
		}
		else {
			cout << "Value f=" << f << endl;
			cout << "AlarmStatus= " << alarmStat.asString(alarmStatus) << " AlarmSeverity=" << alarmSev.asString(alarmSeverity)
          << " TimeStamp= " << ets.secPastEpoch << " sec. " << ets.nsec << " nsec " << endl;		
 		}
		
		
		//get by pvname
		status=cafe->get(pvArray[0].c_str(), d);
		if (status != ICAFE_NORMAL) {
		  cout << "Status indicates an error at line no." << __LINE__ << endl;
		 	cstat.report(status);	
			//or
			cafe->printStatus(handle,status);	
		}	
	
		
		//To explicitly check on timeout error
		if (cstat.csc.isTimeout(status)) {
			cout << "Above is a timeout error" << endl;	
		}
		
		//To interchange between handle and array
		            handle = cafe->getHandleFromPV(pvArray[0].c_str());
		const char * pvName= cafe->getPVFromHandle(handle);
		  
			
		
		
		//------------------------------------------------------------	
		//Data retrieval methods returning structured data
		//------------------------------------------------------------
		unsigned int hWF;
		status=cafe->open(PV_WF,hWF);	
		//status=cafe->open("XHIPA-STA:STA-ND1",hWF);
		
		
		
		double dwff;
		cafe->get(hWF, dwff);
		
		cout << "wfd " << dwff << endl;
		string ms;
		cafe->getWFAsString(hWF, ms);
		cout << "wf asstring 1st " << ms << endl;
		 
		cafe->getWFAsString(hWF, ms);
		cout << "wf asstring 2nd" << ms << endl; 
		 
		cafe->getWFAsStringCache(hWF, ms);
		cout << "wf asstring 3rd" << ms << endl; 
		
			
		PVDataHolder pvd;
		//pvd.setNelem(cafe->getNelemNative(hWF));
		pvd.setNelem(cafe->getNelemNative(hWF));
		status=cafe->get(hWF, pvd);
		
		if (status==ICAFE_NORMAL) {
		
			pvd.print(10); //Show only 10 elements of the waveform
		
			//The PVDataHolder
			cout << "pvd.getAsString()" << endl;
			for (int i=0; i<min(5,(int) cafe->getNelemRequest(hWF)); ++i) {
				cout  << pvd.getAsString(i) << " [" << i << "] ";
			}	
			cout << endl;
			//First Element Only
			cout << "pvd.getAsDbr_string_t() = " << pvd.getAsDbr_string_t() << endl;
			cout << "pvd.getAsDouble()       = " << pvd.getAsDouble() << endl;
			cout << "pvd.getAsFloat()        = " << pvd.getAsFloat() << endl;
			cout << "pvd.getAsChar()         = " << (unsigned short) pvd.getAsChar() << endl;
			cout << "pvd.getAsShort()        = " << pvd.getAsShort() << endl;
			cout << "pvd.getAsUShort()       = " << pvd.getAsUShort() << endl;		
			cout << "pvd.getAsLong()         = " << pvd.getAsLong() << endl;
			cout << "pvd.getAsULong()        = " << pvd.getAsULong() << endl;
			cout << "pvd.getAsLongLong()     = " << pvd.getAsLongLong() << endl;
			cout << "pvd.getAsULongLong()    = " << pvd.getAsULongLong() << endl;
		
			boost::shared_ptr<vector<double> > spVd = pvd.getAsVDouble();				
			cout << "pvd.getAsVDouble()      = " << spVd.get()[0][0] << " [0] " << spVd.get()[0][1] << " [1] " << endl;		
			vector<double> * vd= spVd.get();
			cout << "pvd.getAsVDouble()      = " << vd[0][0] << " [0] " << vd[0][1] << " [1] " << endl;	
		
			boost::shared_ptr<vector<float> >  spVf = pvd.getAsVFloat();
			cout << "pvd.getAsVFloat()       = " << spVf.get()[0][0] << " [0] " << spVf.get()[0][1] << " [1] " << endl;
			vector<float> * vf= spVf.get();
			cout << "pvd.getAsVFloat()       = " << vf[0][0] << " [0] " << vf[0][1] << " [1] " << endl;	
	
		
			cout << "pvd.getPVName()         = " << pvd.getPV() << endl;
			cout << "pvd.getPVAlias()        = " << pvd.getPVAlias() << endl; 
			cout << "pvd.getDevice()         = " << pvd.getDevice() << endl;
			cout << "pvd.getAttribute()      = " << pvd.getAttribute() << endl; 
			cout << "pvd.getNelem()          = " << pvd.getNelem() << endl;	
			cout << "pvd.getSize()           = " << pvd.getSize() << endl; //Size of PVDataHolder container
			cout << "pvd.getVal()            = " << pvd.getVal() << endl; //CAFE_DATATYPE_UNION_SEQ type
			cout << "pvd.getVal()[0].f       = " << pvd.getVal()[0].f << endl; // extract assuming a float
			cout << "pvd.getDataType()       = " << pvd.getDataType() << endl; // CAFE_DATATYPE
		
			cout << "pvd.getStatus()         = " << pvd.getStatus() << endl;
		
			cout << "pvd.getHasAlarm         = " << pvd.getHasAlarm() << endl; // true if Alarm/Severity data returned 
			cout << "pvd.getAlarmStatus()    = " << pvd.getAlarmStatus() << endl;
			cout << "pvd.getAlarmSeverity()  = " << pvd.getAlarmSeverity() << endl; 		
			cout << "pvd.getHasTS            = " << pvd.getHasTS() << endl; // true if Epics Timestamps data returned 
			ets= pvd.getEpicsTimeStamp(); 
			cout << "epicsTimeStamp          = " << ets.secPastEpoch << " " << ets.nsec << endl;

				
			//cout << "pvd.getEnumIntegerValueAsString()  = " << pvd.getEnumIntegerValueAsString() << endl; //for mbbi/o records
			cout << "pvd.concatToString()    = " << pvd.concatToString() << endl; //for waveforesm with data type dbr_chart_t
			cout << "pvd.getWFAsString()    = " << pvd.concatToString() << endl; 
		
		
		 
			//------------------------------------------------------------	
			//Alarm, status/severity values 
			//------------------------------------------------------------
		
			//Alarm status and severities have value/name (int/string) pairs.
			//The string equivalent of a given value may be gotten as shown 				
		
			cout << "Alarm Status    =" << alarmStat.asString(pvd.getAlarmStatus()) << endl;
			cout << "Alarm Severity  =" << alarmSev.asString(pvd.getAlarmSeverity()) << endl;
		
			//Information concerning the meaning of the status of the cafe operation
			cout << cstat.severity(pvd.getStatus()) << endl;
			cout << cstat.asString(pvd.getStatus()) << endl;
			cout << cstat.code(pvd.getStatus()) << endl;
			cout << cstat.info(pvd.getStatus()) << endl;
			cout << cstat.message(pvd.getStatus()) << endl;
			//print summary
			cstat.report(pvd.getStatus());
			
			
			
			
		} // if status=ICAFE_NORMAL	
		//------------------------------------------------------------	
		//Set data
		//The set method is able to intrepret all data types, and can 
		//cater for scalar values and arrays
		//------------------------------------------------------------
	
    status=cafe->set(PV_X1, 0.02453);
	
    if (status!=ICAFE_NORMAL) {
      cstat.report(status);
    }
    //or another way to print an error with pv name
    cafe->printStatusIfError(cafe->getInfo().getHandleFromPV(PV_X1),status);
		
			
		
		//Now perform a get/set operation with a user supplied callback function
			
		
		//Get original policy in case we want to use it again.
	  //ChannelRequestPolicy channelRequestPolicyGetOriginal;
		//status=cafe->getPolicyHelper().getChannelRequestPolicyGet(hArray[0], channelRequestPolicyGetOriginal);
		
		
		ChannelRequestPolicy channelRequestPolicyLocal;	
		//Supply user defined handle	
		channelRequestPolicyLocal.setHandler(handlerGet);
		//Set policy to use
		//ChannelWhenToFlushSendBufferPolicyKind CAFENUM::FLUSH_AFTER_EACH_MESSAGE   
		//ChannelWaitForResponsePolicyKind CAFENUM::WAIT
		//ChannelRequestPolicyKind CAFENUM::WITH_CALLBACK_USER_SUPPLIED
		channelRequestPolicyLocal.setPolicy(CAFENUM::FLUSH_AFTER_EACH_MESSAGE,
									  CAFENUM::WAIT, CAFENUM::WITH_CALLBACK_USER_SUPPLIED); 
										
		//Now set this policy for GET operation for handle[0]											
		status=cafe->getPolicyHelper().setChannelRequestPolicyGet(hArray[0], channelRequestPolicyLocal);
		//Modify policy to accept PutHandler
		channelRequestPolicyLocal.setHandler(handlerPut);
		//Now set this policy for PUT operation for hanlde[0]
		status=cafe->getPolicyHelper().setChannelRequestPolicyPut(hArray[0], channelRequestPolicyLocal);
		
		status=cafe->set(PV_X1, 0.62453);
		
	
		cafe->get(PV_X1, f);	
		
		cout <<PV_X1 << " has value " << f << endl;
		
		cafe->getCache(PV_X1, f);	
		
		cout <<PV_X1 << " has value " << f << endl;
			
		cout << "------------------------------------------------------------" << endl;
		cout << "  END: (2) Basic Single Channel Operations " << endl;
		cout << "------------------------------------------------------------" << endl;

		//cafe->terminate();
		//exit(0);	

		//------------------------------------------------------------
    //(3) Waveforms and Arrays
    //------------------------------------------------------------
		
		cout << "------------------------------------------------------------" << endl;
		cout << "START: (3) Waveforms and Arrays  " << endl;
		cout << "------------------------------------------------------------" << endl;
		
				
		//------------------------------------------------------------	
		//Data retrieval methods returning a waveform array
		//------------------------------------------------------------
		
		PVDataHolder pvd_h;
		pvd_h.setNelem(cafe->getNelemNative(hWF));
		status=cafe->get(hWF, pvd_h);	
		cout << "n elements " << pvd_h.getNelem() << endl;
		status=cafe->getCache(hWF, pvd_h);		
		cout << "n elements " << pvd_h.getNelem() << endl;
		
	
		double * dwf = new double[cafe->getNelemNative(hWF)]; 
	
		double inwf [20] = {1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10, 11, 12, 13,14,15,16,17,18,19,20};
		
		
			
		//by pvName	
		status=cafe->set(PV_WF, inwf);
		status=cafe->get(PV_WF, dwf);
		
		if (status != ICAFE_NORMAL) {
		  cout << "Status indicates an error at line no." << __LINE__ << endl;
		 	cstat.report(status);		
		}
		else {
			cout << PV_WF << " successfully read out " << endl;
			cout << "First five elements have values: " << endl;
			for (int i=0; i<std::min(5,(int) cafe->getNelemNative(hWF)); ++i) {
					cout << dwf[i] << " [" << i << "] ";
			}
			cout << endl;
		}
			  
	
	  //One may also set the number of elements to retrieve
		//int nelem  =cafe->getHandleHelper().setNelemToNative(hWF);
		int nelem  = cafe->getHandleHelper().setNelem(hWF, 10); //returns max(10, cafe->getNelemNative(handle))
		//and offset
		int offset = cafe->getHandleHelper().setOffset(hWF, 2);
		//by Handle
		status=cafe->get(hWF, dwf);
		if (status != ICAFE_NORMAL) {
		  cout << "Status indicates an error at line no." << __LINE__ << endl;
		 	cstat.report(status);		
		}
		else {
			cout << PV_WF << " sucessfully read out " << endl;
			cout << "Elements 2 to 12 of WF (recall offset=2, nelem=10) have values:" << endl;
			for (int i=0; i<nelem; ++i) {
					cout << dwf[i] << " [" << i << "] ";
			}
			cout << endl;
		}
		
		//reset offset for read and the nelemtonative
		cout << "Resetting nelem for wf to native value: " << cafe->getHandleHelper().setNelemToNative(hWF) 
		     << " and resetting offset to: " <<  cafe->getHandleHelper().setOffset(hWF, 0) << endl;
			
		cafe->get(hWF);
		delete  dwf;
		
			
		//------------------------------------------------------------	
		//Data retrieval methods returning a std::vector
		//------------------------------------------------------------
		vector<float> dV;
	  dbr_short_t alarmSta; dbr_short_t alarmSe; epicsTimeStamp tsta;
		 		
		cout << "nELEM n: " << 	cafe->getHandleHelper().getNelemNative (hWF)  << endl;	
		cout << "nELEM r: " << 	cafe->getHandleHelper().getNelemRequest(hWF) << endl;
		cout << "nELEM c: " << 	cafe->getHandleHelper().getNelemClient (hWF)  << endl;
		
		cout << "set nelem to 16, get: " << cafe->getHandleHelper().setNelem(hWF,16) << endl;;
				
		cout << "nELEM n: " << 	cafe->getHandleHelper().getNelemNative (hWF)  << endl;	
		cout << "nELEM r: " << 	cafe->getHandleHelper().getNelemRequest(hWF) << endl;
		cout << "nELEM c: " << 	cafe->getHandleHelper().getNelemClient (hWF)  << endl;
				
				
		cout << "get nelem cache        get: " << cafe->getHandleHelper().getNelemToRetrieveFromCache(hWF) << endl;				
		cout << "set nelem cache to 18, get: " << cafe->getHandleHelper().setNelemToRetrieveFromCache(hWF,18) << endl;
		cout << "get nelem cache        get: " << cafe->getHandleHelper().getNelemToRetrieveFromCache(hWF) << endl;			
			
		//status=cafe->get(hWF, dV);
		//cout << "dV.size " << dV.size() << endl;
		status=cafe->getCache(hWF, dV,   alarmSta, alarmSe, tsta);	
		cout << "status " << status << endl; 
		cout << "dV.size " << dV.size() << endl;	
		if (status != ICAFE_NORMAL) {
		  cout << "Status indicates an error at line no." << __LINE__ << endl;
		 	cstat.report(status);		
		}
		else {
			cout << PV_WF << " successfully read out " << endl;
			cout << "Elements 1-5 of WF  have values:" << endl;
			for (int i=0; i<min(25, (int) dV.size()); ++i) {
					cout << dV[i] << " [" << i << "] ";
			}
			cout << endl;
		}
			
		//status=cafe->get(hWF, dV);
		//cout << "dV.size " << dV.size() << endl;	
		
		PVCtrlHolder pvc_h(cafe->getNelemNative(hWF));
		
		pvc_h.setNelem(12);
		
		status=cafe->getCtrl(hWF, pvc_h);	
		cout << "n elements " << pvc_h.getNelem() << endl;
		
			
		cout << "get nelem ctrl cache        get: " << cafe->getHandleHelper().getNelemToRetrieveFromCtrlCache(hWF) << endl;				
		cout << "set nelem ctrl cache to 18, get: " << cafe->getHandleHelper().setNelemToRetrieveFromCtrlCache(hWF,18) << endl;
		cout << "get nelem ctrl cache        get: " << cafe->getHandleHelper().getNelemToRetrieveFromCtrlCache(hWF) << endl;		
		
		status=cafe->getCtrlCache(hWF, pvc_h);		
		cout << "n elements " << pvc_h.setNelem(cafe->getHandleHelper().getNelemToRetrieveFromCtrlCache(hWF) ) << endl;	
		if (status != ICAFE_NORMAL) {
		  cout << "Status indicates an error at line no." << __LINE__ << endl;
		 	cstat.report(status);		
		}
		else {
			cout << PV_WF << " successfully read out " << endl;
			cout << "Elements of WF  have values:" << endl;
			for (int i=0; i<min(25, (int) pvc_h.getNelem() ); ++i) {
					cout << pvc_h.getAsFloat(i) << " [" << i << "] ";
			}
			cout << endl;
		}
		
		
					
		cout << "------------------------------------------------------------" << endl;
		cout << "  END: (3) Waveforms and Arrays " << endl;
		cout << "------------------------------------------------------------" << endl;
		sleep(10);
	
			
		cout << "------------------------------------------------------------" << endl;
		cout << "START: (4) Multiple Scalar Operations " << endl;
		cout << "------------------------------------------------------------" << endl;
		
	  vector<int> statusV;
	  cafe->getScalars(hV, dV, statusV);
		
		for (unsigned int i=0; i<hV.size(); ++i) {
			cout << pvV[i] << " has value " << dV[i] << " and status " << statusV[i] << endl;
		} 
		
		status=cafe->set(hV[0], 3.214);
		
		//or Asynchronous gets
		cafe->get(hV, statusV);
		cafe->getCache(hV, dV, statusV);
		
		
		for (unsigned int i=0; i<hV.size(); ++i) {
			cout << pvV[i] << " has value " << dV[i] << " and status " << statusV[i] << endl;
		} 
	
	  cout <<  "------------------------------------------------------------" << endl;
		cout << "  END: (4) Multiple Scalar Operations " << endl;
		cout << "------------------------------------------------------------" << endl;
		
		
			
		cout << "------------------------------------------------------------" << endl;
		cout << "START: (5) Multiple Compound Operations " << endl;
		cout << "------------------------------------------------------------" << endl;
		
		PVDataHolder pvdArray[NHANDLES];
		for (int i=0; i<NHANDLES; ++i) {
			pvdArray[i].setNelem(cafe->getNelemNative(hArray[i]));
		}
		
		
		
		//input Array of handles
		//cout << "---------------------------------------" << endl;
		//cout << "status=cafe->get(hArray, NHANDLES, pvdArray)" << endl;
		//cout << "---------------------------------------" << endl;
		
		status=cafe->get(hArray, NHANDLES, pvdArray);
	
		
		//input vector of handles
		//cout << "---------------------------------------" << endl;
		//cout << "status=cafe->getPVArray(hV, pvdArray);" << endl;
		//cout << "---------------------------------------" << endl;
		
		status=cafe->getPVArray(hV, pvdArray);
		
		
		//synchornous get
		cout << "---------------------------------------" << endl;
		cout << "status=cafe->get(hArray, NHANDLES, statusArray);" << endl;
		cout << "---------------------------------------" << endl;
		
		
		//Asynchronous get
    int statusArray[NHANDLES];
    status=cafe->get(hArray, NHANDLES, statusArray);

    if (status != ICAFE_NORMAL) {
        //cstat.report(status);
        for (size_t i=0; i<NHANDLES; ++i) {            
            cafe->printStatusIfError(hArray[i],statusArray[i]);
        }
    }
 			
    //now send message and get values
    status=cafe->getCache(hArray, NHANDLES, pvdArray);
    	
		for (size_t i=0; i<NHANDLES; ++i) {
        //if (pvdArray[i].getStatus() != ICAFE_NORMAL) {
            pvdArray[i].print();
        //}
    }

		
		cout << "------------------------------------------------------------" << endl;
		cout << "  END: (5) Mulitple Compound Operations " << endl;
		cout << "------------------------------------------------------------" << endl;
	
	
	
			
		cout << "------------------------------------------------------------" << endl;
		cout << "START: (6) Synchronous Group Operations " << endl;
		cout << "------------------------------------------------------------" << endl;
	
	
    //CA SYNCHRONOUS GROUPS
    //Define a Group, and perform synchronous group
   

    vector<string> pvSeries;
    for (size_t i=0; i<NHANDLES; ++i) {
        pvSeries.push_back((const char *) pvArray[i].c_str());
    }

    cafe->groupDefine((const char *) "gTest", pvSeries);

    unsigned int gHandle;
    try {
        cafe->groupOpen("gTest", gHandle);
    }
    catch(CAFEException_groupOpen &e) {

        cout << e.what() << endl;
        cstat.report(e.groupEx.statusCode);
        exit(1);
    }

    PVGroup pvgroup;
		
    //fill gvgroup with pv parameters from ghandle
		//Tis is optional; will be done by groupGet
    cafe->groupAttach(gHandle, pvgroup); //do not use groupFetch which will set nelem to 1 for wfs!

	
	  PVDataHolder * pvdA = pvgroup.getPVData();	
		pvdA[0].setNelem(1);
		
    cafe->groupGet(gHandle, pvgroup);
		
		//pvgroup.print();
    pvgroup.print(pvgroup.getNPV(),25); // print all members; if wf, max 25 elements
 
 
    pvdA[0].set(1.111);
		double d4[4]= {1,2,3,4};
    pvdA[2].set(d4);
 
      
    status=cafe->groupSet(gHandle, pvgroup);
 
	  cafe->groupGet(gHandle, pvgroup);
	
	
	  //Alternatively...
    for (size_t i=0; i<pvgroup.getNPV(); ++i) {

    	cout << "Name  = "  << pvdA[i].getPV() << endl;
			cout << "Value = "  << pvdA[i].getAsString() << "[0]" << endl;
			cout << "Status= "  << pvdA[i].getStatus() << endl;
			cout << "Nelem = "  << pvdA[i].getNelem() << endl;			    	
    	cout << "Alarm Status  = " << pvdA[i].getAlarmStatus() << endl;
    	cout << "Alarm Severity= " << pvdA[i].getAlarmSeverity() << endl;
    	cout << "Timestamp = " << pvdA[i].getEpicsTimeStamp().secPastEpoch
    	<<  " sec. " << pvdA[i].getEpicsTimeStamp().nsec  <<  " nsec " << endl;
    	cout << endl;
    }
 
 
    cafe->snapshot2XML(pvgroup);
 
 		//cafe->loadSFGroups("VA");
 
 
    cout << "---------------------------" << endl;
  
  
		cout << "------------------------------------------------------------" << endl;
		cout << "  END: (6) Synchronous Group Operations " << endl;
		cout << "------------------------------------------------------------" << endl;
		
		
		cout << "------------------------------------------------------------" << endl;
		cout << "START: (7) Control System Parameters and Channel Information" << endl;
		cout << "------------------------------------------------------------" << endl;
	
		// An example of a single get that retrieves control display parameters
    // Get ctrl limits
    PVCtrlHolder pvc;

    status=cafe->getCtrl(hArray[0], pvc);
		
    if (status==ICAFE_NORMAL) {
        pvc.print(); //just print
        // Access a members explicitly
				cout << "---------------------------------------" << endl;
				cout << "Control Parameters                     " << endl;
				cout << "---------------------------------------" << endl;
				cout << "pv             = "      << pvc.getPV()   << endl; 
				cout << "val (asString) = "      << pvc.getAsString() << endl;
        cout << "units          = "      << pvc.getUnits() << endl; // as char *
				//cout << "units         = "      << pvc.getUnitsAsString() << endl; // as string
        cout << "precision      = " << pvc.getPrecision() << endl;
				vector<std::string> strV = pvc.getEnumStrings();
				for (unsigned int i=0; i< strV.size(); ++i) {
						cout << strV[0] << " " ;				
				}
				if (strV.size() >0) {cout << endl;}
				cout << "AsDouble: " << endl;
				cout << "upperDispLimit   = " <<  pvc.getUpperDispLimit_AsDouble() << endl;
				cout << "lowerDispLimit   = " <<  pvc.getLowerDispLimit_AsDouble() << endl;
				cout << "upperAlarmLimit  = " <<  pvc.getUpperAlarmLimit_AsDouble() << endl;
				cout << "upperWarningLimit= " <<  pvc.getUpperWarningLimit_AsDouble() << endl;
				cout << "lowerWarningLimit= " <<  pvc.getLowerWarningLimit_AsDouble() << endl;
				cout << "lowerAlarmLimit  = " <<  pvc.getLowerAlarmLimit_AsDouble() << endl;
				cout << "upperCtrlLimit   = " <<  pvc.getUpperCtrlLimit_AsDouble() << endl;
				cout << "lowerCtrlLimit   = " <<  pvc.getLowerCtrlLimit_AsDouble() << endl;
				cout << "AsString: " << endl;
				cout << "upperDispLimit   = " <<  pvc.getUpperDispLimit_AsString() << endl;
				cout << "lowerDispLimit   = " <<  pvc.getLowerDispLimit_AsString() << endl;
				cout << "upperAlarmLimit  = " <<  pvc.getUpperAlarmLimit_AsString() << endl;
				cout << "upperWarningLimit= " <<  pvc.getUpperWarningLimit_AsString() << endl;
				cout << "lowerWarningLimit= " <<  pvc.getLowerWarningLimit_AsString() << endl;
				cout << "lowerAlarmLimit  = " <<  pvc.getLowerAlarmLimit_AsString() << endl;
				cout << "upperCtrlLimit   = " <<  pvc.getUpperCtrlLimit_AsString() << endl;
				cout << "lowerCtrlLimit   = " <<  pvc.getLowerCtrlLimit_AsString() << endl;
				cout << "---------------------------------------\n" << endl;
								
    }
    else {
        cafe->printStatusIfError(hArray[0],status);
    }
	
		ChannelRegalia chInfo;
		CAFEDataTypeCode cdt;
		status=cafe->getChannelInfo(hArray[0], chInfo);
		
		cout << "---------------------------------------" << endl;
		cout << "Channel Information                    " << endl;
		cout << "---------------------------------------" << endl;
		cout << "channelID   = " << chInfo.getChannelID() << endl;
		cout << "channelID   = " << cafe->getChannelIDAsString(chInfo.getChannelID()) << endl;
		cout << "connectFlag = " << chInfo.getConnectFlag() << endl; //bool
		cout << "hostName    = " << chInfo.getHostName() << endl; //char *
		cout << "hostName    = " << chInfo.getHostNameAsString() << endl; //string
		cout << "dataType    = " << chInfo.getDataType() << endl;
		cout << "dataType    = " << cdt.asString( chInfo.getDataType() ) << endl;  
		cout << "className   = " << chInfo.getClassName() << endl; //char *
		cout << "className   = " << chInfo.getClassNameAsString() << endl; //string	
		cout << "readAccess  = " << chInfo.getReadAccess() << endl;
		cout << "writeAccess = " << chInfo.getWriteAccess() << endl;
		cout << "nElem       = " << chInfo.getNelem() << endl;
		cout << "connectionState = " << chInfo.getConnectionState() << endl;
		cout << "connectionState = " << chInfo.getConnectionStateAsString() << endl;
		cout << "cafeConnectionState = " << chInfo.getCafeConnectionState() << endl;
		cout << "cafeConnectionState = " << chInfo.getCafeConnectionStateAsString() << endl;
		cout << "---------------------------------------\n" << endl;
		
			
			
			
		cout << "------------------------------------------------------------" << endl;
		cout << "  END: (7) Control System Parameters and Channel Information" << endl;
		cout << "------------------------------------------------------------" << endl;
	
	
		cout << "------------------------------------------------------------" << endl;
		cout << "START: (8) Monitors, either with or without user supplied callbacks " << endl;
		cout << "------------------------------------------------------------" << endl;
	
	  //Simple Monitor  
		//cafe->monitorStart(hArray[0]);
		//or
		//cafe->monitorStart(cafe->getInfo().getHandleFromPV(PV_X1));
	
			
		//Change Policy so that set is synchronous
		ChannelRequestPolicy channelRequestPolicyPut;
		channelRequestPolicyPut.setPolicy(CAFENUM::FLUSH_AFTER_EACH_MESSAGE,
									  CAFENUM::WAIT, CAFENUM::WITH_CALLBACK_DEFAULT); //Wait!!
		
		ChannelRequestPolicy channelRequestPolicyGet;
		channelRequestPolicyGet.setPolicy(CAFENUM::FLUSH_AFTER_EACH_MESSAGE,
									  CAFENUM::WAIT, CAFENUM::WITHOUT_CALLBACK); //Wait!!
			
		ChannelRequestPolicy channelRequestPolicyGetDefault;
		channelRequestPolicyGetDefault.setPolicy(CAFENUM::FLUSH_AFTER_EACH_MESSAGE,
									  CAFENUM::WAIT, CAFENUM::WITH_CALLBACK_DEFAULT); //Wait!!								
		
		status=cafe->getPolicyHelper().setChannelRequestPolicyPut(channelRequestPolicyPut); //for all handles
	  status=cafe->getPolicyHelper().setChannelRequestPolicyPut(hArray[0], channelRequestPolicyPut); //for single handle
		
		int NLOOPV =1;
		// NLOOPV =100000; //double default) 60 secs 2) withoutcallback 54s
		// NLOOPV =100000;  float default) 43 secs 2) withoutcallback 43s
		// NLOOPV =100000;  short default) 51 secs 2) withoutcallback 46s
		// NLOOPV =100000;  short default) 52 secs 2) withoutcallback 56s //change policy to do conversion at ioc
		// NLOOPV =100000;  dbr_char_t default) 52 secs 2) withoutcallback 52s //change policy to do conversion at ioc
		
		//However for Waveforms 4084 elements
		// NLOOPV =10000;  dbr_char_t default) 80 secs 2) withoutcallback 73s //change policy to CAFENUM::NATIVE_DATATYPE
		// NLOOPV =10000;  dbr_char_t default) 64 secs 2) withoutcallback 63s //change policy to CAFENUM::LOWEST_DATATYPE
		
		ChannelRequestDataTypePolicy channelRequestDataTypePolicy;
		channelRequestDataTypePolicy.setRequestKind(CAFENUM::LOWEST_DATATYPE);
		status=cafe->getPolicyHelper().setChannelRequestDataTypePolicy(hArray[1],channelRequestDataTypePolicy);
		
	  dbr_float_t sl[5000];
		system("date");
		status=cafe->getPolicyHelper().setChannelRequestPolicyGet(hArray[1], channelRequestPolicyGetDefault);
		for (int i=0; i<NLOOPV; ++i) {status=cafe->get(hArray[1], sl);}
		system("date");
		status=cafe->getPolicyHelper().setChannelRequestPolicyGet(hArray[1], channelRequestPolicyGet);
		for (int i=0; i<NLOOPV; ++i) {status=cafe->get(hArray[1], sl);}
		system("date");
		
		
		MonitorPolicy mp, mp2;
		mp.setUserArgs((void *) pvArray[0].c_str());
		mp.setHandler(callbackHandlerMonitor);
		mp.setCafeDbrType(CAFENUM::DBR_TIME);
		mp.setDataType(DBR_STS_FLOAT); //THis will overwrite DBR_TIME above
		mp.setMask(DBE_VALUE | DBE_LOG | DBE_ALARM);
		
		mp2.setUserArgs((void *) hArray[0]);
		mp2.setHandler(callbackHandlerMonitor);
		mp2.setCafeDbrType(CAFENUM::DBR_TIME);
		mp2.setDataType(DBR_STS_FLOAT); //THis will overwrite DBR_TIME above
		mp2.setMask(DBE_VALUE | DBE_LOG | DBE_ALARM);
		
		
		cafe->monitorStart(hArray[0], mp);	
		
		cafe->monitorStart(hArray[0], mp2);
	
		
		cafe->set(hArray[0], 1.1023456);
	  usleep(100000); // 0.1s just about long enough to trigger an extra monitor
		
		
		cafe->monitorStop(hArray[0], mp);
		cafe->monitorStop(hArray[0], mp2.getMonitorID()); //Actually, only the monitorID is required 
		
		
				
    //start many Monitors
    cafe->monitorStart(hArray, NHANDLES);
 
     //sleep(1); 
    //get latest value from cache
    status=cafe->getCache(hArray[0],d);  
 
    //get latest value from cache
    status=cafe->getCache(hArray, NHANDLES, pvdArray);
   	
		for (size_t i=0; i<NHANDLES; ++i) {
       //if (pvdArray[i].getStatus() != ICAFE_NORMAL) {
            pvdArray[i].print();
       //}
    }
		
		
		
		//stop many Monitors
    cafe->monitorStop(hArray, NHANDLES);
		
			
		cout << "------------------------------------------------------------" << endl;
		cout << "  END: (8) Monitors, either with or without user supplied callbacks " << endl;
		cout << "------------------------------------------------------------" << endl;
		
		cafe->terminate();
		exit(1);	
			
		cout << "------------------------------------------------------------" << endl;
		cout << "START: (9) Asynchronous interactions and retrieving data from cache " << endl;
		cout << "------------------------------------------------------------" << endl;
	
	  status=cafe->get(hArray[0]);	
		//by array
		status=cafe->get(hArray, NHANDLES, statusV);
		//by vector
		status=cafe->get(hV, statusV);
		
		//cafe->_ca_flush_io();
		cafe->flushNow();
		
		status=cafe->getCache(hArray, NHANDLES, pvdArray);
		
		int NL=10;
		system("date");
		for (int i=0; i<NL; ++i) {
			status=cafe->getCachePVArray(hV,  pvdArray);
		}
		system("date");
	
	  sleep(1); 
		for (size_t i=0; i<NHANDLES; ++i) {      
            pvdArray[i].print();  
    }
		
		cafe->set(hArray[1], 3.333455666);
		cafe->get(hArray[1]);
		cafe->flushNow();
		system("date");
		for (int i=0; i<NL; ++i) {
		status=cafe->getCache(hV,  pvdArray);
		}
	  system("date");
		
		for (size_t i=0; i<NHANDLES; ++i) {      
           pvdArray[i].print();      
    }
	
	  
	  status=cafe->getCache(hV, dV, statusV);
		
		//for (size_t i=0; i<NHANDLES; ++i) {      
    //       cout << "pv " << pvV[i] << " has value " << dV[i] << " and status " << statusV[i] << endl;      
    //}
	
					
		cout << "------------------------------------------------------------" << endl;
		cout << "  END: (9) Asynchronous interactions and retrieving data from cache " << endl;
		cout << "------------------------------------------------------------" << endl;
		
		
		cout << "------------------------------------------------------------" << endl;
		cout << "START: (10) Synchronous group operations with externally defined groups " << endl;
		cout << "------------------------------------------------------------" << endl;
	
			
		cout << "------------------------------------------------------------" << endl;
		cout << "  END: (10) Synchronous group operations with externally defined groups " << endl;
		cout << "------------------------------------------------------------" << endl;
		
				
		cout << "------------------------------------------------------------" << endl;
		cout << "START: (11) Special methods, match, setAndMatch " << endl;
		cout << "------------------------------------------------------------" << endl;
	
	  
	  status=cafe->setAndGet(hWF, 2.2, d);
		cafe->printStatusIfError(hWF, status);
		
		cout << "val should be 2.2 " << d << endl;
	
			
		cout << "------------------------------------------------------------" << endl;
		cout << "  END: (11) Special methods, match, setAndMatch " << endl;
		cout << "------------------------------------------------------------" << endl;
		
				
		cout << "------------------------------------------------------------" << endl;
		cout << "START: (12) Gracefully terminate CAFE " << endl;
		cout << "------------------------------------------------------------" << endl;
	
			
		cout << "------------------------------------------------------------" << endl;
		cout << "  END: (12) Gracefully terminate CAFE " << endl;
		cout << "------------------------------------------------------------" << endl;
		
		cafe->printHandles();
		
		sleep(1);
		cafe->closeHandles(hV);
		cafe->printHandles();
		cafe->closeHandles();
		cafe->groupClose();
		//cafe->groupHandleErase();
		cafe->printHandles();
	
		cafe->terminate();
		exit(0);
		
		
		
    //------------------------------------------------------------
    //(1) Quick Start
    //------------------------------------------------------------

		//status=cafe->get(PV_X1, d);
	

    //cafe->terminate();
		//exit(0);

    //2 write/read wf
    float fArray[256] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    status=cafe->set(PV_WF, fArray);
    if (status!=ICAFE_NORMAL) {
      cstat.report(status);
    }
    //or another way to print an error with pv name
    cafe->printStatusIfError(cafe->getInfo().getHandleFromPV(PV_WF),status);

    //zero array
    std::fill(fArray, fArray+256, 0);

    //ask for 25 elements of the wf
    cafe->getHandleHelper().setNelem(cafe->getInfo().getHandleFromPV(PV_WF),25);

    status=cafe->get(PV_WF, fArray);
    if (status==ICAFE_NORMAL) {
        cout << PV_WF << " is a wf; the first 25 elements have values: " << endl;
        for (size_t i=0; i<25; ++i) {
            cout << fArray[i] << " [" << i << "] "  ;
        }
        cout << endl;
    }
    else {
       //cstat.report(status);
       cafe->printStatusIfError(cafe->getInfo().getHandleFromPV(PV_WF),status);
    }


		status=cafe->set(PV_WF, 3.2);

		double valD;
		status=cafe->get(PV_WF, valD);
		cout << "First Element of WF " << valD << endl;
		
		cout << "Request=" << cafe->getHandleHelper().getNelemRequest(cafe->getInfo().getHandleFromPV(PV_WF)) << endl;
		cout << "Native =" << cafe->getHandleHelper().getNelemNative (cafe->getInfo().getHandleFromPV(PV_WF)) << endl;
		cout << "Client =" << cafe->getHandleHelper().getNelemClient (cafe->getInfo().getHandleFromPV(PV_WF)) << endl;
		
		
		status=cafe->get(PV_WF, fArray);
		
		cout << "Request=" << cafe->getHandleHelper().getNelemRequest(cafe->getInfo().getHandleFromPV(PV_WF)) << endl;
		cout << "Native =" << cafe->getHandleHelper().getNelemNative (cafe->getInfo().getHandleFromPV(PV_WF)) << endl;
		cout << "Client =" << cafe->getHandleHelper().getNelemClient (cafe->getInfo().getHandleFromPV(PV_WF)) << endl;
		
		
    if (status==ICAFE_NORMAL) {
        cout << PV_WF << " is a wf; the first 25 elements have values: " << endl;
        for (size_t i=0; i<cafe->getHandleHelper().getNelemRequest(cafe->getInfo().getHandleFromPV(PV_WF)); ++i) {
            cout << fArray[i] << " [" << i << "] "  ;
        }
        cout << endl;
    }
    else {
       //cstat.report(status);
       cafe->printStatusIfError(cafe->getInfo().getHandleFromPV(PV_WF),status);
    }

		//cafe->terminate();
		//exit(0);

    //3 write/read mbbi

    status=cafe->set(PV_MBBI, "on");
    if (status!=ICAFE_NORMAL) {
        cstat.report(status);
    }
    //or another way to print an error with pv name
    cafe->printStatusIfError(cafe->getInfo().getHandleFromPV(PV_MBBI),status);

		dbr_string_t sData; // i.e. char[40] sData;
    status=cafe->get(PV_MBBI, sData);
    if (status==ICAFE_NORMAL) {
        cout << PV_MBBI << " has value= " << sData << endl;
    }
    else {
        //cstat.report(status);
        cafe->printStatusIfError(cafe->getInfo().getHandleFromPV(PV_MBBI),status);
    }
   
		unsigned short enumValue; //
    status=cafe->get(PV_MBBI, enumValue);
    if (status==ICAFE_NORMAL) {
        cout << PV_MBBI << " has enumValue= " << enumValue << endl;
    }
    else {
        //cstat.report(status);
        cafe->printStatusIfError(cafe->getInfo().getHandleFromPV(PV_MBBI),status);
    }  


    //Get Enum String from Index   
    cout << "String State = " << cafe->getInfo().getStringFromEnum(cafe->getInfo().getHandleFromPV(PV_MBBI), enumValue) << endl;

    cout << "String State = " << cafe->getInfo().getStringFromEnum(cafe->getInfo().getHandleFromPV(PV_MBBI), 88) << endl;
 
    cout << "String State = " << cafe->getInfo().getEnumFromString(cafe->getInfo().getHandleFromPV(PV_MBBI), " on 1") << endl;
    //cafe->terminate();
		//exit(0);

    //4 Quick Start, reading alarmStatus, alarmSeverity, ts
  
    d=0.0; // zero d
    status=cafe->get(cafe->getInfo().getHandleFromPV(PV_X1), d, alarmStatus, alarmSeverity, ets);
    if (status==ICAFE_NORMAL) {
        cout << PV_X1 << " has value= " << d << endl;
        cout << "alarmStatus= " << alarmStatus << " alarmSeverity=" << alarmSeverity
        << " timeStamp= " << ets.secPastEpoch << " sec. " << ets.nsec << " nsec " << endl;
    }
    else {
        cafe->printStatusIfError(cafe->getInfo().getHandleFromPV(PV_X1),status);
    }

    //5 Quick start, same as above but reading strutured data
    //PVDataHolder pvd;  
		//Allocate memory  
    pvd.setNelem(cafe->getInfo().getNelemNative(cafe->getInfo().getHandleFromPV(PV_WF))); //Important, else memory will not be allocated to receive data
    status=cafe->get(PV_WF, pvd);
    //status=cafe->get(cafe->getInfo().getHandleFromPV(PV_WF), pvd);
    if (status==ICAFE_NORMAL) {
        pvd.print(25); //just print first 25 elements of the waveform
        // Access data explicitly
        cout << "Value (string format): " << endl;
        for (size_t i=0; i<25; ++i) {
            cout << pvd.getAsString(i) << " [" << i << "] "  ;
        }
        cout << endl;
        cout << "Value ( float format) " <<  pvd.getAsFloat() << endl;
        for (size_t i=0; i<25; ++i) {
            cout << pvd.getAsFloat(i) << " [" << i << "] "  ;
        }
        cout << endl;
        cout << "Alarm Status  = " << pvd.getAlarmStatus() << endl;
        cout << "Alarm Severity= " << pvd.getAlarmSeverity() << endl;
        cout << "Timestamp = " << pvd.getEpicsTimeStamp().secPastEpoch
        <<  " sec. " << pvd.getEpicsTimeStamp().nsec  <<  " nsec " << endl;
    }
    else {
        cafe->printStatusIfError(cafe->getInfo().getHandleFromPV(PV_WF),status);
    }


    // An example of a single get
    // get string by handle
    status=cafe->get(hArray[0], sData);  //sData declared earlier
    if (status==ICAFE_NORMAL) {
        cout << PV_MBBI << " has value= " << sData << endl;
    }
    else {
        //cstat.report(status);
        cafe->printStatusIfError(cafe->getInfo().getHandleFromPV(PV_MBBI),status);
    }

    // Another example of a single get
    // get string by handle
    dbr_string_t sDesc; //char[40];
    status=cafe->get(hDesc, sDesc);
    if (status==ICAFE_NORMAL) {
        cout << PV_X1_DESC << " has value= " << sDesc << endl;
    }
    else {
        //cstat.report(status);
        cafe->printStatusIfError(hDesc,status);
    }



    // An example of a single get with data, alarms and timestamp
    // Get structured data by handle
    PVDataHolder pvd2;
    pvd2.setNelem(1); //Important if nelements>1,
                      //else memory will not be allocated to receive data

    status=cafe->get(hArray[0], pvd2);
    if (status==ICAFE_NORMAL) {
        pvd2.print(); //just print
        // Access data explicitly
        cout << "Value (string format): "  << pvd2.getAsString()   << endl;
        cout << "Value ( float format) " <<  pvd2.getAsFloat() << endl;
        cout << "Alarm Status  = " << pvd2.getAlarmStatus() << endl;
        cout << "Alarm Severity= " << pvd2.getAlarmSeverity() << endl;
        cout << "Timestamp = " << pvd2.getEpicsTimeStamp().secPastEpoch
        <<  " sec. " << pvd2.getEpicsTimeStamp().nsec  <<  " nsec " << endl;
    }
    else {
        cafe->printStatusIfError(hArray[0],status);
    }

    

    //CA SYNCHRONOUS GROUPS
    //Define a Group, and perform synchronous group
  


    for (size_t kl=0; kl<1; ++kl) {

    
    	//2. ANOTHER WAY TO DO A GET ON A COLLECTIONM OF CHANNELS
    	//2. Gathers gets and send in one call
    	status=cafe->get(hArray, NHANDLES, pvdArray);

    	if (status != ICAFE_NORMAL) {
        cstat.report(status);
    	}

    	for (size_t i=0; i<NHANDLES; ++i) {
        //if (pvdArray[i].getStatus() != ICAFE_NORMAL) {
            pvdArray[i].print(); //Check how it should print enum!
        //}
    	}

    	status=cafe->set(hArray[6],3.3) ; // change one value
    	if (status!=ICAFE_NORMAL) {
        cafe->printStatusIfError(hArray[6],status);
    	}

    	cout << "---------------------------" << endl;

    	//3. Another way: gets and send in one call
    	//3. IN ESSENCE THIS IS THE SAME AS 2. but broken down into 2 parts
    	int statusArray[NHANDLES];
    	status=cafe->get(hArray, NHANDLES, statusArray);

    	if (status != ICAFE_NORMAL) {
        //cstat.report(status);
        for (size_t i=0; i<NHANDLES; ++i) {            
            cafe->printStatusIfError(hArray[i],statusArray[i]);
        }
    	}

    	cout << "---------------------------" << endl;
    	status=cafe->getCache(hArray, NHANDLES, pvdArray);
    	
			for (size_t i=0; i<NHANDLES; ++i) {
        //if (pvdArray[i].getStatus() != ICAFE_NORMAL) {
            pvdArray[i].print();
        //}
    	}

    	cout << "---------------------------" << endl;

    	//sleep(1);
    } //for lpop
    //cafe->printHandles();
   
   
 
  
	  //Start Monitors on Group
    vector<int> statusMonV;
    status=cafe->groupMonitorStart(gHandle, statusMonV); // monitorIDV);


    cout << "MONITOR STARTED " << endl;

    //sleep(1); 
		
		PVDataHolder * pvda=pvgroup.getPVData();
		
		for (int ij=0; ij< pvgroup.getNPV(); ++ij) {	
			//pvda[ij].set(1.1234);//pvda->val[0].d+ij;
			pvda[ij].set((string)"1.012");
		}
		
		
		pvgroup.setPVData(pvda);
		
		status=cafe->groupSet(gHandle, pvgroup);
		
		cout << "GROUP SET WITH status = " << status << endl;
		
		//sleep(1);

    status=cafe->groupGetCache(gHandle, pvgroup);
    if ( status != ICAFE_NORMAL) {
        cout << "STATUS from groupGetCache for group "
                <<  pvgroup.getName() << " is " << pvgroup.getStatusGroup() << "//" << status << endl;
        for (int i=0; i<pvgroup.getNPV(); ++i) {
            if ( pvgroup.getPVData()[i].getStatus() != ICAFE_NORMAL) {
                cout << "Channel: " << pvgroup.getPVData()[i].getPVName() << endl;
                cout << "Status:  " << pvgroup.getPVData()[i].getStatus() << " [" << i << "] of "
                        << pvgroup.getNPV() << endl;
                cstat.report( pvgroup.getPVData()[i].getStatus() );
            }
        }
    }
    pvgroup.print();
		
		
		cout << "STOPPING MONITORS" << endl;
    status=cafe->monitorStop(hArray, NHANDLES);
		
		
		cout << "STOPPING GROUP MONITORS" << endl;
		
    status=cafe->groupMonitorStop(gHandle, statusV);
   	 
	  cout << "status " << status << endl; 
	 	 
	  vector<string> glist;
		glist.clear();
		glist.reserve(50);
	  cafe->groupList      (glist);
	  for (int i=0; i< glist.size(); ++i) {
	     cout << glist[i]  << " " << i << endl;
	  }
	  
	  sleep(5);
	 
	  cafe->groupClose();
		glist.clear();
	
	  cafe->groupList      (glist);
		cout << " size " << glist.size() << endl;
	  for (int i=0; i< glist.size(); ++i) {
	     cout << glist[i]  << " " << i << endl;
	  }
	 
	 
		#if HAVE_PYTHON
		
		cout << "HAVE PYTHON " << endl;
		//dlopen("/opt/gfa/python-3.5/latest/lib/libpython3.5m.so", RTLD_GLOBAL|RTLD_LAZY);
		  Py_Finalize();	
		
		#endif
	
	 
	  sleep(2);
		//terminate cafe
	
		cafe->terminate();
  	sleep(2);
    return 0;
}
