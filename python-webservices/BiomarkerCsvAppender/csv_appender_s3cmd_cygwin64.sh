#!/bin/bash
#
# CSV appender to run locally in a machine installed with 's3cmd'
# Appends a list of sub-directory CSVs into a single CSV in the parent folder (SOURCE_DIR)

SOURCE_DIR=${1:-${1%/}}
SEARCH_FILE_NAME=${2:-biomarkers.csv}
OUTPUT_FILENAME=${3:-biomarkers.csv}
LOCAL_TMP_DIR=${4:-$(mktemp -d)}

# if source starts with s3 download with s3cmd
if [[ $SOURCE_DIR == s3://* ]]; then
  echo "Downloading ${SEARCH_FILE_NAME} files from ${SOURCE_DIR} to ${LOCAL_TMP_DIR}"
  s3cmd get --include ${SEARCH_FILE_NAME} --exclude '*' --skip-existing --recursive ${SOURCE_DIR} ${LOCAL_TMP_DIR}/
else
  LOCAL_TMP_DIR=${SOURCE_DIR}
fi

FILES=($(ls ${LOCAL_TMP_DIR}/*/${SEARCH_FILE_NAME}))
first_file="${FILES[0]}"                              # extract first file

# create CSV header
HEADER="SubjectId, $(head -n 1 ${first_file}), Source path"
echo ${HEADER} > ${LOCAL_TMP_DIR}/${OUTPUT_FILENAME}

# loop over files appending CSV rows with SUBJECT_ID and SOURCE_PATH
for f in ${FILES[@]}; do
  SUBJECT_ID=$(basename $(dirname ${f}))
  SOURCE_PATH="${SOURCE_DIR}${SUBJECT_ID}"
  LINE="${SUBJECT_ID}, $(tail -n +2 $f), ${SOURCE_PATH}"
  echo ${LINE} >> ${LOCAL_TMP_DIR}/${OUTPUT_FILENAME}
  # tail -n +2 $f >> ${LOCAL_TMP_DIR}/${OUTPUT_FILENAME}
done
echo "Output: ${LOCAL_TMP_DIR}/${OUTPUT_FILENAME}"

# if source starts with s3 upload with s3cmd
if [[ $SOURCE_DIR == s3://* ]]; then
  echo "Uploading ouput ${OUTPUT_FILENAME} to ${SOURCE_DIR}"
  s3cmd sync ${LOCAL_TMP_DIR}/${OUTPUT_FILENAME} ${SOURCE_DIR}
fi
