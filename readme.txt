Run settings:



Deployment
  run custom remote command: killall update-to-debian; sleep 0.8
  Upload files via SFTP
    Incremental deployment: checked
    Ignore missing files: unchecked

  Custom Process step:
    Command: sh
    Arguments: deployUpdate.sh
    Working directory: %{sourceDir}

Run
  Use this command instead: checked
  Alternate executable on device: sh
  Arguments: update_real.sh -qws
  Working directory: /opt/camera
