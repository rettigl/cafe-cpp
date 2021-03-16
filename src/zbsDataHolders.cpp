///
/// \file    zbsDataHolders.cpp
/// \author  Jan Chrin, PSI
/// \date    Release: December 2017
/// \version CAFE 1.5.1
///

#include <zbsDataHolders.h>

using namespace std;

string contentsS="";
string contentsBS="";
unsigned long nCBs=0;
bool callbackLiveFlag=false;

namespace CAFEBS
{

BSChannel BSDataHolder::getBSChannel(unsigned int idx)
{
    if(BSDataHolder::isIndexOutOfRange(idx))
    {
        std::ostringstream oss;
        oss << "Exception! Index " << idx
            << " to getBSChannel method is out of range. Valid range is from 0 to " << nPV-1;
        throw std::out_of_range(oss.str());
    }
    return bsChannel[idx];
};


BSChannel BSDataHolder::getBSChannel(std::string _name)
{
    for (unsigned int i=0; i<bsChannel.size(); ++i)
    {
        if (_name == bsChannel[i].getName())
        {
            return bsChannel[i];
        }
    }
    std::ostringstream oss;
    oss << "Exception! PV " << _name
        << " to getBSChannel method is not recognized. Invalid index to bsChannel " ;
    throw std::out_of_range(oss.str());
};


void BSDataHolder::setBSChannel(unsigned int idx, BSChannel bsc)
{
    bsChannel[idx]=bsc;
    return;
}


void BSDataHolder::setBSChannel(BSChannel bsc)
{
    int idx= BSDataHolder::getIdxFromName(bsc.getName());
    if (idx >-1)
    {
        bsChannel[idx]=bsc;
    }
    else
    {
        std::ostringstream oss;
        oss << "Exception! PV " << bsc.getName()
            << " to setBSChannel method is not recognized. Invalid index (-1) to bsChannel " ;
        throw std::out_of_range(oss.str());
    }
    return;
}


int BSDataHolder::getIdxFromName(std::string _name)
{
    for (unsigned int i=0; i<bsChannel.size(); ++i)
    {
        if (_name == bsChannel[i].getName())
        {
            return (int) i;
        }
    }
    return -1;
}

void BSDataHolder::verifyIndex(unsigned int idx)
{
    if(idx >= nPV)
    {
        std::ostringstream oss;
        oss << "Exception! Index " << idx
            << " to BSDataHolder method is out of range. Valid range is from 0 to " << nPV-1;
        throw std::out_of_range(oss.str());
    }
};



int BSDataHolder::reconnect()
{
#if HAVE_ZEROMQ
    if (BSInitialized && isBS)
    {
        zmq_close (subscriber);
        zmq_ctx_destroy (context);
        //cout << "Reconnecting to ZMQ stream " << endl;
        context = zmq_ctx_new ();
        subscriber = zmq_socket (context, ZMQ_SUB);
        rc = zmq_connect (subscriber, (const char *) globalBSZmqStream.c_str());
        if (rc != 0 )
        {
            cout << " (BSDataHolder::reconnect() Error is " << zmq_errno() << " " << zmq_strerror(zmq_errno()) << endl;
            return ICAFE_ERRNO_BASE+zmq_errno();
        }


        //Failed rc==0. This is a chnage in the API weher crypto_box returns -1 on a null server key, wheres it used
        //to ignore this error (as it ignores other invalid server keys).
        //Hintjens added: can't be sure crypto_box always returns 0
        //Solution is not to assert on this error, handle it gracefully

        //assert (rc == 0);
        rc=zmq_setsockopt (subscriber,ZMQ_RCVHWM, &nhwm, sizeof(int));
        rc=zmq_setsockopt (subscriber,ZMQ_SNDHWM, &nhwm, sizeof(int));
        //assert (rc == 0);

        rc=zmq_setsockopt (subscriber,ZMQ_RCVTIMEO, &timeoutMS, sizeof(int));
        //assert (rc == 0);

        rc=zmq_setsockopt (subscriber,ZMQ_SUBSCRIBE,"",0);
        //assert (rc == 0);

        //not required
        BSInitialized=true;
        resourceConnected=false; // Try false so that bsread can have a couple of attempts to bypass resources not available error
        //cout << "New zmq context and subscriber created " << endl;
    }
#endif
    return ICAFE_NORMAL;
}




bool BSDataHolder::setBS(bool BSFlag)
{

    if(MUTEX)
    {
        cafeMutex.lock();
    }
    if (BSFlag)
    {
#if HAVE_CURL
        string dataChannels=string("{\"channels\":[");
        vector<string> pvNew=pv;
        std::cout << " BSInitialized " << BSInitialized << std::endl;
#if HAVE_ZEROMQ
        if (!BSInitialized)
        {
            globalBSZmqStream.clear();
            // Curl stuff
            CURL *curl;
            CURLcode res;
            struct curl_slist * slist;
            slist = NULL;
            slist = curl_slist_append(slist, "Content-Type: application/json");
            curl_global_init(CURL_GLOBAL_ALL);
            curl = curl_easy_init();
            // Curl stuff - end

            /*
            curl -H "Content-Type: application/json" -X POST -d '{"name": "SINEG01-RCIR-PUP10:SIG-AMPLT", "backend":"sf-databuffer"}' https://dispatcher-api.psi.ch/sf/channel/live
            */
            if (curl)
            {

                curl_easy_setopt(curl, CURLOPT_URL, "https://dispatcher-api.psi.ch/sf/channel/live");
                //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"name\": \"SINEG01-RCIR-PUP10:SIG-AMPLT\", \"backend\":\"sf-databuffer\"}");
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
                //cout << "WAITING FOR CALLBACK... " << endl;
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RecvResponseCallbackLive);
                //res = curl_easy_perform(curl);
                //if (res != CURLE_OK) {
                //	cout << "curl_easy_perform failed " << curl_easy_strerror(res) << endl;
                //}
                //else {
                //cout << " CALLBACK DONE" << endl;
                //}
            }//if curl

            stringstream ss;
            size_t found;                  
            string liveString;
            std::cout << " bsChannel.size() = " <<  bsChannel.size() << std::endl;
            //reset counter
            nBSEnabled=0;

            for (size_t i=0; i < bsChannel.size(); ++i)
            {
                std::cout << " i = " << i << std::endl;
                std::cout << bsChannel[i].getName() << std::endl;
                liveString=string("{\"name\": \"");
                liveString=liveString + string(bsChannel[i].getName());
                liveString=liveString + string("\", \"backend\":\"sf-databuffer\"}" );

                //Verify Channel
                callbackLiveFlag=false;
                //std::cout << "liveString " << std::endl;
                //std::cout << liveString.c_str() << std::endl;
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS,(const char *) liveString.c_str());

                res = curl_easy_perform(curl);

                if (res != CURLE_OK)
                {
                    cout << "curl_easy_perform failed " << curl_easy_strerror(res) << endl;
                }
                else
                {
                    cout << " CALLBACK DONE..." << endl;
                    //curl_easy_cleanup(curl);
                    //curl_slist_free_all(slist);
                    //cout << " Value of callback Live is " << callbackLiveFlag << endl;
                    //slist=NULL;
                    if (!callbackLiveFlag)
                    {
                        cout << "Channel " << bsChannel[i].getName() << " is not available from BSREAD !!" << endl;
                        bsChannel[i].setBSEnabled(false);
                        continue;
                    }
                    else
                    {
                        cout << "Channel " << bsChannel[i].getName() << " IS available from BSREAD " << endl;
                        ++nBSEnabled;
                    }
                }

                /*
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
                */


                /////found = (bsChannel[i].getName()).find("ENERGY");
                ////if (found != std::string::npos) continue;

                //No joining comma for first entry
                if (nBSEnabled==1)
                {
                    dataChannels= dataChannels + string("{\"name\":\"");
                }
                else
                {
                    dataChannels= dataChannels + string(",{\"name\":\"");
                }
                dataChannels= dataChannels + string(bsChannel[i].getName());
                dataChannels= dataChannels + string("\",\"backend\":\"sf-databuffer\",\"modulo\":");
                ss.clear();
                ss.str(""); //clear stream;
                ss << bsChannel[i].getModulo();
                dataChannels= dataChannels + ss.str();
                dataChannels= dataChannels + string(",\"offset\":");
                ss.clear();
                ss.str(""); //clear stream;
                ss << bsChannel[i].getOffset();
                dataChannels= dataChannels + ss.str();
                dataChannels= dataChannels + string("}");
            }

            dataChannels= dataChannels + string("],");
            dataChannels= dataChannels + "\"mapping\":{\"incomplete\":\"fill-null\"},\"channelValidation\":{\"inconsistency\":\"keep-as-is\"},\"sendBehaviour\":{\"strategy\":\"complete-all\"}}";

            cout <<  dataChannels << endl;

            const char * data = dataChannels.c_str();

            if (curl && (nBSEnabled>0))
            {
                curl_easy_cleanup(curl);
                curl_slist_free_all(slist);
                slist=NULL;
                //Reinit
                slist = curl_slist_append(slist, "Content-Type: application/json");
                curl_global_init(CURL_GLOBAL_ALL);
                curl = curl_easy_init();
                curl_easy_setopt(curl, CURLOPT_URL, "https://dispatcher-api.psi.ch/sf/stream");
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data); //"-F file=@./dbpm.json"); //data); //
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

                cout << "WAITING FOR CALLBACK.... " << endl;

                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RecvResponseCallback);

                cout << "curl_easy_perform(curl)" << endl;
                res = curl_easy_perform(curl);

                if (res != CURLE_OK)
                {
                    cout << "curl_easy_perform failed " << curl_easy_strerror(res) << endl;
                }
                else
                {
                    cout << " CALLBACK DONE!!" << endl;

                    curl_easy_cleanup(curl);

                    curl_slist_free_all(slist);

                    slist=NULL;
                }
            }//if curl
            else if (curl)
            {
                curl_easy_cleanup(curl);
                curl_slist_free_all(slist);
                slist=NULL;

            }

            curl_global_cleanup();

            //cout << " //1// SHOW contentS " << endl;
            //cout << contentsBS.c_str() << endl;

            Json::Value parsedFromString;
            Json::Reader reader;
            bool parsingSuccessful;

            Json::FastWriter fastWriter;

            printf("value= %s\n", contentsBS.c_str());

            if (contentsBS.size() > 3)
            {
                parsingSuccessful=reader.parse(contentsBS.c_str(), parsedFromString);
                if (parsingSuccessful)
                {
                    Json::StyledWriter styledWriter;
                    cout << "STYLED: --------------------------------" << endl;
                    //cout << styledWriter.write(parsedFromString) << endl;
                    cout << "----------------------------------" << endl;
                    cout << parsedFromString["stream"] << endl;

                    cout << "----------------------------------" << endl;
                    globalBSZmqStream = fastWriter.write(parsedFromString["stream"]).c_str();
                    cout << "ZMQ STREAM c_str():" << globalBSZmqStream << endl;



                    if ( parsedFromString["stream"].isNull() )
                    {
                        globalBSZmqStream.clear();
                    }
                }
                else
                {
                    cout << "PARSING IN CURL CALLBACK FUNCTION WAS UNSUCCESSFUL !!!" << endl;
                    cout << contentsBS.c_str() << endl;
                    cout << reader.getFormattedErrorMessages() << endl;

                }
            }

            if (globalBSZmqStream.empty())
            {
                cout << "BS Data is not available - EMPTY  globalBSZmqStream" << endl;
                if(MUTEX)
                {
                    cafeMutex.unlock();
                }
                BSInitialized=false;
                contentsBS="";
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
            cout << " globalBSZmqStream.c_str() 1 to end-3: " << globalBSZmqStream.c_str() << endl;

            rc = zmq_connect (subscriber, (const char *) globalBSZmqStream.c_str());

            if (rc != 0 )
            {
                cout << " Error is " << zmq_errno() << " " << zmq_strerror(zmq_errno()) << " " <<   ICAFE_ERRNO_BASE+zmq_errno() <<endl;
                //return ICAFE_ERRNO_BASE+zmq_errno();
            }


            //Failed rc==0. This is a change in the API where crypto_box returns -1 on a null server key, whereas it used
            //to ignore this error (as it ignores other invalid server keys).
            //Hintjens added: can't be sure crypto_box always returns 0
            //Solution is not to assert on this error, handle it gracefully

            //assert (rc == 0);

            //int nhwm=1;
            //int timeoutMS=1000; //10; //-1 Wait for Ever

            rc=zmq_setsockopt (subscriber,ZMQ_RCVHWM, &nhwm, sizeof(int));
            ////assert (rc == 0);
            //For the subscriber we do not send
            rc=zmq_setsockopt (subscriber,ZMQ_SNDHWM, &nhwm, sizeof(int));

            rc=zmq_setsockopt (subscriber,ZMQ_RCVTIMEO, &timeoutMS, sizeof(int));
            ////assert (rc == 0);

            rc=zmq_setsockopt (subscriber,ZMQ_SUBSCRIBE,"",0);
            ////assert (rc == 0);

            BSInitialized=true;

        }//is BS initialized

#endif //have zeromq

        if(MUTEX)
        {
            cafeMutex.unlock();
        }
        contentsBS="";
        return isBS=BSFlag;
#else //have curl

        if(MUTEX)
        {
            cafeMutex.unlock();
        }
        return isBS=false;
#endif //have curl
    }//isBSFlag
    else
    {
        //Reset these as they are not applicabel for non bsread
        global_timestamp.secPastEpoch=0;
        global_timestamp.nsec=0;
        pulse_id=0;
    }

    if(MUTEX)
    {
        cafeMutex.unlock();
    }
    return isBS=BSFlag;
} // setBS


void BSDataHolder::printHeader()
{
    cout << "----------------------------------" << endl;
    cout << "htype     = " << htype << endl;
    cout << "pulse_id  = " << pulse_id << endl;
    cout << "global_ts = " << global_timestamp.secPastEpoch << " sec. " << global_timestamp.nsec << " nsec" << endl;
    cout << "hash      = " << hash << endl;
    cout << "dh_compression = " << dh_compression << endl;
    cout << "----------------------------------" << endl;
    //for (size_t i=0; i<bsChannel.size(); ++i) {
    //cout << bsChannel[i].getName() << " Val= " << getPVData(i).getAsString()
    //	   << " TS = " << getPVData(i).getEpicsTimeStamp().secPastEpoch
    //	   << " sec. " << getPVData(i).getEpicsTimeStamp().nsec << " nsec." << endl;
    //}
}

} //namespace
