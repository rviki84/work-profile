# Registration tool for the reconstructed coronary artery 3D surface registration
Matlab based tool to register a reconstructed coronary artery 3D surface segment onto a whole artery surface with branches

Steps involved are,
- Place the input STL files, 2 surfaces a fixed and a moving one that to be registered in the input folder 'surfaces_stl_input'.
- Replace the filenames in the Matlab script files `test_transform.m` and `register_surface.m`.
- Also replace the output file name in the Matlab script files that will be saved into the output folder 'surfaces_stl_output'.
- Obtain the initial transformation matrix for instance from Meshlab tool. This registers the surface roughly as it is done manually using point based registration.
- Replace the 'Tinit' variable in the script files with the above obtained matrix and set the 'scalingSwitch' with appropriate value. If the transformation involves scaling then use `scalingSwitch='ON'` otherwise `OFF`.
- Test the transformation first by running the script `test_transform.m` to check whether the applied transformation yields intended result.
- Finally run the script `register_surface.m` to register the surface with maximum possible precision or in other words with least RMS distance error.
- The output STL file will be placed in the output folder. Also a Matlab figure will be displayed to reveal the registered surface onto the fixed one.
- The RMS distance, minimum, maximum and mean distances will be available in the Matlab Workspace window after executing both the scripts.
