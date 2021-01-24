/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#ifndef _PatientInfoPluginProcessorCollective_H
#define _PatientInfoPluginProcessorCollective_H

#include "coreSmartPointerMacros.h"
#include "coreObject.h"

namespace patientInfoPlugin
{

/**
This class instantiates all processors used in the plugin and registers them.

\ingroup PatientInfoPlugin
*/

class ProcessorCollective : public Core::SmartPointerObject
{
public:
	//!
	coreDeclareSmartPointerClassMacro(ProcessorCollective, Core::SmartPointerObject);

private:
	//! The constructor instantiates all the processors and connects them.
	ProcessorCollective();

}; // class ProcessorCollective

} // namespace patientInfoPlugin{

#endif //_PatientInfoPluginProcessorCollective_H
