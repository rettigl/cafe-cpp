/*  =====================================================================
    zhelpers.h

    Helper header file for example applications.
    =====================================================================
*/



#ifndef __ZHELPERS_H_INCLUDED__
#define __ZHELPERS_H_INCLUDED__

#if HAVE_ZEROMQ


//  Include a bunch of headers that we will need in the examples

#include <zmq.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#if (defined (WIN32))
#   include <time.h>
#else
#   include <sys/time.h>
#endif

#include <assert.h>
#include <signal.h>

//  Version checking, and patch up missing constants to match 2.1
#if ZMQ_VERSION_MAJOR == 2
#   error "Please upgrade to ZeroMQ/3.2 for these examples"
#endif

//  Provide random number from 0..(num-1)
#if (defined (WIN32))
#   define randof(num)  (int) ((float) (num) * rand () / (RAND_MAX + 1.0))
#else
#   define randof(num)  (int) ((float) (num) * random () / (RAND_MAX + 1.0))
#endif

#if HAVE_JSON
	#include <json/json.h>
#endif
	
#include <cafeService.h>


unsigned short inDumpFlag=2;
string hashIs="";
string hashOriginal="";
unsigned short hashOriginalFlag=0;
bool fillBSPV=false;
vector<std::string> bsPV;
	
//  Receive 0MQ string from socket and convert into C string
//  Caller must free returned string. Returns NULL if the context
//  is being terminated.
static char *
s_recv (void *socket) {
    char buffer [4096];
    int size = zmq_recv (socket, buffer, 4096, 0); //ZMQ_DONTWAIT);
    if (size == -1)
        return NULL;
    if (size > 4096)
        size = 4096;
    buffer [size] = 0;
    return strdup (buffer);
}

//  Convert C string to 0MQ string and send to socket
static int
s_send (void *socket, char *string) {
    int size = zmq_send (socket, string, strlen (string), 0); 
    return size;
}

//  Sends string as 0MQ string, as multipart non-terminal
static int
s_sendmore (void *socket, char *string) {
    int size = zmq_send (socket, string, strlen (string), ZMQ_SNDMORE);
    return size;
}



//  Receives all message parts from socket, prints neatly
//
static void
s_dump (DBPMKeeper & dbpm)
{

		if (inDumpFlag==1) {
			cout << "zeroMQ socket is busy " << endl;
			cout << "waiting got zmq timeout " << endl;
		}
		
   	puts ("//START----------------------------------------//");
		
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

    cout << "df = " << inDumpFlag << endl; 	

	  while (inDumpFlag==1) {
		    cout << "df/ = " << inDumpFlag << endl; 
	      cout << " sleeping " << endl;
				sleep(1);
		}

		int subMessage=0;		
		int nSequentialHeader=0;
		
    while (1) {
			  inDumpFlag=1;
			 				
		   // puts ("//WHILE LOOP ----------------------------------------//");
				//cout << "subMessage " << subMessage << endl;
        //  Process all parts of the message
        zmq_msg_t message;
        zmq_msg_init (&message);
				
        size_t size = zmq_msg_recv (&message, socket, 0);
				//puts ("//MESSAGE RECEIVED     ----------------------------------------//");
				
				if (size == -1) {
					cout << " Error is " << zmq_errno() << " " << zmq_strerror(zmq_errno()) << endl;
					//Resource unavailable means that there is nothing to read now
					
					 zmq_msg_close (&message);
					 dbpm.status=ICAFE_LINUX_ERROR+zmq_errno() ;
					 break;
				} 
				
				
				else if (size == 0) {

					//cout << " Data of Zero SIZE for submessage " << subMessage << endl;
			
					++nZeroSize;
					
					 //zmq_msg_close (&message);
					 //dbpm.status=ECAFE_NODATA;
					 //break;
					 //Comes in pairs; one for val one for timestamp
					 if (nZeroSize%2==1) {
					   ++bsPVIdx;					  
						
					  // cout << " pv with zero size:  " << bsPV[bsPVIdx] << endl;
						 
					 }
					 				 
					 ++subMessage;
					 
					 continue;
				} 
        else { 
			
        //  Dump the message as text or binary
        char *data = (char*)zmq_msg_data (&message);
        int is_text = 1;
        int char_nbr;
				
				//char cmd[5000]="";
				
        for (char_nbr = 0; char_nbr < size; char_nbr++)
            if ((unsigned char) data [char_nbr] < 32
            ||  (unsigned char) data [char_nbr] > 127)
                is_text = 0;

	      // printf (" SIZE [%03d] ", size);

		/*
		if (is_text) {
			cout << " TEXT +++++++++++ " << endl;
		}
		else {
			cout << " NOT TEXT +++++++++++ " << endl;
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
								//	cout << " little endian " << endl;
								//}
								//else {
								//	cout << "big endian " << endl;
								//}
				
								
						}
        }//for
		printf ("\n");
		*/
				
			
				if (is_text) {
						parsingSuccessful=reader.parse(data, parsedFromString);
						if (parsingSuccessful) {
							Json::StyledWriter styledWriter;
							//cout << "STYLED: --------------------------------" << endl;
							//cout << styledWriter.write(parsedFromString) << endl;
							//cout << "----------------------------------" << endl;
							cout << parsedFromString["htype"] << endl;
							
							
							if (fastWriter.write(parsedFromString["htype"]).find("bsr_m-1.1") != std::string::npos) { 

								++nSequentialHeader;

								hashIs=parsedFromString["hash"].asString();

								if (hashOriginalFlag<2) {
									cout << hashIs << " is different to original/// " << hashOriginal << endl;	
									hashOriginal=hashIs;
									++hashOriginalFlag;
									fillBSPV=true;
								}

								if (hashOriginal.compare(hashIs)!=0) {
									cout << hashIs << " is different to original " << hashOriginal << endl;	
									fillBSPV=true;
																
								}
                cout << "p id " << parsedFromString["pulse_id"].asUInt64() << endl;		

								//Reset values as a change of hash signifies that data from two pulse ids
								//is being sent in one zeromq messages

								subMessage=0;
								
								bsPVIdx=-1;
								dbpm.status=ICAFE_NORMAL;

								nZeroSize=0;
								
								
							/*
							cout << "(1)++++++++++++++++++++++++++++++++++++++++MAIN++++++++++++++++++++++++++++++++++++++++++"	<< endl;
						
								cout << "hash " << parsedFromString["hash"] << endl;
								cout << "p id " << parsedFromString["pulse_id"].asUInt64() << endl;
								cout << "g ts " << parsedFromString["global_timestamp"] << endl;
								cout << "comp " << parsedFromString["dh_compression"] << endl;
							
								cout << "sec " << parsedFromString["global_timestamp"]["sec"].asUInt() << endl;
							  cout << "nsec " << parsedFromString["global_timestamp"]["ns"].asUInt() << endl;
							*/
								
							}	
							else if (fastWriter.write(parsedFromString["htype"]).find("bsr_d-1.1") != std::string::npos) { 
							
					
								
							 //if  (fillBSPV) {
							
									bsPV.clear();
									bsPV.reserve(dbpm.getNPV());
									/*
									if (dbpm.getNPV() != parsedFromString["channels"].size() ) {
										cout << "No of CONFIGURED BPMS: " << dbpm.getNPV() 
										     << " is diffent to that being channeled " <<  parsedFromString["channels"].size() << endl;
										bsPV.reserve( max( (size_t) parsedFromString["channels"].size(),dbpm.getNPV()) );	 
										
									}
									
								*/
		
											   					
								//cout << "chan " << parsedFromString["channels"] << endl;
								
								cout << "No of channels " << parsedFromString["channels"].size() << endl;

								cout << "(2)++++++++++++++++++++++++++++++++++++++++HEADER++++++++++++++++++++++++++++++++++++++++++"	<< endl;
													
									 	for (Json::Value::ArrayIndex i=0; i < parsedFromString["channels"].size(); ++ i) {
							
									  		//cout << "name " << parsedFromString["channels"][i]["name"].asString() << endl;
									  		//cout << "enco " << parsedFromString["channels"][i]["encoding"] << endl;
								    		//cout << "type " << parsedFromString["channels"][i]["type"] << endl;
									 
									 			bsPV.push_back( (parsedFromString["channels"][i]["name"]).asString());
										}
										/*
										cout << "LIST OF PVS " << endl;
										for (size_t i=0; i< bsPV.size(); ++i ) {			
											cout << i << " // " <<   bsPV[i].c_str() << " " << endl;
										}
										cout << endl;
										*/
										
										//cout << "NEW FILL: size of bsPV " <<  bsPV.size() << endl;
										fillBSPV=false;
								//}

							}	
							
							else {
							
							  cout << "HEADER IS SOMETHING ELSE: " << endl;
										cout << parsedFromString["htype"] << endl;
							
								exit(1);
							
							}
							
							
							//cout << "----------------------------------" << endl;
								
					}
				}
				
				
				if (nSequentialHeader >5) {
					 cout << "WARNING: ZEROMQ SUB-MESSAGE DOES NOT CLOSE " << endl; 
					 cout << "WARNING: FORCING CLOSE AND BREAKING FROM LOOP " << endl; 
					 zmq_getsockopt (socket, ZMQ_RCVMORE, &more, &more_size);
					 			 
           zmq_msg_close (&message);
					 cout << "message mulipart --> more == " << more << endl;
					 	
						dbpm.status=LINUX_EAGAIN; //ICAFE_LINUX_ERROR +zmq_errno() ;
			 	  //break;
				
				}
				
				
				union foo
				{
				char c[sizeof(double)];
				double d;
			
				} bar;
				
				//SIZE 16 is timestamp
				//SIZE 8  is x,y,Q
				//SIZE 2  is valid
				
				if (subMessage > 1430) {
				  cout << " SIZE " << size << " subMessage " << subMessage << endl;
				  cout << "size of bsPV " <<  bsPV.size() << endl;
				}	
					
				if (subMessage > 0 && subMessage%2 ==0) {
					if (size==8) {
						
						// big endian	
						for (char_nbr = 0; char_nbr < size; char_nbr++) {
						 bar.c[char_nbr]=data[size-1-char_nbr];  // THis works for big engian
						}
						// little endian	
						//for (char_nbr = 0; char_nbr < size; char_nbr++) {
						// bar.c[char_nbr]=data[char_nbr];  
						//}
						
						
						//cout << "UNION D " << bar.d << endl;
					
						
						double v; // = (double*) data;
						memcpy(&v, bar.c, sizeof(double));
							//cout << " double val " << v << endl;
				
					//This is BPM Data - value
					++bsPVIdx;
					 
					if (dbpm.getPVIdx(bsPV[bsPVIdx]) <0)  {
					  cout << " WARNING--> THIS CHANNEL WAS NOT REQUESTED IN CONFIGURATION FILE " << endl;
						cout << " bsPV index = " <<  bsPVIdx << endl;
					  cout << " pv from bs = " << bsPV[bsPVIdx] << endl;
					  cout << " Illegal index Value =" << dbpm.getPVIdx(bsPV[bsPVIdx]) << endl;
						cout << " SKIPPING THIS BPM... " << endl;
						continue;
					}
					
					
					dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].set(v);
					
					//cout << " readback of value that was set = " << dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].val[0].d << endl;
						
					}
					else if (size==2) {
						unsigned short iv;
						//memcpy(&iv, cmd, size);
							//big endian
							for (size_t n=0; n < size; n++) {
								iv = (iv << 8) + data[n];
							}
							//little endian
							//for (size_t n = size; n >= 0; n--) {
						 	//	iv = (iv << 8) + data[n];
							//}
					
					//This is BPM Data	- VALID/INVALID	
							
							//cout << "uint val (1 means valid) " << iv << endl;
							
					  /*
						std::copy(data, data + 32, reinterpret_cast<char *>(&i));
						cout << "uint16  val " << i << endl;
						std::copy(data, data + 8, reinterpret_cast<char *>(&i));
						cout << "uint16  val " << i << endl;
						std::copy(data, data + 4, reinterpret_cast<char *>(&i));
						cout << "uint16  val " << i << endl;
						std::copy(data, data + 2, reinterpret_cast<char *>(&i));
						cout << "uint16  val " << i << endl;
						std::copy(data, data + 1, reinterpret_cast<char *>(&i));
						cout << "uint16  val " << i << endl;
						
						*/
							++bsPVIdx;
						
							 
					if (dbpm.getPVIdx(bsPV[bsPVIdx]) <0)  {
					  cout << " WARNING--> THIS CHANNEL WAS NOT REQUESTED IN CONFIGURATION FILE " << endl;
						cout << " bsPV index = " <<  bsPVIdx << endl;
					  cout << " pv from bs = " << bsPV[bsPVIdx] << endl;
					  cout << " Illegal index Value =" << dbpm.getPVIdx(bsPV[bsPVIdx]) << endl;
						cout << " SKIPPING THIS BPM ENUM TYPE " << endl;
						continue;
					}
						
								
								
								
							if (iv==1) {
								dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].set((std::string) "VALID");
								
							}	
							else {
								dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].set((std::string) "INVALID");
							}
							
								//cout << "value DBPM = " << dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].val[0].str << endl;
					}	
				
					
				}
				else if (subMessage > 1 && subMessage%2 ==1) {
					//cout << "timestamp " << endl;
					unsigned int a,b;
					for (size_t n=0; n < size/2; n++) {
								a = (a << 8) + data[n];
					}
					for (size_t n=size/2; n < size; n++) {
								b = (b << 8) + data[n];
					}
					
					//cout << "a " << a << " b " << b << endl;
					
					 
					if (dbpm.getPVIdx(bsPV[bsPVIdx]) <0)  {
					  cout << " WARNING--> THIS CHANNEL WAS NOT REQUESTED IN CONFIGURATION FILE " << endl;
						cout << " bsPV index = " <<  bsPVIdx << endl;
					  cout << " pv from bs = " << bsPV[bsPVIdx] << endl;
					  cout << " Illegal index Value =" << dbpm.getPVIdx(bsPV[bsPVIdx]) << endl;
						cout << " SKIPPING THIS BPM TIMESTAMP " << endl;
						continue;
					}
					
					
					
					dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].ts.secPastEpoch=a;
					dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].ts.nsec=b;
				}
				
			
				
					//cout << "subMessage above: " << subMessage << endl;
				
				++subMessage;
				
      
				} //ifelse
				
      
        zmq_getsockopt (socket, ZMQ_RCVMORE, &more, &more_size);
        zmq_msg_close (&message);
				
			
				
        if (!more) {
				  cout << "subMessage total: " << subMessage << endl;
				  puts ("//------------------------------------END-------------------------------------//");
					 
            break;      //  Last message part
				}
    } //while 1
		
		inDumpFlag=0;
	
		
		return;
}









//  Receives all message parts from socket, prints neatly
//
static void
s_dump (void *socket)
{
    puts ("//START----------------------------------------//");
		int subMessage=0;
		#if HAVE_JSON
			Json::Value parsedFromString;
			Json::Reader reader;
			bool parsingSuccessful;
			Json::FastWriter fastWriter;
		
		#endif
		
		
		
    while (1) {
        //  Process all parts of the message
        zmq_msg_t message;
        zmq_msg_init (&message);
        size_t size = zmq_msg_recv (&message, socket, 0);
				
				if (size == -1) {
					cout << " Error is " << zmq_errno() << " " << zmq_strerror(zmq_errno()) << endl;
					//Resource unavailable means that there is nothing to read now
					
					 zmq_msg_close (&message);
					 break;
				} 
        else { 
			
        //  Dump the message as text or binary
        char *data = (char*)zmq_msg_data (&message);
        int is_text = 1;
        int char_nbr;
				
				//char cmd[5000]="";
				
        for (char_nbr = 0; char_nbr < size; char_nbr++)
            if ((unsigned char) data [char_nbr] < 32
            ||  (unsigned char) data [char_nbr] > 127)
                is_text = 0;

        printf ("[%03d] ", size);
				
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
								//	cout << " little endian " << endl;
								//}
								//else {
								//	cout << "big endian " << endl;
								//}
				
								
						}
        }//for
					printf ("\n");	
				/*
        for (char_nbr = 0; char_nbr < size; char_nbr++) {
            if (is_text) {
                  printf ("%c", data [char_nbr]);
								  snprintf(cmd + strlen(cmd), (sizeof cmd) - strlen(cmd), "%c", data [char_nbr]);
								}
            else {
                printf ("%02X", (unsigned char) data [char_nbr]);
								snprintf(cmd + strlen(cmd), (sizeof cmd) - strlen(cmd), "%02X", (unsigned char)data [size-1-char_nbr]);
								}
        }
				*/
			
			
			
					
			
				if (is_text) {
						parsingSuccessful=reader.parse(data, parsedFromString);
						if (parsingSuccessful) {
							Json::StyledWriter styledWriter;
							cout << "STYLED: --------------------------------" << endl;
							cout << styledWriter.write(parsedFromString) << endl;
							cout << "----------------------------------" << endl;
							cout << parsedFromString["htype"] << endl;
							
							
							if (fastWriter.write(parsedFromString["htype"]).find("bsr_m-1.1") != std::string::npos) { 
						
								cout << "hash " << parsedFromString["hash"] << endl;
								cout << "p id " << parsedFromString["pulse_id"].asUInt64() << endl;
								cout << "g ts " << parsedFromString["global_timestamp"] << endl;
								cout << "comp " << parsedFromString["dh_compression"] << endl;
							
								 cout << "sec " << parsedFromString["global_timestamp"]["sec"].asUInt() << endl;
							  cout << "nsec " << parsedFromString["global_timestamp"]["ns"].asUInt() << endl;
							
								
							}	
							else if (fastWriter.write(parsedFromString["htype"]).find("bsr_d-1.1") != std::string::npos) { 
						
						
						   
						
								cout << "chan " << parsedFromString["channels"] << endl;
								
							
							
									 for (Json::Value::ArrayIndex i=0; i < parsedFromString["channels"].size(); ++ i) {
							
									  cout << "name " << parsedFromString["channels"][i]["name"] << endl;
									  cout << "enco " << parsedFromString["channels"][i]["encoding"] << endl;
								    cout << "type " << parsedFromString["channels"][i]["type"] << endl;
									 }
								
							}	
							
							
							cout << "----------------------------------" << endl;
								
					}
				}
				
				
				
				
				union foo
				{
				char c[sizeof(double)];
				double d;
			
				} bar;
				
				
					
				if (subMessage > 0 && subMessage%2 ==0) {
					if (size==8) {
						
						// big endian	
						for (char_nbr = 0; char_nbr < size; char_nbr++) {
						 bar.c[char_nbr]=data[size-1-char_nbr];  // THis works for big engian
						}
						// little endian	
						//for (char_nbr = 0; char_nbr < size; char_nbr++) {
						// bar.c[char_nbr]=data[char_nbr];  
						//}
						
						
						cout << "UNION D " << bar.d << endl;
					
						
						double v; // = (double*) data;
						memcpy(&v, bar.c, sizeof(double));
							cout << "double val " << v << endl;
				
				
						
					}
					else if (size==2) {
						unsigned short i;
						//memcpy(&i, cmd, size);
							//big endian
							for (size_t n=0; n < size; n++) {
								i = (i << 8) + data[n];
							}
							//little endian
							//for (size_t n = size; n >= 0; n--) {
						 	//	i = (i << 8) + data[n];
							//}
							
							
							cout << "uint val " << i << endl;
							
					  /*
						std::copy(data, data + 32, reinterpret_cast<char *>(&i));
						cout << "uint16  val " << i << endl;
						std::copy(data, data + 8, reinterpret_cast<char *>(&i));
						cout << "uint16  val " << i << endl;
						std::copy(data, data + 4, reinterpret_cast<char *>(&i));
						cout << "uint16  val " << i << endl;
						std::copy(data, data + 2, reinterpret_cast<char *>(&i));
						cout << "uint16  val " << i << endl;
						std::copy(data, data + 1, reinterpret_cast<char *>(&i));
						cout << "uint16  val " << i << endl;
						
						*/
						
					}	
				}
				else if (subMessage > 1 && subMessage%2 ==1) {
					cout << "timestamp " << endl;
					unsigned int a,b;
					for (size_t n=0; n < size/2; n++) {
								a = (a << 8) + data[n];
					}
					for (size_t n=size/2; n < size; n++) {
								b = (b << 8) + data[n];
					}
					
					cout << "a " << a << " b " << b << endl;
				}
				
				++subMessage;
				
				} //ifelse
				
        int64_t more;           //  Multipart detection
        more = 0;
        size_t more_size = sizeof (more);
        zmq_getsockopt (socket, ZMQ_RCVMORE, &more, &more_size);
        zmq_msg_close (&message);
        if (!more) {
				  puts ("//END----------------------------------------//");
            break;      //  Last message part
				}
    } //while 1
		
		
}

//  Set simple random printable identity on socket
//
static void
s_set_id (void *socket)
{
    char identity [10];
    sprintf (identity, "%04X-%04X", randof (0x10000), randof (0x10000));
    zmq_setsockopt (socket, ZMQ_IDENTITY, identity, strlen (identity));
}

/*
//  Sleep for a number of milliseconds
static void
s_sleep (int msecs)
{
#if (defined (WIN32))
    Sleep (msecs);
#else
    struct timespec t;
    t.tv_sec  =  msecs / 1000;
    t.tv_nsec = (msecs % 1000) * 1000000;
    nanosleep (&t, NULL);
#endif
}
*/

//  Return current system clock as milliseconds
static int64_t
s_clock (void)
{
#if (defined (WIN32))
    SYSTEMTIME st;
    GetSystemTime (&st);
    return (int64_t) st.wSecond * 1000 + st.wMilliseconds;
#else
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return (int64_t) (tv.tv_sec * 1000 + tv.tv_usec / 1000);
#endif
}

//  Print formatted string to stdout, prefixed by date/time and
//  terminated with a newline.

/*
static void
s_console (const char *format, ...)
{
    time_t curtime = time (NULL);
    struct tm *loctime = localtime (&curtime);
    char *formatted = (char*)malloc (20);
    strftime (formatted, 20, "%y-%m-%d %H:%M:%S ", loctime);
    printf ("%s", formatted);
    free (formatted);

    va_list argptr;
    va_start (argptr, format);
    vprintf (format, argptr);
    va_end (argptr);
    printf ("\n");
}
*/
#endif  //  __ZHELPERS_H_INCLUDED__

#endif
