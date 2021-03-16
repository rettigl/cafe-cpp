///
/// \file    zbsHash.h
/// \author  Jan Chrin, PSI
/// \date    Release: January 2018
/// \version CAFE 1.6

#ifndef ZBSHASH_H
#define ZBSHASH_H

// boost include
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/tokenizer.hpp>

using boost::multi_index_container;
using namespace boost::multi_index;


struct bsreadContainer
{
    long          by_bsID;
    std::string by_bsName;

    bsreadContainer(long _id, std::string _name):by_bsID(_id),by_bsName(_name) {};

    friend std::ostream& operator<<(std::ostream& os, const bsreadContainer& m)
    {
        //os<<m.by_bsID << " " << m.by_bsName << std::endl;
        os << m.by_bsName << std::endl;
        return os;
    }
};

struct by_bsID {};
struct by_bsName {};

typedef multi_index_container<
bsreadContainer,
indexed_by<
ordered_unique<
tag<by_bsName>, BOOST_MULTI_INDEX_MEMBER(bsreadContainer, std::string, by_bsName)>,
ordered_non_unique<
tag<by_bsID>,   BOOST_MULTI_INDEX_MEMBER(bsreadContainer, long, by_bsID )> >
> bsreadContainer_set;


template<typename Tag,typename MultiIndexContainer>
void print_out_by(
    const MultiIndexContainer& s,
    Tag* =0
)
{

// obtain a reference to the index tagged by Tag

    const typename boost::multi_index::index<MultiIndexContainer,Tag>::type& i=
        get<Tag>(s);

    typedef typename MultiIndexContainer::value_type value_type;

// dump the elements of the index to cout
    std::cout << "--------------" << std::endl;
    std::cout << " ENUMERATED LIST " << std::endl;
    std::cout << "--------------" << std::endl;
    std::copy(i.begin(),i.end(),std::ostream_iterator<value_type>(std::cout));
    std::cout << "--------------------" << std::endl;
}

typedef bsreadContainer_set::index<by_bsID>::type   bsreadContainer_set_by_id;
typedef bsreadContainer_set::index<by_bsName>::type bsreadContainer_set_by_name;


#endif


