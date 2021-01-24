# CSnake configuration of the CoronaryArteryModelingPlugin

# CSNake imports
from csnAPIPublic import GetAPI
api = GetAPI("2.5.0")
# Other dependencies
from csnGIMIASDef import *
from csnCArteryModellingToolkit import * 
from csnToolkitOpen import *
import csnProject

# Definition of the plugin
coronaryArteryModelingPlugin = GimiasPluginProject("CoronaryArteryModelingPlugin", api)
# plugin dependencies
projects = [
    gmCore, 
    guiBridgeLib, 
    baseLibVTK,
    guiBridgeLibWxWidgets,
	alglib_v3_10_0
	#cArteryModellingLib
]
coronaryArteryModelingPlugin.AddProjects(projects)
coronaryArteryModelingPlugin.AddProjects([cArteryModellingLib], dependency = False)
coronaryArteryModelingPlugin.AddIncludeFolders([csnProject.ToProject(cArteryModellingLib).GetSourceRootFolder() + "/libmodules/TubularGeodesicComputation/include"])
coronaryArteryModelingPlugin.AddIncludeFolders([csnProject.ToProject(cArteryModellingLib).GetSourceRootFolder() + "/libmodules/RotationalAngioIO/include"])
coronaryArteryModelingPlugin.AddIncludeFolders([csnProject.ToProject(cArteryModellingLib).GetSourceRootFolder() + "/libmodules/RotationalAngioGeo/include"])
coronaryArteryModelingPlugin.AddIncludeFolders([csnProject.ToProject(cArteryModellingLib).GetSourceRootFolder() + "/libmodules/OptimalSubsequenceBijection/include"])
coronaryArteryModelingPlugin.AddIncludeFolders([csnProject.ToProject(cArteryModellingLib).GetSourceRootFolder() + "/libmodules/Reconstruction/include"])
#coronaryArteryModelingPlugin.AddIncludeFolders([csnProject.ToProject(cArteryModellingLib).GetBuildFolder() + "/library/CArteryModellingLib"])
#coronaryArteryModelingPlugin.AddLibraries([csnProject.ToProject(cArteryModellingLib).GetBuildResultsFolder() + "/CArteryModellingLib.lib"])


# plugin sources
coronaryArteryModelingPlugin.AddSources(["*.cxx", "*.h"])
coronaryArteryModelingPlugin.SetPrecompiledHeader("CoronaryArteryModelingPCH.h")
# plugin tests
coronaryArteryModelingPlugin.AddTests(["tests/*.*"], cxxTest)

# plugin widgets
widgetModules = [
  "CArteryModellingSegmentCenterlineWidget",
  "CArteryModellingLumenReconstructionWidget"
]
coronaryArteryModelingPlugin.AddWidgetModules(widgetModules, _useQt = 0)
coronaryArteryModelingPlugin.AddSources(["processors/*.cxx", "processors/*.h"])
coronaryArteryModelingPlugin.AddIncludeFolders(["processors",])

# Copy the necessary resources for the functionality of plugin (by clicking on the button 'Install files to Build Folder' in CSnake GUI)
coronaryArteryModelingPlugin.AddFilesToInstall(coronaryArteryModelingPlugin.Glob("../../../Resources/PoissonSurfaceRecon"), "PoissonSurfaceRecon")
#coronaryArteryModelingPlugin.AddFilesToInstall(coronaryArteryModelingPlugin.Glob("../../../Resources/Coeff_tool"), "Coeff_tool")

# Copy the workflow XML files to the build folder (by clicking on the button 'Install files to Build Folder' in CSnake GUI)
coronaryArteryModelingPlugin.AddFilesToInstall(coronaryArteryModelingPlugin.Glob("../../../Resources/Workflows"), "resource/Workflows")