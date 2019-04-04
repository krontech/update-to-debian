DEPLOYDIR=/opt/camera
echo deploydir is $DEPLOYDIR
echo working dir: $PWD
scp -oKexAlgorithms=+diffie-hellman-group1-sha1  "update_real.sh"  root@192.168.12.1:$DEPLOYDIR/;
scp -oKexAlgorithms=+diffie-hellman-group1-sha1  "update_backend.sh"  root@192.168.12.1:$DEPLOYDIR/;
echo "deployUpdate.sh finished"
