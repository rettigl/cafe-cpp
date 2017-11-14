///
/// \file    PVDataHolder.h
/// \author  Jan Chrin, PSI
/// \date    November 2014
/// \version CAFE 1.0.0
///

#ifndef PVDATAHOLDER_H
#define PVDATAHOLDER_H

#include <PVHolder.h>

/**
* \class PVDataHolder
* \brief This class is the holder of data values associated with
* the EPICS DBR_TIME_(dataType) structure of a given handle/pv
*/
class PVDataHolder : public PVHolder {

    friend class CAFE;
    friend class PVGroup;
    friend class Connect;
    friend class Conduit;
		//if HAVE_LIBQTXML
    friend class loadCollectionXMLParser;
    friend class restorePVGroupXMLParser;
		//endif
    friend struct change_dataBufferPVDataHolder;
    friend struct change_dataBufferSize_TIME;
    friend struct change_connectionHandlerArgs;
    friend struct change_pvAlias;

//private:
    //epicsTimeStamp   ts;
	  //bool hasTS;



public:

		epicsTimeStamp   ts;
	  bool hasTS;


    //Derived class does not inherit constructors
    PVDataHolder(unsigned int  _sizeOfArray) {

        alarmStatus=0; alarmSeverity=0; status=ECAFE_NODATA;
        nelem= _sizeOfArray > 0 ? _sizeOfArray : 1;
        size = _sizeOfArray > 0 ? _sizeOfArray : 1;

        dataType=(CAFE_DATATYPE) CAFE_NO_ACCESS;
        dataTypeNative=(CAFE_DATATYPE) CAFE_NO_ACCESS;
        rule=true; beamEventNo=0; userNo=0; ts.nsec=0; ts.secPastEpoch=0;
		    hasAlarm=true; hasTS=true;
        strcpy(pv,""); strcpy(pvAlias,""); strcpy(device,""); strcpy(attrib,"");

        val.reset( new CAFE_DATATYPE_UNION[nelem] );

        for (unsigned int  i=0; i<nelem; ++i) {
            val[i].d=0.0;
        };
    };

    PVDataHolder(){

        alarmStatus=0; alarmSeverity=0; status=ECAFE_NODATA; nelem=1; size=1;
        dataType=(CAFE_DATATYPE) CAFE_NO_ACCESS;
        dataTypeNative=(CAFE_DATATYPE) CAFE_NO_ACCESS;
        rule=true; beamEventNo=0;  userNo=0; ts.nsec=0; ts.secPastEpoch =0;
		    hasAlarm=true; hasTS=true;
        strcpy(pv,""); strcpy(pvAlias,""); strcpy(device,""); strcpy(attrib,"");

        val.reset( new CAFE_DATATYPE_UNION[nelem] );

        val[0].d=0.00;
    };
					
    ~PVDataHolder() {

         val.reset();
    };

	void valReset() {

		val.reset();
	}


	void setHasAlarm(bool a) {
		hasAlarm=a;
		return;
	};

	void setHasTS(bool t){
		hasTS=t;
		if (t) {hasAlarm=t;} //TS will also retrieve alarmStatus
		return;
	}

	bool getHasTS(){
		return hasTS;
	}

    unsigned int  setNelem (unsigned int  _nelem) {

        _nelem>0 ? nelem=_nelem : nelem=1;

		if (nelem>size) {		
			size=nelem;
            val.reset( new CAFE_DATATYPE_UNION[size] );		
        }

        return nelem;
    };

    epicsTimeStamp getEpicsTimeStamp() const {return ts;};

    struct etsNorm{ unsigned int secPastEpoch; unsigned int nsec;} _etsNorm;
    struct etsDate{ unsigned short year; unsigned short mon; unsigned short day;
                    unsigned short hour; unsigned short min; unsigned short sec; unsigned int nsec;} _etsDate ;

    etsNorm  getEpicsTimeStampAsUInt32() {
        _etsNorm.secPastEpoch=ts.secPastEpoch;
        _etsNorm.nsec=(unsigned long) ts.nsec;
        return _etsNorm;};

    etsDate  getEpicsTimeStampAsDate() {

		ts.nsec=(unsigned int) ts.nsec;

		//This may happen in timeouts; epicsTime convertor will report overflow error
		//However this possibility is now captured in conduitFriend.h and other
		if(ts.nsec >= 1000000000) {
			cout << "OVERFLOW IN gets.nsec CORRECTED for epicsTime converter " << endl; ts.nsec=0;
		}

        epicsTime time(ts);

        local_tm_nano_sec local = (local_tm_nano_sec) time;
        _etsDate.year = local.ansi_tm.tm_year + 1900;
        _etsDate.mon = local.ansi_tm.tm_mon  + 1;
        _etsDate.day = local.ansi_tm.tm_mday;
        _etsDate.hour = local.ansi_tm.tm_hour;
        _etsDate.min = local.ansi_tm.tm_min;
        _etsDate.sec = local.ansi_tm.tm_sec;
        _etsDate.nsec = (unsigned long) ts.nsec;
        return _etsDate;
    }
		
    void print() {
        print(nelem) ;
    }
		
    void print(unsigned int  nelemToPrint) {
        nelemToPrint=min(nelemToPrint,nelem);
        if (pv==NULL) {
            std::cout <<  "Process Variable NOT ASSIGNED!" << std::endl;
            std::cout <<  "Variable has not been applied to a get operation!" << std::endl;
            return;
        }
       
        std::cout <<  "------------------------------------------" << std::endl;
        //std::cout <<  "PVDataHolder:" << std::endl;


        std::cout <<  "processVariable= "  << pv << std::endl;
        if (strcmp(pvAlias,pv) && strcmp(pvAlias,"")) {
            std::cout <<  "pvAlias        = "  << pvAlias << std::endl;
        }
        std::cout <<  "device         = "  << device << std::endl;
        std::cout <<  "attrib         = "  << attrib << std::endl;
        std::cout <<  "dataType       = "  << cafeDataTypeCode.message(dataType).c_str()
                << " (" << dataType << ") "  << std::endl;
        std::cout <<  "dbrTypeRequest = "  << dbr_type_to_text(dbrDataType)
                  << " (" << dbrDataType << ") " << std::endl;

        //std::cout <<  "dataType = "  << CAFEDataTypeCode.message(dataType).c_str() << std::endl;

        if (dataType != CAFE_NO_ACCESS &&  dataType != CAFE_TYPENOTCONN) {
            std::cout <<  "nelem          = ";
        }
        else {
            std::cout <<  "nelem          = ";
        }
        std::cout << nelem; std::cout << std::endl;
        if(!rule) {
            std::cout <<  "rule (0=false) = "  << rule <<std::endl;
        }

        if (dbr_type_is_STS(dbrDataType) || dbr_type_is_TIME(dbrDataType) ) {
           
            std::cout <<  "alarmStatus    = " << acond.asString(alarmStatus)  << " ("  <<  alarmStatus <<  ")" << std::endl;
            std::cout <<  "alarmSeverity  = " << aseve.asString(alarmSeverity) << " (" <<alarmSeverity <<  ")" << std::endl;

            if (dbr_type_is_TIME(dbrDataType)) {
            std::cout <<  "epicsTimeStamp = "  << ts.secPastEpoch << " sec. and " << ts.nsec << " nsec" << std::endl;
            }
        }
        if(beamEventNo!=0) {std::cout <<  "beamEventNo    = "  << beamEventNo << std::endl;};
        std::cout <<  "status         = "  << cafeStatusCode.message(status).c_str() << " (" << status << ") " << std::endl;
        std::cout <<  "value(s)       = "  ;
	
        switch (dataType) {
            case CAFE_STRING:
                for (unsigned int  i=0; i <nelemToPrint; ++i ) std::cout << val[i].str << " [" << i << "] " ;
            break;
            case CAFE_SHORT:
                for (unsigned int  i=0; i <nelemToPrint; ++i ) std::cout << val[i].s << " [" << i << "] " ;
            break;
            case CAFE_FLOAT:
                for (unsigned int  i=0; i <nelemToPrint; ++i ) std::cout << val[i].f << " [" << i << "] " ;
            break;
            case CAFE_ENUM:
            for (unsigned int  i=0; i <nelemToPrint; ++i ) { std::cout <<
                getAsString(i) << " (" <<  val[i].us << ")"  << " [" << i << "] " ;}
            break;
            case CAFE_CHAR:
                for (unsigned int  i=0; i <nelemToPrint; ++i ) std::cout << (unsigned short) val[i].ch << " [" << i << "] " ;
            break;
            case CAFE_LONG:
                for (unsigned int  i=0; i <nelemToPrint; ++i ) std::cout << val[i].l << " [" << i << "] " ;
            break;
            case CAFE_DOUBLE:
                for (unsigned int  i=0; i <nelemToPrint; ++i ) std::cout << val[i].d << " [" << i << "] " ;
            break;
            case CAFE_NO_ACCESS:
            std::cout << "DATA_TYPE NOT YET DEFINED " << endl;
            default:
            break;

        };
        std::cout << std::endl;
        std::cout <<  "------------------------------------------" << std::endl;
    };
};


#endif //PVDATAHOLDER_H
