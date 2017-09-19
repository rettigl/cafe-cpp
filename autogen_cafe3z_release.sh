#source usePy3
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

#--enable-boost --with-boost=/afs/psi.ch/intranet/Controls/BD/deps/boost_1_57_0 \
CYCAFE_VERSION=cafez-1.3.0-alpha-1

./configure \
    --prefix=/opt/gfa/cafe/cpp/${CYCAFE_VERSION}/ \
    --libdir=/opt/gfa/cafe/cpp/${CYCAFE_VERSION}/lib \
    --enable-boost-inc --with-boost-inc=/opt/gfa/cafe/boost/boost_1_61_0/include \
    --enable-epics --with-epics=${EPICS}/base \
	  --enable-qt4-lib --with-qt4-lib=/usr/lib64 \
	  --enable-qt4-inc --with-qt4-inc=/usr/include \
	  --enable-zeromq-inc --with-zeromq-inc=/opt/gfa/zmq/zeromq-4.1.5/include \
	  --enable-zeromq-lib --with-zeromq-lib=/opt/gfa/zmq/zeromq-4.1.5/lib \
    --enable-json-inc --with-json-inc=/opt/gfa/zmsglog/json/jsoncpp-src-0.6.0-rc2/include \
	  --enable-json-lib --with-json-lib=/opt/gfa/zmsglog/json/jsoncpp-src-0.6.0-rc2/libs/linux-gcc-4.4.7
	
unset  CAFE_EPICS_V_MAJOR
unset  CAFE_EPICS_V_MINOR
unset  CAFE_EPICS_V_PATCH 

cp include/config.h include/os/Linux/cafe/config.h

