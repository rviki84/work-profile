/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TuoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CoronaryArteryModelingProcessorCollective_H
#define _CoronaryArteryModelingProcessorCollective_H

#include "coreSmartPointerMacros.h"
#include "coreObject.h"

namespace cam
{

/**
This class instantiates all processors used in the plugin and registers them.

\ingroup CoronaryArteryModeling
*/

class ProcessorCollective : public Core::SmartPointerObject
{
public:
	//!
	coreDeclareSmartPointerClassMacro(ProcessorCollective, Core::SmartPointerObject);

private:
	//! The constructor instantiates all the processors and connects them.
	ProcessorCollective();
	~ProcessorCollective();


}; // class ProcessorCollective

} // namespace cam{

#endif //_CoronaryArteryModelingProcessorCollective_H
