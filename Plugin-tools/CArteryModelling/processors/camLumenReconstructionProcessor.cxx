/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#include "camLumenReconstructionProcessor.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "coreReportExceptionMacros.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityHelper.txx"
#include "coreKernel.h"
#include "coreSettings.h"

#include <boost/filesystem.hpp>
#include "vtkSmartPointer.h"
#include "vtkDirectory.h"
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkPLYReader.h>
#include <vtkFileOutputWindow.h>


namespace cam
{

LumenReconstructionProcessor::LumenReconstructionProcessor( )
{
	SetName( "LumenReconstructionProcessor" );
	
	BaseProcessor::SetNumberOfInputs( NO_OF_INPUTS );
	GetInputPort(CENTERLINE_SET)->SetName( "Segmented Centerlines" );
	GetInputPort(CENTERLINE_SET)->SetDataEntityType( Core::SurfaceMeshTypeId | Core::SkeletonTypeId );

	
	BaseProcessor::SetNumberOfOutputs( NO_OF_OUTPUTS );
	GetOutputPort( LUMEN_SURFACE )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetOutputPort( LUMEN_SURFACE)->SetReuseOutput( false );
	GetOutputPort( LUMEN_SURFACE )->SetDataEntityName( "Lumen Surface" );
	GetOutputPort( CENTERLINE )->SetDataEntityType( Core::SurfaceMeshTypeId | Core::SkeletonTypeId );
	GetOutputPort( CENTERLINE)->SetReuseOutput( false );
	GetOutputPort( CENTERLINE )->SetDataEntityName( "Centerline" );

	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
	m_poissonReconX64Path = settings->
		GetApplicationPathSubPath("PoissonSurfaceRecon") + "/PoissonRecon.x64.exe";
	m_poissonReconWin32Path = settings->
		GetApplicationPathSubPath("PoissonSurfaceRecon") + "/PoissonRecon.Win32.exe";
	m_poissonReconSelected = m_poissonReconX64Path;
	
	m_homeOutputPath = settings->GetProjectHomePath() + "\\tmp\\LumenRecon\\finalOut";
	vtkSmartPointer<vtkDirectory> dir = vtkSmartPointer<vtkDirectory>::New();
	if(dir->Open(m_homeOutputPath.c_str())==FALSE)
	{
		dir->MakeDirectory(m_homeOutputPath.c_str());
	}
}

LumenReconstructionProcessor::~LumenReconstructionProcessor()
{
	vtkSmartPointer<vtkDirectory> dir = vtkSmartPointer<vtkDirectory>::New();
	std::string folder = 
		Core::Runtime::Kernel::GetApplicationSettings()->GetProjectHomePath() + "\\tmp";
	if(dir->Open(folder.c_str())==TRUE)
	{
		dir->DeleteDirectory(folder.c_str());
	}
}


void LumenReconstructionProcessor::Update()
{
	this->ReconstructLumenRegion();
	/*
	// create output images
	Core::DataEntity::Pointer outDataEntity = Core::DataEntity::New( );
	outDataEntity->SetType( GetInputDataEntity( RA_IMAGE )->GetType( ) );
	outDataEntity->GetMetadata( )->SetName( "Propagated data" );

	int numOfTimeSteps = GetInputDataEntity(0)->GetNumberOfTimeSteps();
	for ( int t = 0 ; t < numOfTimeSteps ; t++ )
	{
		// here goes the filter or the functions that determine the processor
	    // the output should go in the update functions

		//InputImageType::Pointer itkInputImage;
		//GetProcessingData(INPUT_0, itkInputImage, t);

		//outDataEntity->SetTimeStep(multiScaleEnhancementFilter->GetOutput(),t,GetInputDataEntity( 0 )->GetTimeAtTimeStep(t));
	}
	
	// Set the output to the output of this processor
	//UpdateOutput( CENTERLINE, outDataEntity, GetInputDataEntity( RA_IMAGE ), GetInputDataEntity( RA_IMAGE )->GetMetadata( ).GetPointer( ) );
	*/
}


void LumenReconstructionProcessor::ReconstructLumenRegion()
{
	/**	
	Get image folder path from the metadata of the 
	input RA Image(Refer cam.SegmentCenterlineProcessor)
	*/ 
	std::string imageFolder = "";
	Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
	Core::DataEntityList::Pointer dataList;
	dataList = dataContainer->GetDataEntityList();
	blTag::Pointer tag = NULL;
	Core::DataEntityList::iterator it;
	for (it = dataList->Begin(); it != dataList->End(); it++)
	{
		if ((*it)->GetSubject()->IsImage())
		{
			tag = (*it)->GetSubject()->GetMetadata()->GetTag( "ImageDataPath" );
			imageFolder = tag->GetValueAsString();
			break;
		}
	}
	if ( imageFolder == "" )
	{
		wxMessageBox("Please load the Rotational Angiography Image");
		return;
	}

	//! User selection of the Poisson Surface Reconstruction executable version (64/32 bit)
	//! By default it is 64 bit. Clicking on 'NO' button will consider 32 bit.
	//! This selection is introduced to avoid errors due to the version, when run on different computers.
	wxString confirmVersion(std::string("Do you want to run the Poisson Reconstruction 64 bit,") + 
		"\n\nPress 'YES' for 64 bit: " + m_poissonReconX64Path +
		"\n\nPress 'NO' for 32 bit: " + m_poissonReconWin32Path +
		"\n\n\nPress 'CANCEL' to quit the execution.");
	int buttonPressed = wxID_YES;
	wxMessageDialog dialog( 
		NULL, confirmVersion, "Poisson Reconstruction Version - ***Selection***"
		, wxYES_NO | wxCANCEL | wxICON_QUESTION );
	buttonPressed = dialog.ShowModal();
	if (buttonPressed == wxID_YES)
	{
		m_poissonReconSelected = m_poissonReconX64Path;
	}
	else if (buttonPressed == wxID_NO)
	{
		m_poissonReconSelected = m_poissonReconWin32Path;
	}
	// Pressing CANCEL quits the execution at this point
	else if(buttonPressed == wxID_CANCEL)
	{
		return;
	}

	// check whether the Screened Poisson Surface Reconstruction executable (selected one)
	// is available, return otherwise
	if (!boost::filesystem::exists( m_poissonReconSelected ))
	{
		wxString errorMsg( "Please check for the file at:\n" + m_poissonReconSelected 
			+ "\n\nLumen reconstruction cannot be performed without this executable!");
		wxMessageDialog dialogRun( NULL, errorMsg, "Executable Missing", wxOK | wxICON_ERROR );
		dialogRun.ShowModal();
		return;
	}

	// geometry
	std::string geoFileName = imageFolder + "/geoCoeff.txt";
	PerspectiveGeometryType::Pointer geo_p = PerspectiveGeometryType::New();
	geo_p->LoadGeometry(geoFileName);

	// frames & branches
	blTag::Pointer tagFrameNos = GetInputDataEntity(CENTERLINE_SET)->GetMetadata()->GetTag("Frame Numbers");
	if (tagFrameNos == NULL)
	{
		wxString errorMsg( "The selected input does not have the required metadata." 
			"\n\nPlease check whether you have loaded the correct segmented centerlines data");
		wxMessageDialog dialogRun( NULL, errorMsg, "Metadata 'Frame Numbers' Missing", wxOK | wxICON_ERROR );
		dialogRun.ShowModal();
		return;
	}
	std::string frameNosStr;
	tagFrameNos->GetValue< std::string >( frameNosStr );
	std::vector<unsigned int> frameNumbers; // zero-based
	frameNumbers = this->GetUInts(frameNosStr);
	size_t totalTimesteps = GetInputDataEntity(CENTERLINE_SET)->GetNumberOfTimeSteps();
	unsigned int nb = totalTimesteps/frameNumbers.size();

	// multi-scale reconstruction
	vtkSmartPointer< cam::MultiscaleOsbReconstruction<TPrecision> > multiScaleRecons 
		= vtkSmartPointer< cam::MultiscaleOsbReconstruction<TPrecision> >::New();
	multiScaleRecons->Initialize(frameNumbers, nb, geo_p);
	for(unsigned int f=0; f<frameNumbers.size(); f++)
	{
		std::ostringstream fstr;
		fstr << frameNumbers[f];
		for(unsigned int b=0; b<nb; b++)
		{
			std::ostringstream bstr;
			bstr << b;
			std::cout << "Loading data for frame #" << frameNumbers[f] << " and branch #" << b << std::endl;
			size_t timestep = f + (b * frameNumbers.size());
			vtkSmartPointer<vtkPolyData> branch = vtkSmartPointer<vtkPolyData>::New();
			GetInputDataEntity(CENTERLINE_SET)->GetProcessingData(branch, timestep);
			multiScaleRecons->SetPolyLine(frameNumbers[f], b, branch);
		}
	}
	
	// get and set the registration parameters
	double outlierWt = atof(m_outlierWeight.c_str());
	multiScaleRecons->SetOutlierWeight(outlierWt);
	unsigned int maxIter = atoi(m_maxIterationsCPD.c_str());
	multiScaleRecons->SetMaxIterationsCPD(maxIter);
	double tolerance = atof(m_tolerance.c_str());
	multiScaleRecons->SetTolerance(tolerance);

	// get epipolar distance thresholds from the comma separated string values
	std::vector<double> epiDists;
	epiDists = this->GetDoubles(m_epipolarDistThresh);
	multiScaleRecons->SetEpipolarDistanceThreshold(epiDists);
	// get resampling levels from the comma separated string values
	std::vector<unsigned int> resampleLevels;
	resampleLevels = this->GetUInts(m_resamplingLevels);
	multiScaleRecons->SetResamplingLevels(resampleLevels);
	// get maximum iterations from the comma separated string values
	std::vector<unsigned int> maxIts;
	maxIts = this->GetUInts(m_maxIterationsOSB);
	multiScaleRecons->SetMaxIterations(maxIts);
	// get and set the OSB parameters other than above three
	unsigned int skipDistance = atoi(m_skipDistance.c_str());
	multiScaleRecons->SetSkip(skipDistance);
	double skipCost = atof(m_skipCost.c_str());
	multiScaleRecons->SetJumpCost(skipCost);
	double lengthCost = atof(m_lengthCost.c_str());
	multiScaleRecons->SetLengthCost(lengthCost);
	
	multiScaleRecons->Update();

	// check for errors
	if(multiScaleRecons->GetErrorCode() == 10)
	{
		wxString errorMsg( "Please segment coronary artery branches from 2-3 views.");
		wxMessageDialog dialogRun( NULL, errorMsg, "Reconstruction Failed", wxOK | wxICON_ERROR );
		dialogRun.ShowModal();
		return;
	}
	else if(multiScaleRecons->GetErrorCode() == 20)
	{
		wxString errorMsg( "No branches are reconstructed! Please change your parameters and retry.");
		wxMessageDialog dialogRun( NULL, errorMsg, "Reconstruction Failed", wxOK | wxICON_ERROR );
		dialogRun.ShowModal();
		return;
	}
	
	std::vector<std::string> surfPtsFileNames;
	std::vector<std::string> centPtsFileNames;
	for(unsigned int b=0; b<nb; b++)
	{
		std::ostringstream bstr;
		bstr << b;
		// write the surface points for each branch in the specified file
		std::string surfPtsFileName = m_homeOutputPath + "\\surfPts_branch" + bstr.str() + ".txt";
		surfPtsFileNames.push_back(surfPtsFileName);
		multiScaleRecons->WriteVnlMatrixToFile(surfPtsFileName, multiScaleRecons->GetSurfacePoints(b));

		// write the centerline points for each branch in the specified file 
		std::string centPtsFileName = m_homeOutputPath + "\\centPts_branch" + bstr.str() + ".txt";
		centPtsFileNames.push_back(centPtsFileName);
		multiScaleRecons->WriteVnlMatrixToFile(centPtsFileName, multiScaleRecons->GetCentrelinePoints(b));
	}

	// create vtk centerlines structures for all reconstructed branches
	for(unsigned int b=0; b<nb; b++)
	{
		vnl_matrix<TPrecision> centPt = multiScaleRecons->GetCentrelinePoints(b);
		if( centPt.size() == 0 ) // if not reconstructed, do not try to read ply file
		{
			continue;
		}
		std::ostringstream bstr;
		bstr << b;
		vtkSmartPointer<vtkPolyData> centPtPoly = vtkSmartPointer<vtkPolyData>::New();
		centPtPoly->DeepCopy(this->CreateVtkFromVnlMatrix(centPt));
		centPtPoly->Update();
		vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
		writer->SetInput(centPtPoly);
		std::string vtkFilename = m_homeOutputPath + "\\cent_branch" + bstr.str() + ".vtk";
		writer->SetFileName(vtkFilename.c_str());
		writer->Write();

		// Update the output CENTERLINE
		GetOutputPort( CENTERLINE )->SetDataEntityName("Centerline_Branch" + bstr.str());
		GetOutputPort( CENTERLINE )->UpdateOutput(centPtPoly, 0, GetInputDataEntity(CENTERLINE_SET));
	}
	

	std::vector<std::string> surfFileNames;
	for(unsigned int b=0; b<nb; b++)
	{
		vnl_matrix<TPrecision> surfPt = multiScaleRecons->GetSurfacePoints(b);
		if( surfPt.size() == 0 ) // if not reconstructed, do not call poisson surface reconstruction
		{
			std::cout << "Branch #" << b << " was not reconstructed!" << std::endl;
			continue;
		}
		// create file names
		std::ostringstream bstr;
		bstr << b;
		std::string exefile = m_poissonReconSelected;
		std::string surffilename = m_homeOutputPath + "\\surf_branch" + bstr.str() + ".ply";
		surfFileNames.push_back(surffilename);
		// construct command and execute
		std::string command;
		command = "\"" + exefile + "\"" + " --in " + surfPtsFileNames[b] + " --depth " + m_reconDepth
			+ " --pointWeight " + m_interpolationWeight + " --out " + surffilename;
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		wxString scriptDir = (settings->GetApplicationPathSubPath("PoissonSurfaceRecon") + "/").c_str();
		wxString oldDir = wxGetCwd();
		wxSetWorkingDirectory(scriptDir);
		wxExecute(command.c_str(), wxEXEC_SYNC);
		wxSetWorkingDirectory(oldDir);
	}

	for(unsigned int b=0; b<nb; b++)
	{
		vnl_matrix<TPrecision> surfPt = multiScaleRecons->GetSurfacePoints(b);
		if( surfPt.size() == 0 ) // if not reconstructed, do not try to read ply file
		{
			continue;
		}
		std::ostringstream bstr;
		bstr << b;
		vtkSmartPointer<vtkPLYReader> plyReader = vtkSmartPointer<vtkPLYReader>::New();
		plyReader->SetFileName ( surfFileNames[b].c_str() );
		vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
		writer->SetInputConnection(plyReader->GetOutputPort());
		std::string vtkFilename = m_homeOutputPath + "\\surf_branch" + bstr.str() + ".vtk";
		writer->SetFileName(vtkFilename.c_str());
		writer->Write();

		vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
		reader->SetFileName(vtkFilename.c_str());
		reader->Update();
		vtkSmartPointer<vtkPolyData> surface = vtkSmartPointer<vtkPolyData>::New();
		surface->DeepCopy(reader->GetOutput());
		surface->Update();
		// Update the output LUMEN_SURFACE
		GetOutputPort( LUMEN_SURFACE )->SetDataEntityName("Lumen-Surface_Branch" + bstr.str());
		GetOutputPort( LUMEN_SURFACE )->UpdateOutput(surface, 0, GetInputDataEntity(CENTERLINE_SET));
	}

	// warn user if some of the branches are not reconstructed
	std::vector<unsigned int> notReconstructed;
	for(unsigned int b=0; b<nb; b++)
	{
		vnl_matrix<TPrecision> surfPt = multiScaleRecons->GetSurfacePoints(b);
		if( surfPt.size() == 0 ) // if not reconstructed, do not try to read ply file
		{
			notReconstructed.push_back(b);
		}
	}
	if(notReconstructed.size() != 0)
	{
		std::string msg = "Not able to reconstruct branch #";
		for(unsigned int i=0; i<notReconstructed.size(); i++)
		{
			std::ostringstream bstr;
			bstr << notReconstructed[i];
			msg = msg + bstr.str() + " ";
		}
		msg = msg + ". Please make sure i) that you segment arteries from images that are in the same cardiac phase, or ii) calibration of X-ray device is correct.";
		wxString errorMsg( msg );
		wxMessageDialog dialogRun( NULL, errorMsg, "Reconstruction Failed", wxOK | wxICON_ERROR );
		dialogRun.ShowModal();
		return;
	}

}

vtkSmartPointer<vtkPolyData> LumenReconstructionProcessor::CreateVtkFromVnlMatrix(vnl_matrix<TPrecision> &centPts)
{
	// Manually generate the centerline polydata
	int numOfPts = centPts.rows();
	vtkSmartPointer<vtkPoints>    fcnOutPts  = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> fcnOutVert = vtkSmartPointer<vtkCellArray>::New();
	for(unsigned int j=0; j<numOfPts; j++)
	{
		double pt[3];
		for(unsigned int d=0; d<3; d++)
		{
			pt[d] = centPts(j,d);
		}
		vtkIdType pid[1];
		pid[0] = fcnOutPts->InsertNextPoint(pt);
		fcnOutVert->InsertNextCell(1,pid);
	} 
	vtkSmartPointer<vtkCellArray> fcnOutCells = vtkSmartPointer<vtkCellArray>::New();
	// for each branch create a line and add to the cell array
	int pointIdx = 0;
	vtkSmartPointer<vtkPolyLine> polyLine = vtkSmartPointer<vtkPolyLine>::New();
	polyLine->GetPointIds()->SetNumberOfIds( numOfPts );
	for(unsigned int j=0; j< numOfPts; j++)
	{
		polyLine->GetPointIds()->SetId(j, pointIdx);
		pointIdx++;
	}
	fcnOutCells->InsertNextCell(polyLine);
	// Add the points to the dataset
	vtkSmartPointer<vtkPolyData> segmentedCentreline = vtkSmartPointer<vtkPolyData>::New();
	segmentedCentreline->SetPoints(fcnOutPts);
	segmentedCentreline->SetLines(fcnOutCells);

	return segmentedCentreline;
}


void LumenReconstructionProcessor::SetOutlierWeight(std::string outlierWeight)
{
	m_outlierWeight = outlierWeight;
}

void LumenReconstructionProcessor::SetMaxIterationsCPD(std::string maxIterations)
{
	m_maxIterationsCPD = maxIterations;
}

void LumenReconstructionProcessor::SetTolerance(std::string tolerance)
{
	m_tolerance = tolerance;
}

void LumenReconstructionProcessor::SetEpipolarDistanceThreshold(std::string epipolarDistThresh)
{
	m_epipolarDistThresh = epipolarDistThresh;
}

void LumenReconstructionProcessor::SetSkip(std::string skipDistance)
{
	m_skipDistance = skipDistance;
}

void LumenReconstructionProcessor::SetSkipCost(std::string skipCost)
{
	m_skipCost = skipCost;
}

void LumenReconstructionProcessor::SetLengthCost(std::string lengthCost)
{
	m_lengthCost = lengthCost;
}

void LumenReconstructionProcessor::SetResamplingLevels(std::string resamplingLevels)
{
	m_resamplingLevels = resamplingLevels;
}

void LumenReconstructionProcessor::SetMaxIterationsOSB(std::string maxIterations)
{
	m_maxIterationsOSB = maxIterations;
}

void LumenReconstructionProcessor::SetReconstructionDepth(std::string reconDepth)
{
	m_reconDepth = reconDepth;
}

void LumenReconstructionProcessor::SetInterpolationWeight(std::string interpolationWeight)
{
	m_interpolationWeight = interpolationWeight;
}

std::vector<double> LumenReconstructionProcessor::GetDoubles(std::string paramStr)
{
	std::vector<double> doubleVec;
	std::size_t position = 0, startAt = 0;
	while (position != std::string::npos)
	{
		position = paramStr.find_first_of(",", startAt);
		std::string subStr = paramStr.substr(startAt, position);
		double param = atof(subStr.c_str());
		doubleVec.push_back(param);
		startAt = position + 1;
	}
	return doubleVec;
}

std::vector<unsigned int> LumenReconstructionProcessor::GetUInts(std::string paramStr)
{
	std::vector<unsigned int> uintVec;
	std::size_t position = 0, startAt = 0;
	while (position != std::string::npos)
	{
		position = paramStr.find_first_of(",", startAt);
		std::string subStr = paramStr.substr(startAt, position);
		unsigned int param = atoi(subStr.c_str());
		uintVec.push_back(param);
		startAt = position + 1;
	}
	return uintVec;
}

} // namespace cam
