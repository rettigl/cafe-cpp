///
/// \file    hashConduitGroup.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0
///

#ifndef HASHCONDUIT_GROUP_H
#define HASHCONDUIT_GROUP_H

#include "conduitGroup.h"

// boost include
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/tokenizer.hpp>

using boost::multi_index_container;
// namespace multi-indexing of reference handles to CAFEConduit objects
using namespace boost::multi_index;

struct by_groupHandle{};
struct by_groupID{};
struct by_groupName{};

typedef multi_index_container<
        ConduitGroup,
		indexed_by<
		ordered_unique<
                tag<by_groupHandle>, BOOST_MULTI_INDEX_MEMBER(ConduitGroup, unsigned int, groupHandle)>,
		ordered_non_unique<
                tag<by_groupID>,     BOOST_MULTI_INDEX_MEMBER(ConduitGroup, CA_SYNC_GID, groupID)>,
		ordered_non_unique<
                tag<by_groupName>,   BOOST_MULTI_INDEX_MEMBER(ConduitGroup, std::string, groupName)>,
                hashed_unique<member<ConduitGroup, unsigned int,  &ConduitGroup::groupHandle> >
		>
> cafeGroup_set;

typedef cafeGroup_set::index<by_groupID>::type        cafeGroup_set_by_groupID;
typedef cafeGroup_set::index<by_groupHandle>::type    cafeGroup_set_by_groupHandle;
typedef cafeGroup_set::index<by_groupName>::type      cafeGroup_set_by_groupName;

#endif
