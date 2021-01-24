__author__ = 'Ernesto Coto'

import os
import sys
import soaplib
import time
from soaplib.core.service import soap, rpc, DefinitionBase
from soaplib.core.model.primitive import String, Integer
from soaplib.core.model.clazz import ClassModel
from soaplib.core.server import wsgi
from generateCCL_Class import *

##############################################################################

class CclFileGenerator(DefinitionBase):
        
    @soap(String, String, String, String, String, String, String, _returns=String)
    def generateCCL(self, APD_XML_filename, FlowRate_Fourier_descriptors_CSV_filename, Pressure_Fourier_descriptors_CSV_filename, CCL_output_filename, HeartRate, PulsatilityIndex, MeanFlowrate ):
        filename = ""
        try:
        
            genCCL = GenerateCCL(APD_XML_filename, FlowRate_Fourier_descriptors_CSV_filename, Pressure_Fourier_descriptors_CSV_filename, CCL_output_filename, int(HeartRate), float(PulsatilityIndex), float(MeanFlowrate))
            filename = genCCL.writeCCLFile()

            ###### Script's main part - End #######
       
        except Exception as e:
            pass
            return e.message
        return filename
        

##############################################################################

soap_app = soaplib.core.Application([CclFileGenerator], 'ccl', name='CclFileGenerator')
application = wsgi.Application(soap_app)

if __name__=='__main__':
    try:
        from wsgiref.simple_server import make_server
        server = make_server(host='0.0.0.0', port=8080, app=application)
        server.serve_forever()
    except ImportError:
        print "Error: server code requires Python >= 2.5"
