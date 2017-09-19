///
/// \file    cafeService.h
/// \author  Jan Chrin, PSI
/// \date    Release July: 2017
/// \version CAFE 1.3.0
///

#ifndef CAFE_SERVICE_H
#define CAFE_SERVICE_H

#include <config.h>
#include <vector>
#include <PVDataHolder.h>
#include <statusCodes.h>
#include <global.h>


#if HAVE_ZEROMQ
//include <zhelpers.h>
	#include <zmq.h>
  #if HAVE_JSON
		#include <json/json.h>
	#endif
	#if HAVE_CURL
		#include <curl/curl.h>		
	#endif
#endif

/*
class BSData{
friend class CAFE;
private:
	double val;
	epicsTimeStamp ets;
	int status;
public:
	double getValue() {return val;}
	epicsTimeStamp getEpicsTimeStamp() {return ets;}
	int getStatus() { return status;}	
	
	BSData(){};
};


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
	bool parsingSuccessful;
	#endif
	
	
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

	//struct MemoryStruct {
 	// char *memory;
  //	size_t size;
	//};
	
	static size_t RecvResponseCallback(char * contents, size_t size, size_t nmemb, void  * up) {

		++nCBs;
		cout << "Callback called: " << nCBs << endl;

		///Json::Value parsedFromString;
		///Json::Reader reader;
		///bool parsingSuccessful;
	
		///Json::FastWriter fastWriter;
		
		cout << "SIZE No. of Bytes " << size*nmemb << endl;

		string sLocal=contents;

		std::size_t found = sLocal.find('\n');

		if (found != std::string::npos) {

			sLocal=sLocal.substr(0, found);
		}

		contentsS=contentsS+sLocal;



		//if (nCBs%3==1) {
		//	return size*nmemb;
		//}


		//printf("value= %s\n", contents);
		/*
		if (contents != NULL) {
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
				
					cout << reader.getFormattedErrorMessages() << endl;
				}
		}	
		contentsS="";
		*/
		
	/*
	size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)data;
 
  mem->memory = (char *) realloc(mem->memory, (mem->size + realsize + 1));
	
  if(mem->memory == NULL) {
   
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  //return realsize;
	
			printf("value= %s\n",mem->memory);
		if (mem->memory != NULL) {
				parsingSuccessful=reader.parse(mem->memory, parsedFromString);
				if (parsingSuccessful) {
					Json::StyledWriter styledWriter;
					cout << "STYLED: --------------------------------" << endl;
					cout << styledWriter.write(parsedFromString) << endl;
					cout << "----------------------------------" << endl;
					cout << parsedFromString["stream"] << endl;
					
					cout << "----------------------------------" << endl;
				
		
				}
		}
	*/	
		
		return (size_t) size * nmemb;
	}

	
	
//	


  bool resetBS() {
		closeBS();
		return setBS(true);	
	}


	bool setBS(bool BSFlag) {

   if(MUTEX){cafeMutex.lock();}


		if (BSFlag) {		
			#if HAVE_CURL
			/*
			//Complete all or complete latest	
			//const char * data="{\"channels\":[{\"name\":\"S10BC01-DBPM010:X1\",\"backend\":\"sf-databuffer\",\"modulo\":1,\"offset\":0}, \
			{\"name\":\"S10BC01-DBPM010:X1-VALID\",\"backend\":\"sf-databuffer\",\"modulo\":1,\"offset\":0} \
		],\"mapping\":{\"incomplete\":\"fill-null\"},\"channelValidation\":{\"inconsistency\":\"adjust-global\"},\"sendBehaviour\":{\"strategy\":\"complete-all\"}}";
*/
			string dataChannels=string("{\"channels\":[");
							
			vector<string> pvNew=pv;
#if HAVE_ZEROMQ
if (!BSInitialized) {
		//pvNew.push_back("SINEG01-DBPM340:X1") ;
		//pvNew.push_back("SINSB01-DBPM150:X1") ;
		//pvNew.push_back("SINSB02-DBPM150:X1") ;
		//pvNew.push_back("SINLH01-DBPM060:X1") ;
		//pvNew.push_back("SINLH02-DBPM210:X1") ; 
		//pvNew.push_back("SINLH02-DBPM240:X1") ; 
		//pvNew.push_back("SINLH03-DBPM010:X1") ; 
		//pvNew.push_back("SINLH03-DBPM050:X1") ; 
		//pvNew.push_back("SINLH03-DBPM090:X1") ; 
		//pvNew.push_back("SINSB03-DBPM120:X1") ; 
		//pvNew.push_back("SINSB03-DBPM220:X1") ; 
		//pvNew.push_back("SINSB04-DBPM120:X1") ; 
		//pvNew.push_back("SINSB04-DBPM220:X1") ; 
					
		//pvNew.push_back("SINEG01-DBPM340:X1-VALID") ;
		//pvNew.push_back("SINSB01-DBPM150:X1-VALID") ;
		//pvNew.push_back("SINSB02-DBPM150:X1-VALID") ;
		//pvNew.push_back("SINLH01-DBPM060:X1-VALID") ;
		//pvNew.push_back("SINLH02-DBPM210:X1-VALID") ; 
		//pvNew.push_back("SINLH02-DBPM240:X1-VALID") ; 
		//pvNew.push_back("SINLH03-DBPM010:X1-VALID") ; 
		//pvNew.push_back("SINLH03-DBPM050:X1-VALID") ; 
		//pvNew.push_back("SINLH03-DBPM090:X1-VALID") ; 
		//pvNew.push_back("SINSB03-DBPM120:X1-VALID") ; 
		//pvNew.push_back("SINSB03-DBPM220:X1-VALID") ; 
		//pvNew.push_back("SINSB04-DBPM120:X1-VALID") ; 
		//pvNew.push_back("SINSB04-DBPM220:X1-VALID") ; 
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
				//found = pvNew[i].find("Y1");
				//if (found != std::string::npos) continue;
				//found = pvNew[i].find("X1");
				//if (found != std::string::npos) continue;
				found = pvNew[i].find("ENERGY");
				if (found != std::string::npos) continue;
				
			
			   dataChannels= dataChannels + string(",{\"name\":\"");
				dataChannels= dataChannels + pvNew[i];
				
				//found = pv[i+1].find("ENERGY");
				//if (found != std::string::npos) break;
				//dataChannels= dataChannels + string("\",\"backend\":\"sf-databuffer\"}");
				dataChannels= dataChannels + string("\",\"backend\":\"sf-databuffer\",\"modulo\":1,\"offset\":0}");
					
			}
 				//dataChannels= dataChannels + string("{\"name\":\"");
				//dataChannels= dataChannels + pv[pv.size()-1];
				//dataChannels= dataChannels + string("\",\"backend\":\"sf-databuffer\"}],");
				dataChannels= dataChannels + string("],");
				dataChannels= dataChannels + "\"mapping\":{\"incomplete\":\"fill-null\"},\"channelValidation\":{\"inconsistency\":\"keep-as-is\"},\"sendBehaviour\":{\"strategy\":\"complete-all\"}}";

			cout <<  dataChannels << endl;
	
			//sleep(1);
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
			
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data); //"-F file=@./request.json"); //data); //
			
				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
				curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
				
				cout << "WAITING FOR CALLBACK " << endl;

				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RecvResponseCallback);
				


				res = curl_easy_perform(curl);
				
				if (res != CURLE_OK) {
					cout << "curl_easy_perform failed " << curl_easy_strerror(res) << endl;
				}
				else {
				cout << " CALLBACK DONE" << endl;

				cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

				curl_easy_cleanup(curl);

				cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
				curl=NULL;

				curl_slist_free_all(slist);
				cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
				slist=NULL;
				}
			}
			
			cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
			curl_global_cleanup();
			
			#endif
		
			//cout << " //1// SHOW contentS " << endl;
			//cout << contentsS.c_str() << endl;

			
			//sleep(1);

			Json::Value parsedFromString;
			Json::Reader reader;
			bool parsingSuccessful;

			Json::FastWriter fastWriter;

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



//
//#if HAVE_ZEROMQ
//if (!BSInitialized) {


			   cout << "TESTING STREAM... " << endl;
			
				
				 if (globalZmqStream.empty()) {
				 		cout << "BS Data is not available " << endl;
				   return isBS=false;
				 }
				// else {
				//   cout << globalZmqStream.c_str() << " is not empty " << endl;
				 
				// }
			
			
				context = zmq_ctx_new ();
			
		///  	receiver = zmq_socket (context, ZMQ_PULL);
				//HWM has no effect for PULL
				//See documentation on zmq-socket
				//WHEN PUSH Sender reachers HWM, then it blocks
		///		int nhwm=10;
		///		zmq_setsockopt (receiver,ZMQ_RCVHWM ,&nhwm, sizeof(int));
				//rc = zmq_bind (receiver, "tcp://129.129.145.206:5558"); //ZMQ_PULL
		///		rc = zmq_bind (receiver, "tcp://SIN-CVME-DBPM0421:9000");
		///		assert (rc == 0);
			
				
				subscriber = zmq_socket (context, ZMQ_SUB);
				//rc = zmq_connect (subscriber, "tcp://129.129.145.206:5556");
				//rc = zmq_connect (subscriber, "tcp://SIN-CVME-DBPM0421:9000");
		
				
				
				globalZmqStream=globalZmqStream.substr(1,globalZmqStream.size()-3);
				//cout << " globalZmqStream.c_str() " << globalZmqStream.c_str() << endl;
				//sleep(1);
				
				
				
				rc = zmq_connect (subscriber, (const char *) globalZmqStream.c_str()); // "tcp://sf-daqbuf-28.psi.ch:42465"); //  //"tcp://sf-daqbuf-30.psi.ch:39927");
				
				if (rc != 0 ) {
					cout << " Error is " << zmq_errno() << " " << zmq_strerror(zmq_errno()) << endl;
				}
				
				//rc = zmq_connect (subscriber, "tcp://*:9999");
				assert (rc == 0);	
			
				int nhwm=1;
				int timeoutMS=400; //10; //-1 Wait for Ever
				
				rc=zmq_setsockopt (subscriber,ZMQ_RCVHWM ,&nhwm, sizeof(int));
				assert (rc == 0);	
				
				rc=zmq_setsockopt (subscriber,ZMQ_RCVTIMEO ,&timeoutMS, sizeof(int));
				assert (rc == 0);		
						
				rc=zmq_setsockopt (subscriber,ZMQ_SUBSCRIBE,"",0);				
				assert (rc == 0);
							
				BSInitialized=true;								
		}
		
		if(MUTEX){cafeMutex.unlock();}
		return isBS=BSFlag;
		#else
		
		if(MUTEX){cafeMutex.unlock();}
		return isBS=false;
		#endif
		}	
		 
		if(MUTEX){cafeMutex.unlock();}
		return isBS=BSFlag;
	}	
	
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
