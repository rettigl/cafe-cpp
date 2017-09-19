///
/// \file    enumStrings.cc
/// \author  Modified by Jan Chrin, PSI from Astari's C++11 version \n
///          Using boost::begin() boost::end() for C++
/// \date    Release, February 2015
/// \version CAFE 1.0.0


#include "enumStrings.h"
#include "defines.h"
#include "policies.h"
#include "cafeEnum.h"


template<typename T>
struct enumStrings
{
    static char const* data[];
};

template<typename T>
struct enumRefHolder
{
    T& enumVal;
    enumRefHolder(T& enumVal): enumVal(enumVal) {}
};

template<typename T>
struct enumConstRefHolder
{
    T const& enumVal;
    enumConstRefHolder(T const& enumVal): enumVal(enumVal) {}
};

template<typename T>
std::ostream& operator<<(std::ostream& str, enumConstRefHolder<T> const& data)
{
    //Add check on enumStrings<T>::data size to ensure correspondence with entries in cafeEnumEpics.h

    if ( boost::size( enumStrings<T>::data) > (unsigned int) data.enumVal) {
        return str << enumStrings<T>::data[data.enumVal];
    }
    else {
        return str << "ERROR: enumStrings.h reports data.enumVal= " << data.enumVal
                << " DOES NOT HAVE A STRING EQUIVALENT!";
    }
}


template<typename T>
std::istream& operator>>(std::istream& str, enumRefHolder<T> const& data)
{
    std::string value;
    str >> value;

    // These two can be made easier to read in C++11
    // using std::begin() and std::end()

    //static auto begin  = std::begin(enumStrings<T>::data);
    //static auto end    = std::end(enumStrings<T>::data);
    //auto find   = std::find(begin, end, value);
    //if (find != end)

    if ( std::find( boost::begin(enumStrings<T>::data),  boost::end( enumStrings<T>::data), value) !=
            boost::end( enumStrings<T>::data))
        {
        //data.enumVal = static_cast<T>(std::distance(begin, find));
        data.enumVal = static_cast<T>(std::distance(boost::begin(enumStrings<T>::data),
            std::find (boost::begin(enumStrings<T>::data), boost::end(enumStrings<T>::data), value ) ));
    }


    if (data.enumVal > boost::size( enumStrings<T>::data) ) {

        std::cout << "ERROR: enumStrings.h reports data.enumVal = " << data.enumVal
                << " is out of enum range = " << boost::size( enumStrings<T>::data) << std::endl;
    }

    return str;
}

template<typename T>
enumConstRefHolder<T>  enumToString(T const& e) {return enumConstRefHolder<T>(e);}

template<typename T>
enumRefHolder<T>       enumFromString(T& e)     {return enumRefHolder<T>(e);}






