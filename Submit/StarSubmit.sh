#!/bin/bash
# Since the file-organizing script relies on you having used this, you should always submit jobs with this instead of star-submit
# If you didn't use this script, simply create a directory in ${SubmissionLogsDir} named CommitHash_JobID
# If you don't care about the commit hash, simple name it something like NoCommitHash_JobID
if [ -z "$1" ]
  then
    echo "You need an argument of the form (CommitHash)"
    exit 1
fi
if [ "$2" ]
  then
    echo "You need an argument of the form (CommitHash)"
    exit 1
fi

read -p "Did you run 'cons'? Press Enter if so; otherwise ctrl+C to exit"

User="adams92"
CommitHash=$1

# Make the directories where output files will go
DstAnalysisPwgDir=/star/data03/pwg/${User}/EpdDep
SubmissionLogsDir=${DstAnalysisPwgDir}/SubmissionLogs
AnalysisOutputDir=${DstAnalysisPwgDir}/RootFiles
mkdir -p ${DstAnalysisPwgDir}
mkdir -p ${SubmissionLogsDir}
mkdir -p ${AnalysisOutputDir}

echo "We'll tidy up existing files before submitting this job; running OrganizeOutputFiles.sh..."
source ./OrganizeOutputFiles.sh

echo "No we'll submit this job."
sleep 1
StarSubmitLogTemp=${SubmissionLogsDir}/StarSubmitLog_Temp.txt
rm -rf ${StarSubmitLogTemp}
star-submit /star/u/${User}/EpdDep/Submit/Analysis.xml | tee ${StarSubmitLogTemp}

# Check if the scheduling was successful and make subdirectories which link the CommitHash to the JobID and record the scheduling information
StarSubmitLastLine=$(tail -n 1 ${StarSubmitLogTemp})
if [ "$StarSubmitLastLine" = "Scheduling successful" ]; then
  JobID=$(ls -t1d *.session.xml | head -n 1 | awk -F '[.]' '{print $1}' | sed 's/^[^d]*d//g')
  FileNameID=${CommitHash}_${JobID}
  AnalysisOutputIDDir=${AnalysisOutputDir}/${FileNameID}
  SubmissionLogsIDDir=${SubmissionLogsDir}/${FileNameID}
  mkdir -p ${AnalysisOutputIDDir}
  mkdir -p ${SubmissionLogsIDDir}
  mv ${StarSubmitLogTemp} ${SubmissionLogsIDDir}/StarSubmitLog_${FileNameID}.txt
fi