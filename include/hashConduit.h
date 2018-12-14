///
/// \file    hashConduit.h
/// \author  Jan Chrin, PSI
/// \date    Release: February 2015
/// \version CAFE 1.0.0

#ifndef HASHCONDUIT_H
#define HASHCONDUIT_H

#include <conduit.h>
#include <map>

// boost include
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/tokenizer.hpp>

using boost::multi_index_container;
using namespace boost::multi_index;

struct by_handle {};
struct by_channelID {};
struct by_pv {};
struct by_pvAlias {};


typedef multi_index_container<
Conduit,
indexed_by<
ordered_unique<
tag<by_handle>,    BOOST_MULTI_INDEX_MEMBER(Conduit, unsigned int, handle)>,
ordered_non_unique<
tag<by_channelID>, BOOST_MULTI_INDEX_MEMBER(Conduit, chid, channelID)>,
ordered_non_unique<
tag<by_pv>,        BOOST_MULTI_INDEX_MEMBER(Conduit, std::string, pv)>,
ordered_non_unique<
tag<by_pvAlias>,   BOOST_MULTI_INDEX_MEMBER(Conduit, std::string, pvAlias)>,
//ordered_non_unique<
//tag<by_uniqueID>,  BOOST_MULTI_INDEX_MEMBER(Conduit, unsigned int, uniqueID)>,
hashed_unique<member<Conduit, unsigned int, &Conduit::handle> >
>
> cafeConduit_set;


typedef cafeConduit_set::index<by_channelID>::type cafeConduit_set_by_channelID;
typedef cafeConduit_set::index<by_handle>::type cafeConduit_set_by_handle;
typedef cafeConduit_set::index<by_pv>::type cafeConduit_set_by_pv;
typedef cafeConduit_set::index<by_pvAlias>::type cafeConduit_set_by_pvAlias;


#endif // HASHCONDUIT_H
