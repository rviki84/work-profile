/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#ifndef _camLumenReconstructionProcessor_H
#define _camLumenReconstructionProcessor_H

#include "coreBaseProcessor.h"
#include "camPerspectiveRAGeometry.h"
#include "camMultiscaleOsbReconstruction.h"

namespace cam
{

/**
\brief Processor for Matching & Motion compensation and Surface Reconstruction
based on following steps after the segmentation of centerlines from angiography images.

Matching & Motion compensation:
------------------------------
1) find corresponding centerline points between different angiography images
2) use the corresponding points to create 3D reconstruction
3) re-project the 3D reconstruction onto the angiography images
4) register the segmented centerlines onto the re-projection of the reconstruction
Iterate steps 1 to 4 to find reconstructed centerline points.

Steps 1 and 2 use implementation of optimal subsequence bijection
( http://ieeexplore.ieee.org/xpls/abs_all.jsp?arnumber=4470291&tag=1 ). The implementation
uses some image structures from ITK.

Step 4 uses implementation of the coherent point drift algorithm
https://sites.google.com/site/myronenko/research/cpd )

Fit ellipses and convert to surface:
-----------------------------------
After getting the reconstruction of the centerlines, use the diameter information extracted during 
the segmentation of centerlines, to fit ellipses around centerline points.
Use the points on the ellipses as input to Poisson
surface reconstruction ( http://www.cs.jhu.edu/~misha/Code/PoissonRecon/Version8.0/ ) to
obtain a triangular surface mesh.


\ingroup CoronaryArteryModeling
\author Vignesh Rammohan
\date 10 November 2015
*/
class LumenReconstructionProcessor : public Core::BaseProcessor
{
public:

	typedef enum
	{
		CENTERLINE_SET,
		NO_OF_INPUTS
	} INPUT_TYPE;

	typedef enum
	{
		LUMEN_SURFACE,
		CENTERLINE,
		NO_OF_OUTPUTS
	}OUTPUT_TYPE;

	typedef double							       TPrecision;
	typedef cam::PerspectiveRAGeometry<TPrecision> PerspectiveGeometryType;

private:

	//! Path to the Screened Poisson Surface Reconstruction executables (64 and 32 bit).
	//! Poisson surface reconstruction creates watertight surfaces from oriented point sets.
	//! Refer the class description above for more details.
	std::string m_poissonReconX64Path;
	std::string m_poissonReconWin32Path;
	std::string m_poissonReconSelected;

	std::string m_homeOutputPath;

	//! Registration parameters
	std::string m_outlierWeight;
	std::string m_maxIterationsCPD;
	std::string m_tolerance;
	
	//! Optimal Subsequence Bijection(OSB) parameters 
	std::string m_epipolarDistThresh;
	std::string m_resamplingLevels;
	std::string m_maxIterationsOSB;
	std::string m_skipDistance;
	std::string m_skipCost;
	std::string m_lengthCost;

	//! Poisson surface reconstruction parameters
	std::string m_reconDepth;
	std::string m_interpolationWeight;

public:
	//!
	coreProcessor(LumenReconstructionProcessor, Core::BaseProcessor);
	
	//! Call library to perform operation
	void Update( );

	void ReconstructLumenRegion();

	//! Set Registration parameters
	void SetOutlierWeight(std::string outlierWeight);
	void SetMaxIterationsCPD(std::string maxIterations);
	void SetTolerance(std::string tolerance);

	//! Set OSB parameters
	void SetEpipolarDistanceThreshold(std::string epipolarDistThresh);
	void SetSkip(std::string skipDistance);
	void SetSkipCost(std::string skipCost);
	void SetLengthCost(std::string lengthCost);
	void SetResamplingLevels(std::string resamplingLevels);
	void SetMaxIterationsOSB(std::string maxIterations);
	
	//! Set Poisson surface reconstruction parameters 
	void SetReconstructionDepth(std::string reconDepth);
	void SetInterpolationWeight(std::string interpolationWeight);

	//! Create vtk poly data from vnl matrix
	vtkSmartPointer<vtkPolyData> CreateVtkFromVnlMatrix(vnl_matrix<TPrecision> &centPts);

	//! std::string to 'double' parser
	std::vector<double> GetDoubles(std::string paramStr);
	//! std::string to 'unsigned int' parser
	std::vector<unsigned int> GetUInts(std::string paramStr);

private:
	//!
	LumenReconstructionProcessor();

	//!
	~LumenReconstructionProcessor();

	//! Purposely not implemented
	LumenReconstructionProcessor( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );
}; // class LumenReconstructionProcessor
    
} // namespace cam

#endif // _camLumenReconstructionProcessor_H
