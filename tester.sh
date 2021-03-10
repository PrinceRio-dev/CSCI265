# Princekumar Diyora
# 27 November 2020
# sample script for automated testing of an exectuble
#
#  - assumes the program under test takes data from standard input
#    and writes data to standard output, exits with status 0 if
#    processing completes, and with a non-zero status if it needs
#    to abort or crashes
#
#  - assumming that there is one directory named "testsuite" and it 
#    has three sub directories. for the input data directory name is "inputdata"
#    for the expected result directory name is "expoutput" and for the 
#    actual result directory name is "actoutput".
#  
#
#  - the input file must contain the exact desired input for the run
#    of that test case, and the expected-output file must contain the exact
#    expected output for that test case
#
#  - if the program does not crash during the test case and the actual output
#    is an exact match for the expected output then "passed" is displayed for
#    the test case (along with the filename and directoryname), otherwise it fails
#
#  - the names of the three directories and of the program under test are
#    currently stored in four global variables, and the tester
#    will evaluate every test case
#
#  - global variables are also used to set limits on the amount of cpu time
#    the executable is permitted to use, and the bytes of output it is permitted
#    to store for each test case

#! /bin/bash

# ----- global configuration variables -----
# default name for the demo program and which is under test
demo="./bin/demo"

# default name for the directory containing input-data files for test cases
#it will go to testsuite directory and then go to inputdata sub directory
indir="testsuite/inputdata"

# default name for the directory containing expected-output files for test cases
#it will go to testsuite directory and then go to expoutput directory
expdir="testsuite/expoutput"

# default name for the directory to store actual output files for test cases
#it will go to the testsuite directory and then actoutput directory
actdir="testsuite/actoutput"

# default limits on cpu seconds and chars of output to store
# I have set the character limit to the 20000
cpu=10
climit=20000

# ----- test case processing -----

# run on each test file in the test directory
#it will go to testdata directory and chekc for each and everyfile
for tfile in "${indir}"/* ; do

   # get the name of the expected output file and store it in fname variable
   fname=$(basename "${tfile}")

   # determine the names of the expected output and actual results files
   efile="${expdir}/${fname}"
   ofile="${actdir}/${fname}"

   # create a temp file to store the differences between actual and expected output
   dfile=$(mktemp)

   #variable for storing the exit status
   status=0

   # run one test to check the exit status, don't store the output
   $(ulimit -t "${cpu}"; "${demo}" < "${tfile}" &> /dev/null)
   status=$?

   # check the return value, case failed if it crashed
   if [ "$status" -ne 0 ] ; then
      echo ""${tfile}" FAILED: non-zero exit status"
   else
       
      # run the actual test, limiting the cpu time and output stored
      (ulimit -t "${cpu}"; ("${demo}" < "${tfile}" 2>&1 | head -c "${climit}" > "${ofile}"))

      # use diff to check output correctness (exact match or not) and display passed or failed
      (((diff "${ofile}" "${efile}" &> "${dfile}") && (echo ""${tfile}" PASSED")) || (echo ""${tfile}" FAILED"))

      #capturing the difference in the variable named diffile	
      diffile=$(diff "${ofile}" "${efile}" &> "${dfile}")

      #if it is not equal to zero then test case passes increase by one
      #if it is zero then fails increase by one everytime
      if [ "${diffile}" == "" ] ; then
         ((pass=pass+1))
      else
         ((fail=fail+1))
      fi

   fi      
done
