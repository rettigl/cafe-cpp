///
/// \file PVGroup.h
///
/// \author Jan Chrin
/// \date November 2014
///

#ifndef PVGROUP_H
#define PVGROUP_H


#include <PVDataHolder.h>


/**
 *   class MemberMap
 *   maps string to index 
 */
class MemberMap {
        typedef std::map<long, std::string> mapLongString;
private:
        mapLongString           mapNameIndex;
        mapLongString::iterator pos;
        Helper helper;

public:
    MemberMap(){};
   ~MemberMap(){};

void  insert(int  a, std::string _Name)   {
    mapNameIndex.insert(std::make_pair(a, _Name));
};

std::string getPV (int  i) {

    pos = mapNameIndex.find(i);
    if (pos != mapNameIndex.end()) return    pos->second;
    cout <<  "Index " << i << " not found! Size of group vector is " << mapNameIndex.size() << endl;
    return "";
};

int   getIndex (std::string _Name) {
    char pvStripped[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(_Name.c_str(), pvStripped);
    string Name=pvStripped;

    for (pos=mapNameIndex.begin(); pos != mapNameIndex.end(); ++pos) {

        if (pos->second==Name) {return pos->first;}
        // String searches such as s.find(s1) return string::npos on failure
        //else if ( (pos->second).find(Name)  != std::string::npos) return pos->first;
    }
    cout << "PV: " <<  Name << " IS NOT A MEMBER OF THIS LIST " << endl;
    return  -1;
};
};


/**
* class PVGroup
* This class is the holder of PVDataHolder objects associated with
* of group of handles
*/
class PVGroup {
    friend class Connect;
    friend class CAFE;
		//if HAVE_LIBQTXML
    friend class restorePVGroupXMLParser;
		//endif
private:
    PVDataHolder * pvdata;
    unsigned int   npv;
	  char name [PVGROUP_PSEUDO_SIZE];
    int     statusGroup;
    unsigned int  groupHandle;
    bool isIndexOutOfRange (unsigned int  idx) {
        return (idx >= npv) ? true:false;
    };
    MemberMap memberMap;

public:
     //Initialize 1st two to avoid compiler warning messages
	 PVGroup(){npv=0; pvdata=NULL; statusGroup=ICAFE_NORMAL; groupHandle=0; strcpy(name,"");};
    ~PVGroup(){};
    MemberMap getMemberMap() const {return memberMap;};
    PVDataHolder * getPVData() const {return pvdata;};
    PVDataHolder   getPVData(unsigned int  idx) throw(std::out_of_range){
        if(isIndexOutOfRange(idx)) {
            std::ostringstream oss;
            oss << "Exception! Index " << idx <<
                    " to PVGroup.pvdata() is out of range. Valid range is from 0 to " << npv-1;
            throw std::out_of_range(oss.str());
        }
        return pvdata[idx];
    };
	void setHasAlarm(bool h){ for (unsigned int i=0; i<npv; ++i) {pvdata[i].setHasAlarm(h);} }
	void setHasTS(bool h){ for (unsigned int i=0; i<npv; ++i) {pvdata[i].setHasTS(h);} }

    unsigned int    getNPV() const {return npv;};
    string getNameAsString() const {return name;};
    const char * getName()   const {return (const char *) name;};
    int  getStatusGroup()    const {return statusGroup;};
    int  getGroupStatus()    const {return statusGroup;};
    unsigned int  getGroupHandle() const {return groupHandle;};

    int  PV2Index(std::string _pv)     const {return getMemberMap().getIndex(_pv);}
    string Index2PV(unsigned int _idx) const {return getMemberMap().getPV(_idx);}

    void setPVData(PVDataHolder * _pvdata) {

        //We do not need to make a deep copy
        /*
        for (unsigned int i=0; i<npv; ++i) {
            pvdata[i].alarmStatus=_pvdata[i].alarmStatus;
            pvdata[i].alarmSeverity=_pvdata[i].alarmSeverity;
            pvdata[i].status=_pvdata[i].status;
            pvdata[i].nelem=_pvdata[i].nelem;
            pvdata[i].size=_pvdata[i].size;
            pvdata[i].dataType=_pvdata[i].dataType;
            pvdata[i].dataTypeNative=_pvdata[i].dataTypeNative;
            pvdata[i].rule=_pvdata[i].rule;
            pvdata[i].beamEventNo=_pvdata[i].beamEventNo;
            pvdata[i].userNo=_pvdata[i].userNo;
            pvdata[i].ts.nsec=_pvdata[i].ts.nsec;
            pvdata[i].ts.secPastEpoch=_pvdata[i].ts.secPastEpoch;
            strcpy(pvdata[i].pv,_pvdata[i].pv);
            strcpy(pvdata[i].pvAlias,_pvdata[i].pvAlias);
            strcpy(pvdata[i].device,_pvdata[i].device);
            strcpy(pvdata[i].attrib,_pvdata[i].attrib);
        }
        */

        //Shallow copy will do
        pvdata=_pvdata;
    };

    void set(unsigned int  _npv) {npv=_npv;};
    void setName(const char * _name) {strcpy(name, _name);};
    void setName(string _name) {strcpy(name,_name.c_str());};
    void setStatusGroup(int  _sg) {statusGroup=_sg;}


    void show() {print(npv) ;}
    void showMax(unsigned int  npvToPrint){print(npvToPrint);}
    void showMaxMax(unsigned int  npvToPrint, unsigned int  maxNelemWF){
        print(npvToPrint, maxNelemWF);}

    void print() {print(npv) ;}
    void print(unsigned int  npvToPrint) {
        npvToPrint=min(npvToPrint,npv);
        std::cout << "------------ PVGroup Handle = " << groupHandle << " ------------ "<< std::endl;
        std::cout << "PVGROUP: " << name << " HAS " << npv << " MEMBERS " << std::endl;
        std::cout << "PRINTING THE REQUESTED " << npvToPrint << " MEMBERS " << std::endl;
        std::cout << "OVERALL STATUS OF GROUP REPORTS " << statusGroup << std::endl;
        if (statusGroup!=ICAFE_NORMAL) {
            CAFEStatus cstat; cstat.report(statusGroup);
        }
        for (unsigned int  i=0; i<npvToPrint; ++i) {
            pvdata[i].print();
        }
    }
    void print(unsigned int  npvToPrint, unsigned int  maxNelemWF) {
        npvToPrint=min(npvToPrint,npv);
        std::cout << "------------ PVGroup Handle = " << groupHandle << " ------------ "<< std::endl;
        std::cout << "PVGROUP: " << name << " HAS " << npv << " MEMBERS " << std::endl;
        std::cout << "PRINTING THE REQUESTED " << npvToPrint << " MEMBERS " << std::endl;
        std::cout << "OVERALL STATUS OF GROUP REPORTS " << statusGroup << std::endl;
        if (statusGroup!=ICAFE_NORMAL) {
            CAFEStatus cstat; cstat.report(statusGroup);
        }
        for (unsigned int  i=0; i<npvToPrint; ++i) {
            pvdata[i].print(min(maxNelemWF,pvdata[i].getNelem()));
        }
    }

    void printIfError() {
        unsigned int  npvToPrint=npv;
        bool iErrorFound=false;
        for (unsigned int  i=0; i<npvToPrint; ++i) {
            if (pvdata[i].getStatus() != ICAFE_NORMAL) {
                if(!iErrorFound) {
                    std::cout << "PVGROUP: " << name << " HAS " << npv << " MEMBERS " << std::endl;
                    std::cout << "PRINTING PV TRANSACTIONS WITH ERRORS " << std::endl;
                    iErrorFound=true;
                }
                cout << "------------------------------------------------------------" << endl;
                cout << "Element [" << i << "] of " << npv << " in group: " << name << endl;
                pvdata[i].print();
            }
        }
    }
};



#endif //PVGROUP_H
