#!/bin/bash
#######################################
## "Configure SOAPLib to autostart circadianBloodFlowService"
sudo cat /app/manage/initd.circadianBloodFlowService > /etc/init.d/circadianBloodFlowService
sudo chmod +x /etc/init.d/circadianBloodFlowService
sudo update-rc.d circadianBloodFlowService defaults
#######################################
## "Starting circadianBloodFlowService application"
sudo service circadianBloodFlowService start
