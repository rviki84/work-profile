Python-based web services for the VPH-Share and VPH-Dare platforms.


** VPH-DARE@IT **

- BiomarkerCsvAppender: First version of the script to append biomarkers in CSV files. The actual web service is a GIMIAS CLP in another repository.
- aceplus: First version of web service to execute ESI-ACE+ scripts.
- circadianBloodFlowService: First version of web service for model-based prediction of cerebral blood flow from patient-specific 24h measurements of blood pressure and heart rate.
- QSMReconstructionService: First version of web service for performing quantitative susceptibility mapping (QSM) based on DICOM data of a multi-echo, gradient-echo scan (magnitude and phase) and a T1-weighted, magnetization-prepared scan.
- PhilipsBrainSegService: First version of web service for the Philips Sub-cortical brain segmentation process.

** VPH-SHARE **

- AnsysInvoker: Web service for invoking an ANSYS ICEM script. Developed for running with SOAPLib under MS Windows OS.
- CclFileGenerator: First version of a web service to generate multiple CCL files, under various condition, specified in the input CSV files. The output CCL files are input to another tool that generates a mesh of a vessel section. Used for the Aneurist use case in the last year of the VPH-Share project.
- TavernaReaders: Python classes for dealing with Taverna workflows (.t2flow) and baclava files (.xml) and convert them to other formats