/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#ifndef _camSegmentCenterlineProcessor_H
#define _camSegmentCenterlineProcessor_H

#include "coreBaseProcessor.h"
#include "camRAImageSequenceReader.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include <vcl_fstream.h>
#include <vnl/vnl_matrix.h>
#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkFloatArray.h>
#include "vtkPoints.h"
#include "vtkPolyLine.h"
#include "vtkCellArray.h"
#include <vtkPolyData.h>
#include <vtkIdList.h>
#include <vtkKdTreePointLocator.h>
#include <vtkPolyDataWriter.h>

namespace cam
{

/**
\brief Processor for segmentation (semi-automatic) of the centerline
from angiography images using the method (Tubular Geodesics using
Oriented Flux: An ITK Implementation) referred in \see section

\see http://hdl.handle.net/10380/3398

\ingroup CoronaryArteryModeling
\author Vignesh Rammohan
\date 30 July 2015
*/
class SegmentCenterlineProcessor : public Core::BaseProcessor
{
public:

	typedef enum
	{
		RA_IMAGE,
		START_END_POINTS,
		CENTERLINE1,
		CENTERLINE2,
		NO_OF_INPUTS
	} INPUT_TYPE;

	typedef enum
	{
		CENTERLINE,
		HELPER_POINT,
		CENTERLINE_SET,
		NO_OF_OUTPUTS
	}OUTPUT_TYPE;

	typedef enum
	{
		SAMPLE,
		SEGMENT,
		DUMMY,
		MERGE,
		DELETE_SEGMENT,
		NO_OF_PROCESSES
	}PROCESS_TYPE;

private:

	typedef RAImageSequenceReader::ImageType		ImageType;

	//! Path to the Philips Research's Coeff_tool.exe, a command-line program to extract geometric
	//! information acquired during a scan. This is a company confidential program and distribution
	//! is forbidden!!!
	//std::string m_coeffToolPath; //! not used currently

	std::string m_imageDataPath;
	std::string m_homeTmpPath;

	std::string m_FrameNum; // image frame number
	std::string m_BranchNum; // artery branch number
	std::string m_resampleRate; // resampling rate in mm
	std::string m_smoothWindowSize; // smoothing window radius size rate in mm
	signed long m_startRadius; // radius calculated from the start point(landmark)
	signed long m_endRadius; // radius calculated from the end point(landmark)

	unsigned int m_imageSizeX;
	unsigned int m_imageSizeY;
	double m_imageSpacingX;
	double m_imageSpacingY;

	PROCESS_TYPE m_processType;

	signed long m_startPixel_X;
	signed long m_startPixel_Y;
	double m_startPoint_X;
	double m_startPoint_Y;
	signed long m_endPixel_X;
	signed long m_endPixel_Y;

	int m_currTimestep;
	std::string m_timeStepInString;
	std::vector<boost::any> m_centerlineStack;
	std::vector<unsigned int> m_frameNumbers;

public:
	//!
	coreProcessor(SegmentCenterlineProcessor, Core::BaseProcessor);
	
	//! Call library to perform operation
	void Update( );

	void SetImageFrameNumber(std::string frameNum);
	void SetArteryBranchNumber(std::string branchNum);
	void SetResamplingRate(std::string resampleRate);
	void SetSmoothWindowSize(std::string smoothWindowSize);
	void SetStartRadius(signed long startRadius);
	void SetEndRadius(signed long endRadius);
	void SetProcessType(PROCESS_TYPE processType);
	void SetImagePath(std::string imageDataPath);
	void SetCurrTimeStep(int currTimestep);
	int GetCurrTimeStep();

	void GetInformationFromImage();
	void SampleRadius();
	void SegmentArtery();
	void CheckTubularityScore();
	void ComputeTubularityScore();
	vtkSmartPointer<vtkPolyData> CreateVtkFromSwcOutput();
	void UpdatePixelPts();
	vtkSmartPointer<vtkPolyData> MergeSelectedArteryBranches();
	vtkSmartPointer<vtkIdList> FindSubPath(vtkSmartPointer<vtkPoints> line, vtkIdType midId, double windowsize);
	void AddFrameNumber();
	void DeleteFrameNumber();

private:
	//!
	SegmentCenterlineProcessor();

	//!
	~SegmentCenterlineProcessor();

	//! Purposely not implemented
	SegmentCenterlineProcessor( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );
}; // class SegmentCenterlineProcessor
    
} // namespace cam

#endif // _camSegmentCenterlineProcessor_H
