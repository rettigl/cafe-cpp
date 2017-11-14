
#ifndef ZHELPERS_H
#define ZHELPERS_H


#include <zmq.h>

/*
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
*/

#include <assert.h>
#include <signal.h>


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


//  Receives all message parts from socket, prints neatly
//
static void
s_dump (DBPMKeeper & dbpm)
{

		if (inDumpFlag==1) {
			cout << "zeroMQ socket is busy " << endl;
			cout << "waiting for zmq timeout " << endl;
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
				//sleep(1);
		}

		int subMessage=0;		
		int nSequentialHeader=0;
		

    while (1) {
			  inDumpFlag=1;
			 				
			//puts ("//WHILE LOOP ----------------------------------------//");
			//cout << "subMessage " << subMessage << endl;
        //  Process all parts of the message
        zmq_msg_t message;
        zmq_msg_init (&message);
				
        size_t size = zmq_msg_recv (&message, socket, 0);
				//puts ("//MESSAGE RECEIVED     ----------------------------------------//");
				
				if (size == -1) {
					cout << " Error is " << zmq_errno() << " " << zmq_strerror(zmq_errno()) << endl;
					//Resource unavailable means that there is nothing to read now
					
					zmq_getsockopt (socket, ZMQ_RCVMORE, &more, &more_size);


					//cout << "message mulipart --> more == " << more << endl;


					 zmq_msg_close (&message);


					 //if (!more) {
					 //		   dbpm.status=ERRNO_EAGAIN; //ICAFE_ERRNO_BASE +zmq_errno() ;
					 //		   cout << "EARLY BREAK subMessage total: " << subMessage << endl;
					 //		   puts ("//------------------------------------END-------------------------------------//");
					 //		 break;      //  Last message
					 // }



					 dbpm.status=ICAFE_ERRNO_BASE+zmq_errno() ;
					 break;
				} 
				
				
				else if (size == 0) {

					////cout << " Data of Zero SIZE for submessage " << subMessage << endl;
			
					++nZeroSize;


					 //zmq_msg_close (&message);
					 //dbpm.status=ECAFE_NODATA;
					 //break;
					 //Comes in pairs; one for val one for timestamp
					 if (nZeroSize%2==1) {
					   ++bsPVIdx;


						if (bsPVIdx >0) {
						   cout << bsPV[bsPVIdx] << endl;
						}
					 }

					 ++subMessage;
					 
					 //continue;
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


		//TExt is two header files
		//non-text size=8 is X1, Y1, Q1
		//non-text size=2 is -VALID
		//non-text size=16 is timestamp

		/*
		printf (" SIZE [%03d] ", size);


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
							//Json::StyledWriter styledWriter;
							//cout << "STYLED: --------------------------------" << endl;
							//cout << styledWriter.write(parsedFromString) << endl;
							//cout << "----------------------------------" << endl;
							//cout << parsedFromString["htype"] << endl;
							
							
							if (fastWriter.write(parsedFromString["htype"]).find("bsr_m-1.1") != std::string::npos) { 

								++nSequentialHeader;

								hashIs=parsedFromString["hash"].asString();

								if (hashOriginalFlag==0) {
									cout << hashIs << " is different to original/// " << hashOriginal << endl;	
									hashOriginal=hashIs;
									++hashOriginalFlag;
									fillBSPV=true;
								}

								if (hashOriginal.compare(hashIs)!=0) {
									cout << hashIs << " is different to original " << hashOriginal << endl;	
									hashOriginal=hashIs;
									++hashOriginalFlag;
									fillBSPV=true;
																
								}
								 cout << "p id " << parsedFromString["pulse_id"].asUInt64() << endl;

								 dbpm.setPulse_id(parsedFromString["pulse_id"].asUInt64());

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
							
							++nSequentialHeader;
								
							 if  (fillBSPV) {
							
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
								}

							}	
							
							else {
							
							  cout << "HEADER IS SOMETHING ELSE: " << endl;
										cout << parsedFromString["htype"] << endl;
							
								exit(1);
							
							}
							
							
							cout << "/----------------------------------/" << endl;
								
					}
				}
				
				
				if (nSequentialHeader >3 && is_text==1) {
					 cout << "WARNING: ZEROMQ SUB-MESSAGE DOES NOT CLOSE " << endl; 
					 cout << "WARNING: FORCING ZMQ_MSG_CLOSE  " << endl;

					 cout << "No of sequential headers " << nSequentialHeader << endl;
					 cout << "is_text " << is_text << endl;
					 //zmq_getsockopt (socket, ZMQ_RCVMORE, &more, &more_size);
					 			 
					 //zmq_msg_close (&message);
					 //cout << "message mulipart --> more == " << more << endl;
					 	

					 //nSequentialHeader=0;
					 //cannot break; results in seqmentation fault

					 //if (!more) {
					//		   dbpm.status=ERRNO_EAGAIN; //ICAFE_ERRNO_BASE +zmq_errno() ;
					//		   cout << "EARLY BREAK subMessage total: " << subMessage << endl;
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

					dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].setBSStatus(ICAFE_NORMAL);

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


								dbpm.pvd[dbpm.getPVIdx(bsPV[bsPVIdx])].setBSStatus(ICAFE_NORMAL);
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
				
		//cout << "value of more " << more << endl;
				
        if (!more) {
				  dbpm.status=ICAFE_NORMAL;
				  cout << "subMessage total: " << subMessage << endl;
				  cout << " of which 0 size: " << nZeroSize << endl;
				  cout << " Percentage good: " << (subMessage-nZeroSize-2)*100/(subMessage-2);
				  puts ("//------------------------------------END-------------------------------------//");
					 
            break;      //  Last message part
				}
    } //while 1
		
		inDumpFlag=0;
	
		if (subMessage>2) {
			dbpm.status=ICAFE_NORMAL;
		cout << "subMessage total: " << subMessage << endl;
		cout << " of which 0 size: " << nZeroSize << endl;
		cout << " Percentage good: " << (subMessage-nZeroSize-2)*100/(subMessage-2);
		puts ("//------------------------------------END-------------------------------------//");
		}


		cout << "end of loop " << endl;
		
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



#endif  //  ZHELPERS_H
