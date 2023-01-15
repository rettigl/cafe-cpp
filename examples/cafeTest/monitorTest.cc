//
//  A simple example
//
#include <cafe.h>
#include <fcntl.h>


/* Green States */
#define FEEDBACK_OFF          0
#define READY_FOR_EVENT       1
#define CHECKING_TUNE         2
#define RAMPING_UP            3
#define RAMPING_DOWN          4
#define TUNE_OK               5
#define IPCT_BELOW_THRESHOLD  6

/* Yellow States */
#define NO_TUNE_VALUE         7
#define BAD_TUNE_VALUE        8

/* Red States   */
#define CHANNELS_DISCONNECTED 9
#define ZERO_NOMINAL_TUNE    10
#define ZERO_DQ_MINIMUM      11
#define EMERGENCY_OFF        12

/* Define Anti_States with underscore*/
/* Green States */
#define _FEEDBACK_ON           100
#define _IPCT_ABOVE_THRESHOLD  106

/* Red State   */
#define _CHANNELS_CONNECTED    109
#define _NON_ZERO_NOMINAL_TUNE 110
#define _NON_ZERO_DQ_MINIMUM   111


void callbackHandlerMonitor( struct event_handler_args args)  {
#define __METHOD__  "callbackHandlerMonitor"

    if (args.status != ECA_NORMAL) {
        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        std::cout << "Status=" << args.status << " for channel " << ca_name (args.chid) << std::endl;
        //return;
    //}

    unsigned int _handle = (unsigned long) ca_puser(args.chid);// args.usr; // ca_puser(args.chid);

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find((unsigned int)_handle);

    if (it_handle != handle_index.end()) {
        //std::cout << (*it_handle).getPV() << " " << (*it_handle).getHandle() << std::endl;
        if(MUTEX){cafeMutex.lock();}
        handle_index.modify(it_handle, change_eventHandlerArgs   (args));
        if(MUTEX){cafeMutex.unlock();}


	//**** ADD USER CODE HERE ***//
		
	if (args.type < DBR_GR_STRING) {
            PVDataHolder pvd(args.count);
            (*it_handle).getPVDataHolder(pvd);
            pvd.print();
            //std::cout << "val/D//= " << pvd.getAsString(0) << std::endl;

	}
	else if (args.type < DBR_PUT_ACKT) {
	  PVCtrlHolder pvc(args.count);
	  (*it_handle).getPVCtrlHolder(pvc);
	  pvc.print();
	  //std::cout << "val/C/= " << pvc.getAsString(0) << std::endl;
	}
				 
	
	//****  END USER CODE ***// 
		
    }
    else {
        std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
        std::cout << " Internal CAFE ERROR! Unknown Handle! handle=" <<  _handle << std::endl;
    }

    }

return;
#undef __METHOD__
};



#define NHANDLES 40 
#define ALLHANDLES 72*4


int main( int argc, char *argv[] )
{
    using namespace std;

    int poutFlag = 0;

    char  message[40];
    char s[40];
    sprintf(message," OUTPUT FLAG == %d \n", poutFlag);

    time_t nowJCrhcp=time(NULL);
    char * theTime=ctime(&nowJCrhcp);	
    strftime(s,36,"/tmp/QX_%Y_%m_%d_%H:%M:%S",localtime(&nowJCrhcp));
    char * pmac = s;

    cout << s << " " << theTime << endl;
    cout << pmac << endl;

    open(pmac, O_WRONLY | O_CREAT, 0777); 
    FILE * ofpJCrhcp = fopen(pmac, "w"); 
    fprintf(ofpJCrhcp, "%s%s: STARTING Q_CHECK SEQUENCER \n", theTime, "QX");	
    fclose(ofpJCrhcp);

    char theMessage[40];
    strcpy(theMessage, "FEEDBACK ON\n");

    time_t now=time(NULL);
    theTime=ctime(&now);	
    FILE * ofp = fopen(pmac, "a"); 
   
    fprintf(ofp, "%s%s: %s",theTime, "QX", theMessage);
    fclose(ofp);		

   int code = READY_FOR_EVENT;
   switch(code) {

	case FEEDBACK_OFF: 
	  strcpy(theMessage, "FEEDBACK OFF\n");
	  break;

	case _FEEDBACK_ON: 
	  strcpy(theMessage, "FEEDBACK ON\n");
	  break;

	case READY_FOR_EVENT:
	  strcpy(theMessage, "READY FOR EVENT\n");
	  break;

	case RAMPING_UP:
	  strcpy(theMessage, "RAMPING UP\n");
	  break;

	case RAMPING_DOWN:
	  strcpy(theMessage, "RAMPING DOWN\n");
	  break;  
	
	case TUNE_OK:
	  strcpy(theMessage, "TUNE OK\n");
	  break;   
  
	case IPCT_BELOW_THRESHOLD:
	  strcpy(theMessage, "IPCT BELOW THRESHOLD\n");
	  break;    
	  
	case _IPCT_ABOVE_THRESHOLD:
	  strcpy(theMessage, "IPCT ABOVE THRESHOLD\n");
	  break;   

	case NO_TUNE_VALUE:
	  strcpy(theMessage, "NO TUNE VALUE\n");
	  break;

	case BAD_TUNE_VALUE:
	  strcpy(theMessage, "BAD TUNE VALUE: OUTSIDE ALLOWED RANGE\n");
	  break;

	case CHANNELS_DISCONNECTED:
	  strcpy(theMessage, "CHANNEL DISCONNECTED\n");
	  break;  
	
	case ZERO_NOMINAL_TUNE:
	  strcpy(theMessage, "ZERO NOMINAL TUNE\n");
	  break;   
  
	case _NON_ZERO_NOMINAL_TUNE:
	  strcpy(theMessage, "NOMINAL TUNE NOW NON-ZERO\n");
	  break;  

	case ZERO_DQ_MINIMUM:
	  strcpy(theMessage, "ZERO DQ MINIMUM\n");
	  break;   
  
	case _NON_ZERO_DQ_MINIMUM:
	  strcpy(theMessage, "DQ MINIMUM NOW NON-ZERO\n");
	  break;    

	case EMERGENCY_OFF:
	  strcpy(theMessage, "RAMPING DOWN\n");
	  break;   
	
	case _CHANNELS_CONNECTED:
	  strcpy(theMessage, "CHANNELS CONNECTED\n");
	  break; 

   }

  
  
   now=time(NULL);
   theTime=ctime(&now);	
   ofp = fopen(pmac, "a"); 
   
   fprintf(ofp, "%s%s: %s",theTime, "QX", theMessage);
   fclose(ofp);		



    unsigned int hArray [NHANDLES];

    string pvArray[NHANDLES]= {    
      "ARIDI-BPM-03SD:X-AVG", "ARIDI-BPM-03SD:Y-AVG", "ARIDI-BPM-03SD:INT-AVG", "ARIDI-BPM-03SD:GET-ENABLE",
      "ARIDI-BPM-06SB:X-AVG", "ARIDI-BPM-06SB:Y-AVG", "ARIDI-BPM-06SB:INT-AVG", "ARIDI-BPM-06SB:GET-ENABLE",
      "ARIDI-BPM-11MB:X-AVG", "ARIDI-BPM-11MB:Y-AVG", "ARIDI-BPM-11MB:INT-AVG", "ARIDI-BPM-11MB:GET-ENABLE",
      "ARIDI-BPM-09LB:X-AVG", "ARIDI-BPM-09LB:Y-AVG", "ARIDI-BPM-09LB:INT-AVG", "ARIDI-BPM-09LB:GET-ENABLE",
      "ARIDI-BPM-11SB:X-AVG", "ARIDI-BPM-11SB:Y-AVG", "ARIDI-BPM-11SB:INT-AVG", "ARIDI-BPM-11SB:GET-ENABLE",
      "ARIDI-BPM-10SB:X-AVG", "ARIDI-BPM-10SB:Y-AVG", "ARIDI-BPM-10SB:INT-AVG", "ARIDI-BPM-10SB:GET-ENABLE",
      "ARIDI-BPM-04LD:X-AVG", "ARIDI-BPM-04LB:Y-AVG", "ARIDI-BPM-04LD:INT-AVG", "ARIDI-BPM-04LB:GET-ENABLE",
      "ARIDI-BPM-09LD:X-AVG", "ARIDI-BPM-09LD:Y-AVG", "ARIDI-BPM-09LD:INT-AVG", "ARIDI-BPM-09LD:GET-ENABLE",
      "ARIDI-BPM-05SE:X-AVG", "ARIDI-BPM-05SE:Y-AVG", "ARIDI-BPM-05SE:INT-AVG", "ARIDI-BPM-05SE:GET-ENABLE",
      "ARIDI-BPM-09SD:X-AVG", "ARIDI-BPM-09SD:Y-AVG", "ARIDI-BPM-09SD:INT-AVG", "ARIDI-BPM-09SD:GET-ENABLE"
    };

     char bpmList[73][18]= {"ARIDI-BPM-01LB" , "ARIDI-BPM-01LE" , "ARIDI-BPM-01LD" , "ARIDI-BPM-01SD" , "ARIDI-BPM-01SE" , "ARIDI-BPM-01SB" , "ARIDI-BPM-02SB" , "ARIDI-BPM-02SE" , "ARIDI-BPM-02SD" , "ARIDI-BPM-02MD" , "ARIDI-BPM-02ME" , "ARIDI-BPM-02MB" , "ARIDI-BPM-03MB" , "ARIDI-BPM-03ME" , "ARIDI-BPM-03MD" , "ARIDI-BPM-03SD" , "ARIDI-BPM-03SE" , "ARIDI-BPM-03SB" , "ARIDI-BPM-04SB" , "ARIDI-BPM-04SE" , "ARIDI-BPM-04SD" , "ARIDI-BPM-04LD" , "ARIDI-BPM-04LE" , "ARIDI-BPM-04LB" , "ARIDI-BPM-05LB" , "ARIDI-BPM-05LE" , "ARIDI-BPM-05LD" , "ARIDI-BPM-05SD" , "ARIDI-BPM-05SE" , "ARIDI-BPM-05SB" , "ARIDI-BPM-06SB" , "ARIDI-BPM-06SE" , "ARIDI-BPM-06SD" , "ARIDI-BPM-06MD" , "ARIDI-BPM-06ME" , "ARIDI-BPM-06MB" , "ARIDI-BPM-07MB" , "ARIDI-BPM-07ME" , "ARIDI-BPM-07MD" , "ARIDI-BPM-07SD" , "ARIDI-BPM-07SE" , "ARIDI-BPM-07SB" , "ARIDI-BPM-08SB" , "ARIDI-BPM-08SE" , "ARIDI-BPM-08SD" , "ARIDI-BPM-08LD" , "ARIDI-BPM-08LE" , "ARIDI-BPM-08LB" , "ARIDI-BPM-09LB" , "ARIDI-BPM-09LE" , "ARIDI-BPM-09LD" , "ARIDI-BPM-09SD" , "ARIDI-BPM-09SE" , "ARIDI-BPM-09SB" , "ARIDI-BPM-10SB" , "ARIDI-BPM-10SE" , "ARIDI-BPM-10SD" , "ARIDI-BPM-10MD" , "ARIDI-BPM-10ME" , "ARIDI-BPM-10MB" , "ARIDI-BPM-11MB" , "ARIDI-BPM-11ME" , "ARIDI-BPM-11MD" , "ARIDI-BPM-11SD" , "ARIDI-BPM-11SE" , "ARIDI-BPM-11SB" , "ARIDI-BPM-12SB" , "ARIDI-BPM-12SE" , "ARIDI-BPM-12SD" , "ARIDI-BPM-12LD" , "ARIDI-BPM-12LE" , "ARIDI-BPM-12LB" , "ARIDI-BPM-MP" };

     string dev, att[4], devAtt;
     int bpmRingMax=73;

     att[1]="X-AVG"; att[2]="Y-AVG"; att[0]="INT-AVG"; att[3]="GET-ENABLE";  
     vector<string> pvArrayString;
     pvArrayString.reserve(72*4);
     vector<unsigned int> hArrayString;
     hArrayString.reserve(72*4);


     int ij=0;

     for (int i=0;  i<bpmRingMax-1 ; ++i) {  // max 72          
             dev=bpmList[i];
             for (int j=0; j<4; ++j) {	     
	       devAtt = dev +":" + att[j];  
	       pvArrayString.push_back(devAtt);
	     }
     }


    //Instantaite CAFE
    CAFE  * cafe = new CAFE();

    cafe->openPrepare();
    cafe->open(pvArrayString,  hArrayString); //, ALLHANDLES);
    cafe->openNowAndWait(4);  

    MonitorPolicy mp[ALLHANDLES];
    for (int i=0; i<ALLHANDLES; ++i) {
      mp[i].setUserArgs((void *) pvArrayString[i].c_str());
      mp[i].setHandler(callbackHandlerMonitor);
      mp[i].setCafeDbrType(CAFENUM::DBR_TIME);
      mp[i].setMask(DBE_VALUE | DBE_LOG | DBE_ALARM);
    }
   
    


    for (int i=0; i<ALLHANDLES; ++i) {
      cafe->monitorStart(hArrayString[i], mp[i]);
      cout << pvArrayString[i] << endl;
    }

    cafe->printDisconnectedHandles();

    while (true) {
      //for (int i=0; i<100; ++i) {
      usleep(10000);
    }

    for (int i=0; i<ALLHANDLES; ++i) {
      cafe->monitorStop(hArray[i], mp[i]);
    }
  
    cafe->closeHandles();

    cafe->terminate();
    return 0;


}
