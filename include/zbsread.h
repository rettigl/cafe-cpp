
#ifndef ZBSREAD_H
#define ZBSREAD_H

#include <inttypes.h>

#include <zmq.h>
#include <assert.h>
#include <signal.h>

#if HAVE_JSON
#include <json/json.h>
#endif

#include <zbsDataHolders.h>
#include <zbsdtHelper.h>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace CAFE_BSHELPER;

//Preferred to BitsetV because of speed

IntegerBitset<unsigned char> bsetUChar;
IntegerBitset<char> bsetChar;
IntegerBitset<unsigned short> bsetUShort;
IntegerBitset<short> bsetShort;
IntegerBitset<unsigned int> bsetUInt;
IntegerBitset<int> bsetInt;
IntegerBitset<unsigned long long> bsetULongLong;
IntegerBitset<long long> bsetLongLong;

//IntegerBitsetV<unsigned char> bsetUCharV;
//IntegerBitsetV<char> bsetCharV;
//IntegerBitsetV<unsigned short> bsetUShortV;
//IntegerBitsetV<short> bsetShortV;
//IntegerBitsetV<unsigned int> bsetUIntV;
//IntegerBitsetV<int> bsetIntV;
//IntegerBitsetV<unsigned long long> bsetULongLongV;
//IntegerBitsetV<long long> bsetLongLongV;

FloatBitset<float>  bsetFloat;
FloatBitset<double> bsetDouble;
//FloatBitsetV<float>  bsetFloatV;
//FloatBitsetV<double> bsetDoubleV;


bool bsdtInsertFlag=false;
bool dataHeaderReceived=false;

unsigned short inDumpFlag=2;
std::string hashIs="";
std::string hashOriginal="";
unsigned short hashOriginalFlag=0;
bool fillBSPV=false;
std::vector<std::string> bsPV;

bool fill_bs_read_PV=false;
std::vector<std::string> bs_read_PV;



/**
 *  \brief Receives all message parts from zeromq socket
 *  \param dbpm in/output: DBPMKeeper object
 *  \return long ICAFE_NORMAL else ECAFE_error
 */
static void
z_bsread_dbpm (DBPMKeeper & dbpm)
{
    if (inDumpFlag==1)
    {
        std::cout << "zeroMQ socket is busy " << std::endl;
        std::cout << "waiting for zmq timeout " << std::endl;
    }

    //puts ("//START----------------------------------------//");

    void * socket = dbpm.subscriber;

#if HAVE_JSON
    Json::Value parsedFromString;
    Json::Reader reader;
    bool parsingSuccessful;
    Json::FastWriter fastWriter;
#endif

    int64_t more;           //  Multipart detection
    more = 0;
    size_t more_size = sizeof (more);

    int bsPVIdx=-1;
    dbpm.status=ICAFE_NORMAL;

    int nZeroSize=0;

    //std::cout << "df = " << inDumpFlag << std::endl;

    while (inDumpFlag==1)
    {
        std::cout << "df/ = " << inDumpFlag << std::endl;
        std::cout << " sleeping " << std::endl;
    }

    int subMessage=0;
    int nSequentialHeader=0;

    

    while (1)
    {
        inDumpFlag=1;

        //puts ("//WHILE LOOP ----------------------------------------//");
        //std::cout << "subMessage " << subMessage << std::endl;
        //  Process all parts of the message
        zmq_msg_t message;
        zmq_msg_init (&message);

        size_t size = zmq_msg_recv (&message, socket, 0);
        //puts ("//MESSAGE RECEIVED     ----------------------------------------//");

	

        if (size == -1)
        {
            std::cout << " Error is " << zmq_errno() << " " << zmq_strerror(zmq_errno()) << std::endl;
            //Resource unavailable means that there is nothing to read now

            zmq_getsockopt (socket, ZMQ_RCVMORE, &more, &more_size);
            //std::cout << "message mulipart --> more == " << more << std::endl;
            zmq_msg_close (&message);
            //if (!more) {
            //		   dbpm.status=ERRNO_EAGAIN; //ICAFE_ERRNO_BASE +zmq_errno() ;
            //		   std::cout << "EARLY BREAK subMessage total: " << subMessage << std::endl;
            //		   puts ("//------------------------------------END-------------------------------------//");
            //		 break;      //  Last message
            // }



            dbpm.status=ICAFE_ERRNO_BASE+zmq_errno() ;
            break;
        }


        else if (size == 0)
        {

            ////std::cout << " Data of Zero SIZE for submessage " << subMessage << std::endl;

            ++nZeroSize;


            //zmq_msg_close (&message);
            //dbpm.status=ECAFE_NODATA;
            //break;
            //Comes in pairs; one for val one for timestamp
            if (nZeroSize%2==1)
            {
                ++bsPVIdx;
                if (bsPVIdx >0)
                {
		  std::cout << " Data of Zero SIZE for submessage " << subMessage << std::endl;
                  std::cout << bsPV[bsPVIdx] << std::endl;
                }
            }

            ++subMessage;

            //continue;
        }
        else
        {

            //  Dump the message as text or binary
            char *data = (char*)zmq_msg_data (&message);
            int is_text = 1;
            int char_nbr;

            //char cmd[5000]="";

            for (char_nbr = 0; char_nbr < size; char_nbr++)
                if ((unsigned char) data [char_nbr] < 32
                        ||  (unsigned char) data [char_nbr] > 127)
                    is_text = 0;


            //TExt is two header files
            //non-text size=8 is X1, Y1, Q1
            //non-text size=2 is -VALID
            //non-text size=16 is timestamp

            /*
            printf (" SIZE [%03d] ", size);


            if (is_text) {
            	std::cout << " TEXT +++++++++++ " << std::endl;
            }
            else {
            	std::cout << " NOT TEXT +++++++++++ " << std::endl;
            }


            for (char_nbr = 0; char_nbr < size; char_nbr++) {
                if (is_text) {
                    printf ("%c", data [char_nbr]);
            						  //snprintf(cmd + strlen(cmd), (sizeof cmd) - strlen(cmd), "%c", data [char_nbr]);

            				}
                else {
                     printf ("%02X", (unsigned char) data [char_nbr]);
            						 //printf ("%d",   (unsigned char) data [char_nbr]);

            						//snprintf(cmd + strlen(cmd), (sizeof cmd) - strlen(cmd), "%d", (unsigned char)data [char_nbr]);

            						//if (data[0] == '\x7') {
            						//	std::cout << " little endian " << std::endl;
            						//}
            						//else {
            						//	std::cout << "big endian " << std::endl;
            						//}


            				}
            }//for
            printf ("\n");
            */


            if (is_text)
            {
                parsingSuccessful=reader.parse(data, parsedFromString);
                if (parsingSuccessful)
                {
                    //Json::StyledWriter styledWriter;
                    //std::cout << "STYLED: --------------------------------" << std::endl;
                    //std::cout << styledWriter.write(parsedFromString) << std::endl;
                    //std::cout << "----------------------------------" << std::endl;
                    //std::cout << parsedFromString["htype"] << std::endl;


                    if (fastWriter.write(parsedFromString["htype"]).find("bsr_m-1.1") != std::string::npos)
                    {

                        ++nSequentialHeader;

                        hashIs=parsedFromString["hash"].asString();

                        if (hashOriginalFlag==0)
                        {
                            std::cout << hashIs << " is different to original/// " << hashOriginal << std::endl;
                            hashOriginal=hashIs;
                            ++hashOriginalFlag;
                            fillBSPV=true;
                        }

                        if (hashOriginal.compare(hashIs)!=0)
                        {
                            std::cout << hashIs << " is different to original " << hashOriginal << std::endl;
                            hashOriginal=hashIs;
                            ++hashOriginalFlag;
                            fillBSPV=true;

                        }
                        //std::cout << "p id " << parsedFromString["pulse_id"].asUInt64() << std::endl;

                        dbpm.setPulse_id(parsedFromString["pulse_id"].asUInt64());

                        //Reset values as a change of hash signifies that data from two pulse ids
                        //is being sent in one zeromq messages

                        subMessage=0;

                        bsPVIdx=-1;
                        dbpm.status=ICAFE_NORMAL;

                        nZeroSize=0;


                        /*
                        std::cout << "(1)++++++++++++++++++++++++++++++++++++++++MAIN++++++++++++++++++++++++++++++++++++++++++"	<< std::endl;

                        	std::cout << "hash " << parsedFromString["hash"] << std::endl;
                        	std::cout << "p id " << parsedFromString["pulse_id"].asUInt64() << std::endl;
                        	std::cout << "g ts " << parsedFromString["global_timestamp"] << std::endl;
                        	std::cout << "comp " << parsedFromString["dh_compression"] << std::endl;

                        	std::cout << "sec " << parsedFromString["global_timestamp"]["sec"].asUInt() << std::endl;
                          std::cout << "nsec " << parsedFromString["global_timestamp"]["ns"].asUInt() << std::endl;
                        */

                    }
                    else if (fastWriter.write(parsedFromString["htype"]).find("bsr_d-1.1") != std::string::npos)
                    {

                        ++nSequentialHeader;

                        if  (fillBSPV)
                        {

                            bsPV.clear();
                            bsPV.reserve(dbpm.getNPV());
                            /*
                            if (dbpm.getNPV() != parsedFromString["channels"].size() ) {
                            	std::cout << "No of CONFIGURED BPMS: " << dbpm.getNPV()
                            	     << " is diffent to that being channeled " <<  parsedFromString["channels"].size() << std::endl;
                            	bsPV.reserve( std::max( (size_t) parsedFromString["channels"].size(),dbpm.getNPV()) );

                            }

                            */


                            //std::cout << "chan " << parsedFromString["channels"] << std::endl;

                            //std::cout << "No of channels " << parsedFromString["channels"].size() << std::endl;

                            //std::cout << "(2)++++++++++++++++++++++++++++++++++++++++HEADER++++++++++++++++++++++++++++++++++++++++++"	<< std::endl;

                            for (Json::Value::ArrayIndex i=0; i < parsedFromString["channels"].size(); ++ i)
                            {
			      /*
                                std::cout << "name " << parsedFromString["channels"][i]["name"].asString() << std::endl;
                                std::cout << "enco " << parsedFromString["channels"][i]["encoding"] << std::endl;
                                std::cout << "type " << parsedFromString["channels"][i]["type"] << std::endl;
			      */

                                bsPV.push_back( (parsedFromString["channels"][i]["name"]).asString());
                            }
                            /*
                            std::cout << "LIST OF PVS " << std::endl;
                            for (size_t i=0; i< bsPV.size(); ++i ) {
                            	std::cout << i << " // " <<   bsPV[i].c_str() << " " << std::endl;
                            }
                            std::cout << std::endl;
                            */

                            //std::cout << "NEW FILL: size of bsPV " <<  bsPV.size() << std::endl;
                            fillBSPV=false;
                        } //if fill

                    }

                    else if (fastWriter.write(parsedFromString["htype"]).find("bsr_reconnect-1.0") != std::string::npos)
                    {

                        std::cout << "RECONNECT CONTROL MESSAGE " << std::endl;
                        std::cout << "HAS NEW SOURCE ADDRESS: " <<  fastWriter.write(parsedFromString["address"])  << std::endl;

                    }
                    else if (fastWriter.write(parsedFromString["htype"]).find("bsr_stop-1.0") != std::string::npos)
                    {

                        std::cout << "STOP CONTROL MESSAGE " << std::endl;
                        std::cout << "RECEIVED: " <<  fastWriter.write(parsedFromString["htype"])  << std::endl;
                        std::cout << "From BS Documentation: Message can be ignored as source will send from same address after startup" << std::endl;

                    }

                    else
                    {

                        std::cout << "HEADER IS SOMETHING ELSE: " << std::endl;
                        std::cout << parsedFromString["htype"] << std::endl;

                        exit(1);

                    }


                    //std::cout << "/----------------------------------/" << std::endl;

                }
            }


            if (nSequentialHeader >3 && is_text==1)
            {
                std::cout << "WARNING: ZEROMQ SUB-MESSAGE DOES NOT CLOSE " << std::endl;
                std::cout << "WARNING: FORCING ZMQ_MSG_CLOSE  " << std::endl;

                std::cout << "No of sequential headers " << nSequentialHeader << std::endl;
                std::cout << "is_text " << is_text << std::endl;
                //zmq_getsockopt (socket, ZMQ_RCVMORE, &more, &more_size);

                //zmq_msg_close (&message);
                //std::cout << "message mulipart --> more == " << more << std::endl;


                //nSequentialHeader=0;
                //cannot break; results in seqmentation fault

                //if (!more) {
                //		   dbpm.status=ERRNO_EAGAIN; //ICAFE_ERRNO_BASE +zmq_errno() ;
                //		   std::cout << "EARLY BREAK subMessage total: " << subMessage << std::endl;
                //		   puts ("//------------------------------------END-------------------------------------//");

                // break;      //  Last message
                //}

            }



            union foo
            {
                char c[sizeof(double)];
                double d;

            } bar;

            //SIZE 16 is timestamp
            //SIZE 8  is x,y,Q
            //SIZE 2  is valid

            if (subMessage > 1430)
            {
                std::cout << " SIZE " << size << " subMessage " << subMessage << std::endl;
                std::cout << "size of bsPV " <<  bsPV.size() << std::endl;
            }

            if (subMessage > 0 && subMessage%2 ==0)
            {
                if (size==8)
                {

                    // big endian
                    for (char_nbr = 0; char_nbr < size; char_nbr++)
                    {
                        bar.c[char_nbr]=data[size-1-char_nbr];  // THis works for big engian
                    }
                    // little endian
                    //for (char_nbr = 0; char_nbr < size; char_nbr++) {
                    // bar.c[char_nbr]=data[char_nbr];
                    //}


                    //std::cout << "UNION D " << bar.d << std::endl;


                    double v; // = (double*) data;
                    memcpy(&v, bar.c, sizeof(double));
                    //std::cout << " double val " << v << std::endl;

                    //This is BPM Data - value
                    ++bsPVIdx;

                    if (dbpm.getPVIdx(bsPV[bsPVIdx]) <0)
                    {
                        std::cout << " WARNING--> THIS CHANNEL WAS NOT REQUESTED IN CONFIGURATION FILE " << std::endl;
                        std::cout << " bsPV index = " <<  bsPVIdx << std::endl;
                        std::cout << " pv from bs = " << bsPV[bsPVIdx] << std::endl;
                        std::cout << " Illegal index Value =" << dbpm.getPVIdx(bsPV[bsPVIdx]) << std::endl;
                        std::cout << " SKIPPING THIS BPM... " << std::endl;
                        continue;
                    }


                    dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].set(v);

                    dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].setStatus(ICAFE_NORMAL);

                    //std::cout << " readback of value that was set = " << dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].val[0].d << std::endl;

                }
                else if (size==2)
                {
                    unsigned short iv = 0;
                    //memcpy(&iv, cmd, size);
                    //big endian
                    for (size_t n=0; n < size; n++)
                    {
                        iv = (iv << 8) + data[n];
                    }
                    //little endian
                    //for (size_t n = size; n >= 0; n--) {
                    //	iv = (iv << 8) + data[n];
                    //}

                    //This is BPM Data	- VALID/INVALID

                    //std::cout << "uint val (1 means valid) " << iv << std::endl;

                    /*
                    std::copy(data, data + 32, reinterpret_cast<char *>(&i));
                    std::cout << "uint16  val " << i << std::endl;
                    std::copy(data, data + 8, reinterpret_cast<char *>(&i));
                    std::cout << "uint16  val " << i << std::endl;
                    std::copy(data, data + 4, reinterpret_cast<char *>(&i));
                    std::cout << "uint16  val " << i << std::endl;
                    std::copy(data, data + 2, reinterpret_cast<char *>(&i));
                    std::cout << "uint16  val " << i << std::endl;
                    std::copy(data, data + 1, reinterpret_cast<char *>(&i));
                    std::cout << "uint16  val " << i << std::endl;

                    */
                    ++bsPVIdx;


                    if (dbpm.getPVIdx(bsPV[bsPVIdx]) <0)
                    {
                        std::cout << " WARNING--> THIS CHANNEL WAS NOT REQUESTED IN CONFIGURATION FILE " << std::endl;
                        std::cout << " bsPV index = " <<  bsPVIdx << std::endl;
                        std::cout << " pv from bs = " << bsPV[bsPVIdx] << std::endl;
                        std::cout << " Illegal index Value =" << dbpm.getPVIdx(bsPV[bsPVIdx]) << std::endl;
                        std::cout << " SKIPPING THIS BPM ENUM TYPE " << std::endl;
                        continue;
                    }




                    if (iv==1)
                    {
                        dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].set((std::string) "VALID");

                    }
                    else
                    {
                        dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].set((std::string) "INVALID");
                    }

                    //std::cout << "value DBPM = " << dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].val[0].str << std::endl;


                    dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].setStatus(ICAFE_NORMAL);
                }


            }
            else if (subMessage > 1 && subMessage%2 ==1)
            {
                //std::cout << "timestamp " << std::endl;
                unsigned int a=0,b=0;
                for (size_t n=0; n < size/2; n++)
                {
                    a = (a << 8) + data[n];
                }
                for (size_t n=size/2; n < size; n++)
                {
                    b = (b << 8) + data[n];
                }

                //std::cout << "a " << a << " b " << b << std::endl;


                if (dbpm.getPVIdx(bsPV[bsPVIdx]) <0)
                {
                    std::cout << " WARNING--> THIS CHANNEL WAS NOT REQUESTED IN CONFIGURATION FILE " << std::endl;
                    std::cout << " bsPV index = " <<  bsPVIdx << std::endl;
                    std::cout << " pv from bs = " << bsPV[bsPVIdx] << std::endl;
                    std::cout << " Illegal index Value =" << dbpm.getPVIdx(bsPV[bsPVIdx]) << std::endl;
                    std::cout << " SKIPPING THIS BPM TIMESTAMP " << std::endl;
                    continue;
                }



                dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].ts.secPastEpoch=a;
                dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].ts.nsec=b;
            }



            //std::cout << "subMessage above: " << subMessage << std::endl;

            ++subMessage;


        } //ifelse


        zmq_getsockopt (socket, ZMQ_RCVMORE, &more, &more_size);

        zmq_msg_close (&message);

        //std::cout << "value of more " << more << std::endl;

        if (!more)
        {
            dbpm.status=ICAFE_NORMAL;
            // std::cout << "subMessage total: " << subMessage << std::endl;
            // std::cout << " of which 0 size: " << nZeroSize << std::endl;
            // std::cout << " Percentage good: " << (subMessage-nZeroSize-2)*100/(subMessage-2);
            // puts ("//------------------------------------END-------------------------------------//");

            break;      //  Last message part
        }
    } //while 1

    inDumpFlag=0;

    if (subMessage>2)
    {
        dbpm.status=ICAFE_NORMAL;
        //std::cout << "subMessage total: " << subMessage << std::endl;
        //std::cout << " of which 0 size: " << nZeroSize << std::endl;
        //std::cout << " Percentage good: " << (subMessage-nZeroSize-2)*100/(subMessage-2);
        //puts ("//------------------------------------END-------------------------------------//");
    }


    //std::cout << "end of loop " << std::endl;

    return;
}


/**
 *  \brief Receives all message parts from zeromq socket
 *  \param bsd in/output: BSDataHolder object
 *  \return long ICAFE_NORMAL else ECAFE_error
 */
static void z_bsread (BSDataHolder &bsd)
{
#define __METHOD__  "z_bsread (BSDataHolder &bsd)"
    unsigned int localDebug=0;
    //We need to read dat a header every time since
    //so that header data is caught after a bs restart(!)
    dataHeaderReceived=false;
    bsd.overallStatus=ICAFE_NORMAL;

    //Data type index
    if (!bsdtInsertFlag)
    {
        CAFE_BSHELPER::bsdtInsert();
        bsdtInsertFlag=true;
    }
    bsreadContainer_set_by_name & name_index = CAFE_BSHELPER::bsdt.get<by_bsName> ();
    bsreadContainer_set_by_name::iterator it_name;
    unsigned int bsdtIndex;

    if(localDebug)puts ("//START----------------------------------------//\n");
    int subMessage=0;
    void * socket = bsd.subscriber;
    int64_t more =0;           //  Multipart detection
    size_t more_size = sizeof (more);
    //Keep a count of the number of MULTIpart messages with zero data
    int nZeroSize=0;

    int64_t ts, tnsec;

    //Matching PV Index to BSChannel in bsd
    int bsPVIdx=-1;

#if HAVE_JSON
    Json::Value parsedFromString;
    Json::Reader reader;
    Json::FastWriter fastWriter;
#endif
    bool parsingSuccessful;
    bool mainHeaderReceived=false;
    bool newHash=false;

    
    using namespace boost::posix_time;
    ptime timeStart(microsec_clock::local_time());

    double  timeElapsed=0;
    unsigned int npoll =0;


    //subMessage is incremeneted at end of multi-part message
    //Process all parts of the message
        
       
    //The Loop
    while (1)
    {
     
        zmq_msg_t message;
       	zmq_msg_init (&message);
        size_t size = zmq_msg_recv (&message, socket, 0);

        //Continue goes to top of while
        //Why not add submessage here
        

	/*
	 ++subMessage; 
        //std::cout << "SUBMESSAGE " << subMessage << std::endl;
        more=0;           //  Multipart detection
        zmq_getsockopt (socket, ZMQ_RCVMORE, &more, &more_size);
        //zmq_msg_close (&message);

        if (!more)
        {
	   zmq_msg_close (&message);
	   //if(localDebug)
	      puts ("//END----------------------------------------//");
            break;      //  Last message part
        }

	continue;	      
        */

        if(localDebug)printf ("[%03lu] \n", size);
        if (size == -1)
        {
            std::cout << "z_bsread.h: Error is " << zmq_errno() << " " << zmq_strerror(zmq_errno()) << std::endl;
            //Resource unavailable means that there is nothing to read now
            zmq_msg_close (&message);
            bsd.overallStatus=ICAFE_ERRNO_BASE+zmq_errno();
	    //std::cout << "z_bsread.h: Error is os " << bsd.overallStatus << std::endl;
            for (size_t i=0; i < bsd.getNPV(); ++i)
            {
                if(bsd.getBSChannel(i).isBSEnabled())
                {
                    bsd.pvd[i].set(0);
                    bsd.pvd[i].setStatus(bsd.overallStatus);
                    bsd.pvd[i].setAlarmStatus(-1);
                    bsd.pvd[i].setAlarmSeverity(-1);
                    bsd.pvd[i].ts.secPastEpoch=0;
                    bsd.pvd[i].ts.nsec        =0;
                    bsd.pvd[i].setPulseID(0);
                }
            }
	    
            break;
        }
        else if (size == 0)
        {
            if(localDebug)std::cout << " Data of Zero SIZE for submessage " << subMessage << std::endl;
            ++nZeroSize;
            //Avoid timestamp blob in count
            if (nZeroSize%2==1)
            {
                ++bsPVIdx;  //Increment when data
                bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set(0);
                bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].setStatus(ECAFE_BSREAD_MULTIPART_MESS_NODATA);
                bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].setPulseID(bsd.getPulse_id());
            }
            else   //Zero Timestamp
            {
                bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].ts.secPastEpoch=0;
                bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].ts.nsec        =0;
            }
            if (bsd.overallStatus == ICAFE_NORMAL)
            {
                bsd.overallStatus=ECAFE_BSREAD_MULTIPART_MESS_NODATA;
            }
        }
        else
        {

            //  Dump the message as text or binary
            if(localDebug)printf ("[%03lu] \n", size);

            char *data = (char*)zmq_msg_data (&message);

            //Check if it is text and is so if it is the main header or not

            //First message is always the Main Header

            if (!mainHeaderReceived)
            {
                //std::cout << "MAIN HEADER ================================================================================= " << std::endl;
                parsingSuccessful=reader.parse(data, parsedFromString);
                if (parsingSuccessful)
                {

                    if (localDebug==1)
                    {
                        Json::StyledWriter styledWriter;
                        std::cout << "STYLED: --------------------------------" << std::endl;
                        std::cout << styledWriter.write(parsedFromString) << std::endl;
                        std::cout << "----------------------------------" << std::endl;
                        std::cout << "HEADER TYPE" << std::endl;
                        std::cout << parsedFromString["htype"].asString() << std::endl;
                    }

                    if (fastWriter.write(parsedFromString["htype"]).find("bsr_m-1.1") != std::string::npos)
                    {

                        if (localDebug)
                        {
                            std::cout << "hash " << parsedFromString["hash"].asString() << std::endl;
                            std::cout << "p id " << parsedFromString["pulse_id"].asUInt64() << std::endl;
                            std::cout << "g ts " << parsedFromString["global_timestamp"] << std::endl;
                            std::cout << "comp " << parsedFromString["dh_compression"].asString() << std::endl;
                            std::cout << "sec  " << parsedFromString["global_timestamp"]["sec"].asUInt() << std::endl;
                            std::cout << "nsec " << parsedFromString["global_timestamp"]["ns"].asUInt() << std::endl;
                        }
                        //required
                        bsd.setHtype( parsedFromString["htype"].asString() );

                        if (bsd.getHash().compare(parsedFromString["hash"].asString()) !=0 )
                        {
                            bsd.setHash( parsedFromString["hash"].asString() );
                            newHash=true;
                            //std::cout << " NEW HASHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHh " << std::endl;
                        }
                        bsd.setPulse_id(parsedFromString["pulse_id"].asUInt64());
			//std::cout << " Pulse ID " << bsd.getPulse_id() << std::endl;
		
                        //optional
                        if (!(parsedFromString["global_timestamp"]).empty())
                        {
                            bsd.setGlobal_timestamp(parsedFromString["global_timestamp"]["sec"].asUInt(),
                                                    parsedFromString["global_timestamp"]["ns"].asUInt());
                        }
                        //Check for Data Compression here!!
                        if (!(parsedFromString["dh_compression"].asString()).empty())
                        {
                            bsd.setDH_compression(parsedFromString["dh_compression"].asString() );
			   
                        }
                    }
                    else if (fastWriter.write(parsedFromString["htype"]).find("bsr_reconnect-1.0") != std::string::npos)
                    {
                        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                        std::cout << "RECONNECT CONTROL MESSAGE " << std::endl;
                        std::cout << "HAS NEW SOURCE ADDRESS: " <<  fastWriter.write(parsedFromString["address"])  << std::endl;
                        bsd.globalBSZmqStream = fastWriter.write(parsedFromString["address"]).c_str();
                        dataHeaderReceived=false;
                    }
                    else if (fastWriter.write(parsedFromString["htype"]).find("bsr_stop-1.0") != std::string::npos)
                    {
                        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                        std::cout << "STOP CONTROL MESSAGE " << std::endl;
                        std::cout << "RECEIVED: " <<  fastWriter.write(parsedFromString["htype"])  << std::endl;
                        std::cout << "From BS Documentation: Message can be ignored as source will send from same address after startup" << std::endl;
                        dataHeaderReceived=false;
                    }
                    else
                    {
                        //Maybe this is not a header!??
                        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                        std::cout << "HEADER IS UNRECOGNIZED: SOMETHING OTHER THAN DOCUMENETED! " << std::endl;
                        std::cout << parsedFromString["htype"] << std::endl;
                        std::cout << "EXIT PROGRAM " << std::endl;
                        dataHeaderReceived=false;
                        exit(1);
                    }
                    mainHeaderReceived=true;
                } //parsing successful
                else
                {
                    std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                    std::cout << "PARSING FAILED !! " << std::endl;
                    std::cout << "CANNOT PROCEED WITHOUT MAIN HEADER INFORMATION! " << std::endl;
                    bsd.overallStatus=ECAFE_BSREAD_PARSEFAIL_MAINHEADER;

                    //Close message and break from loop
                    ++subMessage;

                    std::cout << "SUBMESSAGE " << subMessage << std::endl;
                    more=0;           //  Multipart detection
                    zmq_getsockopt (socket, ZMQ_RCVMORE, &more, &more_size);
                    zmq_msg_close (&message);
                    break;
                }
            } //ifMainHeaderReceived


            else if ( (mainHeaderReceived && !dataHeaderReceived) || (newHash && dataHeaderReceived))
            {
	      

	      //std::cout << "DATA HEADER ================================================================================= " << std::endl;
	      //std::cout << "mh = " << mainHeaderReceived << " dh " << dataHeaderReceived << " nh " << newHash  <<std::endl;
                newHash=false;
	
                //Is it compressed or not??
                if (bsd.getDH_compression().compare("bitshuffle_lz4")==0)
                {
                    /*
                    int uncompressedSize=0; int blockSize=0;
                    unfoldPreBlob(data, uncompressedSize, blockSize);
                    //decompress
                    int src_size=uncompressedSize/sizeof(char);
                    //use bitshuffle terminology
                    char* regen_buffer = (char *) malloc(src_size*sizeof(char));
                    if (regen_buffer == NULL) {
                    	bsd.overallStatus=ECAFE_BITSHUFF_ALLOCMEM;
                    	return;
                    }

                    int sizeBlob = size-BSREAD_PREBLOB_BYTES;
                    if (sizeBlob <1) {sizeBlob=size;} //Should not happen
                    char * dataBlob = new char[sizeBlob];
                    int ij=0;
                    for (int char_nbr = BSREAD_PREBLOB_BYTES; char_nbr < size; char_nbr++) {
                    	dataBlob[ij]=data[char_nbr];
                    	++ij;
                    }
                    //ECAFE_BITSHUFF_DECOMPRESS
                    //ECAFE_BITSHUFF_REALLOCMEM
                    const int decompressed_size = bshuf_decompress_lz4((const char *) dataBlob, regen_buffer, uncompressedSize, sizeof(char), 0);
                    delete [] dataBlob;
                    if(localDebug) std::cout << " " << " decompressed_size " << decompressed_size << std::endl;
                    if (decompressed_size < 0) {bsd.overallStatus=ECAFE_BITSHUFF_DECOMPRESS; return;}
                    else if (decompressed_size == 0){
                    	if(localDebug)std::cout << "0 decompressed size! Should have a positive number for successful deompression?" <<std::endl;
                    }
                    else {
                    	if(localDebug)std::cout << "We successfully decompressed some data!" <<std::endl;
                    }
                    // Not only does a positive return value mean success,
                    // value returned == number of bytes regenerated from compressed_data stream.
                    // We can use this to realloc() *regen_buffer to free up memory
                    //Do this to remove spurious trailing characters
                    regen_buffer = (char *)realloc(regen_buffer, decompressed_size);
                    if (regen_buffer == NULL) {
                    	std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                    	std::cout << "WARNING with status code: " << ECAFE_BITSHUFF_REALLOCMEM << std::endl;
                    	std::cout << "Failed to reallocate memory for compressed data. Not a show stopper" << std::endl;
                    }
                    */
                    char * regen_buffer = NULL;
                    int status=CAFE_BSHELPER::bitshuffleDecompress(data, regen_buffer, size, sizeof(char));
                    if (status==ICAFE_NORMAL)
                    {
                        parsingSuccessful=reader.parse((const char *) regen_buffer, parsedFromString);
                    }
                    else
                    {
                        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                        std::cout << "Data Header Compression Algorithmn: " << bsd.getDH_compression() << " FAILED! " << std:: endl;
                        std::cout << "With Status Code: " << status << std:: endl;
                        std::cout << "Discarding remainder of multi-part message! " << status << std:: endl;
                        bsd.overallStatus=status;
                        free(regen_buffer);

                        //Close message and break from loop
                        ++subMessage;

                        std::cout << "SUBMESSAGE " << subMessage << std::endl;
                        more=0;           //  Multipart detection
                        zmq_getsockopt (socket, ZMQ_RCVMORE, &more, &more_size);
                        zmq_msg_close (&message);
                        break;
                    }

                } //bitshuffle_lz4
                else if (bsd.getDH_compression()=="none")
                {
                    parsingSuccessful=reader.parse(data, parsedFromString);
                }
                else
                {
                    std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                    std::cout << "Data Header Compression Algorithmn: " << bsd.getDH_compression() << " not recognized " << std:: endl;
                    std::cout << "Assuming no compression for data header " << std::endl;
                    parsingSuccessful=reader.parse(data, parsedFromString);
                }

                if (parsingSuccessful)
                {

                    if (localDebug==1)
                    {
                        Json::StyledWriter styledWriter;
                        std::cout << "STYLED: --------------------------------//" << std::endl;
                        std::cout << styledWriter.write(parsedFromString) << std::endl;
                        std::cout << "----------------------------------//" << std::endl;
                        std::cout << "HEADER TYPE" << std::endl;
                        std::cout << parsedFromString["htype"].asString() << std::endl;
                    }

                    if (fastWriter.write(parsedFromString["htype"]).find("bsr_d-1.1") != std::string::npos)
                    {

                        if (localDebug) std::cout << "chan " << parsedFromString["channels"] << std::endl;

                        bsrdV.clear();
                        bsrdV.reserve(parsedFromString["channels"].size());

                        for (Json::Value::ArrayIndex i=0; i < parsedFromString["channels"].size(); ++ i)
                        {

                            //New line is spit out after parsedFromString[]
			  if (localDebug) {
                            std::cout << "name:" << parsedFromString["channels"][i]["name"].asString()     << " //"<< std::endl;
                            std::cout << "enco:" << parsedFromString["channels"][i]["encoding"].asString() << " //"<< std::endl;
                            std::cout << "type:" << parsedFromString["channels"][i]["type"].asString()     << " //" <<std::endl;
                            std::cout << "shape isArray " << parsedFromString["channels"][i]["shape"].isArray() << std::endl;
			  }
                            Json::Value iv;
                            Json::Value::ArrayIndex inpv=0;

                            //std::cout << "Value as int " << parsedFromString["channels"][i]["shape"][inpv].asDouble() << std::endl;
                            //std::cout << "SIZE " << parsedFromString["channels"][i]["shape"].size() << std::endl;
                            //std::cout << "COMP " << parsedFromString["channels"][i]["compression"].asString() << std::endl;
                            std::vector<int> lshape;
                            for (Json::Value::ArrayIndex il=0; il < parsedFromString["channels"][i]["shape"].size(); ++ il)
                            {
                                //std::cout << "Value as int// " << parsedFromString["channels"][i]["shape"][il].asInt() << std::endl;
                                lshape.push_back(std::max(1, parsedFromString["channels"][i]["shape"][il].asInt()) );
                            }

                            std::string NAME = parsedFromString["channels"][i]["name"].asString();
                            std::string ENCO = parsedFromString["channels"][i]["encoding"].asString(); // big default=little
                            if (ENCO.empty())
                            {
                                ENCO="little";    //default
                            }
                            std::string TYPE = parsedFromString["channels"][i]["type"].asString();
                            if (TYPE.empty())
                            {
                                TYPE="float64";    //default
                            }
                            std::string COMP = parsedFromString["channels"][i]["compression"].asString();
                            if (COMP.empty())
                            {
                                COMP="none";    //default
                            }

                            //std::cout << "TYPE " << TYPE << std::endl;
                            //std::cout << "TYPE: SIZE= " << TYPE.size() << std::endl;
                            //std::cout << TYPE.substr(0,TYPE.size()) << std::endl;

                            BSChannel bsc(NAME);

                            //Need to find correct entry
                            //int idx= bsd.getIdxFromName(NAME);
                            //BSChannel bsc=  bsd.getBSChannel(idx); // instead of i

                            bsc.setShape(lshape);

                            //Need to fill in Channel Name, encoding and Type...
                            bsc.setType(TYPE);
                            bsc.setEncoding(ENCO);
                            bsc.setCompression(COMP);

                            bsrdV.push_back(bsc);

                            if ( bsd.getPVIdx(NAME) < 0 )
                            {
                                std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                                std::cout << " WARNING--> THIS CHANNEL WAS NOT REQUESTED IN CONFIGURATION FILE " << std::endl;
                                std::cout << " pv requested = " << NAME << std::endl;
                                std::cout << " Illegal index Value =" << bsd.getPVIdx(NAME) << std::endl;
                                std::cout << " NO FURTHER ACTION REQUIRES " << std::endl;
                            }
                            else
                            {
                                //Ensure there is sufficent space to hold returned data,, e.g. may be an array
                                bsd.pvd[ bsd.getPVIdx(NAME)].setNelem(bsc.getNelem());
                            }
                            int idx= bsd.getIdxFromName(NAME);
                            bsd.setBSChannel(idx, bsc);

                            //std::cout << "The No elements are " << bsd.getBSChannel(i).getNelem() << std::endl;
                            //There may be more channels coming through this stream than requested
                            //therefore first fill a local vector<BSChannel>
                            //and then map this to that in BSDataHolder!
                            //It is conceivable that they do not match exactly

                        } // for JsonValue

                        if (localDebug)
                        {
                            for (int i=0; i< bsrdV.size(); ++i)
                            {
                                std::cout << i << " " <<  bsrdV[i].getName() << " " << bsd.getPVIdx( bsrdV[i].getName()) << std::endl;
                            }
                            std::cout << "No of channels " << bsrdV.size() << " " << parsedFromString["channels"].size() << std::endl;
                            std::cout << "----------------------------------" << std::endl;
                        }
                    } //bsr_d
                    dataHeaderReceived=true; //set FLAG
                } //parsingsuccessful
                else
                {
                    std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                    std::cout << "PARSING FAILED !! " << std::endl;
                    std::cout << "CANNOT PROCEED WITHOUT DATA HEADER INFORMATION! " << std::endl;
                    bsd.overallStatus=ECAFE_BSREAD_PARSEFAIL_DATAHEADER;

                    //Close message and break from loop
                    ++subMessage;

                    std::cout << "SUBMESSAGE " << subMessage << std::endl;
                    more=0;           //  Multipart detection
                    zmq_getsockopt (socket, ZMQ_RCVMORE, &more, &more_size);
                    zmq_msg_close (&message);
                    break;
                }

            } //dataHeader

            else if (subMessage==1)
            {
                //header again so we skip this as hash is the same!
                //Do nothing
                if(localDebug) std::cout << "HEADER INFO ALREADY REGISTERED " << std::endl;
            }
            else if (subMessage%2 ==1)
            {

	


                //std::cout << "timestamp " << std::endl;
                
                if (bsd.getPVIdx(bsrdV[bsPVIdx].getName()) <0)
                {
                    std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                    std::cout << " WARNING--> THIS CHANNEL WAS NOT REQUESTED IN CONFIGURATION FILE " << std::endl;
                    std::cout << " bsPV index = " <<  bsPVIdx << std::endl;
                    std::cout << " pv from bs = " << bsrdV[bsPVIdx].getName() << std::endl;
                    std::cout << " Expect illegal BSDataHolder index value =" << bsd.getPVIdx(bsrdV[bsPVIdx].getName()) << std::endl;
                    std::cout << " SKIPPING THIS SUBMESSAGE... " << subMessage << std::endl;
                }
                else
                {
                    ts=0;
                    tnsec=0;
                    //long test=1518777482;
                    //printBits(sizeof(test), &test);
                    //printBits(size, &data);
                    //size==16
		    /*bits come in this order for 64bit floats: SARFE10-PSSS059:SPECTRUM_CENTER 
01110101
00001011
11000110
01011111
00000000
00000000
00000000
00000000

                    whereas for other datatypes
00000000
00000000
00000000
00000000
01011111
11000110
00001011
01110101

ts
0000000000000000000000000000000001011111110001100101100000001111
ns
0000000000000000000000000000000000000010011100011001110001000000

                    So First need to verify which format is coming through:
		    If First type - then reorder!
		    */


		    bool littleEndian = false;

		    int den = 4; //must be 4
		    for (size_t n=0; n < 16/den; n++) {
		      char jh = data[n];
		      //printBits(sizeof(jh), &jh);
		      if (hasMinOneBitSet(sizeof(jh), &jh) ) 
			{
			  littleEndian = true ;
			 break;
			} 
                    }
		    if (localDebug) {
		      std::cout << "--------------------------------" << std::endl;
		      for (size_t n=0; n < size; n++) {
			char jh = data[n];
			printBits(sizeof(jh), &jh);		    
		      }
		    }
		   
		    //std::cout << "nano_seconds" << std::endl;
                    //for (size_t n=size/2; n < size; n++) {
                    //char jh = data[n];
                    //printBits(sizeof(jh), &jh);
                    //}
                  	
		   
	    
                    /*
		    if (littleEndian ) {
		      //little endian
		      int in = 0;
		      for (unsigned int n = (16-1); n >= 0; n--) {
			std::cout << n << " // " << in << std::endl;
			
			data[in] =  dataOriginal[n];
			++in;
		      }
		      std::cout << "LITTE ENDIAN TS = " << ts  << std::endl;
		    }
		    */

		    if (littleEndian) {
		      //std::cout << " TS littleEndian " <<  std::endl;
		       //little endian
		      //for (size_t n = size; n >= 0; n--) {
		      //	iv = (iv << 8) + data[n];
                    //} sum += bytes[0] | (bytes[1]<<8) | (bytes[2]<<16) | (bytes[3]<<24);
		      ts = 0xFF & data[0] | (0xFFFF & data[1]<<8) | (0xFFFFFF & data[2]<<16) | (0xFFFFFFFF & data[3]<<24);
		       /*
		       short inn=size/2-1;
		    
		       for (; inn>=0; inn--)
			 {    
			   
			   ts = (ts<<8) + data[inn];
			 }
		       */
		    }
		    else {  
		      //std::cout << " TS bigEndian " <<  std::endl;
		      for (size_t n=0; n < size/2; n++)
		      {
                        char jh = data[n];
                        //printBits(sizeof(jh), &jh);
                        if ( (n+1) < size/2 )
                        {
                            if ( data[n+1] & 0x80)
                            {
                                data[n]=data[n]+1;
                            }
                        }
                        //if (n==4) data[n]=data[n]+1; //Just what is the compiler playing at when shifting bits!??
                        //if (n==5) data[n]=data[n]+1; //ditto
                        ts = (ts<<8) + data[n];
		      }
		    }
		    //printBits(sizeof(ts), &ts);
		  
		    /*
		    if (littleEndian) {
		      uint64_t ts = __builtin_bswap64 ((uint64_t) ts);
		    
		      
		    std::cout << "ts after bitswap " << ts << " " << (unsigned long) ts << std::endl;
		    }
		    */
		    /*
		    uint32_t num = (unsigned int) ts;
		    uint32_t b0,b1,b2,b3;
		    uint32_t res;

		    b0 = (num & 0x000000ff) << 24u;
		    b1 = (num & 0x0000ff00) << 8u;
		    b2 = (num & 0x00ff0000) >> 8u;
		    b3 = (num & 0xff000000) >> 24u;
		    res = b0 | b1 | b2 | b3;
		   
		    printf("num %d, RES %d= \n",num, res);
		    printf("RES = %lX \n", res);
		    printBits(sizeof(ts), &ts);
                   
		    */
                    if(localDebug)std::cout << "ts original way " << ts << "///"  << std::endl;
                    //unsigned short idx=0;
                    //int ts_int=( ( (data[idx+7]<<0) & 0x7f) | ((data[idx+7]<<0) & 0x80)  |  (data[idx+6]<<8) | (data[idx+5]<<16) |  (data[idx+4]<<24)
                    //int ts_int=(  (data[idx+7]<<0)   |  (data[idx+6]<<8) | (data[idx+5]<<16) |  (data[idx+4]<<24)
                    //	| ((long long) data[idx+3]<<32) | ((long long) data[idx+2]<<40) | ((long long) data[idx+1]<<48) | ((long long) data[idx]<<56) ) ;

                    //std::cout << "ts_int= " << ts_int << std:: endl;
                    //idx=4;
                    //long long ts_long=( ( (data[idx+3]<<0) ) | ((data[idx+3]<<0) & 0x80)  |  ( (data[idx+2]<<8) ) | ((data[idx+2]<<8) & 0x80)  |
                    //										( (data[idx+1]<<16) ) | ((data[idx+1]<<16) & 0x80)  |  ( (data[idx]<<24) ) | ((data[idx]<<24) & 0x80) );


                    //std::cout << "ts_long= " << ts_long << std:: endl;

		   
		    if (littleEndian) {
		      //std::cout << " nsec littleEndian " <<  std::endl;
		       //little endian
		      //for (size_t n = size; n >= 0; n--) {
		      //	iv = (iv << 8) + data[n];
                    //}	
			 /*
		         short inn=size-1;		     
		         for (; inn>=size/2; inn--)
			 {    			    
			    tnsec = (tnsec) + data[inn];
			 }
			 */
			 tnsec = 0xFF & data[8] | (0xFFFF & data[9]<<8) | (0xFFFFFF & data[10]<<16) | (0xFFFFFFFF & data[11]<<24);

		    }
		    else {  	 
		      //std::cout << " nsec bigEndian " <<  std::endl;
		      for (size_t n=size/2; n < size; n++)
			{
			  char jh = data[n];
			  //printBits(sizeof(jh), &jh);
			  //if (n==12) data[n]=data[n]+1;
			  if ( (n+1) < size )
			    {
			      if ( data[n+1] & 0x80 )
				{
				  data[n]=data[n]+1;
				  //std::cout << "------------" << std::endl;
				  //jh = data[n];
				  //printBits(sizeof(jh), &jh);
				  //std::cout << "------------" << std::endl;
				}
			    }
			  tnsec = (tnsec<<8) + data[n];
			}

		    }

                    //printBits(sizeof(tnsec), &tnsec);
                    if(localDebug)std::cout << "tns original way " << tnsec  << std::endl;
		    //std::cout << "TESTING...................................................... " << std::endl;	
                    //idx=8;
                    //tnsec=( ( (data[idx+7]<<0) & 0x7f) | ((data[idx+7]<<0) & 0x80)  |  (data[idx+6]<<8) | (data[idx+5]<<16) |  (data[idx+4]<<24)
                    //	| ((long long) data[idx+3]<<32) | ((long long) data[idx+2]<<40) | ((long long) data[idx+1]<<48) | ((long long) data[idx]<<56) ) ;


                    if(localDebug)std::cout << "ts " << ts << " tnsec " << tnsec << std::endl;
                    //Add to bsd
                    bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].ts.secPastEpoch=ts;
                    bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].ts.nsec        =tnsec;
                    bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].setBeamEventNo(bsd.getPulse_id());
                }
		
            }
            else
            {
                //std::cout << " NOW COMES THE DATA " << std::endl;
                
                //if (localDebug) {
                //	for (int i=0; i< bsrdV.size(); ++i) {
                //		std::cout << i << " " <<  bsrdV[i].getName() << " " << bsd.getPVIdx(bsrdV[i].getName()) << std::endl;
                //	}
                //	std::cout << "No of channels " << bsrdV.size()  << std::endl;
                //	std::cout << "-----------------------" << std::endl;
                //}

                //Now comes the data
                ++bsPVIdx;

                if (bsd.getPVIdx(bsrdV[bsPVIdx].getName()) <0)
                {

                    std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                    std::cout << " WARNING--> THIS CHANNEL WAS NOT REQUESTED IN CONFIGURATION FILE " << std::endl;
                    std::cout << " bsPV index = " <<  bsPVIdx << std::endl;
                    std::cout << " pv from bs = " << bsrdV[bsPVIdx].getName() << std::endl;
                    std::cout << " Expect illegal BSDataHolder index value =" << bsd.getPVIdx(bsrdV[bsPVIdx].getName()) << std::endl;
                    std::cout << " SKIPPING THIS SUBMESSAGE... " << subMessage << std::endl;

                    more=0;           //  Multipart detection
                    zmq_getsockopt (socket, ZMQ_RCVMORE, &more, &more_size);
                    zmq_msg_close (&message);
                    ++subMessage;
                    if (!more)
                    {
                        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                        std::cout << " No more submessages! " << std::endl;
                        std::cout << " This message implies that pv was not accompanied by a timestamp!! Something not quite right. " << std::endl;
                        break;      //  Last message part
                    }
                    continue; //top of while
                }

                if(localDebug)
                {
                    std::cout << "index= " << bsPVIdx <<  std::endl;
                    std::cout << "Name = " << bsrdV[bsPVIdx].getName() <<  " " << bsrdV[bsPVIdx].getType() << std::endl;
                    std::cout << "IDx  = " << bsd.getPVIdx(bsrdV[bsPVIdx].getName()) << std::endl;
                }

                //Find enum of data type
                it_name = name_index.find(bsrdV[bsPVIdx].getType());
                if ( (it_name) != name_index.end() )
                {
                    bsdtIndex=(*it_name).by_bsID;
                }
                //Do we need to compress the data?
                bool compressedFlag=false;
                BSChannel bsc=bsd.getBSChannel(bsrdV[bsPVIdx].getName());
                char * regen_buffer;
		//std::cout << "CHANNEL==" <<  bsrdV[bsPVIdx].getName() << std::endl;
		//std::cout << "COMPRESSION ============" << bsc.getCompression() << std::endl;

                if ( bsc.getCompression().compare("bitshuffle_lz4")==0)
                {

                    //Decompress

                    int status=CAFE_BSHELPER::bitshuffleDecompress(data, regen_buffer, size, getByteSize(bsdtIndex));
                    if (status==ICAFE_NORMAL)
                    {
                        parsingSuccessful=reader.parse((const char *) regen_buffer, parsedFromString);
			//std::cout << "parsingSuccessful====" << parsingSuccessful << std::endl;
                    }
                    else
                    {
                        free(regen_buffer);
                        bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].setStatus(status);
                        bsd.overallStatus=status;
                        //skip message
                        more=0;           //  Multipart detection
                        zmq_getsockopt (socket, ZMQ_RCVMORE, &more, &more_size);
                        zmq_msg_close (&message);
                        ++subMessage;
                        if (!more)
                        {
                            std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                            std::cout << " No more submessages! " << std::endl;
                            std::cout << " This message implies that pv was not accompanied by a timestamp!! Something not quite right. " << std::endl;
                            break;      //  Last message part
                        }
                        continue; //top of while
                    }
                    compressedFlag=true;

                }
                else
                {
                    //No compression required
                    regen_buffer = data; //new char[size];
                    //regen_buffer = new char[size];
                    //for (int i=0; i<size; ++i) {
		    //  std::cout << i << " " << (unsigned short) data[i] << std::endl;
		    // }
                    //std::cout << size << " /// " << bsrdV[bsPVIdx].getNelem()<< std::endl;
		    //std::cout << "regen_buffer" << std::endl;
		    //std::cout << (int *) regen_buffer << std::endl;

                }

                //std::cout << "Compression//0//--> " << bsd.getBSChannel(0).getCompression() << std::endl;
                //std::cout << "Compression//1//--> " << bsd.getBSChannel(1).getCompression() << std::endl;
                bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].setStatus(ICAFE_NORMAL);

                //char,bool=1, short=2, int =4, long,long long=8

                switch (bsdtIndex)
                {
                case CAFE_BSHELPER::BS_STRING:
                {
                    if (bsrdV[bsPVIdx].getNelem()>1)
                    {
                        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                        std::cout << "An array of strings has not been envisioned for bsREAD" << std::endl;
                        std::cout << "Assuming one string element of max size PVNAME_SIZE" << std::endl;
                    }
                    if (size > MAX_STRING_SIZE)
                    {
                        std::cout << "Datatype is string. Size " << size << " too large; trimming to MAX_STRING_SIZE" << std::endl;
                    }

                    std::string str="";;
                    for (int i=0; i<std::min((int)size, (int)MAX_STRING_SIZE); ++i)
                    {
                        if (data[i] != '\0')
                        {
                            str.append(1, data[i] );
                            //std::cout << str << " [" << i << "] " << std::endl;
                        }
                    }
                    bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set(str);
                    if(localDebug)std::cout << " readback of string value that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[0].str << std::endl;
                    break;
                }
                case CAFE_BSHELPER::BS_FLOAT32:
                {
                    if ( bsrdV[bsPVIdx].getNelem()==1)
                    {
                        bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set( bsetFloat.unfoldScalar(regen_buffer, bsrdV[bsPVIdx].getEncoding()) );
                        if(localDebug)std::cout << " readback of float scalar that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[0].f << std::endl;
                        break;
                    }
                    //std::vector<float> Vbs( bsrdV[bsPVIdx].getNelem()); //Must allocate memory here
                    //bsetFloatV.unfold(regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding(),Vbs);
                    //for (int ik=0; ik<Vbs.size(); ++ik) std::cout << Vbs[ik] << " [" << ik << "] " << std::endl;
		    //std::cout << "NELEM " << bsrdV[bsPVIdx].getNelem() << " Encoding" << bsrdV[bsPVIdx].getEncoding() << std::endl;
                    float * ff = bsetFloat.unfold(regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding());
		    //std::cout << "float " << ff[0] << std::endl;
                    bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set(ff);
                    if(localDebug)std::cout << " readback of float value that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[0].f << std::endl;
                    delete [] ff;
                    break;
                }
                case CAFE_BSHELPER::BS_FLOAT64:
                {
                    if ( bsrdV[bsPVIdx].getNelem()==1)
                    {
                        bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set( bsetDouble.unfoldScalar(regen_buffer, bsrdV[bsPVIdx].getEncoding()) );
                        if (localDebug) std::cout << " readback of double scalar that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[0].d << std::endl;
                        break;
                    }
                    //std::vector<double> Vbs( bsrdV[bsPVIdx].getNelem()); //Must allocate memory here
                    //bsetDoubleV.unfold(regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding(),Vbs);
                    double * dd =  bsetDouble.unfold(regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding());
                    bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set(dd);
                    if (localDebug) std::cout << " readback of double value that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[0].d << std::endl;
                    delete [] dd;
                    break;
                }
                case CAFE_BSHELPER::BS_INT64:
                {
                    if ( bsrdV[bsPVIdx].getNelem()==1)
                    {
                        bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set( bsetLongLong.unfoldScalar(regen_buffer, bsrdV[bsPVIdx].getEncoding()) );
                        if(localDebug)std::cout << " readback of int64 scalar that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].getAsLongLong()  << std::endl;
                        break;
                    }
                    //std::vector<long long> Vbs;
                    //bsetLongLongV.unfold(regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding(),Vbs);
                    long long * ll=bsetLongLong.unfold (regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding());
                    bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set(ll);
		    
                    if(localDebug)
                    {
		        std::cout << bsrdV[bsPVIdx].getName() << std::endl;
                        std::cout << " readback of int64 value that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].getAsLongLong() << std::endl;
                        //for (int i=0; i < bsrdV[bsPVIdx].getNelem(); ++i) {
                        //	std::cout <<  bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].getAsLongLong(i) << " [" << i << "] " ;
                        //}
                        //std::cout << std::endl;
                    }
                    delete [] ll;
                    break;
                }
                case CAFE_BSHELPER::BS_UINT64:
                {
                    if ( bsrdV[bsPVIdx].getNelem()==1)
                    {
                        bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set( bsetULongLong.unfoldScalar(regen_buffer, bsrdV[bsPVIdx].getEncoding()) );
                        if(localDebug)std::cout << " readback of uint64 scalar that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].getAsULongLong()  << std::endl;
                        break;
                    }
                    //std::vector<unsigned long long> Vbs;
                    //bsetULongLongV.unfold(regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding(),Vbs);
                    unsigned long long * ull=bsetULongLong.unfold (regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding());
                    bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set(ull);
                    if(localDebug)
                    {
                        std::cout << " readback of uint64 value that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].getAsULongLong() << std::endl;

                        //for (int i=0; i < bsrdV[bsPVIdx].getNelem(); ++i) {
                        //	std::cout <<  bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].getAsULongLong() << " [" << i << "] " ;
                        //}
                        std::cout << std::endl;
                    }
                    delete [] ull;
                    break;
                }
                case CAFE_BSHELPER::BS_INT32:
                {
                    if ( bsrdV[bsPVIdx].getNelem()==1)
                    {
                        bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set( bsetInt.unfoldScalar(regen_buffer, bsrdV[bsPVIdx].getEncoding()) );
                        if(localDebug)std::cout << " readback of int32 scalar that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[0].l  << std::endl;
                        break;
                    }
                    //std::vector<int> Vbs;
                    //bsetIntV.unfold(regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding(),Vbs);
                    int * l=bsetInt.unfold (regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding());
                    bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set(l);
                    if(localDebug)
                    {
                        std::cout << bsrdV[bsPVIdx].getName() << std::endl;
                        std::cout << " readback of int32 value that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[0].l << std::endl;
                        for (int i=0; i < bsrdV[bsPVIdx].getNelem(); ++i)
                        {
                            std::cout <<  bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[i].l << " [" << i << "] " ;
                        }
                        std::cout << std::endl;
                    }
                    delete [] l;
                    break;
                }
                case CAFE_BSHELPER::BS_UINT32:
                {
                    if ( bsrdV[bsPVIdx].getNelem()==1)
                    {
                        bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set( bsetUInt.unfoldScalar(regen_buffer, bsrdV[bsPVIdx].getEncoding()) );
                        if(localDebug)std::cout << " readback of uint32 scalar that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].getAsULong()  << std::endl;
                        break;
                    }
                    //std::vector<unsigned int> Vbs;
                    //bsetUIntV.unfold(regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding(),Vbs);
                    unsigned int * ul=bsetUInt.unfold (regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding());
                    bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set(ul);
                    if(localDebug)
                    {
                        std::cout << bsrdV[bsPVIdx].getName() << std::endl;
                        std::cout << " readback of uint32 value that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].getAsULong() << std::endl;
                        //for (int i=0; i < bsrdV[bsPVIdx].getNelem(); ++i) {
                        //	std::cout <<  bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].getAsULong(i) << " [" << i << "] " ;
                        //}
                        //std::cout << std::endl;
                    }
                    delete [] ul;
                    break;
                }
                case CAFE_BSHELPER::BS_INT16:
                {
                    if ( bsrdV[bsPVIdx].getNelem()==1)
                    {
                        bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set( bsetShort.unfoldScalar(regen_buffer, bsrdV[bsPVIdx].getEncoding()) );
                        if(localDebug)std::cout << " readback of short scalar that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[0].s << std::endl;
                        break;
                    }
                    //std::vector<short> Vbs;
                    //bsetShortV.unfold(regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding(),Vbs);

                    short * s=bsetShort.unfold (regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding());
                    //std::cout <<  "SHORT VALUE " << s[0] << std::endl;
                    bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set(s);
                    if(localDebug)
                    {
                        std::cout << " readback of short value that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[0].s << std::endl;
                        //std::cout << " nelements in pvdata " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].getNelem() << std::endl;
                        //for (int i=0; i < bsrdV[bsPVIdx].getNelem(); ++i) {
                        //	std::cout <<  bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[i].s << " [" << i << "] " ;
                        //}
                        //std::cout << std::endl;
                    }
                    delete [] s;
                    break;
                }
                case CAFE_BSHELPER::BS_UINT16:
                {
                    if ( bsrdV[bsPVIdx].getNelem()==1)
                    {
                        bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set( bsetUShort.unfoldScalar(regen_buffer, bsrdV[bsPVIdx].getEncoding()) );
                        if(localDebug)std::cout << " readback of ushort scalar that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[0].us << std::endl;
                        break;
                    }
                    //std::vector<unsigned short> Vbs;
                    //bsetUShortV.unfold(regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding(),Vbs);
                    unsigned short * us=bsetUShort.unfold (regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding());
                    bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set(us);
                    if(localDebug)
                    {
                        std::cout << " readback of ushort value that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[0].us << std::endl;
                        //for (int i=0; i < bsrdV[bsPVIdx].getNelem(); ++i) {
                        //	std::cout <<  bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[i].us << " [" << i << "] " ;
                        //}
                        //std::cout << std::endl;
                    }
                    delete [] us;
                    break;
                }
                case CAFE_BSHELPER::BS_INT8:
                {
                    if ( bsrdV[bsPVIdx].getNelem()==1)
                    {
                        bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set( bsetChar.unfoldScalar(regen_buffer, bsrdV[bsPVIdx].getEncoding()) );
                        if(localDebug)std::cout << " readback of char scalar that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[0].ch << std::endl;
                        break;
                    }
                    //std::vector<char> Vbs;
                    //bsetCharV.unfold(regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding(),Vbs);
                    char * ch=bsetChar.unfold (regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding());
                    bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set(ch); //set(ch);
                    if(localDebug)
                    {
                        std::cout << " readback of char value that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[0].ch << std::endl;
                        //for (int i=0; i < bsrdV[bsPVIdx].getNelem(); ++i) {
                        //	std::cout <<  bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[i].ch << " [" << i << "] " ;
                        //}
                        //std::cout << std::endl;
                    }
                    delete [] ch;
                    break;
                }
                case CAFE_BSHELPER::BS_UINT8:
                case CAFE_BSHELPER::BS_BOOL:
                {
                    if ( bsrdV[bsPVIdx].getNelem()==1)
                    {
                        bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set( bsetUChar.unfoldScalar(regen_buffer, bsrdV[bsPVIdx].getEncoding()) );
                        if(localDebug)std::cout << " readback of unsigned char scalar that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[0].ch << std::endl;
                        break;
                    }
                    //std::vector<unsigned char> Vbs;
                    //bsetUCharV.unfold(regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding(),Vbs);
                    unsigned char * uch=bsetUChar.unfold (regen_buffer, bsrdV[bsPVIdx].getNelem(), bsrdV[bsPVIdx].getEncoding());
                    bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].set(uch);
                    if(localDebug)
                    {
                        std::cout << " readback of unsigned char value that was set = " << bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[0].ch << std::endl;
                        //for (int i=0; i < bsrdV[bsPVIdx].getNelem(); ++i) {
                        //	std::cout <<  bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].val[i].ch << " [" << i << "] " ;
                        //}
                        //std::cout << std::endl;
                    }
                    delete [] uch;
                    break;
                }
                default:
                    std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                    std::cout << "ECAFE_INVALID_SWITCH_CASE" << std::endl;
                    print_out_by<by_bsID>(bsdt);
                    bsd.pvd[bsd.getPVIdx(bsrdV[bsPVIdx].getName())].setStatus(ECAFE_INVALID_SWITCH_CASE);
                }
                if (compressedFlag)
                {
                    free(regen_buffer);
                }

	       
            }
        } //top else


        // THIS HAS TO BE COPIED TO BEFORE CONTINUE!

        //Continue goes to top of while
        //Why not add submessage here
        ++subMessage;

        //std::cout << "SUBMESSAGE " << subMessage << std::endl;
        more=0;           //  Multipart detection
        zmq_getsockopt (socket, ZMQ_RCVMORE, &more, &more_size);
        zmq_msg_close (&message);

        if (!more)
        {
            if(localDebug)puts ("//END----------------------------------------//");
            break;      //  Last message part
        }


    } //while 1

    	ptime timeEnd(microsec_clock::local_time());
        time_duration duration(timeEnd-timeStart);
        timeElapsed= (double) duration.total_microseconds()/1000000.0;
	//++npoll;
	//timeAvg = (timeAvg +  timeElapsed)/npoll;
	//std::cout <<  __METHOD__ <<  __LINE__ << " TimeElapsed "  << timeElapsed << " "  << std::endl;
	//std::cout << " Pulse_id: " << bsd.getPulse_id() <<std::endl;


//First two messages are main header and data header
    if (subMessage>1)
    {
        //bsd.overallStatus=ICAFE_NORMAL; // already at top; will be overwritten in case of error
        bsd.setNChannels((subMessage-2)/2);
        bsd.setNNullData(nZeroSize/2);
        bsd.setPGoodData((subMessage-nZeroSize-2)*100/(std::max((subMessage-2),1)) );
        if(localDebug)
        {
            std::cout << " No of submessages: " << subMessage << std::endl;
            std::cout << " of which zero size have: " << nZeroSize << std::endl;
            std::cout << " No of channels: " << bsd.getNChannels()<< std::endl;
            std::cout << " of which zero size have: " << bsd.getNNullData() << std::endl;
            std::cout << " Percentage good: " << bsd.getPGoodData() <<std::endl;
	    std::cout << " Pulse_id: " << bsd.getPulse_id() <<std::endl;

            puts ("//------------------------------------END-------------------------------------//");

           
	      }
    }


    //std::cout << "z_bsread.h overallstatus " <<  bsd.overallStatus << std::endl;

    return;


#undef __METHOD__
}

#endif  //  ZBSREAD_H


            
