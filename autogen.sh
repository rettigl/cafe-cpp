### psi specific - select compiler
rm -f configure.ac
ln -s configurePSI.ac configure.ac
rm -f src/PyCafe.cpp
ln -s PyCafe_sls.cpp src/PyCafe.cpp
rm -f ./aclocal.m4
rm -rf ./autom4te.cache
aclocal  --force -I m4
/opt/gfa/python-3.7/latest/bin/libtoolize --force --copy #select libtools 2.4.6
autoconf --force  #interprets configure.ac
autoheader --force
automake --force --add-missing --copy

#Check what the EPICS RELEASE IS from ${EPICS}/base
#Used by ./configure
#Assumes format ${EPICS}/base-3.14.12
#source cafeVersion-gcc-7.3.0
CAFE_V="cafe-1.13.0"
#For later check of existence of HOST_ARCH for $EPICS/include/os/$HOST_ARCH
#Assume Linux but check if Darwin
CAFE_HOST_FLAG_DARWIN=$(echo ${EPICS_HOST_ARCH} | grep -c "Darwin") 
export CAFE_HOST_FLAG_DARWIN=$CAFE_HOST_FLAG_DARWIN

EPICS_BASE=$(readlink ${EPICS}/base)
#EB=$(readlink /afs/psi.ch/project/cafe/gitworkspace/CAFE/cpp/base)
echo $EPICS_BASE
if [ -z "$EB" ]
then
    EPICS_BASE='base-7.4.1' #7.0 = 7+0 = 7 for major release
fi

echo 'input' "$0" "$1" "$2"
echo $EPICS_BASE


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

ENABLE_OPTIONS="--enable-boost-inc"
ENABLE_OPTIONS+=" --enable-epics3"
#ENABLE_OPTIONS+=" --enable-epics7"
ENABLE_OPTIONS+=" --enable-qt5"
#ENABLE_OPTIONS+=" --enable-qt4"
ENABLE_OPTIONS+=" --enable-python37"
#ENABLE_OPTIONS+=" --enable-python35"
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
    --with-epics7=${EPICS}/base-7.0.4.1 \
    --with-epics3=${EPICS}/base \
    --with-python37=/opt/gfa/python-3.7/latest \
    --with-python35=/opt/gfa/python-3.5/latest \
    --with-qt5=/opt/gfa/python-3.7/latest \
    --with-qt4=/opt/gfa/python-3.5/latest \
    --with-json=/opt/gfa/zmsglog/json/jsoncpp-src-0.6.0-rc2 \
    --with-zeromq=/opt/gfa/zmq/zeromq-4.2.3-gcc-6.3.0 \
    --with-curl=/opt/gfa/zmq/curl-7.55.1 \
    --with-lz4=/opt/gfa/zmq/lz4/lib

unset  CAFE_EPICS_V_PATCH
unset  CAFE_EPICS_V_MINOR
unset  CAFE_EPICS_V_MAJOR 

unset  CAFE_HOST_FLAG_DARWIN

