#! /bin/bash

NAME=$1
CDT_PREFIX=${PREFIX}/${SUBPREFIX}
mkdir -p ${PREFIX}/bin/
mkdir -p ${PREFIX}/lib/cmake/${PROJECT}
mkdir -p ${CDT_PREFIX}/bin 
mkdir -p ${CDT_PREFIX}/include
mkdir -p ${CDT_PREFIX}/lib/cmake/${PROJECT}
mkdir -p ${CDT_PREFIX}/cmake
mkdir -p ${CDT_PREFIX}/scripts

#echo "${PREFIX} ** ${SUBPREFIX} ** ${CDT_PREFIX}"

# install binaries 
cp -R ${BUILD_DIR}/bin/* ${CDT_PREFIX}/bin 

# install cmake modules
sed "s/_PREFIX_/\/${SPREFIX}/g" ${BUILD_DIR}/modules/EnumivoWasmToolchainPackage.cmake &> ${CDT_PREFIX}/lib/cmake/${PROJECT}/EnumivoWasmToolchain.cmake
sed "s/_PREFIX_/\/${SPREFIX}\/${SSUBPREFIX}/g" ${BUILD_DIR}/modules/${PROJECT}-config.cmake.package &> ${CDT_PREFIX}/lib/cmake/${PROJECT}/${PROJECT}-config.cmake

# install scripts
cp -R ${BUILD_DIR}/scripts/* ${CDT_PREFIX}/scripts 

# install misc.
cp ${BUILD_DIR}/enumivo.imports ${CDT_PREFIX}

# install wasm includes
cp -R ${BUILD_DIR}/include/* ${CDT_PREFIX}/include

# install wasm libs
cp ${BUILD_DIR}/lib/*.a ${CDT_PREFIX}/lib

# make symlinks
pushd ${PREFIX}/lib/cmake/${PROJECT} &> /dev/null
ln -sf ../../../${SUBPREFIX}/lib/cmake/${PROJECT}/${PROJECT}-config.cmake ${PROJECT}-config.cmake
ln -sf ../../../${SUBPREFIX}/lib/cmake/${PROJECT}/EnumivoWasmToolchain.cmake EnumivoWasmToolchain.cmake
popd &> /dev/null

pushd ${PREFIX}/bin &> /dev/null
for f in `find ${BUILD_DIR}/bin -name "enumivo-*"`; do
   bn=$(basename $f)
   ln -sf ../${SUBPREFIX}/bin/$bn $bn
done
popd &> /dev/null

tar -cvzf $NAME ./${PREFIX}/*
rm -r ${PREFIX}
