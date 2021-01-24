#!/usr/bin/env bash

# Install Pre-requisites
sudo apt-get update
sudo apt-get install wget python python-dev python-pip libxml2-dev libxslt1-dev

pip install -r manage/requirements.txt
