///
/// \file    policyHelper.cpp
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#include <policyHelper.h>
#include <global.h>

using namespace std;

////////////////////////////////// REQUEST POLICIES ///////////////////////////////////////////////////////////

/**
 *  \brief PolicyHelper::getChannelGetCacheWaitPolicy \n
 *  Retrieves ChannelGetCacheWaitPolicy object whose method getWhenKind() \n
 *  determines if ChannelGetCacheWaitPolicyKind CAFENUM::GET_CACHE_NO_WAIT
 *  or CAFENUM::GET_CACHE_WAIT \n
 *  \param  _handle input:  reference handle
 *  \param  cwp output: ChannelGetCacheWaitPolicy object
 *  \return status ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 *
 */
int PolicyHelper::getChannelGetCacheWaitPolicy(unsigned int _handle,
        ChannelGetCacheWaitPolicy & cwp)
{
#define __METHOD__ "policyHelper::getChannelGetCacheWaitPolicy(handle, ChannelGetCacheWaitPolicy & cwp)"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        cwp=(*it_handle).getChannelGetCacheWaitPolicy();
    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef  __METHOD__
}



/**
 *  \brief PolicyHelper::setChannelGetCacheWaitPolicy \n
 *  Sets ChannelGetCacheWaitPolicy object whose method getWhenKind() \n
 *  determines if ChannelGetCacheWaitPolicyKind CAFENUM::GET_CACHE_NO_CHECK
 *  CAFENUM::GET_CACHE_NO_WAIT or CAFENUM::GET_CACHE_WAIT \n
 *  \param  _handle input: reference handle
 *  \param  cwp     input: ChannelGetCacheWaitPolicy object
 *  \return status ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 *
 */
int PolicyHelper::setChannelGetCacheWaitPolicy(unsigned int _handle,
        ChannelGetCacheWaitPolicy  cwp)
{
#define __METHOD__ "policyHelper:setChannelGetCacheWaitPolicy(handle, ChannelGetCacheWaitPolicy cwp)"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        if (cwp.getWaitKind() < CAFENUM::GET_CACHE_NO_CHECK || cwp.getWaitKind() > CAFENUM::GET_CACHE_WAIT)
        {
            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__  << endl;
            cout << "Handle= "<< (*it_handle).getHandle() << " PV= " << (*it_handle).getPV() << endl;
            cout << cwp.getWaitKind()  << " is an unknown policy!" << endl;
            cout << "Valid types are GET_CACHE_NO_CHECK(0) GET_CACHE_NO_WAIT(1) or  GET_CACHE_WAIT(2)" << endl;
            return ECAFE_INVALID_CAFENUM_POLICY_TYPE;
        }

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelGetCacheWaitPolicy(cwp));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }

    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef  __METHOD__
}




/**
 *  \brief PolicyHelper::setChannelGetCacheWaitPolicy \n
 *  Sets the ChannelGetCacheWaitPolicy object (for all handles) whose method getWhenKind() \n
 *  determines if ChannelGetCacheWaitPolicyKind CAFENUM::GET_CACHE_NO_CEHCK
 *  CAFENUM::GET_CACHE_NO_WAIT or CAFENUM::GET_CACHE_WAIT \n
 *  \param  cwp     input: ChannelGetCacheWaitPolicy object
 *  \return status ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 *
 */
int PolicyHelper::setChannelGetCacheWaitPolicy( ChannelGetCacheWaitPolicy  cwp)
{
#define __METHOD__ "policyHelper:setChannelGetCacheWaitPolicy(ChannelGetCacheWaitPolicy cwp)"

    int localStatus=ICAFE_NORMAL;
    int gStatus=ICAFE_NORMAL;
    bool sflag=false;

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        localStatus= setChannelGetCacheWaitPolicy ((*itcs).getHandle(), cwp);
        if (localStatus !=ICAFE_NORMAL && !sflag)
        {
            gStatus=localStatus;
            sflag=true;
        }
    }
    return gStatus;

#undef  __METHOD__
}



/**
 *  \brief PolicyHelper::getChannelGetActionWhenMonitorPolicy \n
 *  Retrieves ChannelGetActionWhenMonitorPolicy object whose method getWhenKind() \n
 *  determines if ChannelGetActionWhenMonitorPolicyKind CAFENUM::GET_FROM_CACHE
 *  or CAFENUM::GET_FROM_IOC \n
 *  \param  _handle input:  reference handle
 *  \param  awmp output: ChannelGetActionWhenMonitorPolicy object
 *  \return status ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 *
 */
int PolicyHelper::getChannelGetActionWhenMonitorPolicy(unsigned int _handle,
        ChannelGetActionWhenMonitorPolicy & awmp)
{
#define __METHOD__ "policyHelper::getChannelGetActionWhenMonitorPolicy(handle, ChannelGetActionWhenMonitorPolicy & awmp)"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        awmp=(*it_handle).getChannelGetActionWhenMonitorPolicy();
    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef  __METHOD__
}


/**
 *  \brief PolicyHelper::setChannelGetActionWhenMonitorPolicy \n
 *  Retrieves ChannelGetActionWhenMonitorPolicy object whose method getWhenKind() \n
 *  determines if ChannelGetActionWhenMonitorPolicyKind CAFENUM::GET_FROM_CACHE
 *  CAFENUM::GET_FROM_IOC \n
 *  \param  _handle input:  reference handle
 *  \param  awmp   input: ChannelGetActionWhenMonitorPolicy object
 *  \return status ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 *
 */
int PolicyHelper::setChannelGetActionWhenMonitorPolicy(unsigned int _handle,
        ChannelGetActionWhenMonitorPolicy awmp)
{
#define __METHOD__ "policyHelper::setChannelGetActionWhenMonitorPolicy(handle, ChannelGetActionWhenMonitorPolicy  awmp)"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        if (awmp.getActionKind()!=CAFENUM::GET_FROM_CACHE && awmp.getActionKind()!=CAFENUM::GET_FROM_IOC)
        {
            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__  << endl;
            cout << "Handle= "<< (*it_handle).getHandle() << " PV= " << (*it_handle).getPV() << endl;
            cout << awmp.getActionKind()  << " is an unknown policy!" << endl;
            cout << "Valid types are: GET_FROM_CACHE (0) or GET_FROM_IOC (1)" << endl;
            return ECAFE_INVALID_CAFENUM_POLICY_TYPE;
        }

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelGetActionWhenMonitorPolicy(awmp));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }

    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;
#undef  __METHOD__
}



/**
 *  \brief PolicyHelper::setChannelGetActionWhenMonitorPolicy \n
 *  Retrieves ChannelGetActionWhenMonitorPolicy object (for all handles) whose method getWhenKind() \n
 *  determines if ChannelGetActionWhenMonitorPolicyKind CAFENUM::GET_FROM_CACHE
 *  or CAFENUM::GET_FROM_IOC \n
 *  \param  awmp   input: ChannelGetActionWhenMonitorPolicy object
 *  \return status ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 *
 */
int PolicyHelper::setChannelGetActionWhenMonitorPolicy(ChannelGetActionWhenMonitorPolicy awmp)
{
#define __METHOD__ "policyHelper::setChannelGetActionWhenMonitorPolicy(ChannelGetActionWhenMonitorPolicy awmp)"

    int localStatus=ICAFE_NORMAL;
    int gStatus=ICAFE_NORMAL;
    bool sflag=false;

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        localStatus= setChannelGetActionWhenMonitorPolicy ((*itcs).getHandle(), awmp);
        if (localStatus !=ICAFE_NORMAL && !sflag)
        {
            gStatus=localStatus;
            sflag=true;
        }
    }
    return gStatus;


#undef  __METHOD__
}

/**
 *  \brief PolicyHelper::getChannelRequestDataTypePolicy \n
 *  Retrieves ChannelRequestDataTypePolicy object whose method getRequestKind() \n
 *  determines if ChannelRequestDataTypePolicyKind CAFENUM::NATIVE_DATATYPE
 *  or CAFENUM::LOWEST_DATATYPE, \n
 *  which is the smaller in byte size of type requested and native datatype, \n
 *  is being used for get/set operations
 *  \param  _handle input:  reference handle
 *  \param  crdtp   output: ChannelRequestDataTypePolicy object
 *  \return status ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 *
 */
int PolicyHelper::getChannelRequestDataTypePolicy(unsigned int _handle,
        ChannelRequestDataTypePolicy & crdtp)
{
#define __METHOD__ "policyHelper::getChannelRequestDataTypePolicy(handle, ChannelRequestDataTypePolicy & crdtp)"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        crdtp=(*it_handle).getChannelRequestDataTypePolicy();
    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef  __METHOD__
}



/**
 *  \brief  PolicyHelper::setChannelRequestDataTypePolicy \n
 *
 *  Sets ChannelRequestDataTypePolicy object (for all handles) whose \n
 *  method setRequestKind(ChannelRequestDataTypePolicyKind) \n
 *  determines if CAFENUM::NATIVE_DATATYPE or CAFENUM::LOWEST_DATATYPE, \n
 *  which is the smaller in byte size of type requested and native datatype, \n
 *  is being used for get/set operations
 *  \param  crdtp  input: ChannelRequestDataTypePolicy object
 *  \return status ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 *
 */
int PolicyHelper::setChannelRequestDataTypePolicy(ChannelRequestDataTypePolicy crdtp)
{
#define __METHOD__ "policyHelper::setChannelRequestDataTypePolicy(ChannelRequestDataTypePolicy  crdtp)"

    int localStatus=ICAFE_NORMAL;
    int gStatus=ICAFE_NORMAL;
    bool sflag=false;

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        localStatus= setChannelRequestDataTypePolicy ((*itcs).getHandle(), crdtp);
        if (localStatus !=ICAFE_NORMAL && !sflag)
        {
            gStatus=localStatus;
            sflag=true;
        }
    }
    return gStatus;

#undef  __METHOD__
}


/**
 *  \brief PolicyHelper::setChannelRequestDataTypePolicy \n
 *
 *  Sets ChannelRequestDataTypePolicy object whose method \n
 *  setRequestKind(ChannelRequestDataTypePolicyKind) \n
 *  determines if CAFENUM::NATIVE_DATATYPE or CAFENUM::LOWEST_DATATYPE, \n
 *  which is the smaller in byte size of type requested and native datatype, \n
 *  is being used for get/set operations
 *  \param  _handle input:  reference handle
 *  \param  crdtp   input: ChannelRequestDataTypePolicy object
 *  \return status ICAFE_NORMAL else ECAFE_INVALID_HANDLE
 *
 */
int PolicyHelper::setChannelRequestDataTypePolicy(unsigned int _handle,
        ChannelRequestDataTypePolicy crdtp)
{
#define __METHOD__ "policyHelper::setChannelRequestDataTypePolicy(handle, ChannelRequestDataTypePolicy  crdtp)"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        if (crdtp.getRequestKind()!=CAFENUM::NATIVE_DATATYPE &&
                crdtp.getRequestKind()!=CAFENUM::LOWEST_DATATYPE)
        {
            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__  << endl;
            cout << "Handle= "<< (*it_handle).getHandle() << " PV= " << (*it_handle).getPV() << endl;
            cout << crdtp.getRequestKind()  << " is an unknown policy!" << endl;
            cout << "Valid types are: NATIVE_DATATYPE (0) or LOWEST_DATATYPE (1)" << endl;
            return ECAFE_INVALID_CAFENUM_POLICY_TYPE;
        }

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelRequestDataTypePolicy(crdtp));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }

    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef  __METHOD__
}


/**
 *  \brief PolicyHelper::getChannelRequestPolicyGet \n
 *
 *  Retrieves the Callback Policy for get operations
 *
 *  \param  _handle input:  reference handle
 *  \param  crpg   output: ChannelRequestPolicyGet object
 *  \return status ICAFE_NORMAL if all OK else ECAFE_INVALID_HANDLE
 *
 */
int PolicyHelper::getChannelRequestPolicyGet(unsigned int _handle, ChannelRequestPolicy & crpg)
{
#define __METHOD__ "policyHelper::getChannelRequestPolicyGet(unsigned int handle, ChannelRequestPolicy & crpg)"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        crpg=(*it_handle).getChannelRequestPolicyGet();
    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;

#undef  __METHOD__
}

/**
 *  \brief PolicyHelper::setChannelRequestPolicyGet \n
 *
 *  Sets the Blocking Policy for get operations
 *
 *  \param  _handle input:  reference handle
 *  \param  crpg   input: Channel.RequestPolicyGet object
 *  \return status ICAFE_NORMAL if all OK
 *
 */
int PolicyHelper::setChannelRequestPolicyGet(unsigned int _handle, ChannelRequestPolicy crpg)
{
#define __METHOD__ "policyHelper::setChannelRequestPolicyGet(unsigned int handle, ChannelRequestPolicy crpg)"

    int status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        if (crpg.getMethodKind()==WITH_CALLBACK_USER_SUPPLIED && crpg.getHandler()==NULL)
        {
            crpg.setMethodKind(WITH_CALLBACK_DEFAULT);
            status=ICAFE_CHANNEL_BLOCKING_POLICY_CONFLICT;
            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << " Policy Conflict!" << endl;
            cout << "Handle= "<< (*it_handle).getHandle() << " PV= " << (*it_handle).getPV() << endl;
            cout << "Changing ChannelRequestPolicyGet methodKind from WITH_CALLBACK_USER_SUPPLIED " << endl;
            cout << "to WITH_CALLBACK_DEFAULT as user supplied handler is NULL!" << endl;
        }

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelRequestPolicyGet(crpg));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }

    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }

    return status;


#undef  __METHOD__
}



/**
 *  \brief PolicyHelper::setChannelRequestPolicyGet \n
 *
 *  Sets the Blocking Policy for all handles within context
 *
 *  \param  crpg   input: Channel.RequestPolicyGet object
 *  \return status ICAFE_NORMAL if all OK
 *
 */
int PolicyHelper::setChannelRequestPolicyGet(ChannelRequestPolicy crpg)
{
#define __METHOD__ "policyHelper::setChannelRequestPolicyGet(ChannelRequestPolicy crpg)"

    int status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {

        if (crpg.getMethodKind()==WITH_CALLBACK_USER_SUPPLIED && crpg.getHandler()==NULL)
        {
            crpg.setMethodKind(WITH_CALLBACK_DEFAULT);
            status=ICAFE_CHANNEL_BLOCKING_POLICY_CONFLICT;
            cout << "Handle= "<< (*itcs).getHandle() << " PV= " << (*itcs).getPV() << endl;
            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << " Policy Conflict!" << endl;
            cout << "Changing ChannelRequestPolicyGet BlockingKind from WITH_CALLBACK_USER_SUPPLIED " << endl;
            cout << "to WITH_CALLBACK_DEFAULT as user supplied handler is NULL!" << endl;
        }

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(itcs, change_channelRequestPolicyGet(crpg));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }

    }

    return status;

#undef  __METHOD__
}



/**
 *  \brief PolicyHelper::getChannelRequestPolicyPut \n
 *
 *  Retrieves the Blocking Policy for put operations
 *
 *  \param  _handle input:  reference handle
 *  \param  crpp   output: ChannelRequestPolicyPut object
 *  \return status ICAFE_NORMAL if all OK
 *
 */
int PolicyHelper::getChannelRequestPolicyPut(unsigned int _handle, ChannelRequestPolicy & crpp)
{
#define __METHOD__ "PolicyHelper::getChannelRequestPolicyPut(unsigned int handle, ChannelRequestPolicy & crpp)"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        crpp=(*it_handle).getChannelRequestPolicyPut();
    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;


#undef  __METHOD__
}


/**
 *  \brief PolicyHelper::setChannelRequestPolicyPut \n
 *
 *  Sets the Blocking Policy for put operations
 *
 *  \param  _handle input:  reference handle
 *  \param  crpp   input: ChannelRequestPolicyPut object
 *  \return status ICAFE_NORMAL if all OK
 *
 */
int PolicyHelper::setChannelRequestPolicyPut(unsigned int _handle, ChannelRequestPolicy crpp)
{
#define __METHOD__ "PolicyHelper::getChannelRequestPolicyPut(unsigned int handle, ChannelMethodRequesyPolicy crpp)"

    int status=ICAFE_NORMAL;

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        if (crpp.getMethodKind()==WITH_CALLBACK_USER_SUPPLIED && crpp.getHandler()==NULL)
        {
            crpp.setMethodKind(WITH_CALLBACK_DEFAULT);
            status=ICAFE_CHANNEL_BLOCKING_POLICY_CONFLICT;
            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << " Policy Conflict!" << endl;
            cout << "Handle= "<< (*itcs).getHandle() << " PV= " << (*itcs).getPV() << endl;
            cout << "Changing ChannelRequestPolicyPut BlockingKind from WITH_CALLBACK_USER_SUPPLIED " << endl;
            cout << "to WITH_CALLBACK_DEFAULT as user supplied handler is NULL!" << endl;
        }


        if(MUTEX)cafeMutex.lock();
        handle_index.modify(it_handle, change_channelRequestPolicyPut(crpp));
        if(MUTEX)cafeMutex.unlock();

    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }

    return status;


#undef  __METHOD__
}


/**
 *  \brief PolicyHelper::setChannelRequestPolicyPut\n
 *
 *  Sets the Blocking Policy for put operations
 *
 *  \param  crpp   input: ChannelRequestPolicyPut object
 *  \return status ICAFE_NORMAL if all OK
 *
 */
int PolicyHelper::setChannelRequestPolicyPut(ChannelRequestPolicy crpp)
{
#define __METHOD__ "PolicyHelper::getChannelRequestPolicyPut(ChannelMethodRequesyPolicy crpp)"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();

    int status=ICAFE_NORMAL;

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {

        if (crpp.getMethodKind()==WITH_CALLBACK_USER_SUPPLIED && crpp.getHandler()==NULL)
        {
            crpp.setMethodKind(WITH_CALLBACK_DEFAULT);
            status=ICAFE_CHANNEL_BLOCKING_POLICY_CONFLICT;

            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << " Policy Conflict!" << endl;
            cout << "Handle= "<< (*itcs).getHandle() << " PV= " << (*itcs).getPV() << endl;
            cout << "Changing ChannelRequestPolicyPut BlockingKind from WITH_CALLBACK_USER_SUPPLIED " << endl;
            cout << "to WITH_CALLBACK_DEFAULT as user supplied handler is NULL!" << endl;
        }


        if(MUTEX)cafeMutex.lock();
        handle_index.modify(itcs, change_channelRequestPolicyPut(crpp));
        if(MUTEX)cafeMutex.unlock();

    }

    return status;


#undef  __METHOD__
}


////////////////////////////////// TIMEOUTS ///////////////////////////////////////////////////////////

/**
 *  \brief PolicyHelper::getChannelTimeoutPolicyGet \n
 *
 *  Retrieves the Timeout Policy for get operations
 *
 *  \param  _handle input:  reference handle
 *  \param  ctpg   output: ChannelTimeoutPolicyGet object
 *  \return status ICAFE_NORMAL if all OK
 *
 */
int PolicyHelper::getChannelTimeoutPolicyGet(unsigned int _handle, ChannelTimeoutPolicy & ctpg)
{
#define __METHOD__ "PolicyHelper::getChannelTimeoutPolicyGet(unsigned int handle, ChannelTimeoutPolicy & ctpg)"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        ctpg=(*it_handle).getChannelTimeoutPolicyGet();
    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;


#undef  __METHOD__
}


/**
 *  \brief PolicyHelper::setChannelTimeoutPolicyGet \n
 *
 *  Sets the Timeout Policy for put operations
 *
 *  \param  _handle input:  reference handle
 *  \param  ctpg   input: ChannelTimeoutPolicyGet object
 *  \return status ICAFE_NORMAL if all OK
 *
 */
int PolicyHelper::setChannelTimeoutPolicyGet(unsigned int _handle, ChannelTimeoutPolicy ctpg)
{
#define __METHOD__ "PolicyHelper::getChannelTimeoutPolicyGet(unsigned int handle, ChannelTimeoutPolicy ctpg)"


    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelTimeoutPolicyGet(ctpg));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }

    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;


#undef  __METHOD__
}



/**
 *  \brief PolicyHelper::getChannelTimeoutPolicyPut \n
 *
 *  Retrieves the Timeout Policy for put operations
 *
 *  \param  _handle input:  reference handle
 *  \param  ctpp   output: ChannelTimeoutPolicyPut object
 *  \return status ICAFE_NORMAL if all OK
 *
 */
int PolicyHelper::getChannelTimeoutPolicyPut(unsigned int _handle, ChannelTimeoutPolicy & ctpp)
{
#define __METHOD__ "PolicyHelper::getChannelTimeoutPolicyPut(unsigned int handle, ChannelTimeoutPolicy & cptp)"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        ctpp=(*it_handle).getChannelTimeoutPolicyPut();
    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;

#undef  __METHOD__
}


/**
 *  \brief PolicyHelper::setChannelTimeoutPolicyPut \n
 *
 *  Sets the Timout Policy for put operations
 *
 *  \param  _handle input: reference handle
 *  \param  ctpp  input: ChannelTimeoutPolicyPut object
 *  \return status ICAFE_NORMAL if all OK
 *
 */
int PolicyHelper::setChannelTimeoutPolicyPut(unsigned int _handle, ChannelTimeoutPolicy ctpp)
{
#define __METHOD__ "PolicyHelper::setChannelTimeoutPolicyPut(unsigned int handle, ChannelTimeoutPolicy cptp)"

    cafeConduit_set_by_handle & handle_index = cs.get<by_handle> ();
    cafeConduit_set_by_handle::iterator it_handle;

    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelTimeoutPolicyPut(ctpp));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }

    }
    else
    {
        return ECAFE_INVALID_HANDLE;
    }

    return ICAFE_NORMAL;


#undef  __METHOD__
}



/////////////////////////////////////// TIMOUT HELPERS /////////////////////////////////////////////




/**
 *  \brief Allows user to set the policy that governs the corrective adaption of timeouts for put/get operations
 *  for all handles
 *  \param b input: bool
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::setSelfGoverningTimeout(bool b)
{
#define __METHOD__ "PolicyHelper::setSelfGoverningTimout(bool b)"
    int gStatus=ICAFE_NORMAL;
    int localStatus;

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        localStatus=setSelfGoverningTimeout((*itcs).getHandle(),b);
        if (localStatus !=ICAFE_NORMAL)
        {
            gStatus=localStatus;
        }
    }
    return gStatus;
#undef __METHOD__
}



/**
 *  \brief Allows user to set the policy that governs the corrective adaption of timeouts for put/get operations
 *  for a given handle
 *  \param _handle input: Conduit object reference
 *  \param b input: bool
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::setSelfGoverningTimeout(unsigned int _handle, bool b)
{
#define __METHOD__ "PolicyHelper::setSelfGoverningTimeout(unsigned int _handle, bool b)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {

        ChannelTimeoutPolicy ctp=(*it_handle).getChannelTimeoutPolicyPut();
        ChannelTimeoutPolicy ctg=(*it_handle).getChannelTimeoutPolicyGet();

        ctp.setSelfGoverningTimeout(b);
        ctg.setSelfGoverningTimeout(b);


        if(MUTEX)cafeMutex.lock();
        handle_index.modify(it_handle, change_channelTimeoutPolicyPut(ctp));
        handle_index.modify(it_handle, change_channelTimeoutPolicyGet(ctg));
        if(MUTEX)cafeMutex.unlock();

    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef  __METHOD__
}



/**
 *  \brief Allows user to rerieve the policy that governs the corrective adaption of timeouts for put/get operations
 *  for a given handle
 *  \param _handle input: Conduit object reference
 *  \param p output: bool for put operations
 *  \param g output: bool for get operations
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::getSelfGoverningTimeout(unsigned int _handle, bool &p, bool &g)
{
#define __METHOD__ "PolicyHelper::setSelfGoverningTimeout(unsigned int _handle, bool &p, bool &g)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {

        ChannelTimeoutPolicy ctp=(*it_handle).getChannelTimeoutPolicyPut();
        ChannelTimeoutPolicy ctg=(*it_handle).getChannelTimeoutPolicyGet();

        p=ctp.getSelfGoverningTimeout();
        g=ctg.getSelfGoverningTimeout();
    }
    else
    {
        p=false;
        g=false;
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef  __METHOD__
}




/**
 *  \brief Allows user to set the policy that governs the corrective adaption of timeouts for put operations
 *  for a given handle
 *  \param _handle input: Conduit object reference
 *  \param b input: bool
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::setSelfGoverningTimeoutPut(unsigned int _handle, bool b)
{
#define __METHOD__ "PolicyHelper::setSelfGoverningTimeoutPut(unsigned int _handle, bool b)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        ChannelTimeoutPolicy ctp=(*it_handle).getChannelTimeoutPolicyPut();
        ctp.setSelfGoverningTimeout(b);

        if(MUTEX)cafeMutex.lock();
        handle_index.modify(it_handle, change_channelTimeoutPolicyPut(ctp));
        if(MUTEX)cafeMutex.unlock();

    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef  __METHOD__
}


/**
 *  \brief Allows user to set the policy that governs the corrective adaption of timeouts for get operations
 *  for a given handle
 *  \param _handle input: Conduit object reference
 *  \param b input: bool
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::setSelfGoverningTimeoutGet(unsigned int _handle, bool b)
{
#define __METHOD__ "PolicyHelper::setSelfGoverningTimeoutGet(unsigned int _handle, bool b)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);

    if (it_handle != handle_index.end())
    {
        ChannelTimeoutPolicy ctg=(*it_handle).getChannelTimeoutPolicyGet();
        ctg.setSelfGoverningTimeout(b);
        if(MUTEX)cafeMutex.lock();
        handle_index.modify(it_handle, change_channelTimeoutPolicyGet(ctg));
        if(MUTEX)cafeMutex.unlock();
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef  __METHOD__
}



/**
 *  \brief Allows user to retrieve timeout for put/get operations \n
 *  \param _handle input: Conduit object reference
 *  \param p output: put timeout
 *  \param g output: get timeout
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::getTimeout(unsigned int _handle, double & p, double & g)
{
#define __METHOD__ "PolicyHelper::getTimeout(unsigned int _handle, double &p, double &g)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {

        ChannelTimeoutPolicy ctp=(*it_handle).getChannelTimeoutPolicyPut();
        ChannelTimeoutPolicy ctg=(*it_handle).getChannelTimeoutPolicyGet();

        p=ctp.getTimeout();
        g=ctg.getTimeout();
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef __METHOD__
}



/**
 *  \brief Allows user to retrieve minimum timeout for put/get operations
 *  \param p: output: put timeout
 *  \param g: output: get timeout
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::getTimeoutMin(double & p, double & g)
{
#define __METHOD__ "PolicyHelper::getTimeoutMin(double &p, double &g)"

    double a=TIMEOUT_PEND_IO_MAX ;
    double b=TIMEOUT_PEND_IO_MAX ;

    bool isFound=false;

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {

        ChannelTimeoutPolicy ctp=(*itcs).getChannelTimeoutPolicyPut();
        ChannelTimeoutPolicy ctg=(*itcs).getChannelTimeoutPolicyGet();

        a=std::min(a,ctp.getTimeout());
        b=std::min(b,ctg.getTimeout());
        isFound=true;
    }
    if (isFound)
    {
        p=a;
        g=b;
    }
    else
    {
        p=DEFAULT_TIMEOUT_PEND_IO;
        g=DEFAULT_TIMEOUT_PEND_IO;
    }

    return ICAFE_NORMAL;
#undef __METHOD__
}


/**
 *  \brief Allows user to retrieve minimum timeout for put/get operations
 *  \param p: output: put timeout
 *  \param g: output: get timeout
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::getTimeoutMax(double & p, double & g)
{
#define __METHOD__ "PolicyHelper::getTimeoutMin(double &p, double &g)"

    double a=TIMEOUT_PEND_IO_MIN ;
    double b=TIMEOUT_PEND_IO_MIN;

    bool isFound=false;

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {

        ChannelTimeoutPolicy ctp=(*itcs).getChannelTimeoutPolicyPut();
        ChannelTimeoutPolicy ctg=(*itcs).getChannelTimeoutPolicyGet();

        a=std::max(a,ctp.getTimeout());
        b=std::max(b,ctg.getTimeout());
        isFound=true;
    }
    if (isFound)
    {
        p=a;
        g=b;
    }
    else
    {
        p=DEFAULT_TIMEOUT_PEND_IO;
        g=DEFAULT_TIMEOUT_PEND_IO;
    }

    return ICAFE_NORMAL;
#undef __METHOD__
}



/**
 *  \brief Prints timeout for put/get operations
 *  \return ICAFE_NORMAL
 */
int PolicyHelper::printTimeout()
{
#define __METHOD__ "PolicyHelper::printTimeout()"


    short pvstrl=0;
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        pvstrl=std::max(pvstrl,(short) (*itcs).pv.length());
    }

    string spv="Process Variable Name";

    cout << "---------------------TIMEOUTS---------------------" << endl;
    printf("%s %s %*s %s %s\n", "Hndle",spv.c_str(),
           std::max((int) (pvstrl-spv.length()),(int) 0),"","PUTtim","GETtim");
    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {

        ChannelTimeoutPolicy ctp=(*itcs).getChannelTimeoutPolicyPut();
        ChannelTimeoutPolicy ctg=(*itcs).getChannelTimeoutPolicyGet();
        //cout << std::setw(40) << cc.pv <<  std::setw(3)  << cc.getHandle()
        //     << std::setw(10) << ctp.getTimeout()
        //    << std::setw(10)  << ctg.getTimeout() << endl;
        printf("%5d %s %*s %6.3f %6.3f\n",  (*itcs).getHandle(),(*itcs).pv.c_str(),
               std::max((int) (pvstrl-(*itcs).pv.length()),(int) 0),"",
               ctp.getTimeout(), ctg.getTimeout() ) ;
    }
    cout << "--------------------------------------------------" << endl;
    return ICAFE_NORMAL;
#undef __METHOD__
}


/**
 *  \brief Prints timeout for put/get operations for given handle
 *  \param _handle input: handle to Conduit Object
 *  \return ICAFE_NORMAL
 */
int PolicyHelper::printTimeout(unsigned int _handle)
{
#define __METHOD__ "PolicyHelper::printTimeout(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {
        //Conduit cc=(*it_handle);
        ChannelTimeoutPolicy ctp=(*it_handle).getChannelTimeoutPolicyPut();
        ChannelTimeoutPolicy ctg=(*it_handle).getChannelTimeoutPolicyGet();

        string spv="Process Variable Name";
        int wantedl= std::max(28,(int) (*it_handle).pv.length());
        printf("%s %s %*s %s %s\n", "Handle",spv.c_str(),
               std::max((int) (wantedl-spv.length()),(int) 0),"","PUTtim","GETtim");
        printf("%5d %s %*s %6.3f %6.3f\n", (*it_handle).getHandle(),(*it_handle).pv.c_str(),
               std::max((int)(wantedl-(*it_handle).pv.length()),(int) 0),"",
               ctp.getTimeout(),ctg.getTimeout());

    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;

#undef __METHOD__
}




/**
 *  \brief Allows user to reset timeout for put/get operations to default values
 *  for all handles
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::setTimeoutToDefault()
{
#define __METHOD__ "PolicyHelper::setTimeoutToDefault()"

    int gStatus=ICAFE_NORMAL;
    int localStatus;

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        localStatus=setTimeoutToDefault((*itcs).getHandle());
        if (localStatus !=ICAFE_NORMAL)
        {
            gStatus=localStatus;
        }
    }
    return gStatus;
#undef __METHOD__
}



/**
 *  \brief Allows user to reset timeout for put/get operations to default values
 *   for given handle
 *  \param  _handle input: Conduit object reference
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::setTimeoutToDefault(unsigned int _handle)
{
#define __METHOD__ "PolicyHelper::setTimeoutToDefault(unsigned int _handle)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {

        ChannelTimeoutPolicy ctp=(*it_handle).getChannelTimeoutPolicyPut();
        ChannelTimeoutPolicy ctg=(*it_handle).getChannelTimeoutPolicyGet();

        ctp.setTimeoutToDefault();
        ctg.setTimeoutToDefault();

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelTimeoutPolicyPut(ctp));
        handle_index.modify(it_handle, change_channelTimeoutPolicyGet(ctg));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef __METHOD__
}




/**
 *  \brief Allows user to set timeout for put/get operations (within allowed default min/max values)
 *  for all handles
 *  \param p input: put timeout
 *  \param g input: get timeout
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::setTimeout(double p, double g)
{
#define __METHOD__ "PolicyHelper::setTimeout(double p, double g)"

    int gStatus=ICAFE_NORMAL;
    int localStatus;

    // Loop through all elements
    for (itcs = cs.begin(); itcs != cs.end(); ++itcs)
    {
        localStatus=setTimeout((*itcs).getHandle(),p,g);
        if (localStatus !=ICAFE_NORMAL)
        {
            gStatus=localStatus;
        }
    }
    return gStatus;
#undef __METHOD__
}



/**
 *  \brief Allows user to set timeoout for put/get operations (within allowed default min/max values)
 *  \param  _handle input: Conduit object reference
 *  \param p input: put timeout
 *  \param g input: get timeout
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::setTimeout(unsigned int _handle, double p, double g)
{
#define __METHOD__ "PolicyHelper::setTimeout(unsigned int _handle, double p, double g)"
    cafeConduit_set_by_handle & handle_index=cs.get<by_handle>();
    cafeConduit_set_by_handle::iterator it_handle;
    it_handle = handle_index.find(_handle);
    if (it_handle != handle_index.end())
    {

        ChannelTimeoutPolicy ctp=(*it_handle).getChannelTimeoutPolicyPut();
        ChannelTimeoutPolicy ctg=(*it_handle).getChannelTimeoutPolicyGet();

        double a=(double) NULL, b= (double) NULL;
        if (p!=(double) NULL)
        {
            a=ctp.setTimeout(p);
        }
        if (g!=(double) NULL)
        {
            b=ctg.setTimeout(g);
        }

        if (a != p || b != g)
        {
            cout << "------------------------------------------------------" << endl;
            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
            cout << "PV = " << (*it_handle).pv << " Handle = " << (*it_handle).handle;
            cout << ": Requested timeout is not within allowed limits" << endl;

            if (a != p)
            {
                cout << "Timeout Granted (Requested) for put operation = "
                     << a << " (" << p <<")" <<endl;
            }
            if (b != g)
            {
                cout << "Timeout Granted (Requested) for get operation = "
                     << b << " (" << g <<")" <<endl;
            }
            cout << "------------------------------------------------------" << endl;
        }
        if(MUTEX)
        {
            cafeMutex.lock();
        }
        handle_index.modify(it_handle, change_channelTimeoutPolicyPut(ctp));
        handle_index.modify(it_handle, change_channelTimeoutPolicyGet(ctg));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef __METHOD__
}




/**
 *  \brief Allows user to set the policy that governs the corrective adaption of timeouts for SG put/get operations
 *  for all group handles
 *  \param b input: bool
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::setSGSelfGoverningTimeout(bool b)
{
#define __METHOD__ "PolicyHelper::setSGSelfGoverningTimeout(double b)"
    int gStatus=ICAFE_NORMAL;
    int localStatus;

    // Loop through all elements
    for (itgs = gs.begin(); itgs != gs.end(); ++itgs)
    {
        localStatus=setSGSelfGoverningTimeout((*itgs).getGroupHandle(),b);
        if (localStatus !=ICAFE_NORMAL)
        {
            gStatus=localStatus;
        }
    }
    return gStatus;
#undef __METHOD__
}


/**
 *  \brief Allows user to set the policy that governs the corrective adaption of timeouts for SG put/get operations
 *  for a given handle
 *  \param _gHandle input: Conduit Group object reference
 *  \param b input: bool
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::setSGSelfGoverningTimeout(unsigned int _gHandle, bool b)
{
#define __METHOD__ "PolicyHelper::setSelfGoverningTimeout(unsigned int _handle, bool b)"
    cafeGroup_set_by_groupHandle & groupHandle_index=gs.get<by_groupHandle>();
    cafeGroup_set_by_groupHandle::iterator it_groupHandle;
    it_groupHandle = groupHandle_index.find(_gHandle);
    if (it_groupHandle != groupHandle_index.end())
    {
        ChannelTimeoutPolicy ctp=(*it_groupHandle).getChannelTimeoutPolicySGPut();
        ChannelTimeoutPolicy ctg=(*it_groupHandle).getChannelTimeoutPolicySGGet();

        ctp.setSelfGoverningTimeout(b);
        ctg.setSelfGoverningTimeout(b);
        if(MUTEX)cafeMutex.lock();
        groupHandle_index.modify(it_groupHandle, change_channelTimeoutPolicySGPut(ctp));
        groupHandle_index.modify(it_groupHandle, change_channelTimeoutPolicySGGet(ctg));
        if(MUTEX)cafeMutex.unlock();
        //cout << __METHOD__ << " Setting group Handle " << _gHandle << " SG sef-gov policy to " << b << endl;


    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_GROUP_HANDLE);
        return ECAFE_INVALID_GROUP_HANDLE;
    }
    return ICAFE_NORMAL;

#undef  __METHOD__
}



/**
 *  \brief Allows user to retrieve the policy that governs the corrective adaption of timeouts for put/get operations
 *  for a given handle
 *  \param _gHandle input: Conduit Group object reference
 *  \param p output: bool for put operations
 *  \param g output: bool for get operations
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::getSGSelfGoverningTimeout(unsigned int _gHandle, bool &p, bool &g)
{
#define __METHOD__ "PolicyHelper::getSGSelfGoverningTimeout(unsigned int _gHandle, bool &p, bool &g)"
    cafeGroup_set_by_groupHandle & groupHandle_index=gs.get<by_groupHandle>();
    cafeGroup_set_by_groupHandle::iterator it_groupHandle;
    it_groupHandle = groupHandle_index.find(_gHandle);
    if (it_groupHandle != groupHandle_index.end())
    {

        ChannelTimeoutPolicy ctp=(*it_groupHandle).getChannelTimeoutPolicySGPut();
        ChannelTimeoutPolicy ctg=(*it_groupHandle).getChannelTimeoutPolicySGGet();

        p=ctp.getSelfGoverningTimeout();
        g=ctg.getSelfGoverningTimeout();
    }
    else
    {
        p=false;
        g=false;
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;
#undef  __METHOD__
}




/**
 *  \brief Allows user to set the policy that governs the corrective adaption of timeouts for SG put operations
 *  for a given group handle
 *  \param _gHandle input: Conduit Group object reference
 *  \param b input: bool
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::setSGSelfGoverningTimeoutPut(unsigned int _gHandle, bool b)
{
#define __METHOD__ "PolicyHelper::setSGSelfGoverningTimeoutPut(unsigned int _ghandle, bool b)"

    cafeGroup_set_by_groupHandle & groupHandle_index=gs.get<by_groupHandle>();
    cafeGroup_set_by_groupHandle::iterator it_groupHandle;
    it_groupHandle = groupHandle_index.find(_gHandle);
    if (it_groupHandle != groupHandle_index.end())
    {

        ChannelTimeoutPolicy ctp=(*it_groupHandle).getChannelTimeoutPolicySGPut();

        ctp.setSelfGoverningTimeout(b);
        if(MUTEX)cafeMutex.lock();
        groupHandle_index.modify(it_groupHandle, change_channelTimeoutPolicySGPut(ctp));
        if(MUTEX)cafeMutex.unlock();

    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_GROUP_HANDLE);
        return ECAFE_INVALID_GROUP_HANDLE;
    }
    return ICAFE_NORMAL;

#undef  __METHOD__
}


/**
 *  \brief Allows user to set the policy that governs the corrective adaption of timeouts for SG get operations
 *  for a given group handle
 *  \param _gHandle input: Conduit Group object reference
 *  \param b input: bool
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::setSGSelfGoverningTimeoutGet(unsigned int _gHandle, bool b)
{
#define __METHOD__ "PolicyHelper::setSGSelfGoverningTimeoutGet(unsigned int _gHandle, bool b)"

    cafeGroup_set_by_groupHandle & groupHandle_index=gs.get<by_groupHandle>();
    cafeGroup_set_by_groupHandle::iterator it_groupHandle;
    it_groupHandle = groupHandle_index.find(_gHandle);
    if (it_groupHandle != groupHandle_index.end())
    {

        ChannelTimeoutPolicy ctg=(*it_groupHandle).getChannelTimeoutPolicySGGet();

        ctg.setSelfGoverningTimeout(b);

        if(MUTEX)cafeMutex.lock();
        groupHandle_index.modify(it_groupHandle, change_channelTimeoutPolicySGGet(ctg));
        if(MUTEX)cafeMutex.unlock();
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_GROUP_HANDLE);
        return ECAFE_INVALID_GROUP_HANDLE;
    }
    return ICAFE_NORMAL;

#undef  __METHOD__
}


/**
 *  \brief Allows user to retrieve minimum timeout for SG put/get operations
 *  \param p: output: SG put timeout
 *  \param g: output: SG get timeout
 *  \return ICAFE_NORMAL
 */
int PolicyHelper::getSGTimeoutMin(double & p, double & g)
{
#define __METHOD__ "PolicyHelper::getSGTimeoutMin(double &p, double &g)"

    double a=TIMEOUT_PEND_IO_MAX ;
    double b=TIMEOUT_PEND_IO_MAX ;
    bool isFound=false;
    // Loop through all elements
    for (itgs = gs.begin(); itgs != gs.end(); ++itgs)
    {

        ChannelTimeoutPolicy ctp=(*itgs).getChannelTimeoutPolicySGPut();
        ChannelTimeoutPolicy ctg=(*itgs).getChannelTimeoutPolicySGGet();

        a=std::min(a,ctp.getTimeout());
        b=std::min(b,ctg.getTimeout());

        isFound=true;
    }
    if (isFound)
    {
        p=a;
        g=b;
    }
    else
    {
        p=DEFAULT_TIMEOUT_SG_PEND_IO;
        g=DEFAULT_TIMEOUT_SG_PEND_IO;
    }

    return ICAFE_NORMAL;
#undef __METHOD__
}


/**
 *  \brief Allows user to retrieve minimum timeout for put/get operations
 *  \param p: output: SG put timeout
 *  \param g: output: SG get timeout
 *  \return ICAFE_NORMAL
 */
int PolicyHelper::getSGTimeoutMax(double & p, double & g)
{
#define __METHOD__ "PolicyHelper::getSGTimeoutMin(double &p, double &g)"

    double a=TIMEOUT_PEND_IO_MIN ;
    double b=TIMEOUT_PEND_IO_MIN;
    bool isFound=false;
    // Loop through all elements
    for (itgs = gs.begin(); itgs != gs.end(); ++itgs)
    {

        ChannelTimeoutPolicy ctp=(*itgs).getChannelTimeoutPolicySGPut();
        ChannelTimeoutPolicy ctg=(*itgs).getChannelTimeoutPolicySGGet();

        a=std::max(a,ctp.getTimeout());
        b=std::max(b,ctg.getTimeout());

        isFound=true;
    }
    if (isFound)
    {
        p=a;
        g=b;
    }
    else
    {
        p=DEFAULT_TIMEOUT_SG_PEND_IO;
        g=DEFAULT_TIMEOUT_SG_PEND_IO;
    }

    return ICAFE_NORMAL;
#undef __METHOD__
}




/**
 *  \brief Allows user to reset synchronous group timeout for put/get operations to default values
 *  for all group handles
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::setSGTimeoutToDefault()
{
#define __METHOD__ "PolicyHelper::setSGTimeoutToDefault()"

    int gStatus=ICAFE_NORMAL;
    int localStatus;

    // Loop through all elements
    for (itgs = gs.begin(); itgs != gs.end(); ++itgs)
    {
        localStatus=setSGTimeoutToDefault((*itgs).getGroupHandle());
        if (localStatus !=ICAFE_NORMAL)
        {
            gStatus=localStatus;
        }
    }
    return gStatus;
#undef __METHOD__
}



/**
 *  \brief Allows user to reset synchronous group  timeout for put/get operations to default values
 *   for given handle
 *  \param  _gHandle input: ConduitGroup object reference
 *  \return ICAFE_NORMAL or ECAFE_INVALID_HANDLE
 */
int PolicyHelper::setSGTimeoutToDefault(unsigned int _gHandle)
{
#define __METHOD__ "PolicyHelper::setSGTimeoutToDefault(unsigned int _gHandle)"

    cafeGroup_set_by_groupHandle & groupHandle_index=gs.get<by_groupHandle>();
    cafeGroup_set_by_groupHandle::iterator it_groupHandle;
    it_groupHandle = groupHandle_index.find(_gHandle);
    if (it_groupHandle != groupHandle_index.end())
    {

        ChannelTimeoutPolicy ctp=(*it_groupHandle).getChannelTimeoutPolicySGPut();
        ChannelTimeoutPolicy ctg=(*it_groupHandle).getChannelTimeoutPolicySGGet();

        ctp.setTimeoutToDefault();
        ctg.setTimeoutToDefault();

        if(MUTEX)
        {
            cafeMutex.lock();
        }
        groupHandle_index.modify(it_groupHandle, change_channelTimeoutPolicySGPut(ctp));
        groupHandle_index.modify(it_groupHandle, change_channelTimeoutPolicySGGet(ctg));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_GROUP_HANDLE);
        return ECAFE_INVALID_GROUP_HANDLE;
    }
    return ICAFE_NORMAL;
#undef __METHOD__
}







/**
 *  \brief Prints timeout for SG put/get operations
 *  \return ICAFE_NORMAL
 */
int PolicyHelper::printSGTimeout()
{
#define __METHOD__ "PolicyHelper::printSGTimeout()"


    short pvstrl=0;
    for (itgs = gs.begin(); itgs != gs.end(); ++itgs)
    {
        pvstrl=std::max(pvstrl,(short) (*itgs).groupName.length());
    }

    string spv="Group";

    cout << "---------------------TIMEOUTS---------------------" << endl;
    printf("%s %s %*s %s %s\n", "Hndle",spv.c_str(),
           std::max((int) (pvstrl-spv.length()),(int) 0),"","PUTtim","GETtim");
    // Loop through all elements
    for (itgs = gs.begin(); itgs != gs.end(); ++itgs)
    {

        ChannelTimeoutPolicy ctp=(*itgs).getChannelTimeoutPolicySGPut();
        ChannelTimeoutPolicy ctg=(*itgs).getChannelTimeoutPolicySGGet();

        printf("%5d %s %*s %6.3f %6.3f\n",  (*itgs).getGroupHandle(),(*itgs).groupName.c_str(),
               std::max((int) (pvstrl-(*itgs).groupName.length()),(int) 0),"",
               ctp.getTimeout(), ctg.getTimeout() ) ;
    }
    cout << "--------------------------------------------------" << endl;
    return ICAFE_NORMAL;
#undef __METHOD__
}


/**
 *  \brief Prints timeout for put/get operations for given handle
 *  \param _gHandle input: handle to ConduitGroup Object
 *  \return ICAFE_NORMAL
 */
int PolicyHelper::printSGTimeout(unsigned int _gHandle)
{
#define __METHOD__ "PolicyHelper::printTimeout(unsigned int _handle)"
    cafeGroup_set_by_groupHandle & groupHandle_index=gs.get<by_groupHandle>();
    cafeGroup_set_by_groupHandle::iterator it_groupHandle;
    it_groupHandle = groupHandle_index.find(_gHandle);
    if (it_groupHandle != groupHandle_index.end())
    {
        //Conduit cc=(*it_handle);
        ChannelTimeoutPolicy ctp=(*it_groupHandle).getChannelTimeoutPolicySGPut();
        ChannelTimeoutPolicy ctg=(*it_groupHandle).getChannelTimeoutPolicySGGet();

        string spv="Group";
        int wantedl= std::max(28,(int) (*it_groupHandle).groupName.length());
        printf("%s %s %*s %s %s\n", "Handle",spv.c_str(),
               std::max((int) (wantedl-spv.length()),(int) 0),"","PUTtim","GETtim");
        printf("%5d %s %*s %6.3f %6.3f\n", (*it_groupHandle).getGroupHandle(),(*it_groupHandle).groupName.c_str(),
               std::max((int)(wantedl-(*it_groupHandle).groupName.length()),(int) 0),"",
               ctp.getTimeout(),ctg.getTimeout());

    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_HANDLE);
        return ECAFE_INVALID_HANDLE;
    }
    return ICAFE_NORMAL;

#undef __METHOD__
}







/**
 *  \brief Allows user to retrieve timeout for SG put/get operations \n
 *  \param _gHandle input: Conduit Group object reference
 *  \param p output: SG put timeout
 *  \param g output: SG get timeout
 *  \return ICAFE_NORMAL or ECAFE_INVALID_GROUP_HANDLE
 */
int PolicyHelper::getSGTimeout(unsigned int _gHandle, double & p, double & g)
{
#define __METHOD__ "PolicyHelper::getSGTimeout(unsigned int _gHandle, double &p, double &g)"
    cafeGroup_set_by_groupHandle & groupHandle_index=gs.get<by_groupHandle>();
    cafeGroup_set_by_groupHandle::iterator it_groupHandle;
    it_groupHandle = groupHandle_index.find(_gHandle);
    if (it_groupHandle != groupHandle_index.end())
    {

        ChannelTimeoutPolicy ctp=(*it_groupHandle).getChannelTimeoutPolicySGPut();
        ChannelTimeoutPolicy ctg=(*it_groupHandle).getChannelTimeoutPolicySGGet();

        p=ctp.getTimeout();
        g=ctg.getTimeout();
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_GROUP_HANDLE);
        return ECAFE_INVALID_GROUP_HANDLE;
    }
    return ICAFE_NORMAL;
#undef __METHOD__
}


/**
 *  \brief Allows user to set timeout for SG put/get operations (within allowed default min/max values)
 *  for all handles
 *  \param p input: SG put timeout
 *  \param g input: SG get timeout
 *  \return ICAFE_NORMAL or ECAFE_INVALID_GROUP_HANDLE
 */
int PolicyHelper::setSGTimeout(double p, double g)
{
#define __METHOD__ "PolicyHelper::setSGTimeout(double p, double g)"

    int gStatus=ICAFE_NORMAL;
    int localStatus;

    // Loop through all elements
    for (itgs = gs.begin(); itgs != gs.end(); ++itgs)
    {
        localStatus=setSGTimeout((*itgs).getGroupHandle(),p,g);
        if (localStatus !=ICAFE_NORMAL)
        {
            gStatus=localStatus;
        }
    }
    return gStatus;
#undef __METHOD__
}

/**
 *  \brief Allows user to set timeoout for SG put/get operations (within allowed default min/max values)
 *  \param  _gHandle input: Conduit Group object reference
 *  \param p input: SG put timeout
 *  \param g input: SG get timeout
 *  \return ICAFE_NORMAL or ECAFE_INVALID_GROUP_HANDLE
 */
int PolicyHelper::setSGTimeout(unsigned int _gHandle, double p, double g)
{
#define __METHOD__ "PolicyHelper::setSGTimeout(unsigned int _handle, double p, double g)"
    cafeGroup_set_by_groupHandle & groupHandle_index=gs.get<by_groupHandle>();
    cafeGroup_set_by_groupHandle::iterator it_groupHandle;
    it_groupHandle = groupHandle_index.find(_gHandle);
    if (it_groupHandle != groupHandle_index.end())
    {

        ChannelTimeoutPolicy ctp=(*it_groupHandle).getChannelTimeoutPolicySGPut();
        ChannelTimeoutPolicy ctg=(*it_groupHandle).getChannelTimeoutPolicySGGet();

        double a=(double) NULL, b= (double) NULL;
        if (p!=(double) NULL)
        {
            a=ctp.setTimeout(p);
        }
        if (g!=(double) NULL)
        {
            b=ctg.setTimeout(g);
        }

        if (a != p || b != g)
        {
            cout << "------------------------------------------------------" << endl;
            cout << __FILE__ << "/" << __LINE__ << "/" << __METHOD__ << endl;
            cout << "Synch. Group = " << (*it_groupHandle).groupName << " Handle = " << (*it_groupHandle).groupHandle;
            cout << ": Requested timeout is not within allowed limits" << endl;

            if (a != p)
            {
                cout << "Timeout Granted (Requested) for put operation = "
                     << a << " (" << p <<")" <<endl;
            }
            if (b != g)
            {
                cout << "Timeout Granted (Requested) for get operation = "
                     << b << " (" << g <<")" <<endl;
            }
            cout << "------------------------------------------------------" << endl;
        }
        if(MUTEX)
        {
            cafeMutex.lock();
        }
        groupHandle_index.modify(it_groupHandle, change_channelTimeoutPolicySGPut(ctp));
        groupHandle_index.modify(it_groupHandle, change_channelTimeoutPolicySGGet(ctg));
        if(MUTEX)
        {
            cafeMutex.unlock();
        }
    }
    else
    {
        cafeStatus.report(ECAFE_INVALID_GROUP_HANDLE);
        return ECAFE_INVALID_GROUP_HANDLE;
    }
    return ICAFE_NORMAL;
#undef __METHOD__
}


