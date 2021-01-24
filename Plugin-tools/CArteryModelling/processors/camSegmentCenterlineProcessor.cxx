/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#include "camSegmentCenterlineProcessor.h"

#include <string>
#include <iostream>

#include "coreReportExceptionMacros.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityHelper.txx"
#include "coreKernel.h"
#include "coreSettings.h"

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include "vtkSmartPointer.h"
#include "vtkDirectory.h"

namespace cam
{

SegmentCenterlineProcessor::SegmentCenterlineProcessor( )
{
	SetName( "SegmentCenterlineProcessor" );
	
	BaseProcessor::SetNumberOfInputs( NO_OF_INPUTS );
	GetInputPort(RA_IMAGE)->SetName( "RA Image" );
	GetInputPort(RA_IMAGE)->SetDataEntityType( Core::ImageTypeId);
	GetInputPort(RA_IMAGE)->SetUpdateMode( Core::BaseFilterInputPort::UPDATE_ACCESS_MULTIPLE_TIME_STEP );

	GetInputPort(START_END_POINTS)->SetName( "Start-End Points" );
	GetInputPort(START_END_POINTS)->SetDataEntityType( Core::PointSetTypeId );

	GetInputPort(CENTERLINE1)->SetName( "Segment-1" );
	GetInputPort(CENTERLINE1)->SetDataEntityType( Core::SkeletonTypeId );

	GetInputPort(CENTERLINE2)->SetName( "Segment-2" );
	GetInputPort(CENTERLINE2)->SetDataEntityType( Core::SkeletonTypeId );
	
	BaseProcessor::SetNumberOfOutputs( NO_OF_OUTPUTS );
	GetOutputPort( CENTERLINE )->SetName( "Segmented Artery Centerline" );
	GetOutputPort( CENTERLINE )->SetDataEntityName( "Segmented Centerline" );
	GetOutputPort( CENTERLINE )->SetDataEntityType( Core::SurfaceMeshTypeId | Core::SkeletonTypeId );
	GetOutputPort( CENTERLINE )->SetReuseOutput( false );

	GetOutputPort( HELPER_POINT )->SetName( "Helper Point (Select Artery)" );
	GetOutputPort( HELPER_POINT )->SetDataEntityName( "Helper Point" );
	GetOutputPort( HELPER_POINT )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetOutputPort( HELPER_POINT )->SetReuseOutput( true );

	GetOutputPort( CENTERLINE_SET )->SetName( "Set of Centerlines" );
	GetOutputPort( CENTERLINE_SET)->SetDataEntityName( "Centerline Set" );
	GetOutputPort( CENTERLINE_SET )->SetDataEntityType( Core::SurfaceMeshTypeId | Core::SkeletonTypeId );
	GetOutputPort( CENTERLINE_SET )->SetReuseOutput( true );

	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
	//! Following line currently not used
	//m_coeffToolPath = settings->GetApplicationPathSubPath("Coeff_tool") + "/Coeff_tool.exe";

	m_homeTmpPath = settings->GetProjectHomePath() + "\\tmp";
	vtkSmartPointer<vtkDirectory> dir = vtkSmartPointer<vtkDirectory>::New();
	if(dir->Open(m_homeTmpPath.c_str())==FALSE)
	{
		dir->MakeDirectory(m_homeTmpPath.c_str());
	}

	m_processType = SEGMENT;
	m_imageDataPath = "";
	m_currTimestep = -1;
	m_centerlineStack.clear();
	m_frameNumbers.clear();
}

SegmentCenterlineProcessor::~SegmentCenterlineProcessor()
{
	m_centerlineStack.clear();
	m_frameNumbers.clear();
	vtkSmartPointer<vtkDirectory> dir = vtkSmartPointer<vtkDirectory>::New();
	if(dir->Open(m_homeTmpPath.c_str())==TRUE)
	{
		dir->DeleteDirectory(m_homeTmpPath.c_str());
	}
}

void SegmentCenterlineProcessor::SetImageFrameNumber(std::string frameNum)
{
	m_FrameNum = frameNum;
}

void SegmentCenterlineProcessor::SetArteryBranchNumber(std::string branchNum)
{
	m_BranchNum = branchNum;
}

void SegmentCenterlineProcessor::SetResamplingRate(std::string resampleRate)
{
	m_resampleRate = resampleRate;
}

void SegmentCenterlineProcessor::SetSmoothWindowSize(std::string smoothWindowSize)
{
	m_smoothWindowSize = smoothWindowSize;
}

void SegmentCenterlineProcessor::SetStartRadius(signed long startRadius)
{
	m_startRadius = startRadius;
}

void SegmentCenterlineProcessor::SetEndRadius(signed long endRadius)
{
	m_endRadius = endRadius;
}

void SegmentCenterlineProcessor::SetProcessType(PROCESS_TYPE processType)
{
	m_processType = processType;
}

void SegmentCenterlineProcessor::SetImagePath(std::string imageDataPath)
{
	m_imageDataPath = imageDataPath;
}

void SegmentCenterlineProcessor::SetCurrTimeStep(int currTimestep)
{
	m_currTimestep = currTimestep;
}

int SegmentCenterlineProcessor::GetCurrTimeStep()
{
	return m_currTimestep;
}

void SegmentCenterlineProcessor::Update()
{
	this->GetInformationFromImage();

	switch (m_processType)
	{
		case SAMPLE:
		{
			this->CheckTubularityScore();
			this->SampleRadius();
			break;
		}
		case SEGMENT:
		{
			this->CheckTubularityScore();
			this->SegmentArtery();
			vtkSmartPointer<vtkPolyData> outData = vtkSmartPointer<vtkPolyData>::New();
			outData->DeepCopy(this->CreateVtkFromSwcOutput());
			outData->Update();

			// Update the output CENTERLINE
			GetOutputPort(CENTERLINE)->SetTotalTimeSteps(
				GetInputDataEntity(RA_IMAGE)->GetNumberOfTimeSteps());
			GetOutputPort( CENTERLINE )->SetDataEntityName( 
				"Centerline-Frame" + m_FrameNum + "_Branch" + m_BranchNum );
			GetOutputPort( CENTERLINE )->UpdateOutput( 
				outData, m_currTimestep, GetInputDataEntity(RA_IMAGE) );

			// Update the output CENTERLINE_SET
			m_centerlineStack.push_back(outData);
			GetOutputPort(CENTERLINE_SET)->SetTotalTimeSteps(m_centerlineStack.size());
			for (int i = 0; i < m_centerlineStack.size(); i++ )
			{
				GetOutputPort( CENTERLINE_SET )->UpdateOutput(m_centerlineStack[i], i, NULL);
			}
			// add frame number to the CENTERLINE_SET metadata
			this->AddFrameNumber();
			// Set the time step to initial state to prevent repetition of same artery segmentation
			m_currTimestep = -1;
			break;
		}
		case DUMMY:
		{
				vtkSmartPointer<vtkPolyData> outData = vtkSmartPointer<vtkPolyData>::New();
				// Update the output CENTERLINE
				GetOutputPort(CENTERLINE)->SetTotalTimeSteps(
					GetInputDataEntity(RA_IMAGE)->GetNumberOfTimeSteps());
				GetOutputPort( CENTERLINE )->SetDataEntityName( 
					"Centerline-Frame" + m_FrameNum + "_Branch" + m_BranchNum );
				GetOutputPort( CENTERLINE )->UpdateOutput( 
					outData, m_currTimestep, GetInputDataEntity(RA_IMAGE) );
				
				// Update the output CENTERLINE_SET
				m_centerlineStack.push_back(outData);
				GetOutputPort(CENTERLINE_SET)->SetTotalTimeSteps(m_centerlineStack.size());
				for (int i = 0; i < m_centerlineStack.size(); i++ )
				{
					GetOutputPort( CENTERLINE_SET )->UpdateOutput(m_centerlineStack[i], i, NULL);
				}
				// add frame number to the CENTERLINE_SET metadata
				this->AddFrameNumber();
				// Set the time step to initial state to prevent repetition of same artery segmentation
				m_currTimestep = -1;
				break;
		}
		case MERGE:
		{
			vtkSmartPointer<vtkPolyData> outData = vtkSmartPointer<vtkPolyData>::New();
			outData->DeepCopy(this->MergeSelectedArteryBranches());
			outData->Update();
			
			// Update the output CENTERLINE
			GetOutputPort(CENTERLINE)->SetTotalTimeSteps(
				GetInputDataEntity(RA_IMAGE)->GetNumberOfTimeSteps());
			GetOutputPort( CENTERLINE )->SetDataEntityName( 
				"Centerline-Frame" + m_FrameNum + "_Branch" + m_BranchNum );
			GetOutputPort( CENTERLINE )->UpdateOutput( 
				outData, m_currTimestep, GetInputDataEntity(RA_IMAGE) );

			m_centerlineStack.pop_back();
			m_centerlineStack.pop_back();
			m_centerlineStack.push_back(outData);
			// Update the output CENTERLINE_SET
			GetOutputPort(CENTERLINE_SET)->SetTotalTimeSteps(m_centerlineStack.size());
			for (int i = 0; i < m_centerlineStack.size(); i++ )
			{
				GetOutputPort( CENTERLINE_SET )->UpdateOutput(m_centerlineStack[i], i, NULL);
			}
			// add frame number to the CENTERLINE_SET metadata
			this->AddFrameNumber();
			// Set the time step to initial state to prevent repetition of same artery segmentation
			m_currTimestep = -1;
			break;
		}
		case DELETE_SEGMENT:
		{
			m_centerlineStack.pop_back();
			if (!m_centerlineStack.empty())
			{
				GetOutputPort(CENTERLINE_SET)->SetTotalTimeSteps(m_centerlineStack.size());
				// Update the output CENTERLINE_SET
				for (int i = 0; i < m_centerlineStack.size(); i++ )
				{
					GetOutputPort( CENTERLINE_SET )->UpdateOutput(m_centerlineStack[i], i, NULL);
				}
				// Delete frame number from the CENTERLINE_SET metadata
				this->DeleteFrameNumber();
				// Set the time step to initial state to prevent repetition of same artery segmentation
				m_currTimestep = -1;
			}
			else
			{
				Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
				Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
				if ( GetOutputDataEntity( SegmentCenterlineProcessor::CENTERLINE_SET ).IsNotNull() )
				{
					list->Remove( GetOutputDataEntity( SegmentCenterlineProcessor::CENTERLINE_SET ) );
				}
				m_centerlineStack.clear();
				m_frameNumbers.clear();
			}
			break;
		}
		default:
			break;
	}
}

void SegmentCenterlineProcessor::AddFrameNumber()
{
	unsigned int frameNum = std::stoul (m_FrameNum, 0, 0);
	std::vector<unsigned int>::const_iterator found = std::find(
		m_frameNumbers.begin(), m_frameNumbers.end(), frameNum);
	if (found == m_frameNumbers.end()) //! if not found
	{
		m_frameNumbers.push_back(frameNum);
		std::string frameNosStr = "";
		for (int i = 0; i < m_frameNumbers.size(); i++)
		{
			frameNosStr.append(boost::lexical_cast<std::string>(m_frameNumbers[i]));
			if (i != (m_frameNumbers.size()-1))
			{
				frameNosStr.append(",");
			}
		}
		Core::DataEntityMetadata::Pointer metadata = GetOutputDataEntity(CENTERLINE_SET)->GetMetadata();
		metadata->AddTag( "Frame Numbers", frameNosStr );
	} 
}

void SegmentCenterlineProcessor::DeleteFrameNumber()
{
	unsigned int frameNum = std::stoul (m_FrameNum, 0, 0);
	std::vector<unsigned int>::const_iterator found = std::find(
		m_frameNumbers.begin(), m_frameNumbers.end(), frameNum);
	if (found != m_frameNumbers.end() && m_BranchNum == "0") //! if found and branch number = 0 
	{
		m_frameNumbers.pop_back();
		std::string frameNosStr = "";
		for (int i = 0; i < m_frameNumbers.size(); i++)
		{
			frameNosStr.append(boost::lexical_cast<std::string>(m_frameNumbers[i]));
			if (i != (m_frameNumbers.size()-1))
			{
				frameNosStr.append(",");
			}
		}
		Core::DataEntityMetadata::Pointer metadata = GetOutputDataEntity(CENTERLINE_SET)->GetMetadata();
		metadata->AddTag( "Frame Numbers", frameNosStr );
	} 
}

void SegmentCenterlineProcessor::CheckTubularityScore()
{
	std::string tubularityFileName = m_homeTmpPath + "\\oof\\image" + m_timeStepInString + "_tubularity.nrrd";
	std::string tubularitySSFileName = m_homeTmpPath + "\\oof\\image" + m_timeStepInString + "_tubularitySS.nrrd";

	if ( !boost::filesystem::exists( tubularityFileName ) 
		|| !boost::filesystem::exists( tubularitySSFileName ) )
	{
		std::cout << "Computing tubularity" << std::endl;
		this->ComputeTubularityScore();
	}
	else
	{
		std::cout << "Tubularity is already computed" << std::endl;
	}
}


void SegmentCenterlineProcessor::UpdatePixelPts()
{
	// Get Image data
	ImageType::Pointer inputImage;
	GetProcessingData(RA_IMAGE, inputImage, m_currTimestep);
	// Get Points Data
	Core::vtkPolyDataPtr selectedPoints;
	GetProcessingData( START_END_POINTS, selectedPoints, m_currTimestep);
	if (selectedPoints)
	{
		ImageType::IndexType imageIndex;
		ImageType::PointType itkPointStart, itkPointEnd;
		for(unsigned int i=0;i<2;i++)
		{
			itkPointStart[i] = selectedPoints->GetPoint(0)[i];
			itkPointEnd[i] = selectedPoints->GetPoint(1)[i];
		}
		m_startPoint_X = selectedPoints->GetPoint(0)[0];
		m_startPoint_Y = selectedPoints->GetPoint(0)[1];

		if (inputImage->TransformPhysicalPointToIndex(itkPointStart, imageIndex))
		{
			m_startPixel_X = imageIndex[0];
			m_startPixel_Y = imageIndex[1];
		}
		if (inputImage->TransformPhysicalPointToIndex(itkPointEnd, imageIndex))
		{
			m_endPixel_X = imageIndex[0];
			m_endPixel_Y = imageIndex[1];
		}
	}
}

void SegmentCenterlineProcessor::ComputeTubularityScore()
{
	// Create the command to execute
	std::string computeTubularityScoreApp = "CArteryModellingLibApplications_ComputeTubularityScore.exe"; // at the same path as Gimias.exe
	std::string rawImageFilePath = m_imageDataPath + "\\raw\\image" + m_timeStepInString + ".raw"; // input image file name
	std::string vessFilePath = m_homeTmpPath + "\\oof\\image" + m_timeStepInString + "_tubularity.nrrd"; // tubularity score output file name
	bool generateSSImage = true; // generate scale-space image
	std::string vessSSFilePath = m_homeTmpPath + "\\oof\\image" + m_timeStepInString + "_tubularitySS.nrrd"; // scale-space output file name
	bool generateOFMatrixImage = false; // generate oriented flux image
	std::string orientedFluxFilePath = "0"; // oriented flux output file name
	bool generateNPlus1DOFMatrixImage = false; // generate scale image
	std::string NPlus1DOFMatrixImageFilePath = "0"; // scale image output file name
	bool generateScaleImage = false; // generate scale image
	std::string scaleImageFilePath = "0"; // scale image output file name
	double minSigma = 0.5;
	double maxSigma = 2.5;
	unsigned int numOfScales = 11; // number of sigma steps
	double fixedSigma = minSigma; // sigma for gauss derivatives (for smoothing)
	bool brightObject = false; // object relative grey level (bright=1/dark=0)
	bool takeExpOfScoreImage = false; // Take exponential of score image (yes=1/no=0)
	double maxContrastRatio  = 0.0; // maximum contrast ratio
	unsigned int imageSizeX = m_imageSizeX;
	unsigned int imageSizeY = m_imageSizeY;
	double imageSpacingX = m_imageSpacingX;
	double imageSpacingY = m_imageSpacingY;

	// Check for the 'oof' folder at the specified path, if doesn't exists create one
	std::string oofPath = m_homeTmpPath + "\\oof";
	vtkSmartPointer<vtkDirectory> dir = vtkSmartPointer<vtkDirectory>::New();
	if(dir->Open(oofPath.c_str())==FALSE)
	{
		dir->MakeDirectory(oofPath.c_str());
	}

	std::string cmdToExecute;
	cmdToExecute =  computeTubularityScoreApp
						+ " " + "\"" + rawImageFilePath + "\"" 
						+ " " + vessFilePath  
						+ " " + boost::lexical_cast<std::string>(generateSSImage)
						+ " " + vessSSFilePath
						+ " " + boost::lexical_cast<std::string>(generateOFMatrixImage)
						+ " " + orientedFluxFilePath
						+ " " + boost::lexical_cast<std::string>(generateNPlus1DOFMatrixImage)
						+ " " + NPlus1DOFMatrixImageFilePath
						+ " " + boost::lexical_cast<std::string>(generateScaleImage)
						+ " " + scaleImageFilePath
						+ " " + boost::lexical_cast<std::string>(minSigma)
						+ " " + boost::lexical_cast<std::string>(maxSigma)
						+ " " + boost::lexical_cast<std::string>(numOfScales)
						+ " " + boost::lexical_cast<std::string>(fixedSigma)
						+ " " + boost::lexical_cast<std::string>(brightObject)
						+ " " + boost::lexical_cast<std::string>(takeExpOfScoreImage)
						+ " " + boost::lexical_cast<std::string>(maxContrastRatio)
						+ " " + boost::lexical_cast<std::string>(imageSizeX)
						+ " " + boost::lexical_cast<std::string>(imageSizeY)
						+ " " + boost::lexical_cast<std::string>(imageSpacingX)
						+ " " + boost::lexical_cast<std::string>(imageSpacingY);

	wxExecute(cmdToExecute.c_str(), wxEXEC_SYNC);	
}


void SegmentCenterlineProcessor::SegmentArtery()
{
	// Create the command to execute
	std::string segmentArteryApp = "CArteryModellingLibApplications_SegmentArtery.exe"; // at the same path as Gimias.exe
	std::string vessSSFilePath = m_homeTmpPath + "\\oof\\image" + m_timeStepInString + "_tubularitySS.nrrd"; // scale-space input file name
	std::string rawImageFilePath = m_imageDataPath + "\\raw\\image" + m_timeStepInString + ".raw"; // input image file name
	signed long startPixel_X = m_startPixel_X; // will be determined by user click, between 0 and imageSizeX-1
	signed long startPixel_Y = m_startPixel_Y; // will be determined by user click, between 0 and imageSizeY-1
	signed long endPixel_X = m_endPixel_X; // will be determined by user click, between 0 and imageSizeX-1
	signed long endPixel_Y = m_endPixel_Y; // will be determined by user click, between 0 and imageSizeY-1
	bool computeOnSubRegionOnly = false; // compute the path in a sub region of the image (1=yes/0=no)
	int subRegionPaddingSize = 0; // sub region padding size
	std::string outputSWCFilePath = m_homeTmpPath + "\\swc\\output.swc"; // output tubular path in swc format
	bool resampleFlag = true; // 0 turns resampling off, 1 turns resampling on
	bool smoothFlag = true; // 0 turns smoothing off, 1 turns smoothing on
	unsigned int imageSizeX = m_imageSizeX;
	unsigned int imageSizeY = m_imageSizeY;
	double imageSpacingX = m_imageSpacingX;
	double imageSpacingY = m_imageSpacingY;

	signed long startPixel_R = m_startRadius; // typically a value between 0 and numOfScales-1. Set to -1 to indicate that it should be determined by application itself
	signed long endPixel_R = m_endRadius; // typically a value between 0 and numOfScales-1. Set to -1 to indicate that it should be determined by application itself

	// Check for the 'swc' folder at the specified path, if doesn't exists create one
	std::string swcPath = m_homeTmpPath + "\\swc";
	vtkSmartPointer<vtkDirectory> dir = vtkSmartPointer<vtkDirectory>::New();
	if(dir->Open(swcPath.c_str())==FALSE)
	{
		dir->MakeDirectory(swcPath.c_str());
	}

	std::string cmdToExecute;
	cmdToExecute =  segmentArteryApp
		+ " " + vessSSFilePath 
		+ " "  + "\"" + rawImageFilePath + "\""  
		+ " " + boost::lexical_cast<std::string>(startPixel_X)
		+ " " + boost::lexical_cast<std::string>(startPixel_Y)
		+ " " + boost::lexical_cast<std::string>(startPixel_R)
		+ " " + boost::lexical_cast<std::string>(endPixel_X)
		+ " " + boost::lexical_cast<std::string>(endPixel_Y)
		+ " " + boost::lexical_cast<std::string>(endPixel_R)
		+ " " + boost::lexical_cast<std::string>(computeOnSubRegionOnly)
		+ " " + boost::lexical_cast<std::string>(subRegionPaddingSize)
		+ " " + outputSWCFilePath
		+ " " + boost::lexical_cast<std::string>(resampleFlag)
		+ " " + m_resampleRate
		+ " " + boost::lexical_cast<std::string>(smoothFlag)
		+ " " + m_smoothWindowSize
		+ " " + boost::lexical_cast<std::string>(imageSizeX)
		+ " " + boost::lexical_cast<std::string>(imageSizeY)
		+ " " + boost::lexical_cast<std::string>(imageSpacingX)
		+ " " + boost::lexical_cast<std::string>(imageSpacingY);

	wxExecute(cmdToExecute.c_str(), wxEXEC_SYNC);
}

vtkSmartPointer<vtkPolyData> SegmentCenterlineProcessor::CreateVtkFromSwcOutput()
{
	// Read swc output file into a vnl matrix
	// 7x(number of points) size matrix. Each row has the following structure:
	// vertex id(ignore), point type(ignore), x coordnate of point, y coordinate of point, z coordinate of point(ignore), radius of point, parent id(ignore)
	std::string outputSWCFilePath = m_homeTmpPath + "\\swc\\output.swc"; // output tubular path in swc format
	vnl_matrix<double> swcOut; //swcOut.print(std::cout);
	vcl_ifstream swcOutFileStream(outputSWCFilePath.c_str());
	if (!swcOut.read_ascii(swcOutFileStream))
	{
        wxString errorMsg( "Please check for following file: \n\n" + outputSWCFilePath );
		wxMessageDialog dialogRun( NULL, errorMsg, "File(s) Missing", wxOK | wxICON_ERROR );
		dialogRun.ShowModal();
		return NULL;
    }
	
	// Manually generate the centerline polydata
	int numOfPts = swcOut.rows();
	vtkSmartPointer<vtkPoints>    fcnOutPts  = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> fcnOutVert = vtkSmartPointer<vtkCellArray>::New();
	for(unsigned int j=0; j<numOfPts; j++)
	{
		double pt[3];
		// for the first point, perform manual correction
		// if we select the start point as a point from another branch, it is most likely expressed in physical coordinates.
		// however, we convert it to a itkIndex and feed it to "SegmentArtery". Due to this reason, the start point of the
		// segmentation is not the same as the selected start point.
		if(j==0)
		{
			pt[0] = m_startPoint_X;
			pt[1] = m_startPoint_Y;
		}
		else
		{
			pt[0] = swcOut.get(j,2);
			pt[1] = swcOut.get(j,3);
		}
		pt[2] = 0.0001;
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
	// Add the radius information as an extra array
	vtkSmartPointer<vtkFloatArray> radius = vtkSmartPointer<vtkFloatArray>::New(); 
	radius->SetNumberOfComponents(1);
	radius->SetName("Radius");
	for(vtkIdType j=0; j< numOfPts; j++)
	{
		double r = swcOut.get(j,5);
		radius->InsertNextValue(r);
	}
	segmentedCentreline->GetPointData()->AddArray(radius);

	return segmentedCentreline;
}

vtkSmartPointer<vtkPolyData> SegmentCenterlineProcessor::MergeSelectedArteryBranches()
{
	// get input
	vtkSmartPointer<vtkPolyData> b1;
	GetProcessingData(CENTERLINE1, b1, m_currTimestep);

	vtkSmartPointer<vtkPolyData> b2;
	GetProcessingData(CENTERLINE2, b2, m_currTimestep);

	vtkSmartPointer<vtkKdTreePointLocator> kDTree = vtkSmartPointer<vtkKdTreePointLocator>::New();
	kDTree->SetDataSet(b1);
	kDTree->BuildLocator();

	double firstPt_b2[3];
	b2->GetPoint(0,firstPt_b2); // firstPt_b2[0] = 485; firstPt_b2[1] = 259; firstPt_b2[2] = 1e-5;
	vtkIdType iD = kDTree->FindClosestPoint(firstPt_b2);

	double closestPt_b1[3];
	kDTree->GetDataSet()->GetPoint(iD, closestPt_b1); // iD is zero-based
	std::cout << "The closest point is point " << iD << std::endl;
	std::cout << "Coordinates: " << closestPt_b1[0] << " " << closestPt_b1[1] << " " << closestPt_b1[2] << std::endl;

	double distSqr = 0.0;
	double tol = 1e-10;
	for(unsigned int i=0; i<2; i++)
	{
		double val = firstPt_b2[i] - closestPt_b1[i];
		distSqr += (val * val);
	}
	vtkSmartPointer<vtkPolyData> newPolyData = vtkSmartPointer<vtkPolyData>::New();
	if( distSqr < 0.01 ) // or dist < 0.1
	{
		// create a new polydata

		// points & radius
		vtkSmartPointer<vtkPoints> newPts  = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkFloatArray> newRadius = vtkSmartPointer<vtkFloatArray>::New(); 
		newRadius->SetNumberOfComponents(1);
		newRadius->SetName("Radius");

		vtkDataArray* radius_b1 = b1->GetPointData()->GetArray("Radius");
		unsigned int ptId=0;
		for(unsigned int ptId=0; ptId<iD+1; ptId++)
		{
			double pt[3];
			b1->GetPoint(ptId, pt);
			vtkIdType pid[1];
			pid[0] = newPts->InsertNextPoint(pt);

			double r = radius_b1->GetTuple1(ptId);
			newRadius->InsertNextValue(r);
		}
		if(distSqr <= tol) // if the last of the first branch is the same with first point of the second branch
		{
			ptId = 1;
		}
		else
		{
			ptId = 0;
		}
		vtkDataArray* radius_b2 = b2->GetPointData()->GetArray("Radius");
		int numOfPts_b2 = b2->GetNumberOfPoints();
		for(ptId; ptId<numOfPts_b2; ptId++)
		{
			double pt[3];
			b2->GetPoint(ptId, pt);
			vtkIdType pid[1];
			pid[0] = newPts->InsertNextPoint(pt);

			double r = radius_b2->GetTuple1(ptId);
			newRadius->InsertNextValue(r);
		}
		int numOfTotalPts = newPts->GetNumberOfPoints();

		// smooth points & radius - TODO
		vtkSmartPointer<vtkIdList> subpathToSmooth = FindSubPath(newPts, iD, 2.0);
		for(vtkIdType j = 0; j < subpathToSmooth->GetNumberOfIds(); j++)
		{
			vtkIdType midId = subpathToSmooth->GetId(j);
			vtkSmartPointer<vtkIdList> subpath = FindSubPath(newPts, midId, 2.0);
			double smooth_pt[3];
			double smooth_r = 0.0;
			for( unsigned int i=0;i<2;i++ )
			{
				smooth_pt[i] = 0.0;
			}
			int numOfPtsInSubPath = subpath->GetNumberOfIds();
			for(vtkIdType k = 0; k < numOfPtsInSubPath; k++) //find average and write it to current point
			{
				double pt[3];
				newPts->GetPoint(subpath->GetId(k), pt);
				for( unsigned int i=0;i<2;i++ )
				{
					smooth_pt[i] += pt[i];
				}
				smooth_r += newRadius->GetTuple1(subpath->GetId(k));
			}
			for( unsigned int i=0;i<2;i++ )
			{
				smooth_pt[i] *= (1.0 / static_cast<double>(numOfPtsInSubPath));
			}
			smooth_pt[2] = 1e-5;
			smooth_r *= (1.0 / static_cast<double>(numOfPtsInSubPath));

			newPts->SetPoint(midId, smooth_pt);
			newRadius->SetTuple1(midId, smooth_r);

			//double pt[3];
			//newPts->GetPoint(subpathToSmooth->GetId(j), pt);
			//std::cout << subpathToSmooth->GetId(j) << " " << pt[0] << " " << pt[1] << " " << pt[2] << std::endl;
		}

		// add lines
		vtkSmartPointer<vtkCellArray> newCells = vtkSmartPointer<vtkCellArray>::New();
		int pointIdx = 0;
		vtkSmartPointer<vtkPolyLine> newPolyLine = vtkSmartPointer<vtkPolyLine>::New();
		newPolyLine->GetPointIds()->SetNumberOfIds( numOfTotalPts );
		for(unsigned int i=0; i< numOfTotalPts; i++)
		{
			newPolyLine->GetPointIds()->SetId(i, pointIdx);
			pointIdx++;
		}
		newCells->InsertNextCell(newPolyLine);

		// add the points and cells to the dataset
		newPolyData->SetPoints(newPts);
		newPolyData->SetLines(newCells);
		newPolyData->GetPointData()->AddArray(newRadius);
	}
	else
	{
		newPolyData = NULL;
	}

	return newPolyData;
}

vtkSmartPointer<vtkIdList> SegmentCenterlineProcessor::FindSubPath(vtkSmartPointer<vtkPoints> line, vtkIdType midId, double windowsize)
{
	int numOfPts = line->GetNumberOfPoints();

	vtkIdType startId, endId, idIt;
	idIt = midId;
	double traversedDist = 0.0;
	while( traversedDist < (windowsize/2.0) ) // back
	{
		startId = idIt;

		double pt1[3];
		line->GetPoint(idIt, pt1);

		idIt = idIt - 1;
		if( idIt <= 0 )
		{
			break;
		}

		double pt2[3];
		line->GetPoint(idIt, pt2);

		double dist = 0.0;
		for(unsigned int i=0; i<2; i++)
		{
			double val = pt1[i] - pt2[i];
			dist += (val * val);
		}
		dist = vcl_sqrt(dist);

		traversedDist += dist;
	}
	idIt = midId;
	traversedDist = 0.0;
	while( traversedDist < (windowsize/2.0) ) // forward
	{
		endId = idIt;

		double pt1[3];
		line->GetPoint(idIt, pt1);

		idIt = idIt + 1;
		if( idIt >= numOfPts-1 )
		{
			break;
		}

		double pt2[3];
		line->GetPoint(idIt, pt2);

		double dist = 0.0;
		for(unsigned int i=0; i<2; i++)
		{
			double val = pt1[i] - pt2[i];
			dist += (val * val);
		}
		dist = vcl_sqrt(dist);

		traversedDist += dist;
	}

	vtkSmartPointer<vtkIdList> subpathIds = vtkSmartPointer<vtkIdList>::New();
	for(vtkIdType id = startId; id <= endId; id++)
	{
		subpathIds->InsertNextId(id);
	}

	return subpathIds;
}

void SegmentCenterlineProcessor::SampleRadius()
{
	// Create the command to execute
	std::string sampleRadiusApp = "CArteryModellingLibApplications_SampleRadius.exe"; // at the same path as Gimias.exe
	std::string vessSSFilePath = m_homeTmpPath + "\\oof\\image" + m_timeStepInString + "_tubularitySS.nrrd"; // scale-space input file name
	std::string sampledRadiusFilePath = m_homeTmpPath + "\\SampleRadius.txt"; // sampled radius file name
	signed long startPixel_X = m_startPixel_X; // will be determined by user click, between 0 and imageSizeX-1
	signed long startPixel_Y = m_startPixel_Y; // will be determined by user click, between 0 and imageSizeY-1
	signed long endPixel_X = m_endPixel_X; // will be determined by user click, between 0 and imageSizeX-1
	signed long endPixel_Y = m_endPixel_Y; // will be determined by user click, between 0 and imageSizeY-1

	std::string cmdToExecute;
	cmdToExecute =  sampleRadiusApp
		+ " " + vessSSFilePath 
		+ " " + sampledRadiusFilePath
		+ " " + boost::lexical_cast<std::string>(startPixel_X)
		+ " " + boost::lexical_cast<std::string>(startPixel_Y)
		+ " " + boost::lexical_cast<std::string>(endPixel_X)
		+ " " + boost::lexical_cast<std::string>(endPixel_Y);

	wxExecute(cmdToExecute.c_str(), wxEXEC_SYNC);	
}

void SegmentCenterlineProcessor::GetInformationFromImage()
{
	// Get image data
	ImageType::Pointer inputImage;
	GetProcessingData(RA_IMAGE, inputImage, m_currTimestep);
	m_imageSizeX = inputImage->GetLargestPossibleRegion().GetSize()[0];
	m_imageSizeY = inputImage->GetLargestPossibleRegion().GetSize()[1];
	m_imageSpacingX = inputImage->GetSpacing()[0];
	m_imageSpacingY = inputImage->GetSpacing()[1];

	// Convert time step to string
	std::ostringstream convertToStr; 
	convertToStr << std::setw(3) << std::setfill('0') << m_currTimestep + 5; //raw image starts at image005.raw 
	m_timeStepInString = convertToStr.str();
}

} // namespace cam
