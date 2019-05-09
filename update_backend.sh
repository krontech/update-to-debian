#!/bin/busybox ash

>&2 echo "update_backend.sh"

CONTINUE=1


while [ CONTINUE ];
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
        echo "start-dd-here"
    fi
    
    >&2 echo loop finish
    >&2 echo ""
done

#>&2 echo "update_real.sh completed!"

#exit 0
#reboot
#shutdown -h now