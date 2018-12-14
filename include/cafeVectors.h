///
/// \file    cafeVectors.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef CAFEVECTORS_H
#define CAFEVECTORS_H

#include <string>
#include <vector>

int get(const unsigned int  _handle, std::vector<std::string> & V);         //0
int get(const unsigned int  _handle, std::vector<short> & V);          //1
int get(const unsigned int  _handle, std::vector<float> & V);          //2
int get(const unsigned int  _handle, std::vector<unsigned short> & V); //3
int get(const unsigned int  _handle, std::vector<unsigned char>  & V); //4
int get(const unsigned int  _handle, std::vector<dbr_long_t> & V);     //5
int get(const unsigned int  _handle, std::vector<long long> & V);      //5
int get(const unsigned int  _handle, std::vector<double> & V);         //6
int get(const unsigned int  _handle, std::vector<std::string> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//0
int get(const unsigned int  _handle, std::vector<short> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//1
int get(const unsigned int  _handle, std::vector<float> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//2
int get(const unsigned int  _handle, std::vector<unsigned short> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//3
int get(const unsigned int  _handle, std::vector<unsigned char> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//4
int get(const unsigned int  _handle, std::vector<dbr_long_t> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//5
int get(const unsigned int  _handle, std::vector<long long> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//5
int get(const unsigned int  _handle, std::vector<double> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//6
int get(const unsigned int  _handle, std::vector<std::string> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//0
int get(const unsigned int  _handle, std::vector<short> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//1
int get(const unsigned int  _handle, std::vector<float> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//2
int get(const unsigned int  _handle, std::vector<unsigned short> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//3
int get(const unsigned int  _handle, std::vector<unsigned char> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//4
int get(const unsigned int  _handle, std::vector<dbr_long_t> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//5
int get(const unsigned int  _handle, std::vector<long long> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//5
int get(const unsigned int  _handle, std::vector<double> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//

int getCache(const unsigned int  _handle, std::vector<std::string> & V);        //0
int getCache(const unsigned int  _handle, std::vector<short> & V);         //1
int getCache(const unsigned int  _handle, std::vector<float> & V);         //2
int getCache(const unsigned int  _handle, std::vector<unsigned short> & V);//3
int getCache(const unsigned int  _handle, std::vector<unsigned char>  & V);//4
int getCache(const unsigned int  _handle, std::vector<dbr_long_t> & V);    //5
int getCache(const unsigned int  _handle, std::vector<long long> & V);          //5
int getCache(const unsigned int  _handle, std::vector<double> & V);        //6
int getCache(const unsigned int  _handle, std::vector<std::string> & V,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//0
int getCache(const unsigned int  _handle, std::vector<short> & V,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//1
int getCache(const unsigned int  _handle, std::vector<float> & V,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//2
int getCache(const unsigned int  _handle, std::vector<unsigned short> & V,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//3
int getCache(const unsigned int  _handle, std::vector<unsigned char> & V,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//4
int getCache(const unsigned int  _handle, std::vector<dbr_long_t> & V,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//5
int getCache(const unsigned int  _handle, std::vector<long long> & V,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//5
int getCache(const unsigned int  _handle, std::vector<double> & V,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//6


int getCache(const unsigned int  _handle, std::vector<std::string> & V,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//0
int getCache(const unsigned int  _handle, std::vector<short> & V,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//1
int getCache(const unsigned int  _handle, std::vector<float> & V,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//2
int getCache(const unsigned int  _handle, std::vector<unsigned short> & V,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//3
int getCache(const unsigned int  _handle, std::vector<unsigned char> & V,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//4
int getCache(const unsigned int  _handle, std::vector<dbr_long_t> & V,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//5
int getCache(const unsigned int  _handle, std::vector<long long> & V,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//5
int getCache(const unsigned int  _handle, std::vector<double> & V,
             dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//6

//NON_BLOCKING
int get(std::vector<unsigned int> handleV, std::vector<int> &statusV);

int getV(std::vector<unsigned int> handleV, std::vector<int> &statusV)
{
    status=get(handleV, statusV);
    ca_flush_io(); //Yes let's flush here!
    return status;
}

int get(std::vector<std::string> pvV, std::vector<int> &statusV)
{
    std::vector<unsigned int> hV;
    hV.reserve(pvV.size());
    hV=handleHelper.getHandlesFromPVs(pvV);
    return get(hV, statusV);

}


int getScalars(std::vector<unsigned int> handleV, std::vector<std::string> & V, std::vector<int> &statusV)  //0
{
    CAFE::get(handleV, statusV);
    CAFE::waitForBundledEvents(handleV, statusV);
    return CAFE::getCache(handleV, V, statusV);
}
int getScalars(std::vector<unsigned int> handleV, std::vector<dbr_short_t> & V, std::vector<int> &statusV)  //1
{
    CAFE::get(handleV, statusV);
    CAFE::waitForBundledEvents(handleV, statusV);
    return CAFE::getCache(handleV, V, statusV);
}
int getScalars(std::vector<unsigned int> handleV, std::vector<dbr_float_t> & V, std::vector<int> &statusV)  //2
{
    CAFE::get(handleV, statusV);
    CAFE::waitForBundledEvents(handleV, statusV);
    return CAFE::getCache(handleV, V, statusV);
}
int getScalars(std::vector<unsigned int> handleV, std::vector<dbr_enum_t>  & V, std::vector<int> &statusV)  //3
{
    CAFE::get(handleV, statusV);
    CAFE::waitForBundledEvents(handleV, statusV);
    return CAFE::getCache(handleV, V, statusV);
}
int getScalars(std::vector<unsigned int> handleV, std::vector<dbr_char_t>  & V, std::vector<int> &statusV)  //4
{
    CAFE::get(handleV, statusV);
    CAFE::waitForBundledEvents(handleV, statusV);
    return CAFE::getCache(handleV, V, statusV);
}
int getScalars(std::vector<unsigned int> handleV, std::vector<dbr_long_t>  & V, std::vector<int> &statusV)  //5
{
    CAFE::get(handleV, statusV);
    CAFE::waitForBundledEvents(handleV, statusV);
    return CAFE::getCache(handleV, V, statusV);
}
int getScalars(std::vector<unsigned int> handleV, std::vector<long long>  & V, std::vector<int> &statusV)   //5
{
    CAFE::get(handleV, statusV);
    CAFE::waitForBundledEvents(handleV, statusV);
    return CAFE::getCache(handleV, V, statusV);
}
int getScalars(std::vector<unsigned int> handleV, std::vector<dbr_double_t>  & V, std::vector<int> &statusV)   //6
{
    CAFE::get(handleV, statusV);
    //for (size_t i=0; i< handleV.size(); ++i) { cout << "/Asyn/h=" <<  handleV[i] << "s=" << statusV[i] << " [" << i << "] " << endl;}
    CAFE::waitForBundledEvents(handleV, statusV);
    //for (size_t i=0; i< handleV.size(); ++i) { cout << "/Wait/h=" <<  handleV[i] << "s=" << statusV[i] << " [" << i << "] " << endl;}
    status=CAFE::getCache(handleV, V, statusV);
    //for (size_t i=0; i< handleV.size(); ++i) { cout << "/Cach/h=" <<  handleV[i] << "s=" << statusV[i] << " [" << i << "] " << endl;}
    return status;
}




int getCacheVStr(std::vector<unsigned int> handleV, std::vector<std::string> & V, std::vector<int> &statusV)
{
    return getCache(handleV,  V, statusV);
}
int getCacheVLong(std::vector<unsigned int> handleV, std::vector<dbr_long_t> & V, std::vector<int> &statusV)
{
    return getCache(handleV,  V, statusV);
}
int getCacheVDouble(std::vector<unsigned int> handleV, std::vector<dbr_double_t> & V, std::vector<int> &statusV)
{
    return getCache(handleV,  V, statusV);
}


int getCache(std::vector<unsigned int> handleV, std::vector<std::string> & V, std::vector<int> &statusV); //0
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_short_t> & V, std::vector<int> &statusV); //1
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_float_t> & V, std::vector<int> &statusV); //2
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_enum_t>  & V, std::vector<int> &statusV); //3
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_char_t>  & V, std::vector<int> &statusV); //4
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_long_t>  & V, std::vector<int> &statusV); //5
int getCache(std::vector<unsigned int> handleV, std::vector<long long>  & V, std::vector<int> &statusV); //5
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_double_t>  & V, std::vector<int> &statusV); //6

int getCache(std::vector<unsigned int> handleV, std::vector<std::string> & V, std::vector<int> &statusV,
             std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV); //0
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_short_t> & V, std::vector<int> &statusV,
             std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV); //1
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_float_t> & V, std::vector<int> &statusV,
             std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV); //2
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_enum_t> & V, std::vector<int> &statusV,
             std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV); //3
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_char_t> & V, std::vector<int> &statusV,
             std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV); //4
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_long_t> & V, std::vector<int> &statusV,
             std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV); //5
int getCache(std::vector<unsigned int> handleV, std::vector<long long> & V, std::vector<int> &statusV,
             std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV); //5
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_double_t> & V, std::vector<int> &statusV,
             std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV); //6

//0
int getCache(std::vector<unsigned int> handleV, std::vector<std::string> & V, std::vector<int> &statusV,
             std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV,
             std::vector<epicsTimeStamp> &tsV);
//1
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_short_t> & V, std::vector<int> &statusV,
             std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV,
             std::vector<epicsTimeStamp> &tsV);
//2
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_float_t> & V, std::vector<int> &statusV,
             std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV,
             std::vector<epicsTimeStamp> &tsV);
//3
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_enum_t> & V, std::vector<int> &statusV,
             std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV,
             std::vector<epicsTimeStamp> &tsV);
//4
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_char_t> & V, std::vector<int> &statusV,
             std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV,
             std::vector<epicsTimeStamp> &tsV);
//5
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_long_t> & V, std::vector<int> &statusV,
             std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV,
             std::vector<epicsTimeStamp> &tsV);
int getCache(std::vector<unsigned int> handleV, std::vector<long long> & V, std::vector<int> &statusV,
             std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV,
             std::vector<epicsTimeStamp> &tsV);
//6
int getCache(std::vector<unsigned int> handleV, std::vector<dbr_double_t> & V, std::vector<int> &statusV,
             std::vector<dbr_short_t> &alarmStatusV, std::vector<dbr_short_t> &alarmSeverityV,
             std::vector<epicsTimeStamp> &tsV);


///////////////////////////////////////////////////////////////////////////////////////////

int set(const unsigned int  _handle, std::vector<std::string> V);         //0
int set(const unsigned int  _handle, std::vector<dbr_short_t>  V);         //1
int set(const unsigned int  _handle, std::vector<dbr_float_t>  V);         //2
int set(const unsigned int  _handle, std::vector<dbr_enum_t>  V);//3
int set(const unsigned int  _handle, std::vector<dbr_char_t>   V);//4
int set(const unsigned int  _handle, std::vector<dbr_long_t>  V);    //5
int set(const unsigned int  _handle, std::vector<long long>  V);    //5+
int set(const unsigned int  _handle, std::vector<dbr_double_t> V);         //6

int set(std::vector<unsigned int> _handleV, std::vector<std::string> V, std::vector<int> &statusV);         //0
int set(std::vector<unsigned int> _handleV, std::vector<dbr_short_t>  V, std::vector<int> &statusV);         //1
int set(std::vector<unsigned int> _handleV, std::vector<dbr_float_t>  V, std::vector<int> &statusV);         //2
int set(std::vector<unsigned int> _handleV, std::vector<dbr_enum_t> V, std::vector<int> &statusV); //3
int set(std::vector<unsigned int> _handleV, std::vector<dbr_char_t>  V, std::vector<int> &statusV); //4
int set(std::vector<unsigned int> _handleV, std::vector<dbr_long_t> V, std::vector<int> &statusV);     //5
int set(std::vector<unsigned int> _handleV, std::vector<long long> V, std::vector<int> &statusV);     //5+
int set(std::vector<unsigned int> _handleV, std::vector<dbr_double_t> V, std::vector<int> &statusV);         //6



int  get (std::vector<unsigned int> handleV, PVDataHolder * pvd)
{

    return get(&handleV[0], (unsigned int) handleV.size(), pvd);

};


int  getCache (std::vector<unsigned int> handleV, PVDataHolder * pvd)
{

    return getCache(&handleV[0], (unsigned int) handleV.size(), pvd);
};


int  getPVArray (std::vector<unsigned int> handleV, PVDataHolder * pvd)
{
    //unsigned int * handleArray = new unsigned int [handleV.size()];
    //for (size_t i=0; i< (size_t) handleV.size(); ++i) {handleArray[i]=  (unsigned int) handleV[i];}
    //status=get(handleArray, (unsigned int) handleV.size(), pvd);
    //delete [] handleArray; return status;
    return get(&handleV[0], (unsigned int) handleV.size(), pvd);
};

/*
int  getPVArrayCache (std::vector<unsigned int> handleV, PVDataHolder * pvd){
	//unsigned int * handleArray = new unsigned int [handleV.size()];
	//for (size_t i=0; i< (size_t) handleV.size(); ++i) {handleArray[i]=  (unsigned int) handleV[i];}
	//status=getCache(handleArray, (unsigned int) handleV.size(), pvd);
	//delete [] handleArray; return status;
	 return getCache(&handleV[0], (unsigned int) handleV.size(), pvd);
};
*/


//setPVArray is in cafeStd::Vectors.h and .cc
//For Cython
int setPVArray(std::vector<unsigned int> handleV, PVDataHolder * pvd);

int setVVString(std::vector<unsigned int> handleV, std::vector<std::string> V, std::vector<int> &statusV)
{
    return set(handleV, V, statusV);
};
int setVVChar(std::vector<unsigned int> handleV, std::vector<dbr_char_t>  V, std::vector<int> &statusV)
{
    return set(handleV, V, statusV);
};
int setVVLong(std::vector<unsigned int> handleV, std::vector<dbr_long_t> V, std::vector<int> &statusV)
{
    return set(handleV, V, statusV);
};
int setVVDouble(std::vector<unsigned int> handleV, std::vector<dbr_double_t> V, std::vector<int> &statusV)
{
    return set(handleV, V, statusV);
};

int setVString(const unsigned int  handle, std::vector<std::string> V)
{
    return set(handle, V);
};

int setVChar  (const unsigned int  handle, std::vector<dbr_char_t>   V)
{
    return set(handle, V);
};
int setVLong  (const unsigned int  handle, std::vector<dbr_long_t>   V)
{
    return set(handle, V);
};
int setVShort (const unsigned int  handle, std::vector<dbr_short_t>  V)
{
    return set(handle, V);
};
int setVUShort(const unsigned int  handle, std::vector<dbr_enum_t>   V)
{
    return set(handle, V);
};
int setVFloat (const unsigned int  handle, std::vector<dbr_float_t>  V)
{
    return set(handle, V);
};
int setVDouble(const unsigned int  handle, std::vector<dbr_double_t> V)
{
    return set(handle, V);
};




#endif // CAFEVECTORS_H
