import soaplib
import os
from soaplib.core.service import soap, rpc, DefinitionBase
from soaplib.core.model.primitive import String, Integer
from soaplib.core.model.clazz import Array
from soaplib.core.server import wsgi
from subprocess import Popen, PIPE

class icemService(DefinitionBase):
	@soap(String, _returns=String)
	def icemInvoker(self, cgnsFileFullPath):
		# prepare load script
		with open(r"C:\Users\ansys\Desktop\load.rpl", "wt+") as fout:
			with open(r"C:\webapp\AnsysInvoker\load.rpl", "rt") as fin:
				for line in fin:
					fout.write(line.replace("<INPUT_CGNS_FILE_HERE>", cgnsFileFullPath.replace("\\","/") ) )
		# move saving script
		p = Popen(['copy', r"C:\webapp\AnsysInvoker\save.rpl", r"C:\Users\ansys\Desktop\save.rpl"], stdin=PIPE, stdout=PIPE, stderr=PIPE, shell=True)
		output, err = p.communicate()  
		# invoke icem
		os.chdir(r"C:\Program Files\ANSYS Inc\v145\icemcfd\win64_amd\bin")
		p = Popen(["icemcfd.bat" , r"C:\Users\ansys\Desktop\load.rpl"], stdin=PIPE, stdout=PIPE, stderr=PIPE)
		output, err = p.communicate()
		if len(err)>0:
			return err
		# copy to intended destiny
		mshOutputPath = cgnsFileFullPath.replace(".cgns", ".msh")
		p = Popen(['copy', '/Y', r"C:\temp\output.msh" , mshOutputPath], stdin=PIPE, stdout=PIPE, stderr=PIPE , shell=True)
		output, err = p.communicate()        
		if len(err)>0:
			return err
		return mshOutputPath

soap_app = soaplib.core.Application([icemService], 'tns', name='icemService')
application = wsgi.Application(soap_app)

if __name__=='__main__':
	from cherrypy._cpwsgiserver import CherryPyWSGIServer
	server = CherryPyWSGIServer(('0.0.0.0',8080), wsgi_app=application, timeout = 10000000 )
	server.start()

