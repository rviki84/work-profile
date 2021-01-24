#!/bin/bash
#######################################
## "Configure SOAPLib to autostart philipsBrainSegService"
sudo cat /app/manage/initd.philipsBrainSegService > /etc/init.d/philipsBrainSegService
sudo chmod +x /etc/init.d/philipsBrainSegService
sudo update-rc.d philipsBrainSegService defaults
#######################################
## "Starting philipsBrainSegService application"
sudo service philipsBrainSegService start
