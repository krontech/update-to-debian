Run settings:



Deployment
  Run custom remote command: killall update-to-debian; sleep 0.8

  Custom Process step:
    Command: cp
    Arguments:  %{buildDir}/update-to-debian %{sourceDir}/camUpdate/
    Working directory:

  Custom Process step:
    Command: sh
    Arguments: make_update.sh
    Working directory: %{sourceDir}

  Upload files via SFTP
    Incremental deployment: checked
    Ignore missing files: unchecked

  Custom Process step:
    Command: sh
    Arguments: deploy_update.sh
    Working directory: %{sourceDir}

Run
  Use this command instead: checked
  Alternate executable on device: sh
  Arguments: update_real.sh -qws
  Working directory: /opt/camera




Other notes:
You may see the messages that say "ssh_askpass: exec(/usr/bin/ssh-askpass): No such file or directory" in the Compile Output in qt creator. This doesn't seem to be a problem - the files still get copied anyway. The messages come from deploy_update.sh.
