///
/// \file    zbsDataHolders.h
/// \author  Jan Chrin, PSI
/// \date    Release October 2017
/// \version CAFE 1.4.0
///

#ifndef ZBS_DATA_HOLDERS_H
#define ZBS_DATA_HOLDERS_H

#include <config.h>
#include <vector>
#include <PVDataHolder.h>
#include <statusCodes.h>
#include <handleHelper.h>
#include <global.h>


#if HAVE_ZEROMQ
#include <zmq.h>
#if HAVE_JSON
#include <json/json.h>
#endif
#if HAVE_CURL
#include <curl/curl.h>
#endif
#endif

//forward declaration required to establish friend
class CAFE;

namespace CAFEBS
{

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// BSChannel for configuration
// BSDataHolder to keep data
//
///////////////////////////////////////////////////////////////////////////////////////////////////

class BSChannel
{
    friend class BSDataHolder;
private:
    std::string name;
    std::string type;
    std::vector<int> shape;
    unsigned int nelem;
    std::string encoding;
    int offset;
    int modulo;
    std::string compression;
    bool BSEnabled;

    HandleHelper helper;

public:
    //All data is placed to the PVDataHolder object
    //PVDataHolder pvd;
    //void setName(std::string _name)  {    name   =_name;}

    void setNelem(unsigned int _nelem)
    {
        nelem=_nelem;
    }

    void setType(std::string _type)
    {
        type=_type;
    }

    void setOffset(int _offset)
    {
        offset =_offset;
    }
    void setModulo(int _modulo)
    {
        modulo =_modulo;
    }
    void setShape (std::vector<int> _shape)
    {
        nelem=1;
        shape.clear();
        shape.reserve(_shape.size());
        shape.assign(_shape.begin(),_shape.end());
        for (size_t i=0; i<shape.size(); ++i)
        {
            nelem=nelem * shape[i];
        }
    }

    void setEncoding(std::string _encoding)
    {
        encoding =_encoding;
    }
    void setCompression(std::string _compression)
    {
        compression = _compression;
    }
    void setBSEnabled(bool _bse)
    {
        BSEnabled=_bse;
    }

    std::string getName()
    {
        return name;
    }
    std::string getType()
    {
        return type;
    }
    int    getModulo()
    {
        return modulo;
    }
    int    getOffset()
    {
        return offset;
    }
    std::vector<int>  getShape()
    {
        return shape;
    }
    unsigned int getNelem()
    {
        return nelem;
    }
    std::string getEncoding()
    {
        return encoding;
    }
    std::string getCompression()
    {
        return compression;
    }
    bool   isBSEnabled()
    {
        return BSEnabled;
    }

    BSChannel(std::string _name):  offset(0), modulo(1)
    {
        char pv[PVNAME_SIZE];
        helper.removeLeadingAndTrailingSpaces(_name.c_str(), pv);
        name=(std::string) pv;
        shape.clear();
        shape.push_back(1);
        nelem=1;
        encoding=std::string("little");
        compression=std::string("none");
        type=std::string("float64");
        BSEnabled=true;
    };
    BSChannel(std::string _name, int _modulo): offset(0)
    {
        char pv[PVNAME_SIZE];
        helper.removeLeadingAndTrailingSpaces(_name.c_str(), pv);
        name=(std::string) pv;
        modulo=_modulo;
        shape.clear();
        shape.push_back(1);
        nelem=1;
        encoding=std::string("little");
        compression=std::string("none");
        type=std::string("float64");
        BSEnabled=true;
    };
    BSChannel(std::string _name, int _modulo, int _offset)
    {
        char pv[PVNAME_SIZE];
        helper.removeLeadingAndTrailingSpaces(_name.c_str(), pv);
        name=(std::string) pv;
        modulo=_modulo;
        offset=_offset;
        shape.clear();
        shape.push_back(1);
        nelem=1;
        encoding=std::string("little");
        compression=std::string("none");
        type=std::string("float64");
        BSEnabled=true;
    };
};



class BSDataHolder
{
    friend class ::CAFE; //Says CAFE is a member og the global namespace
private:
    std::string htype;
    unsigned long long pulse_id;
    etsNorm global_timestamp;
    std::string hash;
    std::string dh_compression;

    std::vector<BSChannel> bsChannel;
    HandleHelper helper;

    std::string grpBS2CA;
    std::vector<std::string>   pv;
    std::vector<unsigned int>  handle;

    bool isBS;
    bool BSInitialized;
    bool resourceConnected; // Used to confirm initial connection;

    unsigned int nPV;
    unsigned int nBSEnabled;

    unsigned int nChannels;
    unsigned int nNullData;
    float pGoodData; //nChannels-nNulldata/nChannels

    void *context;
    void *receiver;
    int rc;
    unsigned short nhwm;  // high-water mark
    int  timeoutMS;      // timeout in ms; -1 is wait for ever

#if HAVE_JSON
    Json::Value parsedFromString;
    Json::Reader reader;
#endif

public:
    ~BSDataHolder()
    {
        //std::cout << "BSDATAHOLDER DECONSTRUCTOR CALLED " << std::endl;

    };

    //delete [] pvd; resourceConnected=false; nBSEnabled=0;};

    BSDataHolder():isBS(true),BSInitialized(false),nBSEnabled(0)
    {
     
        overallStatus=ICAFE_NORMAL;
        resourceConnected=false;
        nhwm= BSREAD_ZEROMQ_HIGH_WATER_MARK;      // high-water mark
        timeoutMS= BSREAD_ZEROMQ_TIMEOUT_MS;      // timeout in ms; -1 is wait for ever
        hash="undefined";
        //htype="";
        dh_compression="none";
        global_timestamp.secPastEpoch=0;
        global_timestamp.nsec=0;
        pulse_id=0;
        initCalled=false;
        nPV=0;
        nChannels=0;
        nNullData=0;
        pGoodData=0.0;
        grpBS2CA=SYNCHRONOUS_GROUP_NAME_IN_BSDATAHOLDER;
    }

    BSDataHolder(std::vector<std::string> _pv, std::vector<unsigned int> _handle):isBS(false),BSInitialized(false),nBSEnabled(0)
    {
        pv.clear();
        handle.clear();
        pv.assign    (_pv.begin(),    _pv.end());

        bsChannel.clear();
        for (size_t i=0; i< _pv.size(); ++i)
        {
            bsChannel.push_back(BSChannel(_pv[i]));
        }

        handle.assign(_handle.begin(),_handle.end());
        pvd = new PVDataHolder[handle.size()];

        //Set nelem to native
        for (unsigned int i=0; i< handle.size(); ++i)
        {
            pvd[i].setNelem(helper.getNelemNative(handle[i]));
        }

        nPV=_pv.size();
        overallStatus=ICAFE_NORMAL;
        resourceConnected=false;
        nhwm= BSREAD_ZEROMQ_HIGH_WATER_MARK;      // high-water mark
        timeoutMS= BSREAD_ZEROMQ_TIMEOUT_MS;      // timeout in ms; -1 is wait for ever
        hash="undefined";
        dh_compression="none";
        global_timestamp.secPastEpoch=0;
        global_timestamp.nsec=0;
        pulse_id=0;
        initCalled=false;
        nChannels=0;
        nNullData=0;
        pGoodData=0.0;
        grpBS2CA=SYNCHRONOUS_GROUP_NAME_IN_BSDATAHOLDER;
    }

    BSDataHolder(std::vector<std::string> _pv):isBS(true),BSInitialized(false),nBSEnabled(0)
    {

        pv.clear();
        pv.assign(_pv.begin(),_pv.end());

        handle.clear();
        handle.reserve(_pv.size());

        bsChannel.clear();
        for (size_t i=0; i< _pv.size(); ++i)
        {
            bsChannel.push_back(BSChannel(_pv[i]));
            handle.push_back(0); // initialize handle to zero
        }

        pvd = new PVDataHolder[_pv.size()];

        //for (int i=0; i< handle.size(); ++i) {
        //	pvd[i].setNelem(1);
        //}

        nPV=_pv.size();
        overallStatus=ICAFE_NORMAL;
        resourceConnected=false;
        nhwm= BSREAD_ZEROMQ_HIGH_WATER_MARK;      // high-water mark
        timeoutMS= BSREAD_ZEROMQ_TIMEOUT_MS;      // timeout in ms; -1 is wait for ever
        hash="undefined";
        dh_compression="none";
        global_timestamp.secPastEpoch=0;
        global_timestamp.nsec=0;
        pulse_id=0;
        initCalled=false;
        nChannels=0;
        nNullData=0;
        pGoodData=0.0;
        grpBS2CA=SYNCHRONOUS_GROUP_NAME_IN_BSDATAHOLDER;
    }

    void init(std::vector<std::string> _pv)
    {

        if (!initCalled)
        {
            pv.clear();
            pv.assign(_pv.begin(),_pv.end());
            handle.clear();
            handle.reserve(_pv.size());

            bsChannel.clear();
            for (size_t i=0; i< _pv.size(); ++i)
            {
                bsChannel.push_back(BSChannel(_pv[i]));
                handle.push_back(0); // initialize handle to zero
            }
            pvd = new PVDataHolder[_pv.size()];
            nPV=_pv.size();
            initCalled=true;
            nBSEnabled=0;
            BSInitialized=false;
            isBS=false;

        }

    }


    bool initCalled;
    PVDataHolder * pvd;

    std::string globalBSZmqStream;

    unsigned int getHWM()
    {
        return nhwm;
    }
    int  getTimeout()
    {
        return timeoutMS;
    }
    void setHWM(unsigned short _nhwm)
    {
        nhwm=_nhwm;   //Will only be set into action at next (re-)connect or before BSInitialized
    }
    void setTimeout(int _timeoutMS)
    {
        timeoutMS=_timeoutMS;   //Will only be set into action at next (re-)connect or before BSInitialized
    }

    unsigned int getNPV()
    {
        return nPV;
    };
    unsigned int getNBSEnabled()
    {
        return nBSEnabled;
    };

    BSChannel getBSChannel(unsigned int idx);
    BSChannel getBSChannel(std::string _name);

    void setBSChannel(unsigned int idx, BSChannel bsc);
    void setBSChannel(BSChannel bsc); //Name defined so will find correct index

    void setBSModulo(std::string pv, int modulo)
    {
        BSChannel bsc=getBSChannel(pv);
        bsc.setModulo(modulo);
        setBSChannel(bsc);
    }
    void setBSOffset(std::string pv, int offset)
    {
        BSChannel bsc=getBSChannel(pv);
        bsc.setOffset(offset);
        setBSChannel(bsc);
    }
    void setBSModuloOffset(std::string pv, int modulo, int offset)
    {
        BSChannel bsc=getBSChannel(pv);
        bsc.setModulo(modulo);
        bsc.setOffset(offset);
        setBSChannel(bsc);
    }


    int getIdxFromName(std::string _name);

    void verifyIndex(unsigned int  idx);

    bool isIndexOutOfRange (unsigned int  idx)
    {
        return (idx >= nPV) ? true:false;
    };
    void printHeader();
    int overallStatus;
    void *subscriber;
    int timeout;

    bool isResourceConnected()
    {
        return resourceConnected;
    };
    void setResourceConnected(bool _rc)
    {
        resourceConnected=_rc;
        return;
    };

    int getPVIdx(std::string _pv)
    {
#define __METHOD__  "getPVIdx(std::string _pv)"
        std::vector<std::string>::iterator it;
        it = find (pv.begin(), pv.end(), _pv);

        if (it != pv.end())
        {
            //std::cout << "Element found in pv vector: " << *it << '\n' << std::endl;
            //std::cout << "Distance is: " << std::distance(pv.begin(),it) << '\n' << std::endl;
            return std::distance(pv.begin(),it);
        }
        else
        {
            std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
            std::cout << _pv << " element not found in pv vector\n" << std::endl;
            return -1;
        }

        /*
        for (size_t i=0;  i< pv.size(); ++i) {
        	if ( pv[i].compare(_pv) == 0) {
        		return i;
        	}
        }
        */
#undef __METHOD__
    }


    std::vector<std::string>   getPV()
    {
        return pv;
    }
    std::string getPV(unsigned int idx) noexcept(false) //throw(std::out_of_range)
    {
        if(isIndexOutOfRange(idx))
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to BSDataHolder method is out of range. Valid range is from 0 to " << nPV-1;
            throw std::out_of_range(oss.str());
        }
        return pv[idx];
    }

    std::vector<unsigned int> getHandles()
    {
        return handle;
    }
    unsigned int getHandle(unsigned int idx) noexcept(false) // throw(std::out_of_range)
    {
        if(isIndexOutOfRange(idx))
        {
            std::ostringstream oss;
            oss << "Exception! Index " << idx
                << " to BSDataHolder method is out of range. Valid range is from 0 to " << nPV-1;
            throw std::out_of_range(oss.str());
        }
        return handle[idx];
    }

    std::string getHtype()
    {
        return htype;
    }
    void setHtype(std::string _htype)
    {
        htype=_htype;
    }

    unsigned long long getPulse_id()
    {
        return pulse_id;
    }
    void setPulse_id(unsigned long long _pulse_id)
    {
        pulse_id=_pulse_id;
        return;
    }

    etsNorm getGlobal_timestamp()
    {
        return global_timestamp;
    }

    void setGlobal_timestamp(unsigned int _sec, unsigned int _nsec)
    {
        global_timestamp.secPastEpoch=_sec;
        global_timestamp.nsec        =_nsec;
        return;
    }

    std::string getHash()
    {
        return hash;
    }
    void setHash(std::string _hash)
    {
        hash=_hash;
    }

    std::string getDH_compression()
    {
        return  dh_compression;
    }
    void setDH_compression(std::string dhc)
    {
        dh_compression= dhc;
    }

    //Diagnostics
    void setNChannels(unsigned int _nc)
    {
        nChannels=_nc;
    }
    void setNNullData(unsigned int _nn)
    {
        nNullData=_nn;
    }
    void setPGoodData(float _pg)
    {
        pGoodData=_pg;
    }
    unsigned int getNChannels()
    {
        return nChannels;
    }
    unsigned int getNNullData()
    {
        return nNullData;
    }
    float getPGoodData()
    {
        return pGoodData;
    }


    std::vector<int> getStatusV()
    {
        std::vector<int> V;
        V.reserve(nPV);
        for (size_t i=0; i<nPV; ++i)
        {
            V.push_back(pvd[i].getStatus());
        }
        return V;
    }



    std::vector<float> getAsFloatV()
    {
        std::vector<float> V;
        V.reserve(nPV);

        for (size_t i=0; i<nPV; ++i)
        {
            V.push_back(pvd[i].getAsFloat());
        }
        return V;
    }


    int getAsFloatV(std::vector<float> &valueV, std::vector<int> &statusV)
    {

        valueV.clear();
        statusV.clear();
        valueV.reserve(nPV);
        statusV.reserve(nPV);

        for (size_t i=0; i<nPV; ++i)
        {
            valueV.push_back(pvd[i].getAsFloat());
            statusV.push_back(pvd[i].getStatus());
        }
        return overallStatus;
    }


    std::vector<double> getAsDoubleV()
    {
        std::vector<double> V;
        V.reserve(nPV);

        //V.reserve(bsChannel.size());
        //for (size_t i=0; i<bsChannel.size(); ++i){
        for (size_t i=0; i<nPV; ++i)
        {
            //V.push_back(bsChannel[i].pvd.getAsDouble());
            V.push_back(pvd[i].getAsDouble());
        }
        return V;
    }


    int getAsDoubleV(std::vector<double> &valueV, std::vector<int> &statusV)
    {

        valueV.clear();
        statusV.clear();
        valueV.reserve(nPV);
        statusV.reserve(nPV);

        for (size_t i=0; i<nPV; ++i)
        {
            valueV.push_back(pvd[i].getAsDouble());
            statusV.push_back(pvd[i].getStatus());
        }
        return overallStatus;
    }


    std::vector<std::string> getAsStringV()
    {
        std::vector<std::string> V;
        V.reserve(nPV);
        for (size_t i=0; i<nPV; ++i)
        {
            V.push_back(pvd[i].getAsString());
        }
        return V;
    }

    int getAsStringV(std::vector<std::string> &valueV, std::vector<int> &statusV)
    {

        valueV.clear();
        statusV.clear();
        valueV.reserve(nPV);
        statusV.reserve(nPV);

        for (size_t i=0; i<nPV; ++i)
        {
            valueV.push_back(pvd[i].getAsString());
            statusV.push_back(pvd[i].getStatus());
        }
        return overallStatus;
    }



    std::vector<int> getAsIntV()
    {
        std::vector<int> V;
        V.reserve(nPV);
        for (size_t i=0; i<nPV; ++i)
        {
            V.push_back(pvd[i].getAsInt());
        }
        return V;
    }


    int getAsIntV(std::vector<int> &valueV, std::vector<int> &statusV)
    {

        valueV.clear();
        statusV.clear();
        valueV.reserve(nPV);
        statusV.reserve(nPV);

        for (size_t i=0; i<nPV; ++i)
        {
            valueV.push_back(pvd[i].getAsInt());
            statusV.push_back(pvd[i].getStatus());
        }
        return overallStatus;
    }



    std::vector<double> getAttributeAsDoubleV(std::string attribute)
    {
        std::vector<double> V;
        V.reserve(nPV);
        char pvAtt[PVNAME_SIZE];
        helper.removeLeadingAndTrailingSpaces(attribute.c_str(), pvAtt);
        for (size_t i=0; i<nPV; ++i)
        {
            if ( ((std::string)pvAtt).compare((std::string) pvd[i].getAttribute()) ==0)
            {
                V.push_back(pvd[i].getAsDouble());
            }
        }
        return V;
    }

    std::vector<PVDataHolder> getPVDataV()
    {
        std::vector<PVDataHolder> V;
        V.reserve(nPV);
        for (size_t i=0; i<nPV; ++i)
        {
            V.push_back(pvd[i]);
        }
        return V;
    }

    PVDataHolder   getPVData(unsigned int idx)
    {
        if (idx > (nPV-1))
        {
            idx=nPV-1;
        }
        return pvd[idx];
    }


    PVDataHolder   getPVData(std::string name)
    {
        for (size_t i=0; i< nPV; ++i)
        {
            if (bsChannel[i].getName().compare(name) ==0 )
            {
                return pvd[i];
            }
        }
        //Throw exception!!!!
	std::cout << "ERROR IN BSDataHolder.h " << std::endl;
	std::cout << name << " not found " << std::endl;
	std::cout << "Returning empty PVDataHolder!!!!!" << std::endl;
	PVDataHolder _empty;
	return _empty;
    }


    int getStatus()
    {
        return overallStatus;
    }


//cannot declare member function to have static linkage; therefore this must remain in header file
    static size_t RecvResponseCallback(char * contents, size_t size, size_t nmemb, void  * up)
    {

        ++nCBs;
        std::cout << "Callback called: " << nCBs << std::endl;
        std::cout << "SIZE No. of Bytes " << size*nmemb << std::endl;

        std::string sLocal=contents;

        //remove \n for newline
        std::size_t found = sLocal.find('\n');

        if (found != std::string::npos)
        {
            sLocal=sLocal.substr(0, found);
        }

        contentsBS=contentsBS+sLocal;

        //std::cout << contentsBS << std::endl;

        return (size_t) size * nmemb;
    }




//cannot declare member function to have static linkage; therefore this must remain in header file
    static size_t RecvResponseCallbackLive(char * contents, size_t size, size_t nmemb, void  * up)
    {

        callbackLiveFlag=true;

        //std::cout << "SIZE No. of Bytes = " << size*nmemb << std::endl;
        std::string sLocal=contents;

        //remove \n for newline
        std::size_t found = sLocal.find('\n');

        if (found != std::string::npos)
        {
            sLocal=sLocal.substr(0, found);
        }

        //std::cout << sLocal << std::endl;
        //std::cout << " Value of callback Live from callback fn is " << callbackLiveFlag << std::endl;

        return (size_t) size * nmemb;
    }

    int reconnect();

    bool setBS(bool BSFlag);

    bool resetBS()
    {
        closeBS();
        return setBS(true);
    }

    bool setCA(bool CAFlag)
    {
        return CAFlag;
    }

    void closeBS()
    {
        if (BSInitialized && isBS)
        {
#if HAVE_ZEROMQ
            zmq_close (subscriber);           
            zmq_ctx_destroy (context);            
#endif
            delete [] pvd;          
        }

        BSInitialized=false;
        isBS=false;
        initCalled=false;
        return;
    }

    bool getIsBS()
    {
        return isBS;
    }


};



///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Digital BPM Holder
//
///////////////////////////////////////////////////////////////////////////////////////////////////

class DBPMData
{
    friend class ::CAFE;
private:
    double val;
    epicsTimeStamp ets;
    int status;
public:
    double getValue()
    {
        return val;
    }
    epicsTimeStamp getEpicsTimeStamp()
    {
        return ets;
    }
    int getStatus()
    {
        return status;
    }

    DBPMData()
    {
        //status=ECAFE_BPM_DATA_IS_INVALID;
    };
};


class DBPMKeeper
{
    friend class ::CAFE;
private:
    std::vector<DBPMData> x;
    std::vector<DBPMData> y;
    std::vector<DBPMData> q;
    std::vector<DBPMData> energy;

    std::vector<double> offs_x;
    std::vector<double> offs_y;

    unsigned long long pulse_id;

    bool isAllXOK;
    bool isAllYOK;
    bool isAllQOK;
    bool isAllEOK;
    bool isAllOK;

    std::vector<std::string>  pv;
    std::vector<unsigned int> handle;
    std::vector<std::string>  device;
    std::vector<float>    s;

    size_t nDBPM;
    size_t nPV;

    bool isBS;
    bool BSInitialized;
    void *context;

    void *receiver;
    int rc;

#if HAVE_JSON
    Json::Value parsedFromString;
    Json::Reader reader;
#endif

public:

    std::vector<DBPMData> getX()
    {
        return x;
    }
    std::vector<DBPMData> getY()
    {
        return y;
    }
    std::vector<DBPMData> getQ()
    {
        return q;
    }
    std::vector<DBPMData> getEnergy()
    {
        return energy;
    }


    std::vector<double> getOffsetX()
    {
        return offs_x;
    }
    std::vector<double> getOffsetY()
    {
        return offs_y;
    }

    bool getIsAllXOK()
    {
        return isAllXOK;
    }
    bool getIsAllYOK()
    {
        return isAllYOK;
    }
    bool getIsAllQOK()
    {
        return isAllQOK;
    }
    bool getIsAllEOK()
    {
        return isAllEOK;
    }
    bool getIsAllOK()
    {
        return isAllOK;
    }

    std::vector<std::string>  getPV()
    {
        return pv;
    }
    std::vector<unsigned int> getHandle()
    {
        return handle;
    }
    std::vector<std::string>  getDevice()
    {
        return device;
    }
    std::vector<float>        getS()
    {
        return s;
    }
    size_t getNDBPM()
    {
        return nDBPM;
    }
    size_t getNPV()
    {
        return nPV;
    }
    int getStatus()
    {
        return status;
    }


    int getPVIdx(std::string _pv)
    {
        for (size_t i=0;  i< pv.size(); ++i)
        {
            if ( pv[i].compare(_pv) == 0)
            {
                return i;
            }
        }
        return -1;
    }


    unsigned long long getPulse_id()
    {
        return pulse_id;
    }
    void setPulse_id(unsigned long long _pulse_id)
    {
        pulse_id=_pulse_id;
    }

    PVDataHolder * pvd;
    int status;

    size_t xIdx;
    size_t yIdx;
    size_t qIdx;
    size_t xValidIdx;
    size_t yValidIdx;
    size_t qValidIdx;
    size_t energyIdx;
    size_t endIdx;

    void *subscriber;

    static size_t RecvResponseCallback(char * contents, size_t size, size_t nmemb, void  * up)
    {

        ++nCBs;
        //std::cout << "Callback called: " << nCBs << std::endl;
        //std::cout << "SIZE No. of Bytes " << size*nmemb << std::endl;

        std::string sLocal=contents;

        //remove \n for newline
        std::size_t found = sLocal.find('\n');

        if (found != std::string::npos)
        {

            sLocal=sLocal.substr(0, found);
        }

        contentsS=contentsS+sLocal;

        return (size_t) size * nmemb;
    }


    bool setBS(bool BSFlag)
    {

        if(MUTEX)
        {
            cafeMutex.lock();
        }

        if (BSFlag)
        {
#if HAVE_CURL

            std::string dataChannels=std::string("{\"channels\":[");
            std::vector<std::string> pvNew=pv;

#if HAVE_ZEROMQ

            if (!BSInitialized)
            {

                size_t found;
                dataChannels= dataChannels + std::string("{\"name\":\"");
                dataChannels= dataChannels + pvNew[0];
                //dataChannels= dataChannels + std::string("\",\"backend\":\"sf-databuffer\"}"		);
                dataChannels= dataChannels + std::string("\",\"backend\":\"sf-databuffer\",\"modulo\":1,\"offset\":0}" );


                for (size_t i=1; i < pvNew.size(); ++i)
                {

                    found = pvNew[i].find("SARUN08-DBPM210");
                    if (found != std::string::npos) continue;
                    found = pvNew[i].find("SARUN08-DBPM410");
                    if (found != std::string::npos) continue;

                    found = pvNew[i].find("ENERGY");
                    if (found != std::string::npos) continue;


                    dataChannels= dataChannels + std::string(",{\"name\":\"");
                    dataChannels= dataChannels + pvNew[i];

                    dataChannels= dataChannels + std::string("\",\"backend\":\"sf-databuffer\",\"modulo\":1,\"offset\":0}");

                }

                dataChannels= dataChannels + std::string("],");
                dataChannels= dataChannels + "\"mapping\":{\"incomplete\":\"fill-null\"},\"channelValidation\":{\"inconsistency\":\"keep-as-is\"},\"sendBehaviour\":{\"strategy\":\"complete-all\"}}";

                //std::cout <<  dataChannels << std::endl;

                const char * data = dataChannels.c_str();

                //std::cout << "SIZE OF DATA --------------->"  << sizeof(data) << std::endl;

                CURL *curl;
                CURLcode res;
                struct curl_slist * slist;
                slist = NULL;

                slist = curl_slist_append(slist, "Content-Type: application/json");

                curl_global_init(CURL_GLOBAL_ALL);

                curl = curl_easy_init();

                if (curl)
                {

                    curl_easy_setopt(curl, CURLOPT_URL, "https://dispatcher-api.psi.ch/sf/stream");

                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data); //"-F file=@./dbpm.json"); //data); //

                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
                    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

                    //std::cout << "WAITING FOR CALLBACK... " << std::endl;

                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RecvResponseCallback);

                    res = curl_easy_perform(curl);

                    if (res != CURLE_OK)
                    {
                        std::cout << "curl_easy_perform failed " << curl_easy_strerror(res) << std::endl;
                    }
                    else
                    {
                        std::cout << " CALLBACK DONE" << std::endl;

                        curl_easy_cleanup(curl);

                        curl_slist_free_all(slist);

                        slist=NULL;
                    }
                }//if curl

                std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
                curl_global_cleanup();


                //std::cout << " //1// SHOW contentS " << std::endl;
                //std::cout << contentsS.c_str() << std::endl;

                Json::Value parsedFromString;
                Json::Reader reader;

                Json::FastWriter fastWriter;
                std::string globalZmqStream;


                //printf("value= %s\n", contentsS.c_str());

                if (contentsS.size() > 2)
                {
                    bool parsingSuccessful=reader.parse(contentsS.c_str(), parsedFromString);
                    if (parsingSuccessful)
                    {
                        //Json::StyledWriter styledWriter;
                        std::cout << "STYLED: --------------------------------" << std::endl;
                        //std::cout << styledWriter.write(parsedFromString) << std::endl;
                        //std::cout << "----------------------------------" << std::endl;
                        std::cout << parsedFromString["stream"] << std::endl;

                        std::cout << "----------------------------------" << std::endl;
                        globalZmqStream = fastWriter.write(parsedFromString["stream"]).c_str();
                        std::cout << globalZmqStream << std::endl;

                        if ( parsedFromString["stream"].isNull() )
                        {
                            globalZmqStream.clear();
                        }
                    }
                    else
                    {
                        std::cout << "PARSING IN CURL CALLBACK FUNCTION WAS UNSUCCESSFUL !!!" << std::endl;
                        std::cout << contentsS.c_str() << std::endl;
                        std::cout << reader.getFormattedErrorMessages() << std::endl;

                    }
                }

                if (globalZmqStream.empty())
                {
                    std::cout << "BS Data is not available " << std::endl;
                    if(MUTEX)
                    {
                        cafeMutex.unlock();
                    }
                    return isBS=false;
                }

                context = zmq_ctx_new ();

                ////  receiver = zmq_socket (context, ZMQ_PULL);
                //HWM has no effect for PULL
                //See documentation on zmq-socket
                //WHEN PUSH Sender reachers HWM, then it blocks
                ////		int nhwm=10;
                ////		zmq_setsockopt (receiver,ZMQ_RCVHWM ,&nhwm, sizeof(int));
                //			rc = zmq_bind (receiver, "tcp://129.129.145.206:5558"); //ZMQ_PULL
                ////		assert (rc == 0);

                subscriber = zmq_socket (context, ZMQ_SUB);
                //rc = zmq_connect (subscriber, "tcp://129.129.145.206:5556");
                //rc = zmq_connect (subscriber, "tcp://SIN-CVME-DBPM0421:9000");

                globalZmqStream=globalZmqStream.substr(1,globalZmqStream.size()-3);
                //std::cout << " globalZmqStream.c_str() " << globalZmqStream.c_str() << std::endl;

                rc = zmq_connect (subscriber, (const char *) globalZmqStream.c_str());   //"tcp://sf-daqbuf-30.psi.ch:39927");

                if (rc != 0 )
                {
                    std::cout << " Error is " << zmq_errno() << " " << zmq_strerror(zmq_errno()) << std::endl;
                }

                //rc = zmq_connect (subscriber, "tcp://*:9999");
                //assert (rc == 0);

                int nhwm=1;
                int timeoutMS=200; //10; //-1 Wait for Ever



                rc=zmq_setsockopt (subscriber,ZMQ_RCVHWM, &nhwm, sizeof(int));
                rc=zmq_setsockopt (subscriber,ZMQ_SNDHWM, &nhwm, sizeof(int));
                //assert (rc == 0);

                rc=zmq_setsockopt (subscriber,ZMQ_RCVTIMEO, &timeoutMS, sizeof(int));
                //assert (rc == 0);

                rc=zmq_setsockopt (subscriber,ZMQ_SUBSCRIBE,"",0);
                //assert (rc == 0);

                BSInitialized=true;

            }//is BS initialized

#endif //have zeromq

            if(MUTEX)
            {
                cafeMutex.unlock();
            }
            return isBS=BSFlag;
#else //have curl

            if(MUTEX)
            {
                cafeMutex.unlock();
            }
            return isBS=false;
#endif //have curl
        }//isBSFlag

        if(MUTEX)
        {
            cafeMutex.unlock();
        }
        return isBS=BSFlag;
    } // setBS




    bool resetBS()
    {
        closeBS();
        return setBS(true);
    }


    bool setCA(bool CAFlag)
    {
        return CAFlag;
    }

    void closeBS()
    {
        if (BSInitialized && isBS)
        {
#if HAVE_ZEROMQ
            zmq_close (subscriber);
            zmq_ctx_destroy (context);
#endif
        }
        BSInitialized=false;
        isBS=false;
    }

    bool getIsBS()
    {
        return isBS;
    }


    DBPMKeeper() {};

    DBPMKeeper(std::vector<std::string> _pv, std::vector<unsigned int> _handle, std::map<float, std::string> posDev):isBS(false),BSInitialized(false)
    {

        pv.assign    (_pv.begin(),    _pv.end());
        handle.assign(_handle.begin(),_handle.end());


        //fMap posDev;
        std::map<float, std::string>::iterator pos;
        for (pos =posDev.begin(); pos != posDev.end(); ++pos)
        {
            s.push_back(pos->first);
            device.push_back(pos->second);
        }

        pvd = new PVDataHolder[handle.size()];

        //for (int i=0; i< handle.size(); ++i) {
        //	pvd[i].setNelem(1);
        //}

        nDBPM=device.size();
        nPV=_pv.size();
        status=ICAFE_NORMAL;

        xIdx     =  0;
        yIdx     =  nDBPM;
        qIdx     =2*nDBPM;
        xValidIdx=3*nDBPM;
        yValidIdx=4*nDBPM;
        qValidIdx=5*nDBPM;
        energyIdx=6*nDBPM;
        endIdx   =7*nDBPM;
    }


    DBPMKeeper(std::vector<std::string> _pv, std::vector<unsigned int> _handle, std::vector<std::string> _dev,  std::vector<float> _pos):isBS(false),BSInitialized(false)
    {

        pv.assign    (_pv.begin(),    _pv.end());
        handle.assign(_handle.begin(),_handle.end());

        device.assign(_dev.begin(), _dev.end());
        s.assign(_pos.begin(), _pos.end());

        pvd = new PVDataHolder[handle.size()];

        //for (int i=0; i< handle.size(); ++i) {
        //	pvd[i].setNelem(1);
        //}

        nDBPM=device.size();
        nPV=_pv.size();
        status=ICAFE_NORMAL;

        xIdx     =  0;
        yIdx     =  nDBPM;
        qIdx     =2*nDBPM;
        xValidIdx=3*nDBPM;
        yValidIdx=4*nDBPM;
        qValidIdx=5*nDBPM;
        energyIdx=6*nDBPM;
        endIdx   =7*nDBPM;
    }

};


}; //namespace


#endif //ZBS_DATA_HOLDERS_H
