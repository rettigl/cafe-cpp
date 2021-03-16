///
/// \file   cafeVectors.cpp
///
/// \author Jan Chrin, PSI
/// \date   September 2015
/// \version CAFE 1.1.0
///
#include <cafe.h>

using namespace std;

//6
/**
 *  \brief   get vector of dbr_double_t (double)
 *  \param   _handle input: handle
 *  \param   V output: vector of doubles
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::get(const unsigned int  _handle, std::vector<double> & V)
{

    dbr_short_t alarmStatus;
    dbr_short_t alarmSeverity;
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_PRIMITIVE;
    return get(_handle, V, alarmStatus, alarmSeverity, ts);

};
//6
/**
 *  \brief   get vector of dbr_double_t (double)
 *  \param   _handle input: handle
 *  \param   V output: vector of doubles
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::get(const unsigned int  _handle, std::vector<double> & V,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_STS;
    return get(_handle, V, alarmStatus, alarmSeverity, ts);
};
//5
/**
 *  \brief   get vector of dbr_long_t (long)
 *  \param   _handle input: handle
 *  \param   V output: vector of longs
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::get(const unsigned int  _handle, std::vector<dbr_long_t> & V)
{
    dbr_short_t alarmStatus;
    dbr_short_t alarmSeverity;
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_PRIMITIVE;
    return get(_handle, V, alarmStatus, alarmSeverity, ts);
};
//5
/**
 *  \brief   get vector of dbr_long_t (long)
 *  \param   _handle input: handle
 *  \param   V output: vector of longs
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::get(const unsigned int  _handle, std::vector<dbr_long_t> & V,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_STS;
    return get(_handle, V, alarmStatus, alarmSeverity, ts);
};
//5
/**
 *  \brief   get vector of long
 *  \param   _handle input: handle
 *  \param   V output: vector of longs
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::get(const unsigned int  _handle, std::vector<long long> & V)
{
    dbr_short_t alarmStatus;
    dbr_short_t alarmSeverity;
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_PRIMITIVE;
    return get(_handle, V, alarmStatus, alarmSeverity, ts);
};
//5
/**
 *  \brief   get vector of longs
 *  \param   _handle input: handle
 *  \param   V output: vector of longs
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::get(const unsigned int  _handle, std::vector<long long> & V,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_STS;
    return get(_handle, V, alarmStatus, alarmSeverity, ts);
};
//4
/**
 *  \brief get vector of dbr_char_t (unsigned char)
 *  \param   _handle input: handle
 *  \param   V output: vector of unsigned char
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::get(const unsigned int  _handle, std::vector<unsigned char> & V)
{
    dbr_short_t alarmStatus;
    dbr_short_t alarmSeverity;
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_PRIMITIVE;
    return get(_handle, V, alarmStatus, alarmSeverity, ts);
};
//4
/**
 *  \brief   get vector of unsigned dbr_char_t (unsigned char)
 *  \param   _handle input: handle
 *  \param   V output: vector of unsigned char
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::get(const unsigned int  _handle, std::vector<unsigned char> & V,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_STS;
    return get(_handle, V, alarmStatus, alarmSeverity, ts);
};
//3
/**
 *  \brief   get vector of dbr_enum_t (unsigned short)
 *  \param   _handle input: handle
 *  \param   V output: vector of unsigned short
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::get(const unsigned int  _handle, std::vector<unsigned short> & V)
{
    dbr_short_t alarmStatus;
    dbr_short_t alarmSeverity;
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_PRIMITIVE;
    return get(_handle, V, alarmStatus, alarmSeverity, ts);
};
//3
/**
 *  \brief   get vector of dbr_enum_t (unsigned short)
 *  \param   _handle input: handle
 *  \param   V output: vector of unsigned shorts
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::get(const unsigned int  _handle, std::vector<unsigned short> & V,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_STS;
    return get(_handle, V, alarmStatus, alarmSeverity, ts);
};
//2
/**
 *  \brief   get vector of dbr_float_t (float)
 *  \param   _handle input: handle
 *  \param   V output: vector of floats
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::get(const unsigned int  _handle, std::vector<float> & V)
{
    dbr_short_t alarmStatus;
    dbr_short_t alarmSeverity;
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_PRIMITIVE;
    return get(_handle, V, alarmStatus, alarmSeverity, ts);
};
//2
/**
 *  \brief   get vector of dbr_float_t (float)
 *  \param   _handle input: handle
 *  \param   V output: vector of floats
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::get(const unsigned int  _handle, std::vector<float> & V,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_STS;
    return get(_handle, V, alarmStatus, alarmSeverity, ts);
};
//1
/**
 *  \brief   get vector of dbr_short_t (short)
 *  \param   _handle input: handle
 *  \param   V output: vector of shorts
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::get(const unsigned int  _handle, std::vector<short> & V)
{
    dbr_short_t alarmStatus;
    dbr_short_t alarmSeverity;
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_PRIMITIVE;
    return get(_handle, V, alarmStatus, alarmSeverity, ts);
};
//1
/**
 *  \brief   get vector of dbr_short_t (short)
 *  \param   _handle input: handle
 *  \param   V output: vector of shorts
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::get(const unsigned int  _handle, std::vector<short> & V,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_STS;
    return get(_handle, V, alarmStatus, alarmSeverity, ts);
};
//0
/**
 *  \brief   get vector of string
 *  \param   _handle input: handle
 *  \param   V output: vector of string
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::get(const unsigned int  _handle, std::vector<std::string> & V)
{
    dbr_short_t alarmStatus;
    dbr_short_t alarmSeverity;
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_PRIMITIVE;
    return get(_handle, V, alarmStatus, alarmSeverity, ts);
};
//0
/**
 *  \brief   get vector of strings
 *  \param   _handle input: handle
 *  \param   V output: vector of strings
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::get(const unsigned int  _handle, std::vector<std::string> & V,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_STS;
    return get(_handle, V, alarmStatus, alarmSeverity, ts);
};


//6
/**
 *  \brief   get vector of doubles
 *  \param   _handle input: handle
 *  \param   V output: vector of doubles
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \param   ts output: epicsTimeStamp ts.secPastEpoch, ts.nsec
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::get(const unsigned int  _handle, std::vector<double> & V,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{


    V.clear();
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        unsigned int  nelemThatWillBeRequested=(*it_handle).getChannelRegalia().getNelem();

        //Reserve only what will be in the buffer.
        //Lets calculate this
        //Check the number of elements requested?


        if ((*it_handle).getChannelRegalia().getNelem()>1)
        {

            //What did the client request? What is native type? Transfer minimum of this.
            if ( (*it_handle).getChannelRegalia().getNelem()
                    !=  (*it_handle).getChannelRequestMetaDataClient().getNelem())
            {

                nelemThatWillBeRequested= min( (*it_handle).getChannelRegalia().getNelem(),
                                               (*it_handle).getChannelRequestMetaDataClient().getNelem());
            }
        }

        V.reserve(nelemThatWillBeRequested);


        double * val = new double [nelemThatWillBeRequested];

        if(vectorDbrType==CAFENUM::DBR_NONE)
        {
            vectorDbrType=CAFENUM::DBR_TIME;
        };
        switch (vectorDbrType)
        {
        case CAFENUM::DBR_PRIMITIVE:

            status=cafeDoppio.get (_handle, DBR_DOUBLE, val);

        case CAFENUM::DBR_STS:
            status=cafeDoppio.get (_handle, DBR_STS_DOUBLE, val, alarmStatus, alarmSeverity);
            break;
        case CAFENUM::DBR_TIME:
        default:
            status=cafeDoppio.get (_handle, DBR_TIME_DOUBLE, val, alarmStatus, alarmSeverity, ts);
            break;
        }
        vectorDbrType=CAFENUM::DBR_NONE;


        if (status==ICAFE_NORMAL)
        {
            //(*it_handle).getChannelRequestMetaData().getNelem() <<=nelemThatWillBeRequested

            //for (unsigned short i=0; i<(*it_handle).getChannelRequestMetaData().getNelem(); ++i) {
            //   V.push_back(val[i]);
            //}

            V.insert(V.end(), &val[0], &val[(*it_handle).getChannelRequestMetaData().getNelem()]);
        }


        delete [] val;

        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};

//5
/**
 *  \brief   get vector of dbr_long_t
 *  \param   _handle input: handle
 *  \param   V output: vector of longs
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \param   ts output: epicsTimeStamp ts.secPastEpoch, ts.nsec
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::get(const unsigned int  _handle, std::vector<dbr_long_t> & V,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{

    V.clear();

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        unsigned int  nelemThatWillBeRequested= (*it_handle).getChannelRegalia().getNelem();

        //Reserve only what will be in the buffer.
        //Lets calculate this
        //Check the number of elements requested?
        if ((*it_handle).getChannelRegalia().getNelem()>1)
        {
            //What did the client request? What is native type? Transfer minimum of this.
            if ( (*it_handle).getChannelRegalia().getNelem()
                    !=  (*it_handle).getChannelRequestMetaDataClient().getNelem())
            {

                nelemThatWillBeRequested= min( (*it_handle).getChannelRegalia().getNelem(),
                                               (*it_handle).getChannelRequestMetaDataClient().getNelem());
            }
        }

        V.reserve(nelemThatWillBeRequested);

        dbr_long_t * val = new dbr_long_t [nelemThatWillBeRequested];

        if(vectorDbrType==CAFENUM::DBR_NONE)
        {
            vectorDbrType=CAFENUM::DBR_TIME;
        };

        switch (vectorDbrType)
        {
        case CAFENUM::DBR_PRIMITIVE:
            status=cafeLatte.get (_handle, DBR_LONG, val);
            break;
        case CAFENUM::DBR_STS:
            status=cafeLatte.get (_handle, DBR_STS_LONG, val, alarmStatus, alarmSeverity);
            break;
        case CAFENUM::DBR_TIME:
        default:
            status=cafeLatte.get (_handle, DBR_TIME_LONG, val, alarmStatus, alarmSeverity, ts);
            break;
        }

        vectorDbrType=CAFENUM::DBR_NONE;

        if (status==ICAFE_NORMAL)
        {

            //for (unsigned short i=0; i<(*it_handle).getChannelRequestMetaData().getNelem(); ++i) {
            //    V.push_back(val[i]);
            //}

            V.insert(V.end(), &val[0], &val[(*it_handle).getChannelRequestMetaData().getNelem()]);
        }

        delete [] val;
        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};


//5
/**
 *  \brief   get vector of longs
 *  \param   _handle input: handle
 *  \param   V output: vector of longs
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \param   ts output: epicsTimeStamp ts.secPastEpoch, ts.nsec
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::get(const unsigned int  _handle, std::vector<long long> & V,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{

#define __METHOD__ \
    "CAFE::get(const unsigned int _handle, vector<long long> & V, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    V.clear();

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        ChannelRegalia channelInfo;
        CAFE::getChannelInfo(_handle, channelInfo);
        if ( channelInfo.getCafeConnectionState() == ICAFE_CS_NEVER_CONN)
        {
            return ICAFE_CS_NEVER_CONN;
        }
        else if ( channelInfo.getCafeConnectionState()==ICAFE_CS_CLOSED)
        {
            return ICAFE_CS_CLOSED;
        }

        unsigned int  nelemThatWillBeRequested= (*it_handle).getChannelRegalia().getNelem();

        //Reserve only what will be in the buffer.
        //Lets calculate this
        //Check the number of elements requested?
        if ((*it_handle).getChannelRegalia().getNelem()>1)
        {
            //What did the client request? What is native type? Transfer minimum of this.
            if ( (*it_handle).getChannelRegalia().getNelem()
                    !=  (*it_handle).getChannelRequestMetaDataClient().getNelem())
            {

                nelemThatWillBeRequested= min( (*it_handle).getChannelRegalia().getNelem(),
                                               (*it_handle).getChannelRequestMetaDataClient().getNelem());
            }
        }

        V.reserve(nelemThatWillBeRequested);


        if ( channelInfo.getDataType() == DBR_STRING)
        {

            dbr_string_t * val = new dbr_string_t[nelemThatWillBeRequested];

            if(vectorDbrType==CAFENUM::DBR_NONE)
            {
                vectorDbrType=CAFENUM::DBR_TIME;
            };

            switch (vectorDbrType)
            {
            case CAFENUM::DBR_PRIMITIVE:
                status=cafeSoluble.get (_handle, DBR_STRING, val);
                break;
            case CAFENUM::DBR_STS:
                status=cafeSoluble.get (_handle, DBR_STS_STRING, val, alarmStatus, alarmSeverity);
                break;
            case CAFENUM::DBR_TIME:
            default:
                status=cafeSoluble.get (_handle, DBR_TIME_STRING, val, alarmStatus, alarmSeverity, ts);
                break;
            }


            if (status==ICAFE_NORMAL)
            {

                istringstream ss;
                bool allStringsAreValid=true;


                for (unsigned short i=0; i<(*it_handle).getChannelRequestMetaData().getNelem(); ++i)
                {
                    long long l=0;
                    ss.clear();
                    ss.str(val[i]);
                    ss>>l;

                    if ( !ss.fail())
                    {
                        V.push_back(l);
                        std::string strInput=val[i];
                        std::stringstream ssOut;
                        ssOut << l;
                        std::string strOutput=ssOut.str();
                        if (strInput!=strOutput)
                        {
                            cout << __METHOD__ << "//" << __LINE__ << endl;
                            cout << "***WARNING*** STRING TO LONG LONG CONVERSION REPORTS: " << endl;
                            cout << "STRING VALUE: " << strInput << " CONVERTED TO: " << strOutput << endl;
                        }
                    }
                    else
                    {
                        cout << __METHOD__ << "//" << __LINE__ << endl;
                        cout << "***WARNING*** NO STRING TO LONG LONG  CONVERSION for ELEMENT " << i \
                             << " of " << (*it_handle).getChannelRequestMetaData().getNelem() << " !! " << endl;
                        cout << "***WARNING*** COULD NOT CONVERT: ";
                        cout << val[i];
                        cout << " TO LONG LONG ; CONVERSION NOT MADE!" << endl;
                        cout << " DATA POINT OMITTED. LENGTH OF RETURNING VECTOR WILL BE DIFFERENT TO EXPECTED "<< endl;
                        allStringsAreValid=false;
                    }
                }

                if(!allStringsAreValid)
                {
                    cout << "----------------------------------------------------------------" << endl;
                    cout << "NOT ALL ELEMENTS OF STRING ARRAY COULD BE CONVERTED TO LONG LONG " << endl;
                    cout << "THESE HAVE A VALUE OF ZERO IN PLACE OF STRING VALUE " << endl;
                    cout << "----------------------------------------------------------------" << endl;
                    // cout << " RETURNING VECTOR HAS LENGTH " << V.size() << " RATHER THAN THE EXPECTED " <<
                    //   (*it_handle).getChannelRequestMetaData().getNelem()   << endl;
                }

            }

            delete [] val;
        }


        else if ( channelInfo.getDataType() == DBR_DOUBLE)
        {

            dbr_double_t * val = new dbr_double_t[nelemThatWillBeRequested];

            if(vectorDbrType==CAFENUM::DBR_NONE)
            {
                vectorDbrType=CAFENUM::DBR_TIME;
            };

            switch (vectorDbrType)
            {
            case CAFENUM::DBR_PRIMITIVE:
                status=cafeDoppio.get (_handle, DBR_DOUBLE, val);
                break;
            case CAFENUM::DBR_STS:
                status=cafeDoppio.get (_handle, DBR_STS_DOUBLE, val, alarmStatus, alarmSeverity);
                break;
            case CAFENUM::DBR_TIME:
            default:
                status=cafeDoppio.get (_handle, DBR_TIME_DOUBLE, val, alarmStatus, alarmSeverity, ts);
                break;
            }

            if (status==ICAFE_NORMAL)
            {

                for (unsigned short i=0; i<(*it_handle).getChannelRequestMetaData().getNelem(); ++i)
                {
                    V.push_back((long long) val[i]);
                }

                //V.insert(V.end(), &val[0], &val[(*it_handle).getChannelRequestMetaData().getNelem()]);
            }

            delete  [] val;
        }
        else if ( channelInfo.getDataType() == DBR_FLOAT)
        {

            dbr_float_t * val = new dbr_float_t[nelemThatWillBeRequested];
            if(vectorDbrType==CAFENUM::DBR_NONE)
            {
                vectorDbrType=CAFENUM::DBR_TIME;
            };
            switch (vectorDbrType)
            {
            case CAFENUM::DBR_PRIMITIVE:
                status=cafeFrappuccino.get (_handle, DBR_FLOAT, val);
                break;
            case CAFENUM::DBR_STS:
                status=cafeFrappuccino.get (_handle, DBR_STS_FLOAT, val, alarmStatus, alarmSeverity);
                break;
            case CAFENUM::DBR_TIME:
            default:
                status=cafeFrappuccino.get (_handle, DBR_TIME_FLOAT, val, alarmStatus, alarmSeverity, ts);
                break;
            }

            if (status==ICAFE_NORMAL)
            {

                for (unsigned short i=0; i<(*it_handle).getChannelRequestMetaData().getNelem(); ++i)
                {
                    V.push_back((long long) val[i]);
                }
                //V.insert(V.end(), &val[0], &val[(*it_handle).getChannelRequestMetaData().getNelem()]);
            }

            delete [] val;
        }


        else
        {

            dbr_long_t * val = new dbr_long_t [nelemThatWillBeRequested];

            if(vectorDbrType==CAFENUM::DBR_NONE)
            {
                vectorDbrType=CAFENUM::DBR_TIME;
            };

            switch (vectorDbrType)
            {
            case CAFENUM::DBR_PRIMITIVE:
                status=cafeLatte.get (_handle, DBR_LONG, val);
                break;
            case CAFENUM::DBR_STS:
                status=cafeLatte.get (_handle, DBR_STS_LONG, val, alarmStatus, alarmSeverity);
                break;
            case CAFENUM::DBR_TIME:
            default:
                status=cafeLatte.get (_handle, DBR_TIME_LONG, val, alarmStatus, alarmSeverity, ts);
                break;
            }

            if (status==ICAFE_NORMAL)
            {

                for (unsigned short i=0; i<(*it_handle).getChannelRequestMetaData().getNelem(); ++i)
                {
                    V.push_back((long long) val[i]);
                }
                //V.insert(V.end(), &val[0], &val[(*it_handle).getChannelRequestMetaData().getNelem()]);
            }

            delete [] val;

        }

        vectorDbrType=CAFENUM::DBR_NONE;

        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
#undef __METHOD__
};


//4
/**
 *  \brief   get vector of unsigned char
 *  \param   _handle input: handle
 *  \param   V output: vector of unsigned char
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \param   ts output: epicsTimeStamp ts.secPastEpoch, ts.nsec
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::get(const unsigned int  _handle, std::vector<unsigned char> & V,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{

    V.clear();
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        unsigned int  nelemThatWillBeRequested= (*it_handle).getChannelRegalia().getNelem();

        //Reserve only what will be in the buffer.
        //Lets calculate this
        //Check the number of elements requested?
        if ((*it_handle).getChannelRegalia().getNelem()>1)
        {
            //What did the client request? What is native type? Transfer minimum of this.
            if ( (*it_handle).getChannelRegalia().getNelem()
                    !=  (*it_handle).getChannelRequestMetaDataClient().getNelem())
            {

                nelemThatWillBeRequested= min( (*it_handle).getChannelRegalia().getNelem(),
                                               (*it_handle).getChannelRequestMetaDataClient().getNelem());
            }
        }

        V.reserve(nelemThatWillBeRequested);

        unsigned char * val = new unsigned char [nelemThatWillBeRequested];

        if(vectorDbrType==CAFENUM::DBR_NONE)
        {
            vectorDbrType=CAFENUM::DBR_TIME;
        };

        switch (vectorDbrType)
        {
        case CAFENUM::DBR_PRIMITIVE:
            status=cafeCappuccino.get (_handle, DBR_CHAR, val);
            //cout << "status=cafeCappuccino.get (_handle, DBR_CHAR, val) " << endl;
            break;
        case CAFENUM::DBR_STS:
            status=cafeCappuccino.get (_handle, DBR_STS_CHAR, val, alarmStatus, alarmSeverity);
            break;
        case CAFENUM::DBR_TIME:
        default:
            status=cafeCappuccino.get (_handle, DBR_TIME_CHAR, val, alarmStatus, alarmSeverity, ts);
            //cout << "status=cafeCappuccino.get (_handle, DBR_CHAR,val, alarmStatus, alarmSeverity, ts);"
            //     << endl;
            break;
        }
        vectorDbrType=CAFENUM::DBR_NONE;

        if (status==ICAFE_NORMAL)
        {

            for (unsigned int  i=0; i<(*it_handle).getChannelRequestMetaData().getNelem(); ++i)
            {
                V.push_back(val[i]);
            }
            //V.insert(V.end(), &val[0], &val[(*it_handle).getChannelRequestMetaData().getNelem()]);
        }

        delete [] val;
        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};


//3
/**
 *  \brief   get vector of unsigned shorts (enums)
 *  \param   _handle input: handle
 *  \param   V output: vector of unsigned shorts (enum)
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \param   ts output: epicsTimeStamp ts.secPastEpoch, ts.nsec
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::get(const unsigned int  _handle, std::vector<unsigned short> & V,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{

    V.clear();

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        unsigned int  nelemThatWillBeRequested= (*it_handle).getChannelRegalia().getNelem();

        //Reserve only what will be in the buffer.
        //Lets calculate this
        //Check the number of elements requested?
        if ((*it_handle).getChannelRegalia().getNelem()>1)
        {
            //What did the client request? What is native type? Transfer minimum of this.
            if ( (*it_handle).getChannelRegalia().getNelem()
                    !=  (*it_handle).getChannelRequestMetaDataClient().getNelem())
            {

                nelemThatWillBeRequested= min( (*it_handle).getChannelRegalia().getNelem(),
                                               (*it_handle).getChannelRequestMetaDataClient().getNelem());
            }
        }

        V.reserve(nelemThatWillBeRequested);

        unsigned short * val = new unsigned short [nelemThatWillBeRequested];

        if(vectorDbrType==CAFENUM::DBR_NONE)
        {
            vectorDbrType=CAFENUM::DBR_TIME;
        };
        switch (vectorDbrType)
        {
        case CAFENUM::DBR_PRIMITIVE:
            status=cafeEspresso.get (_handle, DBR_ENUM, val);
            break;
        case CAFENUM::DBR_STS:
            status=cafeEspresso.get (_handle, DBR_STS_ENUM, val, alarmStatus, alarmSeverity);
            break;
        case CAFENUM::DBR_TIME:
        default:
            status=cafeEspresso.get (_handle, DBR_TIME_ENUM, val, alarmStatus, alarmSeverity, ts);
            break;
        }
        vectorDbrType=CAFENUM::DBR_NONE;


        if (status==ICAFE_NORMAL)
        {

            //for (unsigned int  i=0; i<(*it_handle).getChannelRequestMetaData().getNelem(); ++i) {
            //    V.push_back(val[i]);
            //}
            V.insert(V.end(), &val[0], &val[(*it_handle).getChannelRequestMetaData().getNelem()]);
        }

        delete  [] val;
        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};

//2
/**
 *  \brief   get vector of floats
 *  \param   _handle input: handle
 *  \param   V output: vector of floats
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \param   ts output: epicsTimeStamp ts.secPastEpoch, ts.nsec
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::get(const unsigned int  _handle, std::vector<float> & V,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{

    V.clear();

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        unsigned int  nelemThatWillBeRequested= (*it_handle).getChannelRegalia().getNelem();
        //Reserve only what will be in the buffer.
        //Lets calculate this
        //Check the number of elements requested?
        if ((*it_handle).getChannelRegalia().getNelem()>1)
        {
            //What did the client request? What is native type? Transfer minimum of this.
            if ( (*it_handle).getChannelRegalia().getNelem()
                    !=  (*it_handle).getChannelRequestMetaDataClient().getNelem())
            {

                nelemThatWillBeRequested= min( (*it_handle).getChannelRegalia().getNelem(),
                                               (*it_handle).getChannelRequestMetaDataClient().getNelem());
            }

        }

        V.reserve(nelemThatWillBeRequested);

        float * val = new float [nelemThatWillBeRequested];

        if(vectorDbrType==CAFENUM::DBR_NONE)
        {
            vectorDbrType=CAFENUM::DBR_TIME;
        };

        switch (vectorDbrType)
        {
        case CAFENUM::DBR_PRIMITIVE:

            status=cafeFrappuccino.get (_handle, DBR_FLOAT, val);

            break;
        case CAFENUM::DBR_STS:
            status=cafeFrappuccino.get (_handle, DBR_STS_FLOAT, val, alarmStatus, alarmSeverity);
            break;
        case CAFENUM::DBR_TIME:
        default:
            status=cafeFrappuccino.get (_handle, DBR_TIME_FLOAT, val, alarmStatus, alarmSeverity, ts);
            break;
        }
        vectorDbrType=CAFENUM::DBR_NONE;


        if (status==ICAFE_NORMAL)
        {

            //for (unsigned int  i=0; i<(*it_handle).getChannelRequestMetaData().getNelem(); ++i) {
            //    V.push_back(val[i]);
            //}
            V.insert(V.end(), &val[0], &val[(*it_handle).getChannelRequestMetaData().getNelem()]);
        }

        delete [] val;
        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};


//1
/**
 *  \brief   get vector of shorts
 *  \param   _handle input: handle
 *  \param   V output: vector of shorts
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \param   ts output: epicsTimeStamp ts.secPastEpoch, ts.nsec
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::get(const unsigned int  _handle, std::vector<short> & V,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{

    V.clear();

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        unsigned int  nelemThatWillBeRequested=(*it_handle).getChannelRegalia().getNelem();

        //Reserve only what will be in the buffer.
        //Lets calculate this
        //Check the number of elements requested?
        if ((*it_handle).getChannelRegalia().getNelem()>1)
        {
            //What did the client request? What is native type? Transfer minimum of this.
            if ( (*it_handle).getChannelRegalia().getNelem()
                    !=  (*it_handle).getChannelRequestMetaDataClient().getNelem())
            {

                nelemThatWillBeRequested= min( (*it_handle).getChannelRegalia().getNelem(),
                                               (*it_handle).getChannelRequestMetaDataClient().getNelem());
            }
        }

        V.reserve(nelemThatWillBeRequested);

        short * val = new short [nelemThatWillBeRequested];


        if(vectorDbrType==CAFENUM::DBR_NONE)
        {
            vectorDbrType=CAFENUM::DBR_TIME;
        };
        switch (vectorDbrType)
        {
        case CAFENUM::DBR_PRIMITIVE:
            status=cafeSchale.get (_handle, DBR_SHORT, val);
            break;
        case CAFENUM::DBR_STS:
            status=cafeSchale.get (_handle, DBR_STS_SHORT, val, alarmStatus, alarmSeverity);
            break;
        case CAFENUM::DBR_TIME:
        default:
            status=cafeSchale.get (_handle, DBR_TIME_SHORT, val, alarmStatus, alarmSeverity, ts);
            break;
        }
        vectorDbrType=CAFENUM::DBR_NONE;


        if (status==ICAFE_NORMAL)
        {
            //(*it_handle).getChannelRequestMetaData().getNelem() <<=nelemThatWillBeRequested
            for (unsigned int  i=0; i<(*it_handle).getChannelRequestMetaData().getNelem(); ++i)
            {
                V.push_back(val[i]);
            }
            //	V.insert(V.end(), &val[0], &val[(*it_handle).getChannelRequestMetaData().getNelem()]);
        }

        delete [] val;
        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};


//0
/**
 *  \brief   get vector of strings
 *  \param   _handle input: handle
 *  \param   V output: vector of strings
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \param   ts output: epicsTimeStamp ts.secPastEpoch, ts.nsec
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::get(const unsigned int  _handle, std::vector<std::string> & V,
               dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{

    V.clear();

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        unsigned int  nelemThatWillBeRequested=(*it_handle).getChannelRegalia().getNelem();

        //Reserve only what will be in the buffer.
        //Lets calculate this
        //Check the number of elements requested?
        if ((*it_handle).getChannelRegalia().getNelem()>1)
        {
            //What did the client request? What is native type? Transfer minimum of this.
            if ( (*it_handle).getChannelRegalia().getNelem()
                    !=  (*it_handle).getChannelRequestMetaDataClient().getNelem())
            {

                nelemThatWillBeRequested= min( (*it_handle).getChannelRegalia().getNelem(),
                                               (*it_handle).getChannelRequestMetaDataClient().getNelem());
            }
        }


        V.reserve(nelemThatWillBeRequested);

        dbr_string_t * vals = new dbr_string_t [nelemThatWillBeRequested];


        if(vectorDbrType==CAFENUM::DBR_NONE)
        {
            vectorDbrType=CAFENUM::DBR_TIME;
        };
        switch (vectorDbrType)
        {
        case CAFENUM::DBR_PRIMITIVE:
            status=cafeSoluble.get (_handle, DBR_STRING, vals);
            break;
        case CAFENUM::DBR_STS:
            status=cafeSoluble.get (_handle, DBR_STS_STRING, vals, alarmStatus, alarmSeverity);
            break;
        case CAFENUM::DBR_TIME:
        default:
            status=cafeSoluble.get (_handle, DBR_TIME_STRING, vals, alarmStatus, alarmSeverity, ts);
            break;
        }
        vectorDbrType=CAFENUM::DBR_NONE;


        if (status==ICAFE_NORMAL)
        {
            //(*it_handle).getChannelRequestMetaData().getNelem() <<=nelemThatWillBeRequested
            for (unsigned int  i=0; i<(*it_handle).getChannelRequestMetaData().getNelem(); ++i)
            {
                V.push_back(vals[i]);
            }
            //	V.insert(V.end(), &vals[0], &vals[(*it_handle).getChannelRequestMetaData().getNelem()]);

        }

        delete [] vals;
        return status;

    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
}

///////////////////////////// getCache /////////////////////////////////////

//6
/**
 *  \brief   get cached vector of doubles
 *  \param   _handle input: handle
 *  \param   V output: vector of doubles
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<double> & V)
{

    dbr_short_t alarmStatus;
    dbr_short_t alarmSeverity;
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_PRIMITIVE;
    return getCache(_handle, V, alarmStatus, alarmSeverity, ts);
};
//6
/**
 *  \brief   get cached vector of doubles, alarmStatus, alarmSeverity
 *  \param   _handle input: handle
 *  \param   V output: vector of doubles
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<double> & V,
                    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_STS;
    return getCache(_handle, V, alarmStatus, alarmSeverity, ts);
};
//5
/**
 *  \brief   get cached vector of dbr_long_t
 *  \param   _handle input: handle
 *  \param   V output: vector of dbr_long_t
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<dbr_long_t> & V)
{
    dbr_short_t alarmStatus;
    dbr_short_t alarmSeverity;
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_PRIMITIVE;
    return getCache(_handle, V, alarmStatus, alarmSeverity, ts);
};
//5
/**
 *  \brief   get cached vector of dbr_long_t, alarmStatus, alarmSeverity
 *  \param   _handle input: handle
 *  \param   V output: vector of dbr_long_t
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<dbr_long_t> & V,
                    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_STS;
    return getCache(_handle, V, alarmStatus, alarmSeverity, ts);
};
//5+
/**
 *  \brief   get cached vector of long long
 *  \param   _handle input: handle
 *  \param   V output: vector of long long
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<long long> & V)
{
    dbr_short_t alarmStatus;
    dbr_short_t alarmSeverity;
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_PRIMITIVE;
    return getCache(_handle, V, alarmStatus, alarmSeverity, ts);
};
//5+
/**
 *  \brief   get cached vector of long long, alarmStatus, alarmSeverity
 *  \param   _handle input: handle
 *  \param   V output: vector of long long
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<long long> & V,
                    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_STS;
    return getCache(_handle, V, alarmStatus, alarmSeverity, ts);
};
//4
/**
 *  \brief   get cached vector of unsigned char
 *  \param   _handle input: handle
 *  \param   V output: vector of unsigned char
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<unsigned char> & V)
{
    dbr_short_t alarmStatus;
    dbr_short_t alarmSeverity;
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_PRIMITIVE;
    return getCache(_handle, V, alarmStatus, alarmSeverity, ts);
};
//4
/**
 *  \brief   get cached vector of unsigned char alarmStatus, alarmSeverity
 *  \param   _handle input: handle
 *  \param   V output: vector of dbr_long_t
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<unsigned char> & V,
                    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_STS;
    return getCache(_handle, V, alarmStatus, alarmSeverity, ts);
};
//3
/**
 *  \brief   get cached vector of unsigned short
 *  \param   _handle input: handle
 *  \param   V output: vector of unsigned short
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<unsigned short> & V)
{
    dbr_short_t alarmStatus;
    dbr_short_t alarmSeverity;
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_PRIMITIVE;
    return getCache(_handle, V, alarmStatus, alarmSeverity, ts);
};
//3
/**
 *  \brief   get cached vector of unsigned short, alarmStatus, alarmSeverity
 *  \param   _handle input: handle
 *  \param   V output: vector of dbr_long_t
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<unsigned short> & V,
                    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_STS;
    return getCache(_handle, V, alarmStatus, alarmSeverity, ts);
};
//2
/**
 *  \brief   get cached vector of float
 *  \param   _handle input: handle
 *  \param   V output: vector of float
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<float> & V)
{
    dbr_short_t alarmStatus;
    dbr_short_t alarmSeverity;
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_PRIMITIVE;
    return getCache(_handle, V, alarmStatus, alarmSeverity, ts);
};
//2
/**
 *  \brief   get cached vector of float, alarmStatus, alarmSeverity
 *  \param   _handle input: handle
 *  \param   V output: vector of dbr_long_t
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<float> & V,
                    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_STS;
    return getCache(_handle, V, alarmStatus, alarmSeverity, ts);
};
//1
/**
 *  \brief   get cached vector of short
 *  \param   _handle input: handle
 *  \param   V output: vector of short
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<short> & V)
{
    dbr_short_t alarmStatus;
    dbr_short_t alarmSeverity;
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_PRIMITIVE;
    return getCache(_handle, V, alarmStatus, alarmSeverity, ts);
};
//1
/**
 *  \brief   get cached vector of short, alarmStatus, alarmSeverity
 *  \param   _handle input: handle
 *  \param   V output: vector of short
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<short> & V,
                    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_STS;
    return getCache(_handle, V, alarmStatus, alarmSeverity, ts);
};
//0
/**
 *  \brief   get cached vector of string
 *  \param   _handle input: handle
 *  \param   V output: vector of string
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<std::string> & V)
{
    dbr_short_t alarmStatus;
    dbr_short_t alarmSeverity;
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_PRIMITIVE;
    return getCache(_handle, V, alarmStatus, alarmSeverity, ts);
};
//0
/**
 *  \brief   get cached vector of string, alarmStatus, alarmSeverity
 *  \param   _handle input: handle
 *  \param   V output: vector of string
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<std::string> & V,
                    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity)
{
    epicsTimeStamp ts;
    alarmStatus=0;
    alarmSeverity=0;
    ts.secPastEpoch=0;
    ts.nsec=0;
    vectorDbrType=CAFENUM::DBR_STS;
    return getCache(_handle, V, alarmStatus, alarmSeverity, ts);
};


//6
/**
 *  \brief   get cached vector of doubles, alarm status/severity and epicsTimeStamp
 *  \param   _handle input: handle
 *  \param   V output: vector of doubles
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \param   ts output: epicsTimeStamp ts.secPastEpoch, ts.nsec
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<double> & V,
                    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{

    V.clear();
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        //Reserve only what will be in the request data buffer.
        unsigned int  nelemThatWillBeRequested=
            min((*it_handle).getChannelRequestMetaData().getNelem(),(*it_handle).getChannelRequestMetaData().getNelemCache());


        V.reserve(nelemThatWillBeRequested);


        double * val = new double [nelemThatWillBeRequested];

        if(vectorDbrType==CAFENUM::DBR_NONE)
        {
            vectorDbrType=CAFENUM::DBR_TIME;
        };
        switch (vectorDbrType)
        {
        case CAFENUM::DBR_PRIMITIVE:
            status=cafeDoppio.getCache (_handle, DBR_DOUBLE, val);
            break;
        case CAFENUM::DBR_STS:
            status=cafeDoppio.getCache (_handle, DBR_STS_DOUBLE, val, alarmStatus, alarmSeverity);
            break;
        case CAFENUM::DBR_TIME:
        default:
            status=cafeDoppio.getCache (_handle, DBR_TIME_DOUBLE, val, alarmStatus, alarmSeverity, ts);
            break;
        }
        vectorDbrType=CAFENUM::DBR_NONE;

        if (status==ICAFE_NORMAL)
        {
            // for (unsigned int   i=0; i<(*it_handle).getChannelRequestMetaData().getNelem(); ++i) {
            //    V.push_back(val[i]);
            // }
            V.insert(V.end(), &val[0], &val[nelemThatWillBeRequested]);
        }

        delete [] val;
        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};

//5
/**
 *  \brief   get vector of cached dbr_long_t, alarm status/severity and epicsTimeStamp
 *  \param   _handle input: handle
 *  \param   V output: vector of dbr_long_t
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \param   ts output: epicsTimeStamp ts.secPastEpoch, ts.nsec
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<dbr_long_t> & V,
                    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{

    V.clear();

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        //Reserve only what will be in the request data buffer.
        unsigned int  nelemThatWillBeRequested=
            min((*it_handle).getChannelRequestMetaData().getNelem(),(*it_handle).getChannelRequestMetaData().getNelemCache());


        V.reserve(nelemThatWillBeRequested);

        dbr_long_t * val = new dbr_long_t [nelemThatWillBeRequested];


        if(vectorDbrType==CAFENUM::DBR_NONE)
        {
            vectorDbrType=CAFENUM::DBR_TIME;
        };
        switch (vectorDbrType)
        {
        case CAFENUM::DBR_PRIMITIVE:
            status=cafeLatte.getCache (_handle, DBR_LONG, val);
            break;
        case CAFENUM::DBR_STS:
            status=cafeLatte.getCache (_handle, DBR_STS_LONG, val, alarmStatus, alarmSeverity);
            break;
        case CAFENUM::DBR_TIME:
        default:
            status=cafeLatte.getCache (_handle, DBR_TIME_LONG, val, alarmStatus, alarmSeverity, ts);
            break;
        }

        vectorDbrType=CAFENUM::DBR_NONE;


        if (status==ICAFE_NORMAL)
        {
            //for (unsigned int   i=0; i<(*it_handle).getChannelRequestMetaData().getNelem(); ++i) {
            //     V.push_back(val[i]);
            // }
            V.insert(V.end(), &val[0], &val[nelemThatWillBeRequested]);
        }

        delete [] val;
        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};

//5+
/**
 *  \brief   get vector of long lonsg, alarm status/severity and epicsTimeStamp
 *  \param   _handle input: handle
 *  \param   V output: vector of long longs
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \param   ts output: epicsTimeStamp ts.secPastEpoch, ts.nsec
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<long long> & V,
                    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{
#define __METHOD__ \
    "CAFE::getCache(const unsigned int _handle, vector<long long> & V, dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)"
    V.clear();

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        //Reserve only what will be in the request data buffer.
        unsigned int  nelemThatWillBeRequested=
            min((*it_handle).getChannelRequestMetaData().getNelem(),(*it_handle).getChannelRequestMetaData().getNelemCache());

        V.reserve(nelemThatWillBeRequested);

        //What is the native type?
        if ( (*it_handle).getChannelRequestMetaData().getDataType() == DBR_STRING)
        {

            dbr_string_t * val = new dbr_string_t[nelemThatWillBeRequested];

            if(vectorDbrType==CAFENUM::DBR_NONE)
            {
                vectorDbrType=CAFENUM::DBR_TIME;
            };

            switch (vectorDbrType)
            {
            case CAFENUM::DBR_PRIMITIVE:
                status=cafeSoluble.getCache (_handle, DBR_STRING, val);
                break;
            case CAFENUM::DBR_STS:
                status=cafeSoluble.getCache (_handle, DBR_STS_STRING, val, alarmStatus, alarmSeverity);
                break;
            case CAFENUM::DBR_TIME:
            default:
                status=cafeSoluble.getCache (_handle, DBR_TIME_STRING, val, alarmStatus, alarmSeverity, ts);
                break;
            }

            if (status==ICAFE_NORMAL)
            {
                istringstream ss;
                bool allStringsAreValid=true;

                for (unsigned short i=0; i<nelemThatWillBeRequested; ++i)
                {
                    long long l=0;
                    ss.clear();
                    ss.str(val[i]);
                    ss>>l;

                    if ( !ss.fail())
                    {
                        V.push_back(l);
                        std::string strInput=val[i];
                        std::stringstream ssOut;
                        ssOut << l;
                        std::string strOutput=ssOut.str();
                        if (strInput!=strOutput)
                        {
                            cout << __METHOD__ << "//" << __LINE__ << endl;
                            cout << "***WARNING*** STRING TO LONG LONG CONVERSION REPORTS: " << endl;
                            cout << "STRING VALUE: " << strInput << " CONVERTED TO: " << strOutput << endl;
                        }
                    }
                    else
                    {
                        cout << __METHOD__ << "//" << __LINE__ << endl;
                        cout << "***WARNING*** NO STRING TO LONG LONG  CONVERSION for ELEMENT " << i \
                             << " of " << (*it_handle).getChannelRequestMetaData().getNelem() << " !! " << endl;
                        cout << "***WARNING*** COULD NOT CONVERT: ";
                        cout << val[i];
                        cout << " TO LONG LONG ; CONVERSION NOT MADE!" << endl;
                        cout << " DATA POINT OMITTED. LENGTH OF RETURNING VECTOR WILL BE DIFFERENT TO EXPECTED "<< endl;
                        allStringsAreValid=false;
                    }
                }

                if(!allStringsAreValid)
                {
                    cout << "----------------------------------------------------------------" << endl;
                    cout << "NOT ALL ELEMENTS OF STRING ARRAY COULD BE CONVERTED TO LONG LONG " << endl;
                    cout << "THESE HAVE A VALUE OF ZERO IN PLACE OF STRING VALUE " << endl;
                    cout << "----------------------------------------------------------------" << endl;
                    // cout << " RETURNING VECTOR HAS LENGTH " << V.size() << " RATHER THAN THE EXPECTED " <<
                    //   (*it_handle).getChannelRequestMetaData().getNelem()   << endl;
                }
            }
            delete  [] val;
        }


        else if ( (*it_handle).getChannelRequestMetaData().getDataType()  == DBR_DOUBLE)
        {

            dbr_double_t * val = new dbr_double_t[nelemThatWillBeRequested];

            if(vectorDbrType==CAFENUM::DBR_NONE)
            {
                vectorDbrType=CAFENUM::DBR_TIME;
            };

            switch (vectorDbrType)
            {
            case CAFENUM::DBR_PRIMITIVE:
                status=cafeDoppio.getCache (_handle, DBR_DOUBLE, val);
                break;
            case CAFENUM::DBR_STS:
                status=cafeDoppio.getCache (_handle, DBR_STS_DOUBLE, val, alarmStatus, alarmSeverity);
                break;
            case CAFENUM::DBR_TIME:
            default:
                status=cafeDoppio.getCache (_handle, DBR_TIME_DOUBLE, val, alarmStatus, alarmSeverity, ts);
                break;
            }

            if (status==ICAFE_NORMAL)
            {
                for (unsigned short i=0; i<nelemThatWillBeRequested; ++i)
                {
                    V.push_back((long long) val[i]);
                }
                //	V.insert(V.end(), &val[0], &val[(*it_handle).getChannelRequestMetaData().getNelem()]);
            }

            delete [] val;
        }
        else if ( (*it_handle).getChannelRequestMetaData().getDataType()  == DBR_FLOAT)
        {

            dbr_float_t * val = new dbr_float_t[nelemThatWillBeRequested];
            if(vectorDbrType==CAFENUM::DBR_NONE)
            {
                vectorDbrType=CAFENUM::DBR_TIME;
            };
            switch (vectorDbrType)
            {
            case CAFENUM::DBR_PRIMITIVE:
                status=cafeFrappuccino.getCache (_handle, DBR_FLOAT, val);
                break;
            case CAFENUM::DBR_STS:
                status=cafeFrappuccino.getCache (_handle, DBR_STS_FLOAT, val, alarmStatus, alarmSeverity);
                break;
            case CAFENUM::DBR_TIME:
            default:
                status=cafeFrappuccino.getCache (_handle, DBR_TIME_FLOAT, val, alarmStatus, alarmSeverity, ts);
                break;
            }

            if (status==ICAFE_NORMAL)
            {
                for (unsigned short i=0; i<nelemThatWillBeRequested; ++i)
                {
                    V.push_back((long long) val[i]);
                }
                //	V.insert(V.end(), &val[0], &val[(*it_handle).getChannelRequestMetaData().getNelem()]);
            }
            delete [] val;
        }


        else
        {

            dbr_long_t * val = new dbr_long_t [nelemThatWillBeRequested];

            if(vectorDbrType==CAFENUM::DBR_NONE)
            {
                vectorDbrType=CAFENUM::DBR_TIME;
            };

            switch (vectorDbrType)
            {
            case CAFENUM::DBR_PRIMITIVE:
                status=cafeLatte.getCache (_handle, DBR_LONG, val);
                break;
            case CAFENUM::DBR_STS:
                status=cafeLatte.getCache (_handle, DBR_STS_LONG, val, alarmStatus, alarmSeverity);
                break;
            case CAFENUM::DBR_TIME:
            default:
                status=cafeLatte.getCache (_handle, DBR_TIME_LONG, val, alarmStatus, alarmSeverity, ts);
                break;
            }

            if (status==ICAFE_NORMAL)
            {
                //for (unsigned short i=0; i<(*it_handle).getChannelRequestMetaData().getNelem(); ++i) {
                //    V.push_back((long long) val[i]);
                //}
                V.insert(V.end(), &val[0], &val[nelemThatWillBeRequested]);
            }

            delete []val;

        }

        vectorDbrType=CAFENUM::DBR_NONE;

        return status;

    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
#undef __METHOD__
};


//4
/**
 *  \brief   get vector of cached unsigned char, alarm status/severity and epicsTimeStamp
 *  \param   _handle input: handle
 *  \param   V output: vector of unsigned char
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \param   ts output: epicsTimeStamp ts.secPastEpoch, ts.nsec
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<unsigned char> & V,
                    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{

    V.clear();
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        //Reserve only what will be in the request data buffer.
        unsigned int  nelemThatWillBeRequested=
            min((*it_handle).getChannelRequestMetaData().getNelem(),(*it_handle).getChannelRequestMetaData().getNelemCache());

        V.reserve(nelemThatWillBeRequested);

        unsigned char * val = new unsigned char [nelemThatWillBeRequested];

        if(vectorDbrType==CAFENUM::DBR_NONE)
        {
            vectorDbrType=CAFENUM::DBR_TIME;
        };

        switch (vectorDbrType)
        {
        case CAFENUM::DBR_PRIMITIVE:
            status=cafeCappuccino.getCache (_handle, DBR_CHAR, val);
            break;
        case CAFENUM::DBR_STS:
            status=cafeCappuccino.getCache (_handle, DBR_STS_CHAR, val, alarmStatus, alarmSeverity);
            break;
        case CAFENUM::DBR_TIME:
        default:
            status=cafeCappuccino.getCache (_handle, DBR_TIME_CHAR, val, alarmStatus, alarmSeverity, ts);
            break;
        }
        vectorDbrType=CAFENUM::DBR_NONE;


        if (status==ICAFE_NORMAL)
        {
            for (unsigned int  i=0; i<nelemThatWillBeRequested; ++i)
            {
                V.push_back(val[i]);
            }
            //	V.insert(V.end(), &val[0], &val[(*it_handle).getChannelRequestMetaData().getNelem()]);
        }

        delete []val;
        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};

//3
/**
 *  \brief   get cached vector of unsigned shorts (enums), alarm status/severity and epicsTimeStamp
 *  \param   _handle input: handle
 *  \param   V output: vector of unsigned shorts (enum)
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \param   ts output: epicsTimeStamp ts.secPastEpoch, ts.nsec
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache (const unsigned int  _handle, std::vector<unsigned short> & V,
                     dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{

    V.clear();

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        //Reserve only what will be in the request data buffer.
        unsigned int  nelemThatWillBeRequested=
            min((*it_handle).getChannelRequestMetaData().getNelem(),(*it_handle).getChannelRequestMetaData().getNelemCache());

        V.reserve(nelemThatWillBeRequested);

        unsigned short * val = new unsigned short [nelemThatWillBeRequested];

        if(vectorDbrType==CAFENUM::DBR_NONE)
        {
            vectorDbrType=CAFENUM::DBR_TIME;
        };
        switch (vectorDbrType)
        {
        case CAFENUM::DBR_PRIMITIVE:
            status=cafeEspresso.getCache  (_handle, DBR_ENUM, val);
            break;
        case CAFENUM::DBR_STS:
            status=cafeEspresso.getCache  (_handle, DBR_STS_ENUM, val, alarmStatus, alarmSeverity);
            break;
        case CAFENUM::DBR_TIME:
        default:
            status=cafeEspresso.getCache (_handle, DBR_TIME_ENUM, val, alarmStatus, alarmSeverity, ts);
            break;
        }
        vectorDbrType=CAFENUM::DBR_NONE;


        if (status==ICAFE_NORMAL)
        {
            //for (unsigned int   i=0; i<(*it_handle).getChannelRequestMetaData().getNelem(); ++i) {
            //    V.push_back(val[i]);
            //}
            V.insert(V.end(), &val[0], &val[nelemThatWillBeRequested]);
        }

        delete []val;
        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};

//2
/**
 *  \brief   get cached vector of floats, alarm status/severity and epicsTimeStamp
 *  \param   _handle input: handle
 *  \param   V output: vector of floats
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \param   ts output: epicsTimeStamp ts.secPastEpoch, ts.nsec
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<float> & V,
                    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{

    V.clear();

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        //Reserve only what will be in the request data buffer.
        unsigned int nelemThatWillBeRequested=
            min((*it_handle).getChannelRequestMetaData().getNelem(),(*it_handle).getChannelRequestMetaData().getNelemCache());

        V.reserve(nelemThatWillBeRequested);

        //cout << "nelemThatWillBeRequested " << nelemThatWillBeRequested << endl;
        //cout << "data cache    " << (*it_handle).getChannelRequestMetaData().getNelemCache()  << endl;
        //cout << "client nelem  " << (*it_handle).getChannelRequestMetaDataClient().getNelem()  << endl;
        //cout << "client cache  " << (*it_handle).getChannelRequestMetaDataClient().getNelemCache()  << endl;

        float * val = new float [nelemThatWillBeRequested];

        if(vectorDbrType==CAFENUM::DBR_NONE)
        {
            vectorDbrType=CAFENUM::DBR_TIME;
        };

        switch (vectorDbrType)
        {
        case CAFENUM::DBR_PRIMITIVE:
            status=cafeFrappuccino.getCache(_handle, DBR_FLOAT, val);
            break;
        case CAFENUM::DBR_STS:
            status=cafeFrappuccino.getCache(_handle, DBR_STS_FLOAT, val, alarmStatus, alarmSeverity);
            break;
        case CAFENUM::DBR_TIME:
        default:
            status=cafeFrappuccino.getCache(_handle, DBR_TIME_FLOAT, val, alarmStatus, alarmSeverity, ts);
            break;
        }
        vectorDbrType=CAFENUM::DBR_NONE;


        if (status==ICAFE_NORMAL)
        {
            for (unsigned int  i=0; i<nelemThatWillBeRequested; ++i)
            {
                V.push_back(val[i]);
            }
            //	V.insert(V.end(), &val[0], &val[(*it_handle).getChannelRequestMetaData().getNelem()]);
        }



        delete []val;
        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};


//1
/**
 *  \brief   get cached vector of shorts, alarm status/severity and epicsTimeStamp
 *  \param   _handle input: handle
 *  \param   V output: vector of shorts
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \param   ts output: epicsTimeStamp ts.secPastEpoch, ts.nsec
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<short> & V,
                    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{

    V.clear();

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {


        //Reserve only what will be in the request data buffer.
        unsigned int  nelemThatWillBeRequested=
            min((*it_handle).getChannelRequestMetaData().getNelem(),(*it_handle).getChannelRequestMetaData().getNelemCache());


        V.reserve(nelemThatWillBeRequested);

        short * val = new short [nelemThatWillBeRequested];

        if(vectorDbrType==CAFENUM::DBR_NONE)
        {
            vectorDbrType=CAFENUM::DBR_TIME;
        };
        switch (vectorDbrType)
        {
        case CAFENUM::DBR_PRIMITIVE:
            status=cafeSchale.getCache (_handle, DBR_SHORT, val);
            break;
        case CAFENUM::DBR_STS:
            status=cafeSchale.getCache (_handle, DBR_STS_SHORT, val, alarmStatus, alarmSeverity);
            break;
        case CAFENUM::DBR_TIME:
        default:
            status=cafeSchale.getCache (_handle, DBR_TIME_SHORT, val, alarmStatus, alarmSeverity, ts);
            break;
        }
        vectorDbrType=CAFENUM::DBR_NONE;

        if (status==ICAFE_NORMAL)
        {
            for (unsigned int  i=0; i<nelemThatWillBeRequested; ++i)
            {
                V.push_back(val[i]);
            }
            //	V.insert(V.end(), &val[0], &val[(*it_handle).getChannelRequestMetaData().getNelem()]);
        }

        delete []val;
        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};



//0
/**
 *  \brief   get cached vector of strings, alarm status/severity and epicsTimeStamp
 *  \param   _handle input: handle
 *  \param   V output: vector of strings
 *  \param   alarmStatus   output: NO_ALARM, READ, WRITE, HIHI, HIGH, READ_ACCESS, \n
 *           LOLO, LOW, STATE, COS, COMM, WRITE_ACCESS, TIMEOUT, HWLIMIT, CALC,\n
 *           SCAN, LINK, SOFT, BAD_SUB, UDF, DISABLE, SIMM
 *  \param   alarmSeverity output: NO_ALARM, MINOR, MAJOR, INVALID
 *  \param   ts output: epicsTimeStamp ts.secPastEpoch, ts.nsec
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::getCache(const unsigned int  _handle, std::vector<std::string> & V,
                    dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts)
{

    V.clear();

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        //Reserve only what will be in the request data buffer.
        unsigned int  nelemThatWillBeRequested=
            min((*it_handle).getChannelRequestMetaData().getNelem(),(*it_handle).getChannelRequestMetaData().getNelemCache());


        V.reserve(nelemThatWillBeRequested);

        dbr_string_t * vals = new dbr_string_t [nelemThatWillBeRequested];

        if(vectorDbrType==CAFENUM::DBR_NONE)
        {
            vectorDbrType=CAFENUM::DBR_TIME;
        };
        switch (vectorDbrType)
        {
        case CAFENUM::DBR_PRIMITIVE:
            status=cafeSoluble.getCache (_handle, DBR_STRING, vals);
            break;
        case CAFENUM::DBR_STS:
            status=cafeSoluble.getCache (_handle, DBR_STS_STRING, vals, alarmStatus, alarmSeverity);
            break;
        case CAFENUM::DBR_TIME:
        default:
            status=cafeSoluble.getCache (_handle, DBR_TIME_STRING, vals, alarmStatus, alarmSeverity, ts);
            break;
        }

        vectorDbrType=CAFENUM::DBR_NONE;

        if (status==ICAFE_NORMAL)
        {
            for (unsigned int   i=0; i<nelemThatWillBeRequested; ++i)
            {
                V.push_back(vals[i]);
            }
            //V.insert(V.end(), &vals[0], &vals[(*it_handle).getChannelRequestMetaData().getNelem()]);
        }

        delete []vals;
        return status;

    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
}


/**
 *  \brief   get for vector of handles
 *  \param   handleV input:  vector of handles
 *  \param   statusV output: vector of longs
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int  CAFE::get(std::vector<unsigned int> handleV, std::vector<int> &statusV)
{
    int  overallStatus=ICAFE_NORMAL;
    bool isGood=true;

    statusV.clear();
    statusV.reserve(handleV.size());

    for (unsigned int  i=0; i<handleV.size(); ++i)
    {

        //Does already have a monitor?
        if (handleHelper.getNmonitorData(handleV[i])>0)
        {
            statusV.push_back(ICAFE_NORMAL); // ICAFE_HAS_MONITOR_GET_DONE_FROM_CACHE);
        }
        else
        {
            statusV.push_back(get(handleV[i]));
        }
        if ( (statusV[i] != ICAFE_NORMAL) && isGood)
        {
            overallStatus=statusV[i];
            isGood=false;
        }
    }
    //ca_flush_io();
    return overallStatus; // report first failure in return statement
}


//0
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<std::string> & V, std::vector<int> &statusV)
{
    vector<dbr_short_t> alarmStatusV;
    vector<dbr_short_t> alarmSeverityV;
    vector<epicsTimeStamp> tsV;
    return getCache(handleV, V, statusV, alarmStatusV, alarmSeverityV, tsV);
};
//1
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_short_t> & V, std::vector<int> &statusV)
{
    vector<dbr_short_t> alarmStatusV;
    vector<dbr_short_t> alarmSeverityV;
    vector<epicsTimeStamp> tsV;
    return getCache(handleV, V, statusV, alarmStatusV, alarmSeverityV, tsV);
};
//2
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_float_t> & V, std::vector<int> &statusV)
{
    vector<dbr_short_t> alarmStatusV;
    vector<dbr_short_t> alarmSeverityV;
    vector<epicsTimeStamp> tsV;
    return getCache(handleV, V, statusV, alarmStatusV, alarmSeverityV, tsV);
};
//3
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_enum_t> & V, std::vector<int> &statusV)
{
    vector<dbr_short_t> alarmStatusV;
    vector<dbr_short_t> alarmSeverityV;
    vector<epicsTimeStamp> tsV;
    return getCache(handleV, V, statusV, alarmStatusV, alarmSeverityV, tsV);
};
//4
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_char_t> & V, std::vector<int> &statusV)
{
    vector<dbr_short_t> alarmStatusV;
    vector<dbr_short_t> alarmSeverityV;
    vector<epicsTimeStamp> tsV;
    return getCache(handleV, V, statusV, alarmStatusV, alarmSeverityV, tsV);
};
//5
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_long_t> & V, std::vector<int> &statusV)
{
    vector<dbr_short_t> alarmStatusV;
    vector<dbr_short_t> alarmSeverityV;
    vector<epicsTimeStamp> tsV;
    return getCache(handleV, V, statusV, alarmStatusV, alarmSeverityV, tsV);
};
//5
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<long long> & V, std::vector<int> &statusV)
{
    vector<dbr_short_t> alarmStatusV;
    vector<dbr_short_t> alarmSeverityV;
    vector<epicsTimeStamp> tsV;
    return getCache(handleV, V, statusV, alarmStatusV, alarmSeverityV, tsV);
};
//6
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_double_t> & V, std::vector<int> &statusV)
{
    vector<dbr_short_t> alarmStatusV;
    vector<dbr_short_t> alarmSeverityV;
    vector<epicsTimeStamp> tsV;
    return getCache(handleV, V, statusV, alarmStatusV, alarmSeverityV, tsV);
};

//0
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<std::string> & V, std::vector<int> &statusV,
                    std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV)
{
    vector<epicsTimeStamp> tsV;
    return getCache(handleV, V, statusV, alarmStatusV, alarmSeverityV, tsV);
};
//1
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_short_t> & V, std::vector<int> &statusV,
                    std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV)
{
    vector<epicsTimeStamp> tsV;
    return getCache(handleV, V, statusV, alarmStatusV, alarmSeverityV, tsV);
};
//2
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_float_t> & V, std::vector<int> &statusV,
                    std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV)
{
    vector<epicsTimeStamp> tsV;
    return getCache(handleV, V, statusV, alarmStatusV, alarmSeverityV, tsV);
};
//3
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_enum_t> & V, std::vector<int> &statusV,
                    std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV)
{
    vector<epicsTimeStamp> tsV;
    return getCache(handleV, V, statusV, alarmStatusV, alarmSeverityV, tsV);
};
//4
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_char_t> & V, std::vector<int> &statusV,
                    std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV)
{
    vector<epicsTimeStamp> tsV;
    return getCache(handleV, V, statusV, alarmStatusV, alarmSeverityV, tsV);
};
//5
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_long_t> & V, std::vector<int> &statusV,
                    std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV)
{
    vector<epicsTimeStamp> tsV;
    return getCache(handleV, V, statusV, alarmStatusV, alarmSeverityV, tsV);
};
//5
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<long long> & V, std::vector<int> &statusV,
                    std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV)
{
    vector<epicsTimeStamp> tsV;
    return getCache(handleV, V, statusV, alarmStatusV, alarmSeverityV, tsV);
};

//6
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_double_t> & V, std::vector<int> &statusV,
                    std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV)
{
    vector<epicsTimeStamp> tsV;
    return getCache(handleV, V, statusV, alarmStatusV, alarmSeverityV, tsV);
};

//0
/**
 *  \brief   get cached vector of strings
 *  \param   handleV input: vector of handles
 *  \param   V output: vector of strings
 *  \param   statusV output: vector of longs
 *  \param   alarmStatusV output: vector of alarm status
 *  \param   alarmSeverityV output: vecor of alarm severity
 *  \param   tsV output: vector of EpicsTimeStamp
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<std::string> & V, std::vector<int> &statusV,
                    std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV,
                    std::vector<epicsTimeStamp> &tsV)
{

    int  overallStatus=ICAFE_NORMAL;

    V.clear();
    statusV.clear();
    alarmStatusV.clear();
    alarmSeverityV.clear();
    tsV.clear();

    V.reserve(handleV.size());
    statusV.reserve(handleV.size());
    alarmStatusV.reserve(handleV.size());
    alarmSeverityV.reserve(handleV.size());
    tsV.reserve(handleV.size());

    //unsigned int     * h = new unsigned int  [handleV.size()];
    dbr_string_t * value = new dbr_string_t  [handleV.size()];
    int  * stats = new int  [handleV.size()];
    dbr_short_t * astat = new dbr_short_t    [handleV.size()];
    dbr_short_t * asev  = new dbr_short_t    [handleV.size()];
    epicsTimeStamp *ets = new epicsTimeStamp [handleV.size()];

    for (unsigned int  i=0; i<handleV.size(); ++i)
    {
        //h[i] = handleV[i];
        strcpy(value[i],"");
    }

    overallStatus = getCache(&handleV[0], (unsigned int) handleV.size(), value, stats, astat, asev, ets);

    for (unsigned int   i=0; i<handleV.size(); ++i)
    {
        V.push_back(value[i]);
        statusV.push_back(stats[i]);
        alarmStatusV.push_back(astat[i]);
        alarmSeverityV.push_back(asev[i]);
        tsV.push_back(ets[i]);
    }

    //delete h;
    delete []value;
    delete []stats;
    delete []astat;
    delete []asev;
    delete []ets;

    return overallStatus; // ICAFE_NORMAL else reports first failure
}


//1
/**
 *  \brief   get cached vector of shorts
 *  \param   handleV input: vector of handles
 *  \param   V output: vector of shorts
 *  \param   statusV output: vector of longs
 *  \param   alarmStatusV output: vector of alarm status
 *  \param   alarmSeverityV output: vecor of alarm severity
 *  \param   tsV output: vector of EpicsTimeStamp
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_short_t> & V, std::vector<int> &statusV,
                    std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV,
                    std::vector<epicsTimeStamp> &tsV)
{

    int  overallStatus=ICAFE_NORMAL;

    V.clear();
    statusV.clear();
    alarmStatusV.clear();
    alarmSeverityV.clear();
    tsV.clear();

    V.reserve(handleV.size());
    statusV.reserve(handleV.size());
    alarmStatusV.reserve(handleV.size());
    alarmSeverityV.reserve(handleV.size());
    tsV.reserve(handleV.size());

    // unsigned int    * h = new unsigned int   [handleV.size()];
    dbr_short_t * value = new dbr_short_t    [handleV.size()];
    int  * stats = new int  [handleV.size()];
    dbr_short_t * astat = new dbr_short_t    [handleV.size()];
    dbr_short_t * asev  = new dbr_short_t    [handleV.size()];
    epicsTimeStamp *ets = new epicsTimeStamp [handleV.size()];

    for (unsigned int  i=0; i<handleV.size(); ++i)
    {
        //h[i] = handleV[i];
        value[i]=0; // initialize to zero
    }

    overallStatus = getCache(&handleV[0], (unsigned int) handleV.size(), value, stats, astat, asev, ets);

    for (unsigned int   i=0; i<handleV.size(); ++i)
    {
        V.push_back(value[i]);
        statusV.push_back(stats[i]);
        alarmStatusV.push_back(astat[i]);
        alarmSeverityV.push_back(asev[i]);
        tsV.push_back(ets[i]);
    }

    //delete h;
    delete []value;
    delete []stats;
    delete []astat;
    delete []asev;
    delete []ets;

    return overallStatus; // ICAFE_NORMAL else reports first failure
}


//2
/**
 *  \brief   get cached vector of floats
 *  \param   handleV input: vector of handles
 *  \param   V output: vector of floats
 *  \param   statusV output: vector of longs
 *  \param   alarmStatusV output: vector of alarm status
 *  \param   alarmSeverityV output: vecor of alarm severity
 *  \param   tsV output: vector of EpicsTimeStamp
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_float_t> & V, std::vector<int> &statusV,
                    std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV,
                    std::vector<epicsTimeStamp> &tsV)
{

    int  overallStatus=ICAFE_NORMAL;

    V.clear();
    statusV.clear();
    alarmStatusV.clear();
    alarmSeverityV.clear();
    tsV.clear();

    V.reserve(handleV.size());
    statusV.reserve(handleV.size());
    alarmStatusV.reserve(handleV.size());
    alarmSeverityV.reserve(handleV.size());
    tsV.reserve(handleV.size());

    //unsigned int    * h = new unsigned int   [handleV.size()];
    dbr_float_t * value = new dbr_float_t    [handleV.size()];
    int  * stats = new int  [handleV.size()];
    dbr_short_t * astat = new dbr_short_t    [handleV.size()];
    dbr_short_t * asev  = new dbr_short_t    [handleV.size()];
    epicsTimeStamp *ets = new epicsTimeStamp [handleV.size()];

    for (unsigned int  i=0; i<handleV.size(); ++i)
    {
        //h[i] = handleV[i];
        value[i]=0; // initialize to zero
    }

    overallStatus = getCache(&handleV[0], (unsigned int) handleV.size(), value, stats, astat, asev, ets);

    for (unsigned int   i=0; i<handleV.size(); ++i)
    {
        V.push_back(value[i]);
        statusV.push_back(stats[i]);
        alarmStatusV.push_back(astat[i]);
        alarmSeverityV.push_back(asev[i]);
        tsV.push_back(ets[i]);
    }

    //delete h;
    delete []value;
    delete []stats;
    delete []astat;
    delete []asev;
    delete []ets;

    return overallStatus; // ICAFE_NORMAL else reports first failure
}


//3
/**
 *  \brief   get cached vector of enums (ushorts)
 *  \param   handleV input: vector of handles
 *  \param   V output: vector of enums (ushorts)
 *  \param   statusV output: vector of longs
 *  \param   alarmStatusV output: vector of alarm status
 *  \param   alarmSeverityV output: vecor of alarm severity
 *  \param   tsV output: vector of EpicsTimeStamp
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_enum_t> & V, std::vector<int> &statusV,
                    std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV,
                    std::vector<epicsTimeStamp> &tsV)
{

    int  overallStatus=ICAFE_NORMAL;

    V.clear();
    statusV.clear();
    alarmStatusV.clear();
    alarmSeverityV.clear();
    tsV.clear();

    V.reserve(handleV.size());
    statusV.reserve(handleV.size());
    alarmStatusV.reserve(handleV.size());
    alarmSeverityV.reserve(handleV.size());
    tsV.reserve(handleV.size());

    unsigned int    * h = new unsigned int   [handleV.size()];
    dbr_enum_t * value  = new dbr_enum_t     [handleV.size()];
    int  * stats = new int  [handleV.size()];
    dbr_short_t * astat = new dbr_short_t    [handleV.size()];
    dbr_short_t * asev  = new dbr_short_t    [handleV.size()];
    epicsTimeStamp *ets = new epicsTimeStamp [handleV.size()];

    for (unsigned int  i=0; i<handleV.size(); ++i)
    {
        h[i] = handleV[i];
        value[i]=0; // initialize to zero
    }


    overallStatus = getCache(h, (unsigned int) handleV.size(), value, stats, astat, asev, ets);


    for (unsigned int   i=0; i<handleV.size(); ++i)
    {
        V.push_back(value[i]);
        statusV.push_back(stats[i]);
        alarmStatusV.push_back(astat[i]);
        alarmSeverityV.push_back(asev[i]);
        tsV.push_back(ets[i]);
    }

    delete []h;
    delete []value;
    delete []stats;
    delete []astat;
    delete []asev;
    delete []ets;

    return overallStatus; // ICAFE_NORMAL else reports first failure
}

//4
/**
 *  \brief   get cached vector of dbr_char_t (unsigned char)
 *  \param   handleV input: vector of handles
 *  \param   V output: vector of dbr_char_t (unsigned char)
 *  \param   statusV output: vector of longs
 *  \param   alarmStatusV output: vector of alarm status
 *  \param   alarmSeverityV output: vecor of alarm severity
 *  \param   tsV output: vector of EpicsTimeStamp
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_char_t> & V, std::vector<int> &statusV,
                    std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV,
                    std::vector<epicsTimeStamp> &tsV)
{

    int  overallStatus=ICAFE_NORMAL;

    V.clear();
    statusV.clear();
    alarmStatusV.clear();
    alarmSeverityV.clear();
    tsV.clear();

    V.reserve(handleV.size());
    statusV.reserve(handleV.size());
    alarmStatusV.reserve(handleV.size());
    alarmSeverityV.reserve(handleV.size());
    tsV.reserve(handleV.size());

    //unsigned int    * h = new unsigned int   [handleV.size()];
    dbr_char_t * value  = new dbr_char_t     [handleV.size()];
    int  * stats = new int  [handleV.size()];
    dbr_short_t * astat = new dbr_short_t    [handleV.size()];
    dbr_short_t * asev  = new dbr_short_t    [handleV.size()];
    epicsTimeStamp *ets = new epicsTimeStamp [handleV.size()];

    for (unsigned int  i=0; i<handleV.size(); ++i)
    {
        //h[i] = handleV[i];
        value[i]=0; // initialize to zero
    }

    overallStatus = getCache(&handleV[0], (unsigned int) handleV.size(), value, stats, astat, asev, ets);

    for (unsigned int   i=0; i<handleV.size(); ++i)
    {
        V.push_back(value[i]);
        statusV.push_back(stats[i]);
        alarmStatusV.push_back(astat[i]);
        alarmSeverityV.push_back(asev[i]);
        tsV.push_back(ets[i]);
    }

    //delete h;
    delete []value;
    delete []stats;
    delete []astat;
    delete []asev;
    delete []ets;

    return overallStatus; // ICAFE_NORMAL else reports first failure
}


//5
/**
 *  \brief   get cached vector of dbr_long_t
 *  \param   handleV input: vector of handles
 *  \param   V output: vector of dbr_long_t
 *  \param   statusV output: vector of longs
 *  \param   alarmStatusV output: vector of alarm status
 *  \param   alarmSeverityV output: vecor of alarm severity
 *  \param   tsV output: vector of EpicsTimeStamp
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_long_t> & V, std::vector<int> &statusV,
                    std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV,
                    std::vector<epicsTimeStamp> &tsV)
{

    int  overallStatus=ICAFE_NORMAL;

    V.clear();
    statusV.clear();
    alarmStatusV.clear();
    alarmSeverityV.clear();
    tsV.clear();

    V.reserve(handleV.size());
    statusV.reserve(handleV.size());
    alarmStatusV.reserve(handleV.size());
    alarmSeverityV.reserve(handleV.size());
    tsV.reserve(handleV.size());

    //unsigned int    * h = new unsigned int   [handleV.size()];
    dbr_long_t * value  = new dbr_long_t     [handleV.size()];
    int  * stats = new int  [handleV.size()];
    dbr_short_t * astat = new dbr_short_t    [handleV.size()];
    dbr_short_t * asev  = new dbr_short_t    [handleV.size()];
    epicsTimeStamp *ets = new epicsTimeStamp [handleV.size()];

    for (unsigned int  i=0; i<handleV.size(); ++i)
    {
        //h[i] = handleV[i];
        value[i]=0; // initialize to zero
    }

    overallStatus = getCache(&handleV[0], (unsigned int) handleV.size(), value, stats, astat, asev, ets);

    for (unsigned int   i=0; i<handleV.size(); ++i)
    {
        V.push_back(value[i]);
        statusV.push_back(stats[i]);
        alarmStatusV.push_back(astat[i]);
        alarmSeverityV.push_back(asev[i]);
        tsV.push_back(ets[i]);
    }

    //delete h;
    delete []value;
    delete []stats;
    delete []astat;
    delete []asev;
    delete []ets;

    return overallStatus; // ICAFE_NORMAL else reports first failure
}



//5
/**
 *  \brief   get cached vector of long long
 *  \param   handleV input: vector of handles
 *  \param   V output: vector of long long
 *  \param   statusV output: vector of longs
 *  \param   alarmStatusV output: vector of alarm status
 *  \param   alarmSeverityV output: vecor of alarm severity
 *  \param   tsV output: vector of EpicsTimeStamp
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<long long> & V, std::vector<int> &statusV,
                    std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV,
                    std::vector<epicsTimeStamp> &tsV)
{

    int  overallStatus=ICAFE_NORMAL;

    V.clear();
    statusV.clear();
    alarmStatusV.clear();
    alarmSeverityV.clear();
    tsV.clear();

    V.reserve(handleV.size());
    statusV.reserve(handleV.size());
    alarmStatusV.reserve(handleV.size());
    alarmSeverityV.reserve(handleV.size());
    tsV.reserve(handleV.size());

    //unsigned int    * h = new unsigned int   [handleV.size()];
    dbr_long_t * value  = new dbr_long_t     [handleV.size()];
    int  * stats = new int  [handleV.size()];
    dbr_short_t * astat = new dbr_short_t    [handleV.size()];
    dbr_short_t * asev  = new dbr_short_t    [handleV.size()];
    epicsTimeStamp *ets = new epicsTimeStamp [handleV.size()];

    for (unsigned int  i=0; i<handleV.size(); ++i)
    {
        //h[i] = handleV[i];
        value[i]=0; // initialize to zero
    }

    overallStatus = getCache(&handleV[0], (unsigned int) handleV.size(), value, stats, astat, asev, ets);

    for (unsigned int   i=0; i<handleV.size(); ++i)
    {
        V.push_back(value[i]);
        statusV.push_back(stats[i]);
        alarmStatusV.push_back(astat[i]);
        alarmSeverityV.push_back(asev[i]);
        tsV.push_back(ets[i]);
    }

    //delete h;
    delete []value;
    delete []stats;
    delete []astat;
    delete []asev;
    delete []ets;

    return overallStatus; // ICAFE_NORMAL else reports first failure
}



//6
/**
 *  \brief   get cached vector of dbr_double_t (double)
 *  \param   handleV input: vector of handles
 *  \param   V output: vector of dbr_double_t (double)
 *  \param   statusV output: vector of longs
 *  \param   alarmStatusV output: vector of alarm status
 *  \param   alarmSeverityV output: vecor of alarm severity
 *  \param   tsV output: vector of EpicsTimeStamp
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_double_t> & V, std::vector<int> &statusV,
                    std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV,
                    std::vector<epicsTimeStamp> &tsV)
{
#define __METHOD__ "int  CAFE::getCache(std::vector<unsigned int> handleV, std::vector<dbr_double_t> & V, std::vector<int> &statusV, \
                    std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV, \
                    std::vector<epicsTimeStamp> &tsV)"

    int  overallStatus=ICAFE_NORMAL;

    V.clear();
    statusV.clear();
    alarmStatusV.clear();
    alarmSeverityV.clear();
    tsV.clear();

    V.reserve(handleV.size());
    statusV.reserve(handleV.size());
    alarmStatusV.reserve(handleV.size());
    alarmSeverityV.reserve(handleV.size());
    tsV.reserve(handleV.size());

    //unsigned int    * h = new unsigned int   [handleV.size()];
    dbr_double_t * value = new dbr_double_t  [handleV.size()];
    int  * stats = new int  [handleV.size()];
    dbr_short_t * astat = new dbr_short_t    [handleV.size()];
    dbr_short_t * asev  = new dbr_short_t    [handleV.size()];
    epicsTimeStamp *ets = new epicsTimeStamp [handleV.size()];

    for (unsigned int  i=0; i<handleV.size(); ++i)
    {
        //h[i] = handleV[i];
        value[i]=0; // initialize to zero
    }


    overallStatus = getCache(&handleV[0], (unsigned int) handleV.size(), value, stats, astat, asev, ets);


    for (unsigned int   i=0; i<handleV.size(); ++i)
    {
        V.push_back(value[i]);
        statusV.push_back(stats[i]);
        alarmStatusV.push_back(astat[i]);
        alarmSeverityV.push_back(asev[i]);
        tsV.push_back(ets[i]);
    }

    //delete h;
    delete []value;
    delete []stats;
    delete []astat;
    delete []asev;
    delete []ets;

    return overallStatus; // ICAFE_NORMAL else reports first failure
#undef __METHOD__
}


/////////////////////////////////////////////////////////////////////////////////////////////////


//0
/**
 *  \brief   set vector of dbr_string_t
 *  \param   _handle input: handle
 *  \param   V input: vector of strings
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::set(const unsigned int  _handle, std::vector<std::string>  V)
{
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        unsigned int  nelemThatWillBeRequested= V.size();
        unsigned int  nelemPrevious=nelemThatWillBeRequested;
        //Check the number of elements requested?
        //JC 25/6/15 Cython must also handle a list of 1!
        //if (nelemThatWillBeRequested>1) {
        //What did the client request? What is native type? Transfer minimum of this.
        if (nelemThatWillBeRequested != (*it_handle).getChannelRequestMetaDataClient().getNelem())
        {
            nelemPrevious=(*it_handle).getChannelRequestMetaDataClient().getNelem();
            nelemThatWillBeRequested= handleHelper.setNelem(_handle, nelemThatWillBeRequested);
        }
        //}
        dbr_string_t * val = new dbr_string_t [nelemThatWillBeRequested];

        for (unsigned int  i=0; i<nelemThatWillBeRequested; ++i )
        {
            strcpy(val[i],V[i].c_str());
        }
        status=cafeSoluble.set (_handle, DBR_STRING, val);
        delete []val;

        if (nelemPrevious!=0)
        {
            handleHelper.setNelem(_handle, nelemPrevious);
        }

        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};


//1
/**
 *  \brief   set vector of dbr_short_t
 *  \param   _handle input: handle
 *  \param   V input: vector of short
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::set(const unsigned int  _handle, std::vector<dbr_short_t>  V)
{
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        unsigned int  nelemThatWillBeRequested= V.size();
        unsigned int  nelemPrevious=nelemThatWillBeRequested;
        //Check the number of elements requested?
        //JC 25/6/15 Cython must also handle a list of 1!
        //if (nelemThatWillBeRequested>1) {
        //What did the client request? What is native type? Transfer minimum of this.
        if (nelemThatWillBeRequested != (*it_handle).getChannelRequestMetaDataClient().getNelem())
        {
            nelemPrevious=(*it_handle).getChannelRequestMetaDataClient().getNelem();
            nelemThatWillBeRequested= handleHelper.setNelem(_handle, nelemThatWillBeRequested);
        }
        //}
        dbr_short_t * val = new dbr_short_t [nelemThatWillBeRequested];
        if (V.size()==nelemThatWillBeRequested)
        {
            std::copy(V.begin(), V.end(), val);
        }
        else
        {
            for (unsigned int  i=0; i<nelemThatWillBeRequested; ++i )
            {
                val[i]=V[i];
            }
        }
        status=cafeSchale.set (_handle, DBR_SHORT, val);
        delete []val;


        //Reset to previous value
        if (nelemPrevious!=nelemThatWillBeRequested)
        {
            handleHelper.setNelem(_handle, nelemPrevious);
        }

        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};

//2
/**
 *  \brief   set vector of dbr_float_t
 *  \param   _handle input: handle
 *  \param   V input: vector of floats
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::set(const unsigned int  _handle, std::vector<dbr_float_t>  V)
{
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        unsigned int  nelemThatWillBeRequested= V.size();
        unsigned int  nelemPrevious=nelemThatWillBeRequested;
        //Check the number of elements requested?
        //JC 25/6/15 Cython must also handle a list of 1!
        //if (nelemThatWillBeRequested>1) {
        //What did the client request? What is native type? Transfer minimum of this.
        if (nelemThatWillBeRequested != (*it_handle).getChannelRequestMetaDataClient().getNelem())
        {
            nelemPrevious=(*it_handle).getChannelRequestMetaDataClient().getNelem();
            nelemThatWillBeRequested= handleHelper.setNelem(_handle, nelemThatWillBeRequested);
        }
        //}
        dbr_float_t * val = new dbr_float_t [nelemThatWillBeRequested];
        if (V.size()==nelemThatWillBeRequested)
        {
            std::copy(V.begin(), V.end(), val);
        }
        else
        {
            for (unsigned int  i=0; i<nelemThatWillBeRequested; ++i )
            {
                val[i]=V[i];
            }
        }
        status=cafeFrappuccino.set (_handle, DBR_FLOAT, val);
        delete []val;


        //Reset to previous value
        if (nelemPrevious!=nelemThatWillBeRequested)
        {
            handleHelper.setNelem(_handle, nelemPrevious);
        }

        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};


//3
/**
 *  \brief   set vector of dbr_enum_t
 *  \param   _handle input: handle
 *  \param   V input: vector of unsigned shorts
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::set(const unsigned int  _handle, std::vector<dbr_enum_t>  V)
{
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        unsigned int  nelemThatWillBeRequested= V.size();
        unsigned int  nelemPrevious=nelemThatWillBeRequested;
        //Check the number of elements requested?
        //JC 25/6/15 Cython must also handle a list of 1!
        //if (nelemThatWillBeRequested>1) {
        //What did the client request? What is native type? Transfer minimum of this.
        if (nelemThatWillBeRequested != (*it_handle).getChannelRequestMetaDataClient().getNelem())
        {
            nelemPrevious=(*it_handle).getChannelRequestMetaDataClient().getNelem();
            nelemThatWillBeRequested= handleHelper.setNelem(_handle, nelemThatWillBeRequested);
        }
        //}
        dbr_enum_t * val = new dbr_enum_t [nelemThatWillBeRequested];
        if (V.size()==nelemThatWillBeRequested)
        {
            std::copy(V.begin(), V.end(), val);
        }
        else
        {
            for (unsigned int  i=0; i<nelemThatWillBeRequested; ++i )
            {
                val[i]=V[i];
            }
        }
        status=cafeEspresso.set (_handle, DBR_ENUM, val);
        delete []val;


        //Reset to previous value
        if (nelemPrevious!=nelemThatWillBeRequested)
        {
            handleHelper.setNelem(_handle, nelemPrevious);
        }

        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};


//4
/**
 *  \brief   set vector of dbr_char_t
 *  \param   _handle input: handle
 *  \param   V input: vector of unsigned chars
 *  \return  ICAFE_NORMAL
 */
int  CAFE::set(const unsigned int  _handle, std::vector<dbr_char_t>  V)
{
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        unsigned int  nelemThatWillBeRequested= V.size();
        unsigned int  nelemPrevious=nelemThatWillBeRequested;
        //Check the number of elements requested?
        //JC 25/6/15 Cython must also handle a list of 1!
        //if (nelemThatWillBeRequested>1) {
        //What did the client request? What is native type? Transfer minimum of this.
        if (nelemThatWillBeRequested != (*it_handle).getChannelRequestMetaDataClient().getNelem())
        {
            nelemPrevious=(*it_handle).getChannelRequestMetaDataClient().getNelem();
            nelemThatWillBeRequested= handleHelper.setNelem(_handle, nelemThatWillBeRequested);
        }
        //}
        dbr_char_t * val = new dbr_char_t [nelemThatWillBeRequested];
        if (V.size()==nelemThatWillBeRequested)
        {
            std::copy(V.begin(), V.end(), val);
        }
        else
        {
            for (unsigned int  i=0; i<nelemThatWillBeRequested; ++i )
            {
                val[i]=V[i];
            }
        }
        status=cafeCappuccino.set (_handle, DBR_CHAR, val);
        delete[] val;
        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};
//5
/**
 *  \brief   set vector of long
 *  \param   _handle input: handle
 *  \param   V input: vector of longs
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::set(const unsigned int  _handle, std::vector<long long> V)
{
    //What is the native data type?

    ChannelRegalia channelInfo;
    CAFE::getChannelInfo(_handle, channelInfo);
    if ( channelInfo.getCafeConnectionState() == ICAFE_CS_NEVER_CONN)
    {
        return ICAFE_CS_NEVER_CONN;
    }
    else if ( channelInfo.getCafeConnectionState()==ICAFE_CS_CLOSED)
    {
        return ICAFE_CS_CLOSED;
    }

    switch ( channelInfo.getDataType())
    {

    case DBR_DOUBLE:
    {
        //cpy vector
        vector<dbr_double_t> Vl;
        Vl.clear();
        Vl.reserve(V.size());
        Vl.assign(V.begin(), V.end());
        return set(_handle, Vl);
    }
    case DBR_FLOAT:
    {
        //cpy vector
        vector<dbr_float_t> Vl;
        Vl.clear();
        Vl.reserve(V.size());
        Vl.assign(V.begin(), V.end());
        return set(_handle, Vl);
    }
    case DBR_LONG:
    {
        //cpy vector
        vector<dbr_long_t> Vl;
        Vl.clear();
        Vl.reserve(V.size());
        Vl.assign(V.begin(), V.end());
        return set(_handle, Vl);
    }
    case DBR_SHORT:
    {
        //cpy vector
        vector<dbr_short_t> Vl;
        Vl.clear();
        Vl.reserve(V.size());
        Vl.assign(V.begin(), V.end());
        return set(_handle, Vl);
    }
    case DBR_ENUM:
    {
        //cpy vector
        vector<dbr_enum_t> Vl;
        Vl.clear();
        Vl.reserve(V.size());
        Vl.assign(V.begin(), V.end());
        return set(_handle, Vl);
    }
    case DBR_CHAR:
    {
        //cpy vector
        vector<dbr_char_t> Vl;
        Vl.clear();
        Vl.reserve(V.size());
        Vl.assign(V.begin(), V.end());
        return set(_handle, Vl);
    }
    case DBR_STRING:
    default:
    {
        vector<string> Vl;
        Vl.clear();
        Vl.reserve(V.size());
        std::stringstream ss;

        for (unsigned int i=0; i< V.size(); ++i)
        {
            ss.clear();
            ss << V[i];
            Vl.push_back(ss.str());
        }
        return set(_handle, Vl);
        break;
    }
    }//switch
}
//5
/**
 *  \brief   set vector of dbr_long_t (int)
 *  \param   _handle input: handle
 *  \param   V input: vector of longs
 *  \return  ICAFE_NORMAL if all OK
 */
int  CAFE::set(const unsigned int  _handle, std::vector<dbr_long_t> V)
{
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        unsigned int  nelemThatWillBeRequested= V.size();
        unsigned int  nelemPrevious=nelemThatWillBeRequested;
        //Check the number of elements requested?

        //JC 25/6/15 Cython must also handle a list of 1!
        //if (nelemThatWillBeRequested>1) {
        //What did the client request? What is native type? Transfer minimum of this.
        if (nelemThatWillBeRequested != (*it_handle).getChannelRequestMetaDataClient().getNelem())
        {
            nelemPrevious=(*it_handle).getChannelRequestMetaDataClient().getNelem();
            nelemThatWillBeRequested= handleHelper.setNelem(_handle, nelemThatWillBeRequested);
        }
        //}


        dbr_long_t * val = new dbr_long_t [nelemThatWillBeRequested];
        if (V.size()==nelemThatWillBeRequested)
        {
            std::copy(V.begin(), V.end(), val);
        }
        else
        {
            for (unsigned int  i=0; i<nelemThatWillBeRequested; ++i )
            {
                val[i]=V[i];
            }
        }

        status=cafeLatte.set (_handle, DBR_LONG, val);
        delete []val;

        //Reset to previous value
        if (nelemPrevious!=nelemThatWillBeRequested)
        {
            handleHelper.setNelem(_handle, nelemPrevious);
        }

        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};

//6
/**
 *  \brief   set vector of doubles
 *  \param   _handle input: handle
 *  \param   V input: vector of doubles
 *  \return  ICAFE_NORMAL or ECAFE_TIMEOUT
 */
int  CAFE::set(const unsigned int  _handle, std::vector<dbr_double_t>  V)
{

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        unsigned int  nelemThatWillBeRequested= V.size();
        unsigned int  nelemPrevious=nelemThatWillBeRequested;
        //Check the number of elements requested?
        //JC 25/6/15 Cython must also handle a list of 1!
        //if (nelemThatWillBeRequested>1) {
        //What did the client request? What is native type? Transfer minimum of this.
        if ( nelemThatWillBeRequested !=  (*it_handle).getChannelRequestMetaDataClient().getNelem())
        {
            nelemPrevious=(*it_handle).getChannelRequestMetaDataClient().getNelem();
            nelemThatWillBeRequested= handleHelper.setNelem(_handle, nelemThatWillBeRequested);
        }
        //}
        double * val = new double [nelemThatWillBeRequested];
        if (V.size()==nelemThatWillBeRequested)
        {
            std::copy(V.begin(), V.end(), val);
        }
        else
        {
            for (unsigned int  i=0; i<nelemThatWillBeRequested; ++i )
            {
                val[i]=V[i];
            }
        }
        status=cafeDoppio.set (_handle, DBR_DOUBLE, val);
        delete []val;


        //Reset to previous value
        if (nelemPrevious!=nelemThatWillBeRequested)
        {
            handleHelper.setNelem(_handle, nelemPrevious);
        }

        return status;
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
};

/**
 *  \brief   set values as given in pvd struct
 *  \param   handleV input: vector of handles
 *  \param   pvd input: PVDataHolder *
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int CAFE::setPVArray(std::vector<unsigned int> handleV, PVDataHolder * pvd)
{
    int  overallStatus=ICAFE_NORMAL;
    unsigned int    * h = new unsigned int   [handleV.size()];
    for (unsigned int  i=0; i<handleV.size(); ++i)
    {
        h[i] = handleV[i];
    }
    overallStatus = CAFE::set(h, (unsigned int) handleV.size(), pvd);
    delete []h;
    return overallStatus; // ICAFE_NORMAL else reports first failure
}


//0
/**
 *  \brief   set vector of string
 *  \param   handleV input: vector of handles
 *  \param   V input: vector of dbr_string_t (for WFs only first element is set)
 *  \param   statusV output: vector of longs
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int  CAFE::set(std::vector<unsigned int> handleV, std::vector<std::string>  V, std::vector<int> &statusV)
{
    int  overallStatus=ICAFE_NORMAL;
    statusV.clear();
    statusV.reserve(handleV.size());
    unsigned int    * h = new unsigned int   [handleV.size()];
    dbr_string_t * value = new dbr_string_t  [handleV.size()];
    int  * stats        = new int  [handleV.size()];
    for (size_t  i=0; i<handleV.size(); ++i)
    {
        h[i] = handleV[i];
        sprintf(value[i],"%s",V[i].c_str());
    }
    overallStatus = CAFE::set(h, (unsigned int) handleV.size(), value, stats);
    for (size_t   i=0; i<handleV.size(); ++i)
    {
        statusV.push_back(stats[i]);
    }
    delete []h;
    delete []value;
    delete []stats;
    return overallStatus; // ICAFE_NORMAL else reports first failure
}
//1
/**
 *  \brief   set vector of dbr_short_t
 *  \param   handleV input: vector of handles
 *  \param   V input: vector of shorts (for WFs only first element is set)
 *  \param   statusV output: vector of longs
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int  CAFE::set(std::vector<unsigned int> handleV, std::vector<dbr_short_t>  V, std::vector<int> &statusV)
{
    int  overallStatus=ICAFE_NORMAL;
    statusV.clear();
    statusV.reserve(handleV.size());
    unsigned int    * h = new unsigned int   [handleV.size()];
    dbr_short_t * value = new dbr_short_t    [handleV.size()];
    int  * stats        = new int  [handleV.size()];
    for (unsigned int  i=0; i<handleV.size(); ++i)
    {
        h[i]     = handleV[i];
        value[i] = V[i];
    }
    overallStatus = CAFE::set(h, (unsigned int) handleV.size(), value, stats);
    for (unsigned int   i=0; i<handleV.size(); ++i)
    {
        statusV.push_back(stats[i]);
    }
    delete []h;
    delete []value;
    delete []stats;
    return overallStatus; // ICAFE_NORMAL else reports first failure
}
//2
/**
 *  \brief   set vector of dbr_float_t
 *  \param   handleV input: vector of handles
 *  \param   V input: vector of floats (for WFs only first element is set)
 *  \param   statusV output: vector of longs
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int  CAFE::set(std::vector<unsigned int> handleV, std::vector<dbr_float_t> V, std::vector<int> &statusV)
{
    int  overallStatus=ICAFE_NORMAL;
    statusV.clear();
    statusV.reserve(handleV.size());
    unsigned int    * h = new unsigned int   [handleV.size()];
    dbr_float_t * value = new dbr_float_t    [handleV.size()];
    int  * stats        = new int  [handleV.size()];
    for (unsigned int  i=0; i<handleV.size(); ++i)
    {
        h[i] = handleV[i];
        value[i] = V[i];
    }
    overallStatus = CAFE::set(h, (unsigned int) handleV.size(), value, stats);
    for (unsigned int   i=0; i<handleV.size(); ++i)
    {
        statusV.push_back(stats[i]);
    }
    delete []h;
    delete []value;
    delete []stats;
    return overallStatus; // ICAFE_NORMAL else reports first failure
}
//3
/**
 *  \brief   set vector of dbr_enum_t (unsigned short)
 *  \param   handleV input: vector of handles
 *  \param   V input: vector of dbr_enum_t (for WFs only first element is set)
 *  \param   statusV output: vector of longs
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int  CAFE::set(std::vector<unsigned int> handleV, std::vector<dbr_enum_t> V, std::vector<int> &statusV)
{
    int  overallStatus=ICAFE_NORMAL;
    statusV.clear();
    statusV.reserve(handleV.size());
    unsigned int    * h = new unsigned int   [handleV.size()];
    dbr_enum_t * value  = new dbr_enum_t    [handleV.size()];
    int  * stats        = new int  [handleV.size()];
    for (unsigned int  i=0; i<handleV.size(); ++i)
    {
        h[i] = handleV[i];
        value[i] = V[i];
    }
    overallStatus = CAFE::set(h, (unsigned int) handleV.size(), value, stats);
    for (unsigned int   i=0; i<handleV.size(); ++i)
    {
        statusV.push_back(stats[i]);
    }
    delete []h;
    delete []value;
    delete []stats;
    return overallStatus; // ICAFE_NORMAL else reports first failure
}
//4
/**
 *  \brief   set vector of dbr_char_t (unsigned char)
 *  \param   handleV input: vector of handles
 *  \param   V input: vector of dbr_char_t (for WFs only first element is set)
 *  \param   statusV output: vector of longs
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int  CAFE::set(std::vector<unsigned int> handleV, std::vector<dbr_char_t> V, std::vector<int> &statusV)
{
    int  overallStatus=ICAFE_NORMAL;
    statusV.clear();
    statusV.reserve(handleV.size());
    unsigned int    * h = new unsigned int   [handleV.size()];
    dbr_char_t * value  = new dbr_char_t    [handleV.size()];
    int  * stats        = new int  [handleV.size()];
    for (unsigned int  i=0; i<handleV.size(); ++i)
    {
        h[i] = handleV[i];
        value[i] = V[i];
    }
    overallStatus = CAFE::set(h, (unsigned int) handleV.size(), value, stats);
    for (unsigned int   i=0; i<handleV.size(); ++i)
    {
        statusV.push_back(stats[i]);
    }
    delete []h;
    delete []value;
    delete []stats;
    return overallStatus; // ICAFE_NORMAL else reports first failure
}
//5
/**
 *  \brief   set vector of dbr_long_t
 *  \param   handleV input: vector of handles
 *  \param   V input: vector of dbr_long_t (for WFs only first element is set)
 *  \param   statusV output: vector of longs
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int  CAFE::set(std::vector<unsigned int> handleV, std::vector<dbr_long_t> V, std::vector<int> &statusV)
{
    int  overallStatus=ICAFE_NORMAL;
    statusV.clear();
    statusV.reserve(handleV.size());
    unsigned int    * h = new unsigned int   [handleV.size()];
    dbr_long_t * value  = new dbr_long_t    [handleV.size()];
    int  * stats        = new int  [handleV.size()];
    for (unsigned int  i=0; i<handleV.size(); ++i)
    {
        h[i] = handleV[i];
        value[i] = V[i];
    }
    overallStatus = CAFE::set(h, (unsigned int) handleV.size(), value, stats);
    for (unsigned int   i=0; i<handleV.size(); ++i)
    {
        statusV.push_back(stats[i]);
    }
    delete []h;
    delete []value;
    delete []stats;
    return overallStatus; // ICAFE_NORMAL else reports first failure
}
//5+
/**
 *  \brief   set vector of long
 *  \param   handleV input: vector of handles
 *  \param   V input: vector of int  (for WFs only first element is set)
 *  \param   statusV output: vector of longs
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int  CAFE::set(std::vector<unsigned int> handleV, std::vector<long long> V, std::vector<int> &statusV)
{
    int  overallStatus=ICAFE_NORMAL;
    statusV.clear();
    statusV.reserve(handleV.size());
    unsigned int    * h = new unsigned int   [handleV.size()];
    long long * value   = new long long [handleV.size()];
    int  * stats        = new int  [handleV.size()];
    for (unsigned int  i=0; i<handleV.size(); ++i)
    {
        h[i] = handleV[i];
        value[i] = (long long) V[i];
    }
    overallStatus = CAFE::set(h, (unsigned int) handleV.size(),value, stats);
    for (unsigned int   i=0; i<handleV.size(); ++i)
    {
        statusV.push_back(stats[i]);
    }
    delete []h;
    delete []value;
    delete []stats;
    return overallStatus; // ICAFE_NORMAL else reports first failure
}
//6
/**
 *  \brief   set vector of dbr_double_t
 *  \param   handleV input: vector of handles
 *  \param   V input: vector of dbr_double_t (for WFs only first element is set)
 *  \param   statusV output: vector of longs
 *  \return  overallStatus is ICAFE_NORMAL or report of first failure
 */
int  CAFE::set(std::vector<unsigned int> handleV, std::vector<dbr_double_t> V, std::vector<int> &statusV)
{
    int  overallStatus=ICAFE_NORMAL;
    statusV.clear();
    statusV.reserve(handleV.size());
    unsigned int    * h = new unsigned int   [handleV.size()];
    dbr_double_t * value  = new dbr_double_t    [handleV.size()];
    int  * stats        = new int  [handleV.size()];
    for (unsigned int  i=0; i<handleV.size(); ++i)
    {
        h[i] = handleV[i];
        value[i] = V[i];
    }
    overallStatus = CAFE::set(h, (unsigned int) handleV.size(), value, stats);
    for (unsigned int   i=0; i<handleV.size(); ++i)
    {
        statusV.push_back(stats[i]);
    }
    delete []h;
    delete []value;
    delete []stats;
    return overallStatus; // ICAFE_NORMAL else reports first failure
}
//#endif
