#!/bin/sh

read -p "-- Warnning: some tools will be installed. continue?[y/n]"  continue_flag

if [ "n" = $continue_flag ]#-o "$continue_flag"="N" ]
then
    exit 0
fi

echo "-- Installing: cbuild"
rm -fr ./cbuild
git clone https://github.com/caibingcheng/cbuild.git --depth=1
cd ./cbuild
sh install.sh
cd ..
echo "-- Removing package."
rm -fr ./cbuild
