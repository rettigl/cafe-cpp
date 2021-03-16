rm -f configure.ac
ln -s configurezbspy37.ac configure.a

source usePy3
aclocal  --force -I m4
libtoolize --force --copy
autoconf --force
autoheader --force
automake --force --add-missing --copy
autoreconf

#Check what the EPICS RELEASE IS from ${EPICS}/base
#Used by ./configure
#Assumes format ${EPICS}/base-3.14.12


EB=$(readlink ${EPICS}/base)
echo 'EPICS BASE IS=' $EB
EB1=$(echo $EB | sed -e "s/[a-zA-Z]*-//g")
EBMAJ=$(echo $EB1 | sed -e "s/[[:punct:]].*//g")
EBMIN1=$(echo $EB1 | sed -e "s/^[[:digit:]]*[[:punct:]]//g")
EBMIN=$(echo $EBMIN1 | sed -e "s/[[:punct:]][[:digit:]]*//g")
EBPAT=$(echo $EB1 | sed -e "s/[[:digit:]]*[[:punct:]]//g")
echo EPICS_MAJOR=$EBMAJ 
echo EPICS_MINOR=$EBMIN 
echo EPICS_PATCH=$EBPAT

export CAFE_EPICS_V_MAJOR=$EBMAJ
export CAFE_EPICS_V_MINOR=$EBMIN
export CAFE_EPICS_V_PATCH=$EBPAT


CYCAFE_VERSION=pycafe-1.12.5

./configure \
    --prefix=/opt/gfa/cafe/python/python-3.7/${CYCAFE_VERSION}/ \
    --libdir=/opt/gfa/cafe/python/python-3.7/${CYCAFE_VERSION}/lib \
    --enable-boost-inc --with-boost-inc=/opt/gfa/cafe/boost/boost_1_61_0/include \
    --enable-epics --with-epics=${EPICS}/base \
    --enable-qt5-lib --with-qt5-lib=/opt/gfa/python-3.7/latest/lib \
    --enable-qt5-inc --with-qt5-inc=/opt/gfa/python-3.7/latest/include/qt \
    --enable-python-inc --with-python-inc=/opt/gfa/python-3.7/latest/include/python3.7m \
    --enable-python-lib --with-python-lib=/opt/gfa/python-3.7/latest/lib \
    --enable-zeromq-inc --with-zeromq-inc=/opt/gfa/zmq/zeromq-4.2.3-gcc-6.3.0/include \
    --enable-zeromq-lib --with-zeromq-lib=/opt/gfa/zmq/zeromq-4.2.3-gcc-6.3.0/lib \
    --enable-json-inc --with-json-inc=/opt/gfa/zmsglog/json/jsoncpp-src-0.6.0-rc2/include \
    --enable-json-lib --with-json-lib=/opt/gfa/zmsglog/json/jsoncpp-src-0.6.0-rc2/libs/linux-gcc-6.3.0 \
    --enable-curl-inc --with-curl-inc=/opt/gfa/zmq/curl-7.55.1/include \
    --enable-curl-lib --with-curl-lib=/opt/gfa/zmq/curl-7.55.1/lib/.libs \
    --enable-lz4-inc --with-lz4-inc=/opt/gfa/zmq/lz4/lib \
    --enable-lz4-lib --with-lz4-lib=/opt/gfa/zmq/lz4/lib

unset  CAFE_EPICS_V_MAJOR
unset  CAFE_EPICS_V_MINOR
unset  CAFE_EPICS_V_PATCH 

cp include/config.h include/os/Linux/pycafe37/config.h

