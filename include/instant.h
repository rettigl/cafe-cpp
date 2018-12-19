///
/// \file    instant.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef INSTANT_H
#define INSTANT_H

#include <transpose.h>
#include <granules.h>
#include <cafeEnum.h>


template <class CTYPE> class Instant {

private:
    Transpose<dbr_string_t> renderString; // 0
    Transpose<dbr_short_t>  renderShort;  // 1
    Transpose<dbr_float_t>  renderFloat;  // 2
    Transpose<dbr_enum_t>   renderEnum;   // 3
    Transpose<dbr_char_t>   renderChar;   // 4
    Transpose<dbr_long_t>   renderLong;   // 5
    Transpose<dbr_double_t> renderDouble; // 6

    CAFEStatus   cafeStatus;
    Granules cafeGranules;

    PolicyHelper policyHelper;

    HandleHelper helper;

    Conduit cc;
    ChannelTimeoutPolicy         channelTimeoutPolicyGet;
    ChannelRequestPolicy         channelRequestPolicyGet;
    ChannelRequestStatus         channelRequestStatusGet;
    ChannelRequestDataTypePolicy channelRequestDataTypePolicy;

    ChannelRequestMetaDataClient channelRequestMetaDataClient; //-1

    int  status;

    int  clientRequests(const unsigned int  _handle,    const chtype dbrType,  const CTYPE * _val);

    int  clientRequests(const unsigned int  _handle,    const chtype dbrType,       CTYPE * _val,
                        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts,
                        bool isCacheRequest);
public:

    Instant () {};
    ~Instant () {};

    int  set(const unsigned int  *handleArray,  const unsigned int  nelem,
             const chtype _dbrType, const  CTYPE * val, int  *statusArray);

    int  set(const unsigned int  _handle, const chtype dbrType, const CTYPE * _val);

    int  get(const unsigned int  _handle, const chtype dbrType, CTYPE * _val,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);

    int  get(const unsigned int  _handle, const chtype dbrType, CTYPE * _val,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
    {
        epicsTimeStamp ts;
        return get(_handle, dbrType, _val, alarmStatus,  alarmSeverity,  ts);
    };

    int  get(const unsigned int  _handle, const chtype dbrType, CTYPE * _val)
    {
        dbr_short_t alarmStatus;
        dbr_short_t alarmSeverity;
        epicsTimeStamp ts;
        return get(_handle, dbrType, _val, alarmStatus,  alarmSeverity,  ts);
    };

    int  getCache(const unsigned int  _handle, const chtype dbrType, CTYPE * _val,
                  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);

    int  getCache(const unsigned int  _handle, const chtype dbrType, CTYPE * _val,
                  dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
    {
        epicsTimeStamp ts;
        return getCache(_handle, dbrType, _val, alarmStatus,  alarmSeverity,  ts);
    };

    int  getCache(const unsigned int  _handle, const chtype dbrType, CTYPE * _val)
    {
        dbr_short_t alarmStatus;
        dbr_short_t alarmSeverity;
        epicsTimeStamp ts;
        return getCache(_handle, dbrType, _val, alarmStatus,  alarmSeverity,  ts);
    };

    int  getCache(const unsigned int  *handleArray, const unsigned int  nelem,
                  const chtype _dbrType,  CTYPE * val, int  *statusArray,
                  dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity, epicsTimeStamp *ts);

    int  getCache(const unsigned int  *handleArray, const unsigned int  nelem,
                  const chtype _dbrType,  CTYPE * val, int  *statusArray,
                  dbr_short_t *alarmStatus, dbr_short_t *alarmSeverity )
    {
        epicsTimeStamp * ts;
        return getCache(handleArray, nelem, _dbrType,  val, statusArray,
                        alarmStatus, alarmSeverity, ts);
    };
    int  getCache(const unsigned int  *handleArray, const unsigned int  nelem,
                  const chtype _dbrType,  CTYPE * val, int  *statusArray)
    {
        dbr_short_t * alarmStatus;
        dbr_short_t * alarmSeverity;
        epicsTimeStamp * ts;
        return getCache(handleArray, nelem, _dbrType,  val, statusArray,
                        alarmStatus, alarmSeverity, ts);
    };

    int  setAndGet(const unsigned int handleSet, const chtype dbrType, CTYPE valSet, CTYPE &valGet);
    int  setAndMatch(const unsigned int handleSet, const chtype dbrType, CTYPE valSet, const unsigned int handleMatch,
                     CTYPE tolerance, double timeout, bool printFlag);

    int  setAndMatchMany(std::vector<unsigned int> handleSetV, const chtype dbrType, std::vector<CTYPE> valSet, std::vector<unsigned int> handleMatch,
                         CTYPE tolerance, double timeout, bool printFlag);

    int  setTriggerAndMatchMany(std::vector<unsigned int> handleSetV, const chtype dbrType, std::vector<CTYPE> valSet,
                                std::vector<unsigned int> handleAction, std::vector<std::string> valAction, std::vector<unsigned int> handleMatch,
                                CTYPE tolerance, double timeout, bool printFlag);

    int  matchMany( 													          	const chtype dbrType, std::vector<CTYPE> valSet, std::vector<unsigned int> handleMatch,
            CTYPE tolerance, double timeout, bool printFlag);

    int  match( 													          	const chtype dbrType, CTYPE valSet, unsigned int handleMatch,
            CTYPE tolerance, double timeout, bool printFlag);

    int  setMany(std::vector<unsigned int> handleSet, const chtype dbrType, std::vector<CTYPE> valSet, bool printFlag);



    int  compareAndMatchMany(std::vector<unsigned int> handleSet, const chtype dbrType, std::vector<CTYPE> valSet, std::vector<unsigned int> handleMatch,
                             CTYPE tolerance, double timeout, bool printFlag);



    int  setManyString(std::vector<unsigned int> handleSet, std::vector<std::string> valSet, bool printFlag)
    {

#define __METHOD__ "Instant<CTYPE>::setMany(vector<unsigned int>handleSet, chtype, vector<CTYPE>valSet)"

        CAFEStatus cstat;


        status=ICAFE_NORMAL;

        if (handleSet.size() != valSet.size() ) {
            return ECAFE_HANDLE_MISMATCH_SET_AND_MATCH;
        }

        for (size_t i=0; i< handleSet.size(); ++i) {

            if (!helper.isChannelConnected(handleSet[i])) {
                std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                std::cout << "NOT ALL CHANNELS CONNECTED: " << std::endl;
                if (!helper.isChannelConnected(handleSet[i])) {
                    helper.printHandle(handleSet[i]);
                    status=helper.getStatus(handleSet[i]);
                }
            }

        }


        if (status!=ICAFE_NORMAL) {
            return status;
        }

        if(printFlag) {
            std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        }


        if (printFlag) {

            for (size_t i=0; i< handleSet.size(); ++i) {

                std::cout << "SETTING  PV=" << helper.getPVFromHandle(handleSet[i]) << " to " << valSet[i] << std::endl;


            } //for

        } //if


        for (size_t i=0; i< handleSet.size(); ++i) {



            //set No of Elements to 1

            unsigned int  nelemPrevious, nelemRequestedCheck=0;
            unsigned int  nelemRequested=1;

            nelemPrevious=helper.getNelemClient(handleSet[i]);
            //Check the number of elements requested?
            if (nelemPrevious>1) {
                nelemRequestedCheck = helper.setNelem(handleSet[i],nelemRequested);
                if (nelemRequestedCheck != nelemRequested) {
                    std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                    std::cout << "Internal CAFE FUNNY: Wanted to set the no. elements from: "
                              << nelemPrevious << std::endl;
                    std::cout << "to: " << nelemRequested << " but got instead: "
                              << nelemRequestedCheck  << std::endl;
                }
            }



            //policy set synchronous
            ChannelRequestPolicy polPrevious, polNow;

            policyHelper.getChannelRequestPolicyPut(handleSet[i], polPrevious);

            polNow.setMethodKind(WITHOUT_CALLBACK);
            polNow.setWaitKind(WAIT);
            polNow.setWhenToFlushSendBuffer(FLUSH_AUTOMATIC);

            policyHelper.setChannelRequestPolicyPut(handleSet[i], polNow);

            dbr_string_t valSetA[1];


            helper.removeLeadingAndTrailingSpaces(valSet[i].c_str(), valSetA[0]);


            status=set(handleSet[i], DBR_STRING, valSetA);

            if (status!=ICAFE_NORMAL) {
                std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                cstat.report(status);
            }

            policyHelper.setChannelRequestPolicyPut(handleSet[i], polPrevious);


            unsigned int  nelemPreviousCheck=nelemPrevious;
            nelemRequested=1;
            //Switch back to previous value
            //if (nelemPrevious>1) {
            if(helper.getNelemRequest(handleSet[i])!= nelemPrevious) {
                nelemPreviousCheck= helper.setNelem(handleSet[i],nelemPrevious);
                if (nelemPreviousCheck != nelemPrevious) {
                    std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                    std::cout << "Internal CAFE FUNNY: Wanted to re-set the no. elements from: "
                              << nelemRequested << std::endl;
                    std::cout << "to the previous: " << nelemPrevious << " but got instead: "
                              << nelemPreviousCheck  << std::endl;
                }
            }


        } //for size_t

        return status;
#undef __METHOD__
    }

    /**
     *  \brief Set followed by an immediate get
     *  \param handleSet input: handle
     *  \param valSet input:  string value to set
     *  \param valGet output: string value to get
     *  \return ECA_NORMAL if all OK else first ECAFE error encountered, else ICAFE_SET_AND_GET_MISMATCH;
     */
    int  setAndGetString(const unsigned int handleSet, std::string valSet, std::string &valGet)
    {
#define __METHOD__ "Instant<CTYPE>::setAndGetString(const unsigned int handleSet, std::string valSet, std::string &valGet"
        //CheckPolicy

        dbr_string_t valGetA[1];
        dbr_string_t valSetA[1];

        strcpy(valGetA[0],"0");
        valGet="0";

        helper.removeLeadingAndTrailingSpaces(valSet.c_str(), valSetA[0]);

        status=Instant::set(handleSet, DBR_STRING, valSetA);
        if (status==ICAFE_NORMAL) {
            status=Instant::get(handleSet, DBR_STRING, valGetA);

            valGet=valGetA[0];
        }
        else {
            return status;
        }

        if (strcmp(valSetA[0],valGetA[0])==0) {
            return ICAFE_NORMAL;
        }

        //Check if number
        std::istringstream ins, ous;
        double oud=0;
        ous.clear();
        ous.str(valGetA[0]);
        ous>>oud;
        double ind=0;
        ins.clear();
        ins.str(valSetA[0]);
        ins>>ind;


        if ( !ous.fail()  && !ins.fail()) {
            if (ind==oud) {
                return ICAFE_NORMAL;
            }
        }
        //Cater for enums that are refered to by their integer values in string format
        else if (!ins.fail()) {
            short enumval=-1;

            enumval=helper.getEnumFromString(handleSet, valGet);

            //Convert integer to string

            std::stringstream ss;
            ss << enumval;
            valGet= ss.str();
            if ((short)ind==enumval)  {
                return ICAFE_NORMAL;
            }

        }


        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        std::cout << "Process Variable = " << helper.getPVFromHandle(handleSet) << std::endl;
        std::cout << "Set Value: " << valSetA[0] << " Get Value: " << valGet.c_str() << std::endl;
        return ICAFE_SET_AND_GET_MISMATCH;
#undef __METHOD__

    };


    /**
     *  \brief Set followed by an immediate get
     *  \param handleSet input: handle
     *  \param valSet input:  dbr_string_t value to set
     *  \param valGet output: dbr_string_t value to get
     *  \return ECA_NORMAL if all OK else first ECAFE error encountered, else ICAFE_SET_AND_GET_MISMATCH;
     */
    int  setAndGetDbrString(const unsigned int handleSet, dbr_string_t valSet, dbr_string_t &valGet)
    {
#define __METHOD__ "Instant<CTYPE>::setAndGetDbrString(const unsigned int handleSet, dbr_string_tvalSet, dbr_string_t &valGet"
        //CheckPolicy

        dbr_string_t valGetA[1];
        dbr_string_t valSetA[1];

        strcpy(valGetA[0],"0");
        strcpy(valGet,"0");

        helper.removeLeadingAndTrailingSpaces(valSet, valSetA[0]);

        status=Instant::set(handleSet, DBR_STRING, valSetA);
        if (status==ICAFE_NORMAL) {
            status=Instant::get(handleSet, DBR_STRING, valGetA);

            strcpy(valGet,valGetA[0]);
        }
        else {
            return status;
        }

        if (strcmp(valSetA[0],valGetA[0])==0) {
            return ICAFE_NORMAL;
        }

        //Check if number
        //Check if number
        std::istringstream ins, ous;
        double oud=0;
        ous.clear();
        ous.str(valGetA[0]);
        ous>>oud;
        double ind=0;
        ins.clear();
        ins.str(valSetA[0]);
        ins>>ind;

        if ( !ous.fail()  && !ins.fail()) {
            if (ind==oud) {
                return ICAFE_NORMAL;
            }
        }
        //Cater for enums that are refered to by their integer values in string format
        else if (!ins.fail()) {
            short enumval=-1;

            enumval=helper.getEnumFromString(handleSet, valGet);

            //Convert integer to string
            std::stringstream ss;
            ss << enumval;
            strcpy(valGet, ss.str().c_str());

            if ((short) ind==enumval)  {
                return ICAFE_NORMAL;
            }


        }


        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        std::cout << "Process Variable = " << helper.getPVFromHandle(handleSet) << std::endl;
        std::cout << "Set Value: " << valSetA[0] << " Get Value: " << valGet << std::endl;
        return ICAFE_SET_AND_GET_MISMATCH;
#undef __METHOD__

    };

};

#include <instant.cpp>

#endif // INSTANT_H

