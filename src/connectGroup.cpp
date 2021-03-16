///
/// \file    connectGroup.cpp
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///
#include <connect.h>
#include <conduitFriends.h>

using namespace std;


template<typename A, typename B>
std::pair<B,A> flip_pair(const std::pair<A,B> &p)
{
    return std::pair<B,A>(p.second, p.first);
}


template <typename A, typename B>
std::multimap<B,A> flip_map(const std::map<A,B> & src)
{

    std::multimap<B,A> dst;

    std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()),
                   flip_pair<A,B>);

    return dst;
}


/////////////////////////  Group functions   /////////////////////////////////////////////////

/**
 * \brief Connect::collectionDefine: Add collection to global vector, deviceCollectionV
 *
 * \param collectionName input: define collection name
 * \param deviceListV input: vector list of strings that comprise the collection of devices
 * \return ICAFE_NORMAL
 */
int Connect::collectionDefine(const char * collectionName, std::vector<std::string> deviceListV)
{
    bool collectionExists = false;
    for (unsigned short i = 0; i < deviceCollectionV.size(); ++i)
    {
        if (strcmp(deviceCollectionV[i].name.c_str(), collectionName) == 0)
        {
            cout << "COLLECTION " << collectionName << " ALREADY EXISTS " << endl;
            collectionExists = true;
            break;
        }
    }
    if (collectionExists)
    {
        return ECAFE_COLLECTION_PREV_DEF;
    }
    deviceCollection dC;
    collectionMember cM;
    dC.name=collectionName;

    dC.cMembers.reserve(deviceListV.size());

    for (size_t i=0; i<deviceListV.size(); ++i)
    {
        cM.deviceName=deviceListV[i];
        dC.cMembers.push_back(cM);
    }
    deviceCollectionV.push_back(dC);
    return ICAFE_NORMAL;
}


/**
 * \brief Connect::collectionDefine: Add collection to global vector, deviceCollectionV
 *
 * \param collectionName input: define collection name
 * \param deviceListV input: vector list of const char * that comprise the collection of devices
 * \return ICAFE_NORMAL
 */
int Connect::collectionDefine(const char * collectionName, std::vector<const char *> deviceListV)
{
    bool collectionExists = false;
    for (unsigned short i = 0; i < deviceCollectionV.size(); ++i)
    {
        if (strcmp(deviceCollectionV[i].name.c_str(), collectionName) == 0)
        {
            cout << "COLLECTION " << collectionName << " ALREADY EXISTS " << endl;
            collectionExists = true;
            break;
        }
    }
    if (collectionExists)
    {
        return ECAFE_COLLECTION_PREV_DEF;
    }
    deviceCollection dC;
    collectionMember cM;
    dC.name=collectionName;

    dC.cMembers.reserve(deviceListV.size());

    for (size_t i=0; i<deviceListV.size(); ++i)
    {
        cM.deviceName=deviceListV[i];
        dC.cMembers.push_back(cM);
    }
    deviceCollectionV.push_back(dC);
    return ICAFE_NORMAL;
}

/**
 * \brief Connect::collectionDefine: Add collection to global vector, deviceCollectionV
 *
 * \param collectionName input: define collection name
 * \param deviceList input: array of pv_string_t that comprises the collection of devices
 * \param deviceListLength input: size of deviceList array
 * \return ICAFE_NORMAL
 */
int Connect::collectionDefine(const char * collectionName, pv_string_t * deviceList, unsigned int deviceListLength)
{
    bool collectionExists = false;
    for (unsigned short i = 0; i < deviceCollectionV.size(); ++i)
    {
        if (strcmp(deviceCollectionV[i].name.c_str(), collectionName) == 0)
        {
            cout << "COLLECTION " << collectionName << " ALREADY EXISTS " << endl;
            collectionExists = true;
            break;
        }
    }
    if (collectionExists)
    {
        return ECAFE_COLLECTION_PREV_DEF;
    }
    deviceCollection dC;
    collectionMember cM;
    dC.name=collectionName;

    dC.cMembers.reserve(deviceListLength);
    for (unsigned int i = 0; i < deviceListLength; ++i)
    {

        cM.deviceName=deviceList[i];
        dC.cMembers.push_back(cM);
    }
    deviceCollectionV.push_back(dC);
    return ICAFE_NORMAL;
}


/**
 * \brief Connect::collectionFetch  - retrieves Collection
 *
 * \param collectionName input: collection name
 * \param dC ouput: deviceCollection class
 * \return ICAFE_NORMAL else ECAFE_UNKNOWN_COLLECTION
 */
int Connect::collectionFetch(const char * collectionName, deviceCollection &dC)
{

    char _collectionName[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(collectionName, _collectionName);

    for (unsigned short i = 0; i < deviceCollectionV.size(); ++i)
    {
        if (strcmp(deviceCollectionV[i].name.c_str(), _collectionName) == 0)
        {
            dC= deviceCollectionV[i];
            return ICAFE_NORMAL;
        }
    }
    return ECAFE_UNKNOWN_COLLECTION;
}


/**
 * \brief Connect::collectionFetch  - retrieves Collection
 *
 * \param collectionName input: collection name
 * \param deviceListV ouput: vector of strings containing collection members
 * \return ICAFE_NORMAL else ECAFE_UNKNOWN_COLLECTION, ECAFE_EMPTY_COLLECTION
 */
int Connect::collectionFetch(const char * collectionName, std::vector<std::string> &deviceListV)
{

    char _collectionName[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(collectionName, _collectionName);

    deviceListV.clear();
    for (unsigned short i = 0; i < deviceCollectionV.size(); ++i)
    {
        if (strcmp(deviceCollectionV[i].name.c_str(), _collectionName) == 0)
        {
            deviceListV.reserve(deviceCollectionV[i].getMembers().size());
            deviceListV= deviceCollectionV[i].getMembers();

            if (deviceListV.empty())
            {
                return ECAFE_EMPTY_COLLECTION;
            }
            else
            {
                return ICAFE_NORMAL;
            }
        }
    }
    return ECAFE_UNKNOWN_COLLECTION;
}

/**
 * \brief Connect::collectionFetch  - retrieves Collection
 *
 * \param collectionName input: collection name
 * \param deviceListV ouput: vector of const char * containing collection members
 * \return ICAFE_NORMAL else ECAFE_UNKNOWN_COLLECTION, ECAFE_EMPTY_COLLECTION
 */
int Connect::collectionFetch(const char * collectionName, std::vector<const char *> &deviceListV)
{

    char _collectionName[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(collectionName, _collectionName);

    deviceListV.clear();
    for (unsigned short i = 0; i < deviceCollectionV.size(); ++i)
    {
        if (strcmp(deviceCollectionV[i].name.c_str(), _collectionName) == 0)
        {
            deviceListV.reserve(deviceCollectionV[i].getMembers().size());
            //copy string vector to cont char * vector
            for (unsigned short j=0; j< deviceCollectionV[i].getMembers().size(); ++j)
            {
                deviceListV.push_back((const char *) deviceCollectionV[i].getMembers()[j].c_str());
            }


            if (deviceListV.empty())
            {
                return ECAFE_EMPTY_COLLECTION;
            }
            else
            {
                return ICAFE_NORMAL;
            }
        }
    }
    return ECAFE_UNKNOWN_COLLECTION;
}

/**
 * \brief Connect::isCollection  - Test const char * to determine if input 'name' is a collection
 * \param collectionID input: collection name
 * \return trool or false
 */
bool Connect::isCollection(const char * collectionID)
{
    char _collectionID[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(collectionID, _collectionID);

    for (unsigned short i = 0; i < deviceCollectionV.size(); ++i)
    {
        //cout << deviceCollectionV[i].name.c_str() << " [" << i << "] " << endl;
        if (strcmp(deviceCollectionV[i].name.c_str(), (const char *) _collectionID) == 0)
        {
            return true;
        }
    }
    return false;
}

/**
 * \brief Connect::isGroup  - Test const char * to determine if input 'name' is a garoup
 * \param groupID input: group name
 * \return true or false
 */
bool Connect::isGroup(const char * groupID)
{
    char _groupID[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(groupID, _groupID);
    for (size_t i = 0; i < PVGroupV.size(); ++i)
    {
        if (strcmp(PVGroupV[i].name, (const char *) _groupID) == 0)
        {
            return true;
        }
    }
    return false;
}

/**
 * \brief Connect::groupCombine  - Combines individual groups to form a new group
 * \param newGroupName input: new group name
 * \param groupNameV input: vector of groups
 * \return ICAFE_NORMAL is all OK else, ECAFE_GROUP_PREV_DEF, ECAFE_UNKNOWN_GROUP
 */
int Connect::groupCombine(const char * newGroupName, std::vector<char *> groupNameV)
{

    char _newGroupName[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(newGroupName, _newGroupName);


    for (unsigned short j = 0; j < groupNameV.size(); ++j)
    {
        char _groupName[PVNAME_SIZE];

        helper.removeLeadingAndTrailingSpaces(groupNameV[j], _groupName);
        //overwrite vector
        groupNameV[j]= _groupName;
    }

    bool groupExists  = false;
    unsigned short nExisting=0;
    bool inputGroupExists=false;
    unsigned short * gIndex = new unsigned short[groupNameV.size()];
    unsigned int nElemInGroup=0;


    for (unsigned short i = 0; i < PVGroupV.size(); ++i)
    {
        if (strcmp(PVGroupV[i].name, _newGroupName ) == 0)
        {
            cout << "REQUESTED NAME FOR GROUP " << _newGroupName << " ALREADY EXISTS!  GROUP COMBINE FAILED" << endl;
            groupExists  = true;
            continue;
        }
    }



    for (unsigned short j = 0; j < groupNameV.size(); ++j)
    {
        inputGroupExists=false;
        for (unsigned short i = 0; i < PVGroupV.size(); ++i)
        {
            if (strcmp(PVGroupV[i].name, groupNameV[j] ) == 0)
            {
                gIndex[j]=i;
                inputGroupExists=true;
                ++nExisting;
                nElemInGroup=  nElemInGroup + PVGroupV[i].npv;
                break;
            }
        }

        if (!inputGroupExists)
        {
            cout << "REQUESTED INPUT GROUP " <<  groupNameV[j]  << " DOES NOT EXIST! GROUP COMBINE FAILED" << endl;
        }
    }

    if (groupExists)
    {
        delete []gIndex;
        return ECAFE_GROUP_PREV_DEF;
    }
    if (nExisting != groupNameV.size())
    {
        delete []gIndex;
        return ECAFE_UNKNOWN_GROUP;
    }

    PVGroup pvGroup;
    pvGroup.npv =  nElemInGroup;
    strcpy(pvGroup.name, _newGroupName);
    PVDataHolder * pvdata = new PVDataHolder[pvGroup.npv];


    unsigned int icount=0;
    for (unsigned short j = 0; j < groupNameV.size(); ++j)
    {
        for (unsigned int i = 0; i < PVGroupV[gIndex[j]].npv; ++i)
        {
            pvdata[icount]=PVGroupV[gIndex[j]].pvdata[i];
            pvGroup.memberMap.insert(icount,pvdata[icount].pv);
            ++icount;
        }
    }
    pvGroup.pvdata = pvdata;
    PVGroupV.push_back(pvGroup);

    delete [] gIndex;
    return ICAFE_NORMAL;
}



/**
 * \brief Connect::groupCombine  - Combines two groups to form a new group
 * \param newGroupName input: new group name
 * \param groupName1 input: already existing group
 * \param groupName2 input: already existing group
 * \return ICAFE_NORMAL is all OK else, ECAFE_GROUP_PREV_DEF, ECAFE_UNKNOWN_GROUP
 */
int Connect::groupCombine(const char * newGroupName, const char * groupName1,
                          const char * groupName2)   // PVGroup &pvGroup) {
{

    char _newGroupName[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(newGroupName, _newGroupName);
    char _groupName1[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(groupName1, _groupName1);
    char _groupName2[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(groupName2, _groupName2);

    bool groupExists  = false;
    bool groupExists1 = false;
    bool groupExists2 = false;
    PVGroup g1;
    PVGroup g2;

    for (unsigned short i = 0; i < PVGroupV.size(); ++i)
    {
        if (strcmp(PVGroupV[i].name, _newGroupName ) == 0)
        {
            cout << "REQUESTED NAME FOR GROUP " << _newGroupName << " ALREADY EXISTS " << endl;
            groupExists  = true;
        }
        if (strcmp(PVGroupV[i].name, _groupName1 ) == 0)
        {
            g1=PVGroupV[i];
            groupExists1 = true;
        }
        if (strcmp(PVGroupV[i].name, _groupName2 ) == 0)
        {
            g2=PVGroupV[i];
            groupExists2 = true;
        }
        if (groupExists && groupExists1 && groupExists2)
        {
            break;
        }
    }

    if (!groupExists1)
    {
        cout <<  "GROUP " << _groupName1 << " DOES NOT EXIST! GROUP COMBINE FAILED" << endl;
    }
    if (!groupExists2)
    {
        cout <<  "GROUP " << _groupName2 << " DOES NOT EXIST! GROUP COMBINE FAILED" << endl;
    }
    if (groupExists)
    {
        return ECAFE_GROUP_PREV_DEF;
    }
    else if (!groupExists1 || !groupExists2)
    {
        return ECAFE_UNKNOWN_GROUP;
    }

    unsigned int nElemInGroup=g1.npv + g2.npv;

    PVGroup pvGroup;
    pvGroup.npv =  nElemInGroup;
    strcpy(pvGroup.name, _newGroupName);
    PVDataHolder * pvdata = new PVDataHolder[pvGroup.npv];

    for (unsigned int i=0; i<g1.npv; ++i )
    {
        pvdata[i] = g1.pvdata[i];
        pvGroup.memberMap.insert(i,pvdata[i].pv);
    }

    unsigned int icount=0;
    for (unsigned int i=g1.npv; i<(g1.npv+g2.npv); ++i )
    {
        pvdata[i]=  g2.pvdata[icount];
        pvGroup.memberMap.insert(i,pvdata[i].pv);
        ++icount;
    }

    pvGroup.pvdata = pvdata;
    PVGroupV.push_back(pvGroup);

    return ICAFE_NORMAL;
}



/**
 * \brief Connect::groupDefine
 * \param groupName input: new group name
 * \param collectionName input: name of already existing collection
 * \param attributeV input: vector of attributes (string)
 * \return ICAFE_NORMAL is all OK else, ECAFE_GROUP_PREV_DEF, ECAFE_UNKNOWN_COLLECTION, ECAFE_EMPTY_COLLECTION
 */
int Connect::groupDefine (const char * groupName, const char * collectionName, std::vector<std::string> attributeV)
{
    vector<string> deviceListV;
    int localStatus = collectionFetch(collectionName, deviceListV);
    if (localStatus != ICAFE_NORMAL)
    {
        return localStatus;
    }
    return groupDefine(groupName, deviceListV, attributeV);
}


/**
 * \brief Connect::groupDefine
 * \param groupName input: new group name
 * \param collectionName input: name of already existing collection
 * \param attributeV input: vector of attributes (const char *)
 * \return ICAFE_NORMAL is all OK else, ECAFE_GROUP_PREV_DEF, ECAFE_UNKNOWN_COLLECTION, ECAFE_EMPTY_COLLECTION
 */
int Connect::groupDefine (const char * groupName, const char * collectionName, std::vector<const char*> attributeV)
{
    vector<const char *> deviceListV;
    int localStatus = collectionFetch(collectionName, deviceListV);
    if (localStatus != ICAFE_NORMAL)
    {
        return localStatus;
    }
    return groupDefine(groupName, deviceListV, attributeV);
}


/**
 * \brief Connect::groupDefine
 * \param groupName input: new group name
 * \param collectionName input: name of already existing collection
 * \param attributeArray input: array of attributes (pv_string_t *)
 * \param attributeLength input: size of input array
 * \return ICAFE_NORMAL is all OK else, ECAFE_GROUP_PREV_DEF, ECAFE_UNKNOWN_COLLECTION, ECAFE_EMPTY_COLLECTION
 */
int Connect::groupDefine (const char * groupName, const char * collectionName,
                          pv_string_t * attributeArray, unsigned short attributeLength)
{

    vector<string> deviceListV;
    vector<string> attributeListV;
    attributeListV.reserve(attributeLength);
    int localStatus = collectionFetch(collectionName, deviceListV);
    if (localStatus != ICAFE_NORMAL)
    {
        return localStatus;
    }
    attributeListV.insert(attributeListV.end(), &attributeArray[0], &attributeArray[attributeLength]);
    return groupDefine(groupName, deviceListV, attributeListV);
}


/**
 * \brief Connect::groupDefine
 * \param groupName input: new group name
 * \param deviceArray input: array of devices (pv_string_t)
 * \param nDevice input : size of device array
 * \param attributeArray input: array of attributes (pv_string_t *)
 * \param nAttribute input: size of attribute array
 * \return ICAFE_NORMAL is all OK else, ECAFE_GROUP_PREV_DEF
 */
int Connect::groupDefine(const char * groupName, pv_string_t * deviceArray, unsigned int nDevice,
                         pv_string_t * attributeArray, unsigned short nAttribute)   // PVGroup &pvGroup) {
{
    std::string s="";
    //std::string deviceAttributeDeliminator=DEFAULT_DEVICE_ATTRIBUTE_DELIMINATOR;
    unsigned int nElemInGroup=nDevice * nAttribute;
    pv_string_t * pvArray = new pv_string_t [nElemInGroup];
    unsigned int icount=0;
    for (unsigned short j=0; j<nAttribute; ++j )
    {
        for (unsigned int i=0; i<nDevice; ++i )
        {
            s=deviceArray[i] +  deviceAttributeDeliminator + attributeArray[j];
            strcpy(pvArray[icount], s.c_str());
            ++icount;
        }
    }
    int status = groupDefine(groupName, pvArray, nElemInGroup); // pvGroup);

    delete []pvArray;
    return status;

}
/**
 * \brief Connect::groupDefine
 * \param groupName input: new group name
 * \param deviceV input: vector of devices (const char *)
 * \param attributeV input: vector of attributes (const char *)
 * \return ICAFE_NORMAL is all OK else, ECAFE_GROUP_PREV_DEF
 */
int Connect::groupDefine(const char * groupName, std::vector<const char *> deviceV,
                         std::vector<const char *> attributeV)  // PVGroup &pvGroup) {
{

    std::string s="";
    //std::string deviceAttributeDeliminator=DEFAULT_DEVICE_ATTRIBUTE_DELIMINATOR;
    unsigned int nElemInGroup=deviceV.size() * attributeV.size();
    pv_string_t * pvArray = new pv_string_t [nElemInGroup];
    unsigned int icount=0;
    for (unsigned short j=0; j<attributeV.size(); ++j )
    {
        for (unsigned int i=0; i<deviceV.size(); ++i )
        {
            s=deviceV[i] +  deviceAttributeDeliminator + attributeV[j];
            strcpy(pvArray[icount], s.c_str());
            ++icount;
        }
    }
    int status = groupDefine(groupName, pvArray, nElemInGroup); // pvGroup);
    delete []pvArray;
    return status;
}

/**
 * \brief Connect::groupDefine
 * \param groupName input: new group name
 * \param deviceV input: vector of devices (string)
 * \param attributeV input: vector of attributes (strig)
 * \return ICAFE_NORMAL is all OK else, ECAFE_GROUP_PREV_DEF
 */
int Connect::groupDefine(const char * groupName, std::vector<std::string> deviceV, std::vector<std::string> attributeV)
{
    //PVGroup &pvGroup) {
    std::string s="";
    //std::string deviceAttributeDeliminator=DEFAULT_DEVICE_ATTRIBUTE_DELIMINATOR;
    unsigned int nElemInGroup=deviceV.size() * attributeV.size();
    pv_string_t * pvArray = new pv_string_t [nElemInGroup];
    unsigned int icount=0;
    for (unsigned short j=0; j<attributeV.size(); ++j )
    {
        for (unsigned int i=0; i<deviceV.size(); ++i )
        {
            s=deviceV[i] +  deviceAttributeDeliminator + attributeV[j];
            strcpy(pvArray[icount], s.c_str());
            ++icount;
        }
    }
    int status = groupDefine(groupName, pvArray, nElemInGroup); // pvGroup);
    delete []pvArray;
    return status;
}

/**
 * \brief Connect::groupDefine
 * \param groupName input: new group name
 * \param pvArrayV input: vector of process varaibles (string)
 * \return ICAFE_NORMAL is all OK else, ECAFE_GROUP_PREV_DEF
 */
int Connect::groupDefine(const char * groupName, std::vector<std::string> pvArrayV)   // PVGroup &pvGroup) {
{

    pv_string_t * pvArray = new pv_string_t [pvArrayV.size()];

    for (unsigned int i=0; i<pvArrayV.size(); ++i )
    {
        strcpy(pvArray[i], pvArrayV[i].c_str());
    }
    int status = groupDefine(groupName, pvArray,  pvArrayV.size()); // pvGroup);
    delete []pvArray;
    return status;
}

/**
 * \brief Connect::groupDefine
 * \param groupName input: new group name
 * \param pvArrayV input: vector of process varaibles (const char *)
 * \return ICAFE_NORMAL is all OK else, ECAFE_GROUP_PREV_DEF
 */
int Connect::groupDefine(const char * groupName, std::vector<const char *> pvArrayV)
{

    pv_string_t * pvArray = new  pv_string_t [pvArrayV.size()];

    for (unsigned int i=0; i<pvArrayV.size(); ++i )
    {
        strcpy(pvArray[i], pvArrayV[i]);

    }

    int status = groupDefine(groupName, pvArray,  pvArrayV.size()); // pvGroup);



    delete []pvArray;
    return status;

}

/**
 * \brief Connect::groupDefine
 * \param groupName input: new group name
 * \param pvArray input: array of process variables (pv_string_t)
 * \param pvArrayLength: length of pvArray
 * \return ICAFE_NORMAL is all OK else, ECAFE_GROUP_PREV_DEF
 */
int Connect::groupDefine(const char * groupName, pv_string_t * pvArray,
                         unsigned int pvArrayLength)
{

    bool groupExists = false;
    char _groupName[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(groupName, _groupName);


    for (unsigned short i = 0; i < PVGroupV.size(); ++i)
    {
        if (strcmp(PVGroupV[i].name, _groupName) == 0)
        {
            cout << "GROUP " << groupName << " ALREADY EXISTS " << endl;
            groupExists = true;
            break;
        }
    }

    if (groupExists)
    {
        return ECAFE_GROUP_PREV_DEF;
    }

    PVGroup pvGroup;
    strcpy(pvGroup.name, _groupName);

    pvGroup.npv = pvArrayLength;
    PVDataHolder * pvdata = new PVDataHolder[pvGroup.npv];

    for (unsigned int i = 0; i < pvGroup.npv; ++i)
    {
        if (strcmp(pvArray[i],"")==0)
        {
            cout << " WARNING:: PV HAS NO NAME!! " << "Element " << i << " of " << pvGroup.npv << endl;
        }
        //Fill PVDataUnit here

        char _pv[PVNAME_SIZE];
        helper.removeLeadingAndTrailingSpaces(pvArray[i], _pv);


        strcpy(pvdata[i].pv,  _pv);



        ChannelDeviceAttribute channelDeviceAttribute;

        channelDeviceAttribute.init(pvdata[i].pv,deviceAttributeDeliminator);

        strcpy(pvdata[i].device, channelDeviceAttribute.getDevice());
        strcpy(pvdata[i].attrib, channelDeviceAttribute.getAttribute());

        pvGroup.memberMap.insert(i,pvdata[i].pv);


    }

    pvGroup.pvdata = pvdata;
    PVGroupV.push_back(pvGroup);

    return ICAFE_NORMAL;
}




vector<std::string> Connect::getFromGlobalChannelList(std::vector<std::string> searchList)
{

    vector<string> retChannelList;

    retChannelList.clear();
    retChannelList.reserve(200);

    //cout << globalChannelList.size() << endl;
    //cout << searchList.size() << endl;

    for (unsigned int i=0;   i < globalChannelList.size(); ++i)
    {
        unsigned int ncount=0;
        for (unsigned int j=0;   j < searchList.size(); ++j )
        {
            std::size_t found = globalChannelList[i].find(searchList[j]);
            if (found!=std::string::npos)
            {
                ++ncount;
            }
        }
        //if (ncount > 0) cout << ncount << " " <<  searchList.size() << endl;
        if (ncount==searchList.size())
        {
            retChannelList.push_back(globalChannelList[i]);
        }
    }

    //cout << retChannelList.size() << endl;

    return retChannelList;
}


/**
 * \brief Creates a pseudo group from input handles and initalizes PVdataHolder array; \
method to be preceded by getV(v) and followed by e.g.getCachePVArrayNoWait(v, pvd)
 * \param handleArray input: array of handles
 * \return PVDataHolder *
 */
PVDataHolder * Connect::getPVData(std::vector<unsigned int> handleArray)
{
#define __METHOD__ "CAFE::getPVData(vector<unsigned int> handleArray)"

    // Check in vector already exists!
    // showing contents:
    bool  groupExists = false;
    char _groupName[PVGROUP_PSEUDO_SIZE];
    std::map<vector<unsigned int>,string>::iterator it = groupPseudoMap.begin();
    //std::cout << "groupPseudoMap contains:\n";
    for (it=groupPseudoMap.begin(); it!=groupPseudoMap.end(); ++it)
    {
        //std::cout << (it->first)[0] << " => " << it->second << '\n';
        if (it->first==handleArray)
        {
            //cout << "MATCH FOUND GROUP EXISTS " << endl;
            strcpy(_groupName,(it->second).c_str());
            groupExists=true;
            for (unsigned short i = 0; i < PVGroupPseudo.size(); ++i)
            {
                if (strcmp(PVGroupPseudo[i].name, _groupName) == 0)
                {
                    //cout << "GROUP " << groupName << " ALREADY EXISTS " << endl;
                    return PVGroupPseudo[i].getPVData();
                }
            }
            cout << __FILE__ << "//" << __METHOD__ << endl;
            cout << "WARNING:: GROUP " << (it->second) << " NOT FOUND IN PVGROUPPSEUDO " << endl;
            cout << "THIS WARNING SHOULD NOT APPEAR! " << endl;
            break;
        }
    }

    status=ICAFE_NORMAL;
    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    std::stringstream converter;
    std::stringstream converter1;
    //std::stringstream converter2;
    string sgn="g";
    converter << handleArray.size();
    sgn.append(converter.str());
    sgn.append("h");
    converter1 << handleArray[0];


    unsigned int nseq=0;
    for (unsigned int i=1; i<handleArray.size()-1; ++i)
    {

        if(handleArray[i] != handleArray[i-1]+1)
        {
            if(nseq>0)
            {
                converter1 << "-";
                converter1 << handleArray[i-1];
                converter1 << "/";
                converter1 << handleArray[i];
            }
            else
            {
                converter1 << "/";
                converter1 << handleArray[i];
            }
            nseq=0;
        }
        else
        {
            nseq=nseq+1;
        }
    }

    if (nseq==0)
    {
        converter1 << "/";
    }
    else
    {
        converter1 << "-";
    }
    converter1 << handleArray[handleArray.size()-1];

    unsigned int minVal=min(converter1.str().size(),(size_t) (PVGROUP_PSEUDO_SIZE-sgn.size()-1));
    sgn.append(converter1.str().substr(0,minVal));

    //Example of GroupName
    //g22h1-3/5/7/9-12/16/19/21/23/25/28/4/6/11-12/33/31/2
    //g164h92-229/212-217/230-249


    const char * groupName=sgn.c_str();
    helper.removeLeadingAndTrailingSpacesPseudo(groupName, _groupName);

    for (unsigned short i = 0; i < PVGroupPseudo.size(); ++i)
    {
        if (strcmp(PVGroupPseudo[i].name, _groupName) == 0)
        {
            //cout << "GROUP " << groupName << " ALREADY EXISTS " << endl;
            groupExists = true;
            return PVGroupPseudo[i].getPVData();
        }
    }


    PVGroup pvGroup;
    strcpy(pvGroup.name, _groupName);

    pvGroup.npv = handleArray.size();
    PVDataHolder * pvdata = new PVDataHolder[pvGroup.npv];

    for (unsigned int iMember = 0; iMember < pvGroup.npv; ++iMember)
    {
        bool hf=false;
        //Fill PVDataUnit here
        //char _pv[PVNAME_SIZE];
        //helper.removeLeadingAndTrailingSpaces(pvArray[i], _pv);

        it_handle = handle_index.find(handleArray[iMember]);

        if (it_handle != handle_index.end())
        {
            hf=true;
        }


        if (!hf)
        {
            pvdata[iMember].status=ECAFE_INVALID_HANDLE;
            if(printErrorPolicy.getInvalidHandle())
            {
                cout << "WARNING: " << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cafeStatus.report(ECAFE_INVALID_HANDLE);
                cout <<  "Invalid handle = " << handleArray[iMember]
                     << " for member [" << iMember << "] " << endl;
            }
            continue;
        }

        pvdata[iMember].alarmStatus=-1;
        pvdata[iMember].alarmSeverity=-1;
        pvdata[iMember].ts.secPastEpoch=0;
        pvdata[iMember].ts.nsec=0;
        pvdata[iMember].status=ICAFE_NORMAL;

        strcpy(pvdata[iMember].pv, ((*it_handle).pv).c_str());
        //this sets size and nelem to 1 if not connected
        pvdata[iMember].setNelem( (*it_handle).getChannelRegalia().getNelem() );

        //set Device, Attribute for handle

        (*it_handle).getPVDataHolder(pvdata[iMember]);

        strcpy(pvdata[iMember].pv,  handleHelper.getPVFromHandle(handleArray[iMember]));

        ChannelDeviceAttribute channelDeviceAttribute;
        channelDeviceAttribute.init(pvdata[iMember].pv,deviceAttributeDeliminator);

        strcpy(pvdata[iMember].device, channelDeviceAttribute.getDevice());
        strcpy(pvdata[iMember].attrib, channelDeviceAttribute.getAttribute());

        pvGroup.memberMap.insert(iMember,pvdata[iMember].pv);

    }


    pvGroup.pvdata = pvdata;
    PVGroupPseudo.push_back(pvGroup);

    groupPseudoMap.insert( std::pair<vector<unsigned int>,string>(handleArray,sgn) );

    return pvGroup.getPVData();
#undef __METHOD__
}




/**
 * \brief Connect::groupList - lists all groups in global vector PVGroupV \n
 * \param glist output: shared_ptr to array of group names (pv_string_t)
 * \param listLength: length of glist array
 * \return ICAFE_NORMAL
 */
//int Connect::groupList(dbr_string_t * &glist, unsigned int &listLength) {
int Connect::groupList(boost::shared_ptr<pv_string_t []> &glist, unsigned int &listLength)
{

    listLength=PVGroupV.size();
    //glist = new dbr_string_t [listLength];

    glist.reset(new pv_string_t[listLength]);

    for (unsigned short i = 0; i < PVGroupV.size(); ++i)
    {
        strcpy(glist[i],PVGroupV[i].name);
    }

    return ICAFE_NORMAL;
}


/**
 * \brief Connect::groupList - lists all groups in global vector PVGroupV
 * \param glist output: vector of group names (string)
 * \return ICAFE_NORMAL
 */
int Connect::groupList(std::vector<std::string> &glist)
{

    glist.clear();
    glist.reserve(PVGroupV.size());

    for (unsigned short i = 0; i < PVGroupV.size(); ++i)
    {
        glist.push_back(PVGroupV[i].name);
    }

    return ICAFE_NORMAL;
}


/**
 * \brief  Connect::groupMemberList - lists the members of the given group \n
 *         User responsible for deleting list array
 * \param  groupName input: group name
 * \param  list output: shared_ptr to array of process variables (pv_string_t)
 * \param  listLength: length of list array
 * \return ICAFE_NORMAL is all OK else, ECAFE_UNKNOWN_GROUP
 */

int Connect::groupMemberList(const char * groupName, boost::shared_ptr<pv_string_t []> &list, unsigned int &listLength)
{

    char _groupName[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(groupName, _groupName);

    bool groupExists  = false;
    listLength=0;

    for (unsigned short i = 0; i < PVGroupV.size(); ++i)
    {
        if (strcmp(PVGroupV[i].name, _groupName ) == 0)
        {
            //list = new pv_string_t [PVGroupV[i].npv];
            list.reset(new pv_string_t[PVGroupV[i].npv]);

            for (unsigned int j=0; j< PVGroupV[i].npv; ++j)
            {
                strcpy(list[j],PVGroupV[i].pvdata[j].pv);
                //cout << "groupMember " << list[j] << " [" << j << "]" << endl;
            }
            listLength=PVGroupV[i].npv;
            groupExists  = true;
            break;
        }
    }

    if(!groupExists)
    {
        return ECAFE_UNKNOWN_GROUP;
    }
    return ICAFE_NORMAL;
}


/*

int Connect::groupMemberList(const char * groupName, dbr_string_t * &list, unsigned int &listLength) {


    char _groupName[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(groupName, _groupName);

    bool groupExists  = false; listLength=0;

    for (unsigned short i = 0; i < PVGroupV.size(); ++i) {
        if (strcmp(PVGroupV[i].name, _groupName ) == 0) {
            list = new dbr_string_t [PVGroupV[i].npv];

            for (unsigned int j=0; j< PVGroupV[i].npv; ++j) {
                strcpy(list[j],PVGroupV[i].pvdata[j].pv);
            }
            listLength=PVGroupV[i].npv;
            groupExists  = true;
            break;
        }
    }

    if(!groupExists) {return ECAFE_UNKNOWN_GROUP;}
    return ICAFE_NORMAL;
}
*/

/**
 * \brief Connect::groupMemberList - lists the members of the given group
 * \param groupName input: group name
 * \param list output: vector of process variables (string)
 * \return ICAFE_NORMAL is all OK else, ECAFE_UNKNOWN_GROUP
 */
int Connect::groupMemberList(const char * groupName, std::vector<std::string> &list)
{

    char _groupName[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(groupName, _groupName);

    list.clear();

    bool groupExists  = false;

    for (unsigned short i = 0; i < PVGroupV.size(); ++i)
    {
        if (strcmp(PVGroupV[i].name, _groupName ) == 0)
        {
            list.reserve(PVGroupV[i].npv);
            for (unsigned int j=0; j< PVGroupV[i].npv; ++j)
            {
                list.push_back(PVGroupV[i].pvdata[j].pv);
            }

            groupExists  = true;
            break;
        }
    }

    if(!groupExists)
    {
        return ECAFE_UNKNOWN_GROUP;
    }
    return ICAFE_NORMAL;
}

/**
 * \brief Connect::fetchIndexOfGroupMember
 * \param groupName input: group name
 * \param pv input: (const char *) process variable
 * \return index of pvdata for pv within group; else -1 if error
 */
int Connect::fetchIndexOfGroupMember(const char *groupName, const char * pv)
{

    bool groupExists=false;

    char _groupName[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(groupName, _groupName);

    char _pv[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(pv, _pv);

    for (unsigned short i = 0; i < PVGroupV.size(); ++i)
    {
        if (strcmp(PVGroupV[i].name, _groupName ) == 0)
        {

            for (unsigned int j=0; j< PVGroupV[i].npv; ++j)
            {
                if (strcmp(PVGroupV[i].pvdata[j].pv, _pv) == 0)
                {
                    return j;
                }
            }

            groupExists  = true;
            break;
        }
    }
    if(!groupExists)
    {
        cout << "group " << groupName << " does not exist! " << endl;
    }
    else
    {
        cout << "group " << groupName << " does not have a pv member with name " <<  pv << endl;
    }

    return -1;
}




////////////////// COLLECTIONS //////////////////////////////


/**
 * \brief Connect::collectionList - lists all collection in global vector deviceCollectionV \n
 * \param clist output: array of collection names (pv_string_t)
 * \param listLength output: length of clist array
 * \return ICAFE_NORMAL
 */
int Connect::collectionList(boost::shared_ptr<pv_string_t []> &clist,  unsigned int &listLength)
{

    listLength=deviceCollectionV.size();
    //clist = new dbr_string_t [listLength];
    clist.reset(new pv_string_t[listLength]);

    for (unsigned short i = 0; i < deviceCollectionV.size(); ++i)
    {
        strcpy(clist[i],deviceCollectionV[i].name.c_str());
    }
    return ICAFE_NORMAL;
}


/**
 * \brief Connect::collectionList - lists all collections in global vector deviceCollectionV
 * \param clist output: vector of collection names (string)
 * \return ICAFE_NORMAL
 */
int Connect::collectionList(std::vector<std::string> &clist)
{

    clist.clear();
    clist.reserve(deviceCollectionV.size());

    for (unsigned short i = 0; i < deviceCollectionV.size(); ++i)
    {
        clist.push_back(deviceCollectionV[i].name);
    }

    return ICAFE_NORMAL;
}


/**
 * \brief Connect::collectionMemberList - lists the members of the given collection \n
 * \param collectionName input: name of Collection
 * \param list output: array of device names (pv_string_t)
 * \param listLength: length of list array
 * \return ICAFE_NORMAL is all OK else, ECAFE_UNKNOWN_COLLECTION
 */
int Connect::collectionMemberList(const char * collectionName, boost::shared_ptr<pv_string_t []> &list, unsigned int &listLength)
{

    char _collectionName[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(collectionName, _collectionName);

    bool collectionExists  = false;
    listLength=0;

    for (unsigned short i = 0; i < deviceCollectionV.size(); ++i)
    {
        if (strcmp(deviceCollectionV[i].name.c_str(), _collectionName ) == 0)
        {
            vector<collectionMember> vc =  deviceCollectionV[i].getCMembers();
            //list = new pv_string_t [vc.size()];
            list.reset(new pv_string_t[vc.size()]);

            for (unsigned int j=0; j< vc.size(); ++j)
            {
                strcpy(list[j],vc[j].deviceName.c_str());
            }
            listLength=vc.size();
            collectionExists  = true;
            break;
        }
    }

    if(!collectionExists)
    {
        return ECAFE_UNKNOWN_COLLECTION;
    }
    return ICAFE_NORMAL;
}


/**
 * \brief Connect::collectionMemberList - lists the members of the given collection
 * \param collectionName input: name of collection
 * \param list output: vector of devices (string)
 * \return ICAFE_NORMAL is all OK else, ECAFE_UNKNOWN_COLLECTION
 */
int Connect::collectionMemberList(const char * collectionName, std::vector<std::string> &list)
{


    char _collectionName[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(collectionName, _collectionName);

    list.clear();
    list.reserve(deviceCollectionV.size());

    bool collectionExists  = false;

    for (unsigned short i = 0; i < deviceCollectionV.size(); ++i)
    {
        if (strcmp(deviceCollectionV[i].name.c_str(), _collectionName ) == 0)
        {

            vector<collectionMember> vc =  deviceCollectionV[i].getCMembers();

            for (unsigned int j=0; j< vc.size(); ++j)
            {
                list.push_back(vc[j].deviceName);
            }
            collectionExists  = true;
            break;
        }
    }

    if(!collectionExists)
    {
        return ECAFE_UNKNOWN_COLLECTION;
    }
    return ICAFE_NORMAL;
}


/**
 * \brief Connect::devicePositionOrderedMultiMap - returns a map of devices and their positions
 * \param collectionName input: name of collection
 * \param posDev output: multimap of devices (string) and their positions (float) - odered in position
 * \return ICAFE_NORMAL is all OK else, ECAFE_UNKNOWN_COLLECTION
 */
int Connect::devicePositionOrderedMultiMap(const char * collectionName, std::multimap<float, std::string> &posDev)
{

    char _collectionName[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(collectionName, _collectionName);

    posDev.clear();

    std::map<string, float> devPos;
    devPos.clear();

    bool collectionExists  = false;

    for (unsigned short i = 0; i < deviceCollectionV.size(); ++i)
    {
        if (strcmp(deviceCollectionV[i].name.c_str(), _collectionName ) == 0)
        {

            vector<collectionMember> vc =  deviceCollectionV[i].getCMembers();

            for (unsigned int j=0; j< vc.size(); ++j)
            {
                devPos.insert(std::make_pair(vc[j].deviceName, vc[j].devicePosition));
            }
            collectionExists  = true;
            break;
        }
    }

    if(collectionExists)
    {
        posDev = flip_map(devPos);
        return ICAFE_NORMAL;
    }
    else
    {
        return ECAFE_UNKNOWN_COLLECTION;
    }

}



/**
 * \brief Connect::devicePositionMap - returns a map of devices and their positions
 * \param collectionName input: name of collection
 * \param posDev output: map of devices (string) and their positions (float) - not ordered
 * \return ICAFE_NORMAL is all OK else, ECAFE_UNKNOWN_COLLECTION
 */
int Connect::devicePositionMap(const char * collectionName, std::map<float, std::string> &posDev)
{

    char _collectionName[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(collectionName, _collectionName);

    posDev.clear();

    bool collectionExists  = false;

    for (unsigned short i = 0; i < deviceCollectionV.size(); ++i)
    {
        if (strcmp(deviceCollectionV[i].name.c_str(), _collectionName ) == 0)
        {

            vector<collectionMember> vc =  deviceCollectionV[i].getCMembers();

            for (unsigned int j=0; j< vc.size(); ++j)
            {
                posDev.insert(std::make_pair(vc[j].devicePosition, vc[j].deviceName));
            }
            collectionExists  = true;
            break;
        }
    }

    if(collectionExists)
    {
        return ICAFE_NORMAL;
    }
    else
    {
        return ECAFE_UNKNOWN_COLLECTION;
    }

}




/**
 * \brief Connect::devicePositionV - returns a vector of devices and their positions
 * \param collectionName input: name of collection
 * \param dev output: vector of devices (string)
 * \param pos output: vector of devices positions (float)
 * \return ICAFE_NORMAL is all OK else, ECAFE_UNKNOWN_COLLECTION
 */
int Connect::devicePositionV(const char * collectionName, std::vector<std::string> &dev, std::vector<float> &pos)
{

    char _collectionName[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(collectionName, _collectionName);

    pos.clear();
    dev.clear();

    bool collectionExists  = false;

    for (unsigned short i = 0; i < deviceCollectionV.size(); ++i)
    {
        if (strcmp(deviceCollectionV[i].name.c_str(), _collectionName ) == 0)
        {

            vector<collectionMember> vc =  deviceCollectionV[i].getCMembers();

            for (unsigned int j=0; j< vc.size(); ++j)
            {
                dev.push_back(vc[j].deviceName);
                pos.push_back(vc[j].devicePosition);
            }
            collectionExists  = true;
            break;
        }
    }

    if(collectionExists)
    {
        return ICAFE_NORMAL;
    }
    else
    {
        return ECAFE_UNKNOWN_COLLECTION;
    }

}




/**
 * \brief  Connect::fetchIndexOfCollectionMember
 * \param  collectionName input: collection name
 * \param  deviceName input: (const char *) device name
 * \return index of device in collection; else -1 if error
 */
int Connect::fetchIndexOfCollectionMember(const char *collectionName, const char * deviceName)
{

    bool collectionExists=false;

    char _collectionName[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(collectionName, _collectionName);

    char _deviceName[PVNAME_SIZE];
    helper.removeLeadingAndTrailingSpaces(deviceName, _deviceName);

    for (unsigned short i = 0; i < deviceCollectionV.size(); ++i)
    {
        if (strcmp(deviceCollectionV[i].name.c_str(), _collectionName ) == 0)
        {

            vector<collectionMember> vc =  deviceCollectionV[i].getCMembers();

            for (unsigned int j=0; j< vc.size(); ++j)
            {
                if (strcmp(vc[j].deviceName.c_str(), _deviceName) == 0)
                {
                    return j;
                }
            }
            collectionExists  = true;
            break;
        }
    }
    if(!collectionExists)
    {
        cout << "collection " << collectionName << " does not exist! " << endl;
    }
    else
    {
        cout << "collection " << collectionName << " does not have a device member with name " <<  _deviceName << endl;
    }

    return -1;
}



/////////////////////////END COLLECTION //////////////////////////////////////////////








/**
 *  Establishes virtual circuit to epics process variable
 *  for each group member by calling Connect::open(pv, &handle)
 *
 *  \param pvgroup input/output: PVGroup object
 *  \param groupHandle output: (unique) group handle
 *  \throw CAFEException re-throws CAFEException from open
 *  \return status ECA_NORMAL if OK
 */
int Connect::groupOpen(PVGroup &pvgroup, unsigned int &groupHandle)
//throw (CAFEException_groupOpen)
{
#define __METHOD__ "Connect::groupOpen(PVGroup, &groupHandle) "
    try
    {
        status = Connect::groupOpen (pvgroup.getName(), groupHandle);
    }
    catch(CAFEException_groupOpen &e) 
    {   
      throw(e);
    }

    return status;

#undef __METHOD__
}


/**
 *  \brief Establishes virtual circuit to epics process variable
 *  for each group member by calling Connect::open(pv, &handle)
 *  \param groupName    input: (unique) name of the group
 *  \param groupHandle output: (unique) group handle
 *  \throw CAFEException_groupOpen re-throws CAFEException from open
 *  \return status ECA_NORMAL if OK
 */
int Connect::groupOpen(const char *groupName, unsigned int &groupHandle)
//throw (CAFEException_groupOpen)
{
#define __METHOD__ "Connect::groupOpen(groupName, &groupHandle) "

    // Determine ca-client context
    ca_client_context * ccc = ca_current_context();

    if (ccc == NULL)
    {
        try
        {
            status=Connect::init();     // ca_enable_preemptive_callback
        }
        catch(CAFEException_init)
        {
           
	    //status will be ECA_ALLOCMEM
            CAFEException_group e;
            e = exceptionsHelper.prepareCAFEException_group((char *) groupName, 0,
                    ECA_ALLOCMEM,__METHOD__, __LINE__);

	    std::string ewhat ="CAFEException_open," + std::string(e.groupName)  + ","  + std::to_string(0) \
	      + "," + std::to_string(ECA_ALLOCMEM) + "," +  cafeStatus.csc.message(ECA_ALLOCMEM) \
              + "," + cafeStatus.csi.message(ECA_ALLOCMEM); 
   
            CAFEException_groupOpen badgrp(ewhat);
            badgrp.groupEx=e;
            throw(badgrp);

            return status;
        }
        ccc = ca_current_context();
    }

 
    // Check if group already exists
    groupHandle = handleHelper.getGroupHandleFromGroupName(groupName, ccc);

    if (groupHandle != 0)
    {
        return ICAFE_NORMAL;
    }

    // Get group from memory:
    // Loop round vector of groups

    PVGroup pvgroup;


    bool groupExists = false;

    unsigned short iIdx=0;

    for (unsigned short i = 0; i < PVGroupV.size(); ++i)
    {
        if (strcmp(PVGroupV[i].name, groupName) == 0)
        {
            pvgroup = PVGroupV[i];
            groupExists = true;
            iIdx=i;
            break;
        }
    }

    if (!groupExists)
    {
        cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
        cafeStatus.report(ECAFE_UNKNOWN_GROUP);
        return ECAFE_UNKNOWN_GROUP;
    }


    // Loop over all members
    // Open Channels;
    unsigned int nMember = pvgroup.npv;
    unsigned int * memberHandle = new unsigned int [nMember];


    for (unsigned int i = 0; i < nMember; ++i)
    {
        memberHandle[i]=0;
        char _pv[60];
        strcpy( _pv, pvgroup.pvdata[i].pv);
        //char * _pv = pvgroup.pvdata[i].pv;
        unsigned int iNow = i;

        bool pvAlreadyExistsWithinGroup = false;


        for (unsigned int iPrevious = 0; iPrevious < iNow; ++iPrevious)
        {
            //Check if pv already exists within this group

            if (strcmp(pvgroup.pvdata[iPrevious].pv, _pv) == 0)
            {
                cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cout << "WARNING: PV " << _pv << " AT INDEX [" << iNow << "] " \
                     "ALREADY EXISTS with handle " << memberHandle[iPrevious]
                     << " AT INDEX [" << iPrevious << "] "<<  endl;
                cout << "WARNING: PV ADDED TO GROUP BUT IS ASSIGNED TO PREVIOUS HANDLE " << endl;
                pvAlreadyExistsWithinGroup = true;
                memberHandle[iNow] = memberHandle[iPrevious];
                break;
            }
        }


        //Do not create new handle if pv already exists withing this group

        if (!pvAlreadyExistsWithinGroup)
        {
            try
            {
                createHandleWithinGroup(_pv, ccc, memberHandle[i]);
            }
            catch (CAFEException_pv e)
            {

                cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cout << "Handle               : " << e.handle << endl;
                cout << "Process Variable (PV): " << e.pv << endl;
                cout << "Status Code          : " << e.statusCode << endl;
                cout << "Status Message       : " << e.statusCodeText << endl;
                cout << "CANNOT ADD PV TO GROUP SINCE UNIQUE ID ALREADY EXISTS " << endl;
                CAFEException_group ge;
                ge = exceptionsHelper.prepareCAFEException_group((char *) groupName, 0,
                        ECAFE_HASH_UNIQUEID_EXISTS,__METHOD__, __LINE__);
                CAFEException_groupOpen badgrp;
                badgrp.groupEx=ge;
                throw(badgrp);

                return ECAFE_HASH_UNIQUEID_EXISTS;
            }
        }

    } //for nMember



    CA_SYNC_GID groupID;
    status = ca_sg_create(&groupID);
    //Now Add to Hash Table!

    std::pair<cafeGroup_set::iterator, bool> p;

    if(MUTEX)
    {
        cafeMutex.lock();   //lock
    }
    p = gs.insert(ConduitGroup(groupName, ccc, groupID, nMember, memberHandle));
    if(MUTEX)
    {
        cafeMutex.unlock();   //unlock
    }


    // p.second true insert success
    // p.second false insert failed as pair already exists

    if (!(p.second))
    {

        CAFEException_group e;
        e = exceptionsHelper.prepareCAFEException_group((char *) groupName, 0,
                ECAFE_HASH_UNIQUEID_EXISTS,__METHOD__, __LINE__);
        CAFEException_groupOpen badgrp;
        badgrp.groupEx=e;
        throw(badgrp);

        return ECAFE_HASH_UNIQUEID_EXISTS;
    }

    groupHandle = (*(p.first)).getGroupHandle();

    pvgroup.groupHandle=groupHandle;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;



    for (unsigned int i = 0; i < nMember; ++i)
    {

        it_handle = handle_index.find(memberHandle[i]);

        if (it_handle != handle_index.end())
        {
            //Change group handle
            if(MUTEX)
            {
                cafeMutex.lock();
            }

            handle_index.modify((it_handle), change_groupHandle(groupHandle));

            if(MUTEX)
            {
                cafeMutex.unlock();
            }
        }
    }

    //What is the POLICY!??
    //To Flush or to Pend this is the question!
    if (channelOpenGroupPolicy.getWhenToFlushSendBuffer()==FLUSH_AFTER_EACH_GROUP_CREATION)
    {
        //Set timeout to a value that depends on the no of members: 1200 takes 3.5 seconds
        double extraTime=nMember/NMEMBER_PER_SEC_SG_PEND_EVENT;
        //if ( (DEFAULT_TIMEOUT_SG_PEND_EVENT + extraTime) > channelOpenGroupPolicy.getTimeout() ) {
        if ( (channelOpenGroupPolicy.getDefaultTimeout() + extraTime) > channelOpenGroupPolicy.getTimeout() )
        {
            channelOpenGroupPolicy.setTimeout(channelOpenGroupPolicy.getDefaultTimeout()+ extraTime);
        }

        using namespace boost::posix_time;
        ptime timeStart(microsec_clock::local_time());

        double  timeElapsed=0;
        unsigned int  nPoll=0;

        channelOpenGroupPolicy.flushSendBufferNow();

        bool allConnected=true;
        for (unsigned int i = 0; i < nMember; ++i)
        {
            it_handle = handle_index.find(memberHandle[i]);
            if (it_handle != handle_index.end())
            {
                if (!(*it_handle).isConnected())
                {
                    allConnected=false;
                }
            }
        }

        ptime timeEnd(microsec_clock::local_time());
        time_duration duration(timeEnd-timeStart);
        timeElapsed= (double) duration.total_microseconds()/1000000.0;

        while ( !allConnected && timeElapsed <= channelOpenGroupPolicy.getTimeout())
        {

#if HAVE_BOOST_THREAD
            boost::this_thread::sleep_for(boost::chrono::microseconds(1000));
#else
#if HAVE_LINUX
            usleep(1000);
#endif
#endif

            ++nPoll;

            allConnected=true;
            for (unsigned int i = 0; i < nMember; ++i)
            {
                it_handle = handle_index.find(memberHandle[i]);
                if (it_handle != handle_index.end())
                {
                    if (!(*it_handle).isConnected())
                    {
                        allConnected=false;
                    }
                }
            }

            ptime timeEnd(microsec_clock::local_time());
            time_duration duration(timeEnd-timeStart);
            timeElapsed= (double) duration.total_microseconds()/1000000.0;

        }

        /*
        cout << __METHOD__ <<endl;

        if (allConnected)  {
        		cout << groupName << " // is all connected //" << endl;
        }
        else {
        		cout << groupName << " // is not all connected //" << endl;
        }
        cout << "timeElapsed for open " << timeElapsed << " nPoll = " << nPoll << endl;
        */


    }
   
    //memHandle must not be deleted; allocated memory for use elsewhere
    return ICAFE_NORMAL;

#undef __METHOD__
};


/**
 * Closes all channel connections associated within a group,
 * for all groupHandles.
 * Note that this does NOT cause the channel's disconnect handler to be called.
 * It does however invoke event subscriptions (for monitors).
 * All Conduit handles are erased.
 * \return ICAFE_NORMAL if all OK
 */
int Connect::groupClose()
{
#define __METHOD__ "Connect::groupClose() "

    long gStatus=ICAFE_NORMAL;
    long _status=ICAFE_NORMAL;

    vector<PVGroup> m_pvgroup;
    m_pvgroup.clear();

    m_pvgroup.reserve(PVGroupV.size());
    std::copy(PVGroupV.begin(), PVGroupV.end(), std::back_inserter(m_pvgroup));

    unsigned int groupH=0;

    //cout << " PVGroupV.size() " << PVGroupV.size() << endl;
    //cout << " m_pvgroup.size() " <<  m_pvgroup.size() << endl;

    //PVGroupV members do not know about groupHandles
    for (size_t i=0; i<m_pvgroup.size(); ++i)
    {
      //std::cout << __METHOD__ << endl;
      //std::cout << "ccc" << ca_current_context() << endl;
  
        groupH=handleHelper.getGroupHandleFromGroupName(m_pvgroup[i].name);

        //cout << "Close group " << m_pvgroup[i].name << " with handle " << groupH << endl;
        _status=groupClose(groupH);
        if (_status!=ICAFE_NORMAL && gStatus==ICAFE_NORMAL)
        {
            gStatus=_status;
        }

    }//for

    m_pvgroup.erase( m_pvgroup.begin(),  m_pvgroup.end());

    return gStatus;

#undef __METHOD__
};


/**
 * Closes all channel connections associated within a group,
 * indentified by it's groupHandle.
 * Note that this does NOT cause the channel's disconnect handler to be called.
 * It does however invoke event subscriptions (for monitors).
 * All Conduit handles are erased.
 * \param  groupHandle input
 * \return ICAFE_NORMAL if all OK
 */
int Connect::groupClose(unsigned int groupHandle, bool keepGroupName, bool keepHandles)
{
#define __METHOD__ "Connect::groupClose(groupHandle) "

  //std::cout << __METHOD__ << " " << std::endl;

    //PVGroupV members do not know about groupHandles
    //for (size_t i=0; i<PVGroupV.size(); ++i) {
    //    std::cout << PVGroupV[i].name << " " << i << std::endl;
    //}

    long gStatus=ICAFE_NORMAL;
    cafeGroup_set_by_groupHandle & groupHandle_index = gs.get<by_groupHandle> ();
    cafeGroup_set_by_groupHandle::iterator it_groupHandle;

    it_groupHandle = groupHandle_index.find(groupHandle);

    string groupName=handleHelper.getGroupNameFromGroupHandle(groupHandle);

    if (it_groupHandle != groupHandle_index.end())
    {

      // std::cout << "GROUP ID " << (*it_groupHandle).groupID << std::endl;

        //this combines instructions into one single message

      if (keepHandles == true) {
	  closeChannelsKeepHandles((*it_groupHandle).mHandle, (*it_groupHandle).getNMember());
      }
      else {
          closeHandles((*it_groupHandle).mHandle, (*it_groupHandle).getNMember());
      }

        //Delete group
        //if(MUTEX)
        //{
        //    cafeMutex.lock();
        //}
        gStatus=ca_sg_delete((*it_groupHandle).groupID);
        //if(MUTEX)
        //{
        //    cafeMutex.unlock();
        //}
 
       

        if (gStatus != ICAFE_NORMAL)
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(gStatus);
        }

        
        //Now erase groupHandle
        gs.erase(it_groupHandle);

    }
    else
    {
        std::cout << __METHOD__ << " " << groupName << " NOT FOUND " << std::endl;
        return ECAFE_INVALID_GROUP_HANDLE;
    }

    if (keepGroupName == true) {
        return gStatus;
    }

    //Remove from PVGroup Vector
    //WARNING
    //PVGroupV members do not know about groupHandles
    for (size_t i=0; i<PVGroupV.size(); ++i)
    {

        if (PVGroupV[i].name==groupName)
        {
            //if ( PVGroupV[i].groupHandle==groupHandle) {//wrong!
            PVGroupV.erase (PVGroupV.begin()+i) ;
            break;
        }
    }
    return gStatus;
#undef __METHOD__
}


/**
 * Erases groupHandles
 * All Groupconduit handles are erased
 * \return ICAFE_NORMAL if all OK
 */
int Connect::groupHandleErase()
{
#define __METHOD__ "Connect::groupHandleErase() "


    if(MUTEX)
    {
        cafeMutex.lock();
    }

    for (itgs = gs.begin(); itgs != gs.end(); )
    {
        gs.erase(itgs++);

    }//for

    if(MUTEX)
    {
        cafeMutex.unlock();
    }

    return ICAFE_NORMAL;
#undef __METHOD__
}



/**
 * Erases groupHandle (assumes associated connections are already closed)
 * All Groupconduit handles are erased withingt the give ca_client_context
 * \param  ccc input: ca_client_context *
 * \return ICAFE_NORMAL always
 */
int Connect::groupHandleErase(ca_client_context * ccc)
{
#define __METHOD__ "Connect::groupHandleErase(ca_client_context *) "

    if(MUTEX)
    {
        cafeMutex.lock();
    }

    for (itgs = gs.begin(); itgs != gs.end(); )
    {
        if ( ((*itgs).getClientContext() == ccc))
        {
            gs.erase(itgs++);
        }
        else
        {
            ++itgs;
        }
    }//for

    if(MUTEX)
    {
        cafeMutex.unlock();
    }

    return ICAFE_NORMAL;

#undef __METHOD__
}

/**
 *  Connect::createChannelWithinGroup (as for createChannel)
 *
 *  Establishes virtual circuit to epics process variable
 *
 *  \param _handle input
 *  \param pv input: process variable
 *  \param pCh output: channel identifier (chid)
 *  \return status ECA_NORMAL if all OK else ECA_BADTYPE, ECA_STRTOBIG, ECA_ALLOCMEM
 *
 */
int Connect::createChannelWithinGroup(unsigned int _handle, const char * pv, chid &pCh)
{
#define __METHOD__ "Connect::createChannelWithinGroup(unsigned int _handle, const char * pv, chid &pCh)"

    ChannelRegalia channelRegalia; 

    cafeConduit_set_by_handle &  handle_index= cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle =handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {


        /* 11 March 2019
        // Allow Python Users to fire their own native callback for when using default callbackHandlerCreate
        // Check pyCallbackFlag in ChannelOpenFolicy and set pyCallbackFlag in conduit object accordingly
        //
	Moved to create Handle 29 Sep 2020 following Connect::createChannel
#if HAVE_PYTHON_H
        bool _pyCallbackFlag=channelCreatePolicy.getPyCallbackFlag();
        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }
        handle_index.modify(it_handle, change_pyOpenCallbackFlag (_pyCallbackFlag) );
        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }
#endif
        */
        status = ca_create_channel(pv, channelCreatePolicy.getHandler(), (void *) (long long) _handle,
                                   channelCreatePolicy.getPriority(), &pCh);


        if(pCh!=NULL)
        {

	    if (MUTEX)
            {
                cafeMutex.lock();   //lock
            }

            handle_index.modify(it_handle, change_channelID (pCh));

            if (MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }

	     channelRegalia = (*it_handle).getChannelRegalia();    
	    //std::cout << " OLD VALUE " << channelRegalia.cafeConnectionState <<  " " << (*it_handle).pv << std::endl;  
     
            if ( channelRegalia.cafeConnectionState == ICAFE_CS_CLOSED) {
	        //This is to overwrite the last values arising from a user supplied closechannelkeephandle method
	        channelRegalia.cafeConnectionState = ICAFE_CS_NEVER_CONN;
                channelRegalia.connectFlag = false;
		if(MUTEX)
		{
		    cafeMutex.lock();   //lock
		}
                handle_index.modify(it_handle, change_channelRegalia (channelRegalia));
                handle_index.modify(it_handle, change_status (ICAFE_CA_OP_CONN_DOWN));
		if(MUTEX)
		{
                    cafeMutex.unlock();   //unlock
		}		     
	    }
            channelRegalia = (*it_handle).getChannelRegalia(); 
	    //std::cout << " NEW VALUE " << channelRegalia.cafeConnectionState <<  " " << (*it_handle).pv << std::endl;
            


            //Install the access rights state change callback handler
            
            if(ADD_EXCEPTION_EVENT==true)
            {
                ca_add_exception_event(callbackHandlerException, (void*) (long long) _handle);
            }
	    ca_replace_access_rights_event(pCh, callbackHandlerAccessRights);

          
        }

        if (status != ECA_NORMAL)
        {
            cout << __FILE__ << "/" << __LINE__ << "/"<< __METHOD__ << " ca_create_channel failed: " << endl;
            if (status == ECA_EVDISALLOW)
            {
                cout << __FILE__ << "/" << __LINE__ << "/"<< __METHOD__ << " inappropriate function " << endl;
            }


            if(MUTEX)
            {
                cafeMutex.lock();   //lock
            }
            handle_index.modify(it_handle, change_status (status) );
            if(MUTEX)
            {
                cafeMutex.unlock();   //unlock
            }
            return (int)  status;
        }

        // Peculiar if true
        /*
        if (status == ECA_EVDISALLOW) {
        cout << __FILE__ << "/" << __LINE__ << "/"<< __METHOD__ << " inappropriate function " << endl;
        cafeStatus.report(status);
        if(MUTEX){cafeMutex.lock();}  //lock
        handle_index.modify(it_handle, change_status (status) );
        if(MUTEX){cafeMutex.unlock();} //unlock
        }
        */
    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }

    return (int) status;

#undef __METHOD__
}


/**
 * \brief Connect::createHandleWithinGroup
 *
 * \param pv input:  process variable
 * \param ccc input: ca_client_context
 * \param _handle input: handle
 * \return status
 */
int Connect::createHandleWithinGroup(const char  * pv, ca_client_context * ccc,
                                     unsigned int & _handle) //throw (CAFEException_pv)
{
#define __METHOD__ "Connect::createHandleCreateWithinGroup"


    // create channel
    chid pCh = NULL;
    std::pair<cafeConduit_set::iterator, bool> p;


    if(MUTEX)
    {
        cafeMutex.lock();   //lock
    }


    p = cs.insert(Conduit(pv, ccc,
                          channelRequestPolicyMasterPut, channelRequestPolicyMasterGet,
                          channelGetActionWhenMonitorPolicyMaster, pyCafeFlag) );


    if(MUTEX)
    {
        cafeMutex.unlock();   //unlock
    }

    // p.second true insert success
    // p.second false insert failed as pair already exists
    //has handle no exceeded limit
    if (!(p.second))
    {
        cout << "HANDLE ALREADY IN USE! " << endl;
        cout << "ISSUING NEW HANDLE FOR PV= " << pv << endl;
        cout << "Recovering with next free handle which is " << handleHelper.getNextFreeHandle() << endl;
        Conduit::handleNext=handleHelper.getNextFreeHandle()-1;
        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }
        p = cs.insert(Conduit(pv, ccc,
                              channelRequestPolicyMasterPut, channelRequestPolicyMasterGet,
                              channelGetActionWhenMonitorPolicyMaster, pyCafeFlag));
        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }
    }


    //Conduit cc = *(p.first);

    // The following should not happen as we do a pre-check in Connect::open
    // p.second true insert success
    // p.second false insert failed as pair already exists

    if (!(p.second))
    {

        cout << " p SECOND - THROWING EXCEPTION FOR PV= " << pv << endl;
        cout << (*(p.first)).pv.c_str()     << endl;
        cout << (*(p.first)).getHandle()    << endl;
        cout << (*(p.first)).getChannelID() << endl;
        CAFEException_pv e;
        e = exceptionsHelper.prepareCAFEException_pv((*(p.first)).pv.c_str(),
                (*(p.first)).pvAlias.c_str(), (*(p.first)).getHandle(), (*(p.first)).getChannelID(),
                ECAFE_HASH_UNIQUEID_EXISTS, __METHOD__, __LINE__);
        throw (e);
        return ECAFE_HASH_UNIQUEID_EXISTS;
    }


        _handle= (*(p.first)).getHandle();

	cafeConduit_set_by_handle &  handle_index= cs.get<by_handle>();

 
        // 11 March 2019 - Copied from Connect:: 29 September 2020
        // Allow Python Users to fire their own native callback for when using default callbackHandlerCreate
        // Check pyCallbackFlag in ChannelOpenFolicy and set pyCallbackFlag in conduit object accordingly
        //
	/*
#if HAVE_PYTHON_H
        bool _pyCallbackFlag=channelCreatePolicy.getPyCallbackFlag();
        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }
        handle_index.modify(p.first, change_pyOpenCallbackFlag (_pyCallbackFlag) );
        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }

	//channelCreatePolicy.getPyHandler and add to cafe.conduit!
        void * _pyConnectCallbackFn=channelCreatePolicy.getPyConnectHandler();
	
        if(MUTEX)
        {
            cafeMutex.lock();   //lock
        }
        handle_index.modify(p.first, change_pyConnectCallbackFn (_pyConnectCallbackFn) );


        if(MUTEX)
        {
            cafeMutex.unlock();   //unlock
        }
        
#endif
	*/


    status = createChannelWithinGroup(_handle, pv, pCh);


    // Possible errors from ca_create_channel: ECA_NORMAL, ECA_BADTYPE, ECA_STRTOBIG, ECA_ALLOCMEM
    // Possible error from ca_pend_event: ECA_EVDISALLOW

    //IF FAILED THROW EXCEPTION
    if (status != ECA_NORMAL && status != ECA_TIMEOUT)
    {
      //Connect::close(_handle);
        CAFEException_pv e;
        e = exceptionsHelper.prepareCAFEException_pv((*(p.first)).pv.c_str(),
                (*(p.first)).pvAlias.c_str(), (*(p.first)).getHandle(),
                pCh, status, __METHOD__, __LINE__);
        throw (e);
        return status;
    }
    else if (pCh == NULL)
    {
      //Connect::close(_handle);
        CAFEException_pv e;
        e = exceptionsHelper.prepareCAFEException_pv((*(p.first)).pv.c_str(),
                (*(p.first)).pvAlias.c_str(), (*(p.first)).getHandle(),
                pCh, ECAFE_NULLCHID, __METHOD__, __LINE__);
        throw (e);
        return ECAFE_NULLCHID;
    }

    return ICAFE_NORMAL;

}



/**
 *  \brief send the command to the ioc to open channels within all group(s).
 *  \param _timeout input: max pend time to establish connections \n
 */
void Connect::openGroupNowAndWait(double _timeout)
{
    double dto = channelOpenGroupPolicy.getTimeout();
    channelOpenGroupPolicy.setTimeout(_timeout);

    //Time lapsed

    using namespace boost::posix_time;
    ptime timeStart(microsec_clock::local_time());

    double  timeElapsed=0;
    unsigned int  nPoll=0;

    channelOpenGroupPolicy.flushSendBufferNow();

    ptime timeEnd(microsec_clock::local_time());
    time_duration duration(timeEnd-timeStart);
    timeElapsed= (double) duration.total_microseconds()/1000000.0;

    while ( !handleHelper.allChannelsWithinGroupConnected() && (timeElapsed <= channelOpenGroupPolicy.getTimeout()))
    {

#if HAVE_BOOST_THREAD
        boost::this_thread::sleep_for(boost::chrono::microseconds(1000));
#else
#if HAVE_LINUX
        usleep(1000);
#endif
#endif

        ++nPoll;

        ptime timeEnd(microsec_clock::local_time());
        time_duration duration(timeEnd-timeStart);
        timeElapsed= (double) duration.total_microseconds()/1000000.0;

    }

    /*
    if ( handleHelper.allChannelsWithinGroupConnected() ) {
        cout << " // all group members are connected //" << endl;
    }
    else {
        cout << " // not all group members are connected //" << endl;
    }
    cout << "timeElapsed for group open " << timeElapsed << " nPoll = " << nPoll << endl;
    */


    //reset
    channelOpenGroupPolicy.setWhenToFlushSendBuffer(FLUSH_NOW);
    channelOpenGroupPolicy.setFlushSendBufferKind(WITH_POLL); //PEND_EVENT);
    //channelOpenPolicy.setTimeoutToDefault();
    channelOpenGroupPolicy.setTimeout(dto);
    return;
}


/**
 *  \brief send the command to the ioc to open channels within given group(s).
*  \param _timeout input: max pend time to establish connections \n
*  \param  gHandles input: vector of groups handles \n
 */
void Connect::openGroupNowAndWaitForInputGroups(double _timeout, std::vector<unsigned int> gHandles)
{
    double dto = channelOpenGroupPolicy.getTimeout();
    channelOpenGroupPolicy.setTimeout(_timeout);

    //Time lapsed

    using namespace boost::posix_time;
    ptime timeStart(microsec_clock::local_time());

    double  timeElapsed=0;
    unsigned int  nPoll=0;

    channelOpenGroupPolicy.flushSendBufferNow();

    ptime timeEnd(microsec_clock::local_time());
    time_duration duration(timeEnd-timeStart);
    timeElapsed= (double) duration.total_microseconds()/1000000.0;

    while ( !handleHelper.allChannelsWithinGroupConnectedV(gHandles) && (timeElapsed <= channelOpenGroupPolicy.getTimeout()))
    {

#if HAVE_BOOST_THREAD
        boost::this_thread::sleep_for(boost::chrono::microseconds(1000));
#else
#if HAVE_LINUX
        usleep(1000);
#endif
#endif

        ++nPoll;

        ptime timeEnd(microsec_clock::local_time());
        time_duration duration(timeEnd-timeStart);
        timeElapsed= (double) duration.total_microseconds()/1000000.0;

    }

    /*
    if ( handleHelper.allChannelsWithinGroupConnectedV(gHandles) )
    {
        cout << " // all group members of all groups are connected //" << endl;
    }
    else
    {
        cout << " // not all group members of all groups are connected //" << endl;
    }
    cout << "timeElapsed for mulitple group open " << timeElapsed << " nPoll = " << nPoll << endl;
    */


    //reset
    channelOpenGroupPolicy.setWhenToFlushSendBuffer(FLUSH_NOW);
    channelOpenGroupPolicy.setFlushSendBufferKind(WITH_POLL); //PEND_EVENT);
    //channelOpenPolicy.setTimeoutToDefault();
    channelOpenGroupPolicy.setTimeout(dto);
    return;
}


