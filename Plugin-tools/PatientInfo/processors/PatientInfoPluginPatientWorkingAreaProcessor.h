/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#ifndef _PatientInfoPluginPatientWorkingAreaProcessor_H
#define _PatientInfoPluginPatientWorkingAreaProcessor_H

#include "coreBaseProcessor.h"

namespace patientInfoPlugin
{

/**
Processor for ...

\ingroup PatientInfoPlugin
\author Vignesh Rammohan
\date 17 August 2015
*/
class PatientWorkingAreaProcessor : public Core::BaseProcessor
{
public:

	typedef itk::Image<float,3> ImageType;

	typedef enum
	{
		INPUT_0,
		INPUT_1,
		INPUTS_NUMBER
	} INPUT_TYPE;

	typedef enum
	{
		OUTPUT_0,
		OUTPUT_1,
		OUTPUT_2,
		OUTPUTS_NUMBER
	}OUTPUT_TYPE;
public:
	//!
	coreProcessor(PatientWorkingAreaProcessor, Core::BaseProcessor);
	
	//! Call library to perform operation
	void Update( );

private:
	//!
	PatientWorkingAreaProcessor();

	//!
	~PatientWorkingAreaProcessor();

	//! Purposely not implemented
	PatientWorkingAreaProcessor( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );
}; // class PatientWorkingAreaProcessor
    
} // namespace patientInfoPlugin

#endif //_PatientInfoPluginPatientWorkingAreaProcessor_H
