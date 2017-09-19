///
/// \file    cafeVectors.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef CAFEVECTORS_H
#define CAFEVECTORS_H

#include <string>

int get(const unsigned int  _handle, vector<string> & V);         //0
int get(const unsigned int  _handle, vector<short> & V);          //1
int get(const unsigned int  _handle, vector<float> & V);          //2
int get(const unsigned int  _handle, vector<unsigned short> & V); //3
int get(const unsigned int  _handle, vector<unsigned char>  & V); //4
int get(const unsigned int  _handle, vector<dbr_long_t> & V);     //5
int get(const unsigned int  _handle, vector<long long> & V);      //5
int get(const unsigned int  _handle, vector<double> & V);         //6
int get(const unsigned int  _handle, vector<string> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//0
int get(const unsigned int  _handle, vector<short> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//1
int get(const unsigned int  _handle, vector<float> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//2
int get(const unsigned int  _handle, vector<unsigned short> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//3
int get(const unsigned int  _handle, vector<unsigned char> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//4
int get(const unsigned int  _handle, vector<dbr_long_t> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//5
int get(const unsigned int  _handle, vector<long long> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//5
int get(const unsigned int  _handle, vector<double> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//6
int get(const unsigned int  _handle, vector<string> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//0
int get(const unsigned int  _handle, vector<short> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//1
int get(const unsigned int  _handle, vector<float> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//2
int get(const unsigned int  _handle, vector<unsigned short> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//3
int get(const unsigned int  _handle, vector<unsigned char> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//4
int get(const unsigned int  _handle, vector<dbr_long_t> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//5
int get(const unsigned int  _handle, vector<long long> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//5
int get(const unsigned int  _handle, vector<double> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//

int getCache(const unsigned int  _handle, vector<string> & V);        //0
int getCache(const unsigned int  _handle, vector<short> & V);         //1
int getCache(const unsigned int  _handle, vector<float> & V);         //2
int getCache(const unsigned int  _handle, vector<unsigned short> & V);//3
int getCache(const unsigned int  _handle, vector<unsigned char>  & V);//4
int getCache(const unsigned int  _handle, vector<dbr_long_t> & V);    //5
int getCache(const unsigned int  _handle, vector<long long> & V);          //5
int getCache(const unsigned int  _handle, vector<double> & V);        //6
int getCache(const unsigned int  _handle, vector<string> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//0
int getCache(const unsigned int  _handle, vector<short> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//1
int getCache(const unsigned int  _handle, vector<float> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//2
int getCache(const unsigned int  _handle, vector<unsigned short> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//3
int getCache(const unsigned int  _handle, vector<unsigned char> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//4
int getCache(const unsigned int  _handle, vector<dbr_long_t> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//5
int getCache(const unsigned int  _handle, vector<long long> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//5
int getCache(const unsigned int  _handle, vector<double> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity);//6


int getCache(const unsigned int  _handle, vector<string> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//0
int getCache(const unsigned int  _handle, vector<short> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//1
int getCache(const unsigned int  _handle, vector<float> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//2
int getCache(const unsigned int  _handle, vector<unsigned short> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//3
int getCache(const unsigned int  _handle, vector<unsigned char> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//4
int getCache(const unsigned int  _handle, vector<dbr_long_t> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//5
int getCache(const unsigned int  _handle, vector<long long> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//5
int getCache(const unsigned int  _handle, vector<double> & V,
        dbr_short_t &alarmStatus, dbr_short_t &alarmSeverity, epicsTimeStamp &ts);//6


int get(vector<unsigned int> handleV, vector<int> &statusV);

int getV(vector<unsigned int> handleV, vector<int> &statusV){
	status=get(handleV, statusV); ca_flush_io(); //Yes let's flush here!
    return status;}

int getScalars(vector<unsigned int> handleV, vector<string> & V, vector<int> &statusV){ //0
	CAFE::get(handleV, statusV);
	CAFE::waitForBundledEvents(handleV, statusV);
	return CAFE::getCache(handleV, V, statusV);
}
int getScalars(vector<unsigned int> handleV, vector<dbr_short_t> & V, vector<int> &statusV){ //1
	CAFE::get(handleV, statusV);
	CAFE::waitForBundledEvents(handleV, statusV);
	return CAFE::getCache(handleV, V, statusV);
}
int getScalars(vector<unsigned int> handleV, vector<dbr_float_t> & V, vector<int> &statusV){ //2
	CAFE::get(handleV, statusV);
	CAFE::waitForBundledEvents(handleV, statusV);
	return CAFE::getCache(handleV, V, statusV);
}
int getScalars(vector<unsigned int> handleV, vector<dbr_enum_t>  & V, vector<int> &statusV){ //3
	CAFE::get(handleV, statusV);
	CAFE::waitForBundledEvents(handleV, statusV);
	return CAFE::getCache(handleV, V, statusV);
}
int getScalars(vector<unsigned int> handleV, vector<dbr_char_t>  & V, vector<int> &statusV){ //4
	CAFE::get(handleV, statusV);
	CAFE::waitForBundledEvents(handleV, statusV);
	return CAFE::getCache(handleV, V, statusV);
}
int getScalars(vector<unsigned int> handleV, vector<dbr_long_t>  & V, vector<int> &statusV){ //5
	CAFE::get(handleV, statusV);
	CAFE::waitForBundledEvents(handleV, statusV);
	return CAFE::getCache(handleV, V, statusV);
}
int getScalars(vector<unsigned int> handleV, vector<long long>  & V, vector<int> &statusV) { //5
	CAFE::get(handleV, statusV);
	CAFE::waitForBundledEvents(handleV, statusV);
	return CAFE::getCache(handleV, V, statusV);
}
int getScalars(vector<unsigned int> handleV, vector<dbr_double_t>  & V, vector<int> &statusV) { //6
	CAFE::get(handleV, statusV);
	//for (size_t i=0; i< handleV.size(); ++i) { cout << "/Asyn/h=" <<  handleV[i] << "s=" << statusV[i] << " [" << i << "] " << endl;}
	CAFE::waitForBundledEvents(handleV, statusV);
	//for (size_t i=0; i< handleV.size(); ++i) { cout << "/Wait/h=" <<  handleV[i] << "s=" << statusV[i] << " [" << i << "] " << endl;}
	status=CAFE::getCache(handleV, V, statusV);
	//for (size_t i=0; i< handleV.size(); ++i) { cout << "/Cach/h=" <<  handleV[i] << "s=" << statusV[i] << " [" << i << "] " << endl;}
	return status;
}




int getCacheVStr(vector<unsigned int> handleV, vector<string> & V, vector<int> &statusV){
    return getCache(handleV,  V, statusV);}
int getCacheVLong(vector<unsigned int> handleV, vector<dbr_long_t> & V, vector<int> &statusV){
	return getCache(handleV,  V, statusV);}
int getCacheVDouble(vector<unsigned int> handleV, vector<dbr_double_t> & V, vector<int> &statusV){
	return getCache(handleV,  V, statusV);}


int getCache(vector<unsigned int> handleV, vector<string> & V, vector<int> &statusV); //0
int getCache(vector<unsigned int> handleV, vector<dbr_short_t> & V, vector<int> &statusV); //1
int getCache(vector<unsigned int> handleV, vector<dbr_float_t> & V, vector<int> &statusV); //2
int getCache(vector<unsigned int> handleV, vector<dbr_enum_t>  & V, vector<int> &statusV); //3
int getCache(vector<unsigned int> handleV, vector<dbr_char_t>  & V, vector<int> &statusV); //4
int getCache(vector<unsigned int> handleV, vector<dbr_long_t>  & V, vector<int> &statusV); //5
int getCache(vector<unsigned int> handleV, vector<long long>  & V, vector<int> &statusV); //5
int getCache(vector<unsigned int> handleV, vector<dbr_double_t>  & V, vector<int> &statusV); //6

int getCache(vector<unsigned int> handleV, vector<string> & V, vector<int> &statusV,
               vector<dbr_short_t> &alarmStatusV, vector<dbr_short_t> &alarmSeverityV); //0
int getCache(vector<unsigned int> handleV, vector<dbr_short_t> & V, vector<int> &statusV,
               vector<dbr_short_t> &alarmStatusV, vector<dbr_short_t> &alarmSeverityV); //1
int getCache(vector<unsigned int> handleV, vector<dbr_float_t> & V, vector<int> &statusV,
               vector<dbr_short_t> &alarmStatusV, vector<dbr_short_t> &alarmSeverityV); //2
int getCache(vector<unsigned int> handleV, vector<dbr_enum_t> & V, vector<int> &statusV,
               vector<dbr_short_t> &alarmStatusV, vector<dbr_short_t> &alarmSeverityV); //3
int getCache(vector<unsigned int> handleV, vector<dbr_char_t> & V, vector<int> &statusV,
               vector<dbr_short_t> &alarmStatusV, vector<dbr_short_t> &alarmSeverityV); //4
int getCache(vector<unsigned int> handleV, vector<dbr_long_t> & V, vector<int> &statusV,
               vector<dbr_short_t> &alarmStatusV, vector<dbr_short_t> &alarmSeverityV); //5
int getCache(vector<unsigned int> handleV, vector<long long> & V, vector<int> &statusV,
               vector<dbr_short_t> &alarmStatusV, vector<dbr_short_t> &alarmSeverityV); //5
int getCache(vector<unsigned int> handleV, vector<dbr_double_t> & V, vector<int> &statusV,
               vector<dbr_short_t> &alarmStatusV, vector<dbr_short_t> &alarmSeverityV); //6

//0
int getCache(vector<unsigned int> handleV, vector<string> & V, vector<int> &statusV,
               vector<dbr_short_t> &alarmStatusV, vector<dbr_short_t> &alarmSeverityV,
               vector<epicsTimeStamp> &tsV);
//1
int getCache(vector<unsigned int> handleV, vector<dbr_short_t> & V, vector<int> &statusV,
               vector<dbr_short_t> &alarmStatusV, vector<dbr_short_t> &alarmSeverityV,
               vector<epicsTimeStamp> &tsV);
//2
int getCache(vector<unsigned int> handleV, vector<dbr_float_t> & V, vector<int> &statusV,
               vector<dbr_short_t> &alarmStatusV, vector<dbr_short_t> &alarmSeverityV,
               vector<epicsTimeStamp> &tsV);
//3
int getCache(vector<unsigned int> handleV, vector<dbr_enum_t> & V, vector<int> &statusV,
               vector<dbr_short_t> &alarmStatusV, vector<dbr_short_t> &alarmSeverityV,
               vector<epicsTimeStamp> &tsV);
//4
int getCache(vector<unsigned int> handleV, vector<dbr_char_t> & V, vector<int> &statusV,
               vector<dbr_short_t> &alarmStatusV, vector<dbr_short_t> &alarmSeverityV,
               vector<epicsTimeStamp> &tsV);
//5
int getCache(vector<unsigned int> handleV, vector<dbr_long_t> & V, vector<int> &statusV,
               vector<dbr_short_t> &alarmStatusV, vector<dbr_short_t> &alarmSeverityV,
               vector<epicsTimeStamp> &tsV);
int getCache(vector<unsigned int> handleV, vector<long long> & V, vector<int> &statusV,
               vector<dbr_short_t> &alarmStatusV, vector<dbr_short_t> &alarmSeverityV,
               vector<epicsTimeStamp> &tsV);
//6
int getCache(vector<unsigned int> handleV, vector<dbr_double_t> & V, vector<int> &statusV,
               vector<dbr_short_t> &alarmStatusV, vector<dbr_short_t> &alarmSeverityV,
               vector<epicsTimeStamp> &tsV);


///////////////////////////////////////////////////////////////////////////////////////////

int set(const unsigned int  handle, vector<string> V);         //0
int set(const unsigned int  handle, vector<dbr_short_t>  V);         //1
int set(const unsigned int  handle, vector<dbr_float_t>  V);         //2
int set(const unsigned int  handle, vector<dbr_enum_t>  V);//3
int set(const unsigned int  handle, vector<dbr_char_t>   V);//4
int set(const unsigned int  handle, vector<dbr_long_t>  V);    //5
int set(const unsigned int  handle, vector<long long>  V);    //5+
int set(const unsigned int  handle, vector<dbr_double_t> V);         //6

int set(vector<unsigned int> handleV, vector<string> V, vector<int> &statusV);         //0
int set(vector<unsigned int> handleV, vector<dbr_short_t>  V, vector<int> &statusV);         //1
int set(vector<unsigned int> handleV, vector<dbr_float_t>  V, vector<int> &statusV);         //2
int set(vector<unsigned int> handleV, vector<dbr_enum_t> V, vector<int> &statusV); //3
int set(vector<unsigned int> handleV, vector<dbr_char_t>  V, vector<int> &statusV); //4
int set(vector<unsigned int> handleV, vector<dbr_long_t> V, vector<int> &statusV);     //5
int set(vector<unsigned int> handleV, vector<long long> V, vector<int> &statusV);     //5+
int set(vector<unsigned int> handleV, vector<dbr_double_t> V, vector<int> &statusV);         //6



int  get (vector<unsigned int> handleV, PVDataHolder * pvd){
	
	return get(&handleV[0], (unsigned int) handleV.size(), pvd);
	
};


int  getCache (vector<unsigned int> handleV, PVDataHolder * pvd){
	
	return getCache(&handleV[0], (unsigned int) handleV.size(), pvd);
};


int  getPVArray (vector<unsigned int> handleV, PVDataHolder * pvd){
	//unsigned int * handleArray = new unsigned int [handleV.size()];
	//for (size_t i=0; i< (size_t) handleV.size(); ++i) {handleArray[i]=  (unsigned int) handleV[i];}
	//status=get(handleArray, (unsigned int) handleV.size(), pvd);
	//delete [] handleArray; return status;
		return get(&handleV[0], (unsigned int) handleV.size(), pvd);
};

/*
int  getPVArrayCache (vector<unsigned int> handleV, PVDataHolder * pvd){
	//unsigned int * handleArray = new unsigned int [handleV.size()];
	//for (size_t i=0; i< (size_t) handleV.size(); ++i) {handleArray[i]=  (unsigned int) handleV[i];}
	//status=getCache(handleArray, (unsigned int) handleV.size(), pvd);
	//delete [] handleArray; return status;
	 return getCache(&handleV[0], (unsigned int) handleV.size(), pvd);
};
*/


//setPVArray is in cafeVectors.h and .cc
//For Cython
int setPVArray(vector<unsigned int> handleV, PVDataHolder * pvd);

int setVVString(vector<unsigned int> handleV, vector<string> V, vector<int> &statusV) {
    return set(handleV, V, statusV);};
int setVVChar(vector<unsigned int> handleV, vector<dbr_char_t>  V, vector<int> &statusV){
    return set(handleV, V, statusV);};
int setVVLong(vector<unsigned int> handleV, vector<dbr_long_t> V, vector<int> &statusV){
    return set(handleV, V, statusV);};
int setVVDouble(vector<unsigned int> handleV, vector<dbr_double_t> V, vector<int> &statusV){
    return set(handleV, V, statusV);};

int setVString(const unsigned int  handle, vector<string> V)        {return set(handle, V);};

int setVChar  (const unsigned int  handle, vector<dbr_char_t>   V)  {return set(handle, V);};
int setVLong  (const unsigned int  handle, vector<dbr_long_t>   V)  {return set(handle, V);};
int setVShort (const unsigned int  handle, vector<dbr_short_t>  V)  {return set(handle, V);};
int setVUShort(const unsigned int  handle, vector<dbr_enum_t>   V)  {return set(handle, V);};
int setVFloat (const unsigned int  handle, vector<dbr_float_t>  V)  {return set(handle, V);};
int setVDouble(const unsigned int  handle, vector<dbr_double_t> V)  {return set(handle, V);};




#endif // CAFEVECTORS_H
