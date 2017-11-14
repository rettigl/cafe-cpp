///
/// \file   channelRegalia.h
///
/// Classes are: 
/// ChannelRegalia
/// ChannelRequestMetaData
/// ChannelRequestMetaDataClient
/// ChannelRequestMetaDataRepository
/// ChannelRequestStatus
///
/// \author Jan Chrin, PSI
/// \date   Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef CHANNELINFO_H
#define CHANNELINFO_H

#include <cstring>
#include <statusCodes.h>
#include <defines.h>
#include <cafeEnum.h>

using namespace std;

/**
 *  Struct defining channel properties and connection status for use in Conduit container
 *
 */
class ChannelRegalia{
private:
	CAFEStatus cafeStatus;
public:
    friend struct change_accessRightsHandlerArgs;
    friend struct change_accessRead;
    friend struct change_accessWrite;
    friend struct change_channelID;
    friend struct change_connectionHandlerArgs; 
    friend struct change_eventHandlerArgs;
    //friend struct change_channelRegalia;
    friend class  Connect;
    friend class  HandleHelper;
    friend class  Conduit;
    friend class  ConduitGroup;
protected:
    chid                channelID;              //
    bool                connectFlag;
    const char *              hostName;
    unsigned int        nelem;                  //native
    chtype              dataType;               //native
    unsigned short      accessRead;             //0 or 1
    unsigned short      accessWrite;            //0 or 1
    dbr_class_name_t    className;              //dbr_string_t
    //connectionState as given by connection_handler args: CA_OP_CONN_UP or CA_OP_CONN_DOWN
    int                 connectionState;       //as given by CA_OP_
    int                 cafeConnectionState;   //as given by cafe_cs_state in statusCodes

    //void                setChannelID    (chid id)       {channelID=id;};
    void                setConnectFlag  (bool c)        {connectFlag=c;};
    void                setHostName     (const char * h)      {hostName=h;};
    void                setDataType     (chtype d)      {dataType=d;};
    void                setAccessRead (unsigned short r){accessRead=r;};
    void                setAccessWrite(unsigned short w){accessWrite=w;};
    void                setReadAccess (unsigned short r){accessRead=r;};
    void                setWriteAccess(unsigned short w){accessWrite=w;};
    void                setNelem      (unsigned int  n) {nelem=n;};
    void                setConnectionState     (int cs)   {connectionState=cs;};
    void                setCafeConnectionState (int ccs)  {cafeConnectionState=ccs;};

public:
    chid                getChannelID()  const   {return channelID;};
    bool                getConnectFlag()const   {return connectFlag;};
    const char *        getHostName()   const   {return hostName;};
    string              getHostNameAsString()   {string h= hostName; return h;};
    chtype              getDataType()   const   {return dataType;};
    const char *        getClassName()  const   {return className;};
    string              getClassNameAsString()  {string c=className; return c;};
    unsigned short      getAccessRead() const   {return accessRead;};
    unsigned short      getAccessWrite()const   {return accessWrite;};
    unsigned short      getReadAccess() const   {return accessRead;};
    unsigned short      getWriteAccess()const   {return accessWrite;};
    unsigned int        getNelem()      const   {return nelem;};
    int                 getConnectionState()       const {return connectionState;};
    int                 getCafeConnectionState()   const {return cafeConnectionState;};
	  string              getConnectionStateAsString()       {
		if(connectionState==CA_OP_CONN_UP){return "CA_OP_CONN_UP";}
		else if(connectionState==CA_OP_CONN_DOWN){return "CA_OP_CONN_DOWN"; }		
		else {return "CA_OP_CONN is UNKNOWN: THIS SHOULD NEVER APPEAR!";}};

	string              getCafeConnectionStateAsString()    {
		return cafeStatus.csc.message(cafeConnectionState);};

    ChannelRegalia():channelID((chid) NULL), connectFlag(false), nelem((unsigned int) 1),
	dataType((chtype) CAFE_TYPENOTCONN),
    accessRead((unsigned short) 0), accessWrite((unsigned short) 0),
    connectionState((int) CA_OP_CONN_DOWN), cafeConnectionState((int) ICAFE_CS_NEVER_CONN)
    {strcpy(className, ""); hostName="";};

    ~ChannelRegalia(){};
};


/**
 *  Stores strings for Enum datatypes
 *
 */
class ChannelEnumStrings{
public:
    friend struct change_eventHandlerArgs;
private:
    char  options [MAX_ENUM_STATES][MAX_ENUM_STRING_SIZE];
    short noOptions;
public:
    ChannelEnumStrings():noOptions( (short) 0){};
    short getNoOptions() const {return noOptions;};
    char  getOptions() const {return options[MAX_ENUM_STATES][MAX_ENUM_STRING_SIZE];};
};


/**
 *  Stores device/attribute pairing
 *
 */
class ChannelDeviceAttribute{
private:
    std::string pv;
    std::string device;
    std::string attribute;
    std::string deliminator;
public:
    ChannelDeviceAttribute(){};
    ~ChannelDeviceAttribute(){};

    int  init(std::string _pv, std::string _deliminator)
    {
        pv=_pv;
        deliminator=_deliminator;
        short posOfSeparator=pv.find_first_of(deliminator);
        if (posOfSeparator<0){
            device="";
            attribute="";
            return ECAFE_DEVICE_ATTRIB_NOT_FOUND;
        }
        else {
            device= pv.substr(0,posOfSeparator);
            attribute=pv.substr(posOfSeparator+1,pv.size());
        }
        return ICAFE_NORMAL;
    };

    const char * getDeliminator() const {return deliminator.c_str();};
    const char * getDevice()      const  {return device.c_str();};
    const char * getAttribute()   const  {return attribute.c_str();};
    const char * getAttrib()      const  {return attribute.c_str();};
    std::string getDeliminatorAsString() const {return deliminator;};
    std::string getDeviceAsString()      const {return device;};
    std::string getAttributeAsString()   const {return attribute;};
};


/**
 *   0. Struct defining channel datatype/nelem executed by CAFE for use in Conduit container
 *
 */
class ChannelRequestMetaData{
public:
    friend struct change_connectionHandlerArgs;
    friend struct change_dataBufferSize_CTRL;
    friend struct change_dataBufferSize_TIME;
    friend struct change_dataBufferSize_PRIMITIVE;
    friend struct change_dataBufferSize_STSACK;
    friend struct change_eventHandlerArgs;

    friend struct copy_channelRequestMetaDataToChannelRequestMetaDataRepository;

    friend class  Conduit;
    friend class  ConduitGroup;
    
protected:
    chid                channelID;              //of requested item
    unsigned int        nelem;                  //depends on Policy
    unsigned int        nelemCache;             //nelem To Retrieve From Cache
    chtype              dataType;               //depends on Policy
    chtype              dbrDataType;            //depends on Policy
    CAFENUM::DBR_TYPE   cafeDbrType;
    void *              usrArg;                 //from Conduit.usrArg
    unsigned int        byteSize;               //data buffer (bytes) must be large enough to store data
    unsigned int        offset;

public:
	void setNelem(unsigned int  n){nelem= n > 0 ? n : 1;
		//nelemCache= nelem > nelemCache ? nelemCache : nelem;
	}; // byteSize=dbr_size_n(dbrDataType,nelem); };
    void setNelemCache(unsigned int  n){nelemCache= n > 0 ? n : 1;}
    void setUsrArg(void * u){usrArg=u;};
    void setDataType(chtype d){dataType=d;};
    void setDbrDataType(chtype dbr){dbrDataType=dbr; }; //byteSize=dbr_size_n(dbrDataType,nelem);};
    void setCafeDbrType(CAFENUM::DBR_TYPE cd){cafeDbrType=cd;};
    void setDbrTypesFromCafeDbrType(CAFENUM::DBR_TYPE cd){
        cafeDbrType=cd;
        switch(cafeDbrType)
        {
            case CAFENUM::DBR_TIME:
                dbrDataType = dbf_type_to_DBR_TIME(dataType);
                break;
            case CAFENUM::DBR_STS:
                dbrDataType = dbf_type_to_DBR_STS(dataType);
                break;
            case CAFENUM::DBR_PRIMITIVE:
                dbrDataType = dbf_type_to_DBR(dataType);
                break;
            default:
				//Print Warning Message?
                dbrDataType = dbf_type_to_DBR_TIME(dataType);
                cafeDbrType = CAFENUM::DBR_TIME;
                break;
        }
    };


    void setByteSize(unsigned int  b){ byteSize=b;};

    unsigned int  getNelem() const{return nelem;};
    unsigned int  getNelemCache() const{return nelemCache;};
    void * getUsrArg() const{return usrArg;};
    chtype getDataType() const { return dataType;};
    chtype getDbrDataType() const { return dbrDataType;};
    CAFENUM::DBR_TYPE getCafeDbrType() const {return cafeDbrType;};

    unsigned int  getByteSize() const {return byteSize;};

    void setOffset(unsigned int o) {offset=o;};
    unsigned int getOffset() const {return offset;};

    //Constructors
    ChannelRequestMetaData():channelID((chid) NULL), nelem((unsigned int) 1), nelemCache((unsigned int) 1),
    dataType((chtype) DBF_NO_ACCESS), dbrDataType((chtype) TYPENOTCONN), //
    cafeDbrType( (CAFENUM::DBR_TYPE) NULL),
    usrArg((void *) NULL), byteSize((unsigned int) 0), offset((unsigned int) 0){

    }; //CAStatus((long) ECA_NORMAL){};

    ~ChannelRequestMetaData(){};
};


/**
 *  -1. Struct defining channel datatype/nelem requested by client for use in Conduit container
 *
 */
class ChannelRequestMetaDataClient: public ChannelRequestMetaData{
public:
  
//protected:
    //unsigned int        offset;
public:    
    //void setOffset(unsigned int o) {offset=o;};
    //unsigned int getOffset() const {return offset;};
   
    //Constructors
    ChannelRequestMetaDataClient(){};//:offset((unsigned int) 0){};
    ~ChannelRequestMetaDataClient(){};
};


/**
 *  Gives status of the Request message \n
 *  messageStatus = requestStatus if requestStatus != ECA_NORMAL else \n
 *  messageStatus = requestStatus or messageStatus=callbackStatus (if policy WITH_CALLBACK)
 *
 */
class ChannelRequestStatus{

private:
    int  preRequestStatus;  // current status of channel
    int  requestStatus;     // from get/put operation
    int  pendStatus;        // from pend_io operation
    int  callbackStatus;    // used by CAFE::waitForEvent() to record status of callback
    int  messageStatus;     // overall status
    CAFENUM::StatusMessageKind messageKind;  // statusMessageKind indicating method status occuping overall messageStatus

    bool hasCallbackInitiated;
    bool hasCallbackTriggered;
    CAFENUM::CallbackProgressKind callbackProgressKind;

public:
    int  getPreRequestStatus()  const {return preRequestStatus;};
    int  getRequestStatus()     const {return requestStatus;};
    int  getPendStatus()        const {return pendStatus;};
    int  getCallbackStatus()    const {return callbackStatus;};
    int  getMessageStatus()     const {return messageStatus;};
    CAFENUM::StatusMessageKind    getMessageKind()           const {return messageKind;};
    CAFENUM::CallbackProgressKind getCallbackProgressKind()  const {return callbackProgressKind;};

    void setPreRequestStatus (int s) {preRequestStatus=s; messageStatus=s; messageKind=(CAFENUM::StatusMessageKind) CAFENUM::PRE_REQUEST;};
    void setRequestStatus    (int r) {requestStatus = r;  messageStatus=r; messageKind=(CAFENUM::StatusMessageKind) CAFENUM::FROM_REQUEST;};
    void setPendStatus       (int p) {pendStatus    = p;  messageStatus=p; messageKind=(CAFENUM::StatusMessageKind) CAFENUM::FROM_PEND;};
    void setCallbackStatus   (int c) {callbackStatus= c;  messageStatus=c; messageKind=(CAFENUM::StatusMessageKind) CAFENUM::FROM_CALLBACK;};
    void setCallbackKind     (bool hasInit, bool hasTrig) {
        hasCallbackInitiated=hasInit;   hasCallbackTriggered=hasTrig;
        if      ( hasInit && !hasTrig) {callbackProgressKind=(CAFENUM::CallbackProgressKind) CAFENUM::PENDING;}
        else if (!hasInit &&  hasTrig) {callbackProgressKind=(CAFENUM::CallbackProgressKind) CAFENUM::COMPLETE;}
        else if (!hasInit && !hasTrig) {callbackProgressKind=(CAFENUM::CallbackProgressKind) CAFENUM::NOT_INITIATED;}
        else {std::cout << "CAFE INTERNAL POLICY ERROR" << std::endl;
              std::cout << "ChannelRequestStatus::setCallbackKind gives an INVALID callbackProgressKind" << endl;}
    };
    //void setMessageStatus    (long mstatus) {messageStatus = mstatus;};
    // void setMessageKind      (StatusMessageKind mkind) { if (mkind<=CAFENUM::FROM_CALLBACK && mkind >= CAFENUM::PRE_REQUEST)
    //   {messageKind = mkind;} else {cout<< mkind << " is an invalid statusMessageKind!" << endl;}};

    ChannelRequestStatus():preRequestStatus(ICAFE_CS_NEVER_CONN),requestStatus(ICAFE_CS_NEVER_CONN),pendStatus(ICAFE_CS_NEVER_CONN),
    callbackStatus(ICAFE_CS_NEVER_CONN),messageStatus(ICAFE_CS_NEVER_CONN),messageKind((CAFENUM::StatusMessageKind) CAFENUM::NO_MESSAGE),
    hasCallbackInitiated(false),hasCallbackTriggered(false),callbackProgressKind((CAFENUM::CallbackProgressKind) CAFENUM::NOT_INITIATED){};
};

#endif // CHANNELINFO_H
