#!/bin/busybox ash


while [ 1 ];
do
    >&2 echo loop start
    read -r -t 1 LINE
    >&2 echo $LINE
    
    if [ $LINE = "Syscheck" ];
    then #UI has advanced to second screen; check presence of SD card and readiness of update files on USB
        >&2 echo "list dev"
        LSDEV=$(ls /dev | grep mmcblk1)
        if [[ "$LSDEV" = '' ]]; then
            echo NoSDPresent
        else
            echo WaitForUserInput
        fi
    fi
    
    if [ $LINE = "Start" ];
    then #
        break
    fi

    >&2 echo loop finish
    >&2 echo ""
done
>&2 echo 'loop done'

zcat /media/sda4/debian-2019-05-03.img.gz | dd of=/dev/mmcblk1 count=12345 &
#temporarily use a small number of blocks so that tests are quicker and write less to the SD card
sleep 1
while [ 1 ];
do
    KILLALL=$(killall -USR1 dd)
    if [ KILLALL = 'killall: dd: no process killed' ];
    then
        break
    else
        echo $KILLALL 
        sleep 1
    fi
done

>&2 echo "update_backend.sh completed!"

#exit 0
#reboot
#shutdown -h now