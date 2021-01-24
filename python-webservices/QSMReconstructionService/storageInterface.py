import sys, os, time
import math
from subprocess import call

def makeDirAndGetName(base, suffix):
    localDir = os.path.join(base, suffix)
    if not os.path.exists(localDir):
        try:
            os.makedirs(localDir)
        except:
            pass

    return localDir

def getLocalFilenameFrom(uri,dstPath):
    """get corrected local path from external uri"""
    subjectImageBaseName = os.path.basename(uri)
    return os.path.join(dstPath, subjectImageBaseName)

def waitExpBackoff(c):
    """return for c trials exp back off time to wait. Return n secs"""
    return int(round(((math.pow(2.0,float(c)) - 1.0) / (2.0))))

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
        
        if too[ len(too)-1 ]!='/' and not isFile:
            too = os.path.join(too + os.path.sep)
            
        if source.startswith("s3://") or to.startswith("s3://"):
            cmd = "s3cmd sync -c /root/.s3cfg --no-check-md5 {0} {1}".format(source,too)
        else:
            cmd = "rsync --timeout=60 -qvrt --size-only {0} {1}".format(source, too )
        
        fail = call(cmd, shell=True)
        if not fail:
            return True

        return False

def normalizePath(path):
    """people can use lobcder uri"""
    token = path
    if path.startswith("s3://"):
        token = path
    elif path.startswith("lobcder:/"):
        token = os.path.join(clpPath, "media/lobcder", (path.split(":/"))[-1])

    return token

def downloadFromURIAndGetLocalFilename(uri, to, retry, isDestinationFile=True):
    # copy sourceimg to local subject directory
    localName = None
    ok = copyFrom(uri, to,retry, isDestinationFile)
    if not ok:
        return localName

    localName = getLocalFilenameFrom(uri, to)
    return localName