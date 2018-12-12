#!/bin/busybox ash
set -e -o pipefail #fail fast

#May fail, probably because they already exist. That's OK.
mkfifo to-update-ui || true
mkfifo to-update-backend || true

>&2 echo "stopping camApp"
killall camApp || true

>&2 echo "starting update-to-debian"
./update_backend.sh < to-update-backend > to-update-ui &
update-to-debian -qws > to-update-backend < to-update-ui &
wait