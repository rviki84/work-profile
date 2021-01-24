#!/usr/bin/python

#
# Webservice: To run the Philips Subcortical Brain Segmentation CLP(in Gimias)
#
# (c) 2016, CISTIB, The University of Sheffield
#

__authors__ = "Vignesh Rammohan"

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
BRAIN_SEGMENTATION_RUN_FILE_PATH = r'/app/prh_brainsegmentationCLP'
BRAIN_SEGMENTATION_ERROR_FILE = r'/tmp/brainsegmentation.stderr'
BRAIN_SEGMENTATION_OUTPUT_FILE = r'/tmp/brainsegmentation.stdout'
BRAIN_SEGMENTATION_SCRATCH_FOLDER = r'/ephemeral/'
BRAIN_SEGMENTATION_TEMP_INPUT = r'/tmp/input/'
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

def start_brainSegmentation_thread(subjectId, input, outputLabels, outputStructureVolumes, outputFolder, additionalTemplateLabels=None):
    """copy files from S3, start Philips Brain Segmentation process"""

    if os.path.isfile(BRAIN_SEGMENTATION_ERROR_FILE):
        os.remove(BRAIN_SEGMENTATION_ERROR_FILE)
    if os.path.isfile(BRAIN_SEGMENTATION_OUTPUT_FILE):
        os.remove(BRAIN_SEGMENTATION_OUTPUT_FILE)
    ferr = open( BRAIN_SEGMENTATION_ERROR_FILE,'w')
    fout = open( BRAIN_SEGMENTATION_OUTPUT_FILE,'w')

    try:   
        # normalize file names
        fout.write("normalize file names\n")
        input = storageInterface.normalizePath( input)
        fout.write("input -- " + input + "\n")
        if additionalTemplateLabels is not None:
            additionalTemplateLabels = storageInterface.normalizePath( additionalTemplateLabels)
            fout.write("additionalTemplateLabels -- " + additionalTemplateLabels + "\n")
        else:
            fout.write("additionalTemplateLabels -- No additional template label is provided." + "\n")

        outputFolder = storageInterface.normalizePath( outputFolder)
        if outputFolder[ len(outputFolder)-1 ]!='/':
            outputFolder = outputFolder + "/"
        fout.write("outputFolder -- " + outputFolder + "\n")

        # create local folders and files
        fout.write("create local folders and files\n")
        #subjectFolderName = input.split('/Imaging')[0]
        #subjectFolderName = os.path.basename(subjectFolderName)
        #subjectFolderName = subjectFolderName.replace('\ ',' ')
        subjectFolderName = subjectId.replace(' ','_') # replace problematic spaces and restore at the end
        fout.write("subjectFolderName --  " + subjectFolderName + "\n")
        localDataFolder = storageInterface.makeDirAndGetName(BRAIN_SEGMENTATION_SCRATCH_FOLDER, subjectFolderName )
        if localDataFolder[ len(localDataFolder)-1 ]!='/':
            localDataFolder = localDataFolder + "/"        
        #localDataFolder = localDataFolder.replace(' ','\ ')
        fout.write("localDataFolder --  " + localDataFolder + "\n")
        if not os.path.exists(BRAIN_SEGMENTATION_TEMP_INPUT):
            os.makedirs(BRAIN_SEGMENTATION_TEMP_INPUT)

        localInputFolder = storageInterface.makeDirAndGetName(BRAIN_SEGMENTATION_TEMP_INPUT, subjectFolderName )
        if localInputFolder[ len(localInputFolder)-1 ]!='/':
            localInputFolder = localInputFolder + "/"        
        fout.write("localInputFolder --  " + localInputFolder + "\n")
        # T1 input image download
        localT1InputImageFile = storageInterface.downloadFromURIAndGetLocalFilename(input, localInputFolder, retry , True)
        fout.write("localT1InputImageFile --  " + localT1InputImageFile + "\n")
        # Additional Template Label download, if provided
        if additionalTemplateLabels is not None:    
            localAddtlTempLblFile = storageInterface.downloadFromURIAndGetLocalFilename(additionalTemplateLabels, localInputFolder, retry , True)
            fout.write("localAddtlTempLblFile --  " + localAddtlTempLblFile + "\n")

        if additionalTemplateLabels is not None:
            commLine = "{0} --input {1} --outputlabels {2}{3} --outputstructurevolumes {4}{5} --additionaltemplatelabels {6}".format( 
                    BRAIN_SEGMENTATION_RUN_FILE_PATH,
                    localT1InputImageFile,
                    localDataFolder,
                    outputLabels,
                    localDataFolder,
                    outputStructureVolumes,
                    localAddtlTempLblFile)
        else:
            commLine = "{0} --input {1} --outputlabels {2}{3} --outputstructurevolumes {4}{5}".format( 
                    BRAIN_SEGMENTATION_RUN_FILE_PATH,
                    localT1InputImageFile,
                    localDataFolder,
                    outputLabels,
                    localDataFolder,
                    outputStructureVolumes)

        fout.write( "Running Command: " + commLine + "\n")         
        
        # close and re-open so that the output of call is written after all previous output
        fout.close()
        fout = open( BRAIN_SEGMENTATION_OUTPUT_FILE,'a')
        # finally do the call
        fail = call(commLine, shell=True, stdout=fout, stderr=ferr) 

        # check for failure
        if not fail:
            # copy output back to baseDir
            fout.write("copy output back to baseDir\n")
            if subjectFolderName != subjectId:
                fout.write("Changing folder " + BRAIN_SEGMENTATION_SCRATCH_FOLDER + subjectFolderName + " to " + BRAIN_SEGMENTATION_SCRATCH_FOLDER + subjectId + "\n")
                os.rename(BRAIN_SEGMENTATION_SCRATCH_FOLDER + subjectFolderName, BRAIN_SEGMENTATION_SCRATCH_FOLDER + subjectId) 
                subjectFolderName = subjectId
            ok = storageInterface.copyFrom( BRAIN_SEGMENTATION_SCRATCH_FOLDER + subjectFolderName.replace(' ', '\ ') + '/' , outputFolder, retry, False)
            if not ok:
                ferr.write("failed to copy " + BRAIN_SEGMENTATION_SCRATCH_FOLDER + subjectFolderName.replace(' ', '\ ') + " to " + outputFolder + "\n")
        else:
            ferr.write('failed to execute: ' +  commLine)
            
        # do some clean-up
        fout.write("do final clean-up\n")
        #localDataFolder = localDataFolder.replace('\ ',' ')
        shutil.rmtree(BRAIN_SEGMENTATION_SCRATCH_FOLDER + subjectFolderName + '/')
        if os.path.exists(BRAIN_SEGMENTATION_TEMP_INPUT):
            shutil.rmtree(BRAIN_SEGMENTATION_TEMP_INPUT)
        
    except Exception as e:
        pass
        ferr.write("Exception: " + e.message + "\n")
        
    fout.close()
    ferr.close()  

##############################################################################
# Brain Segmentation Service definition

class brainSegmentationService(DefinitionBase):
    """class that implements the SOAP service to execute Philips Brain Segmentation process"""
    responses_map = {}
    threads_map = {}

    @soap(String, String, _returns=String)
    def WSAsyncRun(self, operationName, parameterList):
        try:
        
            if operationName=='executeBrainSegmentation':
                # Parse parameters
                paramDict = {}
                for param in parameterList.split(","):
                    p = param.split("=")
                    paramDict[ str.strip(p[0]) ] =  str.strip(p[1])

                subjectId = str.strip( paramDict['subjectId'] ) 
                input = str.strip( paramDict['input'] )
                outputLabels = str.strip( paramDict['outputLabels'] )
                outputStructureVolumes = str.strip( paramDict['outputStructureVolumes'] )
                outputFolder = str.strip( paramDict['outputFolder'] )
                try:
                    additionalTemplateLabels = str.strip( paramDict['additionalTemplateLabels'] )
                except KeyError as e:
                    additionalTemplateLabels = None

                r = ConvertorResponse(operationName)
                r.cwd = BRAIN_SEGMENTATION_SCRATCH_FOLDER
                r.result = outputFolder

                # # start thread
                t = threading.Thread(target=start_brainSegmentation_thread, args=(subjectId, input, outputLabels, outputStructureVolumes, outputFolder, additionalTemplateLabels))
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
            if operationName=='executeBrainSegmentation':
                # read output file  
                if os.path.isfile(BRAIN_SEGMENTATION_OUTPUT_FILE):
                    r.stdout = open( BRAIN_SEGMENTATION_OUTPUT_FILE, 'r').read()
                r.stdout = r.stdout.replace('=',' ') # extra '=' are not allowed in the XML response
                # read error file
                if os.path.isfile(BRAIN_SEGMENTATION_ERROR_FILE):
                    r.stderr = open(BRAIN_SEGMENTATION_ERROR_FILE, 'r').read()
                r.stderr = r.stderr.replace('=',' ') # extra '=' are not allowed in the XML response
                # if there was no error, check the expected_output files are there
                if r.stderr!="":
                    r.statuscode = 1  ## Don't return here. The output files might be generated even if there was an error.
        except Exception as e:
            pass
            r.statuscode = 1
            r.result = e.message
        # output must be string, but resembling the ConvertorResponse originally returned by executeBrainSegmentation
        soapResponse =  'executeXXXResponse=<executeXXXResponse><executeXXXResult>\
<stdout>%s</stdout><cwd>%s</cwd><command>%s</command><result>%s</result><stderr>%s</stderr><statuscode>%s</statuscode>\
</executeXXXResult></executeXXXResponse>' % (r.stdout, r.cwd, r.command, r.result , r.stderr, str(r.statuscode) )
        soapResponse = soapResponse.replace('executeXXX', operationName)
        return soapResponse

    @soap(String, String, String, String, String, String, _returns=ConvertorResponse)
    def executeBrainSegmentation(self, subjectId, input, outputLabels, outputStructureVolumes, outputFolder, additionalTemplateLabels):
        r = ConvertorResponse('executeBrainSegmentation')
        r.result = "ERROR: This service can only be executed in non-blocking mode"
        r.statuscode = 1;
        return r
        
##############################################################################

soap_app = soaplib.core.Application([brainSegmentationService], 'brainSegmentation', name='PhilipsBrainSegmentationService')
application = wsgi.Application(soap_app)

if __name__=='__main__':
    try:
        from wsgiref.simple_server import make_server
        server = make_server(host='0.0.0.0', port=8080, app=application)
        server.serve_forever()
    except ImportError:
        print "Error: server code requires Python >= 2.5"
