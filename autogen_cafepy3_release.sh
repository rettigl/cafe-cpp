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

#--enable-boost --with-boost=/afs/psi.ch/intranet/Controls/BD/deps/boost_1_57_0 \
CYCAFE_VERSION=pycafe-1.3.0-final-1

./configure \
    --prefix=/opt/gfa/cafe/python/python-3.5/${CYCAFE_VERSION}/ \
    --libdir=/opt/gfa/cafe/python/python-3.5/${CYCAFE_VERSION}/lib \
    --enable-boost-inc --with-boost-inc=/opt/gfa/cafe/boost/boost_1_61_0/include \
    --enable-epics --with-epics=${EPICS}/base \
	--enable-qt4-lib --with-qt4-lib=/usr/lib64 \
	--enable-qt4-inc --with-qt4-inc=/usr/include \
	--enable-python-inc --with-python-inc=/opt/gfa/python-3.5/latest/include/python3.5m \
	--enable-python-lib --with-python-lib=/opt/gfa/python-3.5/latest/lib
unset  CAFE_EPICS_V_MAJOR
unset  CAFE_EPICS_V_MINOR
unset  CAFE_EPICS_V_PATCH 

cp include/config.h include/os/Linux/pycafe35/config.h

