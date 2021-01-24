#!/usr/bin/python

#
# GenerateCCL - Generates the CCL(CFX Command Language) file from
#                   the APD(Abstract Problem Description) XML file, the
#                   FlowRate and Pressure Fourier Descriptors CSV files,
#                   the HeartRate, Pulsatility Index and Mean Flowrate.
#
# Description: The class contains different components of the CCL file, defined
#               as functions with corresponding names.
#               The writeCCLFile() creates, writes and returns the CCL file.
#               Most of the functions return a block of text that is combined in
#               an order, to generate the output CCL file.
#
# The class is invoked as follows,
#   GenerateCCL( <XML-filename>
#               , <FlowRate Fourier-Descriptors-CSV-filename>
#               , <Pressure Fourier-Descriptors-CSV-filename>
#               , <CCL-output-filename>
#               , <HeartRate>
#               , <PulsatilityIndex>
#               , <MeanFlowrate> )
#
# Sample code:
#               from generateCCL_Class import *
#               genCCL = GenerateCCL("apd.xml", "flowRateFourier.csv", "pressureFourier.csv", "blood_flow.ccl", 75, 2.9103, 3.9859e-06)
#               genCCL.writeCCLFile()
#
#
#
# Alberto Biancardi
# Ernesto Coto
# Vignesh Rammohan
#
# Last modified on 08 April 2015
#
# Copyright 2015 CISTIB - The University of Sheffield. All rights reserved.
#


import os
import sys
import csv
import time
import xml.etree.ElementTree as ET


class GenerateCCL:
    """
    Class to generate the CCL(CFX Command Language) file from
    the APD(Abstract Problem Description) XML file, the
    FlowRate and Pressure Fourier Descriptors CSV files,
    the HeartRate, Pulsatility Index and Mean Flowrate.
    """
    
    def __init__(self, xmlFileName, flowRateFourierFile, pressureFourierFile, cclFileName, heartRate, pulsatilityIndex, meanFlowrate):
        # Initialize the data attributes
        self.xmlFileName = xmlFileName
        self.flowRateFourierFile = flowRateFourierFile
        self.pressureFourierFile = pressureFourierFile
        self.cclFileName = cclFileName
        self.heartRate = heartRate
        self.pulsatilityIndex = pulsatilityIndex
        self.meanFlowrate = meanFlowrate

        # Parse the XML file
        tree = ET.parse(self.xmlFileName)
        self.root = tree.getroot()


    def Command_file(self):
        text = "COMMAND FILE:\n" \
            "  Version = 14.5\n" \
            "END\n"
        return text

    def Solution_units(self):
        text = "  SOLUTION UNITS:\n" \
            "    Angle Units = [rad]\n" \
            "    Length Units = [m]\n" \
            "    Mass Units = [kg]\n" \
            "    Solid Angle Units = [sr]\n" \
            "    Temperature Units = [K]\n" \
            "    Time Units = [s]\n" \
            "  END\n"
        return text

    def Analysis_type(self):
        text = """  ANALYSIS TYPE:
    Option = Transient
    EXTERNAL SOLVER COUPLING:
      Option = None
    END
    INITIAL TIME:
      Option = Automatic with Value
      Time = 0 [s]
    END
    TIME DURATION:
      Option = Total Time
      Total Time = 3*CardiacCyclePeriod
    END
    TIME STEPS:
      Option = Timesteps
      Timesteps = CardiacCyclePeriod/200
    END
  END
  """
        return text


    # Boundary for vessel wall
    def Boundary_wall(self, name, bdry_type, location):
        """
        Creates the CCL boundary for WALL_VESSEL.
        """
        options = ['Value', 'No Slip Wall', 'Normal to Boundary Condition', 'Subsonic', 'Zero Flux', \
                'Mass Flow Rate', 'Opening Pressure and Direction']
        boundary = """BOUNDARY: %s
      Boundary Type = %s
      Location = %s
      BOUNDARY CONDITIONS:
        ADDITIONAL VARIABLE: WSSField
          Additional Variable Value = WallShearMag
          Option = %s
        END
        MASS AND MOMENTUM:
          Option = %s
        END
      END
    END
    """ %(name, bdry_type, location, options[0], options[1])
           
        return boundary


    # Boundary for Inlets and Outlets
    def Boundary(self, name, bdry_type, location):
        """
        Creates the CCL boundary for INLETs and OUTLETs.
        """
        options = ['Value', 'No Slip', 'Normal to Boundary Condition', 'Subsonic', 'Zero Flux', \
                'Mass Flow Rate', 'Opening Pressure and Direction']

        mass_momentum = ""
        if bdry_type == "INLET":
            add_var_contrast = """ADDITIONAL VARIABLE: contrast
          Additional Variable Value = 1 []
          Option = %s
        END""" % (options[0])
            mass_momentum = """MASS AND MOMENTUM:
          Mass Flow Rate = %sf
          Option = %s
        END""" % (location, options[5])
        elif bdry_type == "OPENING":
            add_var_contrast = """ADDITIONAL VARIABLE: contrast
          Additional Variable Value = 0 []
          Option = %s
        END""" % (options[0])
            mass_momentum = """MASS AND MOMENTUM:
          Option = %s
          Relative Pressure = %sf
        END""" % (options[6], location)
        boundary = """BOUNDARY: %s
      Boundary Type = %s
      Location = %s
      BOUNDARY CONDITIONS:
        FLOW DIRECTION:
          Option = %s
        END
        FLOW REGIME:
          Option = %s
        END
        ADDITIONAL VARIABLE: WSSField
          Option = %s
        END
        %s
        %s
      END
    END
    """ %(name, bdry_type, location, options[2], options[3], options[4], add_var_contrast, mass_momentum)
           
        return boundary


    def Domain_models(self):
        text = """DOMAIN MODELS:
      BUOYANCY MODEL:
        Option = Non Buoyant
      END
      DOMAIN MOTION:
        Option = Stationary
      END
      MESH DEFORMATION:
        Option = None
      END
      REFERENCE PRESSURE:
        Reference Pressure = 9144.82 [Pa]
      END
    END
    """
        return text

    def Fluid_definition(self, fluid_name):
        text = """FLUID DEFINITION: %s
      Material = Blood
      Option = Material Library
      MORPHOLOGY:
        Option = Continuous Fluid
      END
    END
    """ %(fluid_name)
        
        return text


    def Fluid_models(self):
        text = """FLUID MODELS:
      COMBUSTION MODEL:
        Option = None
      END
      HEAT TRANSFER MODEL:
        Option = None
      END
      THERMAL RADIATION MODEL:
        Option = None
      END
      TURBULENCE MODEL:
        Option = Laminar
      END
      ADDITIONAL VARIABLE: AveAneurysmVel
        Additional Variable Value = volumeAve(Velocity)@ANEURYSM_1sub
        Option = Algebraic Equation
      END
      ADDITIONAL VARIABLE: PressGauge
        Additional Variable Value = pref + Pressure
        Option = Algebraic Equation
      END
      ADDITIONAL VARIABLE: OSIfield
        Option = Algebraic Equation
        Additional Variable Value = \\
         (1 - (WSTaveMag/(WSSField.Trnavg+1e-15[Pa])))/2
      END
      ADDITIONAL VARIABLE: TAWSSField
        Additional Variable Value = WSSField.Trnavg
        Option = Algebraic Equation
      END
      ADDITIONAL VARIABLE: TAWSSG
        Additional Variable Value = 1/TAWSSField * (TavWSSGX*(Wall Shear \\
          X.Trnavg) + TavWSSGY*(Wall Shear Y.Trnavg) + TavWSSGZ*(Wall Shear \\
          Z.Trnavg))
        Option = Algebraic Equation
      END
      ADDITIONAL VARIABLE: ViscDisp
        Option = Algebraic Equation
        Additional Variable Value = (Shear Strain Rate)^2
      END
      ADDITIONAL VARIABLE: WSSField
        Option = Poisson Equation
        Kinematic Diffusivity = 1e-15 [m^2 s^-1]
      END
      ADDITIONAL VARIABLE: WSSG
        Option = Vector Algebraic Equation
        Vector xValue = WSSField.Gradient X
        Vector yValue = WSSField.Gradient Y
        Vector zValue = WSSField.Gradient Z
      END
      ADDITIONAL VARIABLE: WSSPI
        Additional Variable Value = \\
          (WSSField.Trnmax-WSSField.Trnmin)/WSSField.Trnavg
        Option = Algebraic Equation
      END
      ADDITIONAL VARIABLE: WSSq
        Additional Variable Value = abs((Wall Shear X*qx)+(Wall Shear \\
          Y*qy)+(Wall Shear Z*qz))
        Option = Algebraic Equation
      END
      ADDITIONAL VARIABLE: contrast
        Option = Transport Equation
      END
    END
  """
        return text


    def Subdomain(self, subdomain_name, subdomain_location):
        text = """  SUBDOMAIN: %s
      Coord Frame = Coord 0
      Location = %s
    END
  """ %(subdomain_name, subdomain_location)
        
        return text


    def Domain(self, dom_type):
        res = """DOMAIN: %s
    Coord Frame = Coord 0
    Domain Type = Fluid
    Fluids List = Blood
    Location = Assembly
    """ %(dom_type)
        bcEnts = [x.attrib for x in self.root.iter('bc')]
        bcNum = len( bcEnts)
        res += self.Boundary_wall("WALL_VESSEL", "WALL", "WALL_PARENT_VESSEL")
        for i in range( bcNum):
            bdry_type = str(bcEnts[i]['opening']).upper()
            if bdry_type == "OUTLET":
                bdry_type = "OPENING"
            location = str(bcEnts[i]['region']).upper()
            name = location
            res+= self.Boundary(name, bdry_type, location)
        res += self.Domain_models() + self.Fluid_definition("Blood") + self.Fluid_models() \
                + self.Subdomain("ANEURYSM_1sub", "ANEURYSM_1") + self.Subdomain("PARENT_VESSELsub", "PARENT_VESSEL")

        return res


    def Initialisation(self):
        text = """  INITIALISATION:
    Option = Automatic
    INITIAL CONDITIONS:
      Velocity Type = Cartesian
      CARTESIAN VELOCITY COMPONENTS:
        Option = Automatic with Value
        U = 0 [m s^-1]
        V = 0 [m s^-1]
        W = 0 [m s^-1]
      END
      STATIC PRESSURE:
        Option = Automatic with Value
        Relative Pressure = 0 [Pa]
      END
      ADDITIONAL VARIABLE: WSSField
        Additional Variable Value = 0 [kg m^-1 s^-2]
        Option = Automatic with Value
      END
      ADDITIONAL VARIABLE: contrast
        Additional Variable Value = 0 []
        Option = Automatic with Value
      END
    END
  END
  """
        return text

            
    def Output_control(self):
        text = """OUTPUT CONTROL:
    BACKUP RESULTS: Backup Results 1
      File Compression Level = Default
      Option = Standard
      OUTPUT FREQUENCY:
        Option = Timestep Interval
        Timestep Interval = 50
      END
    END
    RESULTS:
      File Compression Level = Default
      Option = Standard
    END
    TRANSIENT RESULTS: Transient Results 1
      File Compression Level = Default
      Include Mesh = No
      Option = Selected Variables
      Output Variables List = Pressure,PressGauge,Velocity,Velocity \\
        u,Velocity v,Velocity w,Wall Shear,Wall Shear X,Wall Shear Y,Wall \\
        Shear Z,contrast,ViscDisp,WSSField,WSSG
      OUTPUT FREQUENCY:
        Option = Timestep List
        Timestep List = 400, 402, 404, 406, 408, 410, 412, 414, 416, 418, \\
          420, 422, 424, 426, 428, 430, 432, 434, 436, 438, 440, 442, 444, \\
          446, 448, 450, 452, 454, 456, 458, 460, 462, 464, 466, 468, 470, \\
          472, 474, 476, 478, 480, 482, 484, 486, 488, 490, 492, 494, 496, \\
          498, 500, 502, 504, 506, 508, 510, 512, 514, 516, 518, 520, 522, \\
          524, 526, 528, 530, 532, 534, 536, 538, 540, 542, 544, 546, 548, \\
          550, 552, 554, 556, 558, 560, 562, 564, 566, 568, 570, 572, 574, \\
          576, 578, 580, 582, 584, 586, 588, 590, 592, 594, 596, 598, 600
      END
    END
    TRANSIENT STATISTICS: trnstat1
      Option = Full
      Output Variables List = Velocity,Wall \\
        Shear,AveAneurysmVel,WSSField,TAWSSField
      Start Iteration List = 401
      Stop Iteration List = 600
    END
  END
  """
        return text


    def Solver_control(self):
        text = """SOLVER CONTROL:
    ADVECTION SCHEME:
      Option = Specified Blend Factor
      Blend Factor = 1.0
    END
    EQUATION CLASS: av
      ADVECTION SCHEME:
        Option = High Resolution
      END
    END
    CONVERGENCE CONTROL:
      Maximum Number of Coefficient Loops = 5
      Minimum Number of Coefficient Loops = 1
      Timescale Control = Coefficient Loops
    END
    CONVERGENCE CRITERIA:
      Residual Target = 0.005
      Residual Type = MAX
    END
    TRANSIENT SCHEME:
      Option = Second Order Backward Euler
      TIMESTEP INITIALISATION:
        Option = Extrapolation
      END
    END
  END
  """
        return text


    def Expert_parameters(self):
        text = """EXPERT PARAMETERS:
    linearly exact numerics = t
  END
"""
        return text


    def Additional_variables(self):
        text = """
  ADDITIONAL VARIABLE: AveAneurysmVel
    Option = Definition
    Tensor Type = SCALAR
    Units = [m s^-1]
    Variable Type = Volumetric
  END
  ADDITIONAL VARIABLE: PressGauge
    Option = Definition
    Tensor Type = SCALAR
    Units = [Pa]
    Variable Type = Unspecified
  END
  ADDITIONAL VARIABLE: OSIfield
    Boundary Only Field = t
    Option = Definition
    Tensor Type = SCALAR
    Units = []
    Variable Type = Unspecified
  END
  ADDITIONAL VARIABLE: TAWSSField
    Option = Definition
    Tensor Type = SCALAR
    Units = [Pa]
    Variable Type = Unspecified
  END
  ADDITIONAL VARIABLE: TAWSSG
    Option = Definition
    Tensor Type = SCALAR
    Units = [Pa m^-1]
    Variable Type = Unspecified
  END
  ADDITIONAL VARIABLE: WSSField
    Option = Definition
    Tensor Type = SCALAR
    Units = [Pa]
    Variable Type = Specific
  END
  ADDITIONAL VARIABLE: WSSG
    Option = Definition
    Tensor Type = VECTOR
    Units = [Pa m^-1]
    Variable Type = Unspecified
  END
  ADDITIONAL VARIABLE: WSSPI
    Option = Definition
    Tensor Type = SCALAR
    Units = [ ]
    Variable Type = Unspecified
  END
  ADDITIONAL VARIABLE: WSSq
    Option = Definition
    Tensor Type = SCALAR
    Units = [Pa]
    Variable Type = Unspecified
  END
  ADDITIONAL VARIABLE: contrast
    Option = Definition
    Tensor Type = SCALAR
    Units = [ ]
    Variable Type = Volumetric
  END
  ADDITIONAL VARIABLE: ViscDisp
    Option = Definition
    Tensor Type = SCALAR
    Units = [s^-2]
    Variable Type = Unspecified
  END
  """
        return text

    def Material(self):
        text = """MATERIAL: Blood
    Material Group = User
    Option = Pure Substance
    PROPERTIES:
      Option = General Material
      DYNAMIC VISCOSITY:
        Dynamic Viscosity = visc
        Option = Value
      END
      EQUATION OF STATE:
        Density = dens
        Molar Mass = 1.0 [kg kmol^-1]
        Option = Value
      END
    END
  END
  """
        return text


    def Waveforms(self):
        """
        Generates the waveform equations for INLETs and OUTLETs boundaries, from the Fourier Descriptors
        """
    
        ##### Get the systemic_model_segment of the defined boundaries #####
        bcEnts = [x.attrib for x in self.root.iter('bc')]
        bcNum = len( bcEnts)
        sysSegmNos = []
        sysSegmLocNos = []
        regNames = []
        sysSegmDict = {}
        for i in range( bcNum):
            regNames.append(str(bcEnts[i]['region']))
            for cutEnt in self.root.iter('cut'):
                nm = cutEnt.find('name')
                if nm.text == regNames[i]:
                    break
            sysSegmNos.append(cutEnt.attrib['systemic_model_segment'])
            if cutEnt.attrib['systemic_model_segment_loc'] == "":
                sysSegmLocNos.append(3)
            else:
                sysSegmLocNos.append(cutEnt.attrib['systemic_model_segment_loc'])
            sysSegmDict[regNames[i]] = sysSegmNos[i]

        ##### Get the corresponding FlowRate Fourier from the CSV file #####    
        flowRateFourierDict = {}
        sysSegCount = len(sysSegmNos)
        with open( self.flowRateFourierFile, 'r') as csvFile:
            fourierReader = csv.reader(csvFile)
            for row in fourierReader:
                for i in range(sysSegCount):
                    if float(row[0]) == ((int(sysSegmNos[i])-1)*5)+int(sysSegmLocNos[i]):
                        try:
                            flowRateFourierDict[sysSegmNos[i]] = row[1:]
                        except:
                           pass
                        break

        ##### Get the corresponding Pressure Fourier from the CSV file #####    
        pressureFourierDict = {}
        sysSegCount = len(sysSegmNos)
        with open( self.pressureFourierFile, 'r') as csvFile:
            fourierReader = csv.reader(csvFile)
            for row in fourierReader:
                for i in range(sysSegCount):
                    if float(row[0]) == ((int(sysSegmNos[i])-1)*5)+int(sysSegmLocNos[i]):
                        try:
                            pressureFourierDict[sysSegmNos[i]] = row[1:]
                        except:
                           pass
                        break

        ##### Generate the waveform equations #####
        regCount = len(regNames)
        waveforms = ""
        space6 = "      "
        space7 = "       "
        fourierCount = len(flowRateFourierDict[sysSegmDict[regNames[0]]])
        frequency = 6.283185*int(self.heartRate)/60
        for i in range(regCount):
            if regNames[i].find('inlet') >= 0:
                factorConst = self.meanFlowrate*(1-self.pulsatilityIndex)
                globalFactor = self.meanFlowrate*self.pulsatilityIndex
                waveforms += regNames[i].upper() + "f = \\\n" + space6 + "(("
                for j in range(fourierCount):
                    if j == 0:
                        waveforms += flowRateFourierDict[sysSegmDict[regNames[i]]][j] + "+(" \
                                     + str(factorConst) + "))\\\n"
                    if (j>0) & (j%2 != 0) & (j < fourierCount-1):
                        waveforms += space7+ "+(" + flowRateFourierDict[sysSegmDict[regNames[i]]][j] \
                                     + "*sin(" + str(int((j+1)/2)) + "*" + str(frequency) + "*t/1[s])) "
                    elif (j>0) & (j%2 == 0):
                        waveforms += "+(" + flowRateFourierDict[sysSegmDict[regNames[i]]][j] \
                                     + "*cos(" + str(int(j/2)) + "*" + str(frequency) + "*t/1[s]))\\\n"
                    elif (j>0) & (j == fourierCount -1):
                        waveforms += space7 + "+(" + flowRateFourierDict[sysSegmDict[regNames[i]]][j] \
                                     + "*cos(" + str(int((j+1)/2)) + "*" + str(frequency) + "*t/1[s])))" \
                                     + "*" + str(globalFactor) + "*1[m^3 s^-1]*areaAve(density)@" \
                                     + regNames[i].upper() + "\n"
            else:
                waveforms += space6 + regNames[i].upper() + "f = \\\n" + space6 + "("
                for j in range(fourierCount):
                    if j == 0:
                        waveforms += pressureFourierDict[sysSegmDict[regNames[i]]][j] + "\\\n"
                    if (j>0) & (j%2 != 0) & (j < fourierCount-1):
                        waveforms += space7+ "+(" + pressureFourierDict[sysSegmDict[regNames[i]]][j] \
                                     + "*sin(" + str(int((j+1)/2)) + "*" + str(frequency) + "*t/1[s])) "
                    elif (j>0) & (j%2 == 0):
                        waveforms += "+(" + pressureFourierDict[sysSegmDict[regNames[i]]][j] \
                                     + "*cos(" + str(int(j/2)) + "*" + str(frequency) + "*t/1[s]))\\\n"
                    elif (j>0) & (j == fourierCount -1):
                        waveforms += space7 + "+(" + pressureFourierDict[sysSegmDict[regNames[i]]][j] \
                                     + "*cos(" + str(int((j+1)/2)) + "*" + str(frequency) + "*t/1[s])))" \
                                     + "*1[Pa]" + "\n"

        return waveforms    


    def Expressions(self):
        for viscosity in self.root.iter('viscosity'):
            viscosity = viscosity.text
            break
        for density in self.root.iter('density'):
            density = density.text
            break
        text = """    EXPRESSIONS:
      CardiacCyclePeriod = 60/HR *1[s]
      HR = %s
      viscval = %s
      densval = %s
      visc = viscval * 1[Pa s]
      dens = densval * 1[kg m^-3]
      TavWSSGX = (TAWSSField.Gradient X)-(TAWSSField.Gradient X*Normal X + \\
        TAWSSField.Gradient Y*Normal Y + TAWSSField.Gradient Z*Normal \\
        Z)*Normal X
      TavWSSGY = (TAWSSField.Gradient Y)-(TAWSSField.Gradient X*Normal X + \\
        TAWSSField.Gradient Y*Normal Y + TAWSSField.Gradient Z*Normal \\
        Z)*Normal Y
      TavWSSGZ = (TAWSSField.Gradient Z)-(TAWSSField.Gradient X*Normal X + \\
        TAWSSField.Gradient Y*Normal Y + TAWSSField.Gradient Z*Normal \\
        Z)*Normal Z
      WallShearMag = sqrt(Wall Shear X^2 + Wall Shear Y^2 + Wall Shear Z^2)
      WSTaveMag = sqrt(Wall Shear X.Trnavg^2 + Wall Shear Y.Trnavg^2 + \\
        Wall Shear Z.Trnavg^2)
      px = Wall Shear X.Trnavg/(WSTaveMag+1e-15[Pa])
      py = Wall Shear Y.Trnavg/(WSTaveMag+1e-15[Pa])
      pz = Wall Shear Z.Trnavg/(WSTaveMag+1e-15[Pa])
      qx = (py*Normal Z) - (pz*Normal Y)
      qy = (pz*Normal X) - (px*Normal Z)
      qz = (px*Normal Y) - (py*Normal X)
      %s    END
  """ %(self.heartRate, viscosity, density, self.Waveforms())
        return text

        
    def Flow(self, flow_name, outCCLfilename):
        """
        FLOW component of the CCL file
        """
        outCCLfilename.write(flow_name)
        
        outCCLfilename.write(self.Solution_units())
        outCCLfilename.write(self.Analysis_type())
        outCCLfilename.write(self.Domain("Fluid"))
        outCCLfilename.write("END\n")
        outCCLfilename.write(self.Initialisation())   
        outCCLfilename.write(self.Output_control())
        outCCLfilename.write(self.Solver_control())
        outCCLfilename.write(self.Expert_parameters())

        outCCLfilename.write("END\n")


    def Library(self, outCCLfilename):
        """
        LIBRARY component of the CCL file
        """
        outCCLfilename.write("LIBRARY:")

        outCCLfilename.write(self.Additional_variables())   
        outCCLfilename.write(self.Material())
        
        outCCLfilename.write("CEL:\n")
        outCCLfilename.write(self.Expressions())
        outCCLfilename.write("END\n")

        outCCLfilename.write("END\n")



    def writeCCLFile(self):
        """
        Writes the CCL file in the current directory
        """
        outCCL = open( self.cclFileName, 'w')
        date = time.ctime()
        filePath = os.path.abspath(__file__)
        fileInfo = ("# Generated by Python script %s\n"
        "# Version  $Id: 1.0.1 $\n"
        "# Date  %s\n\n") % (filePath, date)

        # CCL file generation
        outCCL.write(fileInfo)
        outCCL.write(self.Command_file())
        self.Flow("FLOW: Flow Analysis 1\n", outCCL)
        self.Library(outCCL)
        outCCL.close()
        return self.cclFileName
