///
/// \file    cafeService.h
/// \author  Jan Chrin, PSI
/// \date    Release October 2017
/// \version CAFE 1.4.0
///

#ifndef CAFE_SERVICE_H
#define CAFE_SERVICE_H

#include <config.h>
#include <vector>
#include <PVDataHolder.h>
#include <statusCodes.h>
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


class BSChannel {
private:
	string name;
	int modulo;
	int offset;
	HandleHelper helper;

public:
	PVDataHolder pvd;
	std::vector<std::string>  pv;
	//void setName(string _name)  { name=_name;}
	void setOffset(int _offset) { offset=_offset;}
	void setModulo(int _modulo) { modulo=_modulo;}
	string getName(){return name;}
	int getModulo(){return modulo;}
	int getOffset(){return offset;}
	BSChannel(string _name): modulo(1), offset(0) {
		char pv[PVNAME_SIZE];
		helper.removeLeadingAndTrailingSpaces(_name.c_str(), pv);
		name=(string) pv;};
	BSChannel(string _name, int _modulo): offset(0)	{
		char pv[PVNAME_SIZE];
		helper.removeLeadingAndTrailingSpaces(_name.c_str(), pv);
		name=(string) pv; modulo=_modulo;};
	BSChannel(string _name, int _modulo, int _offset) {
		char pv[PVNAME_SIZE];
		helper.removeLeadingAndTrailingSpaces(_name.c_str(), pv);
		name=(string) pv; modulo=_modulo; offset=_offset;};
};


class BSDataHolder{
friend class CAFE;
private:	
	int overallStatus;	
	unsigned long long pulse_id;
	vector<BSChannel> bsChannel;
	HandleHelper helper;

	std::vector<std::string>  pv;

	bool isBS;
	bool BSInitialized;
	void *context;

	void *receiver;
	int rc;

	#if HAVE_JSON
	Json::Value parsedFromString;
	Json::Reader reader;
	#endif
	bool parsingSuccessful;


public:	
	BSDataHolder(){};

	void *subscriber;

	vector<double> getAsDoubleV() {
		vector<double> V;
		V.reserve(bsChannel.size());
		for (size_t i=0; i<bsChannel.size(); ++i){
			V.push_back(bsChannel[i].pvd.getAsDouble());
		}
		return V;
	}

	vector<string> getAsStringV() {
		vector<string> V;
		V.reserve(bsChannel.size());
		for (size_t i=0; i<bsChannel.size(); ++i){
			V.push_back(bsChannel[i].pvd.getAsString());
		}
		return V;
	}

	vector<int> getAsIntV() {
		vector<int> V;
		V.reserve(bsChannel.size());
		for (size_t i=0; i<bsChannel.size(); ++i){
			V.push_back(bsChannel[i].pvd.getAsInt());
		}
		return V;
	}

	vector<double> getAttributeAsDoubleV(string attribute) {
		vector<double> V;
		V.reserve(bsChannel.size());
		char pvAtt[PVNAME_SIZE];
		helper.removeLeadingAndTrailingSpaces(attribute.c_str(), pvAtt);
		for (size_t i=0; i<bsChannel.size(); ++i){
			if ( ((string)pvAtt).compare((string) bsChannel[i].pvd.getAttribute()) ==0){
				V.push_back(bsChannel[i].pvd.getAsDouble());
			}
		}
		return V;
	}

	PVDataHolder   getPVData(unsigned int idx) {
		if (idx > (bsChannel.size()-1)) {
			idx=bsChannel.size()-1;
		}
		return bsChannel[idx].pvd;
	}


	PVDataHolder   getPVData(string name) {

		for (size_t i=0; i< bsChannel.size(); ++i) {
			if (bsChannel[i].getName().compare(name) ==0 ) {
				return bsChannel[i].pvd;
			}
		}
	}


	int getStatus() { return overallStatus;}


	static size_t RecvResponseCallback(char * contents, size_t size, size_t nmemb, void  * up) {

		++nCBs;
		//cout << "Callback called: " << nCBs << endl;
		//cout << "SIZE No. of Bytes " << size*nmemb << endl;

		string sLocal=contents;

		//remove \n for newline
		std::size_t found = sLocal.find('\n');

		if (found != std::string::npos) {

			sLocal=sLocal.substr(0, found);
		}

		contentsBS=contentsBS+sLocal;

		return (size_t) size * nmemb;
	}


	bool setBS(bool BSFlag) {

	if(MUTEX){cafeMutex.lock();}

	if (BSFlag) {
		#if HAVE_CURL

		string dataChannels=string("{\"channels\":[");
		vector<string> pvNew=pv;

		#if HAVE_ZEROMQ

		if (!BSInitialized) {

			//size_t found;
			dataChannels= dataChannels + string("{\"name\":\"");
			dataChannels= dataChannels + pvNew[0];
			dataChannels= dataChannels + string("\",\"backend\":\"sf-databuffer\",\"modulo\":1,\"offset\":0}" );


			for (size_t i=1; i < pvNew.size(); ++i) {
				dataChannels= dataChannels + string(",{\"name\":\"");
				dataChannels= dataChannels + pvNew[i];
				dataChannels= dataChannels + string("\",\"backend\":\"sf-databuffer\",\"modulo\":1,\"offset\":0}");
			}

			dataChannels= dataChannels + string("],");
			dataChannels= dataChannels + "\"mapping\":{\"incomplete\":\"fill-null\"},\"channelValidation\":{\"inconsistency\":\"keep-as-is\"},\"sendBehaviour\":{\"strategy\":\"complete-all\"}}";

			cout <<  dataChannels << endl;

			const char * data = dataChannels.c_str();

			///cout << "SIZE OF DATA --------------->"  << sizeof(data) << endl;

			CURL *curl;
			CURLcode res;
			struct curl_slist * slist;
			slist = NULL;

			slist = curl_slist_append(slist, "Content-Type: application/json");

			curl_global_init(CURL_GLOBAL_ALL);

			curl = curl_easy_init();

			if (curl) {

				curl_easy_setopt(curl, CURLOPT_URL, "https://dispatcher-api.psi.ch/sf/stream");

				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data); //"-F file=@./dbpm.json"); //data); //

				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
				curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

				//cout << "WAITING FOR CALLBACK... " << endl;

				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RecvResponseCallback);

				res = curl_easy_perform(curl);

				if (res != CURLE_OK) {
					cout << "curl_easy_perform failed " << curl_easy_strerror(res) << endl;
				}
				else {
					cout << " CALLBACK DONE" << endl;

					curl_easy_cleanup(curl);

					curl_slist_free_all(slist);

					slist=NULL;
				}
			}//if curl

			cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
			curl_global_cleanup();


			//cout << " //1// SHOW contentS " << endl;
			//cout << contentsBS.c_str() << endl;

			Json::Value parsedFromString;
			Json::Reader reader;
			bool parsingSuccessful;

			Json::FastWriter fastWriter;
			string globalBSZmqStream="";
			//printf("value= %s\n", contentsBS.c_str());

			if (contentsBS.size() > 2) {
				parsingSuccessful=reader.parse(contentsBS.c_str(), parsedFromString);
				if (parsingSuccessful) {
					//Json::StyledWriter styledWriter;
					//cout << "STYLED: --------------------------------" << endl;
					//cout << styledWriter.write(parsedFromString) << endl;
					//cout << "----------------------------------" << endl;
					cout << parsedFromString["stream"] << endl;

					cout << "----------------------------------" << endl;
					globalBSZmqStream = fastWriter.write(parsedFromString["stream"]).c_str();
					cout << globalBSZmqStream << endl;

					if ( parsedFromString["stream"].isNull() ) {
						globalBSZmqStream.clear();
					}
				}
				else {
					cout << "PARSING IN CURL CALLBACK FUNCTION WAS UNSUCCESSFUL !!!" << endl;
					cout << contentsBS.c_str() << endl;
					cout << reader.getFormattedErrorMessages() << endl;

				}
			}

			if (globalBSZmqStream.empty()) {
				 cout << "BS Data is not available " << endl;
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


			globalBSZmqStream=globalBSZmqStream.substr(1,globalBSZmqStream.size()-3);
			//cout << " globalBSZmqStream.c_str() " << globalBSZmqStream.c_str() << endl;

			rc = zmq_connect (subscriber, (const char *) globalBSZmqStream.c_str());

			if (rc != 0 ) {
				cout << " Error is " << zmq_errno() << " " << zmq_strerror(zmq_errno()) << endl;
			}


			assert (rc == 0);

			int nhwm=1;
			int timeoutMS=200; //10; //-1 Wait for Ever

			rc=zmq_setsockopt (subscriber,ZMQ_RCVHWM, &nhwm, sizeof(int));
			assert (rc == 0);

			rc=zmq_setsockopt (subscriber,ZMQ_RCVTIMEO, &timeoutMS, sizeof(int));
			assert (rc == 0);

			rc=zmq_setsockopt (subscriber,ZMQ_SUBSCRIBE,"",0);
			assert (rc == 0);

			BSInitialized=true;

		}//is BS initialized

		#endif //have zeromq

		if(MUTEX){cafeMutex.unlock();}
		return isBS=BSFlag;
		#else //have curl

		if(MUTEX){cafeMutex.unlock();}
		return isBS=false;
		#endif //have curl
	}//isBSFlag


	if(MUTEX){cafeMutex.unlock();}
	return isBS=BSFlag;
	} // setBS

};

/*
class RFData{
friend class CAFE;
private:
	std::vector<BSData> phase;
	std::vector<BSData> amplitude;
	
	
	std::vector<std::string>  pv;
	std::vector<unsigned int> handle;
	std::vector<std::string>  device;
	std::vector<float>    s; 
		
	size_t nDevice;
	size_t nPV;	
}
*/

class DBPMData{
friend class CAFE;
private:
	double val;
	epicsTimeStamp ets;
	int status;
public:
	double getValue() {return val;}
	epicsTimeStamp getEpicsTimeStamp() {return ets;}
	int getStatus() { return status;}	
	
	DBPMData(){
	//status=ECAFE_BPM_DATA_IS_INVALID;	
	};
};


class DBPMKeeper
{
friend class CAFE;
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
	bool parsingSuccessful;

public:	

	std::vector<DBPMData> getX() { return x;}
	std::vector<DBPMData> getY() { return y;}
	std::vector<DBPMData> getQ() { return q;}
	std::vector<DBPMData> getEnergy() { return energy;}
	
	
	std::vector<double> getOffsetX() { return offs_x;}
	std::vector<double> getOffsetY() { return offs_y;}
	
	bool getIsAllXOK() {return isAllXOK;}
	bool getIsAllYOK() {return isAllYOK;}
	bool getIsAllQOK() {return isAllQOK;}	
	bool getIsAllEOK() {return isAllEOK;}	
	bool getIsAllOK()  {return isAllOK;}	
		
	std::vector<std::string>  getPV(){ return pv;}
	std::vector<unsigned int> getHandle() { return handle;}
	std::vector<std::string>  getDevice() { return device;}
	std::vector<float>        getS() { return s;} 
	size_t getNDBPM() {return nDBPM;}
	size_t getNPV()   {return nPV;}
	int getStatus()   {return status;}
	
	
	int getPVIdx(string _pv) {				
		for (size_t i=0;  i< pv.size(); ++i) {	 
			if ( pv[i].compare(_pv) == 0) {			
				return i;
			}		
		}
		return -1;
	}
	
	
	unsigned long long getPulse_id(){return pulse_id;}
	void setPulse_id(unsigned long long _pulse_id){pulse_id=_pulse_id;}
	
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

	static size_t RecvResponseCallback(char * contents, size_t size, size_t nmemb, void  * up) {

		++nCBs;
		//cout << "Callback called: " << nCBs << endl;
		//cout << "SIZE No. of Bytes " << size*nmemb << endl;

		string sLocal=contents;

		//remove \n for newline
		std::size_t found = sLocal.find('\n');

		if (found != std::string::npos) {

			sLocal=sLocal.substr(0, found);
		}

		contentsS=contentsS+sLocal;
		
		return (size_t) size * nmemb;
	}

	

	bool resetBS() {
		closeBS();
		return setBS(true);	
	}


	bool setBS(bool BSFlag) {

	if(MUTEX){cafeMutex.lock();}

	if (BSFlag) {
		#if HAVE_CURL

		string dataChannels=string("{\"channels\":[");
		vector<string> pvNew=pv;

		#if HAVE_ZEROMQ

		if (!BSInitialized) {

			size_t found;
			dataChannels= dataChannels + string("{\"name\":\"");
			dataChannels= dataChannels + pvNew[0];
			//dataChannels= dataChannels + string("\",\"backend\":\"sf-databuffer\"}"		);
			dataChannels= dataChannels + string("\",\"backend\":\"sf-databuffer\",\"modulo\":1,\"offset\":0}" );

				
			for (size_t i=1; i < pvNew.size(); ++i) {
					
				found = pvNew[i].find("SARUN08-DBPM210");
				if (found != std::string::npos) continue;
				found = pvNew[i].find("SARUN08-DBPM410");
				if (found != std::string::npos) continue;

				found = pvNew[i].find("ENERGY");
				if (found != std::string::npos) continue;
				
			
				dataChannels= dataChannels + string(",{\"name\":\"");
				dataChannels= dataChannels + pvNew[i];

				dataChannels= dataChannels + string("\",\"backend\":\"sf-databuffer\",\"modulo\":1,\"offset\":0}");
					
			}

			dataChannels= dataChannels + string("],");
			dataChannels= dataChannels + "\"mapping\":{\"incomplete\":\"fill-null\"},\"channelValidation\":{\"inconsistency\":\"keep-as-is\"},\"sendBehaviour\":{\"strategy\":\"complete-all\"}}";

			cout <<  dataChannels << endl;

			const char * data = dataChannels.c_str();

			///cout << "SIZE OF DATA --------------->"  << sizeof(data) << endl;

			CURL *curl;
			CURLcode res;
			struct curl_slist * slist;
			slist = NULL;
			
			slist = curl_slist_append(slist, "Content-Type: application/json");
		
			curl_global_init(CURL_GLOBAL_ALL);
								
			curl = curl_easy_init();
							
			if (curl) {
			
				curl_easy_setopt(curl, CURLOPT_URL, "https://dispatcher-api.psi.ch/sf/stream");
			
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data); //"-F file=@./dbpm.json"); //data); //
			
				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
				curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
				
				//cout << "WAITING FOR CALLBACK... " << endl;

				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RecvResponseCallback);

				res = curl_easy_perform(curl);
				
				if (res != CURLE_OK) {
					cout << "curl_easy_perform failed " << curl_easy_strerror(res) << endl;
				}
				else {
					cout << " CALLBACK DONE" << endl;

					curl_easy_cleanup(curl);

					curl_slist_free_all(slist);

					slist=NULL;
				}
			}//if curl
			
			cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
			curl_global_cleanup();
			

			//cout << " //1// SHOW contentS " << endl;
			//cout << contentsS.c_str() << endl;

			Json::Value parsedFromString;
			Json::Reader reader;
			bool parsingSuccessful;

			Json::FastWriter fastWriter;
			string globalZmqStream;


			//printf("value= %s\n", contentsS.c_str());
			
			if (contentsS.size() > 2) {
				parsingSuccessful=reader.parse(contentsS.c_str(), parsedFromString);
				if (parsingSuccessful) {
					//Json::StyledWriter styledWriter;
					cout << "STYLED: --------------------------------" << endl;
					//cout << styledWriter.write(parsedFromString) << endl;
					//cout << "----------------------------------" << endl;
					cout << parsedFromString["stream"] << endl;

					cout << "----------------------------------" << endl;
					globalZmqStream = fastWriter.write(parsedFromString["stream"]).c_str();
					cout << globalZmqStream << endl;

					if ( parsedFromString["stream"].isNull() ) {
						globalZmqStream.clear();
					}
				}
				else {
					cout << "PARSING IN CURL CALLBACK FUNCTION WAS UNSUCCESSFUL !!!" << endl;
					cout << contentsS.c_str() << endl;
					cout << reader.getFormattedErrorMessages() << endl;

				}
			}	
						
			if (globalZmqStream.empty()) {
				 cout << "BS Data is not available " << endl;
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
			//cout << " globalZmqStream.c_str() " << globalZmqStream.c_str() << endl;

			rc = zmq_connect (subscriber, (const char *) globalZmqStream.c_str());   //"tcp://sf-daqbuf-30.psi.ch:39927");
				
			if (rc != 0 ) {
				cout << " Error is " << zmq_errno() << " " << zmq_strerror(zmq_errno()) << endl;
			}
				
			//rc = zmq_connect (subscriber, "tcp://*:9999");
			assert (rc == 0);
			
			int nhwm=1;
			int timeoutMS=200; //10; //-1 Wait for Ever
				
			rc=zmq_setsockopt (subscriber,ZMQ_RCVHWM, &nhwm, sizeof(int));
			assert (rc == 0);
				
			rc=zmq_setsockopt (subscriber,ZMQ_RCVTIMEO, &timeoutMS, sizeof(int));
			assert (rc == 0);
						
			rc=zmq_setsockopt (subscriber,ZMQ_SUBSCRIBE,"",0);
			assert (rc == 0);
							
			BSInitialized=true;

		}//is BS initialized

		#endif //have zeromq

		if(MUTEX){cafeMutex.unlock();}
		return isBS=BSFlag;
		#else //have curl
		
		if(MUTEX){cafeMutex.unlock();}
		return isBS=false;
		#endif //have curl
	}//isBSFlag
		 
	if(MUTEX){cafeMutex.unlock();}
	return isBS=BSFlag;
	} // setBS
	

	bool setCA(bool CAFlag) {	
		return CAFlag;
	}
	
	void closeBS() {
		if (BSInitialized && isBS) {
		  #if HAVE_ZEROMQ
			zmq_close (subscriber);	
    	zmq_ctx_destroy (context);
			#endif
		}
		BSInitialized=false;
		isBS=false;
	}
	
	bool getIsBS() { return isBS;}
		
	
	DBPMKeeper() {};
		
	DBPMKeeper(std::vector<string> _pv, std::vector<unsigned int> _handle, std::map<float, std::string> posDev):isBS(false),BSInitialized(false)
	{
		
		pv.assign    (_pv.begin(),    _pv.end());
		handle.assign(_handle.begin(),_handle.end());
		
	
		//fMap posDev;
		std::map<float, std::string>::iterator pos;
		for (pos =posDev.begin(); pos != posDev.end(); ++pos) {
			s.push_back(pos->first);  device.push_back(pos->second);					
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
	
		
	DBPMKeeper(std::vector<string> _pv, std::vector<unsigned int> _handle, std::vector<std::string> _dev,  std::vector<float> _pos):isBS(false),BSInitialized(false)
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




#endif //CAFE_SERVICE_H
