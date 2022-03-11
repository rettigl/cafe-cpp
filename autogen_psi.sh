### psi specific - select compiler
#source <this_autogen_file> <facility> <target_language>
#<facility = sls sls2 sf>  default is sf
#<target_language = cpp matlab python> 
module unload gcc
module load gcc/7.3.0
### psi specific - end

rm -f configure.ac
ln -s configurePSI.ac configure.ac

rm -f ./aclocal.m4
rm -rf ./autom4te.cache
aclocal  --force -I m4
/opt/gfa/python-3.7/latest/bin/libtoolize --force --copy #select libtools 2.4.6
autoconf --force  #interprets configure.ac
autoheader --force
automake --force --add-missing --copy
#autoreconf not wanted

#Check what the EPICS RELEASE IS from ${EPICS}/base
#Used by ./configure
#Assumes format ${EPICS}/base-3.14.12
#source cafeVersion-gcc-7.3.0
CAFE_V="cafe-1.14.5"
#For later check of existence of HOST_ARCH for $EPICS/include/os/$HOST_ARCH
#Assume Linux but check if Darwin
CAFE_HOST_FLAG_DARWIN=$(echo ${EPICS_HOST_ARCH} | grep -c "Darwin") 
export CAFE_HOST_FLAG_DARWIN=$CAFE_HOST_FLAG_DARWIN

RETURN_ON_WRONG_INPUT=true
RETURN_FLAG=false

EPICS_BASE=$(readlink ${EPICS}/base)
#EB=$(readlink /afs/psi.ch/project/cafe/gitworkspace/CAFE/cpp/base)
echo $EPICS_BASE
if [ -z "$EPICS_BASE" ]
then
    EPICS_BASE='base-7' #7.0 = 7+0 = 7 for major release
fi

echo 'input' "$0" "$1" "$2"
echo $EPICS_BASE

FACILITY=$(echo "$1" | tr '[:upper:]' '[:lower:]')
CACLIENT=$(echo "$2" | tr '[:upper:]' '[:lower:]')

FACILITY_DEFAULT="sf"
CACLIENT_DEFAULT="python"

FACILITY_TEMP=""
CACLIENT_TEMP=""

echo 'input arg. facility/target' $FACILITY $CACLIENT

if [ -n "$FACILITY" ]
then
    if [ "$FACILITY" = "matlab" ] || [ "$FACILITY" = "python" ] || \
       [ "$FACILITY" = "py310" ] || [ "$FACILITY" = "py38" ] || \
       [ "$FACILITY" = "py37" ] || [ "$FACILITY" = "py35" ] || \
       [ "$FACILITY" = "julia" ] || [ "$FACILITY" = "cpp" ] || \
       [ "$FACILITY" = "cc" ] 
    then
	   CACLIENT_TEMP=$FACILITY
	   FACILITY_TEMP=$2
    fi
fi

if [ -n "$CACLIENT" ]
then
    if [ "$CACLIENT" = "sls" ] || [ "$CACLIENT" = "sls2" ] || \
     [ "$CACLIENT" = "sf" ] || [ "$CACLIENT" = "swissfel" ] || \
     [ "$CACLIENT" = "sfel" ] || [ "$CACLIENT" = "hipa" ]
    then	
	if [ -z $FACILITY_TEMP ]
	then
	    FACILITY_TEMP=$CACLIENT
	    CACLIENT_TEMP=$1
	fi
    fi
fi



if [ -z "$FACILITY" ]
then
    FACILITY="sf"     
    echo "FACILITY = " $FACILITY
    if [ -z $CACLIENT ]
    then
	CACLIENT=$CACLIENT_DEFAULT
    fi
elif [ "$FACILITY" = "sls" ] || [ "$FACILITY" = "sls2" ] || \
     [ "$FACILITY" = "sf" ] || [ "$FACILITY" = "swissfel" ] || \
     [ "$FACILITY" = "sfel" ] || [ "$FACILITY" = "hipa" ]
then
    echo "FACILITY = " $FACILITY
elif [ -n "$FACILITY_TEMP" ]
then    
    if [ "$FACILITY_TEMP" = "sls" ]  || [ "$FACILITY_TEMP" = "sls2" ] || \
       [ "$FACILITY_TEMP" = "sf" ]   || [ "$FACILITY_TEMP" = "swissfel" ] || \
       [ "$FACILITY_TEMP" = "sfel" ] || [ "$FACILITY_TEMP" = "hipa" ]
    then

	
	FACILITY=$FACILITY_TEMP
    else
	if [ -n "$2" ] 
	then
	    echo "***UNRECOGNIZED INPUT FACILITY***" $FACILITY
	    echo "***VALID VALUES ARE: sls sls2 sf hipa"
	    RETURN_FLAG=true
	fi
	echo "***OTHERWISE THE DEFAULT FACILITY IS:" $FACILITY_DEFAULT	
	FACILITY=$FACILITY_DEFAULT
    fi
else
    echo "***UNRECOGNIZED INPUT FACILITY***" $FACILITY
    echo "***VALID VALUES ARE: sls sls2 sf hipa"
    RETURN_FLAG=true
    echo "***OTHERWISE THE DEFAULT FACILITY IS: " $FACILITY_DEFAULT	
    FACILITY=$FACILITY_DEFAULT    
fi

if [ -n "$CACLIENT" ]
then
    if [ "$CACLIENT" = "matlab" ] || [ "$CACLIENT" = "python" ] || \
       [ "$CACLIENT" = "py310" ] ||  [ "$CACLIENT" = "py38" ] || \
       [ "$CACLIENT" = "py37" ] || [ "$CACLIENT" = "py35" ] || \
       [ "$CACLIENT" = "julia" ] || [ "$CACLIENT" = "cpp" ] || \
       [ "$CACLIENT" = "cc" ] 
    then 	  
	echo "CA TARGET = " $CACLIENT
    else
	if [ -n "$CACLIENT_TEMP" ]
	then    
	    if [ "$CACLIENT_TEMP" = "matlab" ] || [ "$CACLIENT_TEMP" = "python" ] || \
	       [ "$CACLIENT_TEMP" = "py310" ] ||  [ "$CACLIENT_TEMP" = "py38" ] || \	
	       [ "$CACLIENT_TEMP" = "py37" ] || [ "$CACLIENT_TEMP" = "py35" ] || \
	       [ "$CACLIENT_TEMP" = "julia" ] || [ "$CACLIENT_TEMP" = "cpp" ] || \
	       [ "$CACLIENT_TEMP" = "cc" ] 
	    then
		CACLIENT=$CACLIENT_TEMP	
	    else
		echo "***UNRECOGNIZED INPUT CA TARGET***" $CACLIENT 
		echo "***VALID VALUES ARE: cpp matlab python"
		RETURN_FLAG=true
		echo "***OTHERWISE THE DEFAULT CA TARGET IS:" $CACLIENT_DEFAULT
		CACLIENT=$CACLIENT_DEFAULT
	    fi
	else
	    echo "***UNRECOGNIZED INPUT CA TARGET***" $CACLIENT 
	    echo "***VALID VALUES ARE: cpp matlab python"
	    RETURN_FLAG=true
	    echo "***OTHERWISE THE DEFAULT CA TARGET IS:" $CACLIENT_DEFAULT 
	    CACLIENT=$CACLIENT_DEFAULT	
	fi
    fi
else
    CACLIENT=$CACLIENT_DEFAULT
fi



if [ $RETURN_ON_WRONG_INPUT = true ] ; then
    if [ $RETURN_FLAG = true ] ; then 
	return 1 2>/dev/null
	exit 1
    fi
fi

echo "FACILITY SELECTED = " $FACILITY 
echo "CA TARGET  SELECTED = "  $CACLIENT

ENABLE_OPTIONS="--enable-boost-inc"

if [ "$FACILITY" = "sfel" ] || [ "$FACILITY" = "swissfel" ] 
then
    FACILITY="sf"
fi

if [ "$CACLIENT" = "python" ] || [ "$CACLIENT" = "julia" ]
then
    CACLIENT="py37"
fi


if [ "$FACILITY" = "sls2" ]
then
    ENABLE_OPTIONS+=" --enable-epics7"
    CAFE_V+="-sls2"
else
    ENABLE_OPTIONS+=" --enable-epics3"
    if [ "$FACILITY" = "sf" ]  && [ "$CACLIENT" != "matlab" ]
    then
	CAFE_V+="-sf"
    fi
fi

if [ "$CACLIENT" = "py35" ]
then
    ENABLE_OPTIONS+=" --enable-python35"
    ENABLE_OPTIONS+=" --enable-qt4"
    CAFE_V+="-py35"
elif  [ "$CACLIENT" = "py37" ]
then
    ENABLE_OPTIONS+=" --enable-python37"
    ENABLE_OPTIONS+=" --enable-qt5"
    CAFE_V+="-py37" 
elif  [ "$CACLIENT" = "py38" ]
then
    ENABLE_OPTIONS+=" --enable-python38"
    ENABLE_OPTIONS+=" --enable-qt5py38"
    CAFE_V+="-py38"
elif  [ "$CACLIENT" = "py310" ]
then
    ENABLE_OPTIONS+=" --enable-python310"
    #ENABLE_OPTIONS+=" --enable-qt5py310"
    CAFE_V+="-py310"
else
    ENABLE_OPTIONS+=" --enable-qt5" 
fi

if [ "$FACILITY" = "sf" ] && [ "$CACLIENT" != "matlab" ]
then
    #rm -f src/PyCafe.cpp
    #ln -s PyCafe3.cpp src/PyCafe.cpp
    ENABLE_OPTIONS+=" --enable-json"
    ENABLE_OPTIONS+=" --enable-zeromq"
    ENABLE_OPTIONS+=" --enable-curl"
    ENABLE_OPTIONS+=" --enable-lz4"
#else
    #rm -f src/PyCafe.cpp
    #ln -s PyCafe_sls.cpp src/PyCafe.cpp 
fi

CAFE_V+="-gcc-"$GCC_VERSION

echo "$CAFE_V"

#echo 'EPICS BASE IS='$EPICS_BASE
#echo 'EPICS BASE DIR='${EPICS}/base-7.0.4.1

EB1=$(echo $EPICS_BASE | sed -e "s/[a-zA-Z]*-//g")
EBMAJ=$(echo $EB1 | sed -e "s/[[:punct:]].*//g")
EBMIN1=$(echo $EB1 | sed -e "s/^[[:digit:]]*[[:punct:]]//g")
EBMIN=$(echo $EBMIN1 | sed -e "s/[[:punct:]][[:digit:]]*//g")
EBPAT=$(echo $EB1 | sed -e "s/[[:digit:]]*[[:punct:]]//g")
echo EPICS_MAJOR=$EBMAJ 
echo EPICS_MINOR=$EBMIN 
echo EPICS_PATCH=$EBPAT
#echo $EBMIN1

export CAFE_EPICS_V_MAJOR=$EBMAJ
export CAFE_EPICS_V_MINOR=$EBMIN
export CAFE_EPICS_V_PATCH=$EBPAT

#ENABLE_OPTIONS+=" --enable-qt5"
##ENABLE_OPTIONS+=" --enable-qt4"
#ENABLE_OPTIONS+=" --enable-python37"
##ENABLE_OPTIONS+=" --enable-python35"
#ENABLE_OPTIONS+=" --enable-json"
#ENABLE_OPTIONS+=" --enable-zeromq"
#ENABLE_OPTIONS+=" --enable-curl"
#ENABLE_OPTIONS+=" --enable-lz4"

echo 'ENABLE_OPTIONS='$ENABLE_OPTIONS

./configure \
    --prefix=/opt/gfa/cafe/cpp/${CAFE_V} \
    --libdir=/opt/gfa/cafe/cpp/${CAFE_V}/lib/${EPICS_HOST_ARCH} \
    ${ENABLE_OPTIONS} \
    --with-boost-inc=/opt/gfa/cafe/boost/boost_1_61_0/include \
    --with-epics3=${EPICS}/base \
    --with-python310=/ioc/python/latest \
    --with-python38=/opt/gfa/python-3.8/latest \
    --with-python37=/opt/gfa/python-3.7/latest \
    --with-python35=/opt/gfa/python-3.5/latest \
    --with-qt5py310=/ioc/python/latest \
    --with-qt5py38=/opt/gfa/python-3.8/latest \
    --with-qt5=/opt/gfa/python-3.7/latest \
    --with-qt4=/opt/gfa/python-3.5/latest \
    --with-json=/opt/gfa/zmq/json/jsoncpp-src-0.6.0-rc2 \
    --with-zeromq=/opt/gfa/zmq/zeromq-4.2.3-gcc-6.3.0 \
    --with-curl=/opt/gfa/zmq/curl-7.55.1 \
    --with-lz4=/opt/gfa/zmq/lz4/lib \
    --with-epics7=${EPICS}/base-7.0.6  
unset  CAFE_EPICS_V_PATCH
unset  CAFE_EPICS_V_MINOR
unset  CAFE_EPICS_V_MAJOR 

unset  CAFE_HOST_FLAG_DARWIN

