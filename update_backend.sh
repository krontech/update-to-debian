#!/bin/busybox ash

sleep 5
echo "WaitForUserInput"

read -r LINE
if [ $LINE = "WaitForUserInput" ];
then
    echo WaitForUserInput
else
    >&2 echo "Process cancelled."
    exit 128
fi


>&2 echo "update_real.sh completed!"

#exit 0
#reboot
#shutdown -h now