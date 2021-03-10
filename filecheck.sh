#! /bin/bash

#we are taking the for loop and our variable name is fname
#this will go through each arguments and check that it is readable file or not and
#if it is directory then will show the error message
#if it is the readable file then it will display the message like who comitted that file,
#the first five lines of that file and number of lines and number of characters in that file
#if the file is not readable it will show the error message
#if the filename is invalid then it also will display the error message

#for each file it will go and check
for fname in "${@}" ; do
   #checking for if it is a directory
   if [[ -d "${fname}" ]] ; then
      echo " "
      echo "ERROR: "${fname}" is a Directory not a file."
   #if it is a file
   elif [[ -f "${fname}" ]] ; then
      #if it readable file
      if [[ -r "${fname}" ]] ; then
         echo " "
         #printing the message if it is redable file
	 echo ""${fname}" is Readable File."
	 echo " "
    
	 #showing the who committed the given file using git blame command
	 #but we are piping out the output to the less
	 echo "Now we are showing the who committed each line of the "${fname}" file : "
	 git blame "${fname}" | less
	 echo " "

         #showing the first five lines of the given named file
	 echo "First Five Lines of "${fname}" file : "
	 head -n 5 "${fname}"
         echo " "
      
         #running the wc command on the file and capturing the output of number of lines
	 #in the variable
	 lines_count=$(wc -l < "${fname}")
	 echo "The number of lines are : "${lines_count}" in the given file."
      
	 #running the wc command on the file and capturing the output of number of characters
	 #in the variable
	 character_count=$(wc -m "${fname}")
	 echo "The number of characters are : "${character_count}" in the given file."
	 echo " "
      #if the file is not readable then it will show the message
      else
         echo " "
	 echo ""${fname}" is NOT a Readable file."
	 echo " "
	 #end of the if statement
      fi
   #if not provided the valid file name then it will show this message
   else
      echo " "
      echo "ERROR: "${fname}" is NOT a valid filename."
      echo "Please enter a valid file name:)"
      echo " "
   #end of the if statement
   fi
#end of the loop
done
