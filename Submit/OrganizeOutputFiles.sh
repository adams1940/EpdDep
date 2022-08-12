#!/bin/bash
# Assuming that you submitted with StarSubmit.sh, this script will organize all of the Job's associated output files 

User="adams92"

echo

# The directories where generated files should appear
DstAnalysisPwgDir=/star/data03/pwg/${User}/EpdDep
SubmissionLogsDir=${DstAnalysisPwgDir}/SubmissionLogs
AnalysisOutputDir=${DstAnalysisPwgDir}/RootFiles
mkdir -p ${DstAnalysisPwgDir}
mkdir -p ${SubmissionLogsDir}
mkdir -p ${AnalysisOutputDir}

# In case package files ended up in the submission directory, move them back here for a moment since they'll mess things up later on.
find ${SubmissionLogsDir} -maxdepth 1 -type d -name "*package*" -prune -exec mv {} . \;

# Make a list of all Job IDs that need their files to be sorted
JobIDList=$(find . ${SubmissionLogsDir} ${AnalysisOutputDir} -maxdepth 1 \( -name "*.list" -o -name "*.err" -o -name "*.log" -o -name "*.package*" -o -name "*.root" \) | awk '{print $NF}' FS=/ | sed 's/^[^d]*d//g' | awk -F '[._]' '{print $1}' | sort -u)
JobIDs=(${JobIDList//$'\n'/ })
RunningJobs=$(condor_q --global ${User} | grep '.csh' | grep -v ' X ' | awk '{print $NF}' FS=/ | sed 's/^[^d]*d//g' | awk -F '[_]' '{print $1}' | sort -u)
for JobID in ${JobIDs[@]}; do
  if [[ ${RunningJobs} == *"${JobID}"* ]]; 
  then 
    echo "Skipping Job \"${JobID}\" because it's still running."; echo
    continue
  fi
  if [[ "${JobID}" == "" ]];
  then 
    echo "There are files in the output directories that have an empty Job ID. Check the following directories:"
    echo "    ${SubmissionLogsDir}"
    echo "    ${AnalysisOutputDir}"; echo
    continue
  fi

  echo "Examining files associated with Job ID \"${JobID}\"..."; echo
  JobCompletionLogFile=${SubmissionLogsDir}/JobCompletionLog_Temp.txt
  rm -f ${JobCompletionLogFile}; touch ${JobCompletionLogFile}

  # Print out the number of events started for this Job
  NumEventsStarted=$(grep 'events started' ${SubmissionLogsDir}/*${JobID}*.log | awk -F '[ :]' '{print $2}' | paste -sd+ - | bc)
  if [[ ${NumEventsStarted} == "" ]];
  then
    EventsStartedPhrase="Can't read events started from the log files. Print this out in your analysis maker if you want this functionality in the future."
    echo ${EventsStartedPhrase}; echo
  else
    EventsStartedPhrase="There were ${NumEventsStarted} = $((${NumEventsStarted}/1000))K = $((${NumEventsStarted}/1000000))M events started."
    read -p "${EventsStartedPhrase} Press Enter to continue."; echo
  fi
  echo ${EventsStartedPhrase} >> ${JobCompletionLogFile}

  # Check for file access issues (which generally show up as a 3011 error); it's normal to have between 0 and 1% of files inaccessible
  Num3011Errors=$(grep '\[3011\]' ${SubmissionLogsDir}/*${JobID}*.log | wc -l)
  StarSubmitLog=$(find ${SubmissionLogsDir} -maxdepth 2 -name "StarSubmitLog*${JobID}*.txt")
  NumFiles=$(grep 'Dataset size' ${StarSubmitLog} | tail -1 | awk -F "is " '{print $2}' | awk -F " files" '{print $1}')
  Fraction3011_Int=$((${Num3011Errors}00/$NumFiles))
  Fraction3011_Dec=$(tail -c 3 <<< $((${Num3011Errors}0000/$NumFiles)))
  ErrorPhrase="There were ${Num3011Errors} 3011 errors; this represents ${Fraction3011_Int}.${Fraction3011_Dec}% of the files."
  read -p "${ErrorPhrase} Press Enter to continue."; echo
  echo ${ErrorPhrase} >> ${JobCompletionLogFile}

  # Check for jobs that mysteriously disappeared. This happens sometimes for reasons unknown to me.
  NumJobsDispached=$(($(grep 'Writting files for process' ${StarSubmitLog} | head -1 | awk -F "_" '{print $2}' | awk -F "C" '{print $1}')+1))
  NumLogFiles=$(ls ${SubmissionLogsDir}/*${JobID}*log | wc -l)
  FractionLog_Int=$((${NumLogFiles}00/${NumJobsDispached}))
  FractionLog_Dec=$(tail -c 3 <<< $((${NumLogFiles}00/${NumJobsDispached})))
  JobsDispachedPhrase="There were ${NumLogFiles} log files out of ${NumJobsDispached} jobs dispached; this represents ${FractionLog_Int}.${FractionLog_Dec}% completion."
  read -p "${JobsDispachedPhrase} Press Enter to continue."; echo
  echo ${JobsDispachedPhrase} >> ${JobCompletionLogFile}

  # Find the folders named by the Job ID and commit hash (created by StarSubmit.sh) and make those directories if they don't exist
  echo "Cleaning up files associated with Job ID \"${JobID}\"..."; echo
  SubmissionLogsIDDir=$(find ${SubmissionLogsDir} -maxdepth 1 -type d -name "*${JobID}*")
  AnalysisOutputIDDir=$(find ${AnalysisOutputDir} -maxdepth 1 -type d -name "*${JobID}*")
  if [[ "${SubmissionLogsIDDir}" == "" ]] || [[ "${AnalysisOutputIDDir}" == "" ]];
  then
    echo "You don't have custom output directories ready for Job ID \"${JobID}\"; perhaps you just used star-submit instead of ./StarSubmit.sh. Putting these files in the NoCommitHash directories"; echo
    SubmissionLogsIDDir="${SubmissionLogsDir}/NoCommitHash"
    AnalysisOutputIDDir="${AnalysisOutputDir}/NoCommitHash"
  fi
  mkdir -p ${SubmissionLogsIDDir}
  mkdir -p ${AnalysisOutputIDDir}

  # Move the output files to their appropriate folders
  find ${SubmissionLogsDir} -maxdepth 1 -type f -name "*${JobID}*" -prune -exec mv {} ${SubmissionLogsIDDir} \;
  find ${AnalysisOutputDir} -maxdepth 1 -type f -name "*${JobID}*" -prune -exec mv {} ${AnalysisOutputIDDir} \;
  find . -maxdepth 1 -name "*${JobID}*" -prune -exec mv {} ${SubmissionLogsIDDir} \;
  mv ${JobCompletionLogFile} ${SubmissionLogsIDDir}/JobCompletionLog_${JobID}.txt
done