///
/// \file    cafeXML.cpp
/// \author  Jan Chrin, PSI
/// \date    November 2014
/// \version CAFE 1.0.0
///

#include <iostream>
#include <fstream>
#include <ctime>
#include <cafe.h>

#include <boost/date_time/posix_time/posix_time.hpp>


using namespace std;
using namespace boost::posix_time;



//has config.h
#include <loadCollectionXMLParser.h>
#include <loadGroupXMLParser.h>
#include <restorePVGroupXMLParser.h>

#if HAVE_LIBQTXML

/**
 *  \brief loadCollections from XML \n
 *  Routine first searches the given directory before CAFE_XML_PATH environment variable
 *  \param collectionFile input: name of collection file
 *  \return ICAFE_NORMAL if OK else ECAFE_LOAD_COLLECTION;
 */
int  CAFE::loadCollectionsFromXML(const char * collectionFile)
{
#define __METHOD__ "CAFE::loadCollectionsFromXML(char * collectionFile)"


    //First check for existence of file in current directory
    //before searching in CAFE_XML_PATH

    QFile * file;

    file = new QFile(collectionFile);

    if (!file->exists())
    {

        std::string envS;
        char * env = getenv("CAFE_XML_PATH");
        env == NULL ? envS=std::string(".") : envS=std::string(env);
        envS.append("/"); //("/Collections/");
        envS.append(collectionFile);

        file = new QFile(envS.c_str());

        if (!file->exists())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "envS=" << envS << endl;
            cout << "COLLECTION FILE " << collectionFile << " NOT FOUND " << endl;
            cout << "IN THE CURRENT (OR GIVEN) DIRECTORY" << endl;
            if (env != NULL)
            {
                cout << "NOR IN CAFE_XML_PATH/=" << endl;
                cout << env << endl;
            }
            else
            {
                cout << "OPTIONAL ENVIRONMENT VARIABLE CAFE_XML_PATH IS UNDEFINED" << endl;
            }
            delete file;
            return ECAFE_LOAD_COLLECTION;
        }
    }
    else
    {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << " COLLECTION LOADED FROM CURRENT (OR GIVEN) DIRECTORY" << endl;
    }

    loadCollectionXMLParser handler;

    QXmlInputSource source(file);
    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);
    reader.setFeature("http://trolltech.com/xml/features/report-whitespace-only-CharData", false);

    bool isOK = reader.parse(source);

    if (!isOK)
    {
        return ECAFE_LOAD_COLLECTION;
    }
    deviceCollectionV=handler.deviceCollectionV;

    //cout << deviceCollectionV[0].getCMembers()[0].devicePosition << endl;
    //cout << deviceCollectionV[1].getCMembers()[0].devicePosition << endl;

    return ICAFE_NORMAL;

#undef __METHOD__
}



/**
 *  \brief loadGroups from XML \n
 *  Routine first searches the given directory before CAFE_XML_PATH environment variable
 *  \param groupFile input: name of group file
 *  \return ICAFE_NORMAL if OK else ECAFE_LOAD_GROUP;
 */
int  CAFE::loadGroupsFromXML     (const char * groupFile)
{
#define __METHOD__ "CAFE::loadGroupsFromXML(char * groupFile)"

    int  localStatus=ICAFE_NORMAL;

    QFile * file;
    file = new QFile(groupFile);

    if (!file->exists())
    {

        std::string envS;
        char * env = getenv("CAFE_XML_PATH");
        env == NULL ? envS=std::string(".") : envS=std::string(env);
        envS.append("/"); //envS.append("/Groups/");
        envS.append(groupFile);

        file = new QFile(envS.c_str());

        if (!file->exists())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "GROUP FILE " << groupFile << " NOT FOUND " << endl;
            cout << "NEITHER IN THE CURRENT (OR GIVEN) DIRECTORY" << endl;

            if (env != NULL)
            {
                cout << "NOR IN CAFE_XML_PATH/=" << endl;
                cout << env << endl;
            }
            else
            {
                cout << "OPTIONAL ENVIRONMENT VARIABLE CAFE_XML_PATH IS UNDEFINED" << endl;
            }
            delete file;

            return ECAFE_LOAD_GROUP;
        }
    }
    else
    {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << " GROUPS LOADED FROM CURRENT (OR GIVEN) DIRECTORY" << endl;
    }


    loadGroupXMLParser handler;
    QXmlInputSource source(file);

    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);
    reader.setFeature("http://trolltech.com/xml/features/report-whitespace-only-CharData", false);

    bool isOK = reader.parse(source);

    if (!isOK)
    {
        return ECAFE_LOAD_GROUP;
    }


    for (std::vector<deviceGroup>::const_iterator group=handler.groups.begin();
            group!=handler.groups.end(); ++group)
    {

        if(isGroup((char *)(*group).getName().c_str()))
        {

            localStatus=ECAFE_GROUP_PREV_DEF;
            std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
            std::cout << "FAILED TO LOAD GROUP WITH NAME(ID) " << (*group).getName() << std::endl;
            std::cout << "AS GROUP WITH THIS NAME ALREADY EXISTS " << std::endl;
            cafeStatus.report(ECAFE_GROUP_PREV_DEF);
            std::cout << "WE CONTINUE WITH LOADING ANY REMAINING GROUPS " << std::endl;
            continue;
        }

        PVGroup pg;
        vector<std::string> pvList;
        pvList.clear();
        strcpy(pg.name,(*group).getName().c_str());


        //Loop round all collections;
        vector<collectionInGroup> cg;
        cg.clear();

        cg=(*group).getCollections();

        for (size_t i=0; i <cg.size(); ++i)
        {
            //cout << cg[i].id << " " << cg[i].attrib << " " <<  endl; //cg[i].collectiveType << endl;

            if(!isCollection((char *)cg[i].id.c_str()))
            {
                localStatus=ECAFE_UNKNOWN_COLLECTION;
                std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
                std::cout << "FAILED TO LOAD COLLECTION WITH NAME(ID) " << cg[i].id << std::endl;
                std::cout << "AS COLLECTION WITH THIS NAME DOES NOT EXIST " << std::endl;
                cafeStatus.report(ECAFE_UNKNOWN_COLLECTION);
                std::cout << "WE CONTINUE LOADING GROUP WITH ANY REMAINING COLLECTIONS " << std::endl;
                continue;
            }
            // Fill Group
            vector<string> members;
            collectionFetch((char *)cg[i].id.c_str(),members);

            for(size_t j=0; j<members.size(); ++j)
            {
                string da=members[j] + deviceAttributeDeliminator +  cg[i].attrib;
                pvList.push_back(da);
            }
        }

        //Loop round all members
        vector<string> cm= (*group).getXMLMembers();


        for (size_t i=0; i <cm.size(); ++i)
        {
            pvList.push_back(cm[i]);
        }

        if (pvList.size()>0 )
        {
            groupDefine((const char *) (*group).getName().c_str(), pvList);

        }
        else
        {
            cout << "GROUP "  << (*group).getName().c_str()
                 <<  " NOT DEFINED AS IT HAS NO MEMBERS! " << endl;
        }

    }
    return ICAFE_NORMAL;

#undef __METHOD__
}





/**
 *  \brief restoreFromXML \n
 *  Reads a PVGroup XML file and writes values to EPICS variables \n
 *  from the directory defined the by CAFE_SAR_PATH environment variable
 *  \param snapshotFile input: char * snapshotFile
 *  \return ICAFE_NORMAL if OK else first ECAFE/ICAFE error encountered
 */
int  CAFE::restoreFromXML     (const char * snapshotFile)
{
#define __METHOD__ "CAFE::restoreFromXML(char * snapshotFile)"

    int  localStatus=ICAFE_NORMAL;

    unsigned int  gh;

    string ssf=snapshotFile;

    QFile * file;

    file = new QFile(snapshotFile);


    if (!file->exists())
    {

        std::string envS;
        char * env = getenv("CAFE_SAR_PATH");
        env == NULL ? envS=std::string(".") : envS=std::string(env);
        envS.append("/");
        envS.append(snapshotFile);

        file = new QFile(envS.c_str());

        if (!file->exists())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cout << "PVGROUP FILE " << snapshotFile << " NOT FOUND " << endl;
            cout << "NEITHER IN THE CURRENT (OR GIVEN) DIRECTORY NOR IN CAFE_SAR_PATH=" << envS << endl;
            return ECAFE_LOAD_GROUP;
        }

        ssf=envS;
    }
    else
    {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << " PVGROUP LOADED FROM CURRENT (OR GIVEN) DIRECTORY" << endl;
    }


    QXmlInputSource source(file);

    restorePVGroupXMLParser handler;

    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);
    reader.setFeature("http://trolltech.com/xml/features/report-whitespace-only-CharData", false);

    bool isOK = reader.parse(source);

    if (!isOK)
    {
        return ECAFE_LOAD_GROUP;
    }

    PVGroup pvg = handler.group;

    string gNameNew= pvg.name;
    gNameNew.append("Restore");
    strcpy(pvg.name,gNameNew.c_str());

    PVGroupV.push_back(pvg);

    localStatus=groupOpen(pvg, gh);

    if (localStatus != ICAFE_NORMAL)
    {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "GROUP OPEN REPORTS FOLLOWING ERROR " << endl;
        for (unsigned int  i=0; i<pvg.npv; ++i )
        {
            if(pvg.pvdata[i].status !=ICAFE_NORMAL)
            {
                pvg.pvdata[i].print();
            }
        }
    }



    //Now parse a second time to read in values!

    restorePVGroupXMLParser handler2;
    QXmlSimpleReader reader2;
    reader2.setContentHandler(&handler2);
    reader2.setFeature("http://trolltech.com/xml/features/report-whitespace-only-CharData", false);

    QFile file2(ssf.c_str());
    QXmlInputSource source2(&file2);
    isOK = reader2.parse(source2);

    if (!isOK)
    {
        return ECAFE_LOAD_GROUP;
    }

    pvg =handler2.group;
    pvg.groupHandle=gh;
    strcpy(pvg.name,gNameNew.c_str());

    localStatus=groupSet(gh, pvg);

    if (localStatus != ICAFE_NORMAL)
    {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "GROUP SET REPORTS FOLLOWING ERROR " << endl;
        for (unsigned int  i=0; i<pvg.npv; ++i )
        {
            if(pvg.pvdata[i].status !=ICAFE_NORMAL)
            {
                pvg.pvdata[i].print();
            }
        }
    }

    groupClose(gh);

    return localStatus;

#undef __METHOD__
}

#endif //HAVE_LIBATXML


/**
 *  \brief snapshot2XML \n
 *  Takes a PVGroup snapshot and writes data to a timestamped file \n
 *  in the directory defined the by CAFE_SAR_PATH environment variable
 *  \param pg input: PVGroup pg
 *  \return ICAFE_NORMAL if OK else first ECAFE/ICAFE error encountered
 */
int  CAFE::snapshot2XML (PVGroup pg)
{
#define __METHOD__ "CAFE::snapshot2XML(PVGroup pg)"

    std::string s="<cafe:config>\n";
    s.append("<cafe:group id=\"");

    std::string gname=pg.getName();

    s.append(gname);
    s.append("\">\n");
    char sBuffer[60];
    char sBufferNPV[60];

    int  overallStatus=groupGet(pg.getGroupHandle(),pg);
    PVDataHolder * pvd = pg.getPVData();

    s.append("<cafe:npv> ");
    sprintf(sBufferNPV,"%u", pg.npv);
    s.append(sBufferNPV);
    s.append(" </cafe:npv>\n");

    //First determine maximum pv size
    unsigned short maxL=0;
    std::string sn="";
    for (unsigned int  j=0; j<pg.getNPV(); ++j)
    {
        sn=pvd[j].getPV();
        if(sn.size() > maxL) maxL=sn.size()+0;
    }
    for (unsigned int  j=0; j<pg.getNPV(); ++j)
    {
        s.append("<cafe:member><cafe:name> ");
        s.append(pvd[j].getPV());
        sn=pvd[j].getPV();
        s.append((maxL-sn.size()),' ');

        s.append(" </cafe:name><cafe:nelem> ");
        sprintf(sBuffer,"%d", pvd[j].getNelem());
        s.append(sBuffer);
        s.append(" </cafe:nelem><cafe:val> ");

        for (unsigned int  i=0; i<pvd[j].getNelem(); ++i)
        {
            s.append(pvd[j].getAsString(i));
            s.append(" ");
        }
        s.append("</cafe:val><cafe:settable> ");

        unsigned int hl=getHandleHelper().getHandleFromPVWithinGroup(pvd[j].getPV(), pg.getGroupHandle() );

        ChannelRegalia cr;
        getInfo().getChannelRegalia(hl, cr);

        if ( cr.getWriteAccess() )
        {
            s.append("true");
        }
        else
        {
            s.append("false");
        }

        s.append(" </cafe:settable><cafe:status> ");

        sprintf(sBuffer,"%d", pvd[j].getStatus());

        s.append(sBuffer);
        s.append(" </cafe:status>");
        s.append("</cafe:member>\n");
    }
    s.append("</cafe:group>\n</cafe:config>");


    time_t now = time(0);

    tm *ltm = localtime(&now);

    std::string envS;
    char * env = getenv("CAFE_SAR_PATH");
    env == NULL ? envS=std::string(".") : envS=std::string(env);
    envS.append("/");


    std::string fsname=envS;
    fsname.append(pg.getName());
    char s1[255];
    sprintf(s1,"_%d_%d_%d_%d:%d:%d", (1900 + ltm->tm_year),(1+ltm->tm_mon),
            ltm->tm_mday,ltm->tm_hour,ltm->tm_min,ltm->tm_sec );
    fsname.append(s1);
    fsname.append(".xml");


    std::ofstream myfile;
    myfile.open (fsname.c_str(), std::ios::out);
    myfile << s;
    myfile.close();

    return overallStatus;
#undef __METHOD__
}






/**
 *  \brief openGroupXMLFile \n
 *  Open file given by fileName for writing XML data\n
 *  \param fileName input: filename to which PVGroup data is written
 *  \return ICAFE_NORMAL if OK else first ECAFE/ICAFE error encountered
 */
void CAFE::openGroupXMLFile(std::string fileName)
{
    std::ofstream myfile;
    myfile.open (fileName.c_str(), std::ios::out);
    std::string s="<cafe:config>\n";
    myfile << s;
    return myfile.close();
}


/**
 *  \brief closeGroupXMLFile \n
 *  Close file given by fileName for writing XML data\n
 *  \param fileName input: filename to which PVGroup data is written
 *  \return ICAFE_NORMAL if OK else first ECAFE/ICAFE error encountered
 */
void CAFE::closeGroupXMLFile(std::string fileName)
{
    std::ofstream myfile;
    myfile.open (fileName.c_str(), std::ios::app);
    std::string s="</cafe:config>\n";
    myfile << s;
    return  myfile.close();
}


/**
 *  \brief group2XML \n
 *  Takes a PVGroup and writes its members to a file \n
 *  in the directory defined the by CAFE_SAR_PATH environment variable
 *  \param grpName input: PVGroup
 *  \param fileName input: filename to which PVGroup data is written
 *  \return ICAFE_NORMAL if OK else first ECAFE/ICAFE error encountered
 */
int  CAFE::group2XML (const char * grpName, std::string fileName)
{
#define __METHOD__ "CAFE::group2XML(const char *grpName, ofstream myfile)"

    std::ofstream myfile;
    myfile.open (fileName.c_str(), std::ios::app);

    std::string s= "\n  <cafe:group id=\"" ;
    std::string gname=grpName;
    s.append(gname);
    s.append("\">\n");

    std::vector<std::string> lg;
    lg.clear();

    int status=CAFE::groupMemberList(grpName, lg);


    for (unsigned int j=0; j<lg.size(); ++j)
    {
        s.append("    <cafe:member> <cafe:name>  ");
        s.append(lg[j]);
        s.append("  </cafe:name> </cafe:member>\n");
    }
    s.append("  </cafe:group>\n");

    myfile << s;
    myfile.close();
    return status;
#undef __METHOD__
}
