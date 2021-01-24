__authors__ = "Susheel Varma and Vignesh Rammohan"

# Imports
import os
import sys
import shutil
import math
import time
import tempfile
import threading
from subprocess import call
from subprocess import Popen, PIPE

import soaplib
from soaplib.core.service import soap, rpc, DefinitionBase
from soaplib.core.model.primitive import String, Integer
from soaplib.core.model.clazz import ClassModel
from soaplib.core.server import wsgi
import storageInterface


# Constants
QSM_RECONSTRUCTION_RUN_FILE_PATH = r'/app/qsm_vph_recon/qsm_vph_recon'
QSM_RECONSTRUCTION_ERROR_FILE = r'/tmp/qsmreconstruction.stderr'
QSM_RECONSTRUCTION_OUTPUT_FILE = r'/tmp/qsmreconstruction.stdout'
QSM_RECONSTRUCTION_SCRATCH_FOLDER = r'/ephemeral/'
QSM_RECONSTRUCTION_TEMP_INPUT = r'/tmp/input/'
retry = 5

##############################################################################
# SOAP response definition

class ConvertorResponse(ClassModel):
    """Response object holds the commandline execution response"""
    statuscode = Integer
    command = String
    stdout = String
    stderr = String
    cwd = String
    result = String

    def __init__(self, command=None):
        self.command = command
        self.cwd = '.'
        self.statuscode = 0
        self.stdout = ""
        self.stderr = ""

##############################################################################
# Main threads

def start_qsmreconstruction_thread(mffeinputdirArg, mffeInputDir, t1inputdirArg, t1InputDir, outputimageArg, outputImage, meansusceptibilitytableArg, meanSusceptibilityTable, outputFolder):
    """copy files/from LOBCDER, start QSM Reconstruction process"""

    if os.path.isfile(QSM_RECONSTRUCTION_ERROR_FILE):
        os.remove(QSM_RECONSTRUCTION_ERROR_FILE)
    if os.path.isfile(QSM_RECONSTRUCTION_OUTPUT_FILE):
        os.remove(QSM_RECONSTRUCTION_OUTPUT_FILE)
    ferr = open( QSM_RECONSTRUCTION_ERROR_FILE,'w')
    fout = open( QSM_RECONSTRUCTION_OUTPUT_FILE,'w')

    try:   
        # normalize file names
        fout.write("normalize file names\n")
        # Note: mffeInputDir is actually a ZIP file, but later (var) localMffeInputDir will hold the corresponding DICOM files
        mffeInputDir = storageInterface.normalizePath( mffeInputDir)
        fout.write("mffeInputFile -- " + mffeInputDir + "\n")
        # Note: t1InputDir is actually a ZIP file, but later (var) localT1InputDir will hold the corresponding DICOM files
        t1InputDir = storageInterface.normalizePath( t1InputDir)
        fout.write("t1InputFile -- " + t1InputDir + "\n")
        outputFolder = storageInterface.normalizePath( outputFolder)
        if outputFolder[ len(outputFolder)-1 ]!='/':
            outputFolder = outputFolder + "/"
        fout.write("outputFolder -- " + outputFolder + "\n")    
    
        # create local folders and files
        fout.write("create local folders and files\n")
        subjectFolderName = mffeInputDir.split('/DICOM')[0]
        subjectFolderName = os.path.basename(subjectFolderName)
        subjectFolderName = subjectFolderName.replace('\ ',' ')
        fout.write("subjectFolderName --  " + subjectFolderName + "\n")
        localDataFolder = storageInterface.makeDirAndGetName(QSM_RECONSTRUCTION_SCRATCH_FOLDER, subjectFolderName )
        if localDataFolder[ len(localDataFolder)-1 ]!='/':
            localDataFolder = localDataFolder + "/"        
        localDataFolder = localDataFolder.replace(' ','\ ')
        fout.write("localDataFolder --  " + localDataFolder + "\n")
        if not os.path.exists(QSM_RECONSTRUCTION_TEMP_INPUT):
            os.makedirs(QSM_RECONSTRUCTION_TEMP_INPUT)
        subjectFolderName = subjectFolderName.replace(' ','_')
        localInputFolder = storageInterface.makeDirAndGetName(QSM_RECONSTRUCTION_TEMP_INPUT, subjectFolderName )
        if localInputFolder[ len(localInputFolder)-1 ]!='/':
            localInputFolder = localInputFolder + "/"        
        fout.write("localInputFolder --  " + localInputFolder + "\n")
        # mffe input
        localMffeInputFile = storageInterface.downloadFromURIAndGetLocalFilename(mffeInputDir, localInputFolder, retry , True)
        localMffeInputDir = os.path.join(localInputFolder, 'mFFE_QSM/DICOM/')
        if not os.path.exists(localMffeInputDir):
            os.makedirs(localMffeInputDir)
        fout.write("localMffeInputDir --  " + localMffeInputDir + "\n")
        call("unzip -j {0} -d {1}".format(localMffeInputFile, localMffeInputDir), shell=True)
        # T1 input
        localT1InputFile = storageInterface.downloadFromURIAndGetLocalFilename(t1InputDir, localInputFolder, retry , True)
        localT1InputDir = os.path.join(localInputFolder, 'T1_MPRAGE/DICOM/')
        if not os.path.exists(localT1InputDir):
            os.makedirs(localT1InputDir)
        fout.write("localT1InputDir --  " + localT1InputDir + "\n")
        call("unzip -j {0} -d {1}".format(localT1InputFile, localT1InputDir), shell=True)
        commLine = "{0} {1} {2} {3} {4} {5} {6} {7} {8}".format( 
                QSM_RECONSTRUCTION_RUN_FILE_PATH,
                mffeinputdirArg,
                localMffeInputDir,
                t1inputdirArg,
                localT1InputDir,
                outputimageArg,
                outputImage,
                meansusceptibilitytableArg,
                meanSusceptibilityTable)
                
        fout.write( "Running Command: " + commLine + "\n")         
        
        # close and re-open so that the output of call is written after all previous output
        fout.close()
        fout = open( QSM_RECONSTRUCTION_OUTPUT_FILE,'a')
        # Change the working directory
        os.chdir('/app/qsm_vph_recon')
        # finally do the call
        fail = call(commLine, shell=True, stdout=fout, stderr=ferr) 

        # check for failure
        if not fail:
            # copy output back to baseDir
            fout.write("copy output back to baseDir\n")
            ok = storageInterface.copyFrom( localDataFolder, outputFolder, retry, False)
            if not ok:
                ferr.write("failed to copy " + localDataFolder + " to " + outputFolder + "\n")
        else:
            ferr.write('failed to execute: ' +  commLine)
            
        # do some clean-up
        fout.write("do final clean-up\n")
        localDataFolder = localDataFolder.replace('\ ',' ')
        shutil.rmtree(localDataFolder)
        if os.path.exists(QSM_RECONSTRUCTION_TEMP_INPUT):
            shutil.rmtree(QSM_RECONSTRUCTION_TEMP_INPUT)
        
    except Exception as e:
        pass
        ferr.write("Exception: " + e.message + "\n")
        
    fout.close()
    ferr.close()  

##############################################################################
# QSM Reconstruction Service definition

class qsmReconstructionService(DefinitionBase):
    """class that implements the SOAP service to execute QSM Reconstruction"""
    responses_map = {}
    threads_map = {}

    @soap(String, String, _returns=String)
    def WSAsyncRun(self, operationName, parameterList):
        try:
        
            if operationName=='executeQSMReconstruction':
                # Parse parameters
                paramDict = {}
                for param in parameterList.split(","):
                    p = param.split("=")
                    paramDict[ str.strip(p[0]) ] =  str.strip(p[1])

                mffeinputdirArg = str.strip( paramDict['mffeinputdirArg'] )
                mffeInputDir = str.strip( paramDict['mffeInputDir'] )
                t1inputdirArg = str.strip( paramDict['t1inputdirArg'] )
                t1InputDir = str.strip( paramDict['t1InputDir'] )
                outputimageArg = str.strip( paramDict['outputimageArg'] )
                outputImage = str.strip( paramDict['outputImage'] )
                meansusceptibilitytableArg = str.strip( paramDict['meansusceptibilitytableArg'] )
                meanSusceptibilityTable = str.strip( paramDict['meanSusceptibilityTable'] )
                outputFolder = str.strip( paramDict['outputFolder'] )

                r = ConvertorResponse(operationName)
                r.cwd = QSM_RECONSTRUCTION_SCRATCH_FOLDER
                r.result = outputFolder

                # # start thread
                t = threading.Thread(target=start_qsmreconstruction_thread, args=(mffeinputdirArg, mffeInputDir, t1inputdirArg, t1InputDir, outputimageArg, outputImage, meansusceptibilitytableArg, meanSusceptibilityTable, outputFolder))
                t.start()
                self.threads_map[str(t.ident)] = t
                self.responses_map[str(t.ident)] = r
                return str(t.ident)

               
        # check if there was any error
        except Exception as e:
            pass
            return e.message


    @soap(String, _returns=String)
    def WSAsyncStatus(self, processID):
        try:
            if self.threads_map[processID].is_alive() == True:
                return "STATE_ACTIVE"
            else:
                return "STATE_FINISHED"
        except Exception as e:
            pass
            return "STATE_UNKNOWN"

    @soap(String, _returns=String)
    def WSAsyncResults(self, processID):
        r = ConvertorResponse()
        operationName = ''
        try:
            r = self.responses_map[processID]
            operationName = r.command
            if operationName=='executeQSMReconstruction':
                # read output file  
                if os.path.isfile(QSM_RECONSTRUCTION_OUTPUT_FILE):
                    r.stdout = open( QSM_RECONSTRUCTION_OUTPUT_FILE, 'r').read()
                r.stdout = r.stdout.replace('=',' ') # extra '=' are not allowed in the XML response
                # read error file
                if os.path.isfile(QSM_RECONSTRUCTION_ERROR_FILE):
                    r.stderr = open(QSM_RECONSTRUCTION_ERROR_FILE, 'r').read()
                r.stderr = r.stderr.replace('=',' ') # extra '=' are not allowed in the XML response
                # if there was no error, check the expected_output files are there
                if r.stderr!="":
                    r.statuscode = 1  ## Don't return here. The output files might be generated even if there was an error.
        except Exception as e:
            pass
            r.statuscode = 1
            r.result = e.message
        # output must be string, but resembling the ConvertorResponse originally returned by executeQSMReconstruction
        soapResponse =  'executeXXXResponse=<executeXXXResponse><executeXXXResult>\
<stdout>%s</stdout><cwd>%s</cwd><command>%s</command><result>%s</result><stderr>%s</stderr><statuscode>%s</statuscode>\
</executeXXXResult></executeXXXResponse>' % (r.stdout, r.cwd, r.command, r.result , r.stderr, str(r.statuscode) )
        soapResponse = soapResponse.replace('executeXXX', operationName)
        return soapResponse

    @soap(String, String, String, String, String, String, String, String, String, _returns=ConvertorResponse)
    def executeQSMReconstruction(self, mffeinputdirArg, mffeInputDir, t1inputdirArg, t1InputDir, outputimageArg, outputImage, meansusceptibilitytableArg, meanSusceptibilityTable, outputFolder):
        r = ConvertorResponse('executeQSMReconstruction')
        r.result = "ERROR: This service can only be executed in non-blocking mode"
        r.statuscode = 1;
        return r
        
##############################################################################

soap_app = soaplib.core.Application([qsmReconstructionService], 'qsmReconstruction', name='qsmReconstructionService')
application = wsgi.Application(soap_app)

if __name__=='__main__':
    try:
        from wsgiref.simple_server import make_server
        server = make_server(host='0.0.0.0', port=8080, app=application)
        server.serve_forever()
    except ImportError:
        print "Error: server code requires Python >= 2.5"
