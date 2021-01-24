#!/bin/bash
#######################################
## "Configure SOAPLib to autostart qsmReconstructionService"
sudo cat /app/manage/initd.qsmReconstructionService > /etc/init.d/qsmReconstructionService
sudo chmod +x /etc/init.d/qsmReconstructionService
sudo update-rc.d qsmReconstructionService defaults
#######################################
## "Starting qsmReconstructionService application"
sudo service qsmReconstructionService start
