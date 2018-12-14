///
/// \file    conduitGroup.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef CONDUITGROUP_H
#define CONDUITGROUP_H

#include <iostream>
#include <cadef.h>

#include <hashConduit.h>
#include <statusCodes.h>


/**
 *   Principal constructor has:\n
 *   \param _groupName      assigned group name
 *   \param _ccc          	ca_client_context
 *   \param _groupID     	unique group identifier (CA_SYNC_GID)
 *   \param _nMember		number of members in group
 *   \param _memberCC       CAFEConduit object representing each group member
 *   \param _groupStatus    groupStatus: error indicates >0 members have an error
 */
class ConduitGroup {
    friend struct change_channelTimeoutPolicySGPut;
    friend struct change_channelTimeoutPolicySGGet;
    friend struct change_timeout_sg_pend_io;
    friend struct change_sg_rule;
    friend class Connect;

private:
    static unsigned int  groupHandleNext;
    unsigned int         nMember;
    ca_client_context *  ccc;

    double  timeout_sg_pend_io;

    ChannelTimeoutPolicy channelTimeoutPolicySGGet;
    ChannelTimeoutPolicy channelTimeoutPolicySGPut;

    union   db_access_val ** dataBuffer;

    friend std::ostream& operator<<(std::ostream& os, const ConduitGroup& e)
    {
        os<< "handle="  << e.groupHandle<<" name=" << e.groupName<< " nMember=" << e.nMember << std::endl;
        return os;
    };

public:
    std::string groupName;
    unsigned int  groupHandle;
    CA_SYNC_GID   groupID;

    int     * mStatus;
    bool    * mRule;

    unsigned int  * mHandle;


    virtual ~ConduitGroup();

    ConduitGroup();
    ConduitGroup(const char * _groupName, ca_client_context * _ccc,  CA_SYNC_GID _groupID,
                 unsigned int  _nMember,   unsigned int  * _handle);

    int  get(void) const;
    int  put(void) const;

    ca_client_context * getClientContext(void) const
    {
        return ccc;
    };
    const char *        getGroupName(void) const
    {
        return groupName.c_str();
    };
    std::string         getGroupNameAsString(void) const
    {
        return groupName;
    };
    CA_SYNC_GID         getGroupID(void) const
    {
        return groupID;
    };
    unsigned int        getNMember(void) const
    {
        return nMember;
    };
    int  *              getStatus(void) const
    {
        return mStatus;
    };
    bool *              getRule(void) const
    {
        return mRule;
    };
    bool                getRule(unsigned int  i) const
    {
        return mRule[i];
    };
    unsigned int        getGroupHandle(void) const
    {
        return groupHandle;
    };
    ChannelTimeoutPolicy getChannelTimeoutPolicySGPut(void) const
    {
        return channelTimeoutPolicySGPut;
    };
    ChannelTimeoutPolicy getChannelTimeoutPolicySGGet(void) const
    {
        return channelTimeoutPolicySGGet;
    };

};



/**
 *   Friend to ConduitGroup permitting fast modification of the rule in group hash table;
 *   the rule flag only functions within a PVGroup
 */
struct change_sg_rule {
    change_sg_rule (const bool & new_rule, const unsigned int  & iMember):
        new_rule(new_rule),iMember(iMember) {}

    void operator() (ConduitGroup& g)
    {
        g.mRule[iMember] =  new_rule;
    }

private:
    bool new_rule;
    unsigned int  iMember;
};


/**
 *   Friend to ConduitGroup permitting fast modification of status in group hash table;
 */
struct change_sg_status {
    change_sg_status (const int  & new_status, const unsigned int  & iMember):
        new_status(new_status),iMember(iMember) {}

    void operator() (ConduitGroup& g)
    {
        g.mStatus[iMember] =  new_status;
    }

private:
    int  new_status;
    unsigned int  iMember;
};

/**
 *   friend to ConduitGroup permitting fast modification to \n
 *   the timeout_sg_pend_event period recorded in ConduitGroup hash table
 */
struct change_timeout_sg_pend_io {
    change_timeout_sg_pend_io (const double & new_timeout_sg_pend_io):
        new_timeout_sg_pend_io(new_timeout_sg_pend_io) {}
    void operator() (ConduitGroup& g)
    {
        g.timeout_sg_pend_io =  new_timeout_sg_pend_io;
    }

private:
    double new_timeout_sg_pend_io;
};


/**
 *   Friend to ConduitGroup records the channelTimeoutPolicySGGet in hash table
 */
struct change_channelTimeoutPolicySGGet {
    change_channelTimeoutPolicySGGet (const ChannelTimeoutPolicy & new_channelTimeoutPolicy):
        new_channelTimeoutPolicy(new_channelTimeoutPolicy) {}

    void operator() (ConduitGroup & g)
    {
        //This does a deep copy!
        g.channelTimeoutPolicySGGet =  new_channelTimeoutPolicy;
    }
private:
    ChannelTimeoutPolicy new_channelTimeoutPolicy;
};


/**
 *   Friend to ConduitGroup records the channelTimeoutPolicySGPut in hash table
 */
struct change_channelTimeoutPolicySGPut {
    change_channelTimeoutPolicySGPut (const ChannelTimeoutPolicy & new_channelTimeoutPolicy):
        new_channelTimeoutPolicy(new_channelTimeoutPolicy) {}

    void operator() (ConduitGroup & g)
    {
        //This does a deep copy!
        g.channelTimeoutPolicySGPut =  new_channelTimeoutPolicy;
    }
private:
    ChannelTimeoutPolicy new_channelTimeoutPolicy;
};



#endif
