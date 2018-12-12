#!/bin/busybox ash
#set -e -o pipefail #fail fast

echo "start"

read -r LINE
if [ $LINE = "start" ];
then
    echo starting
else
    >&2 echo "Process cancelled."
    exit 128
fi


#start multistrap/debootstrap here

>&2 echo "update_real.sh completed!"

#exit 0
#reboot
#shutdown -h now