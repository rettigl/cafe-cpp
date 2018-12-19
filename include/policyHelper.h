///
/// \file    policyHelper.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef POLICYHELPER_H
#define POLICYHELPER_H

#include <cadef.h>
#include <helper.h>
#include <defines.h>
#include <hashConduit.h>
#include <hashConduitGroup.h>
#include <conduitFriends.h>

class PolicyHelper : Helper {
private:
    CAFEStatus cafeStatus;
    cafeConduit_set::iterator itcs;
    cafeGroup_set::iterator itgs;
public:
    PolicyHelper() {};
    ~PolicyHelper() {};

    int  getChannelGetActionWhenMonitorPolicy(unsigned int _handle, ChannelGetActionWhenMonitorPolicy & awmp);
    int  setChannelGetActionWhenMonitorPolicy(unsigned int _handle, ChannelGetActionWhenMonitorPolicy awmp);
    int  setChannelGetActionWhenMonitorPolicy(ChannelGetActionWhenMonitorPolicy awmp);
    int  setChannelGetActionWhenMonitorPolicyAllHandles(ChannelGetActionWhenMonitorPolicy awmp)
    {
        return  setChannelGetActionWhenMonitorPolicy(awmp);
    }

    int  getChannelGetCacheWaitPolicy(unsigned int  _handle, ChannelGetCacheWaitPolicy & cwp);
    int  setChannelGetCacheWaitPolicy(unsigned int  _handle, ChannelGetCacheWaitPolicy  cwp);
    int  setChannelGetCacheWaitPolicy(ChannelGetCacheWaitPolicy  cwp);
    int  setChannelGetCacheWaitPolicyAllHandles(ChannelGetCacheWaitPolicy  cwp)
    {
        return  setChannelGetCacheWaitPolicy(cwp);
    }

    int  getChannelRequestDataTypePolicy(unsigned int  _handle, ChannelRequestDataTypePolicy &crdtp);
    int  setChannelRequestDataTypePolicy(unsigned int  _handle, ChannelRequestDataTypePolicy crdtp);
    int  setChannelRequestDataTypePolicy(ChannelRequestDataTypePolicy crdtp);
    int  setChannelRequestDataTypePolicyAllHandles(ChannelRequestDataTypePolicy crdtp)
    {
        return  setChannelRequestDataTypePolicy(crdtp);
    }

    int  getChannelRequestPolicyGet (unsigned int  _handle, ChannelRequestPolicy & crpg);
    int  setChannelRequestPolicyGet (unsigned int  _handle, ChannelRequestPolicy crpg);
    int  setChannelRequestPolicyGet (ChannelRequestPolicy crpg);
    int  setChannelRequestPolicyGetAllHandles (ChannelRequestPolicy crpg)
    {
        return setChannelRequestPolicyGet (crpg);
    }

    int  getChannelRequestPolicyPut (unsigned int  _handle, ChannelRequestPolicy & crpp);
    int  setChannelRequestPolicyPut (unsigned int  _handle, ChannelRequestPolicy crpp);
    int  setChannelRequestPolicyPut (ChannelRequestPolicy crpp);
    int  setChannelRequestPolicyPutAllHandles (ChannelRequestPolicy crpp)
    {
        return setChannelRequestPolicyPut (crpp);
    }

    int  getChannelTimeoutPolicyGet (unsigned int  _handle, ChannelTimeoutPolicy & ctpg);
    int  setChannelTimeoutPolicyGet (unsigned int  _handle, ChannelTimeoutPolicy ctpg);

    int  getChannelTimeoutPolicyPut (unsigned int  _handle, ChannelTimeoutPolicy & ctpp);
    int  setChannelTimeoutPolicyPut (unsigned int  _handle, ChannelTimeoutPolicy ctpp);


    int  setSelfGoverningTimeout(bool b);
    int  setSelfGoverningTimeout(unsigned int _handle, bool b);
    int  setSelfGoverningTimeoutPut(unsigned int _handle, bool p);
    int  setSelfGoverningTimeoutGet(unsigned int _handle, bool g);
    int  getSelfGoverningTimeout(unsigned int _handle, bool &p, bool &g);

    int  setSGSelfGoverningTimeout(bool b);
    int  setSGSelfGoverningTimeout(unsigned int _handle, bool b);
    int  setSGSelfGoverningTimeoutPut(unsigned int _handle, bool p);
    int  setSGSelfGoverningTimeoutGet(unsigned int _handle, bool g);
    int  getSGSelfGoverningTimeout(unsigned int _handle, bool &p, bool &g);

    //Short cuts
    //put, get, put, get
    int  getTimeoutRange (double &p, double &p2, double &g, double &g2)
    {
        int  stp;
        int  stg;
        stp=getTimeoutMin(p,g);
        stg=getTimeoutMax(p2, g2);
        if (stp!=ICAFE_NORMAL) {
            return stp;
        }
        else if(stg!=ICAFE_NORMAL) {
            return stg;
        }
        else {
            return ICAFE_NORMAL;
        }
    };

    int  getTimeoutMin (double &p, double &g);
    int  getTimeoutMax (double &p, double &g);

    int  getTimeout (unsigned int  _handle, double &p, double &g);
    //returns lesser of the two
    int  getTimeout (unsigned int  _handle, double &pg)
    {
        double p;
        double g;
        int  st;
        st=getTimeout(_handle, p, g);
        pg=std::min(p,g);
        return st;
    }
    int  getTimeoutPut(unsigned int  _handle, double &p)
    {
        double g;
        return getTimeout(_handle, p, g);
    }
    int  getTimeoutGet(unsigned int  _handle, double &g)
    {
        double p;
        return getTimeout(_handle, p, g);
    }

    int  printTimeout ();
    int  printTimeout (unsigned int  _handle);

    int  setTimeout   (double p, double g); // for all handles put and get separately
    int  setTimeout   (double pg)
    {
        return setTimeout(pg, pg);
    };
    int  setTimeoutPut(double p)
    {
        return setTimeout(p, (double) NULL);
    };
    int  setTimeoutGet(double g)
    {
        return setTimeout((double) NULL, g);
    };

    int  setTimeout   (unsigned int  _handle, double p, double g);
    int  setTimeout   (unsigned int  _handle, double pg)
    {
        return setTimeout(_handle, pg, pg);
    };
    int  setTimeoutPut(unsigned int  _handle, double p)
    {
        return setTimeout(_handle, p, (double) NULL);
    };
    int  setTimeoutGet(unsigned int  _handle, double g)
    {
        return setTimeout(_handle, (double) NULL, g);
    };
    int  setTimeoutToDefault(unsigned int  _handle);
    int  setTimeoutToDefault();

    //get SGTimeout
    int  getSGTimeoutRange (double &p, double &p2, double &g, double &g2)
    {
        int  stp;
        int  stg;
        stp=getSGTimeoutMin(p,g);
        stg=getSGTimeoutMax(p2, g2);
        if (stp!=ICAFE_NORMAL) {
            return stp;
        }
        else if(stg!=ICAFE_NORMAL) {
            return stg;
        }
        else {
            return ICAFE_NORMAL;
        }
    };

    int  getSGTimeoutMin (double &p, double &g);
    int  getSGTimeoutMax (double &p, double &g);
    int  printSGTimeout ();
    int  printSGTimeout (unsigned int  _handle);


    int  getSGTimeout (unsigned int  _handle, double &p, double &g);
    //returns lesser of the two
    int  getSGTimeout (unsigned int  _handle, double &pg)
    {
        double p;
        double g;
        int  st;
        st=getSGTimeout(_handle, p, g);
        pg=std::min(p,g);
        return st;
    }
    int  getSGTimeoutPut(unsigned int  _handle, double &p)
    {
        double g;
        return getSGTimeout(_handle, p, g);
    }
    int  getSGTimeoutGet(unsigned int  _handle, double &g)
    {
        double p;
        return getSGTimeout(_handle, p, g);
    }


    //set SGTimeout
    int  setSGTimeout   (double p, double g); // for all group handles put and get separately
    int  setSGTimeout   (double pg)
    {
        return setSGTimeout(pg, pg);
    };
    int  setSGTimeoutPut(double p)
    {
        return setSGTimeout(p, (double) NULL);
    };
    int  setSGTimeoutGet(double g)
    {
        return setSGTimeout((double) NULL, g);
    };
    int  setSGTimeout   (unsigned int  _gHandle, double p, double g);
    int  setSGTimeout   (unsigned int  _gHandle, double pg)
    {
        return setSGTimeout(_gHandle, pg, pg);
    };
    int  setSGTimeoutPut(unsigned int  _gHandle, double p)
    {
        return setSGTimeout(_gHandle, p, (double) NULL);
    };
    int  setSGTimeoutGet(unsigned int  _gHandle, double g)
    {
        return setSGTimeout(_gHandle, (double) NULL, g);
    };

    int  setSGTimeoutToDefault(unsigned int  _handle);
    int  setSGTimeoutToDefault();


};
#endif
