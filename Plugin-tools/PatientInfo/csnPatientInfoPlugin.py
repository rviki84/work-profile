# CSNake configuration of the PatientInfoPlugin

# CSNake imports
from csnAPIPublic import GetAPI
api = GetAPI("2.5.0")
# Other dependencies
from csnGIMIASDef import *
from csnPatientClinicalInfoToolkit import * 

from csnToolkitOpen import * 

# Definition of the template plugin
patientInfoPlugin = GimiasPluginProject("PatientInfoPlugin", api)
# plugin dependencies
projects = [
    baseLibITK,
    tpExtLibMITK, 
    gmCore,  
    boost, 
    cilabMacros,
    dcmAPI,
    wxMitk,
	dicomPlugin
]
patientInfoPlugin.AddProjects(projects)
# plugin sources
patientInfoPlugin.AddSources(["*.cxx", "*.h"])
patientInfoPlugin.SetPrecompiledHeader("PatientInfoPluginPCH.h")
# plugin tests
patientInfoPlugin.AddTests(["tests/*.*"], cxxTest)

# plugin widgets
widgetModules = [
  "PatientInfoPluginPatientWorkingAreaWidget",
]
patientInfoPlugin.AddWidgetModules(widgetModules, _useQt = 0)
patientInfoPlugin.AddSources(["processors/*.cxx", "processors/*.h"])
patientInfoPlugin.AddIncludeFolders(["processors",])