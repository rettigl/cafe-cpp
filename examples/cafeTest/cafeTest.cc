//
//  A simple example
//
#include <cafe.h>

#include "callbacks.h" //ca callback funtions


// Test with 6 PVS
// PV_X1,PV_Y1 are scalar numerics
// PV_WF is a vector numeric
// PV_MBBI is an mbbi record with a number of enum vakues
// PV_X!_DESC is of data type dbr_string_t 
// PV_JOKE is a non-existant channel (used to show CAFE response is such cases)

#define PV_X1      "ARIDI-BPM-01LE:X-AVG"
#define PV_Y1      "ARIDI-BPM-01LE:Y-AVG"
#define PV_WF      "ARIDI-BPM-01LE:WF-INT-1"   //256 elements
#define PV_MBBI    "ARIDI-BPM-01LE:GET-ENABLE" //mmbi
#define PV_X1_DESC "ARIDI-BPM-01LE:X-AVG.DESC" //dbr_string_t
#define PV_JOKE    "PV:JOKE"

#define NHANDLES 6 // should match number of pvs above that we are to open.



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//  (1) Establishing connections to EPICS Process Variables (PVs) 
//  (2) Simple single channel operations (and the PVData Object)
//      Understanding the Status Error Code 
//      Waveforms and Arrays
//  (5) Handling Enumerated Types
//  (6) Multiple scalar operations, i.e., operations on several PVs with scalar values 
//  (7) Multiple'compound' operations, i.e., on several PVS with scalar/vector values
//  (8) Multiple Struct operations on several PVS returning PVData Objects plus Multiple Sets for Scalars/Vectors
//  (9) Asynchronous interactions and retrieving data from Cache
// (10) Synchronous group operations, also with externally defined groups 
// (11) Monitors, either with or without user supplied callbacks (in essence, matlab scripts)
// (12) Control system parameters, i.e., operating limits, engineerimg units
// (13) Retrieve channel medadata, e.g., data type, native no. elements, connection state, read/write access, etc.
// (14) Setting timeouts for set/get operations
// (15) Setting synchronous group timeouts for set/get operations
// (16) Special Methods, e.g., setAndMatch
// (17:END) Gracefully terminate CAFE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



int main( int argc, char *argv[] )
{
    using namespace std;
		
		
    string pvArray[NHANDLES]={PV_X1,PV_Y1,PV_WF,PV_MBBI,PV_X1_DESC,PV_JOKE};
		unsigned int hArray[NHANDLES];
		
		
	  //Instantaite CAFE
										 						
    CAFE  * cafe = new CAFE();
		
		
	  unsigned int handle;
    int status;
		//Class with methods to report on the value and meaning of status codes
    CAFEStatus cstat;
		//Classes defining value/name pairs of alarm status andseverity
		CAFEGlobalAlarmSeverity  alarmSev;
		CAFEGlobalAlarmCondition alarmStat;
		
	
		//------------------------------------------------------------
    //(1) Establishing connections to EPICS Process Variables (PVs)
    //------------------------------------------------------------
		
		cout << "---------------------------------------------------------------------" << endl;
		cout << "START: (1) Establishing connections to EPICS Process Variables (PVs) " << endl;
		cout << "---------------------------------------------------------------------" << endl;
		
  	
		
		// An open operation will pend the ioc for a default amount of time given by:
		// cafe->channelOpenPolicy.getDefaultTimeout() 
		// else otherwise specified.	
		cafe->channelOpenPolicy.setTimeout(0.1); //pend 0.1 seconds
		
		
		// Open one channel
		// It is not an error if the channel is not connected!	 
	  
		try {       		
			cafe->open(pvArray[0].c_str(), handle);			
		}
		catch(CAFEException_open & e) {
		  cout << e.what() << endl;
			exit(1);
		}
	
		
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
		
		  // Vector interfaces also exist
			// vector<string> pvVector
	 	  // vector<unsigned int> hVector
			// cafe->open(pvVector,  hVector, NHANDLES);		
			

      //cafe->openNow() ; // and wait for cafe->channelOpenPolicy.getTimeout()
			//or
      cafe->openNowAndWait(0.5);  //wait for specified time, here 0.5 seconds
			
			
			//Is the given channel connected? Returns true/false
			bool ifYes= cafe->isChannelConnected(hArray[0]); 
			
			//Are all channels connected? Returns true/false
			ifYes= cafe->allChannelsConnected();
			
			
			if (!ifYes) {
      	cafe->printDisconnectedHandles();
			}
	
			//Handle helper functions also provide information on PV handles and their connection states
			status=cafe->getHandleHelper().getDisconnectedHandles(hV, pvV);
			status=cafe->getHandleHelper().getHandles(hV, pvV);
			status=cafe->getHandleHelper().getHandleStates(hV, pvV,stateV);
							
	
		}
		catch(CAFEException_open & e) {
		  cout << e.what() << endl;
		}
		
		
		// To close channels within a given ca_client_context
		
		// To close a single Channel
		// cafe->close(hArray[0]);
		
		// To close many channels
		// cafe->closeChannels(hArray, NHANDLES);
	
	  // To close alls channesl
		// cafe->closeChannels();
			
		// If you wish to close channels that are in other threads 
		// (i.e., with other ca_client_context)
		// use instead the following methods:	
		// To close a single Channel irrespective of ca_client_context
		// i.e. these will also close handles in other threads
		// cafe->closeHandle(hArray[0[);
		
		// To close many handles
		// cafe->closeHandles(hArray, NHANDLES);
	
	  // To close all handles
		// cafe->closeHandles();	
	
		
	
		
		
		
		cout << "---------------------------------------------------------------------" << endl;
		cout << "STOP:  (1) Establishing connections to EPICS Process Variables (PVs) " << endl;
		cout << "---------------------------------------------------------------------" << endl;
		
		
		
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
		//get PV value by handle
		status=cafe->get(hArray[0], d);
			
		if (status != ICAFE_NORMAL) {
		  cout << "Status indicates an error at line no." << __LINE__ << endl;
		 	cstat.report(status);	
			//or
			cafe->printStatus(handle,status);	
		}
		else {
			cout << "Value d=" << d << endl; 
		}
			
		//get PV value, alarm, timeStamp by handle	
		float f; short alarmStatus, alarmSeverity; epicsTimeStamp ets;
		status=cafe->get(handle, f, alarmStatus, alarmSeverity, ets);
			
		if (status != ICAFE_NORMAL) {
		  cout << "Status indicates an error at line no." << __LINE__ << endl;
			cafe->printStatus(hArray[0],status);	
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
		
		
			//To explicitly check on timeout error
			if (cstat.csc.isTimeout(status)) {
				cout << "Above is a timeout error" << endl;	
			}
		
		}	
	
		
		//To interchange between handle and array
		            handle = cafe->getHandleFromPV(pvArray[0].c_str());
		const char * pvName= cafe->getPVFromHandle(handle);
		  
			
		
		
		//------------------------------------------------------------	
		//Data retrieval methods returning structured data
		//------------------------------------------------------------
	
	
			
		PVDataHolder pvd;
	
		pvd.setNelem(cafe->getNelemNative(hArray[2]));
		status=cafe->get(hArray[2], pvd);
		
		if (status==ICAFE_NORMAL) {
		
			pvd.print(10); //Show only 10 elements of the waveform
		
			//The PVDataHolder
			cout << "pvd.getAsString()" << endl;
			for (int i=0; i<min(5,(int) cafe->getNelemRequest(hArray[2])); ++i) {
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
			cout << cstat.severity(pvd.getStatus()) << endl; //Whether WARNING OR ERROR
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
    cafe->printStatusIfError(cafe->getHandleFromPV(PV_X1),status);
		
		cout << "------------------------------------------------------------" << endl;
		cout << "END: (2) Basic Single Channel Operations " << endl;
		cout << "------------------------------------------------------------" << endl;
		
		
		
		//------------------------------------------------------------
    //(3) Waveforms and Arrays
    //------------------------------------------------------------
		
		cout << "------------------------------------------------------------" << endl;
		cout << "START: (3) Waveforms and Arrays  " << endl;
		cout << "------------------------------------------------------------" << endl;
		
				
		//------------------------------------------------------------	
		//Data retrieval methods returning a waveform array
		//------------------------------------------------------------
	
			
		double * dwf = new double[cafe->getNelemNative(hArray[2])]; 
	
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
			for (int i=0; i<std::min(5,(int) cafe->getNelemNative(hArray[2])); ++i) {
					cout << dwf[i] << " [" << i << "] ";
			}
			cout << endl;
		}
			  
	
	  //One may also set the number of elements to retrieve
		//int nelem  =cafe->getHandleHelper().setNelemToNative(hArray[2]);
		int nelem  = cafe->getHandleHelper().setNelem(hArray[2], 10); //returns max(10, cafe->getNelemNative(handle))
		//and offset
		int offset = cafe->getHandleHelper().setOffset(hArray[2], 2);
		//by Handle
		status=cafe->get(hArray[2], dwf);
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
		cout << "Resetting nelem for wf to native value: " << cafe->getHandleHelper().setNelemToNative(hArray[2]) 
		     << " and resetting offset to: " <<  cafe->getHandleHelper().setOffset(hArray[2], 0) << endl;
			
		cafe->get(hArray[2]);
		delete  dwf;
		
			
		//------------------------------------------------------------	
		//Data retrieval methods returning a std::vector
		//------------------------------------------------------------
		vector<float> dV;
	  dbr_short_t alarmSta; dbr_short_t alarmSe; epicsTimeStamp tsta;
		 		
		cout << "nELEM n: " << 	cafe->getHandleHelper().getNelemNative (hArray[2])  << endl;	
		cout << "nELEM r: " << 	cafe->getHandleHelper().getNelemRequest(hArray[2]) << endl;
		cout << "nELEM c: " << 	cafe->getHandleHelper().getNelemClient (hArray[2])  << endl;
		
		cout << "set nelem to 16, get: " << cafe->getHandleHelper().setNelem(hArray[2],16) << endl;;
				
		cout << "nELEM n: " << 	cafe->getHandleHelper().getNelemNative (hArray[2])  << endl;	
		cout << "nELEM r: " << 	cafe->getHandleHelper().getNelemRequest(hArray[2]) << endl;
		cout << "nELEM c: " << 	cafe->getHandleHelper().getNelemClient (hArray[2])  << endl;
				
		//		
		cout << "get nelem cache        get: " << cafe->getHandleHelper().getNelemToRetrieveFromCache(hArray[2]) << endl;				
		cout << "set nelem cache to 12, get: " << cafe->getHandleHelper().setNelemToRetrieveFromCache(hArray[2],12) << endl;
		cout << "get nelem cache        get: " << cafe->getHandleHelper().getNelemToRetrieveFromCache(hArray[2]) << endl;			
		
		//Will retrieve 16 elements	
		status=cafe->get(hArray[2], dV);
		cout << "dV.size " << dV.size() << endl;
		
		//Will retrieve 12 elements from cache
		status=cafe->getCache(hArray[2], dV,   alarmSta, alarmSe, tsta);	
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
								
		cout << "------------------------------------------------------------" << endl;
		cout << "END: (3) Waveforms and Arrays " << endl;
		cout << "------------------------------------------------------------" << endl;
	
	
			
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
		cout << "---------------------------------------" << endl;
		cout << "status=cafe->get(hArray, NHANDLES, pvdArray)" << endl;
		cout << "---------------------------------------" << endl;
		
		status=cafe->get(hArray, NHANDLES, pvdArray);
	
		
		//input vector of handles
		cout << "---------------------------------------" << endl;
		cout << "status=cafe->getPVArray(hV, pvdArray);" << endl;
		cout << "---------------------------------------" << endl;
		
		status=cafe->getPVArray(hV, pvdArray);
		
		
		//Asynchronous get
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
    cafe->groupAttach(gHandle, pvgroup); //also allocates memory for pvgroup members!

	
	  PVDataHolder * pvdA = pvgroup.getPVData();	
		pvdA[0].setNelem(1); //just an example, only required if user wishes to set nelemens for wf from native
		
    cafe->groupGet(gHandle, pvgroup);
		
		// print all group memebers
		// pvgroup.print();
		// print all members; if wf, max 25 elements
    pvgroup.print(pvgroup.getNPV(),25); 
 
 
 
    //Overwrite values for set method 
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
 
 
 		//Will send to /tmp
    cafe->snapshot2XML(pvgroup);
 
  
  
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
	
	  
		
				
		cout << "------------------------------------------------------------" << endl;
		cout << "  END: (7) Control System Parameters and Channel Information" << endl;
		cout << "------------------------------------------------------------" << endl;
	
		cout << "------------------------------------------------------------" << endl;
		cout << "START: (8) Channel Information" << endl;
		cout << "------------------------------------------------------------" << endl;
	
	
	
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
		cout << "  END: (8) Channel Information" << endl;
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
	
	  float fwf;
	  status=cafe->setAndGet(hArray[2],  (dbr_float_t) 1.01, fwf);
		cafe->printStatusIfError(hArray[2], status);
		
		cout << "val should be 1.01 and actually reads " << fwf << endl;
	
			
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
		
		cafe->groupClose();
		cafe->closeHandles();
	
	
		cafe->terminate();
		return 0;
		
	
  
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
	 
	 
}
