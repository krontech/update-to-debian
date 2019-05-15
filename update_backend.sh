#!/bin/busybox ash

cd $(realpath $(dirname $0))
USBCHECKED=0

while [ 1 ];
do
    >&2 echo loop start
    read -r -t 1 LINE
    >&2 echo "$LINE"
    
    if [ "$LINE" = "Syscheck" ];
    then #UI has advanced to second screen; check presence of SD card and readiness of update files on USB
    
        # check presence of SD card in top slot 
        LSDEV=$(ls /dev | grep mmcblk1)
        if [[ "$LSDEV" == '' ]]; then
            echo NoSDPresent
        else
            echo WaitForUserInput
        fi
        if [[ $USBCHECKED == 0 ]]; then
                USBCHECKED=1
                echo USBCheckStart
                md5sum -c update.md5sum 2>&1
                echo USBCheckDone
        fi
    fi
    
    if [[ "$LINE" == "Start" ]];
    then # User has pressed Proceed button to start update
        break
    fi
done
>&2 echo 'loop done'

zcat debian.img.gz | dd of=/dev/mmcblk1 count=123456 2>&1 &
#temporarily use a small number of blocks so that tests are quicker and write less to the SD card
sleep 1
while [ 1 ];
do
    KILLALL="$(2>&1 killall -USR1 dd)"
    if [[ "$KILLALL" == '' ]];
    then
        sleep 1 # If killall command was successful (ie. and dd has output its progress), KILLALL will be empty.
    else
        break # If killall command errored out, dd has finished or errored out.
    fi
done

>&2 echo "update_backend.sh completed!"

#exit 0
#reboot
#shutdown -h now