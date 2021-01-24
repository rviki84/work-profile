/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TuoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#include "CoronaryArteryModelingProcessorCollective.h"

#include "coreDataEntityReader.h"
#include "camRAImageSequenceReader.h"

namespace cam
{

ProcessorCollective::ProcessorCollective()
{
	Core::IO::DataEntityReader::RegisterFormatReader( 
		Core::IO::BaseDataEntityReader::Pointer( RAImageSequenceReader::New()) );
}

ProcessorCollective::~ProcessorCollective()
{
	Core::IO::DataEntityReader::UnRegisterFormatReader( 
		Core::IO::DataEntityReader::GetRegisteredReader( RAImageSequenceReader::GetNameClass() ) );
}

} // namespace cam{
