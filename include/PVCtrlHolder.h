///
/// \file    PVCtrlHolder.h
/// \author  Jan Chrin, PSI
/// \date    November 2014
/// \version CAFE 1.0.0
///

#ifndef PVCTRLHOLDER_H
#define PVCTRLHOLDER_H

#include <PVHolder.h>


/**
* \class PVCtrlHolder
* \brief This class is the holder of values associated with the
* EPICS DBR_CTRL_(dataType) control structure of a given handle/pv
*/
class PVCtrlHolder : public PVHolder {

    friend class CAFE;
    friend class Connect;
    friend class Conduit;
    friend struct change_dataBufferPVCtrlHolder;
    friend struct change_dataBufferSize_CTRL;
    friend struct change_connectionHandlerArgs;
    friend struct change_pvAlias;

private:

    short precision;
    char  units[MAX_UNITS_SIZE];

    CAFE_DATATYPE_UNION RISC_pad;
    CAFE_DATATYPE_UNION upperDispLimit;
    CAFE_DATATYPE_UNION lowerDispLimit;
    CAFE_DATATYPE_UNION upperAlarmLimit;
    CAFE_DATATYPE_UNION upperWarningLimit;
    CAFE_DATATYPE_UNION lowerWarningLimit;
    CAFE_DATATYPE_UNION lowerAlarmLimit;
    CAFE_DATATYPE_UNION upperCtrlLimit;
    CAFE_DATATYPE_UNION lowerCtrlLimit;

    Helper helper;
  
    public:
	
    PVCtrlHolder(){
        alarmStatus=0; alarmSeverity=0; precision=0; status=ECA_NORMAL;
        nelem=1; size=1;  rule=true; beamEventNo=0;
		hasAlarm=true;
        noStr=0; dataType=(CAFE_DATATYPE) CAFE_NO_ACCESS;
        dataTypeNative=(CAFE_DATATYPE) CAFE_NO_ACCESS;
        strcpy(pv,""); strcpy(pvAlias,"");strcpy(device,""); strcpy(attrib,""); strcpy(units,"");

        val.reset( new CAFE_DATATYPE_UNION[nelem] );
        val[0].d=0.00;
    };
	
    PVCtrlHolder(unsigned int  _sizeOfArray) {
        alarmStatus=0; alarmSeverity=0; precision=0; status=ECA_NORMAL;
        nelem=_sizeOfArray; size=_sizeOfArray;  rule=true; beamEventNo=0;
		hasAlarm=true;
        noStr=0; dataType=(CAFE_DATATYPE) CAFE_NO_ACCESS;
        dataTypeNative=(CAFE_DATATYPE) CAFE_NO_ACCESS;
        strcpy(pv,""); strcpy(pvAlias,"");strcpy(device,""); strcpy(attrib,"");  strcpy(units,"");

        val.reset( new CAFE_DATATYPE_UNION[nelem] );

        for (unsigned int  i=0; i<nelem; ++i) {
            val[i].d=0.0;
        };
    };

    ~PVCtrlHolder() {val.reset();};

	void setHasAlarm(bool a) {
		hasAlarm=a;
		return;
	};

    unsigned int  setNelem (unsigned int  _nelem) {
        _nelem>0 ? nelem=_nelem : nelem=1;

        if (_nelem>size) {

            size=_nelem;

            val.reset( new CAFE_DATATYPE_UNION[size] );
        }

        return nelem;
    };

    short getPrecision()       const  {return precision;}
	
    const char * getUnits()    const  {return units;}
    string getUnitsAsString()    const  {return (string) units;}
    short getNoEnumStrings ()  const  {return noStr;};
    char * getEnumString(short indx) const {return (char *) strs[indx];};


    vector<std::string>  getEnumStrings() const {

        vector<std::string> vEnumStrings;

        vEnumStrings.reserve(noStr>0?noStr:1);
        for ( short i=0; i<noStr; ++i) {
            vEnumStrings.push_back(strs[i]);
        }
        return vEnumStrings;
    };
	
	
	  short getEnumFromString(string enumString){
				
			 short returnValue=-1;	
		
			 for ( short i=0; i<noStr; ++i) {
			    if (strcmp(enumString.c_str(), strs[i])==0) { 
						return i;
					} 
			 }
			 
			 
			 char pvStripped[MAX_ENUM_STRING_SIZE];
			 helper.removeLeadingAndTrailingSpaces((char *) enumString.c_str(), pvStripped);
			 
			 for ( short i=0; i<noStr; ++i) {
			    if (strcmp(pvStripped, strs[i])==0) { 
						return i;
					} 
			 }
			 cout << "*** WARNING FROM PvCtrlHolder.h ***" << endl;
			 cout << "*** Method getEnumFromString(string enumString) ***" << endl;
			 cout << "The given input string '" << enumString << "' was not recognized! "  << endl;
			 cout << "Valid values are: " << endl;
			 for ( short i=0; i<noStr; ++i) {
					cout << i << ":" << strs[i] << endl;
			}  
				
					 
			 return returnValue;
			 
		}
	
	  string getStringFromEnum(unsigned short enumValue) const{
				
			 string returnValue="";
			 
			 if (enumValue<noStr) {
			 	return (string) strs[enumValue];
			 }	
		   else   {
			  cout << "*** WARNING FROM PvCtrlHolder.h ***" << endl;
				cout << "*** Method getStringFromEnum(unsigned short enumValue) ***" << endl;				
			  cout << "The given input index " << enumValue << " exceeds the number of enum states " << noStr << endl;
				cout << "Valid values are: " << endl;
				for ( short i=0; i<noStr; ++i) {
					cout << i << ":" << strs[i] << endl;
				}  
				
			  if (enumValue<MAX_ENUM_STATES)	{
			 	 	return (string) strs[enumValue];		 
			  }
			  else { 
			 		return returnValue;
			 	}
			}	
		}
	


    CafeDataTypeHelper getRISC_pad()        {CafeDataTypeHelper cdth(RISC_pad, dataType); return cdth;}
    CafeDataTypeHelper getUpperDispLimit()  {CafeDataTypeHelper cdth(upperDispLimit, dataType); return cdth;}
    CafeDataTypeHelper getLowerDispLimit()  {CafeDataTypeHelper cdth(lowerDispLimit, dataType); return cdth;}
    CafeDataTypeHelper getUpperAlarmLimit() {CafeDataTypeHelper cdth(upperAlarmLimit, dataType); return cdth;}
    CafeDataTypeHelper getUpperWarningLimit() {CafeDataTypeHelper cdth(upperWarningLimit, dataType); return cdth;}
    CafeDataTypeHelper getLowerWarningLimit() {CafeDataTypeHelper cdth(lowerWarningLimit, dataType); return cdth;}
    CafeDataTypeHelper getLowerAlarmLimit()   {CafeDataTypeHelper cdth(lowerAlarmLimit, dataType); return cdth;}
    CafeDataTypeHelper getUpperCtrlLimit()    {CafeDataTypeHelper cdth(upperCtrlLimit, dataType); return cdth;}
    CafeDataTypeHelper getLowerCtrlLimit()    {CafeDataTypeHelper cdth(lowerCtrlLimit, dataType); return cdth;}



    string getUpperDispLimit_AsString()    { return getAsString(upperDispLimit); }
    string getLowerDispLimit_AsString()    { return getAsString(lowerDispLimit); }
    string getUpperAlarmLimit_AsString()   { return getAsString(upperAlarmLimit); }
    string getUpperWarningLimit_AsString() { return getAsString(upperWarningLimit); }
    string getLowerWarningLimit_AsString() { return getAsString(lowerWarningLimit); }
    string getLowerAlarmLimit_AsString()   { return getAsString(lowerAlarmLimit); }
    string getUpperCtrlLimit_AsString()    { return getAsString(upperCtrlLimit); }
    string getLowerCtrlLimit_AsString()    { return getAsString(lowerCtrlLimit); }
	
    double getUpperDispLimit_AsDouble()    { return getAsDouble(upperDispLimit); }
    double getLowerDispLimit_AsDouble()    { return getAsDouble(lowerDispLimit); }
    double getUpperAlarmLimit_AsDouble()   { return getAsDouble(upperAlarmLimit); }
    double getUpperWarningLimit_AsDouble() { return getAsDouble(upperWarningLimit); }
    double getLowerWarningLimit_AsDouble() { return getAsDouble(lowerWarningLimit); }
    double getLowerAlarmLimit_AsDouble()   { return getAsDouble(lowerAlarmLimit); }
    double getUpperCtrlLimit_AsDouble()    { return getAsDouble(upperCtrlLimit); }
    double getLowerCtrlLimit_AsDouble()    { return getAsDouble(lowerCtrlLimit); }

    void print() {
        print(nelem) ;
    }

    void print(unsigned int  nelemToPrint) {

        nelemToPrint=min(nelemToPrint,nelem);
        std::cout <<  "------------------------------------------" << std::endl;
        std::cout <<  "PVCtrlHolder:" << std::endl;
	std::cout <<  "processVariable= "  << pv << std::endl;	
        if (strcmp(pvAlias,pv)) {
            std::cout <<  "pvAlias        = "  << pvAlias << std::endl;
        }
        std::cout <<  "device         = "  << device << std::endl;
        std::cout <<  "attrib         = "  << attrib << std::endl;
        std::cout <<  "dataType       = "  << cafeDataTypeCode.message(dataType).c_str()
                << " (" << dataType << ") "  << std::endl;
        std::cout <<  "dbrTypeRequest = "  << dbr_type_to_text(dbrDataType)<< std::endl;
        if (dataType!=CAFE_NO_ACCESS || dataType != CAFE_TYPENOTCONN) {
	std::cout <<  "nelem          = "  << nelem << std::endl;

	std::cout <<  "alarmStatus    = "  << alarmStatus << std::endl;
	std::cout <<  "alarmSeverity  = "  << alarmSeverity << std::endl;
	std::cout <<  "precision      = "  << precision << std::endl;
        std::cout <<  "units          = "  << units << std::endl;
        /*
        if (dataType==CAFE_DOUBLE){
        std::cout <<  "RISC_Pad0      = "  << (dbr_short_t) RISC_pad.d << std::endl
                // in dbr_ctrl_double this is a RISC_pad
        }
        else if (dataType==CAFE_CHAR) {
        std::cout <<  "RISC_Pad       = "  << (unsigned short) RISC_pad.ch << std::endl;
        }
        else if (dataType==CAFE_FLOAT) {
	std::cout <<  "RISC_Pad       = "  << (dbr_short_t) RISC_pad.f << std::endl;
        }
        */
        if (dataType!=CAFE_ENUM) {
            std::cout <<  "upperDispLimit = "  << getAsString(upperDispLimit)    << std::endl;
            std::cout <<  "lowerDispLimit = "  << getAsString(lowerDispLimit)    << std::endl;
            std::cout <<  "upperAlarmLimit= "  << getAsString(upperAlarmLimit)   << std::endl;
            std::cout <<  "upperWarnLimit = "  << getAsString(upperWarningLimit) << std::endl;
            std::cout <<  "lowerWarnLimit = "  << getAsString(lowerWarningLimit) << std::endl;
            std::cout <<  "lowerAlarmLimit= "  << getAsString(lowerAlarmLimit)   << std::endl;
            if(dbr_type_is_CTRL(dbrDataType)) {
                std::cout <<  "upperCtrlLimit = "  << getAsString(upperCtrlLimit)  << std::endl;
                std::cout <<  "lowerCtrlLimit = "  << getAsString(lowerCtrlLimit)  << std::endl;
            }
	} 
	else {
            std::cout <<  "NoStr (ENUM)   = "  << noStr << std::endl;
            std::cout <<  "strs  (ENUM)   = "  ;
        for (short i=0; i< noStr; ++i) {std::cout  << "{" << strs[i] << "} " ;}
            cout <<std::endl;
	}

       std::cout <<  "status         = "  << cafeStatusCode.message(status).c_str() << std::endl;
        if(nelem>0) {std::cout <<  "value(s)       = "  ;}
	
        switch (dataType) {
        case CAFE_STRING:
            for (unsigned int  i=0; i <nelemToPrint; ++i ) {std::cout << val[i].str << " [" << i << "] " ;}
            break;
        case CAFE_SHORT:
            for (unsigned int  i=0; i <nelemToPrint; ++i ) {std::cout << val[i].s   << " [" << i << "] " ;}
            break;
        case CAFE_FLOAT:
            for (unsigned int  i=0; i <nelemToPrint; ++i ) {std::cout << val[i].f   << " [" << i << "] " ;}
            break;
        case CAFE_ENUM:
            for (unsigned int  i=0; i <nelemToPrint; ++i ) {std::cout <<
                getAsString(i) << " (" <<  val[i].us << ")"  << " [" << i << "] " ;}
            break;
        case CAFE_CHAR:
            for (unsigned int  i=0; i <nelemToPrint; ++i ) {std::cout << (unsigned short) val[i].ch  << " [" << i << "] " ;}
            break;
        case CAFE_LONG:
            for (unsigned int  i=0; i <nelemToPrint; ++i ) {std::cout << val[i].l   << " [" << i << "] " ;}
            break;
        case CAFE_DOUBLE:
            for (unsigned int  i=0; i <nelemToPrint; ++i ) {std::cout << val[i].d   << " [" << i << "] " ;}
            break;
	case CAFE_NO_ACCESS:
            default:
            std::cout << "DATA_TYPE NOT YET DEFINED " << endl;
            break;

        }
            std::cout << std::endl;
        }
	std::cout <<  "------------------------------------------" << std::endl;		

    };
};

#endif // PVCTRLHOLDER_H
