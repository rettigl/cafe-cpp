///
/// \file    handleHelper.cpp
/// \author  Jan Chrin, PSI
/// \date    Release, February 2015
/// \version CAFE 1.0.0
///

#include <handleHelper.h>
#include <policyHelper.h>
#include <global.h>

using namespace std;

#if HAVE_PYTHON_H
/**
 *  \brief HandleHelper::setPyGetCallbackFn \n
 *  Set Python callback function for asynchronous get operations\n
 *
 *  \param  _handle input: handle identifying Conduit object
 *  \param  callbackFn input: void * callback function
 *  \return ICAFE_NORMAL if OK else ECAFE_INVALID_HANDLE
 */
int HandleHelper::setPyGetCallbackFn(unsigned int _handle, void * callbackFn)
{
#define __METHOD__ "HandleHelper::setPyGetCallbackFn(unsigned int _handle, void * callbackFn"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        handle_index.modify(it_handle, change_pyGetCallbackFn(callbackFn));
    }
    else
    {
        cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;

#undef __METHOD__
}


/**
 *  \brief HandleHelper::setPyPutCallbackFn \n
 *  Set Python callback function for asynchronous put operations\n
 *
 *  \param  _handle input: handle identifying Conduit object
 *  \param  callbackFn input: void * callback function
 *  \return ICAFE_NORMAL if OK else ECAFE_INVALID_HANDLE
 */
int HandleHelper::setPyPutCallbackFn(unsigned int _handle, void * callbackFn)
{
#define __METHOD__ "HandleHelper::setPyPutCallbackFn(unsigned int _handle, void * callbackFn"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        handle_index.modify(it_handle, change_pyPutCallbackFn(callbackFn));
    }
    else
    {
        cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;

#undef __METHOD__
}


/**
 *  \brief HandleHelper::setPyConnectCallbackFn \n
 *  Set Python callback function invoked on connect/disconnect\n
 *
 *  \param  _handle input: handle identifying Conduit object
 *  \param  callbackFn input: void * callback function
 *  \return ICAFE_NORMAL if OK else ECAFE_INVALID_HANDLE
 */
int HandleHelper::setPyConnectCallbackFn(unsigned int _handle, void * callbackFn)
{
#define __METHOD__ "HandleHelper::setPyConnectCallbackFn(unsigned int _handle, void * callbackFn "

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        bool _pycbflag = true;
        handle_index.modify(it_handle, change_pyOpenCallbackFlag(_pycbflag));
        handle_index.modify(it_handle, change_pyConnectCallbackFn(callbackFn));
	//cout << "Handle=" << _handle << __METHOD__ << endl;
	//cout << callbackFn << endl;
    }
    else
    {
        cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;

#undef __METHOD__
}


#endif


/**
 *  \brief HandleHelper::addWidget \n
 *  Inserts widget to vector of widgets associated to handle\n
 *
 *  \param  _handle input: handle identifying Conduit object
 *  \param   eidget input: void * giving the widget
 *  \return ICAFE_NORMAL if OK else ECAFE_INVALID_HANDLE
 */
int HandleHelper::addWidget(unsigned int _handle, void * widget)
{
#define __METHOD__ "HandleHelper::addWidget(unsigned int _handle, void * widget"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        handle_index.modify(it_handle, change_widgetInsert(widget));
    }
    else
    {
        cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;

#undef __METHOD__
}


/**
 *  \brief Get widgets associated to handle.
 *  Typically one widget is associated to a handle (but can be more).
 *  \param _handle input : Conduit object reference
 *  \return returns vector of widgets; length 0 is none or invalid handle
 */
int HandleHelper::getWidgets(unsigned int _handle, std::vector<void *> &widgetV)
{
#define __METHOD__ \
    "HandleHelper::getWidgets(unsigned int _handle, vector<void *> &widgetV"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    int status=ICAFE_NORMAL;

    if (it_handle != handle_index.end())
    {
        widgetV=vector<void *>((*it_handle).getWidgetV());
    }
    else
    {
        status=ECAFE_INVALID_HANDLE;
        if (printErrorPolicy.getInvalidHandle())
        {
            cafeStatus.report(status);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return status;
#undef __METHOD__

}


/**
 *  \brief HandleHelper::removeWidget \n
 *  Removes widget from vector of widgets associated to handle\n
 *
 *  \param  _handle input: handle identifying Conduit object
 *  \param   widget input: void * giving the widget to remove
 *  \return ICAFE_NORMAL if OK else ECAFE_INVALID_HANDLE
 */
int HandleHelper::removeWidget(unsigned int _handle, void * widget)
{
#define __METHOD__ "HandleHelper::removeWidget(unsigned int _handle, void * widget"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        handle_index.modify(it_handle, change_widgetErase(widget));
    }
    else
    {
        cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;

#undef __METHOD__
}





/**
 *  \brief Checks what's in the hash table against actual ca server data \n
 *  Do for all handles
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int HandleHelper::checkConsistency()
{
#define __METHOD__ "HandleHelper::checkConsistency()"

    int gStatus=ICAFE_NORMAL;
    int localStatus;

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        localStatus=checkConsistency( (*itcs).getHandle());
        if (localStatus !=ICAFE_NORMAL)
        {
            gStatus=localStatus;
        }
    }
    return gStatus;
#undef __METHOD__
}


/**
 *  \brief Checks what's in the hash table against actual ca server data
 *  \param _handle input : Conduit object reference
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int HandleHelper::checkConsistency(unsigned int _handle)
{
#define __METHOD__ "HandleHelper::checkConsistency(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {

        unsigned int nflag=0;
        string message="";
        ChannelRegalia chInfo=(*it_handle).getChannelRegalia();

        if (chInfo.getChannelID()==NULL) return ECAFE_NULLCHID;

        int op=chInfo.getConnectionState();

        if (op==CA_OP_CONN_UP)
        {
            if (ca_state(chInfo.getChannelID()) !=  cs_conn)
            {
                chInfo.setConnectionState   (CA_OP_CONN_DOWN);
                chInfo.setCafeConnectionState(ICAFE_CS_DISCONN);
                chInfo.setConnectFlag(false);
                nflag=1;
                message.append("CONNECT TRUE WHEN FALSE\n");
            }
            if (ca_element_count(chInfo.getChannelID()) != chInfo.getNelem())
            {
                chInfo.setNelem       (ca_element_count(chInfo.getChannelID()));
                message.append("NELEM COUNT WRONG IN HASH TABLE\n");
                nflag=10;
            }

            if (ca_field_type(chInfo.getChannelID()) !=   chInfo.getDataType())
            {
                chInfo.setDataType    (ca_field_type(chInfo.getChannelID()));
                message.append("NATIVE DATATYPE WRONG IN HASH TABLE\n");
                nflag=100;
            }
        }
        else
        {
            if (ca_state(chInfo.getChannelID()) ==  cs_conn)
            {
                chInfo.setConnectionState   (CA_OP_CONN_UP);
                chInfo.setCafeConnectionState(ICAFE_CS_CONN);
                chInfo.setConnectFlag(true);
                nflag=2;
                message.append("CONNECT FALSE WHEN TRUE\n");
            }
        }

        if (ca_read_access( chInfo.getChannelID()) != chInfo.getReadAccess())
        {
            chInfo.setReadAccess  (ca_read_access(chInfo.getChannelID()));
            message.append("READ ACCESS WRONG IN HASH TABLE\n");
            nflag=1000;
        }

        if (ca_write_access(chInfo.getChannelID()) != chInfo.getWriteAccess())
        {
            chInfo.setWriteAccess (ca_write_access(chInfo.getChannelID()));
            message.append("WRITE ACCESS WRONG IN HASH TABLE\n");
            nflag=10000;
        }


        if (nflag>0)
        {
            std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
            std::cout << "Following Corrections Made:" << std::endl;
            std::cout << message << endl;
            //else {
            //	std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
            //	std::cout << "CA CONSISTENCY VERIFIED " << std::endl;
            //}


            if(MUTEX)
            {
                cafeMutex.lock();
            }
            handle_index.modify(it_handle, change_channelRegalia(chInfo));
            if(MUTEX)
            {
                cafeMutex.unlock();
            }

            return ECAFE_INCONSISTENT_CONTAINER_CORRECTED;

        }

    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef __METHOD__
}



/**
 *  \brief Retrieves whether pv is of enumerated type
 *  \param _handle  input: handle
 *  \return true if enum type else false
 */
bool HandleHelper::isEnum(unsigned int _handle)
{
#define __METHOD__ "HandleHelper::isEnum(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        if  (!(*it_handle).isConnected() &&  (*it_handle).isConnected() == ICAFE_CS_NEVER_CONN )
        {
            return false;
        }

        PVCtrlHolder  _pvc;
        unsigned int nelem=getNelemRequestCtrl(_handle);

        if (nelem >1)
        {
            setNelemToRetrieveFromCtrlCache(_handle,1);
        }
        _pvc.setNelem(nelem);

        int _stat = (*it_handle).getPVCtrlHolder(_pvc) ;

        //return to previous
        if (nelem!=1)
        {
            setNelemToRetrieveFromCtrlCache(_handle,nelem);
        }
        if (_stat==ICAFE_NORMAL)
        {
            if (_pvc.getNoEnumStrings()>0 )
            {
                // cafeBeta 1.0 uses shared pointer
                //_pvc.deleteVal();
                return true;
            }
        }
        // cafeBeta 1.0 uses shared pointer
        //_pvc.deleteVal();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return false;
    }

    return false;
#undef __METHOD__
}



/**
 *  \brief Retrieves enum unsigned short state of enumerated string type
 *  \param _handle  input: handle
 *  \param enumStringValue input: enum string state
 *  \return short enum value else -1
 */
short HandleHelper::getEnumFromString(unsigned int _handle, std::string enumStringValue)
{
#define __METHOD__ "HandleHelper::getEnumFromString(unsigned int _handle, string enumStringValue)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        //Never connected so do not know
        if  (!(*it_handle).isConnected() &&  (*it_handle).getStatus() == ICAFE_CS_NEVER_CONN )
        {
            return -1;
        }

        PVCtrlHolder  _pvc;

        unsigned int nelem=getNelemRequestCtrl(_handle);

        if (nelem >1)
        {
            setNelemToRetrieveFromCtrlCache(_handle,1);
        }
        _pvc.setNelem(nelem);

        int _stat = (*it_handle).getPVCtrlHolder(_pvc) ;

        //return to previous
        if (nelem != 1)
        {
            setNelemToRetrieveFromCtrlCache(_handle,nelem);
        }
        if (_stat==ICAFE_NORMAL)
        {
            return _pvc.getEnumFromString(enumStringValue);
        }
        else
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(_stat);
            cout << "Error for handle=" << _handle << " in (*it_handle).getPVCtrlHolder(_pvc) " << endl;
            return -1;
        }
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return -1;
    }

    return false;
#undef __METHOD__
}



/**
 *  \brief Retrieves string value of enum unsigned short
 *  \param _handle  input: handle
 *  \param enumValue input: unsigned short enum value
 *  \return string stringValue else ""
 */
string HandleHelper::getStringFromEnum(unsigned int _handle, unsigned short enumValue)
{
#define __METHOD__ "HandleHelper::getStringFromEnum(unsigned int _handle, unsigned short enumValue)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        //Never connected so do not know
        if  (!(*it_handle).isConnected() &&  (*it_handle).getStatus() == ICAFE_CS_NEVER_CONN )
        {
            return (string) "";
        }

        PVCtrlHolder  _pvc;
        unsigned int nelem=getNelemRequestCtrl(_handle);

        if (nelem >1)
        {
            setNelemToRetrieveFromCtrlCache(_handle,1);
        }
        _pvc.setNelem(nelem);

        int _stat = (*it_handle).getPVCtrlHolder(_pvc) ;

        //return to previous
        if (nelem!=1)
        {
            setNelemToRetrieveFromCtrlCache(_handle,nelem);
        }
        if (_stat==ICAFE_NORMAL)
        {
            return _pvc.getStringFromEnum(enumValue);
        }
        else
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(_stat);
            cout << "Error for handle=" << _handle << " in (*it_handle).getPVCtrlHolder(_pvc) " << endl;
            return (string) "";
        }
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return (string) "";
    }

    return (string) "";
#undef __METHOD__
}


/**
 *  \brief Retrieves vector<string> value options of enum unsigned short
 *  \param _handle  input: handle
 *  \return string stringValue else ""
 */
vector<std::string> HandleHelper::getEnumStrings(unsigned int _handle)
{
#define __METHOD__ "HandleHelper::getEnumStrings(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    vector<std::string> stringEnumValues;

    if (it_handle != handle_index.end())
    {

        //Never connected so do not know
        if  (!(*it_handle).isConnected() &&  (*it_handle).getStatus() == ICAFE_CS_NEVER_CONN )
        {
            return stringEnumValues; //empty
        }

        PVCtrlHolder  _pvc;
        unsigned int nelem=getNelemRequestCtrl(_handle);

        if (nelem >1)
        {
            setNelemToRetrieveFromCtrlCache(_handle,1);
        }
        _pvc.setNelem(nelem);

        int _stat = (*it_handle).getPVCtrlHolder(_pvc) ;

        //return to previous
        if (nelem!=1)
        {
            setNelemToRetrieveFromCtrlCache(_handle,nelem);
        }
        if (_stat==ICAFE_NORMAL)
        {
            return _pvc.getEnumStrings();
        }
        else
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(_stat);
            cout << "Error for handle=" << _handle << " in (*it_handle).getPVCtrlHolder(_pvc) " << endl;
            return stringEnumValues;
        }
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return stringEnumValues; //empty
    }

    return stringEnumValues; //empty
#undef __METHOD__
}





/**
 *  \brief Gets the user arg as uint in event callback; meant for retrieving the current monitor ID
 *  \param _handle  input: handle
 *  \return (uint) usr.args; meant for monitorID
 */
unsigned int HandleHelper::getUsrArgsAsUInt(unsigned int _handle)
{
#define __METHOD__ "unsigned int getUsrArgsAsUInt(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    unsigned long monid=0;

    if (it_handle != handle_index.end())
    {
        monid = (unsigned long) (*it_handle).getUsrArgs();
        //monid = (unsigned long) (*it_handle).getChannelRequestMetaData().getUsrArg();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return monid;
#undef __METHOD__
}

/**
 *  \brief Gets the datatype of handle from within the callback function
 *  \param _handle  input: handle
 *  \return chtype dataType
 */
chtype HandleHelper::getDataTypeCB(unsigned int _handle)
{
#define __METHOD__ "chtype getDataTypeCB(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    chtype chval=0;

    if (it_handle != handle_index.end())
    {
        chval = (unsigned long) (*it_handle).getDataType();

    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return chval;
#undef __METHOD__
}


/**
 *  \brief Gets the dbrdatatype of handle from within the callback function
 *  \param _handle  input: handle
 *  \return chtype dataType
 */
chtype HandleHelper::getDbrDataTypeCB(unsigned int _handle)
{
#define __METHOD__ "chtype getDbrDataTypeCB(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    chtype chval=0;

    if (it_handle != handle_index.end())
    {
        chval = (unsigned long) (*it_handle).getDbrDataType();

    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return chval;
#undef __METHOD__
}


/**
 *  \brief Gets the cafeDbrType of handle from within the callback function
 *  \param _handle  input: handle
 *  \return CAFENUM::DBR_TYPE cafeDbrDataType
 */
CAFENUM::DBR_TYPE HandleHelper::getCafeDbrTypeCB(unsigned int _handle)
{
#define __METHOD__ "CAFENUM:DBR_TYPE  getCafeDbrTypeCB(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    CAFENUM::DBR_TYPE chval=CAFENUM::DBR_NONE;

    if (it_handle != handle_index.end())
    {
        chval =  (*it_handle).getCafeDbrType();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return chval;
#undef __METHOD__
}


/**
 *  \brief Retrieves the native data type from ChannelRegalia object
 *  \param _handle  input: handle
 *  \param ndt output: CAFE_DATATYPE object
 *  \return ICAFE_NORMAL if all OK else ECAFE_INVALID_HANDLE
 */
int HandleHelper::getDataTypeNative(unsigned int _handle, chtype & ndt)
{
#define __METHOD__ "HandleHelper::getDataTypeNative(unsigned int _handle, CAFE_DATATYPE &cdt)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        ndt = (*it_handle).getChannelRegalia().getDataType();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef __METHOD__
}







/**
 *  \brief Retrieves channel meta deta held in ChannelRequestStatusGetClassName object
 *  \param _handle  input: handle
 *  \param crsClassName output: ChannelRequestStatus instance
 *  \return ICAFE_NORMAL if all OK else ECAFE_INVALID_HANDLE
 */
int  HandleHelper::getChannelRequestStatusGetClassName(unsigned int  _handle, ChannelRequestStatus &crsClassName)
{
#define __METHOD__ "HandleHelper::getChannelRequestStatusGetClassName(unsigned int _handle, ChannelRequestStatus &crsClassName)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        crsClassName = (*it_handle).getChannelRequestStatusGetClassName();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;

#undef __METHOD__
}


/**
 *  \brief Retrieves channel meta deta held in ChannelRequestStatusGetSTSACK object
 *  \param _handle  input: handle
 *  \param crsSTSACK output: ChannelRequestStatus instance
 *  \return ICAFE_NORMAL if all OK else ECAFE_INVALID_HANDLE
 */
int  HandleHelper::getChannelRequestStatusGetSTSACK(unsigned int  _handle, ChannelRequestStatus &crsSTSACK)
{
#define __METHOD__ "HandleHelper::getChannelRequestStatusGetSTACK(unsigned int _handle, ChannelRequestStatus &crsSTSACK)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        crsSTSACK = (*it_handle).getChannelRequestStatusGetSTSACK();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;

#undef __METHOD__
}



/**
 *  \brief Retrieves channel meta deta held in ChannelRequestStatusGetCtrl object
 *  \param _handle  input: handle
 *  \param crsCtrl output: ChannelRequestStatus instance
 *  \return ICAFE_NORMAL if all OK else ECAFE_INVALID_HANDLE
 */
int  HandleHelper::getChannelRequestStatusGetCtrl(unsigned int  _handle, ChannelRequestStatus &crsCtrl)
{
#define __METHOD__ "HandleHelper::getChannelRequestStatusGetCtrl(unsigned int _handle, ChannelRequestStatus &crsCtrl)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        crsCtrl = (*it_handle).getChannelRequestStatusGetCtrl();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;

#undef __METHOD__
}




/**
 *  \brief Retrieves channel meta deta held in ChannelRequestStatusGet object
 *  \param _handle  input: handle
 *  \param crs output: ChannelRequestStatus instance
 *  \return ICAFE_NORMAL if all OK else ECAFE_INVALID_HANDLE
 */
int  HandleHelper::getChannelRequestStatusGet    (unsigned int  _handle, ChannelRequestStatus &crs)
{
#define __METHOD__ "HandleHelper::getChannelRequestStatusGet(unsigned int _handle, ChannelRequestStatus &crs)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        crs = (*it_handle).getChannelRequestStatusGet();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef __METHOD__
}



/**
 *  \brief Retrieves channel meta deta held in the ChannelRegalia object
 *  \param _handle  input: handle
 *  \param channelInfo output: ChannelRegalia object
 *  \return ICAFE_NORMAL if all OK else ECAFE_INVALID_HANDLE
 */
int HandleHelper::getChannelRegalia(unsigned int _handle, ChannelRegalia & channelInfo)
{
#define __METHOD__ "HandleHelper::getChannelInfo(unsigned int _handle, ChannelRegalia channelInfo)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        channelInfo = (*it_handle).getChannelRegalia();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef __METHOD__
}

/**
 *  \brief Given the deliminator, sets the device/attribute components of the epics channel, for all handles
 *  \param deliminator deliminator: string (at PSI this is ususally a colon, i.e. ":")
 *  \return ICAFE_NORMAL if all OK else ERROR
 */
int HandleHelper::setChannelDeviceAttribute(std::string deliminator)
{

    int localStatus=ICAFE_NORMAL;
    int gStatus=ICAFE_NORMAL;
    bool sflag=false;

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        localStatus= setChannelDeviceAttribute ((*itcs).getHandle(), deliminator);
        if (localStatus !=ICAFE_NORMAL && !sflag)
        {
            gStatus=localStatus;
            sflag=true;
        }
    }
    return gStatus;
}

/**
 *  \brief Given the deliminator, sets the device/attribute components of the epics channel for a givem handle
 *  \param _handle input: handle to Conduit Objecte
 *  \param deliminator input: string (at PSI this is ususally a colon, i.e. ":")
 *  \return ICAFE_NORMAL if all OK else ERROR
 */
int HandleHelper::setChannelDeviceAttribute(unsigned int _handle, std::string deliminator)
{
#define __METHOD__ "HandleHelper::setChannelDeviceAttribute(unsigned int _handle, std::string deliminator)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);


    if (it_handle != handle_index.end())
    {

        ChannelDeviceAttribute channelDeviceAttribute;
        channelDeviceAttribute.init((*it_handle).pv, deliminator);

        if(MUTEX)
        {
            cafeMutex.lock();
        }

        handle_index.modify(it_handle, change_channelDeviceAttribute(
                                channelDeviceAttribute));

        if(MUTEX)
        {
            cafeMutex.unlock();
        }

    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef __METHOD__
}



/**
 *  \brief Retrieves pv units for a given handle
 *  \param _handle input: handle to Conduit Object
 *  \param precision output: device precision
 *  \return ICAFE_NORMAL if all OK else ERROR
 */
int HandleHelper::getPrecision(unsigned int _handle, short & precision)
{
#define __METHOD__ "HandleHelper::getPrecision"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        precision = (*it_handle).getPrecision();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;
#undef __METHOD__
}


/**
 *  \brief Retrieves pv units for a given handle
 *  \param _handle input: handle to Conduit Object
 *  \param units output: pv units
 *  \return ICAFE_NORMAL if all OK else ERROR
 */
int HandleHelper::getUnits(unsigned int _handle, std::string & units)
{
#define __METHOD__ "HandleHelper::getUnits"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        units = (*it_handle).getUnits();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;
#undef __METHOD__
}



/**
 *  \brief Retrieves pv description for a given handle
 *  \param _handle input: handle to Conduit Object
 *  \param desc output: pv description
 *  \return ICAFE_NORMAL if all OK else ERROR
 */
int HandleHelper::getDescription(unsigned int _handle, std::string & desc)
{
#define __METHOD__ "HandleHelper::getDescription"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        desc = (*it_handle).getDescription();

    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;
#undef __METHOD__
}



/**
 *  \brief  Whether description field has been filled for a given handle
 *  \param _handle input: handle to Conduit Object
 *  \param desc output: true if description field has been filled
 *  \return ICAFE_NORMAL if all OK else ERROR
 */
bool HandleHelper::hasDescription(unsigned int _handle)
{
#define __METHOD__ "HandleHelper::getDescription"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;

    bool hasDesc=false;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        if  (!(*it_handle).isConnected() &&  (*it_handle).getStatus() == ICAFE_CS_NEVER_CONN )
        {
            return hasDesc;
        }

        hasDesc = (*it_handle).hasDescription();

    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return hasDesc;
    }


    return hasDesc;
#undef __METHOD__
}




/**
 *  \brief Retrieves device name for a given handle
 *  \param _handle input: handle to Conduit Object
 *  \param device output: device name
 *  \return ICAFE_NORMAL if all OK else ERROR
 */
int HandleHelper::getChannelDevice(unsigned int _handle, std::string & device)
{
#define __METHOD__ "HandleHelper::getChannelDevice"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        device = (*it_handle).getDevice();

    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef __METHOD__
}

/**
 *  \brief Retrieves attribute name for a given handle
 *  \param _handle input: handle to Conduit Object
 *  \param attribute output: attribute name
 *  \return ICAFE_NORMAL if all OK else ERROR
 */
int HandleHelper::getChannelAttribute(unsigned int _handle, std::string & attribute)
{
#define __METHOD__ "HandleHelper::getChannelAttribute"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        attribute = (*it_handle).getAttribute();

    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef __METHOD__
}



/**
 *  \brief Retrieves the total number of handled
 *  \return noHandles: Total no. of handles
 */
unsigned int HandleHelper::getNoHandles()
{
#define __METHOD__ "HandleHelper::getNoHandles()"

    unsigned int nHandle=0;

    // Loop through all elements;
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        ++nHandle;
    }
    return nHandle;
#undef __METHOD__
}




/**
 *  \brief Retrieves the next available handle
 *  \return handle: value of next available handle
 */
unsigned int HandleHelper::getNextFreeHandle()
{
#define __METHOD__ "HandleHelper::getNextFreeHandle()"

    unsigned int freeHandle=0;

    vector<unsigned int> handleVector;

    handleVector.reserve(getNoHandles());

    // Loop through all elements;
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        handleVector.push_back((*itcs).getHandle());
    }

    std::sort (handleVector.begin(), handleVector.begin());

    for (std::vector<unsigned int>::iterator itui=handleVector.begin(); itui!=handleVector.end(); ++itui)
    {
        ++freeHandle;
        if ( (*itui) !=freeHandle)
        {
            return freeHandle;
        }
    }
    //const unsigned int min_int = std::numeric_limits<unsigned int>::min();
    const unsigned int max_int = std::numeric_limits<unsigned int>::max();

    if (freeHandle == max_int)
    {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "THE MAXIMUM ALLOWED NUMBER OF HANDLES HAS BEEN REACHED " << endl;
        cout << "THIS MAXIMUM VALUE IS " << max_int << endl;
    }

    return ++freeHandle; //this will give zero if max_int reached
#undef __METHOD__
}



/**
 *  \brief Retrieves context for a first matching of this process variable
 *  \param _pv  input: process variable
 *  \return ccc : ca_client_context (NULL if not found)
 */
ca_client_context *  HandleHelper::getContextFromPV(const char * _pv)
{
#define __METHOD__ "HandleHelper::getContextFromPV(const char * _pv)"

    cafeConduit_set_by_pv & pv_index = cs.get<by_pv> ();
    cafeConduit_set_by_pv::iterator it_pv;

    char pv[PVNAME_SIZE];
    removeLeadingAndTrailingSpaces(_pv, pv);

    it_pv = pv_index.find(pv);

    // Three possibilities of getting a match!
    if (it_pv != pv_index.end())
    {
        // Examine ca_client_context noting that channels within a group dO count!
        return (*it_pv).getClientContext() ;
    }
    else
    {
        // Loop through all elements and search for pv match
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
        {
            //if((*itcs).getGroupHandle()>0) {continue;} // Channels within a group don't count!
            if (!strcmp((*itcs).getPV(), _pv))
            {
                //cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                //out << " INFORMATION to author: MATCHed Handle= " << (*itcs).handle << " to PV= " << _pv << endl;
                //cout << " by looping through tcs::iterator, while the pv::iterator was NOT found! " << endl;
                return (*itcs).getClientContext();
            }
        }
    }

    return NULL;
#undef __METHOD__
}




/**
 *  \brief  Retrieves the ca_client_context for first matching of handle
 *  \param  handle  input: handle
 *  \return ccc output: ca_client_context
 */
ca_client_context * HandleHelper::getContextFromHandle(unsigned int handle)
{
#define __METHOD__ "HandleHelper::getContextFromHandle(unsigned int handle)"

    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);

    if (it_handle != handle_index.end())
    {
        return (*it_handle).getClientContext();
    }
    else
    {
        // Loop through all elements and search for handle/ca_client_context match
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
        {
            // if((*itcs).getGroupHandle()>0) {continue;} // Channels within a group don't count!

            if ( (*itcs).getHandle()==handle)
            {
                //cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                //cout << " INFORMATION to author: MATCHed Handle= " << handle << " to PV= " << (*itcs).getPV() << endl;
                //cout << " by looping through tcs::iterator, while the by_handle::iterator was NOT found! " << endl;
                return (*itcs).getClientContext();
            }
        }
    }
    //cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
    //cout << "PV for Handle=" << handle << " not found " << endl;

    return NULL;
#undef __METHOD__
}




/**
 *  \brief Retrieves handle for a given process variable
 *  \param _pv  input: process variable
 *  \return handle else 0 if no handle exists
 */
unsigned int HandleHelper::getHandleFromPV(const char * _pv)
{
#define __METHOD__ "HandleHelper::getHandleFromPV(const char * _pv)"

    ca_client_context * ccc = ca_current_context();

    return getHandleFromPV(_pv,ccc);

#undef __METHOD__
}


/**
 *  \brief Retrieves handle for a given process variable
 *  \param _pv  input: process variable
 *  \return handle else 0 if no handle exists
 */
unsigned int HandleHelper::getHandleFromPVAlias(const char * _pv)
{
#define __METHOD__ "HandleHelper::getHandleFromPVAlias(const char * _pv)"

    ca_client_context * ccc = ca_current_context();


    return getHandleFromPVAlias(_pv,ccc);

#undef __METHOD__
}



/**
 *  \brief Retrieves handle for a given process variable
 *  \param _pv  input: process variable
 *  \param ccc input: ca_client_context
 *  \return handle else 0 if no handle exists
 */
unsigned int HandleHelper::getHandleFromPV(const char * _pv, ca_client_context * ccc)
{
#define __METHOD__ "HandleHelper::getHandleFromPV(const char * _pv, ca_client_context * ccc)"

    cafeConduit_set_by_pv & pv_index = cs.get<by_pv> ();
    cafeConduit_set_by_pv::iterator it_pv;

    char pv[PVNAME_SIZE];
    removeLeadingAndTrailingSpaces(_pv, pv);

    it_pv = pv_index.find(pv);


    // Three possibilities of getting a match!
    if (it_pv != pv_index.end())
    {

        // Examine ca_client_context noting that channels within a group don't count!
        if (ccc == (*it_pv).getClientContext() &&  (*it_pv).getGroupHandle()==0 )
        {
            //std::cout << "One: " << (*it_pv).handle << std::endl;
            return (*it_pv).handle;
        }
        else
        {
            // Loop through all elements and search for pv/ca_client_context match
            for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
            {
                if((*itcs).getGroupHandle()>0)
                {
                    continue;   // Channels within a group don't count!
                }

                if (!strcmp((*itcs).getPV(), _pv) && (*itcs).getClientContext()== ccc)
                {
                    //std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
                    //std::cout << " INFORMATION to author: MATCHed Handle= " << (*itcs).handle << " to PV= " << _pv << std::endl;
                    //std::cout << " by looping through tcs::iterator, even though pv::iterator was found! " << std::endl;

                    return (*itcs).handle;
                }
            }
        }
    }
    else
    {
        // Loop through all elements and search for pv/ca_client_context match
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
        {
            if((*itcs).getGroupHandle()>0)
            {
                continue;   // Channels within a group don't count!
            }

            if (!strcmp((*itcs).getPV(), _pv) && (*itcs).getClientContext() == ccc)
            {
                //std::cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << std::endl;
                //std::cout << " INFORMATION to author: MATCHed Handle= " << (*itcs).handle << " to PV= " << _pv << std::endl;
                //std::cout << " by looping through tcs::iterator, while the pv::iterator was NOT found! " << std::endl;
                return (*itcs).handle;
            }
        }
    }

    return getHandleFromPVAlias(_pv, ccc);
#undef __METHOD__
}


/**
 *  \brief Retrieves handle for a given process variable alias
 *  \param _pv  input: process variable alias
 *  \param ccc input: ca_client_context
 *  \return handle else 0 if no handle exists
 */
unsigned int HandleHelper::getHandleFromPVAlias(const char * _pv, ca_client_context * ccc)
{
#define __METHOD__ "HandleHelper::getHandleFromPVAlias(const char * _pv, ca_client_context * ccc)"

    cafeConduit_set_by_pvAlias & pv_index = cs.get<by_pvAlias> ();
    cafeConduit_set_by_pvAlias::iterator it_pv;


    char pv[PVNAME_SIZE];
    removeLeadingAndTrailingSpaces(_pv, pv);

    it_pv = pv_index.find(pv);

    // Three possibilities of getting a match!
    if (it_pv != pv_index.end())
    {

        // Examine ca_client_context noting that channels within a group don't count!
        if (ccc == (*it_pv).getClientContext() &&  (*it_pv).getGroupHandle()==0 )
        {

            return (*it_pv).handle;
        }
        else
        {
            // Loop through all elements and search for pv/ca_client_context match
            for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
            {
                if((*itcs).getGroupHandle()>0)
                {
                    continue;   // Channels within a group don't count!
                }

                if (!strcmp((*itcs).getPVAlias(), _pv) && (*itcs).getClientContext()== ccc)
                {
                    //cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                    //cout << " INFORMATION to author: MATCHed Handle= " << (*itcs).handle << " to PVAlias= " << _pv << endl;
                    //cout << " by looping through tcs::iterator, even though pvAlias::iterator was found! " << endl;
                    return (*itcs).handle;
                }
            }
        }
    }
    else
    {
        // Loop through all elements and search for pv/ca_client_context match
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
        {
            if((*itcs).getGroupHandle()>0)
            {
                continue;   // Channels within a group don't count!
            }

            if (!strcmp((*itcs).getPVAlias(), _pv) && (*itcs).getClientContext() == ccc)
            {
                //cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                //cout << " INFORMATION to author: MATCHed Handle= " << (*itcs).handle << " to PVAlias= " << _pv << endl;
                //cout << " by looping through itcs::iterator, while the pvAlias::iterator was NOT found! " << endl;
                return (*itcs).handle;
            }
        }
    }

    return 0;

#undef __METHOD__
}




/**
 *  \brief Retrieves the process variable from a given handle
 *  \param  handle  input: handle
 *  \return pv else NULL if no handle exists
 */
const char * HandleHelper::getPVAlias(unsigned int handle)
{
#define __METHOD__ "HandleHelper::getPVAlias(unsigned int handle)"

    ca_client_context * ccc = ca_current_context();

    return getPVAlias(handle,ccc);

#undef __METHOD__
}


/**
 *  \brief Retrieves the process variable from a given handle
 *  \param  handle  input: handle
 *  \param ccc input: ca_client_context
 *  \return pv else NULL if no handle exists
 */
const char * HandleHelper::getPVAlias(unsigned int handle, ca_client_context * ccc)
{
#define __METHOD__ "HandleHelper::getPVAlkias(unsigned int handle, ca_client_context * ccc)"

    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);


    if (it_handle != handle_index.end())
    {
        return (*it_handle).getPVAlias();
    }
    else
    {
        // Loop through all elements and search for handle/ca_client_context match
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
        {
            if((*itcs).getGroupHandle()>0)
            {
                continue;   // Channels within a group don't count!
            }

            if ( (*itcs).getHandle()==handle && (*itcs).getClientContext() == ccc)
            {
                //cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                //cout << " INFORMATION to author: MATCHed Handle= " << handle << " to PV= " << (*itcs).getPV() << endl;
                //cout << " by looping through tcs::iterator, while the by_handle::iterator was NOT found! " << endl;
                return (*itcs).getPVAlias();
            }
        }
    }

    if (printErrorPolicy.getInvalidHandle())
    {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "PVAlias for Handle=" << handle << " not found " << endl;
    }

    return "";
#undef __METHOD__
}


/**
 *  \brief Retrieves the process variable from a given handle
 *  \param  handle  input: handle
 *  \return pv else NULL if no handle exists
 */
const char * HandleHelper::getPVFromHandle(unsigned int handle)
{
#define __METHOD__ "HandleHelper::getPVFromHandle(unsigned int handle)"

    ca_client_context * ccc = ca_current_context();

    return getPVFromHandle(handle,ccc);

#undef __METHOD__
}


/**
 *  \brief Retrieves the process variable from a given handle
 *  \param  handle  input: handle
 *  \param ccc input: ca_client_context
 *  \return pv else NULL if no handle exists
 */
const char * HandleHelper::getPVFromHandle(unsigned int handle, ca_client_context * ccc)
{
#define __METHOD__ "HandleHelper::getPVFromHandle(unsigned int handle, ca_client_context * ccc)"

    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);


    if (it_handle != handle_index.end())
    {
        return (*it_handle).getPV();
    }
    else
    {
        // Loop through all elements and search for handle/ca_client_context match
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
        {
            if((*itcs).getGroupHandle()>0)
            {
                continue;   // Channels within a group don't count!
            }

            if ( (*itcs).getHandle()==handle && (*itcs).getClientContext() == ccc)
            {
                //cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                //cout << " INFORMATION to author: MATCHed Handle= " << handle << " to PV= " << (*itcs).getPV() << endl;
                //cout << " by looping through tcs::iterator, while the by_handle::iterator was NOT found! " << endl;
                return (*itcs).getPV();
            }
        }
    }

    if (printErrorPolicy.getInvalidHandle())
    {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "PV for Handle=" << handle << " not found " << endl;
    }

    return "";
#undef __METHOD__
}



/**
 *  \brief Retrieves handle for a given process variable with a CAFE group
 *  \param _pv  input: process variable
 *  \param gh input: groupHandle
 *  \return handle else 0 if no handle exists
 */
unsigned int HandleHelper::getHandleFromPVWithinGroup(const char * _pv, unsigned int gh)
{
#define __METHOD__ "HandleHelper::getHandleFromPVWithinGroup(const char * _pv, unsigned int gh)"

    ca_client_context * ccc = ca_current_context();

    return getHandleFromPVWithinGroup(_pv,ccc, gh);

#undef __METHOD__
}




/**
 *  \brief Retrieves handle for a given process variable with a CAFE group within a give ca context
 *  \param _pv  input: process variable
 *  \param ccc input: ca_client_context
 *  \param gh input: groupHandle
 *  \return handle else 0 if no handle exists
 */
unsigned int HandleHelper::getHandleFromPVWithinGroup(const char * _pv,
        ca_client_context * ccc, unsigned int gh)
{
#define __METHOD__ \
    "HandleHelper::getHandleFromPVWithinGroup(const char * _pv, ca_client_context * ccc, unsigned int gh)"

    cafeConduit_set_by_pv & pv_index = cs.get<by_pv> ();
    cafeConduit_set_by_pv::iterator it_pv;

    char pv[PVNAME_SIZE];
    removeLeadingAndTrailingSpaces(_pv, pv);

    it_pv = pv_index.find(pv);

    // Three possibilities of getting a match!
    if (it_pv != pv_index.end())
    {

        // Examine ca_client_context noting that channels within a group don't count!
        if (ccc == (*it_pv).getClientContext() &&  (*it_pv).getGroupHandle()==gh )
        {

            return (*it_pv).handle;
        }
        else
        {
            // Loop through all elements and search for pv/ca_client_context match
            for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
            {
                if((*itcs).getGroupHandle()!=gh)
                {
                    continue;   // Channels within a group don't count!
                }

                if (!strcmp((*itcs).getPV(), _pv) && (*itcs).getClientContext()== ccc)
                {
                    //cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                    //cout << " INFORMATION to author: MATCHed Handle= " << (*itcs).handle << " to PV= " << _pv << endl;
                    //cout << " by looping through tcs::iterator, even though pv::iterator was found! " << endl;
                    return (*itcs).handle;
                }
                //Maybe called from within thread with null context
                else if (!strcmp((*itcs).getPV(), _pv) && 0 == ccc)
                {
                    return (*itcs).handle;
                }
            }
        }
    }
    else
    {
        // Loop through all elements and search for pv/ca_client_context match
        for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
        {
            if((*itcs).getGroupHandle()!=gh)
            {
                continue;   // Channels within a group don't count!
            }

            if (!strcmp((*itcs).getPV(), _pv) && (*itcs).getClientContext() == ccc)
            {
                //cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
                //cout << " INFORMATION to author: MATCHed Handle= " << (*itcs).handle << " to PV= " << _pv << endl;
                //cout << " by looping through tcs::iterator, while the pv::iterator was NOT found! " << endl;
                return (*itcs).handle;
            }
        }
    }

    return 0; //getHandleFromPVAliasWithinGroup(_pv, ccc);
#undef __METHOD__
}



/**
 *  \brief  Retrieves all handles belonging to a group referenced by its groupHandle
 *  \param  _groupHandle input: groupHandle
 *  \return vector of handles within Group
 */
vector<unsigned int>   HandleHelper::getHandlesFromWithinGroupV(unsigned int _groupHandle)
{
#define __METHOD__ "HandleHelper::getHandlesFromWithinGroupV"

    cafeGroup_set_by_groupHandle & groupHandle_index = gs.get<by_groupHandle> ();
    cafeGroup_set_by_groupHandle::iterator it_groupHandle;

    it_groupHandle = groupHandle_index.find(_groupHandle);

    vector<unsigned int> vhg;

    if (it_groupHandle != groupHandle_index.end())
    {
        vhg.reserve( (*it_groupHandle).getNMember());
        for (unsigned int i=0; i <(*it_groupHandle).getNMember(); ++i )
        {
            vhg.push_back((*it_groupHandle).mHandle[i]);
        }
        return vhg;
    }
    else
    {
        // Loop through all elements and search for grouphandle match
        for (itgs = gs.begin(); itgs != gs.end(); ++itgs)
        {

            if ((*itgs).getGroupHandle() == _groupHandle )
            {
                vhg.reserve( (*itgs).getNMember());
                for (unsigned int i=0; i <(*itgs).getNMember(); ++i )
                {
                    vhg.push_back((*itgs).mHandle[i]);
                }
                return vhg;
            }
        }
    }

    cafeStatus.report(ECAFE_UNKNOWN_GROUP);

    return vhg;
#undef __METHOD__
}


/**
 *  \brief  Retrieves all handles belonging to a group referenced by its groupHandle
 *  \param  _groupHandle input: groupHandle
 *  \return array of handles within Group
 */
unsigned int *   HandleHelper::getHandlesFromWithinGroup(unsigned int _groupHandle)
{
#define __METHOD__ "HandleHelper::getHandlesFromWithinGroup"

    cafeGroup_set_by_groupHandle & groupHandle_index = gs.get<by_groupHandle> ();
    cafeGroup_set_by_groupHandle::iterator it_groupHandle;

    it_groupHandle = groupHandle_index.find(_groupHandle);

    if (it_groupHandle != groupHandle_index.end())
    {
        return (*it_groupHandle).mHandle;
    }
    else
    {
        // Loop through all elements and search for grouphandle match
        for (itgs = gs.begin(); itgs != gs.end(); ++itgs)
        {
            //ConduitGroup cg = *itgs;

            if ((*itgs).getGroupHandle() == _groupHandle )
            {
                return (*itgs).mHandle;
                //    cout << " MATCH FOUND Handle Number is= " << (cg).groupHandle << endl;
            }
        }
    }

    cafeStatus.report(ECAFE_UNKNOWN_GROUP);

    // Find number of members
    // Declare vector
    // Loop round all members
    // Find pv
    // find handle
    // add to vector
    return (unsigned int *) NULL;
#undef __METHOD__
}



/**
 *  \brief  Retrieves all handles belonging to a group referenced by its groupHandle
 *  \param  _groupHandle input: groupHandle
 *  \return vector of handles within Group
 */
vector<unsigned int>   HandleHelper::getDisconnectedHandlesFromWithinGroupV(unsigned int _groupHandle)
{
#define __METHOD__ "HandleHelper::getDisconnectedHandlesFromWithinGroupV"

    cafeGroup_set_by_groupHandle & groupHandle_index = gs.get<by_groupHandle> ();
    cafeGroup_set_by_groupHandle::iterator it_groupHandle;

    it_groupHandle = groupHandle_index.find(_groupHandle);

    vector<unsigned int> vhg;

    if (it_groupHandle != groupHandle_index.end())
    {
        vhg.reserve( (*it_groupHandle).getNMember());

        cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
        cafeConduit_set_by_handle::iterator it_handle;

        for (unsigned int i=0; i <(*it_groupHandle).getNMember(); ++i )
        {

            it_handle = handle_index.find((*it_groupHandle).mHandle[i]);

            if (it_handle != handle_index.end())
            {
                if ( ! ((*it_handle).isConnected()) )
                {
                    vhg.push_back((*it_groupHandle).mHandle[i]);
                }
            }
        }
        return vhg;
    }
    else
    {
        // Loop through all elements and search for grouphandle match
        for (itgs = gs.begin(); itgs != gs.end(); ++itgs)
        {

            if ((*itgs).getGroupHandle() == _groupHandle )
            {
                vhg.reserve( (*itgs).getNMember());

                cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
                cafeConduit_set_by_handle::iterator it_handle;


                for (unsigned int i=0; i <(*itgs).getNMember(); ++i )
                {

                    it_handle = handle_index.find((*itgs).mHandle[i]);
                    if (it_handle != handle_index.end())
                    {
                        if ( !((*it_handle).isConnected() ) )
                        {
                            vhg.push_back((*itgs).mHandle[i]);
                        }
                    }
                }
                return vhg;
            }
        }
    }

    cafeStatus.report(ECAFE_UNKNOWN_GROUP);

    return vhg;
#undef __METHOD__
}


/**
 *  \brief  Method returns true if channel is connected, else false
 *  \param  handle input: handle
 *  \return bool
 */
bool HandleHelper::isChannelConnected(unsigned int handle)
{
#define __METHOD__ "HandleHelper::isChannelConnected"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(handle);
    if (it_handle != handle_index.end())
    {
        return (*it_handle).isConnected();
    }
    else
    {
        //if (printErrorPolicy.getInvalidHandle()) {
        std::cout << __METHOD__  << " Input handle " << handle << " does not exist! " << std::endl;
        //}
        return false;
    }
#undef __METHOD__
}


/**
 *  \brief  Method returns true if all given channels are connected, else false
 *  \parameter handleArray input: array of handles to test connection state of
 *  \parameter nHandles input: no of handles in array
 *  \return bool
 */
bool HandleHelper::areChannelsConnected(unsigned int * handleArray, const unsigned int nHandles)
{
#define __METHOD__ "HandleHelper::areChannelsConnected"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;

    for (int i=0; i < nHandles; ++i)
    {
        it_handle = handle_index.find(handleArray[i]);
        if (it_handle != handle_index.end())
        {
            if (!(*it_handle).isConnected())
            {
                return false;
            }
        }
        else
        {
            //if (printErrorPolicy.getInvalidHandle()) {
            std::cout << __METHOD__ << " Input handle " << handleArray[i] << " does not exist! " << std::endl;
            //}
            return false;
        }
    }

    return true;
#undef __METHOD__
}


/**
 *  \brief  Method returns true if all given channels are connected, else false
 *  \parameter handleV input: vector of handles to test connection state of
 *  \return bool
 */
bool HandleHelper::areChannelsConnectedV(std::vector<unsigned int> handleV)
{
#define __METHOD__ "HandleHelper::areChannelsConnectedV"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;

    for (int i=0; i < handleV.size(); ++i)
    {
        it_handle = handle_index.find(handleV[i]);
        if (it_handle != handle_index.end())
        {
            if (!(*it_handle).isConnected())
            {
                return false;
            }
        }
        else
        {
            //if (printErrorPolicy.getInvalidHandle()) {
            std::cout << __METHOD__ << " Input handle " << handleV[i] << " does not exist! " << std::endl;
            //}
            return false;
        }
    }
    return true;
#undef __METHOD__
}




/**
 *  \brief  Method returns true if all channels are connected, else false
 *  \return bool
 */
bool HandleHelper::allChannelsConnected()
{
#define __METHOD__ "HandleHelper::allChannelsConnected"
    ca_client_context * ccc = ca_current_context();
    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        if (!(*itcs).isConnected() && (ccc ==(*itcs).getClientContext()) )
        {
            return false;
        }
    }
    return true;
#undef __METHOD__
}




/**
 *  \brief  Method returns true if all channels are connected, else false
 *  \return bool
 */
bool HandleHelper::allChannelsWithinGroupConnected()
{
#define __METHOD__ "HandleHelper::allChannelsWithinGroupConnected"
    ca_client_context * ccc = ca_current_context();
    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        if (!(*itcs).isConnected() && (ccc ==(*itcs).getClientContext()) && (*itcs).getGroupHandle()>0 )
        {
            return false;
        }
    }

    return true;
#undef __METHOD__
}

/**
 *  \brief  Method returns true if all channels within the given vector of groups handles are connected, else false
 *  \param  grpID input: vector of group handles
 *  \return bool
 */
bool HandleHelper::allChannelsWithinGroupConnectedV(std::vector<unsigned int> grpID)
{
#define __METHOD__ "HandleHelper::allChannelsWithinGroupConnectedV"
    if (grpID.size()==0)
    {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cout << "Input vector listing group handles is of zero size " << endl;
    }

    for (size_t i=0; i<grpID.size(); ++i)
    {
        unsigned int nMem= HandleHelper::getDisconnectedHandlesFromWithinGroupV(grpID[i]).size();
        if ( nMem > 0)
        {
            //cout << "group handle " << grpID[i] << " NOT all members connected " << endl;
            return false;
        }
        //cout << "group handle " << grpID[i] << " has all members connected " << endl;
    }

    return true;

#undef __METHOD__
}




/**
 *  \brief  Retrieves last cached status information for given handle
 *  \param  _handle input: handle
 *  \return status
 */
int HandleHelper::getStatus(unsigned int _handle)
{
#define __METHOD__ "HandleHelper::getStatus(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        return (*it_handle).getStatus();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout <<  "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }
#undef __METHOD__
}


/**
 *  \brief  Retrieves last cached timestamp information for given handle
 *  \param  _handle input: handle
 *  \param  ts output: epicsTimeStamp
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int HandleHelper::getTimeStamp(unsigned int _handle, epicsTimeStamp &ts)
{
#define __METHOD__ "HandleHelper::getTimeStamp(unsigned int _handle, epicsTimeStamp &ts)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    ts.secPastEpoch=0;
    ts.nsec=0;
    if (it_handle != handle_index.end())
    {
        ts= (*it_handle).getTimeStamp();
        return ICAFE_NORMAL;
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }
#undef __METHOD__
}


/**
 *  \brief  Checks for nan values in HIHI HIGH LOW LOLO fields.
 *  \param  _handle input: handle
 *  \return true if one of HIHI|HIGH|LOW|LOLO is not nan, else false if all are nan
 */
bool HandleHelper::hasAlarmStatusSeverity(unsigned int _handle)
{
#define __METHOD__ "HandleHelper::hasAlarmStatusSeverity(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    //Only these have alarm/severity
    std::vector<std::string>alarmSeverityRecordTypes;
    alarmSeverityRecordTypes.reserve(10);
    alarmSeverityRecordTypes.push_back( std::string ( "ai"));
    alarmSeverityRecordTypes.push_back( std::string ( "ao"));
    alarmSeverityRecordTypes.push_back( std::string ( "calc"));
    alarmSeverityRecordTypes.push_back( std::string ( "dfanout"));
    alarmSeverityRecordTypes.push_back( std::string ( "longin"));
    alarmSeverityRecordTypes.push_back( std::string ( "longout"));
   
    alarmSeverityRecordTypes.push_back( std::string ( "pid"));
    alarmSeverityRecordTypes.push_back( std::string ( "sel"));
    alarmSeverityRecordTypes.push_back( std::string ( "steppermotor"));
    alarmSeverityRecordTypes.push_back( std::string ( "sub"));


    if (it_handle != handle_index.end())
    {
        //Never connected so do not know
        if  (!(*it_handle).isConnected() &&  (*it_handle).getStatus() == ICAFE_CS_NEVER_CONN )
        {
            return false;
        }

	PVDataHolder  _pvd;
        unsigned int nelem=getNelemRequest(_handle);

        if (nelem >1)
        {
            setNelemToRetrieveFromCache(_handle,1);
        }

        _pvd.setNelem(nelem);

        //return to previous
        if (nelem != 1)
        {
            setNelemToRetrieveFromCtrlCache(_handle,nelem);
        }

	bool returnFlag = false;
        int _status = (*it_handle).getPVDataHolder(_pvd) ;
	
        if (_status == ICAFE_NORMAL) 
	{
	  if (_pvd.getAlarmSeverity() == EPICS_GLOBAL_ALARM_SEVERITY::SEV_MAJOR || 
	      _pvd.getAlarmSeverity() == EPICS_GLOBAL_ALARM_SEVERITY::SEV_MINOR) 
	    {
	      returnFlag = true;
	    }
	}
	
        //return to previous
        if (nelem != 1)
        {
            setNelemToRetrieveFromCache(_handle,nelem);
        }

	if (returnFlag == true)
        {
	    return true;
        }


        std::string className= (std::string) (*it_handle).getClassName();
        std::vector<std::string>::const_iterator it = std::find(alarmSeverityRecordTypes.begin(), alarmSeverityRecordTypes.end(), className);

       
        // if never connected getStatus is 600 ICAFE_CS_NEVER_CONN
        // std::cout << (*it_handle).getPV() << " is Connected " << (*it_handle).isConnected()     << std::endl;
        // std::cout << "getStatus    " << (*it_handle).getStatus()       << std::endl;

        // Check if handle has already been supplemented
        if  ((*it_handle).hasAlarmSeverityStruct())
        {
            std::cout << __METHOD__ << " is true " << std::endl;
            return (*it_handle).hasAlarmSeverity();
        }

       
	if (it == alarmSeverityRecordTypes.end())
        {
            return false;
        }


        PVCtrlHolder  _pvc;

        nelem=getNelemRequestCtrl(_handle);

        if (nelem >1)
        {
            setNelemToRetrieveFromCtrlCache(_handle,1);
        }
        _pvc.setNelem(nelem);


        int _stat = (*it_handle).getPVCtrlHolder(_pvc) ;

        // Should already have been caught by first check above
        if  (!(*it_handle).isConnected() &&  _pvc.getStatus() == ICAFE_CS_NEVER_CONN )
        {
            return false;
        }

        // if Never connected getStatus is 600 ICAFE_CS_NEVER_CONN
        // std::cout << "status from getPVCtrlHolder // " << _stat << std::endl;
        // std::cout << "status from getPVCtrlHolder // " << _pvc.getStatus() << std::endl;
        //_pvc.print();

        //return to previous
        if (nelem!=1)
        {
            setNelemToRetrieveFromCtrlCache(_handle,nelem);
        }
 

	

        if (_stat==ICAFE_NORMAL)
        {
            //Special test for ENUM value - should never reach here as is mbbi/o record
            //if (_pvc.getNoEnumStrings()>0 )
            //{
            //    return false;
            //}
	    //enums can have alarms states!!!

            if      (!((boost::math::isnan)(_pvc.getLowerAlarmLimit_AsDouble())))
            {
	      if (_pvc.getLowerAlarmLimit_AsDouble() != 0) {
                 return true;
	      }
            }
            else if (!((boost::math::isnan)(_pvc.getLowerWarningLimit_AsDouble())))
            {
               if (_pvc.getLowerWarningLimit_AsDouble() != 0) {
                   return true;
	       } 
            }
            else if (!((boost::math::isnan)(_pvc.getUpperWarningLimit_AsDouble())))
            {
                if (_pvc.getUpperWarningLimit_AsDouble() != 0) {
                   return true;
	       } 
            }
            else if (!((boost::math::isnan)(_pvc.getUpperAlarmLimit_AsDouble())))
            {
                if (_pvc.getUpperAlarmLimit_AsDouble() != 0) {
                   return true;
	       } 
            }

	    
            return false;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return false;
    }
#undef __METHOD__
}


/**
 *  \brief  Retrieves last cached status information for given handle
 *  \param  _handle input: handle
 *  \param  as output: alarmStatus [0] and alarmSeverity [1] in array of dbr_short_t[2]
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int HandleHelper::getAlarmStatusSeverity(unsigned int _handle, dbr_short_t as[2])
{
#define __METHOD__ "HandleHelper::getAlarmStatusSeverity(unsigned int _handle, short & *as)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        as[0]= (*it_handle).getAlarmStatus();
        as[1]= (*it_handle).getAlarmSeverity();
        return ICAFE_NORMAL;
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }
#undef __METHOD__
}


/**
 *  \brief  Retrieves last cached status information for given handle
 *  \param  _handle input: handle
 *  \param  asas output: alarmStatus [0] and alarmSeverity [1] in array of dbr_short_t[2]
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int HandleHelper::getAlarmStatusSeverityAsString(unsigned int _handle, std::string asas[2])
{
#define __METHOD__ "HandleHelper::getAlarmStatusSeverityAsString(unsigned int _handle, string & *asas)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        asas[0]= (*it_handle).getAlarmStatusAsString();
        asas[1]= (*it_handle).getAlarmSeverityAsString();
        return ICAFE_NORMAL;
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }
#undef __METHOD__
}




/**
 *  \brief Rerieves vector of handles for given vector of PVs
 *  \param pvV input: vector of PVS
 *  \return vector of handles
 */
std::vector<unsigned int>  HandleHelper::getHandlesFromPVs(std::vector<std::string> pvV)
{

    ca_client_context * ccc = ca_current_context();

    return getHandlesFromPVs(pvV,ccc);

}


/**
 *  \brief Rerieves vector of handles for given vector of PVs
 *  \param pvV input: vector of PVS
 *  \param ccc input: ca_client_context *
 *  \return ICAFE_NORMAL if all OK else ECAFE_INVALID_HANDLE
 */
std::vector<unsigned int> HandleHelper::getHandlesFromPVs(std::vector<std::string> pvV,  ca_client_context * ccc)
{
#define __METHOD__ "HandleHelper::getHandlesFromPVs()"

    vector<unsigned int> handleV;

    handleV.reserve(pvV.size());


    cafeConduit_set_by_pv & pv_index = cs.get<by_pv> ();
    cafeConduit_set_by_pv::iterator it_pv;

    for (unsigned int i=0; i<pvV.size(); ++i)
    {


        char pv[PVNAME_SIZE];
        removeLeadingAndTrailingSpaces(pvV[i].c_str(), pv);

        it_pv = pv_index.find(pv);

        // Possibilities of getting a match!
        if (it_pv != pv_index.end())
        {

            // Examine ca_client_context noting that channels within a group don't count!
            if (ccc == (*it_pv).getClientContext() &&  (*it_pv).getGroupHandle()==0 )
            {
                handleV.push_back((*it_pv).handle);
            }
        }
        else
        {
            // Loop through all elements and search for pv/ca_client_context match
            for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
            {
                if((*itcs).getGroupHandle()>0)
                {
                    continue;   // Channels within a group don't count!
                }

                if (!strcmp((*itcs).getPV(), pvV[i].c_str()) && (*itcs).getClientContext()== ccc)
                {

                    handleV.push_back((*itcs).handle);
                    break;
                }
            }
            handleV.push_back(0);
        }
    } //for



    return handleV;
#undef __METHOD__

}


/**
 *  \brief Prints Conduit member values for all given handles
 *  \return ICAFE_NORMAL if all OK else ECAFE_INVALID_HANDLE
 */
int HandleHelper::printHandlesV(std::vector<unsigned int> handleV)
{
#define __METHOD__ "HandleHelper::printHandle(unsigned int)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;

    long lStatus=ICAFE_NORMAL;

    for (unsigned int i=0; i<handleV.size(); ++i)
    {

        it_handle = handle_index.find(handleV[i]);

        if (it_handle != handle_index.end())
        {
            cout << "--------------------------------------------------" << endl;
            cout << "HANDLE=            " << (*it_handle).handle << "   " ;
            if ((*it_handle).getGroupHandle()>0)
            {
                cout << "groupHandle= " << (*it_handle).getGroupHandle() << endl;
            }
            else
            {
                cout << endl;
            }

            cout << "PV Name:           " << (*it_handle).getPV() << endl;
            if ( strcmp((*it_handle).getPV(),(*it_handle).getPVAlias()) )
            {
                cout << "PV Alias=          " << (*it_handle).getPVAlias() << endl;
            }
            cout << "Data Type:         "
                 << cafeDataTypeCode.message((*it_handle).getChannelRegalia().dataType).c_str() << endl;


            cout << "Class Name:        " << (*it_handle).getChannelRegalia().className << endl;
            cout << "cxt/chid:          " << (*it_handle).getClientContext()
                 <<"/"<<(*it_handle).getChannelID()<< endl;
            // "/"<< (*it_handle).uniqueID << endl;
            // Returns normal completion
            // cout << "status=         " << cafeStatus.csi.message(cc.getStatus()) << endl;

            double p,g;
            PolicyHelper ph;
            ph.getTimeout((*it_handle).getHandle(),p,g);

            cout << "Timeouts put/get:  " << p << "/" << g << " sec. " << endl;


            if ((*it_handle).getGroupHandle()>0)
            {
                cafeGroup_set_by_groupHandle & groupHandle_index = gs.get<by_groupHandle> ();
                cafeGroup_set_by_groupHandle::iterator it_groupHandle;
                it_groupHandle = groupHandle_index.find((*it_handle).getGroupHandle());


                if (it_groupHandle != groupHandle_index.end())
                {
                    ph.getSGTimeout((*it_groupHandle).getGroupHandle(),p,g);
                    cout << "Syn. Grp put/get:  " << p << "/" << g << " sec. " << endl;
                }

            }


            cout << "isConnected?:      " ;
            if ((*it_handle).isConnected())
            {
                cout << "Yes" << endl;
            }
            else
            {
                cout << "No" << endl;
            }
            cout << "--------------------------------------------------" << endl;

            vector<unsigned int> mids=getMonitorIDs((*it_handle).getHandle());
            if (mids.size()>0)
            {
                cout << "Monitor ID:        " ;
                for (unsigned int i=0; i< mids.size(); ++i)
                {
                    cout << mids[i] << " " ;
                }
                cout << endl;
            }
            vector<unsigned int> midsiw=getMonitorIDsInWaiting((*it_handle).getHandle());
            if (midsiw.size()>0)
            {
                cout << "Monitors to start: "  << midsiw.size() << endl;
            }

        }
        else
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << handleV[i] << " either never existed or no longer exists " << endl;
            lStatus=ECAFE_INVALID_HANDLE;
        }
    }//for
    return lStatus;
#undef __METHOD__
}


/**
 *  \brief Prints Conduit member values for all given handles
 *  \return ICAFE_NORMAL if all OK else ECAFE_INVALID_HANDLE
 */
int HandleHelper::printHandles(unsigned int * handleArray, unsigned int nHandles)
{
#define __METHOD__ "HandleHelper::printHandle(unsigned int)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;

    long lStatus=ICAFE_NORMAL;

    for (unsigned int i=0; i<nHandles; ++i)
    {

        it_handle = handle_index.find(handleArray[i]);

        if (it_handle != handle_index.end())
        {
            cout << "--------------------------------------------------" << endl;
            cout << "HANDLE=            " << (*it_handle).handle << "   " ;
            if ((*it_handle).getGroupHandle()>0)
            {
                cout << "groupHandle= " << (*it_handle).getGroupHandle() << endl;
            }
            else
            {
                cout << endl;
            }

            cout << "PV Name:           " << (*it_handle).getPV() << endl;
            if ( strcmp((*it_handle).getPV(),(*it_handle).getPVAlias()) )
            {
                cout << "PV Alias=          " << (*it_handle).getPVAlias() << endl;
            }
            cout << "Data Type:         "
                 << cafeDataTypeCode.message((*it_handle).getChannelRegalia().dataType).c_str() << endl;


            cout << "Class Name:        " << (*it_handle).getChannelRegalia().className << endl;
            cout << "cxt/chid:          " << (*it_handle).getClientContext()
                 <<"/"<<(*it_handle).getChannelID()<< endl;
            // "/"<< (*it_handle).uniqueID << endl;
            // Returns normal completion
            // cout << "status=         " << cafeStatus.csi.message(cc.getStatus()) << endl;

            double p,g;
            PolicyHelper ph;
            ph.getTimeout((*it_handle).getHandle(),p,g);

            cout << "Timeouts put/get:  " << p << "/" << g << " sec. " << endl;



            if ((*it_handle).getGroupHandle()>0)
            {
                cafeGroup_set_by_groupHandle & groupHandle_index = gs.get<by_groupHandle> ();
                cafeGroup_set_by_groupHandle::iterator it_groupHandle;
                it_groupHandle = groupHandle_index.find((*it_handle).getGroupHandle());


                if (it_groupHandle != groupHandle_index.end())
                {
                    ph.getSGTimeout((*it_groupHandle).getGroupHandle(),p,g);
                    cout << "Syn. Grp put/get:  " << p << "/" << g << " sec. " << endl;
                }

            }




            //Find Group
            cout << "isConnected?:      " ;
            if ((*it_handle).isConnected())
            {
                cout << "Yes" << endl;
            }
            else
            {
                cout << "No" << endl;
            }

            vector<unsigned int> mids=getMonitorIDs((*it_handle).getHandle());
            if (mids.size()>0)
            {
                cout << "Monitor ID:        " ;
                for (unsigned int i=0; i< mids.size(); ++i)
                {
                    cout << mids[i] << " " ;
                }
                cout << endl;
            }
            vector<unsigned int> midsiw=getMonitorIDsInWaiting((*it_handle).getHandle());
            if (midsiw.size()>0)
            {
                cout << "Monitors to start: "  << midsiw.size() << endl;
            }

            cout << "--------------------------------------------------" << endl;

        }
        else
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << handleArray[i] << " either never existed or no longer exists " << endl;
            lStatus=ECAFE_INVALID_HANDLE;
        }
    }//for
    return lStatus;
#undef __METHOD__
}


/**
 *  \brief Prints Conduit member values for all given handle
 *  \return ICAFE_NORMAL if all OK else ECAFE_INVALID_HANDLE
 */
int HandleHelper::printHandle(unsigned int _handle)
{
#define __METHOD__ "HandleHelper::printHandle(unsigned int)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        cout << "--------------------------------------------------" << endl;
        cout << "HANDLE=            " << (*it_handle).handle << "   " ;
        if ((*it_handle).getGroupHandle()>0)
        {
            cout << "groupHandle= " << (*it_handle).getGroupHandle() << endl;
        }
        else
        {
            cout << endl;
        }

        cout << "PV Name:           " << (*it_handle).getPV() << endl;
        if ( strcmp((*it_handle).getPV(),(*it_handle).getPVAlias()) )
        {
            cout << "PV Alias=          " << (*it_handle).getPVAlias() << endl;
        }
        cout << "Data Type:         "
             << cafeDataTypeCode.message((*it_handle).getChannelRegalia().dataType).c_str() << endl;


        cout << "Class Name:        " << (*it_handle).getChannelRegalia().className << endl;
        cout << "cxt/chid:          " << (*it_handle).getClientContext()
             <<"/"<<(*it_handle).getChannelID()<< endl;
        // "/"<< (*it_handle).uniqueID << endl;
        // Returns normal completion
        // cout << "status=         " << cafeStatus.csi.message(cc.getStatus()) << endl;

        double p,g;
        PolicyHelper ph;
        ph.getTimeout((*it_handle).getHandle(),p,g);

        cout << "Timeouts put/get:  " << p << "/" << g << " sec. " << endl;

        if ((*it_handle).getGroupHandle()>0)
        {
            cafeGroup_set_by_groupHandle & groupHandle_index = gs.get<by_groupHandle> ();
            cafeGroup_set_by_groupHandle::iterator it_groupHandle;
            it_groupHandle = groupHandle_index.find((*it_handle).getGroupHandle());


            if (it_groupHandle != groupHandle_index.end())
            {
                ph.getSGTimeout((*it_groupHandle).getGroupHandle(),p,g);
                cout << "Syn. Grp put/get:  " << p << "/" << g << " sec. " << endl;
            }

        }


        cout << "isConnected?:      " ;
        if ((*it_handle).isConnected())
        {
            cout << "Yes" << endl;
        }
        else
        {
            cout << "No" << endl;
        }

        vector<unsigned int> mids=getMonitorIDs((*it_handle).getHandle());
        if (mids.size()>0)
        {
            cout << "Monitor ID:        " ;
            for (unsigned int i=0; i< mids.size(); ++i)
            {
                cout << mids[i] << " " ;
            }
            cout << endl;
        }
        vector<unsigned int> midsiw=getMonitorIDsInWaiting((*it_handle).getHandle());
        if (midsiw.size()>0)
        {
            cout << "Monitors to start: "  << midsiw.size() << endl;
        }

        cout << "--------------------------------------------------" << endl;
        return ICAFE_NORMAL;
    }
    else
    {
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        return ECAFE_INVALID_HANDLE;
    }

#undef __METHOD__
}



/**
 *  \brief Prints Conduit member values for all Conduit objects (handles)
 *  \return Total number of handles;
 */
unsigned int HandleHelper::printHandles()
{
#define __METHOD__ "HandleHelper::printHandles"

    unsigned int noHandles = 0;

    // Loop through all elements and search for pv/ca_client_context match
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {

        if (noHandles==0)
        {
            cout << endl << "--------------------------------------------------" << endl;
            cout << "              PRINTING ALL HANDLES                " << endl;
            cout << "--------------------------------------------------" << endl;
        }
        cout << "--------------------------------------------------" << endl;

        cout << "HANDLE=            " << (*itcs).handle << "   " ;


        if ((*itcs).getGroupHandle()>0)
        {
            cout << "groupHandle= " << (*itcs).getGroupHandle() << endl;
        }
        else
        {
            cout << endl;
        }

        cout << "PV Name=           " << (*itcs).getPV() << endl;
        if ( strcmp((*itcs).getPV(),(*itcs).getPVAlias()) )
        {
            cout << "PV Alias=          " << (*itcs).getPVAlias() << endl;
        }
        cout << "Data Type =        "
             << cafeDataTypeCode.message((*itcs).getChannelRegalia().dataType).c_str() << endl;

        cout << "Class Name=        " << (*itcs).getChannelRegalia().className << endl;
        cout << "cxt/chid: " << (*itcs).getClientContext()
             <<"/"<<(*itcs).getChannelID()<< endl;
        //"/"<< (*itcs).uniqueID << endl;
        // Returns normal completion
        // cout << "status=         " << cafeStatus.csi.message(cc.getStatus()) << endl;


        double p,g;
        PolicyHelper ph;
        ph.getTimeout((*itcs).getHandle(),p,g);

        cout << "Timeouts put/get=  " << p << "/" << g << " sec. " << endl;


        if ((*itcs).getGroupHandle()>0)
        {
            cafeGroup_set_by_groupHandle & groupHandle_index = gs.get<by_groupHandle> ();
            cafeGroup_set_by_groupHandle::iterator it_groupHandle;
            it_groupHandle = groupHandle_index.find((*itcs).getGroupHandle());


            if (it_groupHandle != groupHandle_index.end())
            {
                ph.getSGTimeout((*it_groupHandle).getGroupHandle(),p,g);
                cout << "Syc. Grp timeouts put/get:  " << p << "/" << g << " sec. " << endl;
            }

        }



        cout << "isConnected?:      " ;
        if ((*itcs).isConnected())
        {
            cout << "Yes" << endl;
        }
        else
        {
            cout << "No" << endl;
        }

        vector<unsigned int> mids=getMonitorIDs((*itcs).getHandle());
        if (mids.size()>0)
        {
            cout << "Monitor ID:        " ;
            for (unsigned int i=0; i< mids.size(); ++i)
            {
                cout << mids[i] << " ";
            }
            cout << endl;
        }
        vector<unsigned int> midsiw=getMonitorIDsInWaiting((*itcs).getHandle());
        if (midsiw.size()>0)
        {
            cout << "Monitors to start: "  << midsiw.size() << endl;
        }

        cout << "--------------------------------------------------" << endl;
        ++noHandles;
    }

    if (noHandles==0)
    {
        cout << "--------------------------------------------------" << endl;
        cout << "          THERE ARE NO HANDLES TO PRINT           " << endl;
        cout << "--------------------------------------------------" << endl;
    }
    else
    {
        cout << "--------------------------------------------------" << endl;
        cout << "          END PRINTING ALL HANDLES                " << endl;
        cout << "--------------------------------------------------" << endl << endl;

    }
    std::cout.flush();
    return noHandles;
#undef __METHOD__
}



/**
 *  \brief Return a vector of handles that have monitors
 *  If a handle has n monitors it appears n time in the return vector.
 *  Monitors in waiting are als included in the count.
 *  \return Total number of handles with monitors (whether started or in waiting);
 */
vector<unsigned int> HandleHelper::getHandlesWithMonitors()
{
#define __METHOD__ "HandleHelper::getHandlesWithMonitors"


    vector<unsigned int> handleV;
    handleV.clear();
    handleV.reserve(cs.size());

    // Loop through all elements and search for pv/ca_client_context match
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {

        vector<unsigned int> mids=getMonitorIDs((*itcs).getHandle());
        vector<unsigned int> midsiw=getMonitorIDsInWaiting((*itcs).getHandle());
        if (mids.size()==0 && midsiw.size()==0)
        {
            continue;
        }

        for (unsigned int i=0; i<mids.size(); ++i)
        {
            handleV.push_back((*itcs).handle);
        };
        for (unsigned int i=0; i<midsiw.size(); ++i)
        {
            handleV.push_back((*itcs).handle);
        };
    }

    return handleV;
#undef __METHOD__
}



/**
 *  \brief Prints Conduit member values for all Conduit objects (handles) that have a monitor
 *  \return Total number of handles;
 */
unsigned int HandleHelper::printMonitors()
{
#define __METHOD__ "HandleHelper::printMonitors"

    unsigned int noHandles = 0;

    // Loop through all elements and search for pv/ca_client_context match
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {

        vector<unsigned int> mids=getMonitorIDs((*itcs).getHandle());
        vector<unsigned int> midsiw=getMonitorIDsInWaiting((*itcs).getHandle());
        if (mids.size()==0 && midsiw.size()==0)
        {
            continue;
        }

        if (noHandles==0)
        {
            cout << endl << "--------------------------------------------------" << endl;
            cout << "        PRINTING ALL HANDLES WITH MONITORS                " << endl;
            cout << "--------------------------------------------------" << endl;
        }
        cout << "--------------------------------------------------" << endl;
        cout << "HANDLE=            " << (*itcs).handle << "   " ;

        if ((*itcs).getGroupHandle()>0)
        {
            cout << "groupHandle= " << (*itcs).getGroupHandle() << endl;
        }
        else
        {
            cout << endl;
        }

        cout << "PV Name=           " << (*itcs).getPV() << endl;
        if ( strcmp((*itcs).getPV(),(*itcs).getPVAlias()) )
        {
            cout << "PV Alias=          " << (*itcs).getPVAlias() << endl;
        }
        cout << "Data Type =        "
             << cafeDataTypeCode.message((*itcs).getChannelRegalia().dataType).c_str() << endl;

        cout << "Class Name=        " << (*itcs).getChannelRegalia().className << endl;
        cout << "cxt/chid: " << (*itcs).getClientContext()
             <<"/"<<(*itcs).getChannelID()<< endl;
        //"/"<< (*itcs).uniqueID << endl;
        // Returns normal completion
        // cout << "status=         " << cafeStatus.csi.message(cc.getStatus()) << endl;

        double p,g;
        PolicyHelper ph;
        ph.getTimeout((*itcs).getHandle(),p,g);

        cout << "Timeouts put/get=  " << p << "/" << g << " sec. " << endl;


        cout << "isConnected?:      " ;
        if ((*itcs).isConnected())
        {
            cout << "Yes" << endl;
        }
        else
        {
            cout << "No" << endl;
        }



        cout << "Monitor ID:        " ;
        for (unsigned int i=0; i< mids.size(); ++i)
        {
            cout << mids[i] << " ";
        }
        cout << endl;
        if (midsiw.size()>0)
        {
            cout << "Monitors to start: "  << midsiw.size() << endl;
        }

        cout << "--------------------------------------------------" << endl;
        ++noHandles;
    }

    if (noHandles==0)
    {
        cout << "--------------------------------------------------" << endl;
        cout << "          THERE ARE NO HANDLES TO PRINT           " << endl;
        cout << "--------------------------------------------------" << endl;
    }
    else
    {
        cout << "--------------------------------------------------" << endl;
        cout << "          END PRINTING ALL HANDLES                " << endl;
        cout << "--------------------------------------------------" << endl << endl;

    }
    return noHandles;
#undef __METHOD__
}



/**
 *  \brief Retrieves a list of handles and PVs, whether connected or disconnected
 *  \param dhV output: Vector<unsigned int> of all handles
 *  \param pvV output: Vector<string> of corresponding PVs
 *  \return No of disconnected handles
 */
unsigned int HandleHelper::getHandles(std::vector<unsigned int> &dhV, std::vector<std::string> &pvV)
{
#define __METHOD__ "HandleHelper::getHandles(vector<unsigned int> &dhV, vector<string> &pvV)"

    dhV.clear();
    pvV.clear();
    dhV.reserve(cs.size());
    pvV.reserve(cs.size());
    // Loop through all elements and search for pv/ca_client_context match
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        dhV.push_back((*itcs).handle);
        pvV.push_back((*itcs).getPV());
    }
    return dhV.size();
#undef __METHOD__
}


/**
 *  \brief Retrieves a list of handles and PVs, whether connected or disconnected
 *  \param dhV output: Vector<unsigned int> of all handles
 *  \param pvV output: Vector<string> of corresponding PVs
 *  \param connV output: Vector<unsigned short> of connection states
 *  \return No of disconnected handles
 */
unsigned int HandleHelper::getHandleStates(std::vector<unsigned int> &dhV, std::vector<std::string> &pvV,
        std::vector<unsigned short> &connV)
{
#define __METHOD__ "HandleHelper::getHandleStates(vector<unsigned int> &dhV, vector<string> &pvV, vector<unsigned short> &connV)"

    dhV.clear();
    pvV.clear();
    connV.clear();
    dhV.reserve(cs.size());
    pvV.reserve(cs.size());
    connV.reserve(cs.size());
    // Loop through all elements and search for pv/ca_client_context match
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        dhV.push_back((*itcs).handle);
        pvV.push_back((*itcs).getPV());
        connV.push_back( (unsigned short) (*itcs).isConnected());
    }
    return dhV.size();
#undef __METHOD__
}


/**
 *  \brief Retrieves a list of connected PVs and their handles
 *  \param dhV output: Vector<unsigned int> of disconnected handles
 *  \param pvV output: Vector<string> of corresponding disconnected PVs
 *  \return No of disconnected handles
 */
unsigned int HandleHelper::getConnectedHandles(std::vector<unsigned int> &dhV, std::vector<std::string> &pvV)
{
#define __METHOD__ "HandleHelper::getConnectedHandles()"

    dhV.clear();
    pvV.clear();
    dhV.reserve(cs.size());
    pvV.reserve(cs.size());
    // Loop through all elements and search for pv/ca_client_context match
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        if (!(*itcs).isConnected())
        {
            continue;
        };
        dhV.push_back((*itcs).handle);
        pvV.push_back((*itcs).getPV());
    }
    return dhV.size();
#undef __METHOD__
}


/**
 *  \brief Retrieves a list of disconnected PVs and their handles
 *  \param dhV output: Vector<unsigned int> of disconnected handles
 *  \param pvV output: Vector<string> of corresponding disconnected PVs
 *  \return No of disconnected handles
 */
unsigned int HandleHelper::getDisconnectedHandles(std::vector<unsigned int> &dhV, std::vector<std::string> &pvV)
{
#define __METHOD__ "HandleHelper::getDisconnectedHandles()"

    dhV.clear();
    pvV.clear();
    dhV.reserve(cs.size());
    pvV.reserve(cs.size());
    // Loop through all elements and search for pv/ca_client_context match
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        if ((*itcs).isConnected())
        {
            continue;
        };
        dhV.push_back((*itcs).handle);
        pvV.push_back((*itcs).getPV());
    }
    return dhV.size();
#undef __METHOD__
}


/**
 *  \brief Prints Conduit member values for all disconnected CAFEConduit objects (handles)
 *  \return Total number of disconnected handles;
 */
unsigned int HandleHelper::printDisconnectedHandles()
{
#define __METHOD__ "HandleHelper::printDisconnectedHandles"

    unsigned int noDisconnectedHandles = 0;

    // Loop through all elements and search for pv/ca_client_context match
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {

        if ((*itcs).isConnected())
        {
            continue;
        };

        if (noDisconnectedHandles==0)
        {
            cout << endl << "--------------------------------------------------" << endl;
            cout << "              DISCONNECTED HANDLES                " << endl;
            cout << "--------------------------------------------------" << endl;
        }
        cout << "--------------------------------------------------" << endl;
        cout << "HANDLE=            " << (*itcs).handle << "   ";
        if ((*itcs).getGroupHandle()>0)
        {
            cout << "groupHandle= " << (*itcs).getGroupHandle() << endl;
        }
        else
        {
            cout << endl;
        }
        cout << "PV Name=           " << (*itcs).getPV() << endl;
        if ( strcmp((*itcs).getPV(),(*itcs).getPVAlias()) )
        {
            cout << "PV Alias=          " << (*itcs).getPVAlias() << endl;
        }
        cout << "cxt/chid: " << (*itcs).getClientContext()
             <<"/"<<(*itcs).getChannelID()<< endl;
        //"/"<< (*itcs).uniqueID<< endl;
        // Returns normal completion
        // cout << "status=         " << cafeStatus.csi.message((*itcs).getStatus()) << endl;
        cout << "isConnected?:      " ;
        if ((*itcs).isConnected())
        {
            cout << "Yes" << endl;
        }
        else
        {
            cout << "No" << endl;
        }

        vector<unsigned int> mids=getMonitorIDs((*itcs).getHandle());
        if (mids.size()>0)
        {
            cout << "Monitor ID:       " ;
            for (unsigned int i=0; i< mids.size(); ++i)
            {
                cout << mids[i] << " " << endl;
            }
        }


        cout << "--------------------------------------------------" << endl;

        ++noDisconnectedHandles;

    }

    if (noDisconnectedHandles==0)
    {
        cout << "--------------------------------------------------" << endl;
        cout << "   GOOD NEWS: THERE ARE NO DISCONNECTED HANDLES   " << endl;
        cout << "--------------------------------------------------" << endl;
    }
    else
    {
        cout << "--------------------------------------------------" << endl;
        cout << "          END DISCONNECTED HANDLES                " << endl;
        cout << "--------------------------------------------------" << endl << endl;

    }
    return noDisconnectedHandles;
#undef __METHOD__
}

//////////////////////////////////////////////////////////////////////////////////////////////////

/**
 *  \brief Sets the alarm, severity ts in conduit (used by get method WITH_CALLBACK)
 *  \param _handle input: Conduit object reference
 *  \param a input: alarm
 *  \param s input: severity
 *  \param ets input: epicsTimeStamp
 *  \return ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 */
int HandleHelper::setSTS(unsigned int _handle, dbr_short_t a, dbr_short_t s, epicsTimeStamp ets)
{
#define __METHOD__ "HandleHelper::setTS(unsigned int _handle, dbr_short_t a, dbr_short_t s, epicsTimeStamp ets)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    int status=ICAFE_NORMAL;

    if (it_handle != handle_index.end())
    {
        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_alarmStatus(a));
        handle_index.modify(it_handle, change_alarmSeverity(s));
        handle_index.modify(it_handle, change_epicsTimeStamp(ets));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }
    }
    else
    {

        status=ECAFE_INVALID_HANDLE;
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(status);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return status;
#undef __METHOD__
}



/**
 *  \brief Sets the cafeDbrType (e.g. DBR_TIME) and associated dbrDataTpe (e.g. DBR_TIME_zzz)
 *  \param _handleV input: vector<unsigned int> Conduit object references
 *  \param cdt input: CAFENUM::DBR_TYPE
 *  \return ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 */
int HandleHelper::setCafeDbrTypeV(std::vector<unsigned int> _handleV, CAFENUM::DBR_TYPE cdt )
{
#define __METHOD__ "HandleHelper::setCafeDbrType(vector<unsigned int> _handle, CAFENUM::DBR_TYPE cdt)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;


    int status=ICAFE_NORMAL;

    for (unsigned int i=0; i<_handleV.size(); ++i )
    {

        it_handle = handle_index.find(_handleV[i]);

        if (it_handle != handle_index.end())
        {

            channelRequestMetaDataClient=(*it_handle).getChannelRequestMetaDataClient();
            channelRequestMetaDataClient.setDbrTypesFromCafeDbrType(cdt);
            channelRequestMetaData=(*it_handle).getChannelRequestMetaData();
            channelRequestMetaData.setDbrTypesFromCafeDbrType(cdt);

            if(MUTEX)
            {
                cafeMutex.lock();
            }
            handle_index.modify(it_handle, change_channelRequestMetaDataClient(
                                    channelRequestMetaDataClient));
            handle_index.modify(it_handle, change_channelRequestMetaData(
                                    channelRequestMetaData));
            if(MUTEX)
            {
                cafeMutex.unlock();
            }

        }
        else
        {
            status=ECAFE_INVALID_HANDLE;
            if (printErrorPolicy.getInvalidHandle())
            {
                cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
                cafeStatus.report(status);
                cout << "Handle=" << _handleV[i] << " either never existed or no longer exists " << endl;
            }
        }

    }
    return status;
#undef __METHOD__
}




/**
 *  \brief Sets the cafeDbrType (e.g. DBR_TIME) and associated dbrDataTpe (e.g. DBR_TIME_zzz)
 *  \param _handle input: Conduit object reference
 *  \param cdt input: CAFENUM::DBR_TYPE
 *  \return ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 */
int HandleHelper::setCafeDbrType(unsigned int _handle, CAFENUM::DBR_TYPE cdt )
{
#define __METHOD__ "HandleHelper::setCafeDbrType(unsigned int _handle, CAFENUM::DBR_TYPE cdt)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    int status=ICAFE_NORMAL;

    if (it_handle != handle_index.end())
    {

        channelRequestMetaDataClient=(*it_handle).getChannelRequestMetaDataClient();
        channelRequestMetaDataClient.setDbrTypesFromCafeDbrType(cdt);
        channelRequestMetaData=(*it_handle).getChannelRequestMetaData();
        channelRequestMetaData.setDbrTypesFromCafeDbrType(cdt);

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelRequestMetaDataClient(
                                channelRequestMetaDataClient));
        handle_index.modify(it_handle, change_channelRequestMetaData(
                                channelRequestMetaData));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }

    }
    else
    {
        status=ECAFE_INVALID_HANDLE;
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(status);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return status;
#undef __METHOD__
}






/**
 *  \brief Gets the cafeDbrType (e.g. DBR_TIME)
 *  \param _handle input: Conduit object reference
 *  \param cdt output: CAFENUM::DBR_TYPE
 *  \return ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 */
int HandleHelper::getCafeDbrType(unsigned int _handle, CAFENUM::DBR_TYPE &cdt )
{
#define __METHOD__ "HandleHelper::getCafeDbrType(unsigned int _handle, CAFENUM::DBR_TYPE &cdt)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    int status=ICAFE_NORMAL;

    if (it_handle != handle_index.end())
    {

        channelRequestMetaData=(*it_handle).getChannelRequestMetaData();
        cdt=channelRequestMetaData.getCafeDbrType();
    }
    else
    {
        status=ECAFE_INVALID_HANDLE;
        cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
        cafeStatus.report(status);
        cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
    }
    return status;
#undef __METHOD__
}



/**
 *  \brief Gets the dataType requested (e.g. DBR_STRING etc.)
 *  \param  _handle input: Conduit object reference
 *  \param  rdt output: DBR_XXX
 *  \return ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 */
int HandleHelper::getDataTypeRequest(unsigned int _handle, chtype &rdt )
{
#define __METHOD__ "HandleHelper::getDataTypeRequest(unsigned int _handle, chtype &rdt)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    int status=ICAFE_NORMAL;

    if (it_handle != handle_index.end())
    {

        channelRequestMetaData=(*it_handle).getChannelRequestMetaData();
        rdt=channelRequestMetaData.getDataType();
    }
    else
    {
        status=ECAFE_INVALID_HANDLE;
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(status);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return status;
#undef __METHOD__
}



/**
 *
 *  \brief HandleHelper::addMonitorAction \n
 *
 *  \param  _handle input: handle identifying Conduit object
 *  \param   monitorAction input: string giviing the monitor action
 *  \return ICAFE_NORMAL if OK else ECAFE_INVALID_HANDLE
 */
int HandleHelper::addMonitorAction(unsigned int _handle, std::string monitorAction)
{
#define __METHOD__ "HandleHelper::addMonitorAction(unsigned int _handle, string monitorAction"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        handle_index.modify(it_handle, change_monitorAction(monitorAction));
    }
    else
    {
        cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;

#undef __METHOD__
}



/**
 *
 *  \brief HandleHelper::clearMonitorAction for all handles \n
 *
 *  \return ICAFE_NORMAL if OK else ECAFE_INVALID_HANDLE
 */
int HandleHelper::clearMonitorAction()
{
#define __METHOD__ "HandleHelper::clearMonitorAction()"

    int gStatus=ICAFE_NORMAL;
    int localStatus;

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {

        localStatus=clearMonitorAction( (*itcs).getHandle());
        if (localStatus !=ICAFE_NORMAL)
        {
            gStatus=localStatus;
        }
    }
    return gStatus;

#undef __METHOD__
}


/**
 *
 *  \brief HandleHelper::clearMonitorAction \n
 *
 *  \param  _handle input: handle identifying Conduit object
 *  \return ICAFE_NORMAL if OK else ECAFE_INVALID_HANDLE
 */
int HandleHelper::clearMonitorAction(unsigned int _handle)
{
#define __METHOD__ "HandleHelper::clearMonitorAction(unsigned int _handle)"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        handle_index.modify(it_handle, change_monitorActionClear());
    }
    else
    {
        cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;

#undef __METHOD__
}


/**
 *
 *  \brief HandleHelper::eraseMonitorAction \n
 *
 *  \param  _handle input: handle identifying Conduit object
 *  \return ICAFE_NORMAL if OK else ECAFE_INVALID_HANDLE
 */
int HandleHelper::eraseMonitorAction(unsigned int _handle)
{
#define __METHOD__ "HandleHelper::eraseMonitorAction(unsigned int _handle)"
    vector<string> msV;
    msV.clear();
    msV.reserve(2);

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        msV=(*it_handle).getMonitorAction();
        for (unsigned int i=0; i<msV.size(); ++i)
        {
            handle_index.modify(it_handle, change_monitorActionErase(msV[i]));
        }
    }
    else
    {
        cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;

#undef __METHOD__
}




/**
 *
 *  \brief HandleHelper::getMonitorAction
 *  \param  _handle input: handle identifying Conduit object
 *  \param  msV output: vector<string> giving the monitor actions
 *  \return ICAFE_NORMAL if OK else ECAFE_INVALID_HANDLE
 */
int HandleHelper::getMonitorAction(unsigned int _handle, std::vector<std::string> & msV)
{
#define __METHOD__ "HandleHelper::getMonitorAction(unsigned int _handle, vector<string> msV"

    msV.clear();
    msV.reserve(2);

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        msV=(*it_handle).getMonitorAction();
        return ICAFE_NORMAL;
    }
    else
    {

        cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        return ECAFE_INVALID_HANDLE;
    }


#undef __METHOD__
}


/**
 *
 *  \brief HandleHelper::getMonitorAction retrieves list of actions to perform on change of value \n
 *  If input boolean is true; then only oustantanding actions are listed
 *  \param  onlyIfNewData input: boolean to
 *  \return string of actions
 */
vector<string> HandleHelper::getMonitorAction(bool onlyIfNewData)
{
#define __METHOD__ "HandleHelper::getMonitorAction()"

    vector<string> msActionV;
    msActionV.clear();
    unsigned int nReserve=min( (unsigned int) cs.size(), (unsigned int) 50);
    msActionV.reserve(nReserve); //conservative
    std::vector<string>::iterator pos;
    vector<string> msLocal;
    // Loop through all elements and search for handles
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        if ( ((*itcs).getHasNewData() && onlyIfNewData) || !onlyIfNewData)
        {
            msLocal.clear();
            msLocal.reserve(2);
            msLocal=(*itcs).getMonitorAction();
            //Add to global vector
            for (size_t i=0;  i< msLocal.size(); ++i)
            {
                pos = find(msActionV.begin(), msActionV.end(), msLocal.begin()[i]);
                //No duplicates!
                if(pos==msActionV.end())
                {
                    msActionV.push_back(msLocal.begin()[i]);
                }
            }
            cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();


            if(MUTEX)
            {
                cafeMutex.lock();
            }
            handle_index.modify(itcs, change_hasNewData(false));
            if(MUTEX)
            {
                cafeMutex.unlock();
            }
        }
    }

    return msActionV;

#undef __METHOD__
}



/**
 *
 *  \brief HandleHelper::getMonitorHandlesAndActions retrieves list of handles and
 *  their actions to perform on change of value \n
 *  \param handleV output: Vector of handles
 *  \param actionV output: Vector of monitor actios
 *  \return ICAFE_NORMAL
 */
int HandleHelper::getMonitorHandlesAndActions(std::vector<unsigned int> & handleV, std::vector<std::string> & actionV)
{
#define __METHOD__ "HandleHelper::getMonitorHandlesAndActions(&handleV, &actionV)"

    actionV.clear();
    handleV.clear();
    unsigned int nReserve=min( (unsigned int) cs.size(), (unsigned int) 50);
    actionV.reserve(nReserve); //conservative
    handleV.reserve(nReserve); //conservative
    // Loop through all elements and search for handles
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        //if Monitor check action

        vector<unsigned int> mids=getMonitorIDs((*itcs).getHandle());
        vector<unsigned int> midsiw=getMonitorIDsInWaiting((*itcs).getHandle());
        if (mids.size()==0 && midsiw.size()==0)
        {
            continue;
        }

        vector<string> msV;
        HandleHelper::getMonitorAction((*itcs).getHandle(), msV);

        for (unsigned int i=0; i<msV.size(); ++i)
        {
            handleV.push_back((*itcs).handle);
            actionV.push_back(msV[i]);
        };

        if ( (mids.size()+midsiw.size())>msV.size())
        {
            handleV.push_back((*itcs).handle);
            actionV.push_back("");
        }


    }

    return ICAFE_NORMAL;

#undef __METHOD__
}


int HandleHelper::updateMonitorPolicyDeltaMS(unsigned int _handle, unsigned int _monid, 
					     unsigned short _deltaMS)
{
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    int status=ICAFE_NORMAL;

    if (it_handle != handle_index.end())
    {
        MonitorPolicy mp = (*it_handle).getMonitorPolicy(_monid);
	mp.setNotifyDeltaMilliSeconds(_deltaMS);
        if(MUTEX) { cafeMutex.lock();}
	handle_index.modify(it_handle, change_monitorPolicy(mp));  
	if(MUTEX) { cafeMutex.unlock();}
    }
    else
    {    
        status=ECAFE_INVALID_HANDLE;
        if (printErrorPolicy.getInvalidHandle())
        {
            cafeStatus.report(status);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
	
    }
    return status;
}

/**
 *  \brief Gets vector of MonitorIDs in waiting (value=0), i.e., waiting for channel connection
 *  \param _handle input : Conduit object reference
 *  \return returns vector of MonitorIDsInWaiting; length 0 is none or invalid handle
 */
vector<unsigned int> HandleHelper::getMonitorIDsInWaiting(unsigned int _handle)
{
    vector<MonitorPolicy> mpVec;
    mpVec.clear();
    getMonitorPolicyInWaitingVector(_handle, mpVec);
    vector<unsigned int> monidList;
    monidList.clear();
    monidList.reserve(mpVec.size());
    for (unsigned int i=0; i<mpVec.size(); ++i)
    {
        monidList.push_back(mpVec[i].getMonitorID());	//Will be zero!
    }
    return monidList;
}

/**
 *  \brief Gets vector of MonitorIDs
 *  \param _handle input : Conduit object reference
 *  \return returns vector of MonitorIDs; length 0 is none or invalid handle
 */
vector<unsigned int> HandleHelper::getMonitorIDs(unsigned int _handle)
{
    vector<MonitorPolicy> mpVec;
    mpVec.clear();
    getMonitorPolicyVector(_handle, mpVec);
    vector<unsigned int> monidList;
    monidList.clear();
    monidList.reserve(mpVec.size());
    for (unsigned int i=0; i<mpVec.size(); ++i)
    {
        monidList.push_back(mpVec[i].getMonitorID());
    }
    return monidList;
}

/**
 *  \brief Gets Number of MonitorPolicyVectors
 *  \param _handle input : Conduit object reference
 *  \return nmon output: No. of monitors
 *  \return ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 */
int HandleHelper::getNmonitor(unsigned int _handle)
{
    vector<MonitorPolicy> mpVec;
    mpVec.clear();
    getMonitorPolicyVector(_handle, mpVec);
    return mpVec.size();
}

/**
 *  \brief Gets Number of MonitorPolicyVectors for <= DBR_TIME type
 *  \param _handle input : Conduit object reference
 *  \return nmon output: No. of <= DBR_TIME type monitors
 *  \return ICAFE_NORMAL else ECAFE_INVILID_HANDLE
 */
int HandleHelper::getNmonitorData(unsigned int _handle)
{
    vector<MonitorPolicy> mpVec;
    mpVec.clear();
    getMonitorPolicyVector(_handle, mpVec);
    unsigned int nmonData=0;
    //Loop mpVec
    for (unsigned int i=0; i<mpVec.size(); ++i)
    {
        if (mpVec[i].getCafeDbrType()<= CAFENUM::DBR_TIME)
        {
            ++nmonData;
        }
    }
    return nmonData;
}


/**
 *  \brief Gets Number of MonitorPolicyVectors for CTRL_ type
 *  \param _handle input : Conduit object reference
 *  \return nmon output: No. of CTRL_ type monitors
 *  \return ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 */
int HandleHelper::getNmonitorCtrl(unsigned int _handle)
{
    vector<MonitorPolicy> mpVec;
    mpVec.clear();
    getMonitorPolicyVector(_handle, mpVec);
    unsigned int nmonCtrl=0;
    //Loop mpVec
    for (unsigned int i=0; i<mpVec.size(); ++i)
    {
        if (mpVec[i].getCafeDbrType() >= CAFENUM::DBR_GR && mpVec[i].getCafeDbrType()<= CAFENUM::DBR_CTRL)
        {
            ++nmonCtrl;
        }
    }
    return nmonCtrl;
}

/**
 *  \brief Gets the MonitorPolicyVector
 *  \param _handle input : Conduit object reference
 *  \param mpV output: vector<MonitorPolicy>
 *  \return ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 */
int HandleHelper::getMonitorPolicyVector(unsigned int _handle, std::vector<MonitorPolicy> &mpV)
{
#define __METHOD__ \
    "HandleHelper::getMonitorPolicyVector(unsigned int _handle, vector<MonitorPolicy> &mpV"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    int status=ICAFE_NORMAL;

    if (it_handle != handle_index.end())
    {
        mpV=(*it_handle).getMonitorPolicyVector();
    }
    else
    {
        status=ECAFE_INVALID_HANDLE;
        if (printErrorPolicy.getInvalidHandle())
        {
            cafeStatus.report(status);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return status;
#undef __METHOD__
}


/**
 *  \brief Gets the MonitorPolicyVector
 *  \param _handle input : Conduit object reference
 *  \param mpV output: vector<MonitorPolicy>
 *  \return ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 */
int HandleHelper::getMonitorPolicyInWaitingVector(unsigned int _handle, std::vector<MonitorPolicy> &mpV)
{
#define __METHOD__ \
    "HandleHelper::getMonitorPolicyInWaitingVector(unsigned int _handle, vector<MonitorPolicy> &mpV"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    int status=ICAFE_NORMAL;

    if (it_handle != handle_index.end())
    {
        mpV=(*it_handle).getMonitorPolicyInWaitingVector();
    }
    else
    {
        status=ECAFE_INVALID_HANDLE;
        if (printErrorPolicy.getInvalidHandle())
        {
            cafeStatus.report(status);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return status;
#undef __METHOD__
}


/**
 *  \brief Sets the no of elements to read out from an array, e.g. waveform, to native value for all handles
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int HandleHelper::setNelem()
{
#define __METHOD__ "HandleHelper::setNelem()"


    int gStatus=ICAFE_NORMAL;
    int localStatus;

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {

        localStatus=setNelem( (*itcs).getHandle());
        if (localStatus !=ICAFE_NORMAL)
        {
            gStatus=localStatus;
        }
    }

    return gStatus;

#undef __METHOD__
}



/**
 *  \brief Sets the no of elements to read out from an array, e.g. waveform, to native value
 *  \param  _handle input: Conduit object reference
 *  \return no of elements else 0 if ECAFE_INVALID_HANDLE
 */
unsigned int HandleHelper::setNelem(unsigned int _handle)
{
#define __METHOD__ "HandleHelper::setNelem(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    unsigned int nelemForDataTransfer=0;

    if (it_handle != handle_index.end())
    {

        nelemForDataTransfer=(*it_handle).getChannelRegalia().getNelem() ;

        nelemForDataTransfer=  max(nelemForDataTransfer, (unsigned int) 1); // Should not really be needed

        channelRequestMetaDataClient=(*it_handle).getChannelRequestMetaDataClient();
        channelRequestMetaDataClient.setNelem( nelemForDataTransfer );

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelRequestMetaDataClient(
                                channelRequestMetaDataClient));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }

    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return nelemForDataTransfer;
#undef __METHOD__
}



/**
 *  \brief Sets the no of elements to read out from an array, e.g. waveform
 *  \param _handle input: Conduit object reference
 *  \param _nelem input:  No of elements for ca data transfer
 *  \return no of elements set = min( (*it_handle).getChannelRegalia().getNelem(),_nelem)
 *          else 0 if ECAFE_INVALID_HANDLE
 */
unsigned int HandleHelper::setNelem(unsigned int _handle, unsigned int _nelem)
{
#define __METHOD__ "HandleHelper::setNelem(unsigned int _handle, unsigned int _nelem)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    unsigned int nelemForDataTransfer=0;

    if (it_handle != handle_index.end())
    {

        nelemForDataTransfer=  min( (*it_handle).getChannelRegalia().getNelem(),_nelem);
        nelemForDataTransfer=  max(nelemForDataTransfer, (unsigned int) 1);
        channelRequestMetaDataClient=(*it_handle).getChannelRequestMetaDataClient();

        channelRequestMetaDataClient.setNelem( nelemForDataTransfer );

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelRequestMetaDataClient(
                                channelRequestMetaDataClient));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }
        /*
        cout << __FILE__ << "//" << __METHOD__ << endl;
        cout << "handle " << _handle << endl;
        cout << "nelemForDataTransfer " <<  nelemForDataTransfer << endl;
        cout << "nelemClient"  << getNelemClient (_handle) << endl;
        cout << "nelemNative"  << getNelemNative (_handle) << endl;
        cout << "nelemRequest" << getNelemRequest(_handle) << endl;
        */

        //Check on getNelemCache in channelRequestMetaData
        //Can't use this as this method is also used by set!
        //A set to a WF for one element then sets nelemCache to 1!
        /*
        channelRequestMetaData=(*it_handle).getChannelRequestMetaData();
        if (nelemForDataTransfer < channelRequestMetaData.getNelemCache() ) {
            channelRequestMetaData.setNelemCache( nelemForDataTransfer );
            if(MUTEX){cafeMutex.lock();}
            handle_index.modify(it_handle, change_channelRequestMetaData(
                    channelRequestMetaData));
            if(MUTEX){cafeMutex.unlock();}
        }
        */
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return nelemForDataTransfer;
#undef __METHOD__
}


/**
 *  \brief Sets the no of elements to read out from an cached array to native value, e.g. waveform
 *  \param _handle input: Conduit object reference
 *  \return no of elements (*it_handle).getChannelRegalia().getNelem()
 *          else 0 if ECAFE_INVALID_HANDLE
 */
unsigned int   HandleHelper::setNelemToRetrieveFromCache (unsigned int _handle)
{
#define __METHOD__ "HandleHelper::setNelemToRetrieveFromCache(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    unsigned int nelemForDataTransfer=0;



    if (it_handle != handle_index.end())
    {

        nelemForDataTransfer=   (*it_handle).getChannelRegalia().getNelem();
        nelemForDataTransfer=  max(nelemForDataTransfer, (unsigned int) 1);


        channelRequestMetaData=(*it_handle).getChannelRequestMetaData();

        //lessen the number if less in buffer
        nelemForDataTransfer=  min(nelemForDataTransfer, channelRequestMetaData.getNelem()-channelRequestMetaData.getOffset());

        channelRequestMetaData.setNelemCache( nelemForDataTransfer );

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelRequestMetaData(
                                channelRequestMetaData));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }

    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return nelemForDataTransfer;
#undef __METHOD__
}



/**
 *  \brief Sets the no of elements to read out from an cached array, e.g. waveform
 *  \param _handle input: Conduit object reference
 *  \param _nelem input:  No of elements for data transfer from cache
 *  \return no of elements set = min( (*it_handle).getChannelRegalia().getNelem(),_nelem)
 *          else 0 if ECAFE_INVALID_HANDLE
 */
unsigned int   HandleHelper::setNelemToRetrieveFromCache (unsigned int _handle, unsigned int _nelem)
{
#define __METHOD__ "HandleHelper::setNelemToRetrieveFromCache(unsigned int _handle, unsigned int _nelem)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    unsigned int nelemForDataTransfer=0;

    if (it_handle != handle_index.end())
    {

        nelemForDataTransfer=  min( (*it_handle).getChannelRegalia().getNelem(),_nelem);

        nelemForDataTransfer=  max(nelemForDataTransfer, (unsigned int) 1);

        channelRequestMetaData=(*it_handle).getChannelRequestMetaData();
        //lessen the number if less in buffer

        nelemForDataTransfer=  min(nelemForDataTransfer,channelRequestMetaData.getNelem()-channelRequestMetaData.getOffset() );

        channelRequestMetaData.setNelemCache( nelemForDataTransfer );

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelRequestMetaData(
                                channelRequestMetaData));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }

    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return nelemForDataTransfer;
#undef __METHOD__
}


/**
 *  \brief Sets the no of elements to read out from an cached array to min(native, ) value, e.g. waveform
 *  \param _handle input: Conduit object reference
 *  \return no of elements (*it_handle).getChannelRegalia().getNelem()
 *          else 0 if ECAFE_INVALID_HANDLE
 */
unsigned int   HandleHelper::setNelemToRetrieveFromCtrlCache (unsigned int _handle)
{
#define __METHOD__ "HandleHelper::setNelemToRetrieveFromCtrlCache(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    unsigned int nelemForDataTransfer=0;

    if (it_handle != handle_index.end())
    {


        nelemForDataTransfer=  min(nelemForDataTransfer, MAX_NELEM_FOR_CTRL_BUFFER);
        nelemForDataTransfer=  max(nelemForDataTransfer, (unsigned int) 1);
        channelRequestMetaCtrl=(*it_handle).getChannelRequestMetaCtrl();
        //lessen the number if less in buffer
        nelemForDataTransfer=  min(nelemForDataTransfer, channelRequestMetaCtrl.getNelem()-channelRequestMetaCtrl.getOffset());
        channelRequestMetaCtrl.setNelemCache( nelemForDataTransfer );

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelRequestMetaCtrl(
                                channelRequestMetaCtrl));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }

    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return nelemForDataTransfer;
#undef __METHOD__
}



/**
 *  \brief Sets the no of elements to read out from an cached array, e.g. waveform
 *  \param _handle input: Conduit object reference
 *  \param _nelem input:  No of elements for data transfer from cache
 *  \return no of elements set = min( (*it_handle).getChannelRegalia().getNelem(),_nelem)
 *          else 0 if ECAFE_INVALID_HANDLE
 */
unsigned int   HandleHelper::setNelemToRetrieveFromCtrlCache (unsigned int _handle, unsigned int _nelem)
{
#define __METHOD__ "HandleHelper::setNelemToRetrieveFromCtrlCache(unsigned int _handle, unsigned int _nelem)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    unsigned int nelemForDataTransfer=0;

    if (it_handle != handle_index.end())
    {

        nelemForDataTransfer=  min( (*it_handle).getChannelRegalia().getNelem(),_nelem);
        nelemForDataTransfer=  min(nelemForDataTransfer, MAX_NELEM_FOR_CTRL_BUFFER);
        nelemForDataTransfer=  max(nelemForDataTransfer, (unsigned int) 1);
        channelRequestMetaCtrl=(*it_handle).getChannelRequestMetaCtrl();
        //lessen the number if less in buffer
        nelemForDataTransfer=  min(nelemForDataTransfer, channelRequestMetaCtrl.getNelem()-channelRequestMetaCtrl.getOffset());
        channelRequestMetaCtrl.setNelemCache( nelemForDataTransfer );

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelRequestMetaCtrl(
                                channelRequestMetaCtrl));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }

    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return nelemForDataTransfer;
#undef __METHOD__
}



/**
 *  \brief Gets the no of elements to be read out from a cached array, e.g. waveform
 *  \param _handle input: Conduit object reference
 *  \return no of elements from channelRequestMetaData.getNelemCache();
 *          else 0 if ECAFE_INVALID_HANDLE
 */
unsigned int HandleHelper::getNelemToRetrieveFromCache (unsigned int _handle)
{
#define __METHOD__ "HandleHelper::getNelemToRetrieveFromCache(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    unsigned int nelemFromCache=0;

    if (it_handle != handle_index.end())
    {
        channelRequestMetaData=(*it_handle).getChannelRequestMetaData();

        //There may be less in buffer than requested
        //cout <<__METHOD__ << __LINE__ << endl;
        //cout << channelRequestMetaData.getNelemCache() << " // " << channelRequestMetaData.getNelem() << endl;
        nelemFromCache=min(channelRequestMetaData.getNelemCache(),channelRequestMetaData.getNelem()-channelRequestMetaData.getOffset()) ;
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return nelemFromCache;
#undef __METHOD__
}



/**
 *  \brief Gets the no of elements to be read out from a cached array, e.g. waveform
 *  \param _handle input: Conduit object reference
 *  \return no of elements set = min( (*it_handle).getChannelRegalia().getNelem(),_nelem)
 *          else 0 if ECAFE_INVALID_HANDLE
 */
unsigned int   HandleHelper::getNelemToRetrieveFromCtrlCache (unsigned int _handle)
{
#define __METHOD__ "HandleHelper::getNelemToRetrieveFromCtrlCache(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    unsigned int nelemFromCache=0;

    if (it_handle != handle_index.end())
    {
        channelRequestMetaCtrl=(*it_handle).getChannelRequestMetaCtrl();
        //There may be less in buffer than requested
        nelemFromCache=min(channelRequestMetaCtrl.getNelemCache(),channelRequestMetaCtrl.getNelem()-channelRequestMetaCtrl.getOffset()) ;
        //nelemFromCache=channelRequestMetaCtrl.getNelemCache();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists " << endl;
        }
    }
    return nelemFromCache;
#undef __METHOD__
}



/**
 *  \brief Retrieves the no of elements to read out from an array, e.g. waveform as requested by client
 *  \param _handle input: Conduit object reference
 *  \return no of elements from (*it_handle).getChannelRequestMetaDataClient().getNelem()
 *          else 0 if ECAFE_INVALID_HANDLE
 */
unsigned int HandleHelper::getNelemClient(unsigned int _handle)
{
#define __METHOD__ "HandleHelper::getNelem(unsigned int _handle, unsigned int _nelem)"


    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    unsigned int nelemForDataTransfer=0;

    if (it_handle != handle_index.end())
    {
        nelemForDataTransfer=  (*it_handle).getChannelRequestMetaDataClient().getNelem();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists! " << endl;
        }
    }

    return nelemForDataTransfer;
#undef __METHOD__
}


/**
 *  \brief Retrieves the no of elements to read out from an array, e.g. waveform as delivered to ca
 *  \param _handle input: Conduit object reference
 *  \return no of elements from (*it_handle).getChannelRequestMetaData().getNelem()
 *          else 0 if ECAFE_INVALID_HANDLE
 */
unsigned int HandleHelper::getNelemRequest(unsigned int _handle)
{
#define __METHOD__ "HandleHelper::getNelemRequest(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    unsigned int nelemForDataTransfer=0;

    if (it_handle != handle_index.end())
    {
        nelemForDataTransfer=  (*it_handle).getChannelRequestMetaData().getNelem();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists! " << endl;
        }
    }
    return nelemForDataTransfer;
#undef __METHOD__
}


/**
 *  \brief Retrieves the no of native elements
 *  \param _handle input: Conduit object reference
 *  \return no of elements from (*it_handle).getChannelRegalia().getNelem()
 *          else 0 if ECAFE_INVALID_HANDLE
 */
unsigned int HandleHelper::getNelemNative(unsigned int _handle)
{
#define __METHOD__ "HandleHelper::getNelemNative(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    unsigned int nelemForDataTransfer=0;

    if (it_handle != handle_index.end())
    {
        nelemForDataTransfer=  (*it_handle).getChannelRegalia().getNelem();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists! " << endl;
        }
    }
    return nelemForDataTransfer;
#undef __METHOD__
}



/**
 *  \brief Retrieves the no of elements as initiated by client, no of native elements,
 *  no of elements requested to ca
 *  \param _handle input: Conduit object reference
 *  \param c output: no of elements as stated by client
 *  \param n output: no of native elements
 *  \param r output: no of elements as requested to ca
 *  \return ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 */
int HandleHelper::getNelem(unsigned int _handle,
                           unsigned int &c, unsigned int &n, unsigned int &r)
{
#define __METHOD__ "HandleHelper::getNelem(unsigned int _handle, client, native, request)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        c=  (*it_handle).getChannelRequestMetaDataClient().getNelem();
        n=  (*it_handle).getChannelRegalia().getNelem();
        r=  (*it_handle).getChannelRequestMetaData().getNelem();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists! " << endl;
        }
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef __METHOD__
}


/**
 *  \brief Retrieves the no of elements read out from an array
 *  e.g. waveform, as delivered to ca through DBR_CTRL
 *  \param _handle input: Conduit object reference
 *  \return no of elements from (*it_handle).getChannelRequestMetaCtrl().getNelem()
 *          else 0 if ECAFE_INVALID_HANDLE
 */
unsigned int HandleHelper::getNelemRequestCtrl(unsigned int _handle)
{
#define __METHOD__ "HandleHelper::getNelemRequestCtrl(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    unsigned int nelemForDataTransfer=0;

    if (it_handle != handle_index.end())
    {
        nelemForDataTransfer=  (*it_handle).getChannelRequestMetaCtrl().getNelem();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists! " << endl;
        }
    }
    return nelemForDataTransfer;
#undef __METHOD__
}



/**
 *  \brief Retrieves the no of elements read out from an array
 *  e.g. waveform, as delivered to ca through DBR_CTRL
 *  \param _handle input: Conduit object reference
 *  \return no of elements from (*it_handle).getChannelRequestMetaCtrl().getNelem()
 *          else 0 if ECAFE_INVALID_HANDLE
 */
unsigned int HandleHelper::getNelemClientCtrl(unsigned int _handle)
{
#define __METHOD__ "HandleHelper::getNelemClientCtrl(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    unsigned int nelemForDataTransfer=0;

    if (it_handle != handle_index.end())
    {
        nelemForDataTransfer=  (*it_handle).getChannelRequestMetaCtrlClient().getNelem();
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists! " << endl;
        }
    }
    return nelemForDataTransfer;
#undef __METHOD__
}



/**
 *  \brief Sets the no of elements to read out from an array, e.g. waveform, via DBR_CTRL
 *  \param _handle input: Conduit object reference
 *  \param _nelem input:  No of elements for ca data transfer
 *  \return no of elements set = min( (*it_handle).getChannelRegalia().getNelem(),_nelem)
 *          else 0 if ECAFE_INVALID_HANDLE
 */
unsigned int HandleHelper::setNelemCtrl(unsigned int _handle, unsigned int _nelem)
{
#define __METHOD__ "HandleHelper::setNelemCtrl(unsigned int _handle, unsigned int _nelem)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    unsigned int nelemForDataTransfer=0;

    if (it_handle != handle_index.end())
    {

        nelemForDataTransfer=  min( (*it_handle).getChannelRegalia().getNelem(),_nelem);
        nelemForDataTransfer=  min(nelemForDataTransfer, MAX_NELEM_FOR_CTRL_BUFFER);
        nelemForDataTransfer=  max(nelemForDataTransfer, (unsigned int) 1); //precaution
        channelRequestMetaCtrlClient=(*it_handle).getChannelRequestMetaCtrlClient();
        channelRequestMetaCtrlClient.setNelem( nelemForDataTransfer );



        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelRequestMetaCtrlClient(
                                channelRequestMetaCtrlClient));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }

    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists! " << endl;
        }
    }
    return nelemForDataTransfer;
#undef __METHOD__
}

/**
 *  \brief Sets the offset; give element no from which data should be returned
 *  \param _handle input: Conduit object reference
 *  \param _offset input: Offset value
 *  \return offset: min( (*it_handle).getChannelRegalia().getNelem()-1, _offset))
 */
unsigned int HandleHelper::setOffset(unsigned int _handle, unsigned int _offset)
{
#define __METHOD__ "HandleHelper::setOffset(unsigned int _handle, unsigned int _offset)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    unsigned int offsetForDataTransfer=0;

    if (it_handle != handle_index.end())
    {
        offsetForDataTransfer= min( (*it_handle).getChannelRegalia().getNelem()-1, _offset);
        channelRequestMetaDataClient=(*it_handle).getChannelRequestMetaDataClient();
        channelRequestMetaDataClient.setOffset( offsetForDataTransfer);
        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelRequestMetaDataClient(
                                channelRequestMetaDataClient));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists! " << endl;
        }
    }
    return offsetForDataTransfer;
#undef __METHOD__
}



/**
 *  \brief Retrieves the offset; gives element no from which data should be returned
 *  \param _handle input: Conduit object reference
 *  \return offset: Offset valu
 */
unsigned int HandleHelper::getOffset(unsigned int _handle)
{
#define __METHOD__ "HandleHelper::getOffset(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    unsigned int offsetForDataTransfer=0;

    if (it_handle != handle_index.end())
    {
        channelRequestMetaDataClient=(*it_handle).getChannelRequestMetaDataClient();
        offsetForDataTransfer=channelRequestMetaDataClient.getOffset( );
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists! " << endl;
        }
    }
    return offsetForDataTransfer;
#undef __METHOD__
}


/**
 *  \brief Retrieves the last offset; gives element no from which data was actually returned
 *  \param _handle input: Conduit object reference
 *  \return offset: Offset valu
 */
unsigned int HandleHelper::getOffsetLast(unsigned int _handle)
{
#define __METHOD__ "HandleHelper::getOffsetLast(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    unsigned int offsetForDataTransfer=0;

    if (it_handle != handle_index.end())
    {
        channelRequestMetaData=(*it_handle).getChannelRequestMetaData();
        offsetForDataTransfer=channelRequestMetaData.getOffset( );
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists! " << endl;
        }
    }
    return offsetForDataTransfer;
#undef __METHOD__
}


/**
 *  \brief Retrieves groupHandle from groupName from within a given ca context
 *  \param _groupName input: group Name
 *  \param ccc input: ca client context
 *  \return groupHandle else 0 if group not found
 */
unsigned int HandleHelper::getGroupHandleFromGroupName(const char * _groupName, ca_client_context * ccc)
{
#define __METHOD__  "HandleHelper::getGroupHandleFromGroupName"

    cafeGroup_set_by_groupName & groupName_index = gs.get<by_groupName> ();
    cafeGroup_set_by_groupName::iterator it_groupName;

    it_groupName = groupName_index.find(_groupName);

    if (it_groupName != groupName_index.end())
    {
      //std::cout << __METHOD__ << std::endl;
      //std::cout << ccc << " " <<  (*it_groupName).getClientContext() << std::endl;
        if (ccc == (*it_groupName).getClientContext())
        {
            return (*it_groupName).groupHandle;
        }
        else
        {
            // Loop through all elements and search for pv/ca_client_context match
            for (itgs = gs.begin(); itgs != gs.end(); ++itgs)
            {
	      //std::cout << __METHOD__ << std::endl;
	      // std::cout << "overload-1 " << *itgs << " " << std:: endl;
	      //std::cout << (*itgs).getClientContext() << std::endl;
                if (!strcmp((*itgs).getGroupName(), _groupName) && (*itgs).getClientContext() == ccc)
                {
                    return (*itgs).groupHandle;
                }
                //Should we be closing from a thread (!!)
                else if (!strcmp((*itgs).getGroupName(), _groupName) &&  0 == ccc)
                {
                    return (*itgs).groupHandle;
                }
            }
        }
    }
    else
    {
        // Loop through all elements and search for pv/ca_client_context match
        for (itgs = gs.begin(); itgs != gs.end(); ++itgs)
        {
            if ((strcmp((*itgs).getGroupName(), _groupName) == 0) && ((*itgs).getClientContext() == ccc))
            {
                cout << " MATCH FOUND Handle Number is= " << (*itgs).groupHandle << endl;
                return (*itgs).groupHandle;
            }
        }
    }

    return 0; //Handle Not found
#undef __METHOD__
}


/**
 *  \brief Retrieves groupName from grouphanlde
 *  \param _groupHandle input: handle to groupConduit object
 *  \return groupName else "" if groupHandle not found
 */
std::string HandleHelper::getGroupNameFromGroupHandle(unsigned int _groupHandle)
{

#define __METHOD__ "HandleHelper::getGroupNameFromGroupHandle"

    cafeGroup_set_by_groupHandle & groupHandle_index = gs.get<by_groupHandle> ();
    cafeGroup_set_by_groupHandle::iterator it_groupHandle;

    it_groupHandle = groupHandle_index.find(_groupHandle);

    if (it_groupHandle != groupHandle_index.end())
    {

        return (*it_groupHandle).groupName;

    }
    else
    {
        // Loop through all elements and search for grouphandle match
        for (itgs = gs.begin(); itgs != gs.end(); ++itgs)
        {
            //ConduitGroup cg = *itgs;

            if ((*itgs).getGroupHandle() == _groupHandle )
            {
                //    cout << " MATCH FOUND Handle Number is= " << (cg).groupHandle << endl;
                return (*itgs).groupName;
            }
        }
    }

    cafeStatus.report(ECAFE_UNKNOWN_GROUP);
    return ""; //GroupName Not found
#undef __METHOD__
}

/**
 *  \brief Retrieves number of process variables within a group as identified by the groupHandle
 *  \param _groupHandle input: handle to groupConduit object
 *  \return number of PVs within group
 */
unsigned int HandleHelper::getGroupNPV(unsigned int _groupHandle)
{

#define __METHOD__ "HandleHelper::getGroupNPV(unsigned int)"

    cafeGroup_set_by_groupHandle & groupHandle_index = gs.get<by_groupHandle> ();
    cafeGroup_set_by_groupHandle::iterator it_groupHandle;

    it_groupHandle = groupHandle_index.find(_groupHandle);

    if (it_groupHandle != groupHandle_index.end())
    {
        return (*it_groupHandle).getNMember();

    }
    else
    {
        // Loop through all elements and search for grouphandle match
        for (itgs = gs.begin(); itgs != gs.end(); ++itgs)
        {

            if ((*itgs).getGroupHandle() == _groupHandle )
            {
                //    cout << " MATCH FOUND Handle Number is= " << (cg).groupHandle << endl;
                return (*itgs).getNMember();
            }
        }
    }

    cafeStatus.report(ECAFE_UNKNOWN_GROUP);
    return 0; //GroupHandle Not found
#undef __METHOD__
}

/**
 *  \brief Retrieves number of process variables within a group for a given ca client context
 *  \param _groupName input: name of CAFAE PVGroup
 *  \param ccc input: ca_client_context
 *  \return number of PVs within group
 */
unsigned int HandleHelper::getGroupNPV(const char * _groupName, ca_client_context * ccc)
{
#define __METHOD__  "HandleHelper::getGroupNPV"

    cafeGroup_set_by_groupName & groupName_index = gs.get<by_groupName> ();
    cafeGroup_set_by_groupName::iterator it_groupName;

    it_groupName = groupName_index.find(_groupName);

    if (it_groupName != groupName_index.end())
    {
        if (ccc == (*it_groupName).getClientContext())
        {
            return (*it_groupName).getNMember();
        }
        else
        {
            // Loop through all elements and search for pv/ca_client_context match
            for (itgs = gs.begin(); itgs != gs.end(); ++itgs)
            {
                //cout << "overload-1 " << *itgs << " " << (*itgs).getClientContext() << endl;
                if (!strcmp((*itgs).getGroupName(), _groupName) && (*itgs).getClientContext() == ccc)
                {
                    return (*itgs).getNMember();
                }
            }
        }
    }
    else
    {
        // Loop through all elements and search for pv/ca_client_context match
        for (itgs = gs.begin(); itgs != gs.end(); ++itgs)
        {

            if ((strcmp((*itgs).getGroupName(), _groupName) == 0) && ((*itgs).getClientContext() == ccc))
            {
                // cout << " MATCH FOUND Handle Number is= " << (*itgs).groupHandle << endl;
                return (*itgs).getNMember();
            }
        }
    }

    return 0; //Handle Not found
#undef __METHOD__
}




/**
 *  \brief Sets the buffer size for map<pulseID,pvdata> for use in cafe bsread for all handles
 *  \param _bufferSize input: bufferSize value
 *  \return ICAFE_NORMAL
 */
int HandleHelper::setMapPulseIDBufferSizeAll(unsigned short _bufferSize)
{
#define __METHOD__ "HandleHelper::setMapPulseIDBufferSize(unsigned short _bufferSize)"

    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(itcs, change_mapPulseIDBufferSize(
                                std::max(1,(int)_bufferSize)));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }
    }

    return ICAFE_NORMAL;
#undef __METHOD__
}



/**
 *  \brief Sets the buffer size for map<pulseID,pvdata> for use in cafe bsread
 *  \param _handle input: Conduit object reference
 *  \param _bufferSize input: bufferSize value
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int HandleHelper::setMapPulseIDBufferSize(unsigned int _handle, unsigned short _bufferSize)
{
#define __METHOD__ "HandleHelper::setMapPulseIDBufferSize(unsigned int _handle, unsigned short _bufferSize)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_mapPulseIDBufferSize(
                                std::max(1,(int)_bufferSize)));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }
    }
    else
    {
        if (printErrorPolicy.getInvalidHandle())
        {
            cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << endl;
            cafeStatus.report(ECAFE_INVALID_HANDLE);
            cout << "Handle=" << _handle << " either never existed or no longer exists! " << endl;
            return ECAFE_INVALID_HANDLE;
        }
    }
    return ICAFE_NORMAL;
#undef __METHOD__
}
