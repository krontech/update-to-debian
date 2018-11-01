#! /bin/sh
# Script to perform software update for Chronos 1.4 camera
#
# Authors: David Kronstein, Kron Technologies Inc.
#          Matthew Peters, Kron Technologies Inc.
#          Simon Kronstein, Kron Technologies Inc.
#

echo "Stopping camApp"
#killall camApp

echo "starting update-to-debian"
#"/usr/bin/update-to-debian -qws" &

pipe=/tmp/status.pipe
trap "rm -f $pipe" EXIT

if [[ ! -p $pipe ]]; then
    mkfifo $pipe
fi


#start multistrap/debootstrap here

echo "update_real.sh completed!"

#exit 0
#reboot
