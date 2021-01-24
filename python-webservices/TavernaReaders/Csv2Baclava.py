from Taverna2WorkflowIO import *
import sys

if len(sys.argv)>1 and sys.argv[1].endswith('.csv'):
    cvs2baclava = Taverna2WorkflowIO()
    cvs2baclava.loadInputsFromCSVFile(sys.argv[1])
    f = open(sys.argv[1].replace('.csv','.xml'),'w')
    f.write(cvs2baclava.inputsToBaclava()) 
    f.close()
else:
    print "Usage: python Csv2Baclava <full-path-and-name-of-csv-file>"
