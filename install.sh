#!/bin/sh

echo "running this shell script will install some tools on your machine."

read -p "continue?[y/n]"  continue_flag

if [ "$continue_flag"="n" -o "$continue_flag"="N" ]
then
    exit 0
fi

