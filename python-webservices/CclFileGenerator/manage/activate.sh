#!/bin/bash
#######################################
## "Configure SOAPLib to autostart"
sudo cat /webapp/CclFileGenerator/manage/initd.CclFileGenerator > /etc/init.d/CclFileGenerator
sudo chmod +x /etc/init.d/CclFileGenerator
sudo update-rc.d CclFileGenerator defaults
#######################################
## "Starting application"
sudo service CclFileGenerator start
