#! /bin/bash

binaries=(enumivo-ranlib
          enumivo-ar
          enumivo-objdump
          enumivo-readelf
          enumivo-pp
          enumivo-cc
          enumivo-cpp
          enumivo-ld)

if [ -d "/usr/local/enu.wasmsdk" ]; then
   printf "\tDo you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            pushd /usr/local &> /dev/null
            rm -rf enu.wasmsdk
            pushd bin &> /dev/null
            for binary in ${binaries[@]}; do
               rm ${binary}
            done
            popd &> /dev/null
            break;;
         [Nn]* ) 
            printf "\tAborting uninstall\n\n"
            exit -1;;
      esac
   done
fi
