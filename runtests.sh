#! /bin/bash

#in this script use recursive function named explore
#which has a recursive traversal of the directory and all the its subdirectories
#and then it will run the executable on each redable file found

#implementing the explore function
function explore()
{
   #we are psssing the input that we got from the user
   local dirname=$1
   local exename=$2

   #this loop will go through each file in the given directory
   for fname in "${dirname}"/* ; do
      #if it is a file and redable then ir will execute that founded file
      if [ -f "${fname}" ] && [ -r "${fname}" ]; then
	 ./"${exename}"  "${fname}"
      fi
      #if it is directory then it will open that directory recursively
      #we are calling the explore function 
      if [[ -d "${fname}" ]]; then 
	 explore "${fname}" "${exename}"
      fi
		
   done #end of the for loop
   return 0 #we are returning zero
}


#asking the user for the input directory name and executable name
#dirname is for the directory name and exename is for the executable name
#if user will enter valid data, then we are calling the explore function
#if one of them is not directory or one of them is not executable
#or both are not valid input then it will go into the else condition
#and it will show the error message
read -p "Please Enter a Directory name and Executable name: " dirname exename
echo "You have entered "${dirname}" as directory name and "${exename}" as Excecutable name."

if [ -d "${dirname}" ] && [ -x "${exename}" ]; then
   explore "${dirname}" "${exename}"
   echo "Thank you!"
else 
   if [ ! -d "${dirname}" ]; then
      echo "ERROR: "${dirname}" Not a valid directory"
   fi
   if [ ! -x "${exename}" ]; then
      echo "ERROR: "${exename}" is not a valid execuatable name"
   fi

fi
