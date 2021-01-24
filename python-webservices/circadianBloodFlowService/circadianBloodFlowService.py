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

import soaplib
from soaplib.core.service import soap, rpc, DefinitionBase
from soaplib.core.model.primitive import String, Integer
from soaplib.core.model.clazz import ClassModel
from soaplib.core.server import wsgi
import storageInterface
import shutil

# Constants

CIRCADIAN_BLOODFLOW_RUN_FILE_PATH = r'/app/CircadianBloodFlow/run_CircadianBloodflow.sh'
CIRCADIAN_BLOODFLOW_MATLAB_RUNTIME_PATH = r'/usr/local/MATLAB/MATLAB_Runtime/v85/'
CIRCADIAN_BLOODFLOW_ERROR_FILE = r'/tmp/circadianbloodflow.stderr'
CIRCADIAN_BLOODFLOW_OUTPUT_FILE = r'/tmp/circadianbloodflow.stdout'
CIRCADIAN_BLOODFLOW_SCRATCH_FOLDER = r'/ephemeral/'
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

def start_circadianbloodflow_thread(holterBPTimeSeriesFile, demographicsWeight, demographicsHeight, demographicsAge, demographicsIsFemale, lifestyleIsSmoker, lifestyleActivityStatus, cardiacUS_LVEjectionFraction, cardiacUS_LVEndDiastolicVolume, carotidUS_ICA_L_Diameter, carotidUS_ICA_R_Diameter, carotidUS_ICA_L_MinVelocity, carotidUS_ICA_R_MinVelocity, subjectID, outputFolder):
    """copy files/from LOBCDER, start CircadianBloodflow process.sh"""

    if os.path.isfile(CIRCADIAN_BLOODFLOW_ERROR_FILE):
        os.remove(CIRCADIAN_BLOODFLOW_ERROR_FILE)
    if os.path.isfile(CIRCADIAN_BLOODFLOW_OUTPUT_FILE):
        os.remove(CIRCADIAN_BLOODFLOW_OUTPUT_FILE)
        
    ferr = open( CIRCADIAN_BLOODFLOW_ERROR_FILE,'w')
    fout = open( CIRCADIAN_BLOODFLOW_OUTPUT_FILE,'w')

    try:   
        # normalize file names
        fout.write("normalize file names\n")
        holterBPTimeSeriesFile = storageInterface.normalizePath( holterBPTimeSeriesFile)
        fout.write("holterBPTimeSeriesFile -- " + holterBPTimeSeriesFile + "\n") 
        outputFolder = storageInterface.normalizePath(outputFolder)
        if outputFolder[ len(outputFolder)-1 ]!='/':
            outputFolder = outputFolder + "/"
        fout.write("outputFolder -- " + outputFolder + "\n")    
    
        # create local folders and files
        fout.write("create local folders and files\n")
        if subjectID=="" or subjectID==None:
            subjectFolderName = os.path.basename(holterBPTimeSeriesFile)
            subjectFolderName = subjectFolderName.split('.')[0]
        else:
            subjectFolderName = subjectID
        fout.write("subjectFolderName --  " + subjectFolderName + "\n")
        localDataFolder = storageInterface.makeDirAndGetName(CIRCADIAN_BLOODFLOW_SCRATCH_FOLDER, subjectFolderName )
        if localDataFolder[ len(localDataFolder)-1 ]!='/':
            localDataFolder = localDataFolder + "/"        
        fout.write("localDataFolder --  " + localDataFolder + "\n")
        localHolterBPTimeSeriesFile = storageInterface.downloadFromURIAndGetLocalFilename(holterBPTimeSeriesFile, localDataFolder, retry , False)
        fout.write("localHolterBPTimeSeriesFile --  " + localHolterBPTimeSeriesFile + "\n")  
        
        commLine = "{0} {1} {2} {3} {4} {5} {6} {7} {8} {9} {10} {11} {12} {13} {14} {15} {16}".format( 
                CIRCADIAN_BLOODFLOW_RUN_FILE_PATH,
                CIRCADIAN_BLOODFLOW_MATLAB_RUNTIME_PATH,
                localHolterBPTimeSeriesFile,
                demographicsWeight,
                demographicsHeight,
                demographicsAge,
                demographicsIsFemale,
                lifestyleIsSmoker,
                lifestyleActivityStatus,
                cardiacUS_LVEjectionFraction,
                cardiacUS_LVEndDiastolicVolume,
                carotidUS_ICA_L_Diameter,
                carotidUS_ICA_R_Diameter,
                carotidUS_ICA_L_MinVelocity,
                carotidUS_ICA_R_MinVelocity,
                subjectID,
                localDataFolder )        
                
        fout.write( "Running Command: " + commLine + "\n")         
        
        # close and re-open so that the output of call is written after all previous output
        fout.close()
        fout = open( CIRCADIAN_BLOODFLOW_OUTPUT_FILE,'a')
        
        # finally do the call
        fail = call(commLine, shell=True, stdout=fout, stderr=ferr) 

        # check for failure
        if not fail:
            # copy output back to baseDir
            fout.write("copy output back to baseDir\n")
            for file in os.listdir(localDataFolder):
                ok = storageInterface.copyFrom( localDataFolder + file , outputFolder + file, retry, True)
                if not ok:
                    ferr.write("failed to copy " + localDataFolder + file + " to " + outputFolder + file + "\n")
        else:
            ferr.write('failed to execute: ' +  commLine)
            
        # do some clean-up
        fout.write("do final clean-up\n")
        shutil.rmtree(localDataFolder)
        
    except Exception as e:
        pass
        ferr.write("Exception: " + e.message + "\n")
        
    fout.close()
    ferr.close()  

##############################################################################
# Circadian Blood Flow Service definition

class circadianBloodFlowService(DefinitionBase):
    """class that implements the SOAP service to execute CircadianBloodFlow"""
    responses_map = {}
    threads_map = {}

    @soap(String, String, _returns=String)
    def WSAsyncRun(self, operationName, parameterList):
        try:
        
            if operationName=='executeCircadianBloodFlow':
                # Parse parameters
                paramDict = {}
                for param in parameterList.split(","):
                    p = param.split("=")
                    paramDict[ str.strip(p[0]) ] =  str.strip(p[1])

                holterBPTimeSeriesFile = str.strip( paramDict['holterBPTimeSeriesFile'] )
                demographicsWeight = str.strip( paramDict['demographicsWeight'] )
                demographicsHeight = str.strip( paramDict['demographicsHeight'] )
                demographicsAge = str.strip( paramDict['demographicsAge'] )
                demographicsIsFemale = str.strip( paramDict['demographicsIsFemale'] )
                lifestyleIsSmoker = str.strip( paramDict['lifestyleIsSmoker'] )
                lifestyleActivityStatus = str.strip( paramDict['lifestyleActivityStatus'] )
                cardiacUS_LVEjectionFraction = str.strip( paramDict['cardiacUS_LVEjectionFraction'] )
                cardiacUS_LVEndDiastolicVolume = str.strip( paramDict['cardiacUS_LVEndDiastolicVolume'] )
                carotidUS_ICA_L_Diameter = str.strip( paramDict['carotidUS_ICA_L_Diameter'] )
                carotidUS_ICA_R_Diameter = str.strip( paramDict['carotidUS_ICA_R_Diameter'] )
                carotidUS_ICA_L_MinVelocity = str.strip( paramDict['carotidUS_ICA_L_MinVelocity'] )
                carotidUS_ICA_R_MinVelocity = str.strip( paramDict['carotidUS_ICA_R_MinVelocity'] )
                subjectID = str.strip( paramDict['subjectID'] )
                outputFolder = str.strip( paramDict['outputFolder'] )

                r = ConvertorResponse(operationName)
                r.cwd = CIRCADIAN_BLOODFLOW_SCRATCH_FOLDER
                r.result = outputFolder

                # # start thread
                t = threading.Thread(target=start_circadianbloodflow_thread, args=(holterBPTimeSeriesFile, 
                demographicsWeight, demographicsHeight, demographicsAge, demographicsIsFemale, lifestyleIsSmoker, lifestyleActivityStatus, cardiacUS_LVEjectionFraction, cardiacUS_LVEndDiastolicVolume, carotidUS_ICA_L_Diameter, carotidUS_ICA_R_Diameter, carotidUS_ICA_L_MinVelocity, carotidUS_ICA_R_MinVelocity, subjectID, outputFolder))
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
            if operationName=='executeCircadianBloodFlow':
                # read output file  
                if os.path.isfile(CIRCADIAN_BLOODFLOW_OUTPUT_FILE):
                    r.stdout = open( CIRCADIAN_BLOODFLOW_OUTPUT_FILE, 'r').read()
                r.stdout = r.stdout.replace('=',' ') # extra '=' are not allowed in the XML response
                if r.stdout.find('[H[2J') != -1:
                    r.stdout = r.stdout.replace('[H[2J','') # These strange chracters have to be removed to avoid XML parse error
                    fileWrite = open( CIRCADIAN_BLOODFLOW_OUTPUT_FILE, 'w')
                    fileWrite.write(r.stdout)
                    fileWrite.close()
                # read error file
                if os.path.isfile(CIRCADIAN_BLOODFLOW_ERROR_FILE):
                    r.stderr = open( CIRCADIAN_BLOODFLOW_ERROR_FILE, 'r').read()
                r.stderr = r.stderr.replace('=',' ') # extra '=' are not allowed in the XML response
                # if there was no error, check the expected_output files are there
                if r.stderr!="":
                    r.statuscode = 1  ## Don't return here. The output files might be generated even if there was an error.
        except Exception as e:
            pass
            r.statuscode = 1
            r.result = e.message
        # output must be string, but resembling the ConvertorResponse originally returned by executeCircadianBloodFlow
        soapResponse =  'executeXXXResponse=<executeXXXResponse><executeXXXResult>\
<stdout>%s</stdout><cwd>%s</cwd><command>%s</command><result>%s</result><stderr>%s</stderr><statuscode>%s</statuscode>\
</executeXXXResult></executeXXXResponse>' % (r.stdout, r.cwd, r.command, r.result , r.stderr, str(r.statuscode) )
        soapResponse = soapResponse.replace('executeXXX', operationName)
        return soapResponse

    @soap(String, String, String, String, String, String, String, String, String, String, String, String, String, String, String, _returns=ConvertorResponse)
    def executeCircadianBloodFlow(self, holterBPTimeSeriesFile, 
                demographicsWeight, demographicsHeight, demographicsAge, demographicsIsFemale, lifestyleIsSmoker, lifestyleActivityStatus, cardiacUS_LVEjectionFraction, cardiacUS_LVEndDiastolicVolume, carotidUS_ICA_L_Diameter, carotidUS_ICA_R_Diameter, carotidUS_ICA_L_MinVelocity, carotidUS_ICA_R_MinVelocity, subjectID, outputFolder):
        r = ConvertorResponse('executeCircadianBloodFlow')
        r.result = "ERROR: This service can only be executed in non-blocking mode"
        r.statuscode = 1;
        return r
        
##############################################################################

soap_app = soaplib.core.Application([circadianBloodFlowService], 'circadianBloodFlow', name='circadianBloodFlowService')
application = wsgi.Application(soap_app)

if __name__=='__main__':
    try:
        from wsgiref.simple_server import make_server
        server = make_server(host='0.0.0.0', port=8082, app=application)
        server.serve_forever()
    except ImportError:
        print "Error: server code requires Python >= 2.5"
