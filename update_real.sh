#!/bin/busybox ash

#May fail, probably because they already exist. That's OK.
mkfifo to-update-ui || true
mkfifo to-update-backend || true

>&2 echo "stopping camApp"
killall camApp || true
sleep 0.8

>&2 echo "starting update-to-debian"
./update_backend.sh < to-update-backend > to-update-ui &
update-to-debian -qws > to-update-backend < to-update-ui &

echo "before sleep"
sleep 5
echo "after sleep"
echo "WaitForUserInput" > to-update-ui
wait