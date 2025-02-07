DEPLOYDIR=/opt/camera/camUpdate
echo deploydir is $DEPLOYDIR
echo working dir: $PWD
scp1 -oKexAlgorithms=+diffie-hellman-group1-sha1  "camUpdate/update_real.sh"  root@192.168.12.1:$DEPLOYDIR/;
scp1 -oKexAlgorithms=+diffie-hellman-group1-sha1  "camUpdate/update_backend.sh"  root@192.168.12.1:$DEPLOYDIR/;
scp1 -oKexAlgorithms=+diffie-hellman-group1-sha1  "camUpdate/update.md5sum"  root@192.168.12.1:$DEPLOYDIR/;
scp1 -oKexAlgorithms=+diffie-hellman-group1-sha1  "camUpdate/update.sh"  root@192.168.12.1:$DEPLOYDIR/;
scp1 -oKexAlgorithms=+diffie-hellman-group1-sha1  "camUpdate/Chronos1_4PowerController.X.production.hex"  root@192.168.12.1:$DEPLOYDIR/;
echo "deployUpdate.sh finished"
