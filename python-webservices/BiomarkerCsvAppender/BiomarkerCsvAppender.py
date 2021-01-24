__author__ = 'Ernesto Coto'

import argparse
import sys
import os
from subprocess import call
import xml.etree.ElementTree as ET
import shutil as sh
import math
import time

# constants
OUTPUT_FILE_NAME = "biomarkers.csv"
localBaseDir = "/tmp/"
retry = 5

def makeDirAndGetName(base, suffix):
    localDir = os.path.join(base, suffix)
    if not os.path.exists(localDir):
        try:
            os.makedirs(localDir)
        except:
            pass

    localDir = localDir.replace(' ', '\ ')
    return localDir
    
def getLocalFilenameFrom(uri,dstPath):
    """get corrected local path from external uri"""
    subjectImageBaseName = os.path.basename(uri)
    return os.path.join(dstPath, subjectImageBaseName)
    
def waitExpBackoff(c):
    """return for c trials exp back off time to wait. Return n secs"""
    return int(round(((math.pow(2.0,float(c)) - 1.0) / (2.0))))
    
def normalizePath(path):
    """people can use lobcder uri"""
    token = path
    if path.startswith("s3://"):
        token = path
        print "s3 prefix path, so is a s3 uri " + path

    elif path.startswith("lobcder:/"):
        token = os.path.join(clpPath, "media/lobcder", (path.split(":/"))[-1])
        print "normalize lobcder uri " + path + " to " + token

    return token

def copyFrom(src,to, nRetries, isFile=False):
    """copy src to dst with <nRetries> times and 
        return True if not failed
    """
    source = src.rstrip(os.path.sep)
    too = to.rstrip(os.path.sep)
    for i in xrange(nRetries):
        wait = waitExpBackoff(i)
        time.sleep(wait)
        cmd = None

        if not too.endswith(os.path.sep) and not isFile:
            too = os.path.join(too + os.path.sep )
            
        if source.startswith("s3://") or to.startswith("s3://"):
            cmd = "/usr/bin/s3cmd sync --no-check-md5 -c /home/dare/.s3cfg {0} {1}".format(source,too)
        else:
            cmd = "/usr/bin/rsync --timeout=60 -qvrt --size-only {0} {1}".format(source,too )


        print "calling command " + cmd
        fail = call(cmd, shell=True)
        if not fail:
            return True
            
def downloadFromURIAndGetLocalFilename(uri, to, retry, isDestinationFile=True):
    # copy sourceimg to local subject directory
    localName = None
    print "trying to copy file from " + uri + " to this path " + to
    ok = copyFrom(uri, to,retry, isDestinationFile)
    if not ok:
        print "copied 0 files!"
        return localName

    localName = getLocalFilenameFrom(uri, to)
    return localName

try: 
    # command-line parsing
    parser = argparse.ArgumentParser()
    parser.add_argument('baseDir_subjectId', help='Fully qualified path to the root output folder (comma) subjectId')
    parser.add_argument('-o', help='(optional) Dummy parameter for web service wrapping')
    parser._optionals.title = "flag arguments"
    args = parser.parse_args()
    if args.baseDir_subjectId==None:
        exit(1)
        
    # parameter adjustments
    baseDir = args.baseDir_subjectId.split(",")[0]
    if baseDir[ len(baseDir)-1 ] != os.path.sep:
        baseDir = baseDir + os.path.sep
    subjectId = args.baseDir_subjectId.split(",")[1]
    subjCsvFile = baseDir + subjectId + os.path.sep + OUTPUT_FILE_NAME

    # get input file and copy it locally
    subjCsvFile = normalizePath( subjCsvFile)
    localSubjectDir = makeDirAndGetName(localBaseDir, subjectId) # make sure dir exists
    subjCsvFile = subjCsvFile.replace(' ', '\ ') # scape spaces 
    subjCsvFileLocalPath = downloadFromURIAndGetLocalFilename(subjCsvFile, localSubjectDir, retry , False)
        
    # read local file
    lines = []
    with open( subjCsvFileLocalPath.replace('\ ', ' '), 'r') as inFile: # revert scape spaces
        content = inFile.read()
        lines = content.split('\n')
        lines[0] = lines[0].replace('"','')
        lines[0] = lines[0].replace(', ',', Structural Volumes|')
        lines[0] = 'SubjectId, Structural Volumes|' + lines[0] + ', Source path'
        lines[1] = subjectId.replace('\ ', ' ') + ', ' + lines[1] + ', ' + subjCsvFile
        
    # get workflow file, if present
    workflowCsvFile = baseDir + OUTPUT_FILE_NAME
    workflowCsvFileLocalPath = localBaseDir + OUTPUT_FILE_NAME
    downloadFromURIAndGetLocalFilename(workflowCsvFile, localBaseDir, retry, False )
    if not os.path.exists(workflowCsvFileLocalPath):
        with open( workflowCsvFileLocalPath, 'w') as outFile:
            outFile.write(lines[0] + "\n")
            outFile.write(lines[1] + "\n")
    else:        
        with open( workflowCsvFileLocalPath, 'a') as outFile:
            outFile.write(lines[1] + "\n")

    # copy output back to baseDir
    ok = copyFrom( workflowCsvFileLocalPath, workflowCsvFile, retry, True)
    if not ok:
        print "failed to copy output file"

    # clean up
    os.remove(workflowCsvFileLocalPath)
    sh.rmtree(localSubjectDir.replace('\ ', ' '),ignore_errors=True)    

except Exception as e:
   pass
   print e.message
