#!/bin/busybox ash

>&2 echo "backend"

CONTINUE=1


while [ CONTINUE ];
do
    >&2 echo loop start
    read -r -t 1 LINE
    >&2 echo $LINE
    
    if [ $LINE = "Syscheck" ];
    then #UI has advanced to second screen; check presence of SD card and readiness of update files on USB
        string=$(ls /dev | grep mmcblk1)
        #echo >&2 string $string
        if [[ "$string" = '' ]]; then
            echo NoSDPresent
            >&2 echo NoSDPresent
        else
            echo "WaitForUserInput"
            >&2 echo "WaitForUserInput"
        fi
    fi
    
    if [ $LINE = "Start" ];
    then #
        echo "start dd here"
    fi
    
    >&2 echo loop finish
    echo ""
    sleep .1
done

#echo "update_real.sh completed!"
#>&2 echo "update_real.sh completed!"

#./update_backend.sh &

#exit 0
#reboot
#shutdown -h now