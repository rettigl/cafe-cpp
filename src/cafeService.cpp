///
/// \file    cafeService.cpp
/// \author  Jan Chrin, PSI
/// \date    Release July: 2017
/// \version CAFE 1.3.0
///


#include <cafe.h>  
#if HAVE_ZEROMQ
#include <zhelpers.h>
#endif

string globalZmqStream="";
string contentsS="";

unsigned long nCBs=0;

int CAFE::calcDBPMStatus(int statusDATA, int statusVALID, string valueVALID) {

		if (statusDATA != ICAFE_NORMAL) {
			return 	statusDATA;	
		}
	  else if (statusVALID != ICAFE_NORMAL) {
			return 	statusVALID;	
		}		
		else if (	valueVALID.compare("VALID")!=0) {
			//cout << "calcDBPMStatus " << valueVALID << endl;
			return ECAFE_BPM_DATA_IS_INVALID;
		}
				
return ICAFE_NORMAL;

}



int CAFE::prepareDBPM(vector<string> &_glist, vector<unsigned int> &_hV, std::vector<std::string> &dev, std::vector<float> &pos ) {
		int localStatus=ICAFE_NORMAL;
		
		if (!CAFE::isCollection("cDBPM")) {
			localStatus=CAFE::loadCollectionsFromXML("cNodes.xml");		
			if (localStatus != ICAFE_NORMAL) {return localStatus;}								
		}		
		
		
		if (!CAFE::isGroup("gDBPM")) {
			localStatus=CAFE::loadGroupsFromXML("gDBPM.xml");		
			if (localStatus != ICAFE_NORMAL) {return localStatus;}
		}
			
		CAFE::groupMemberList("gDBPM", _glist);
		
		CAFE::devicePositionV("cDBPM", dev, pos);

		/*
		std::map<float, std::string>::iterator iter;
		std::map<float, std::string> posDev;


		CAFE::devicePositionMap("cDBPM", posDev);


		for (iter =posDev.begin(); iter != posDev.end(); ++iter) {
			//cout << "position= " << iter->first << " Dev " << iter->second << endl;
			pos.push_back(iter->first)  ;
			dev.push_back(iter->second) ;
		}
		*/

			
		CAFE::openPrepare();
				 		 
		CAFE::open(_glist,  _hV);	

						
		 CAFE::openNowAndWait(2.0);  //wait for specified time, here 2.0 seconds
		
		//CAFE::printDisconnectedHandles();	
		
	
		/*
		for (int ij=0; ij<5; ++ij) {
		
				
			if (!initCallbackComplete(_hV) ) {
				cout << "Will wait for " << 1+ij << " SECONDS " << endl;
				CAFE::openNowAndWait(1+ij);
			}
		}
		*/
		
	
			
					
		std::size_t found;
		std::string attribute;
		
		vector<int> statusV;

		
		//CAFE::get(_hV, statusV); // do Get just to warm up
		//CAFE::flushNow();
		
		CAFE::openMonitorPrepare();
				
		for (std::size_t i=0; i < _hV.size(); ++i) {
			handleHelper.getChannelAttribute(_hV[i], attribute);
			
			found=attribute.find("VALID");
		
			//if (found != std::string::npos) {	
			 
				if 	(handleHelper.getNmonitorData(_hV[i])==0) {	
					CAFE::monitorStart(_hV[i]);	
					
				}
			//}
		}
			
		//Give plenty of time for all callback functions to report		
		CAFE::openMonitorNowAndWait(1.0);

			
  return localStatus;
}



int CAFE::prepareDBPM(vector<string> &_glist, vector<unsigned int> &_hV, std::map<float, std::string> &posDev) {
		int localStatus=ICAFE_NORMAL;
		
		if (!CAFE::isCollection("cDBPM")) {
			localStatus=CAFE::loadCollectionsFromXML("cNodes.xml");		
			if (localStatus != ICAFE_NORMAL) {return localStatus;}							
		}
			
			
			
				
		if (!CAFE::isGroup("gDBPM")) {
			localStatus=CAFE::loadGroupsFromXML("gDBPM.xml");		
			if (localStatus != ICAFE_NORMAL) {return localStatus;}
		}
	
	
			
		CAFE::groupMemberList("gDBPM", _glist);
		
		CAFE::openPrepare();		 		 
		CAFE::open(_glist,  _hV);	
		
		
		//open BPM Reference
						
    CAFE::openNowAndWait(1.5);  //wait for specified time, here 1.5 seconds
			
		std::size_t found;
		std::string attribute;
		
				
		CAFE::openMonitorPrepare();
				
		for (std::size_t i=0; i < _hV.size(); ++i) {
			handleHelper.getChannelAttribute(_hV[i], attribute);
			
			found=attribute.find("VALID");
		
			//if (found != std::string::npos) {	
			 
				if 	(handleHelper.getNmonitorData(_hV[i])==0) {	
					CAFE::monitorStart(_hV[i]);						
				}
			//}
		}
				
		CAFE::openMonitorNowAndWait(1.5);
		
	
							
		std::map<float, std::string>::iterator pos;
			
		CAFE::devicePositionMap("cDBPM", posDev);				
			
		

		//for (pos =posDev.begin(); pos != posDev.end(); ++pos) {
		//	cout << "position= " << pos->first << " Dev " << pos->second << endl;
		//}

		//	cout << " END " << endl;
  return localStatus;
}


int CAFE::readDBPMOffsets(DBPMKeeper &dbpm) {


	  vector<unsigned int> hx;	
		vector<unsigned int> hy;
			
		vector<string> devOffsetX;
		vector<string> devOffsetY;
		
		devOffsetX.clear();
		devOffsetY.clear();
		devOffsetX.reserve(dbpm.device.size());
		devOffsetY.reserve(dbpm.device.size());
		
		string devOffx="";
		string devOffy="";
		
		for (size_t i=0; i<dbpm.device.size(); ++i) {
			devOffx=dbpm.device[i];
			devOffx.append(":OFFS-X");
			devOffy=dbpm.device[i];
			devOffy.append(":OFFS-Y");
			
			devOffsetX.push_back(devOffx);
			devOffsetY.push_back(devOffy);
		}	
			
		CAFE::openPrepare();	
			
		CAFE::open(devOffsetX, hx);
		CAFE::open(devOffsetY, hy);
		
		CAFE::openNowAndWait(1.0);
		
		vector<int> statusV;
		
				
	
		int status1=CAFE::getScalars(hx, dbpm.offs_x, statusV);
	
		statusV.clear();
		int status2=CAFE::getScalars(hy, dbpm.offs_y, statusV);	
	
		CAFE::closeHandles(hx);
		
		CAFE::closeHandles(hy);	
		
		return max(status1, status2);

}

int CAFE::getDBPM(DBPMKeeper &dbpm) {

 
 if(MUTEX){cafeMutex.lock();}
 	cout << "-------------------------------------------MUTEX LOCKED " << endl;
 
	  dbpm.x.clear();
	  dbpm.y.clear();
		dbpm.q.clear();
		dbpm.energy.clear();
		
		dbpm.x.reserve(dbpm.nDBPM);
	  dbpm.y.reserve(dbpm.nDBPM);
		dbpm.q.reserve(dbpm.nDBPM);		
		dbpm.energy.reserve(dbpm.nDBPM);
		for (size_t i=0; i<dbpm.nDBPM; ++i) {
			dbpm.pvd[dbpm.xIdx+i].set(0);
			dbpm.pvd[dbpm.yIdx+i].set(0);
			dbpm.pvd[dbpm.qIdx+i].set(0);
			dbpm.pvd[dbpm.xValidIdx+i].set((std::string) "INVALID");
			dbpm.pvd[dbpm.yValidIdx+i].set((std::string) "INVALID");
			dbpm.pvd[dbpm.qValidIdx+i].set((std::string) "INVALID");
		}
		
		if (!dbpm.isBS){
		//From Channel Access: 	
			dbpm.status=CAFE::getCachePVArray(dbpm.handle, dbpm.pvd);
		}
		else {
		 
		  #if HAVE_ZEROMQ
			s_dump(dbpm); 
		
		  if (dbpm.status >  ICAFE_LINUX_ERROR) {
			cout << "status from cafeService.cpp -------------------------------------------------> " << dbpm.status << endl;
			}
			#endif
		}
			
		
		
		
		dbpm.isAllXOK=true;
		dbpm.isAllYOK=true;
		dbpm.isAllQOK=true;
		dbpm.isAllEOK=true;
		dbpm.isAllOK =true;
		
		DBPMData ddx, ddy, ddq, dde;
	
		
		for (size_t i=0; i<dbpm.nDBPM; ++i) {
		
			ddx.val=dbpm.pvd[dbpm.xIdx+i].getAsDouble();
			ddx.ets=dbpm.pvd[dbpm.xIdx+i].getEpicsTimeStamp();
			if (!dbpm.isBS) {
			  ddx.status=CAFE::calcDBPMStatus(dbpm.pvd[dbpm.xIdx+i].getStatus(), dbpm.pvd[dbpm.xValidIdx+i].getStatus(), dbpm.pvd[dbpm.xValidIdx+i].getAsString()); 
      }
			else {
			ddx.status=CAFE::calcDBPMStatus(ICAFE_NORMAL, ICAFE_NORMAL, dbpm.pvd[dbpm.xValidIdx+i].getAsString()); 
			
			//cout << " IDX " << dbpm.xValidIdx+i << "STATUS " << dbpm.pvd[dbpm.xValidIdx+i].getAsString() << endl;
			//cout << "ddx.status " << ddx.status << endl;
			
     	}
		 
		 
			if (dbpm.isAllXOK && ddx.status != ICAFE_NORMAL) {
				dbpm.isAllXOK=false;
				dbpm.isAllOK=false;
			}

			
			ddy.val=dbpm.pvd[dbpm.yIdx+i].getAsDouble();
			ddy.ets=dbpm.pvd[dbpm.yIdx+i].getEpicsTimeStamp();
			if (!dbpm.isBS) {
      ddy.status=CAFE::calcDBPMStatus(dbpm.pvd[dbpm.yIdx+i].getStatus(), dbpm.pvd[dbpm.yValidIdx+i].getStatus(), dbpm.pvd[dbpm.yValidIdx+i].getAsString()) ;
			}
			else {
			ddy.status=CAFE::calcDBPMStatus(ICAFE_NORMAL, ICAFE_NORMAL, dbpm.pvd[dbpm.yValidIdx+i].getAsString()) ;
			}
			
			if (dbpm.isAllYOK && ddy.status != ICAFE_NORMAL) {
				dbpm.isAllYOK=false;
				dbpm.isAllOK=false;
			}
			

			ddq.val=dbpm.pvd[dbpm.qIdx+i].getAsDouble();
			ddq.ets=dbpm.pvd[dbpm.qIdx+i].getEpicsTimeStamp();
			
			if (!dbpm.isBS) {
			  ddq.status=CAFE::calcDBPMStatus(dbpm.pvd[dbpm.qIdx+i].getStatus(), dbpm.pvd[dbpm.qValidIdx+i].getStatus(), dbpm.pvd[dbpm.qValidIdx+i].getAsString());
  		}
			else {
			  ddq.status=CAFE::calcDBPMStatus(ICAFE_NORMAL, ICAFE_NORMAL,dbpm.pvd[dbpm.qValidIdx+i].getAsString());
			}
				
			if (dbpm.isAllQOK && ddq.status != ICAFE_NORMAL) {
				dbpm.isAllQOK=false;
				dbpm.isAllOK =false;
			}
		
			dde.val   =dbpm.pvd[dbpm.energyIdx+i].getAsDouble();
			dde.ets   =dbpm.pvd[dbpm.energyIdx+i].getEpicsTimeStamp();
			dde.status=dbpm.pvd[dbpm.energyIdx+i].getStatus();
		
			
			if (dbpm.isAllEOK && dbpm.status != ICAFE_NORMAL) {
				dbpm.isAllEOK=false;
				dbpm.isAllOK=false;
			}
				
			dbpm.x.push_back(ddx);									
			dbpm.y.push_back(ddy);
			dbpm.q.push_back(ddq);
			dbpm.energy.push_back(dde);								
		//}
						
	} 
	
	if(MUTEX){cafeMutex.unlock();}

	cout << "-------------------------------------------MUTEX UNLOCKED " << endl;


return dbpm.status;
}









