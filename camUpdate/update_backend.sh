#!/bin/busybox ash

cd $(realpath $(dirname $0))
USBCHECKED=0

if [ $(pwd | grep mmcblk1) != '' ]; then
    echo ProgramRunFromTopSD
fi

while [ 1 ];
do
    >&2 echo loop start
    read -r -t 1 LINE
    >&2 echo "$LINE"
    
    if [ "$LINE" = "Tab-Syscheck" ];
    then # UI has advanced to second screen; check presence of SD card and readiness of update files on USB
    
        # check presence of SD card in top slot 
        LSDEV=$(ls /dev | grep mmcblk1)
        if [[ "$LSDEV" == '' ]]; then
            echo TopSDMissing
        else
            if [ $(sfdisk -s /dev/mmcblk1) -lt 3730000 ]; then echo TopSDTooSmall
            else echo TopSDGood
            fi
            
            if [ $(cat /sys/block/mmcblk1/device/oemid) == 0x5344 ]; # SanDisk
            then echo TopSDVettedBrand
            else echo TopSDUnknownBrand
            fi
        fi
        if [[ $USBCHECKED == 0 ]]; then
                USBCHECKED=1
                echo USBCheckStart
                md5sum -c update.md5sum 2>&1
                echo USBCheckDone
        fi
    fi
    
    if [[ "$LINE" == "Start" ]]; then # User has pressed Proceed button to start update
        break
    fi
done
>&2 echo 'loop done'

zcat debian.img.gz | dd of=/dev/mmcblk1 2>&1 &
#temporarily use a small number of blocks so that tests are quicker and write less to the SD card
sleep 1
while [ 1 ];
do
    KILLALLRESULT="$(2>&1 killall -USR1 dd)"
    if [[ "$KILLALLRESULT" == '' ]]; then
        sleep 1 # If killall command was successful (ie. and dd has output its progress), KILLALL will be empty.
    else
        break # If killall command errored out, dd has finished or errored out.
    fi
done
echo WriteDone

while [ "$LINE" != "PowerDown" ];
do
    >&2 echo loop start
    read -r -t 1 LINE
    >&2 echo "$LINE"
done
>&2 echo "update_backend.sh completed!"
shutdown -h now