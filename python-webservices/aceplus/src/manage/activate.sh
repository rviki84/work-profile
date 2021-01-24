#!/bin/bash
#######################################
## "Configure SOAPLib to autostart"
sudo cat /webapp/aceplus/manage/initd.aceplus > /etc/init.d/aceplus
sudo chmod +x /etc/init.d/aceplus
sudo chkconfig --add aceplus 
#######################################
## "Starting application"
sudo chkconfig aceplus on
