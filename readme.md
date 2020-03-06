update-to-debian
====================
The purpose of this app is to provide a means to write an image to an SD card using only their camera, an SD card in the camera's top slot, and a USB stick from which to run the update. The writing of the image is done by the camera itself, not the user's computer.

Here is an overview of what the app does, step by step:
 * Before letting the user begin the update:
    - Check the integrity of the files on the USB stick.
    - Check the presence and capacity of the SD card in the camera's top slot. Don't allow the the write to start if it isn't there or its capacity is smaller than 4GB.
 * After the user has confirmed they want to update:
    - If the power management IC has not been updated yet, update it before writing the image to the SD card.
    - Uncompress the image and write it to the SD card in the top slot.
If the user presses the cancel button at any time, the camera will reboot back to the Arago system.

To make and test a new debian update zip file:
--------------------
 * Make a debian image archive, named debian.img.gz, using the chronos-updates repository. https://github.com/krontech/chronos-updates/
 * Copy the archive into the camUpdate folder in this repo.
 * run: ./make_update.sh
 * If testing the update from Qt Creator, copy the archive to the /opt/camera directory on the camera's SD card. This only has to be copied if you are using a new SD card or a new image.
 * If installing the update from a USB stick, copy the camUpdate folder to a FAT32 USB stick and start it from the Util screen, just like any other update on Arago.
 * Follow the directions onscreen to apply the update.



Run settings, intended for Qt Creator:
====================
Deployment
--------------------
 * Run custom remote command: killall update-to-debian; sleep 0.8
 * Custom Process step:
    - Command: cp
    - Arguments: %{buildDir}/update-to-debian %{sourceDir}/camUpdate/
    - Working directory:
 * Custom Process step:
    - Command: sh
    - Arguments: make_update.sh
    - Working directory: %{sourceDir}
 * Upload files via SFTP
    - Incremental deployment: checked
    - Ignore missing files: unchecked
 * Custom Process step:
    - Command: sh
    - Arguments: deploy_update.sh
    - Working directory: %{sourceDir}

Run
--------------------
 * Use this command instead: checked
 * Alternate executable on device: sh
 * Arguments: camUpdate/update_real.sh -qws
 * Working directory: /opt/camera



Other notes:
====================
When deploying from Qt Creator, you may see the messages that say "ssh_askpass: exec(/usr/bin/ssh-askpass): No such file or directory" in the Compile Output in qt creator. This doesn't seem to be a problem - the files still get copied anyway. The messages come from deploy_update.sh.
All files needed for the update will be copied automatically every time the update is run, except for the compressed SD card image, because of its size.
