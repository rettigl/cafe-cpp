///
/// \file    deviceCollection.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef DEVICECOLLECTION_H
#define DEVICECOLLECTION_H

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <helper.h>

class collectionMember
{
public:
    collectionMember():deviceName(""),devicePosition(0) {};
    virtual ~collectionMember() {};
    std::string deviceName;
    float devicePosition;
};

class deviceCollection
{
    friend class Connect;
    friend class CAFE;
    //if HAVE_LIBQTXML
    friend class loadCollectionXMLParser;
    //endif
private:
    std::string name;
    std::string description;
    std::vector<collectionMember> cMembers;
    std::vector<std::string> attributes;

    Helper helper;
public:
    deviceCollection():description("collection of devices") {};
    virtual ~deviceCollection() {};

    std::string getName() const
    {
        return name;
    };
    std::string getDescription() const
    {
        return description;
    };
    std::vector<collectionMember> getCMembers() const
    {
        return cMembers;
    };

    std::vector<std::string> getAttributes() const
    {
        return attributes;
    };
    std::vector<float> getPositions()
    {
        std::vector<float> posV;
        posV.clear();
        posV.reserve(cMembers.size());
        for (size_t i=0; i<cMembers.size(); ++i)
        {
            posV.push_back(cMembers[i].devicePosition);
        };
        return posV;
    };
    float getPosition(const char * c)
    {
        char _c[PVNAME_SIZE];
        helper.removeLeadingAndTrailingSpaces(c, _c);
        for (size_t i=0; i<cMembers.size(); ++i)
        {
            if(strcmp(cMembers[i].deviceName.c_str(),_c)==0)
            {
                return cMembers[i].devicePosition;
            }
        }
        std::cout << "INPUT: " << _c << " NOT FOUND IN COLLECTION: " << name << std::endl;
        std::cout << "HENCE RETURNING -1 FOR POSITION " << std::endl;
        return -1;
    }

    std::vector<std::string> getMembers()
    {
        std::vector<std::string> memberV;
        memberV.clear();
        memberV.reserve(cMembers.size());
        for (size_t i=0; i<cMembers.size(); ++i)
        {
            memberV.push_back(cMembers[i].deviceName);
        };
        return memberV;
    };

    unsigned int  getNCollectionMembers() const
    {
        return cMembers.size();
    };
    unsigned int  getNAttribuites() const
    {
        return attributes.size();
    };
};


class collectionInGroup
{
    //friend class loadGroupXMLParser;
public:
    collectionInGroup() {};
    virtual ~collectionInGroup() {};
    std::string id;
    std::string attrib;
};

class deviceGroup
{
    //if HAVE_LIBQTXML
    friend class loadGroupXMLParser;
    //endif
private:
    std::string id;
    std::string description;

    std::vector<collectionMember> members;
    std::vector<std::string> xmlMembers;
    std::vector<collectionInGroup> collections;
public:
    deviceGroup() {};
    virtual ~deviceGroup() {};

    std::string getName() const
    {
        return id;
    };
    std::string getID() const
    {
        return id;
    };

    std::string getDescription() const
    {
        return description;
    };
    std::vector<collectionMember> getCMembers() const
    {
        return members;
    };
    std::vector<collectionInGroup> getCollections() const
    {
        return collections;
    };
    std::vector<std::string>getXMLMembers() const
    {
        return xmlMembers;
    };
};



#endif // DEVICECOLLECTION_H
