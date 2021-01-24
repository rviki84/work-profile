__author__ = 'Ernesto Coto'

import soaplib
import os
from soaplib.core.service import soap, rpc, DefinitionBase
from soaplib.core.model.primitive import String, Integer
from soaplib.core.model.clazz import ClassModel
from soaplib.core.server import wsgi
from subprocess import Popen, PIPE
from subprocess import call
import threading
import time

ACEPLUS_ERROR_FILE = '/tmp/aceplusError'
ACEPLUS_OUTPUT_FILE = '/tmp/aceplusOutput'

def start_aceplus_thread(command_line):
    os.environ['PAM_LMD_LICENSE_FILE']="27000@cistib-xnat.shef.ac.uk"
    os.environ['LD_LIBRARY_PATH']="/esi_ace_plus/2014.1/UTILS/lib"
    os.environ['MANPATH']="/esi_ace_plus/DTF/man"
    os.environ['DTF_HOME']="/esi_ace_plus/DTF"
    os.environ['PATH']="/esi_ace_plus/DTF/bin:/esi_ace_plus/2014.1/UTILS/bin:/usr/lib64/qt-3.3/bin:/usr/local/bin:/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/sbin:/home/guest/bin"
    os.environ['ESI_HOME']="/esi_ace_plus"
    # convert into list and remove extra blanks
    splitted = command_line.split(" ")
    filtered = filter(lambda a: a != '', splitted) 
    # execute and capture standard error and output    
    p = Popen(filtered, stdin=PIPE, stdout=PIPE, stderr=PIPE)
    output, err = p.communicate()
    if os.path.isfile(ACEPLUS_ERROR_FILE):
        os.remove(ACEPLUS_ERROR_FILE)
    if os.path.isfile(ACEPLUS_OUTPUT_FILE):
        os.remove(ACEPLUS_OUTPUT_FILE)    
    f = open( ACEPLUS_ERROR_FILE,'w')
    f.write(err)
    f.close()
    f = open( ACEPLUS_OUTPUT_FILE,'w')
    f.write(output)
    f.close()  
    
##############################################################################

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

class ACEPLUS(DefinitionBase):

    responses_map = {}
    threads_map = {}
    
    @soap(String, String, _returns=String)
    def WSAsyncRun(self, operationName, parameterList):
        try:
        
            if operationName=='aceplus_generic':
            
                command_line = parameterList[:parameterList.find(',')]
                command_line = command_line.split("=")
                command_line = command_line[1]   
                expected_output = parameterList[parameterList.find(',')+1:]
                expected_output = expected_output.split("=")
                expected_output = expected_output[1]

                r = ConvertorResponse(operationName)
                r.result = expected_output # save this for later
                r.cwd = command_line  # save this for later

                # modify command-line and move inputs to temporary folder
                #workingDir = CFX5_TEMP + "/"

                # start thread
                t = threading.Thread(target=start_aceplus_thread, args=(command_line,))
                t.start()
                self.threads_map[str(t.ident)] = t
                self.responses_map[str(t.ident)] = r
                return str(t.ident)
                
            # check if there was any error
        except Exception as e:
            pass
            return ""
            
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
            expected_output = r.result
            if operationName=='aceplus_generic':
                # read output file
                if os.path.isfile(ACEPLUS_OUTPUT_FILE):
                    r.stdout = open( ACEPLUS_OUTPUT_FILE, 'r').read()
                # read error file
                if os.path.isfile(ACEPLUS_ERROR_FILE):
                    r.stderr = open( ACEPLUS_ERROR_FILE, 'r').read()
                # if there was no error, check the expected_output files are there
                if r.stderr!="":
                    r.statuscode = 1  ## Don't return here. The output files might be generated even if there was an error.
                if expected_output!=None and expected_output!="":
                    if not os.path.isfile(expected_output) and r.stderr=="":
                        # if a file is not found, throw an error
                        r.result = expected_output + ' not found. Something went wrong.'
                        r.statuscode = 1 
                    else:
                        r.result = expected_output
        except Exception as e:
            pass
            r.statuscode = 1
            r.result = e.message
        # output must be string, but resembling the ConvertorResponse originally returned by cfx5ws_generic
        soapResponse =  'aceplus_genericResponse=<aceplus_genericResponse><aceplus_genericResult>\
<stdout>%s</stdout><cwd>%s</cwd><command>%s</command><result>%s</result><stderr>%s</stderr><statuscode>%s</statuscode>\
</aceplus_genericResult></aceplus_genericResponse>' % (r.stdout, r.cwd, r.command, r.result , r.stderr, str(r.statuscode) )
        return soapResponse
        
    @soap(String, String, _returns=ConvertorResponse)
    def aceplus_generic(self, command_line, expected_output):
        r = ConvertorResponse(command_line)
        r.result = expected_output    
        return r
        
##############################################################################

# wait for ANSYS vpn to be properly up ...
while True:
    p = Popen(["route", "-n"], stdin=PIPE, stdout=PIPE, stderr=PIPE)
    output, err = p.communicate()
    if output.count("ppp0")==2 and output.count("eth0")>=3:
        break
    time.sleep(5)
# ... now start the web server
soap_app = soaplib.core.Application([ACEPLUS], 'ap', name='ACEPLUS')
application = wsgi.Application(soap_app)

# main ...
if __name__=='__main__':
    try:
        from wsgiref.simple_server import make_server
        server = make_server(host='0.0.0.0', port=8080, app=application)
        server.serve_forever()
    except ImportError:
        print "Error: server code requires Python >= 2.5"
