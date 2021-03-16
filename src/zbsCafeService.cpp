///
/// \file    zbsCafeService.cpp
/// \author  Jan Chrin, PSI
/// \date    Release July: 2017
/// \version CAFE 1.3.0
///


#include <cafe.h>
#if HAVE_ZEROMQ
#include <zbsread.h>
//endif

using namespace std;


/**
 *  \brief returns a pre-defined CAFE code that summarizes the DBPM state
 *  \param statusDATA input: status of the DBPM data pv
 *  \param statusVALID input: status of the VALID/INVALID pv
 *  \param valueVALID input: valueVALID
 *  \return int ICAFE_NORMAL else ECAFE_BPM_DATA_IS_INVALID
 */
int CAFE::calcDBPMStatus(int statusDATA, int statusVALID, std::string valueVALID)
{

    if (statusDATA != ICAFE_NORMAL)
    {
        return 	ECAFE_BPM_DATA_IS_INVALID; //statusDATA;
    }
    else if (statusVALID != ICAFE_NORMAL)
    {
        return 	ECAFE_BPM_DATA_IS_INVALID; //statusVALID;
    }
    else if (	valueVALID.compare("VALID")!=0)
    {
        //cout << "calcDBPMStatus " << valueVALID << endl;
        return ECAFE_BPM_DATA_IS_INVALID;
    }

    return ICAFE_NORMAL;

}

#if HAVE_LIBQTXML
/**
 *  \brief read DBPM data from XML and start monitors
 *  \param _glist output: vector of DBPM names
 *  \param _hV output: vector of DBPM handles
 *  \param dev output: vector of device names
 *  \param pos output: vector of DBPM positions in z
 *  \return long ICAFE_NORMAL else ECAFE_error
 */
int CAFE::prepareDBPM(std::vector<std::string> &_glist, std::vector<unsigned int> &_hV, std::vector<std::string> &dev, std::vector<float> &pos )
{

    int localStatus=ICAFE_NORMAL;



    if (!CAFE::isCollection("cDBPM"))
    {
        localStatus=CAFE::loadCollectionsFromXML("cNodes.xml");

        if (localStatus != ICAFE_NORMAL)
        {
            return localStatus;
        }
    }

    if (!CAFE::isGroup("gDBPM"))
    {
        localStatus=CAFE::loadGroupsFromXML("gDBPM.xml");
        if (localStatus != ICAFE_NORMAL)
        {
            return localStatus;
        }
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

    //vector<int> statusV;
    //CAFE::get(_hV, statusV); // do Get just to warm up
    //CAFE::flushNow();

    CAFE::openMonitorPrepare();

    for (std::size_t i=0; i < _hV.size(); ++i)
    {
        handleHelper.getChannelAttribute(_hV[i], attribute);
        found=attribute.find("VALID");

        //if (found != std::string::npos) {

        if 	(handleHelper.getNmonitorData(_hV[i])==0)
        {
            CAFE::monitorStart(_hV[i]);
        }
        //}
    }

    //Give plenty of time for all callback functions to report
    CAFE::openMonitorNowAndWait(1.0);


    return localStatus;

}



/**
 *  \brief read DBPM data from XML and start monitors
 *  \param _glist output: vector of DBPM names
 *  \param _hV output: vector of DBPM handles
 *  \param dev output: vector of device names
 *  \param pos output: vector of DBPM positions in z
 *  \return long ICAFE_NORMAL else ECAFE_error
 */
int CAFE::prepareDBPM(std::vector<std::string> &_glist, std::vector<unsigned int> &_hV, std::map<float, std::string> &posDev)
{


    int localStatus=ICAFE_NORMAL;


    if (!CAFE::isCollection("cDBPM"))
    {
        localStatus=CAFE::loadCollectionsFromXML("cNodes.xml");
        if (localStatus != ICAFE_NORMAL)
        {
            return localStatus;
        }
    }

    if (!CAFE::isGroup("gDBPM"))
    {
        localStatus=CAFE::loadGroupsFromXML("gDBPM.xml");
        if (localStatus != ICAFE_NORMAL)
        {
            return localStatus;
        }
    }

    CAFE::groupMemberList("gDBPM", _glist);

    CAFE::openPrepare();
    CAFE::open(_glist,  _hV);

    //open BPM Reference

    CAFE::openNowAndWait(1.5);  //wait for specified time, here 1.5 seconds

    std::size_t found;
    std::string attribute;

    CAFE::openMonitorPrepare();

    for (std::size_t i=0; i < _hV.size(); ++i)
    {
        handleHelper.getChannelAttribute(_hV[i], attribute);

        found=attribute.find("VALID");

        //if (found != std::string::npos) {

        if 	(handleHelper.getNmonitorData(_hV[i])==0)
        {
            CAFE::monitorStart(_hV[i]);
        }
        //}
    }

    CAFE::openMonitorNowAndWait(1.5);

    //std::map<float, std::string>::iterator pos;

    CAFE::devicePositionMap("cDBPM", posDev);


    //for (pos =posDev.begin(); pos != posDev.end(); ++pos) {
    //	cout << "position= " << pos->first << " Dev " << pos->second << endl;
    //}

    //	cout << " END " << endl;
    //#endif
    return localStatus;
}

#endif //if HAVE_XMLQTLIB

/**
 *  \brief read DBPM offsets for given bpms and enter them to DBPMKeeper object
 *  \param dbpm in/output: DBPMKeeper object
 *  \return long ICAFE_NORMAL else ECAFE_error
 */
int CAFE::readDBPMOffsets(CAFEBS::DBPMKeeper &dbpm)
{

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

    for (size_t i=0; i<dbpm.device.size(); ++i)
    {
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



/**
 *  \brief Fill DBPMKeeper object with dbpm data - either from CA or BS
 *  \param dbpm in/output: DBPMKeeper object
 *  \return long ICAFE_NORMAL else ECAFE_error
 */

int CAFE::getDBPM(DBPMKeeper &dbpm)
{

    int HOLE_VALUE=0;

    if(MUTEX)
    {
        cafeMutex.lock();
    }
   
    dbpm.x.clear();
    dbpm.y.clear();
    dbpm.q.clear();
    dbpm.energy.clear();

    dbpm.x.reserve(dbpm.nDBPM);
    dbpm.y.reserve(dbpm.nDBPM);
    dbpm.q.reserve(dbpm.nDBPM);
    dbpm.energy.reserve(dbpm.nDBPM);

    for (size_t i=0; i<dbpm.nDBPM; ++i)
    {
        dbpm.pvd[dbpm.xIdx+i].set(HOLE_VALUE);
        dbpm.pvd[dbpm.yIdx+i].set(HOLE_VALUE);
        dbpm.pvd[dbpm.qIdx+i].set(HOLE_VALUE);
        dbpm.pvd[dbpm.xValidIdx+i].set((std::string) "INVALID");
        dbpm.pvd[dbpm.yValidIdx+i].set((std::string) "INVALID");
        dbpm.pvd[dbpm.qValidIdx+i].set((std::string) "INVALID"); //INVALID

        dbpm.pvd[dbpm.xIdx+i].status=ECAFE_NODATA;
        dbpm.pvd[dbpm.yIdx+i].status=ECAFE_NODATA;
        dbpm.pvd[dbpm.qIdx+i].status=ECAFE_NODATA;
        dbpm.pvd[dbpm.xValidIdx+i].status=ECAFE_NODATA;
        dbpm.pvd[dbpm.yValidIdx+i].status=ECAFE_NODATA;
        dbpm.pvd[dbpm.qValidIdx+i].status=ECAFE_NODATA;
    }

    if (!dbpm.isBS)
    {
        //From Channel Access:

        dbpm.status=CAFE::getCachePVArray(dbpm.handle, dbpm.pvd);

    }
    else
    {

        //Stop Monitors

        //#if HAVE_ZEROMQ
        z_bsread_dbpm(dbpm);

        //if (dbpm.status >  ICAFE_ERRNO_BASE) {
        //    cout << "status from zbsCafeService.cpp -------------------------------------------------> " << dbpm.status << endl;
        //}
        //#endif
    }

    dbpm.isAllXOK=true;
    dbpm.isAllYOK=true;
    dbpm.isAllQOK=true;
    dbpm.isAllEOK=true;
    dbpm.isAllOK =true;

    DBPMData ddx, ddy, ddq, dde;


    for (size_t i=0; i<dbpm.nDBPM; ++i)
    {

        ddx.val=dbpm.pvd[dbpm.xIdx+i].getAsDouble();
        ddx.ets=dbpm.pvd[dbpm.xIdx+i].getEpicsTimeStamp();
      
        ddx.status=CAFE::calcDBPMStatus(dbpm.pvd[dbpm.xIdx+i].getStatus(), dbpm.pvd[dbpm.xValidIdx+i].getStatus(), dbpm.pvd[dbpm.xValidIdx+i].getAsString());
        

        if (dbpm.isAllXOK && ddx.status != ICAFE_NORMAL)
        {
            dbpm.isAllXOK=false;
            dbpm.isAllOK=false;
        }


        ddy.val=dbpm.pvd[dbpm.yIdx+i].getAsDouble();
        ddy.ets=dbpm.pvd[dbpm.yIdx+i].getEpicsTimeStamp();
        
        ddy.status=CAFE::calcDBPMStatus(dbpm.pvd[dbpm.yIdx+i].getStatus(), dbpm.pvd[dbpm.yValidIdx+i].getStatus(), dbpm.pvd[dbpm.yValidIdx+i].getAsString()) ;
       

        if (dbpm.isAllYOK && ddy.status != ICAFE_NORMAL)
        {
            dbpm.isAllYOK=false;
            dbpm.isAllOK=false;
        }


        ddq.val=dbpm.pvd[dbpm.qIdx+i].getAsDouble();
        ddq.ets=dbpm.pvd[dbpm.qIdx+i].getEpicsTimeStamp();

       
        ddq.status=CAFE::calcDBPMStatus(dbpm.pvd[dbpm.qIdx+i].getStatus(), dbpm.pvd[dbpm.qValidIdx+i].getStatus(), dbpm.pvd[dbpm.qValidIdx+i].getAsString());
       
        if (dbpm.isAllQOK && ddq.status != ICAFE_NORMAL)
        {
            dbpm.isAllQOK=false;
            dbpm.isAllOK =false;
        }

        dde.val   =dbpm.pvd[dbpm.energyIdx+i].getAsDouble();
        dde.ets   =dbpm.pvd[dbpm.energyIdx+i].getEpicsTimeStamp();
        dde.status=dbpm.pvd[dbpm.energyIdx+i].getStatus();

        if (dbpm.isAllEOK && dbpm.status != ICAFE_NORMAL)
        {
            dbpm.isAllEOK=false;
            dbpm.isAllOK =false;
        }

        dbpm.x.push_back(ddx);
        dbpm.y.push_back(ddy);
        dbpm.q.push_back(ddq);
        dbpm.energy.push_back(dde);
        

    }

    if(MUTEX)
    {
        cafeMutex.unlock();
    }
    //cout << "-------------------------------------------MUTEX UNLOCKED " << endl;

    return dbpm.status;
}


/**
 *  \brief instructs CAFEBS data retrieval method to initialize with CA meta-data
 *  \param bsd in/output: BSDataHolder object
 *  \return long ICAFE_NORMAL else ECAFE_error
 */
int CAFE::initBSwithCA(CAFEBS::BSDataHolder &bsd)
{
#define __METHOD__ "CAFE::initBSwithCA "
  //cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << endl;

    unsigned int gHandle = 0;
    short overallStatus = ICAFE_NORMAL;

    if (CAFE::isGroup(bsd.grpBS2CA.c_str()) == false)
    {   
      status = CAFE::groupDefine((const char *) bsd.grpBS2CA.c_str(), bsd.pv);
  
      std::vector<unsigned int> gHV;
      gHV.reserve(1);

      try
      {       
	
	bool _pyCafeFlag=false;
  
	if (CAFE::getPyCafe() )
	  {
	    CAFE::setPyCafe(false);
	    _pyCafeFlag=true;
	  }
 
 
          Connect::groupOpenPrepare();
          Connect::groupOpen(bsd.grpBS2CA.c_str(), gHandle);       
          gHV.push_back(gHandle);
          Connect::openGroupNowAndWaitForInputGroups(2.0, gHV);	

	  


	  if (_pyCafeFlag)
	    {
	      CAFE::setPyCafe(true);
	    }


	  std::vector<unsigned int> handlesV = CAFE::getHandlesFromWithinGroupV(gHandle);
	  for (int ij=0; ij<40; ++ij) {
	    if (!initCallbackComplete(handlesV) ) {
	      //cout << "Will wait for " << ij << " " << (1+ij)*0.1 << " ======= SECONDS " << endl;
	      usleep(100000); //tenth of a sec and loop over 4 sec
	    }
	  }
	  
          //CAFE::monitorPulseID();      
      }
      catch(CAFEException_groupOpen &e)
      {
	  cout << e.what() << endl;
          CAFEStatus cstat;
          cstat.report(e.groupEx.statusCode);
          //exit(1);
      }
    }
    
    //else  
    //  {
    //	cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << endl;
    //	cout << "Group alreaddy exists" << endl;	
    //  }
    
    

    if (gHandle == 0)
    {	
     gHandle = handleHelper.getGroupHandleFromGroupName(bsd.grpBS2CA.c_str());
    }
    

    unsigned int handle=0;
    for (unsigned int i=0; i< bsd.pv.size(); ++i)
    {
        bsd.pvd[i].alarmStatus=-1;
        bsd.pvd[i].alarmSeverity=-1;
        bsd.pvd[i].ts.secPastEpoch=0;
        bsd.pvd[i].ts.nsec=0;
        bsd.pvd[i].setPulseID(0);

        handle = handleHelper.getHandleFromPVWithinGroup(bsd.pv[i].c_str(), gHandle);
        bsd.handle[i] = handle;
	bsd.pvd[i].setNelem(CAFE::getNelemNative(handle));
	
        bsd.bsChannel[i].setNelem(CAFE::getNelemNative(handle));

	//std::cout << handle << " is handle within group for pv " << bsd.pv[i] << std::endl;
    }
   
   

    
    return ICAFE_NORMAL;

#undef __METHOD__
}



/** FAKE NRES!  Sep 2020
 *  Comments on opening/closing Synchronous Groups
 *  We found that after 4 open/close groups, monitors from other non-group members would stop(!)
 *  Remedy was to be sure to do a groupGet after each groupOpen.
 *  Otherwise for this purpose we remove groupOpen and stick with just closing Monitors for group members,
 *  i.e., we keep the virtual circuit open!
 */

/**
 *  \brief instruct CAFEBS data retrieval method to use Channel Access
 *  \param bsd in/output: BSDataHolder object
 *  \return long ICAFE_NORMAL else ECAFE_error
 */
int CAFE::setBS2CAGroup(CAFEBS::BSDataHolder &bsd)
{
#define __METHOD__ "CAFE::setBS2CAGroup "
  //cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << endl;
    //Loop around all handles and

    unsigned int gHandle = 0;
    vector<unsigned int> handlesV;
    if (CAFE::isGroup(bsd.grpBS2CA.c_str()) == false)  
    {	
       CAFE::initBSwithCA(bsd);
       gHandle = handleHelper.getGroupHandleFromGroupName(bsd.grpBS2CA.c_str());
       handlesV = CAFE::getHandlesFromWithinGroupV(gHandle);
       
    } 
    else
    {
       gHandle = handleHelper.getGroupHandleFromGroupName(bsd.grpBS2CA.c_str());
       handlesV = CAFE::getHandlesFromWithinGroupV(gHandle);

       bool _pyCafeFlag=false;

       if (CAFE::getPyCafe())
       {
	    CAFE::setPyCafe(false);
	    _pyCafeFlag=true;
       }
       CAFE::openPrepare();	
       CAFE::openChannelsWithHandles(handlesV);	
       CAFE::openNowAndWait(2.0);

       if (_pyCafeFlag)
       {
           CAFE::setPyCafe(true);
       }
 

       //bs.close resets to zero
       unsigned int handle=0;
       
       for (unsigned int i=0; i< bsd.pv.size(); ++i)
	 {
	    bsd.pvd[i].alarmStatus=-1;
	    bsd.pvd[i].alarmSeverity=-1;
	    bsd.pvd[i].ts.secPastEpoch=0;
	    bsd.pvd[i].ts.nsec=0;
	    bsd.pvd[i].setPulseID(0);

	    handle = handleHelper.getHandleFromPVWithinGroup(bsd.pv[i].c_str(), gHandle);
	    bsd.handle[i] = handle;
	    bsd.pvd[i].setNelem(CAFE::getNelemNative(handle));
	    bsd.bsChannel[i].setNelem(CAFE::getNelemNative(handle));
	  
        //std::cout << handle << " is handle within group for pv " << bsd.pv[i] << std::endl;
	 }

    }

    //PVCtrlHolder pvc;
    //int _stat = getCtrlCache(handlesV[0], pvc);
    //pvc.print();  
    for (int ij=0; ij<40; ++ij) {
	    if (!initCallbackComplete(handlesV) ) {
	      //cout << "Will wait for " << ij << " " << (1+ij)*0.1 << " ======= SECONDS " << endl;
	      usleep(100000); //tenth of a sec and loop over 4 sec
	    }
    }
   
   
    bsd.setBS(false);

     //CAFE::monitorPulseID();  Sep2020  
 
    //vector<unsigned int> handlesV = CAFE::getHandlesFromWithinGroupV(gHandle);
    int overallStatus  = ICAFE_NORMAL;
    /***
    CAFE::openMonitorPrepare();
    for (int i=0; i<handlesV.size(); ++i) 
      {	
	CAFE::monitorStart(handlesV[i]);
	
      }
     CAFE::openMonitorNow();
    */

    //monitorGroup
      
    std::vector<int> statusV;
    overallStatus = CAFE::groupMonitorStart(gHandle, statusV);
    //sleep(1); //Need time for all callbacks to be initiated
   



    if (overallStatus != ICAFE_NORMAL)
    {
        CAFEStatus cstat;
        cstat.report(overallStatus);
    }
   
   

    return overallStatus;
#undef __METHOD__
}



/**
 *  \brief instruct CAFEBS data retrieval method to use Channel Access
 *  \param bsd in/output: BSDataHolder object
 *  \return long ICAFE_NORMAL else ECAFE_error
 */
int CAFE::setBS2CA(CAFEBS::BSDataHolder &bsd)
{
#define __METHOD__ "CAFE::setBS2CA "
  //cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << endl;
    bsd.setBS(false);
   
    for (unsigned int i=0; i< bsd.getNPV(); ++i)
    {
      //std::cout << "CLOSING " << bsd.pv[i] << std::endl;
        CAFE::closeChannel(CAFE::getHandleFromPV(bsd.pv[i].c_str()));
    }
   
    bool _pyCafeFlag=false;

    if (CAFE::getPyCafe() )
    {
        CAFE::setPyCafe(false);
        _pyCafeFlag=true;
    }

    CAFE::openPrepare();
    for (unsigned int i=0; i< bsd.getNPV(); ++i)
    {
        unsigned int _handle=0;
        
        CAFE::open(bsd.pv[i],_handle);
        
        bsd.handle[i]=_handle;			
    }
    CAFE::openNowAndWait(2.0);

    if (_pyCafeFlag)
    {
        CAFE::setPyCafe(true);
    }
   
    CAFE::openMonitorPrepare();
    for (unsigned int i=0; i< bsd.getNPV(); ++i)
    {
        //Start monitor if necessary
        if (handleHelper.getNmonitorData(bsd.handle[i])==0)
        {
            int status=CAFE::monitorStart(bsd.handle[i] );
            //std::cout << bsd.pv[i] << " STARTING MONITOR " <<  bsd.handle[i] << std::endl;
            //std::cout << "status = " << status << std::endl;
            CAFE::printStatus(bsd.handle[i], status);
        }
    }
    CAFE::openMonitorNowAndWait(2.2);

   
    return ICAFE_NORMAL;
#undef __METHOD__
}



/**
 *  \brief instruct CAFEBS data retrieval method to use BSRead
 *  \param bsd in/output: BSDataHolder object
 *  \param closeCA input: true means close channel access connections for those channels with BSRead enabled
 *  \return long ICAFE_NORMAL else ECAFE_error
 */
int CAFE::setBS(CAFEBS::BSDataHolder &bsd)
{
#define __METHOD__ "CAFE::setBS(CAFEBS::BSDataHolder &bsd) "

  //cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << endl;

    if (CAFE::isGroup(bsd.grpBS2CA.c_str())== false)
    {
      CAFE::initBSwithCA(bsd);
      
    }
    
    unsigned int gHandle = handleHelper.getGroupHandleFromGroupName(bsd.grpBS2CA.c_str());
    unsigned int handle=0;

    //Initialize
    for (unsigned int i=0; i< bsd.getNPV(); ++i)
    {           
        bsd.pvd[i].alarmStatus=-1;
        bsd.pvd[i].alarmSeverity=-1;
        bsd.pvd[i].ts.secPastEpoch=0;
        bsd.pvd[i].ts.nsec=0;
        bsd.pvd[i].setPulseID(0);
        //std::cout << bsd.bsChannel[i].getName() << " modulo= " <<  bsd.bsChannel[i].getModulo() << std::endl;
        //bs.close resets to zero
	handle = handleHelper.getHandleFromPVWithinGroup(bsd.pv[i].c_str(), gHandle);
	bsd.handle[i] = handle;
	bsd.pvd[i].setNelem(CAFE::getNelemNative(handle));
        bsd.bsChannel[i].setNelem(CAFE::getNelemNative(handle));
    }

     
       //bs.close resets to zero
    

    bool zmqStreamOK=bsd.setBS(true);
    if (!zmqStreamOK) return ECAFE_BSREAD_ZMQSTREAM_NULL;

    
    if (bsd.getNBSEnabled() == bsd.getNPV())
    {
        return ICAFE_NORMAL;
    }
    else
    {
        //std::cout << "/bsd.getNBSEnabled()/= " << bsd.getNBSEnabled() << std::endl;
        //std::cout << "/bsd.getNPV()/=        " << bsd.getNPV() << std::endl;
    }
   

    short sv;
    int stat;

    //IF NO BSREAD!
    vector<unsigned int> handlesV;
    handlesV.reserve(max(1,(int)(bsd.getNPV()-bsd.getNBSEnabled())));

    vector<unsigned int> handleIdxToZero;
    handleIdxToZero.reserve(max(1,(int)(bsd.getNPV()-bsd.getNBSEnabled())));

    vector<std::string> pvs;
    pvs.reserve(max(1,(int)(bsd.getNPV()-bsd.getNBSEnabled())));

    vector<unsigned int> pvsIdx;
    pvsIdx.reserve(max(1,(int)(bsd.getNPV()-bsd.getNBSEnabled())));

    for (unsigned int i=0; i< bsd.getNPV(); ++i)
    {

        if (! ((bsd.getBSChannel(i)).isBSEnabled()) )
        {      
	    handlesV.push_back(bsd.handle[i]);
            pvs.push_back(bsd.pv[i]);
            pvsIdx.push_back(i);
        }
        else if (bsd.handle[i]>0)
        {
          //SEP2020
	  handleIdxToZero.push_back(i);
        }
    }
        
    CAFE::openChannelsWithHandles(handlesV);
  
    std::vector<int> statusV;
    std::vector<unsigned int> monitorIDV;
    status = Connect::monitorStart(handlesV, statusV, monitorIDV);
  
    return status;
#undef __METHOD__
}


/**
 *  \brief terminate BSRead and CA for said channels
 *  \param bsd in/output: BSDataHolder object
 *  \return long ICAFE_NORMAL else ECAFE_error
 */
int CAFE::closeBS(CAFEBS::BSDataHolder &bsd)
{
#define __METHOD__ "CAFE::closeBS(CAFEBS::BSDataHolder &bsd) "
  //std::cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << std::endl;

    short overallStatus = ICAFE_NORMAL;
    unsigned int gHandle = handleHelper.getGroupHandleFromGroupName(bsd.grpBS2CA.c_str());

    bsd.closeBS();
 
    if (CAFE::isGroup(bsd.grpBS2CA.c_str()))
    {
        //close group
        vector<int> statusV;
        overallStatus=CAFE::groupMonitorStop(gHandle, statusV);

        if (overallStatus != ICAFE_NORMAL)
        {
            CAFEStatus cs;
            std::cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << std::endl;
            std::cout << "groupMonitorStop for GROUP " << bsd.grpBS2CA << " has overallStatus: " <<std::endl;
            cs.report(overallStatus);
            for (int i=0; i<statusV.size(); ++i)
            {
                if (statusV[i] != ICAFE_NORMAL)
                {
                    //std::cout << "PV " << bsd.pv[i] <<  " has status " << std::endl;
                    cs.report(statusV[i]);
                }
            }
        }
 
	vector<unsigned int> handlesV = \
	    CAFE::getHandlesFromWithinGroupV(handleHelper.getGroupHandleFromGroupName(bsd.grpBS2CA.c_str()));
        CAFE::closeChannelsKeepHandles(handlesV);

	//CAFE::printHandles();
	      
    }

    
    return overallStatus;

#undef __METHOD__
}



/**
 *  \brief getBS date using zeroMQ channel or CA as requested by setBS or setBS2CA
 *  \param bsd in/output: BSDataHolder object
 *  \return long ICAFE_NORMAL else ECAFE_error
 */
int CAFE::getBS(CAFEBS::BSDataHolder &bsd)
{
#define __METHOD__ "CAFE::getBS(CAFEBS::BSDataHolder &bsd) "

    unsigned int hphase;

   
    if (!bsd.isBS)
    {
        //From Channel Access:


        //using namespace boost::posix_time;
        //ptime timeStart2(microsec_clock::local_time());

        //double  timeElapsed=0;

        bsd.overallStatus=CAFE::getCachePVArray(bsd.handle, bsd.pvd);
	//cout << "pv " << bsd.pv[0] << " " << "  nelem=" << bsd.pvd[0].nelem << endl;
	//cout << "nelem " <<  bsd.bsChannel[0].getNelem() << endl;
	//bsd.pvd[0].print();
        //ptime timeEnd2(microsec_clock::local_time());
        //time_duration duration2(timeEnd2-timeStart2);
        //timeElapsed= (double) duration2.total_microseconds()/1000000.0;

        //std::cout << "PVCacheArray" << timeElapsed << " sec.  "  << std::endl;
        //std::cout << "No of entries " << bsd.getNPV()  << std::endl;

	//JC MARCH 2021 - up to the user
  	   /*
            if (bsd.overallStatus != ICAFE_NORMAL) {
	        std::cout << __FILE__ << "//" << __METHOD__ << "//" << __LINE__ << std::endl;
            	for (size_t i=0; i<bsd.getNPV(); ++i) {
            	  if (bsd.pvd[i].getStatus() != ICAFE_NORMAL) {
		    cout << "pv " << bsd.pv[i] << " " << "  nelem=" << bsd.pvd[i].nelem << " " <<  \
		      bsd.pvd[i].getAsString() << " handle " << bsd.handle[i] << " status " << bsd.pvd[i].getStatus() << endl;
			  }
            	}
            }
	    */
    }
    else
    {
        //#if HAVE_ZEROMQ

        
        using namespace boost::posix_time;
        double  timeElapsed=0;
        ptime timeStart(microsec_clock::local_time());
        //std::cout << "bsread ===========================  " << std::endl;
        
        z_bsread(bsd);

        //std::cout << bsd.getPulse_id() << " " <<  bsd.overallStatus << std:: endl;
        
        ptime timeEnd(microsec_clock::local_time());
        time_duration duration(timeEnd-timeStart);
        timeElapsed= (double) duration.total_microseconds()/1000000.0;
        //std::cout << "time for bsread = " << timeElapsed << std::endl;
        
        if (!bsd.isResourceConnected())
        {

            using namespace boost::posix_time;
            ptime timeStart(microsec_clock::local_time());

            double  timeElapsed=0;
            unsigned int nTries=1;

            ptime timeEnd(microsec_clock::local_time());
            time_duration duration(timeEnd-timeStart);
            timeElapsed= (double) duration.total_microseconds()/1000000.0;

	    double max_connection_time =  BSREAD_MAX_CONNECT_TIME;
	    if (bsd.getNPV() < 20) {
	      max_connection_time = 2.0;
	    }

            while (bsd.overallStatus >  ICAFE_ERRNO_BASE && timeElapsed <= max_connection_time )  
            {
                z_bsread(bsd);
                ++nTries;

                ptime timeEnd(microsec_clock::local_time());
                time_duration duration(timeEnd-timeStart);
                timeElapsed= (double) duration.total_microseconds()/1000000.0;
            }
            //z_bsread(bsd);
            bsd.setResourceConnected(true);
            cout << "Connection took " << nTries << " tries in " << timeElapsed << " sec. Max allowed is " << BSREAD_MAX_CONNECT_TIME << " sec. " << endl;
        }
        if (bsd.overallStatus >  ICAFE_ERRNO_BASE)
        {
            //cout << "status from zbsCafeService.cpp (1) -------------------------------------------------> " << bsd.overallStatus << endl;
            z_bsread(bsd); //Try again after e.g. from CA to BS
        }


        if (bsd.overallStatus >  ICAFE_ERRNO_BASE)
        {
            cout << "status from zbsCafeService.cpp (2) -------------------------------------------------> " << bsd.overallStatus << endl;
        }

        //bsd.bsChannel[0].isBSEnabled();

        //NO BSREAD!
        if (bsd.getNBSEnabled() != bsd.getNPV() )
        {
            for (unsigned int i=0; i< bsd.getNPV(); ++i)
            {
                if (! (bsd.bsChannel[i].isBSEnabled()) )
                {
                    try
                    {
                        CAFE::getCacheFromPulseID(bsd.handle[i], bsd.getPulse_id(), bsd.pvd[i]);

                        if (bsd.overallStatus == ICAFE_NORMAL)
                        {
                            if (bsd.pvd[i].getStatus() != ICAFE_NORMAL)
                            {
                                bsd.overallStatus=bsd.pvd[i].getStatus();
                            }
                        }
                    }
                    catch (std::bad_alloc &e)
                    {
                        std::cout  << __METHOD__ << e.what() << std:: endl;
                        exit(1);
                    }
                }
            }
        }

        //for (size_t i=0; i<bsd.getNPV(); ++i) {
        //	cout << "pv= " << bsd.pv[i] << " handle= " << bsd.handle[i] << " string_val= " << bsd.pvd[i].getAsString() << " int_64_val= " << bsd.pvd[i].getAsLongLong() << endl;
        //}


        //#endif
    }

    //std::cout << bsd.getPulse_id() << " // " <<  bsd.overallStatus << std:: endl;
    return bsd.overallStatus; //ICAFE_NORMAL;
#undef __METHOD__
}

#endif
