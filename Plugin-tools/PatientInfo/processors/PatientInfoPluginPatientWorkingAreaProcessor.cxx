/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#include "PatientInfoPluginPatientWorkingAreaProcessor.h"

#include <string>
#include <iostream>

#include "coreReportExceptionMacros.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityHelper.txx"
#include "coreKernel.h"
#include "coreVTKPolyDataHolder.h"
#include "coreVTKImageDataHolder.h"

#include "vtkSmartPointer.h"

namespace patientInfoPlugin
{

PatientWorkingAreaProcessor::PatientWorkingAreaProcessor( )
{
	SetName( "PatientWorkingAreaProcessor" );
	
	BaseProcessor::SetNumberOfInputs( INPUTS_NUMBER );
	GetInputPort(INPUT_0)->SetName(  "Input Image" );
	GetInputPort(INPUT_0)->SetDataEntityType( Core::ImageTypeId);
	GetInputPort(INPUT_1)->SetName(  "Input Surface" );
	GetInputPort(INPUT_1)->SetDataEntityType( Core::SurfaceMeshTypeId);
	BaseProcessor::SetNumberOfOutputs( OUTPUTS_NUMBER );
	
}

PatientWorkingAreaProcessor::~PatientWorkingAreaProcessor()
{
}

void PatientWorkingAreaProcessor::Update()
{
	// Get the first image
	ImageType::Pointer itkInputImage;
	GetProcessingData(INPUT_0, itkInputImage);

	Core::vtkPolyDataPtr vtkInput;
	GetProcessingData(INPUT_1, vtkInput );
		
	Core::vtkImageDataPtr vtkInputImage;
	GetProcessingData(INPUT_0, vtkInputImage );
		
	// here goes the filter or the functions that determine the processor
	// the output should go in the update functions
	
	// Set the output to the output of this processor
	UpdateOutput( 0, itkInputImage, "PatientWorkingAreaProcessorImage");	
	UpdateOutput( 1, vtkInput, "PatientWorkingAreaProcessorSurface");
	UpdateOutput( 2, vtkInputImage, "PatientWorkingAreaProcessorImageVTK");
}

} // namespace patientInfoPlugin
