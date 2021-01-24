__author__ = 'Ernesto Coto'

import xmltodict
import base64
import types

class Taverna2Baclava:
    """ 
    Class for allowing the management of Taverna 2.x baclava files
    Note that this class only supports input/output ports of depth 0 and depth 1.
    
    Fields:

            inputsDic (dictionary): data structure holding the mapping of input ports to input port values.
    """

    inputsDic = {}

    def toString(self):
        """ Returns a simple string version of the contents of the class. """
        return "INPUTS = %s" % str(self.inputsDic)
        
    def getInputPorts(self):
        """ Returns a list with the input ports names. """
        return self.inputsDic.keys()
        
    def setInputPorts(self, ports_list):
        """ Sets the list of input ports names. 
            
            Fields:
            
                ports_list (list): list of input ports names.
        """
        self.inputsDic = dict.fromkeys(ports_list)
        
    def setInputPortValues(self, port_str, values_list):
        """ Sets the input values for a specific port. 
            Won't do anything if the port is not defined or values_list is not a list.
            
            Fields:
            
                port_str (string): name of port to modify.
                values_list (list): list of input values.
        """    
        if port_str in self.inputsDic.keys() and type(values_list) is list:
            self.inputsDic[port_str] = values_list
            
    def getInputPortValues(self, port_str):
        """ Returns the input values for a specific port. 
            Returns None if the port is not defined.
            
            Fields:
            
                port_str (string): name of port to query
        """       
        if port_str in self.inputsDic.keys():
            return self.inputsDic[port_str]
        return None;

    def loadInputsFromBaclavaString(self, baclava_xml_str):
        """ Loads the input ports and values from a baclava-xml-style string. 
         
            Fields:
            
                baclava_xml_str (string): a baclava-xml-style string.
        """   
        baclavaContent = xmltodict.parse(baclava_xml_str)
        singlePort = None  # special treatment for input definitions that specify only one input port          
        for dataThing in baclavaContent['b:dataThingMap']['b:dataThing']:
            if dataThing=='@key': # special treatment for input definitions that specify only one input port
                singlePort = baclavaContent['b:dataThingMap']['b:dataThing']['@key'] 
                myGridDataDocument = baclavaContent['b:dataThingMap']['b:dataThing']['b:myGridDataDocument']
                continue
            elif dataThing<>'b:myGridDataDocument' :
                myGridDataDocument = dataThing.get('b:myGridDataDocument', None)
            partialOrder = myGridDataDocument.get('b:partialOrder', None)
            # if partialOrder tag is not found, the input corresponds to a single value
            if partialOrder is None:
                dataElement = myGridDataDocument.get('b:dataElement', None)
                elementData = dataElement['b:dataElementData']
                decodedString = base64.b64decode(elementData)
                if singlePort!=None:
                    self.inputsDic[ singlePort ] = [ decodedString]
                    singlePort = None
                else:
                    self.inputsDic[ dataThing['@key']   ] = [ decodedString]
            else:
            # if partialOrder tag is found, the input corresponds to a list of values
                if u'@type' in partialOrder and partialOrder[u'@type'] == "list":
                    itemList = partialOrder.get('b:itemList', None).items()[0][1]
                    if not(type(itemList) is list):
                        itemList = [{'b:dataElementData':  itemList['b:dataElementData'], u'@index':  u'0'}] 
                    for dataElement in itemList:
                        # take the input file string, decode it, insert the new folder name on it an modify the input definition XML
                        elementData = dataElement['b:dataElementData']
                        decodedString = base64.b64decode(elementData)
                        print "decodedString", decodedString
                        print "singlePort", singlePort
                        port = None
                        if singlePort!=None:
                            port = singlePort
                        else:
                            port = dataThing['@key']
                        if port in self.inputsDic.keys():
                            self.inputsDic[ port ].append(decodedString)
                        else:
                            self.inputsDic[ port ] = [ decodedString ]
                    if singlePort!=None:
                        singlePort = None
            
    def loadInputsFromBaclavaFile(self, filePath_str):
        """ Loads the input ports and values from a baclava file.
        
            Fields:
            
                filePath_str (string): full path of input baclava file.
        """    
        content = ""
        with open(filePath_str, 'r') as content_file:
            content = content_file.read()
            self.loadInputsFromBaclavaString(content)

    def inputsToBaclava(self):
        """ Returns an XML in baclava format corresponding to previously specified input ports and values. """  
        if len(self.inputsDic)==0:
            return None;
            
        enclosingDicMap = {}
        enclosingDicMap['@xmlns:b']='http://org.embl.ebi.escience/baclava/0.1alpha'
        baclavaDic = { 'b:dataThingMap': enclosingDicMap}
        baseDoc = xmltodict.unparse(baclavaDic, pretty=True)
        fullDataThingStringList = ""
        for port in self.inputsDic.keys():
            if self.inputsDic[port]!=None:
                mimeTypesDict = { 's:mimeTypes' : {'s:mimeType' : 'text/plain'}}
                mimeTypesDict['@xmlns:s'] = 'http://org.embl.ebi.escience/xscufl/0.1alpha'
                metadataDict = { 's:metadata' : mimeTypesDict}
                metadataDict[ '@lsid'] =''
                metadataDict[ '@syntactictype']="'text/plain'"
                if len(self.inputsDic[port])==1:
                    dataElementDataDict = { 'b:dataElementData': base64.b64encode(self.inputsDic[port][0])}
                    dataElementDataDict [ '@lsid'] =''
                    metadataDict[ 'b:dataElement'] = dataElementDataDict
                else:
                    relationEmptyDict = [{ '@parent': "0", '@child': "1" }]
                    for i in range(2,len(self.inputsDic[port])):
                        relationEmptyDict.append({  '@parent': str(i-1), '@child': str(i) })
                    relationDict = { 'b:relation' : relationEmptyDict }
                    relationListDict = { 'b:relationList': relationDict , '@lsid': "" , '@type': "list"} 
                    dataElementDataDict = []
                    for i in range(len(self.inputsDic[port])):
                        dataElementDataDict.append( { 'b:dataElementData': base64.b64encode(self.inputsDic[port][i]), '@lsid': "", '@index': str(i)} )
                    dataElementDict = { 'b:dataElement':  dataElementDataDict}
                    relationListDict['b:itemList'] = dataElementDict
                    metadataDict[ 'b:partialOrder'] = relationListDict
                myGridDataDocumentDict = { 'b:myGridDataDocument': metadataDict, '@key': port}
                dataThingDic = {'b:dataThing': myGridDataDocumentDict}
                dataThingDicString = xmltodict.unparse(dataThingDic, pretty=True)
                dataThingDicString = dataThingDicString[ dataThingDicString.find('\n') + 1 : ]
                fullDataThingStringList = fullDataThingStringList + dataThingDicString
       
        if fullDataThingStringList!="":
            baseDoc = baseDoc.replace("</b:dataThingMap>" , "\n" + fullDataThingStringList + "\n")
            baseDoc = baseDoc + "</b:dataThingMap>"

        return baseDoc

# Sample usage
#test = Taverna2Baclava()
#test.loadInputsFromBaclavaFile("SimpleWorkflowInputs.xml")
#print test.toString()
#print test.inputsToBaclava()

