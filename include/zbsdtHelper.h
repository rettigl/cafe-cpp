///
/// \file    zbsdtHelper.h
/// \author  Jan Chrin, PSI
/// \date    Release: January 2018
/// \version CAFE 1.6

#ifndef ZBSDTHELPER_H
#define ZBSDTHELPER_H

#include <zbsHash.h>
#include <bitshuffle/bitshuffle.h>
namespace CAFE_BSHELPER
{

using boost::multi_index_container;
using namespace boost::multi_index;

bsreadContainer_set bsdt;

std::vector<BSChannel> bsrdV;


//Type maching is as follows:
//  int8  [1 byte] to char
// uint8  [1] to unsigned char
//  int16 [2] to short
// uint16 [2] to unsigned short
//  int32 [4] to long
// uint32 [4] to unsigned long
//  int64 [8] to long long
// uint64 [8] to unsigned long long
// float32[4] to float
// float64[8] to double
// bool   [1] same as uint8; 0=false, !0 = true
// string to char  (Byte order of strings are irrelevant - always big-endian). The String encoding is utf8; from bsread documentation

enum bsdtIndex {BS_INT8=0,  BS_UINT8,   BS_INT16, BS_UINT16,
                BS_INT32,   BS_UINT32,  BS_INT64, BS_UINT64,
                BS_FLOAT32, BS_FLOAT64, BS_BOOL,  BS_STRING, BS_CHAR=BS_STRING
               };

void bsdtInsert()
{
    bsdt.insert(bsreadContainer(BS_INT8,      "int8"));
    bsdt.insert(bsreadContainer(BS_UINT8,    "uint8"));
    bsdt.insert(bsreadContainer(BS_INT16,    "int16"));
    bsdt.insert(bsreadContainer(BS_UINT16,  "uint16"));
    bsdt.insert(bsreadContainer(BS_INT32,    "int32"));
    bsdt.insert(bsreadContainer(BS_UINT32,  "uint32"));
    bsdt.insert(bsreadContainer(BS_INT64,    "int64"));
    bsdt.insert(bsreadContainer(BS_UINT64,  "uint64"));
    bsdt.insert(bsreadContainer(BS_FLOAT32,"float32"));
    bsdt.insert(bsreadContainer(BS_FLOAT64,"float64"));
    bsdt.insert(bsreadContainer(BS_BOOL,      "bool"));
    bsdt.insert(bsreadContainer(BS_STRING,  "string"));
    bsdt.insert(bsreadContainer(BS_CHAR,    "string"));
}


int getByteSize(unsigned int dt)
{
#define __METHOD__ "getByteSize (unsigned int)"
    switch(dt)
    {
    case BS_INT8:
        return sizeof(int8_t);
    case BS_UINT8:
    case BS_BOOL:
        return sizeof(uint8_t);
    case BS_INT16:
        return sizeof(int16_t);
    case BS_UINT16:
        return sizeof(uint16_t);
    case BS_INT32:
        return sizeof(int32_t);
    case BS_UINT32:
        return sizeof(uint32_t);
    case BS_INT64:
        return sizeof(int64_t);
    case BS_UINT64:
        return sizeof(uint64_t);
    case BS_FLOAT32:
        return sizeof(float);
    case BS_FLOAT64:
        return sizeof(double);
    case BS_STRING: //is also BS_CHAR
        return sizeof(char);
    default:
        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        std::cout << "Enumerated input value " << dt << " is outside allowed band: " << BS_INT8 << " and " << BS_STRING << std::endl;
        print_out_by<by_bsID>(bsdt);
        std::cout << "Returning byte size of 1" << std::endl;
        return 1;
    }
#undef __METHOD__
}


int getByteSize(std::string dt)
{
#define __METHOD__ "getByteSize (string)"
    bsreadContainer_set_by_name & name_index = bsdt.get<by_bsName> ();
    bsreadContainer_set_by_name::iterator it_name;
    unsigned int bsdtIndex;

    //Find enum of data type
    it_name = name_index.find(dt);
    if ( (it_name) != name_index.end() )
    {
        bsdtIndex=(*it_name).by_bsID;
        return getByteSize(bsdtIndex);
    }

    std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
    std::cout << "string value " << dt << " is not recognized " << std::endl;
    print_out_by<by_bsID>(bsdt);
    std::cout << "Returning byte size of 1" << std::endl;
    return 1;

#undef __METHOD__
}



//Easy show-error-and-bail function.
//void run_screaming(const char *message, const int code) {
//	printf("%s\n", message);
//	exit(code);
//	return;
//}

/**
 * \brief Generic function to display bits of a number
 * used for testing in zmq stream
 */
void printBits(int const size, void const * const ptr)
{
    unsigned char *b= (unsigned char *) ptr;
    unsigned char byte;
    int i, j;
    for (i=size-1; i>=0; i--)
    {
        for (j=7; j>=0; j--)
        {
            byte =(b[i]>>j ) &1;
            printf("%u", byte);
        }
    }
    puts("");
    return;
}

/**
 * \brief Returns true if at least one bit within byte is set
 * Used to test endianess of timestamp
 */
bool hasMinOneBitSet(int const size, void const * const ptr) 
{
    unsigned char *b= (unsigned char *) ptr;
    unsigned char byte;
   
    bool hasBit = false;
    int i, j;
    for (i=size-1; i>=0; i--)
    {
        for (j=7; j>=0; j--)
        {
            byte =(b[i]>>j ) &1;
	    if (byte) {
	      return true;
	    }
        }
    }
    return false;
}


/**
 * \brief Unfolds the two bytes that precede the blob
 * See: https://git.psi.ch/sf_daq/ch.psi.daq.dispatcherrest
 * and: https://git.psi.ch/sf_daq/bsread_specification
 */
int unfoldPreBlob(const char * data, long &uncompressedSize, int &blockSize)
{

    //Check for -ve numbers
    uncompressedSize = ( ((data[7] <<0) & 0x7f) | ((data[7]<<0) & 0x80) |  (data[6]<<8) | (data[5]<<16) |  (data[4]<<24)
                         |  ((long long) data[3]<<32) | ((long long) data[2]<<40) | ((long long) data[1]<<48) | ((long long) data[0]<<56) ) ;

    // printBits(2, &uncompressedSize);


    //std::cout << "UNCOMPRESSED SIZE / " << uncompressedSizeL << " " << uncompressedSize << std::endl;



    //uncompressedSize = ( ((data[7] <<0) & 0x7f) | ((data[7]<<0) & 0x80) |  (data[6]<<8) | (data[5]<<16) |  (data[4]<<24)
    //	| ((long long) data[3]<<32) | ((long long) data[2]<<40) | ((long long) data[1]<<48) | ((long long) data[0]<<56) ) ;
    //std::cout << "UNCOMPRESSED SIZE // " << uncompressedSize << std::endl;


    //All three give equivalent block size
    //blockSize= ( (data[11]<<0)  |  (data[10]<<8) | (data[9]<<16) |  (data[8]<<24) );
    //std::cout << "BLOCKSIZE  " << blockSize << std::endl;

    blockSize= ( ((data[11]<<0)) | ((data[11]<<0)& 0x80) |  (data[10]<<8) | (data[9]<<16) |  (data[8]<<24) );
    //std::cout << "BLOCKSIZE / " << blockSize << std::endl;

    //blockSize= ( ((data[11]<<0) & 0x7f) | ((data[11]<<0)& 0x80) |  (data[10]<<8) | (data[9]<<16) |  (data[8]<<24) );
    //std::cout << "BLOCKSIZE // " << blockSize << std::endl;

    return ICAFE_NORMAL;;
}


/**
 * \brief DataBitset Template \n
 * CTYPE is the integer type the data type is to be rendered to the client \n
 * method unfold() will receive data and then convert to CTYPE
 */
template <class CTYPE> class DataBitset
{
public:
    DataBitset () {};
    ~DataBitset () {};
    CTYPE  unfold(char * data, size_t start, size_t end, std::string encoding )
    {
        //std::cout << "encoding=" << encoding << std::endl;
        if (encoding=="big")
        {
            //big endian
            for (short n=start; n < end; n++)
            {
                i = (i << 8) + data[n];
            }
        }
        else
        {
            //little endian
            for (short n = end; n >= start; n--)
            {
                i = (i << 8) + data[n];
            }
        }
        //std::cout << "Value = " << i << std::endl;
        return i;
    }
private:
    CTYPE i;
};


/**
 * \brief IntegerBitset Template \n
 * CTYPE is the integer type the data type is to be rendered to the client \n
 * method unfold() will receive data and then convert to vector<CTYPE>
 */
template <class CTYPE> class IntegerBitsetV
{
public:
    IntegerBitsetV  () {};
    ~IntegerBitsetV () {};

    CTYPE unfold(char * data, unsigned int nelem, std::string encoding, std::vector<CTYPE> &valV )
    {
        valV.clear();
        valV.reserve(nelem);
        cSize=sizeof(CTYPE);
        idx=0;
        if (encoding=="big")
        {
            switch (cSize)
            {
            case 1:
                for (unsigned int i=0; i<nelem; ++i)
                {
                    idx=i*cSize;
                    iVal= (data[idx+1]<<0) & 0xff;
                    valV.push_back(iVal);
                }
                break;
            case 2:
                for (unsigned int i=0; i<nelem; ++i)
                {
                    idx=i*cSize;
                    iVal= ( ((data[idx+1]<<0) & 0xff)  | ((data[idx]<<8) & 0xffff));
                    valV.push_back(iVal);
                }
                break;
            case 4:
                for (unsigned int i=0; i<nelem; ++i)
                {
                    idx=i*cSize;
                    iVal= ( ((data[idx+3]<<0)  & 0xff  )   | ((data[idx+2]<<8) & 0xffff ) 
			    | ((data[idx+1]<<16) & 0xffffff) | ((data[idx]<<24)  & 0xffffffff) );
                    valV.push_back(iVal);
                }
                break;
            case 8:
                for (unsigned int i=0; i<nelem; ++i)
                {
                    idx=i*cSize;
                    iVal= ( ((data[idx+7]<<0) & 0xff)  |  ((data[idx+6]<<8) & 0xffff ) 
			     | ((data[idx+5]<<16) & 0xffffff )  |  ((data[idx+4]<<24) & 0xffffffff )
                             | (((long long) data[idx+3]<<32) & 0xffffffffff) 
			     | (((long long) data[idx+2]<<40) & 0xffffffffffff)  
			     | (((long long) data[idx+1]<<48) & 0xffffffffffffff)  
			     | (((long long) data[idx]<<56)   & 0xffffffffffffffff)  ) ;
                    valV.push_back(iVal);
                }
		break;
            default:
                std::cout << "Cannot process element of size " << cSize << " bytes" << std::endl;
            }
        }
        else   //little endian
        {
            switch (cSize)
            {
            case 1:
                for (unsigned int i=0; i<nelem; ++i)
                {
                    idx=i*cSize;
                    iVal= (data[idx]<<0) & 0xff;
                    valV.push_back(iVal);
                }
                break;
            case 2:
                for (unsigned int i=0; i<nelem; ++i)
                {
                    idx=i*cSize;
                    iVal= ( ( (data[idx]<<0) & 0xff )   | ( (data[idx+1]<<8) & 0xffff) ); 
                    valV.push_back(iVal);
                }
                break;
            case 4:
                for (unsigned int i=0; i<nelem; ++i)
                {
                    idx=i*cSize;
                    iVal= ( ( (data[idx]<<0)  & 0xff)   | ( (data[idx+1]<<8) & 0xffff ) 
                             | ( (data[idx+2]<<16) & 0xffffff) |  ((data[idx+3]<<24) & 0xffffffff ));
                    valV.push_back(iVal);
                }
                break;
            case 8:
                for (unsigned int i=0; i<nelem; ++i)
                {
                    idx=i*cSize;
		    iVal = ( ( (data[idx]<<0) & 0xFF)   |  ( (data[idx+1]<<8) & 0xFFFF)
                             | ((data[idx+2]<<16) & 0xFFFFFF) |  ( (data[idx+3]<<24) & 0xFFFFFFFF)
                             | (((long long) data[idx+4]<<32) & 0xFFFFFFFFFF  ) 
                             | (((long long) data[idx+5]<<40) & 0xFFFFFFFFFFFF  ) 
			     | (((long long) data[idx+6]<<48) & 0xFFFFFFFFFFFFFF  ) 
                             | (((long long) data[idx+7]<<56) & 0xFFFFFFFFFFFFFFFF  ) ) ; 
                    valV.push_back(iVal);
                }
		break;
            default:
                std::cout << "Cannot process element of size " << cSize << " bytes" << std::endl;
            }
        }
        return valV[0]; //scalar value
    }
private:
    unsigned int cSize;
    unsigned int idx;
    CTYPE iVal;
};



/**
 * \brief IntegerBitset Template \n
 * CTYPE is the integer type the data type is to be rendered to the client \n
 * method unfold() will receive data and then convert to CTYPE *
 */
template <class CTYPE> class IntegerBitset
{
public:
    IntegerBitset  () {};
    ~IntegerBitset () {};

    CTYPE * unfold(char * data, unsigned int nelem, std::string encoding )
    {
        //std::cout << sizeof(char) << " s+  " << sizeof(short) << " i+ " << sizeof(int) << " l+ " <<sizeof(long) << " ll+ " << sizeof(long long) << std::endl;
        cSize=sizeof(CTYPE);
        idx=0;
        iVal=new CTYPE[nelem];
        //std::cout << "cSize " << cSize << std::endl;
	//std::cout << "nelem " << nelem << " encoding" << encoding << std::endl;
        if (encoding=="big")
        {
            switch (cSize)
            {
            case 1:
                for (unsigned int i=0; i<nelem; ++i)
                {
                    idx=i*cSize;
                    iVal[i]= data[idx]<<0 & 0xff; 
                }
                break;
            case 2:
                for (unsigned int i=0; i<nelem; ++i)
                {
                    idx=i*cSize;
                    iVal[i]= ( ((data[idx+1]<<0) & 0xff)  | ((data[idx]<<8) & 0xffff));
                    //char is=data[idx]; printBits(1,&is);
                    //is=data[idx+1]; printBits(1,&is);
                    //iVal[i]=(  (data[idx+1]<<0)   | (data[idx]<<8) );
                    //short ss=iVal[i];
                    //printBits(2, &ss);
                    //std::cout << iVal[i] << " [" << i << "] " ;
                }
                //std::cout << std::endl;
                break;
            case 4:
                for (unsigned int i=0; i<nelem; ++i)
                {		 
                    idx=i*cSize;
                    iVal[i]=( ((data[idx+3]<<0)  & 0xff  )   | ((data[idx+2]<<8) & 0xffff ) 
			    | ((data[idx+1]<<16) & 0xffffff) | ((data[idx]<<24)  & 0xffffffff) );
                }
                break;
            case 8:
                for (unsigned int i=0; i<nelem; ++i)
                {
                    idx=i*cSize;
                    iVal[i]= ( ((data[idx+7]<<0) & 0xff)  |  ((data[idx+6]<<8) & 0xffff ) 
			     | ((data[idx+5]<<16) & 0xffffff )  |  ((data[idx+4]<<24) & 0xffffffff )
                             | (((long long) data[idx+3]<<32) & 0xffffffffff) 
			     | (((long long) data[idx+2]<<40) & 0xffffffffffff)  
			     | (((long long) data[idx+1]<<48) & 0xffffffffffffff)  
			     | (((long long) data[idx]<<56)   & 0xffffffffffffffff)  ) ;
                }
            default:
                std::cout << "Cannot process element of size " << cSize << " bytes" << std::endl;
            }
        }
        else   //little endian
        {
            switch (cSize)
            {
            case 1:
                for (unsigned int i=0; i<nelem; ++i)
                {
                    idx=i*cSize;
                    iVal[i]= (data[idx]<<0) & 0xff;
                }
                break;
            case 2:
                for (unsigned int i=0; i<nelem; ++i)
                {
                    idx=i*cSize;
                    iVal[i]=( ( (data[idx]<<0) & 0xff )   | ( (data[idx+1]<<8) & 0xffff) ); 
                }
                break;
            case 4:
                for (unsigned int i=0; i<nelem; ++i)
                {
                    idx=i*cSize;
                    
                    iVal[i]=( ( (data[idx]<<0)  & 0xff)   | ( (data[idx+1]<<8) & 0xffff ) 
                             | ( (data[idx+2]<<16) & 0xffffff) |  ((data[idx+3]<<24) & 0xffffffff ));
                }
                break;
            case 8:
                for (unsigned int i=0; i<nelem; ++i)
                {
                    idx=i*cSize;
                    iVal[i]=( ( (data[idx]<<0) & 0xFF)   |  ( (data[idx+1]<<8) & 0xFFFF)
                             | ((data[idx+2]<<16) & 0xFFFFFF) |  ( (data[idx+3]<<24) & 0xFFFFFFFF)
                             | (((long long) data[idx+4]<<32) & 0xFFFFFFFFFF  ) 
                             | (((long long) data[idx+5]<<40) & 0xFFFFFFFFFFFF  ) 
			     | (((long long) data[idx+6]<<48) & 0xFFFFFFFFFFFFFF  ) 
                             | (((long long) data[idx+7]<<56) & 0xFFFFFFFFFFFFFFFF  ) ) ; 
                }
		break;
            default:
                std::cout << "Cannot process element of size " << cSize << " bytes" << std::endl;
            }
        }

        return iVal;
    }



    CTYPE  unfoldScalar(char * data, std::string encoding )
    {
        cSize=sizeof(CTYPE);
        idx=0;

        if (encoding=="big")
        {
            switch (cSize)
            {
            case 1:
                idx=0;
                iValScalar = data[idx]<<0 & 0xff; 
                break;
            case 2:
                idx=0;  
                iValScalar=( ((data[idx+1]<<0) & 0xff)  | ((data[idx]<<8) & 0xffff));
                break;
            case 4:
                idx=0;
                iValScalar=( ((data[idx+3]<<0)  & 0xff)     | ((data[idx+2]<<8) & 0xffff) 
			   | ((data[idx+1]<<16) & 0xffffff) | ((data[idx]<<24)  & 0xffffffff) );
                break;
            case 8:
                idx=0;               
		iValScalar=( ((data[idx+7]<<0) & 0xff)  |  ((data[idx+6]<<8) & 0xffff ) 
			     | ((data[idx+5]<<16) & 0xffffff )  |  ((data[idx+4]<<24) & 0xffffffff )
                             | (((long long) data[idx+3]<<32) & 0xffffffffff) 
			     | (((long long) data[idx+2]<<40) & 0xffffffffffff)  
			     | (((long long) data[idx+1]<<48) & 0xffffffffffffff)  
			     | (((long long) data[idx]<<56)   & 0xffffffffffffffff)  ) ;
		break;
            default:
                std::cout << "Cannot process element of size " << cSize << " bytes" << std::endl;
            }
        }
        else   //little endian
        {
            switch (cSize)
            {
            case 1:
                idx=0;
                iValScalar=  (data[idx]<<0) & 0xff; 
                break;
            case 2:
                idx=0;
		iValScalar=( ( (data[idx]<<0) & 0xff )   | ( (data[idx+1]<<8) & 0xffff) );              
                break;
            case 4:
                idx=0;
                iValScalar=( ( (data[idx]<<0)  & 0xff)   | ( (data[idx+1]<<8) & 0xffff ) 
                             | ( (data[idx+2]<<16) & 0xffffff) |  ((data[idx+3]<<24) & 0xffffffff ));                
                break;
            case 8:
                idx=0;
                iValScalar=( ( (data[idx]<<0) & 0xFF)   |  ( (data[idx+1]<<8) & 0xFFFF)
                             | ((data[idx+2]<<16) & 0xFFFFFF) |  ( (data[idx+3]<<24) & 0xFFFFFFFF)
                             | (((long long) data[idx+4]<<32) & 0xFFFFFFFFFF  ) 
                             | (((long long) data[idx+5]<<40) & 0xFFFFFFFFFFFF  ) 
			     | (((long long) data[idx+6]<<48) & 0xFFFFFFFFFFFFFF  ) 
                             | (((long long) data[idx+7]<<56) & 0xFFFFFFFFFFFFFFFF  ) ) ;               
		break;
            default:
                std::cout << "Cannot process element of size " << cSize << " bytes" << std::endl;
            }
        }
        return iValScalar;
    }

private:
    unsigned int cSize;
    unsigned int idx;
    CTYPE * iVal;
    CTYPE iValScalar;
};


// big endian
//	int uc = ( ( (data[7]<<0) & 0x7f) | ((data[7]<<0) & 0x80)  |  (data[6]<<8) | (data[5]<<16) |  (data[4]<<24) | ((long long) data[3]<<32)
//			                     | ((long long) data[2]<<40) | ((long long) data[1]<<48) |  ((long long) data[0]<<56) ) ;
//
//			  int bs = ( ( (data[11]<<0)  & 0x7f) | ((data[11]<<0)  & 0x80)  | (data[10]<<8) | (data[9]<<16) |  (data[8]<<24));

// big endian
// bs = ( ( (regen_buffer[n] <<24)  ) |  ( (regen_buffer[n+1] <<26)  ) |  ( (regen_buffer[n+2] <<8)  )
//						                        |  ( (regen_buffer[n+3] <<0) & 0x7f) | (regen_buffer[n+3] <<0) & 0x80 ) ;

/**
 * \brief FloatBitset Template \n
 * CTYPE is the float type of the data type to be rendered to the client \n
 * method unfold() will receive data and then convert to CTYPE
 */
template <class CTYPE> class FloatBitset
{
public:
    FloatBitset () {};
    ~FloatBitset () {};
    CTYPE * unfold(char * data, unsigned int nelem, std::string encoding)
    {
        cSize=sizeof(CTYPE);
        val=new CTYPE[nelem];
        //std::cout << "HELPER>H " << nelem << " " << encoding  << " cSize " << cSize << std::endl;
        if (encoding=="big")
        {
            for (unsigned int i=0; i<nelem; ++i)
            {
                start=i*cSize;
                end=(i+1)*cSize;
                //std::cout << start << " // " << end << std::endl;
                inst=0;

                for (size_t char_nbr = start; char_nbr < end ; char_nbr++)
                {

                    idx=(nelem*cSize)-1-char_nbr;

                    //std::cout << inst << " " << idx << std::endl;
                    bar.c[inst]=data[idx];
                    ++inst;
                }
                //std::cout << "i= " << i << " " << std::endl;
                //std::cout << bar.v << " [" << i << "] " << std::endl;
                val[nelem-1-i]=bar.v;
                //std::cout << val[nelem-1-i] << " [" << (nelem-1-i) << "] " << std::endl;
            }
        }
        // little endian

        else
        {
            for (unsigned int i=0; i<nelem; ++i)
            {
                start=i*cSize;
                end=(i+1)*cSize;
                inst=0;

                for (size_t char_nbr = start; char_nbr < end ; char_nbr++)
                {
                    bar.c[inst]=data[char_nbr];
                    ++inst;
                }
                val[i]=bar.v;
            }
        }
        return val;
    }

    CTYPE  unfoldScalar(char * data,  std::string encoding)
    {
        cSize=sizeof(CTYPE);

        if (encoding=="big")
        {
            start=0;
            end=cSize;
            inst=0;
            for (size_t char_nbr = start; char_nbr < end ; char_nbr++)
            {
                idx=cSize-1-char_nbr;
                bar.c[inst]=data[idx];
                ++inst;
            }
        }

        // little endian
        else
        {

            start=0;
            end=cSize;
            inst=0;

            for (size_t char_nbr = start; char_nbr < end ; char_nbr++)
            {
                bar.c[inst]=data[char_nbr];
                ++inst;
            }


        }
        return bar.v;
    };


    /*
    	CTYPE  unfold(char * data, size_t start, size_t end, std::string encoding )
    	{
    		if (encoding=="big") {
    			// big endian
    			for (size_t char_nbr = start; char_nbr < end; char_nbr++) {
    				bar.c[char_nbr]=data[end-1-char_nbr];
    			}
    		} else {
    			// little endian
    			for (size_t char_nbr = start; char_nbr < end; char_nbr++) {
    				bar.c[char_nbr]=data[char_nbr];
    			}
    		}

    		//std::cout << "UNION D " << bar.v << std::endl;
    		return bar.v;
    	}
    */
private:
    union dUnion
    {
        char c[sizeof(CTYPE)];
        CTYPE v;
    }  bar;
    CTYPE * val;
    unsigned int cSize;
    int start;
    int end;
    int inst;
    int idx;
};




/**
 * \brief FloatBitsetV Template \n
 * CTYPE is the float type of the data type to be rendered to the client \n
 * method unfold() will receive data and then convert to vector<CTYPE>
 */
template <class CTYPE> class FloatBitsetV
{
public:
    FloatBitsetV () {};
    ~FloatBitsetV () {};
    CTYPE unfold(char * data, unsigned int nelem, std::string encoding, std::vector<CTYPE> &valV)
    {

        //memory already allocated in zbsread.h
        //valV.clear();
        //valV.reserve(nelem);
        cSize=sizeof(CTYPE);
        //std::cout << "HELPER_H " << nelem << " " << encoding  << " cSize " << cSize << std::endl;

        if (encoding=="big")
        {
            for (unsigned int i=0; i<nelem; ++i)
            {

                start=i*cSize;
                end=(i+1)*cSize;
                std::cout << start << " // " << end << std::endl;
                inst=0;
                for (size_t char_nbr = start; char_nbr < end ; char_nbr++)
                {
                    idx=(nelem*cSize)-1-char_nbr;
                    bar.c[inst]=data[idx];
                    ++inst;
                }

                std::cout << bar.v << " [" << (nelem-1-i) << "] " ;
                valV[nelem-1-i]=bar.v;
                std::cout << valV[nelem-1-i] << " [" << (nelem-1-i) << "] " ;

                //valV.push_back(bar.v);

            }
        }
        else     // little endian
        {
            for (unsigned int i=0; i<nelem; ++i)
            {
                start=i*cSize;
                end=(i+1)*cSize;
                inst=0;
                for (size_t char_nbr = start; char_nbr < end ; char_nbr++)
                {
                    bar.c[inst]=data[char_nbr];
                    ++inst;
                }
                std::cout << bar.v << " [" << i << "] " ;
                valV[i]=bar.v;
                //valV.push_back(bar.v);
            }
        }
        std::cout << std::endl;
        return valV[0]; //scalar value
    }

private:
    union dUnion
    {
        char c[sizeof(CTYPE)];
        CTYPE v;
    }  bar;
    unsigned int cSize;
    int start;
    int end;
    int inst;
    int idx;
};



/**
 * \brief Routine that decompresses the compressed WFs and other data
 */
int bitshuffleDecompress( const char * data, char* & regen_buffer, int sizeData, int dtByteSize)
{
#define __METHOD__ "bitshuffleDecompress"
    long uncompressedSize=0;
    int blockSize=0;
    int localDebug=0;

    int sizeBlob = sizeData-BSREAD_PREBLOB_BYTES;
    if (sizeBlob <1)
    {
        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        std::cout << "WARNING: data size " << sizeData << " is less than BSREAD_PREBLOB_BYTES " << BSREAD_PREBLOB_BYTES << std::endl;
        std::cout << "This should not happen. Probably we are trying to decompress uncompressed data! " << std::endl;
        return ECAFE_BITSHUFF_BADCOUNT;
    } //Should not happen

    unfoldPreBlob(data, uncompressedSize, blockSize);
    //std::cout << "Uncompressed/Block size: " << uncompressedSize << " / " << blockSize << std::endl;

    //decompress
    int dataNelem=uncompressedSize/dtByteSize;
    //use bitshuffle terminology
    regen_buffer = (char *) malloc(uncompressedSize);
    if (regen_buffer == NULL)
    {
        std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
        std::cout << "FAILED TO ALLOC MEM of size: " << uncompressedSize << std::endl;
        return ECAFE_BITSHUFF_ALLOCMEM;
    }

    char * dataBlob = new char[sizeBlob];
    int ij=0;
    if(localDebug) std::cout << " BSREAD_PREBLOB_BYTES " << BSREAD_PREBLOB_BYTES <<std::endl;
    if(localDebug) std::cout << " sizeData   " << sizeData   <<std::endl;
    if(localDebug) std::cout << " sizeBlob   " << sizeBlob   <<std::endl;
    if(localDebug) std::cout << " dataNelem  " << dataNelem  <<std::endl;
    if(localDebug) std::cout << " dtByteSize " << dtByteSize <<std::endl;
    for (int char_nbr = BSREAD_PREBLOB_BYTES; char_nbr < sizeData; char_nbr++)
    {
        dataBlob[ij]=data[char_nbr];
        ++ij;
    }
    //Use 0 rather than given block size as is recommended
    const int decompressed_size = bshuf_decompress_lz4((const char *) dataBlob, regen_buffer, dataNelem, dtByteSize, 0);
    delete [] dataBlob;
    if(localDebug) std::cout << " " << " decompressed_size " << decompressed_size << std::endl;
    if (decompressed_size < 0)
    {
        return ECAFE_BITSHUFF_DECOMPRESS;
    }
    else if (decompressed_size == 0)
    {
        if(localDebug)std::cout << "0 decompressed size! Should have a positive number for successful deompression?" <<std::endl;
    }
    else
    {
        if(localDebug)std::cout << "We successfully decompressed some data!" <<std::endl;
    }
    // Not only does a positive return value mean success,
    // value returned == number of bytes regenerated from compressed_data stream.
    // We can use this to realloc() *regen_buffer to free up memory
    // Did this at first to remove spurious trailing characters
    // BUT AVOID AT ALL COSTS; screws up regen_buffer content!!
    /*
    regen_buffer = (char *)realloc(regen_buffer, decompressed_size);
    if (regen_buffer == NULL) {
    	std::cout << __FILE__ << "//" << __LINE__ << "//" << __METHOD__ << std::endl;
    	std::cout << "WARNING with status code: " << ECAFE_BITSHUFF_REALLOCMEM << std::endl;
    	std::cout << "Failed to reallocate memory for compressed data. Not a show stopper" << std::endl;
    }
    */
    return ICAFE_NORMAL;
#undef __METHOD__
}

}
#endif
