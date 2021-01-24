BiomarkerCsvAppender.py: Python-based web services to append biomarkers in CSV files. Currently deployed in the VPH-Share/DARE platform under the application name "GIMIAS DARE Services v0.3.1".

csv_appender_s3cmd_linux.sh: This script is meant for running locally in a Linux VM/PC installed with 's3cmd'. Its quite faster compared to above webservice. It actually creates the new consolidated CSV file everytime from the subject list available in the source directory passed as a parameter.

csv_appender_s3cmd_cygwin64.sh: This script is meant for running locally in a Windows PC installed with Cygwin and 's3cmd'. The functionality is same as above.

csv_appender_rclone.sh: This script is same as the above 2 scripts, except it uses 'Rclone' program instead of 's3cmd'. Currently tested on Cygwin.
