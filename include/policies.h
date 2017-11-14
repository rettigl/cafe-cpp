///
/// \file   policies.h
///
/// Policies are: 
/// ChannelCreatePolicy
/// ChannelOpenPolicy
/// ChannelRequestDataTypePolicy
/// ChannelGetCacheWaitPolicy  
/// ChannelGetActionWhenMonitorPolicy
/// ChannelTimeoutPolicy 
/// ChannelRequestPolicy 
/// ChannelMonitorPolicy
///
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef POLICIES_H
#define POLICIES_H

#include <cafeEnum.h>
#include <defines.h>
#include <statusCodes.h>
#include <algorithm>
#include <cstring>
#include <iostream>

//include <config.h> //In the below!
#include <methodCallbacks.h>

//include <enumStrings.h>

using namespace CAFENUM;
using namespace std;

/**
 *  Define Policy for printing messages
 */
class PrintErrorPolicy{
protected:
	bool invalidHandle;
	bool info;
	bool warn;
	bool error;
public:
	bool setInvalidHandle(bool inv) {return invalidHandle=inv;}
	bool setInfo(bool i) {return info=i;}
	bool setWarn(bool w) {return warn=w;}
	bool setError(bool e) {return error=e;}
  bool setAll(bool a) {invalidHandle=a; info=a; warn=a; error=a; return a;}
 	bool getInvalidHandle() {return invalidHandle;}
	bool getInfo()  {return info;}
	bool getWarn()  {return warn;}
	bool getError() {return error;}
	PrintErrorPolicy():invalidHandle(false),info(true),warn(true),error(true){};	
};


/**
 *  Define Policy for creating channel; this just sets the priority level
 *  for dispatch within the server or network
 */
class ChannelCreatePolicy{
private:
    unsigned short priority;
    
    //special method to find handle thru conduit_set iterator
    pCallbackConnection handler;
public:
		static void callbackHandlerCreate(struct connection_handler_args args);
    pCallbackConnection getHandler(){return handler;};
    void setHandler(pCallbackConnection h){handler=h;};
    unsigned short getPriority() const {return priority;}
    unsigned short setPriority(unsigned short p){
        priority=std::min(p,(unsigned short) CA_SERVER_DISPATCH_PRIORITY_MAX);
        return priority;
    }
    ChannelCreatePolicy():priority(CA_SERVER_DISPATCH_PRIORITY_DEFAULT),handler(callbackHandlerCreate){};   
};


/**
 *  Define Policy to open/close/monitor channels one-by-one
 *  or to accumulate messages for later dispatch
 */
class ChannelOpenPolicy{
public:

    //Constructors
    ChannelOpenPolicy():whenKind(FLUSH_AFTER_EACH_CHANNEL_CREATION), flushKind(WITH_PEND_EVENT),
    timeout(DEFAULT_TIMEOUT_PEND_EVENT),defaultTimeout(DEFAULT_TIMEOUT_PEND_EVENT){};

    ChannelOpenPolicy(ChannelFlushSendBufferPolicyKind f,
                      ChannelWhenToFlushSendBufferPolicyKind w, double t){
        if (f>=WITH_PEND_EVENT && f<=WITH_POLL)
        {setFlushSendBufferKind(f);} else {std::cout << f << " is an INVALID ChannelFlushSendBufferPolicyKind" << endl;}
        if (w>=FLUSH_AFTER_EACH_CHANNEL_CREATION && w<=FLUSH_DESIGNATED_TO_CLIENT)
        {setWhenToFlushSendBuffer(w);} else {std::cout << w << " is an INVALID ChannelWhenToFlushSendBufferPolicyKind" << endl;}
        setTimeout(t), setDefaultTimeout(DEFAULT_TIMEOUT_PEND_EVENT);				
    };
    ~ChannelOpenPolicy(){};
private:
    ChannelWhenToFlushSendBufferPolicyKind  whenKind;
    ChannelFlushSendBufferPolicyKind        flushKind;    
    double timeout;
		double defaultTimeout;
public:   
    void flushSendBufferNow() {
        switch(flushKind){
        case WITH_PEND_EVENT:         
            ca_pend_event(timeout);
            break;
        case WITH_PEND_IO:
            ca_pend_io(timeout);
            break;
        case WITH_FLUSH_IO:
            ca_flush_io();
            break;
        case WITH_POLL:
            ca_poll();
            break;
        default:
            ca_pend_event(timeout);
            break;
        }
        //Reset to default
        setWhenToFlushSendBuffer(FLUSH_AFTER_EACH_CHANNEL_CREATION);
    };
    ChannelFlushSendBufferPolicyKind        getFlushSendBufferKind()   const {return flushKind;}
    ChannelWhenToFlushSendBufferPolicyKind  getWhenToFlushSendBuffer() const {return whenKind;}
    double  getTimeout() const {
		//std::cout << "CHANNELOPENPOLICY "  << " *GET* timeout " << timeout << endl;
		return timeout;
		}
		
    double  getDefaultTimeout() const {return defaultTimeout;}
		
    void setFlushSendBufferKind(ChannelFlushSendBufferPolicyKind f){if (f>=WITH_FLUSH_IO && f<=WITH_POLL)
        {flushKind=f;} else {std::cout << f << " is an INVALID ChannelFlushSendBufferPolicyKind" << endl;}};

    void setWhenToFlushSendBuffer(ChannelWhenToFlushSendBufferPolicyKind w) {
        if (w>=FLUSH_AFTER_EACH_CHANNEL_CREATION && w<=FLUSH_DESIGNATED_TO_CLIENT)
        {whenKind=w;} else {std::cout << w << " is an INVALID ChannelWhenToFlushSendBufferPolicyKind" << endl;}};


    double setTimeout(double t) {
		if (t<0){ 
		 std::cout << "CHANNELOPENPOLICY:setTimeout "  << t << " seconds is an illegal value!" << endl;
		 return timeout;
		} 	  
		else if (t==0) {
		 timeout=0.001;
		 std::cout << "CHANNELOPENPOLICY:setTimeout  "  << " A value of zero would block the ioc for ever! "<< endl;
		 std::cout << "CHANNELOPENPOLICY:setTimeout  "  << " Setting timeout to " << timeout << endl;
		 return timeout=0.001;
		}
		return timeout=t;}
		
		double setDefaultTimeout(double t) {
		if (t<0){ 
		 std::cout << "CHANNELOPENPOLICY:setDefaultTimeout "  << t << " seconds is an illegal value!" << endl;
		 return defaultTimeout;
		} 	  
		else if (t==0) {
		 defaultTimeout=0.001;
		 std::cout << "CHANNELOPENPOLICY:setDefaultTimeout "  << " A value of zero would block the ioc for ever! "<< endl;
		 std::cout << "CHANNELOPENPOLICY:setDefaultTimeout "  << " Setting timeout to " << defaultTimeout << endl;
		 return defaultTimeout;
		}
		return defaultTimeout=t;}
		
		double setTimeoutToDefault() {return timeout=defaultTimeout;}
		
    void   setPolicy(ChannelWhenToFlushSendBufferPolicyKind w, ChannelFlushSendBufferPolicyKind f, double t){
        if (f>=WITH_FLUSH_IO && f<=WITH_POLL){flushKind=f;} 
				else {std::cout << f << " is an INVALID ChannelFlushSendBufferPolicyKind" << endl;}
        if (w>=FLUSH_AFTER_EACH_CHANNEL_CREATION && w<=FLUSH_DESIGNATED_TO_CLIENT)
        {whenKind=w;}
        else {std::cout << w << " is an INVALID ChannelWhenToFlushSendBufferPolicyKind" << endl;}
        timeout   =  t;
				defaultTimeout   =  t;
				//std::cout << "CHANNELOPENPOLICY "  << " timeout " << timeout << endl;
    };
};



/**
 *  Define Policy to control datatype conversion for data transfer
 */
class ChannelRequestDataTypePolicy{
private:
    CAFENUM::ChannelRequestDataTypePolicyKind requestKind;
public:
    void setRequestKind(CAFENUM::ChannelRequestDataTypePolicyKind rk) {
        if (rk>=CAFENUM::NATIVE_DATATYPE && rk<=CAFENUM::LOWEST_DATATYPE)
                {requestKind=rk;}
        else {std::cout << rk << " is an INVALID ChannelDataTypePolicyKind" << endl;}
    };

    CAFENUM::ChannelRequestDataTypePolicyKind getRequestKind() const  {return requestKind;}
    //Constructors
    ChannelRequestDataTypePolicy():requestKind(CAFENUM::NATIVE_DATATYPE){};
    ChannelRequestDataTypePolicy(CAFENUM::ChannelRequestDataTypePolicyKind rk){requestKind=rk;};
};



/**
 *  Define Policy to control wait/nowait strategy in getCache operations
 */
class ChannelGetCacheWaitPolicy{
private:
    CAFENUM::ChannelGetCacheWaitPolicyKind getCacheWaitKind;
public:
    void setWaitKind(CAFENUM::ChannelGetCacheWaitPolicyKind wk) {
        if (wk>=CAFENUM::GET_CACHE_NO_WAIT && wk<=CAFENUM::GET_CACHE_WAIT)
                {getCacheWaitKind=wk;}
        else {std::cout << wk << " is an INVALID ChannelGetCacheWaitKind" << endl;}
    };

    CAFENUM::ChannelGetCacheWaitPolicyKind getWaitKind() const  {return getCacheWaitKind;}
    //Constructors
    ChannelGetCacheWaitPolicy():getCacheWaitKind(CAFENUM::GET_CACHE_WAIT){};
    ChannelGetCacheWaitPolicy(CAFENUM::ChannelGetCacheWaitPolicyKind wk){getCacheWaitKind=wk;};
};



/**
 *  Define Policy to control whether the get operations targets the ioc or not when there is a monitor
 */
class ChannelGetActionWhenMonitorPolicy{
private:
    CAFENUM::ChannelGetActionWhenMonitorPolicyKind getActionWhenMonitorKind;
public:
    void setActionKind(CAFENUM::ChannelGetActionWhenMonitorPolicyKind ak) {
        if (ak>=CAFENUM::GET_FROM_CACHE && ak<=CAFENUM::GET_FROM_IOC)
                {getActionWhenMonitorKind=ak;}
        else {std::cout << ak << " is an INVALID ChannelGetActionWhenMonitorKind" << endl;}
    };

    CAFENUM::ChannelGetActionWhenMonitorPolicyKind getActionKind() const  {return getActionWhenMonitorKind;}
    //Constructors
    ChannelGetActionWhenMonitorPolicy():getActionWhenMonitorKind(CAFENUM::GET_FROM_IOC){};
    ChannelGetActionWhenMonitorPolicy(CAFENUM::ChannelGetActionWhenMonitorPolicyKind ak){getActionWhenMonitorKind=ak;};
};



/**
 *  Defines Timeout policy for channel access transactions
 *  and configures the number of transaction attempts in the
 *  event of an ECA_TIMEOUT error; deltaTimeout gives the
 *  increment in timeout for each additional attempt.
 */
class ChannelTimeoutPolicy{
private:
    bool   selfGoverningTimeout;
    double timeout;
    double deltaTimeout;
    unsigned short ntries;
		double defaultTimeout;
public:
    bool   getSelfGoverningTimeout() const {return selfGoverningTimeout;};
 	  void   setSelfGoverningTimeout(bool sgt){selfGoverningTimeout=sgt;};
    double getTimeout()         const {return timeout;};
		double getDefaultTimeout()  const {return defaultTimeout;}
    double getDeltaTimeout()    const {return deltaTimeout;};
    unsigned short getNtries()  const {return ntries;};
    double setTimeout(double t) {timeout=max(t,TIMEOUT_PEND_IO_MIN);
        return timeout=min(timeout,TIMEOUT_PEND_IO_MAX);};
    double setDeltaTimeout(double dt)  { deltaTimeout=max(dt,PEND_IO_INCREMENT_TIME_MIN);
        return deltaTimeout=min(deltaTimeout,PEND_IO_INCREMENT_TIME_MAX);};
    unsigned short setNtries(unsigned short nt) {return ntries=min(nt, PEND_IO_MAX_TRIES);};
		
		double setDefaultTimeout(double t) {return defaultTimeout=t;}
		double setTimeoutToDefault() {return timeout=defaultTimeout;}

    ChannelTimeoutPolicy():selfGoverningTimeout(DEFAULT_SELF_GOVERNING_TIMEOUT),
    timeout(DEFAULT_TIMEOUT_PEND_IO),deltaTimeout(DEFAULT_PEND_IO_INCREMENT_TIME),
    ntries(DEFAULT_PEND_IO_NO_TRIES),defaultTimeout(DEFAULT_TIMEOUT_PEND_IO){};

};


/**
 *  Define Policy to get/set channels whether in blocking/non-blocking mode:
 *  Blocking can be achieved with or without callback
 */
class ChannelRequestPolicy{
private:  

    ChannelWhenToFlushSendBufferPolicyKind  whenKind;      // used for set
    ChannelWaitForResponsePolicyKind waitKind; //only for WITH_CALLBACK  WAIT or NO_WAIT

    ChannelRequestPolicyKind methodKind;

    pCallback handler;    // for blockingKind=WITH_CALLBACK_DEFAULT or WITH_CALLBACK_USER_SUPPLIED
    int  callbackStatus;  // used by CAFE::waitForGetEvent() to record status of callback

public:

    ChannelWhenToFlushSendBufferPolicyKind  getWhenToFlushSendBuffer() const {return whenKind;}
    ChannelWaitForResponsePolicyKind getWaitKind() const {return waitKind;};

    ChannelRequestPolicyKind getMethodKind() const {return methodKind;};

    pCallback getHandler()      const {return handler;};
   
    int  getCallbackStatus()    const {return callbackStatus;};

    void setHandler(pCallback h){if (h!=NULL) {handler=h; methodKind=WITH_CALLBACK_USER_SUPPLIED;}};
      
#if HAVE_PYTHON_H  
		void setPyHandlerGet(){handler= CALLBACK_CAFE::PyHandlerGet; methodKind=WITH_CALLBACK_USER_SUPPLIED;}; //CAFE_CALLBACK::PyHandlerGet
		void setPyHandlerPut(){handler= CALLBACK_CAFE::PyHandlerPut; methodKind=WITH_CALLBACK_USER_SUPPLIED;}; //CAFE_CALLBACK::PyHandlerPut
#endif
			
    void setMethodKind(ChannelRequestPolicyKind m) { if (m>=WITHOUT_CALLBACK && m<=WITH_CALLBACK_USER_SUPPLIED)
        {methodKind=m;} else {std::cout << m << " is an INVALID ChannelRequestPolicyKind" << std::endl;} };

    void setWhenToFlushSendBuffer(ChannelWhenToFlushSendBufferPolicyKind w) {
        if (w>=FLUSH_AFTER_EACH_MESSAGE && w<=FLUSH_DESIGNATED_TO_CLIENT)
        {whenKind=w;} else {std::cout << w << " is an INVALID ChannelWhenToFlushSendBufferPolicyKind" << std::endl;}};

    void setWaitKind(ChannelWaitForResponsePolicyKind r) { if (r>=WAIT && r<=NO_WAIT)
        {waitKind=r;} else {std::cout << r << " is an INVALID ChannelWaitForResponsePolicyKind" << std::endl;}};

    void setCallbackStatus   (int cstatus) {callbackStatus   =cstatus;};

    void setPolicy(
        ChannelWhenToFlushSendBufferPolicyKind w, ChannelWaitForResponsePolicyKind r,
        ChannelRequestPolicyKind m)
    {
        if (w>=FLUSH_AFTER_EACH_MESSAGE && w<=FLUSH_DESIGNATED_TO_CLIENT)
        {whenKind=w;} else {std::cout << "ERROR in setting ChannelRequestPolicy " << std::endl;
            std::cout << w << " is an INVALID ChannelWhenToFlushSendBufferPolicyKind" << endl;
            std::cout << "Sticking to default value ChannelWhenToFlushSendBufferPolicyKind=" << whenKind << std::endl;
        }
        if (r>=WAIT && r<=NO_WAIT)
        {waitKind=r;} else {std::cout << "ERROR in setting ChannelRequestPolicy " << std::endl;
            std::cout << r<< " is an INVALID ChannelWaitForResponsePolicyKind" << endl;
            std::cout << "Sticking to default value ChannelWaitForRespomsePolicyKind=" << waitKind << std::endl;
        }
        if (m>=WITHOUT_CALLBACK && m<=WITH_CALLBACK_USER_SUPPLIED)
        {methodKind=m;} else {std::cout << "ERROR in setting ChannelRequestPolicy " << std::endl;
            std::cout << r<< " is an INVALID ChannelRequestPolicyKind" << endl;
            std::cout << "Sticking to default value ChannelRequestPolicyKind=" << methodKind << std::endl;
        }

        if (methodKind==WITHOUT_CALLBACK && waitKind==NO_WAIT) {
            std::cout << "WARNING when setting ChannelRequestPolicy " << std::endl;
            std::cout << "waitKind=NO_WAIT does not apply when methodKind=WITHOUT_CALLBACK " << std::endl;

        }
    };

    //Constructors  
    ChannelRequestPolicy():
			whenKind(FLUSH_AFTER_EACH_MESSAGE),waitKind(WAIT),methodKind(WITH_CALLBACK_DEFAULT), //WITHOUT_CALLBACK), 
    callbackStatus(ICAFE_NORMAL){
        handler=NULL;
			
    };

    ChannelRequestPolicy(ChannelRequestPolicyKind  b){
        if (b>=WITHOUT_CALLBACK && b<=WITH_CALLBACK_USER_SUPPLIED)
        {methodKind=b;} else {cout << b << " is anINVALID ChannelRequestPolicyKind" << endl;}
        handler=NULL;       
        callbackStatus=ICAFE_NORMAL;
        whenKind=FLUSH_AFTER_EACH_MESSAGE;
        waitKind=WAIT;
    };

    ChannelRequestPolicy(pCallback h){
       handler=h;
       methodKind=WITH_CALLBACK_DEFAULT;
       whenKind=FLUSH_AFTER_EACH_MESSAGE;
       waitKind=WAIT;
       callbackStatus   =ICAFE_NORMAL;
    }

    ~ChannelRequestPolicy(){};
};


/**
 *  Defines policy to start a monitor on a channel
 *  upon first connection. Note that CAFE will not
 *  start a monitor on a channel if it is not connected
 *  as the datatype in such cases is unknown. \n
 *  The monitor is rather placed in a monitor_in_waiting
 *  pseudo-queue and only started upon connection
 */
class MonitorPolicy        
{
    friend class Conduit;
	friend class Connect;
private:
    static unsigned int  idNext;
    chtype        dataType;      //specify primitive datatype for monitoring
    chtype        dbrDataType;   //1
    CAFENUM::DBR_TYPE cafeDbrType;
    unsigned int  nelem;         //2
    //chid          channelID;     //3
    unsigned int  mask;          //4
    pCallback     handler;//5
    void *        userArgs;      //6
    evid          eventID;       //output
    int           status;        //output
    unsigned int  id;

   
	  static void PyCallbackHandlerMonitorData(struct event_handler_args args); //pushes pvd,handle,pvname
	  static void PyCallbackHandlerMonitor(struct event_handler_args args); //pushes handle

    void setEventID(evid e){eventID=e;};

public:
    MonitorPolicy():dataType((CAFE_DATATYPE) CAFE_NOT_REQUESTED),
    dbrDataType((CAFE_DATATYPE) CAFE_NOT_REQUESTED),
	  cafeDbrType((CAFENUM::DBR_TYPE) CAFENUM::DBR_TIME),
	  nelem(0), mask(DBE_VALUE | DBE_LOG | DBE_ALARM),
    handler(callbackHandlerMonitor), userArgs(NULL), eventID(NULL),
    status(ICAFE_NORMAL){
        ++idNext;
        id = idNext;
    };
		//Make public
		static void callbackHandlerMonitor(struct event_handler_args args);
		
    chtype getDataType() const {return dataType;};
    chtype getDbrDataType() const {return dbrDataType;};
    CAFENUM::DBR_TYPE getCafeDbrType() const {return cafeDbrType;};
    unsigned int  getNelem() const {return nelem;};
    //chid getChannelID() const {return channelID;};
    unsigned int  getMask() const {return mask;};

    bool maskHasDBE_PROPERTY() const {bool has=false;
# if (EPICS_MAJOR==3 && EPICS_MINOR>=14 && EPICS_PATCH >=11)
            mask & DBE_PROPERTY ? has=true : has=false;
# endif
        return has;
    }; //8
    bool maskHasDBE_VALUE() const {bool has=false; mask & DBE_VALUE ? has=true : has=false; return has;}; //4
    bool maskHasDBE_LOG() const {bool has=false; mask & DBE_LOG ? has=true : has=false; return has;}; //2
    bool maskHasDBE_ALARM() const {bool has=false; mask & DBE_ALARM ? has=true : has=false; return has;}; //1
    pCallback getHandler() const {return handler;};
    void * getUserArgs() const {return userArgs;};
	//On most platforms pointers and longs are the same size, but ints and pointers often are not the same size
	//on 64bit platforms. If you convert (void*) to (long) no precision is lost, then by assigning the (long) to
	//an (int), it properly truncates the number to fit.
	  unsigned int getUserArgsAsInt() const {return (unsigned int) ((long long) (void *)userArgs);};
    evid getEventID() const {return eventID;};
    unsigned int  getMonitorID() const {return id;};
    int  getStatus() const {return status;};
    unsigned int  getID() const {return id;};


    void setMask(unsigned int  m) {mask=m;};

	  void setPyHandler(){handler= PyCallbackHandlerMonitor;};
	  void setPyHandlerData(){handler= PyCallbackHandlerMonitorData;};
	  void setDefaultHandler(){handler= callbackHandlerMonitor;};

    void setHandler(pCallback h){handler=h;};
    void setNelem(unsigned int  n){nelem=n;};
    void setDataType(chtype dt){ if (dt < DBR_PUT_ACKT) {
            dataType=dt%(LAST_TYPE+1);} else {
            cout << "monitorPolicy FUNNY! " << dt << " is an INVALID DATATYPE! " << endl;
            return;}
        switch(cafeDbrType) {
        case CAFENUM::DBR_TIME:
            dbrDataType=(dbf_type_to_DBR_TIME(dataType));
            break;
        case CAFENUM::DBR_STS:
            dbrDataType=(dbf_type_to_DBR_STS (dataType));
            break;
        case CAFENUM::DBR_PRIMITIVE:
            dbrDataType=(dbf_type_to_DBR     (dataType));
            break;
        case CAFENUM::DBR_CTRL:
            dbrDataType=(dbf_type_to_DBR_CTRL(dataType));
            break;
        case CAFENUM::DBR_GR:
            dbrDataType=(dbf_type_to_DBR_GR  (dataType));
            break;
        default:
            dbrDataType=(dbf_type_to_DBR_TIME(dataType));
        }
    };

    void setCafeDbrType( CAFENUM::DBR_TYPE cdt) {if (cdt > DBR_PUT) {
            cout << "monitorPolicy FUNNY! " << cdt << " is an INVALID CAFENUM::DBR_TYPE! " << endl;
            return;} else { cafeDbrType=cdt;}
				//cout << "monitorPolicy Class: " << " cafeDbrType = " << cafeDbrType << endl;		
				//cout << "setDataType: " <<  dataType << endl;
        switch(cafeDbrType) {
        case CAFENUM::DBR_TIME:
            dbrDataType=(dbf_type_to_DBR_TIME(dataType));
            break;
        case CAFENUM::DBR_STS:
            dbrDataType=(dbf_type_to_DBR_STS (dataType));
            break;
        case CAFENUM::DBR_PRIMITIVE:
            dbrDataType=(dbf_type_to_DBR     (dataType));
            break;
        case CAFENUM::DBR_CTRL:
            dbrDataType=(dbf_type_to_DBR_CTRL(dataType));
            break;
        case CAFENUM::DBR_GR:
            dbrDataType=(dbf_type_to_DBR_GR  (dataType));
            break;
        default:
            dbrDataType=(dbf_type_to_DBR_TIME(dataType));
        }
				//cout << "monitorPolicy Class: " << " dbrDataType = " << dbrDataType << endl;	
    }



    void setUserArgs(void * u){userArgs=u;};
    void setStatus(int s){status=s;};
    void print() {
        cout << "-------------------------------" << endl;
        cout << "Monitor Policy " << endl;
        cout << "-------------------------------" << endl;      
        cout << "dbrDataType = " << dbr_type_to_text(dbrDataType)  << endl;
        cout << "nelem       = " << nelem         << endl;
        cout << "eventID     = " << eventID << endl;
        cout << "monitorID   = " << id << endl;
        cout << "-------------------------------" << endl;
    }
};



#endif // POLICIES_H
