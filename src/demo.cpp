/* 
Purpose: This is the demo program and this is the demo.cpp file.
         In this program we are making a quadrant tree and first of all,
         it will print the welcome message and it will show the number of
         command that user can use for running this program. User can create a new 
         tree with the command N and then immedialtely they can enter I command for
         inserting the items into the tree, after that it will ask for the title and one
         line description for the tree. Then it will ask user for LL and UR address and 
         if invalid address then it will print the error message. As soon as user have inserted
         items into the tree they can print the tree and quad contents, they also can 
         find the particular tree and quad after isnerting more than ones quad and trees.
         They can also delete the entire tree and they can aslo remove the quad.
*/

#include "../hdr/demo.h"
#include <iostream>
#include <clocale>

int main()
{
   setlocale(LC_ALL,"");
   bool verbose = true;
   Command cmd;
   ItemTree *iTree = NULL;

   helpCmd();

   do {
      cmd = getCommand(verbose);
      processCommand(iTree, cmd, verbose);
   } while (cmd != Quit);

   return 0;
}

// obtain the next command from the user (silent mode if verbose is false)
Command getCommand(bool verbose)
{
   wchar_t ch; // user-entered command character
   Command cmd; // translated command (None if invalid command entered)

   //if it is verbose mode then we have to prompt this message
   if (verbose) {

      std::wcout << L"Enter a command (QHNDPFVIR: H is help)";
   }
   //if not in verbose mode then we don't have to show any message for entering the commands 
   std::wcout << L"> ";
   std::wcin >> ch;

   ch = toupper(ch);

   switch (ch) {

      case Quit:
      case Help:
      case Print:
      case New:
      case Delete:
      case Find:
      case Verbose:
      case Insert:
      case Remove:
         // valid command entered
         cmd = (Command)(ch);
         break;

      default:
         // anything else is invalid command
         cmd = None;
   }

   return cmd;
}

// let the user specify whether they wish to work on a single quadrant or an entire subtree
//if user will enter invalid command then it will show the error message to user
Modifier getModifier(bool verbose)
{
   wchar_t ch; // user-entered command character
   Modifier cmd; // translated modifier (Invalid unless valid choise made)

   //if verbose mode then we have to prompt user with this message
   if (verbose) {

      std::wcout << L"Single quadrant (Q) or entire subtree (T)?";
   }
   //if not verbose mode then simply ask user with > this sign
   std::wcout << L"> ";
   std::wcin >> ch;

   ch = toupper(ch);

   switch (ch) {

      case Tree:
      case Quad:
         // valid command entered
         cmd = (Modifier)(ch);
         break;

      default:
         // anything else is invalid
         cmd = Invalid;
   }

   return cmd;
}

// let the user specify a quaternary address
std::wstring getAddress(bool verbose)
{
   //if it is verbose mode then prompt the given message and if 
   //not a verbose mode then it will not show anything
   if (verbose) {

      std::wcout << L"Enter the quaternary address or * for global" << std::endl;
   }

   std::wstring addr = L"";
   std::wcin >> addr;

   //if user will enter * that means empty string
   if (addr == L"*") {
      addr = L"";
   }
   
   //if user will enter any invalid address then it will show the warning message
   if (!Address::isValidAddr(addr) && verbose) {
      std::wcerr << L"WARNING: " << addr << L" is not a valid address" << std::endl;
      std::wcout << L"(either just *, or 1-" << Address::MaxAddr << L" digits 0-3)" << std::endl;
   }

   //returning the address that user have entered
   return addr;
}

//this function where we are implementing the welcome message of the program
void helpCmd()
{
   std::wcout << std::endl;
   std::wcout << L"Welcome to the ItemTree demo" << std::endl;
   std::wcout << L"Enter any one of the following commands:" << std::endl;
   std::wcout << L"   " << (char)(Quit) << L" to quit" << std::endl;
   std::wcout << L"   " << (char)(Help) << L" to display this menu" << std::endl;
   std::wcout << L"   " << (char)(Verbose) << L" to toggle verbose mode (prompts/messages)" << std::endl;
   std::wcout << L"   " << (char)(New) << L" to create a new tree (deletes old)" << std::endl;
   std::wcout << L"   " << (char)(Delete) << L" to delete the current tree" << std::endl;
   std::wcout << L"   " << (char)(Print) << L" to print items from the tree" << std::endl;
   std::wcout << L"   " << (char)(Insert) << L" to insert new elements in the tree" << std::endl;
   std::wcout << L"   " << (char)(Remove) << L" to remove elements from the tree" << std::endl;
   std::wcout << L"   " << (char)(Find) << L" to find elements in the tree" << std::endl;
   std::wcout << std::endl;
}

// if user will enter invalid command then display 
//the invalid command message in the verbose mode and it will not show anything
//in the non-verbose mode
void errorCmd(bool verbose)
{
   if (verbose) {
      std::cerr << L"Error: an invalid command was entered, please try again" << std::endl;
      std::cerr << L"       (H is the command for the help menu)" << std::endl;
   }
}

// obtain any required arguments then apply the command to the tree
//in this function we are calling the various functions for that
//particular command and we are using the switch statement
void processCommand(ItemTree* &IT, Command cmd, bool &verbose)
{
   switch (cmd) {
      case Quit: quitCmd(IT, verbose);
         break;
      case Help: helpCmd();
         break;
      case Verbose: verbose = !verbose;
         break;
      case Print: printCmd(IT, verbose);
         break;
      case Insert: insertCmd(IT, verbose);
         break;
      case Remove: removeCmd(IT, verbose);
         break;
      case Find: findCmd(IT, verbose);
         break;
      case New: newCmd(IT, verbose);
         break;
      case Delete: deleteCmd(IT, verbose);
         break;
      default: errorCmd(verbose);
         break;
   }
}

//this function will terminate the program if user will enter command Q
//and if the verbose mode than it will print the given message and in the
//quite mode it will terminate the program without printing any message
void quitCmd(ItemTree* &IT, bool verbose)
{
   if (verbose) {

      std::wcout << L"Deallocating item tree and shutting down" << std::endl;
   }

   //calling the function named deletecmd
   deleteCmd(IT, verbose);

   std::wcout << std::endl;
}

// delete and nullify current tree
// and if it is verbose mode then it will prompt the message and
//which is not required as per specification of the project
void deleteCmd(ItemTree* &IT, bool verbose)
{
   if (IT) {

      delete IT;
      IT = NULL;
   } else if (verbose) {

      std::wcerr << L"Warning: no item tree present to delete" << std::endl;
   }
}

// create a new item tree (deletes and replaces old)
void newCmd(ItemTree* &IT, bool verbose)
{
   //if N command then first we will delete the tree which is exist already
   //so we are calling the function named deletecmd
   if (IT) {
      deleteCmd(IT, verbose);
   }

   //if there is no tree then it will create the new tree
   IT = new ItemTree;
   //if tree creation will be failed then it will simply return 
   //we can print message for faling of the new tree creation 
   //but it is not specified in the program specification
   if(!IT)
   {
      return;
   }
}

//this function will take the command from the user and print 
//whatever user wants, I mean it will print the tree or the quad
void printCmd(ItemTree* IT, bool verbose)
{
   //if there is no tree has been created than if it is verbose mode then it will
   //ptint the given messsage and if non verbose mode then it will not print anything 
   //but ask for the valid command
   if (!IT) {
      if (verbose) {
         std::wcerr << L"WARNING: no tree has been created to print" << std::endl;
      }
      return;
   }

   //here we are checking that what user wants to print
   //it can be entire tree or the specific quadrant
   Modifier cmd;

   //it will take the command which is entered by the user and ask user until they
   //will enter a valid commad so we are taking do while loop here
   do {

      //calling the function named getmodifier
      cmd = getModifier(verbose);

      //if the command is not valid then in the vrbose mode it will print the 
      //given message
      if (cmd == Invalid) {
         //************************fixed defect******************
         //if user enters invalid data in verbose mode it will show this error message
         //if user enters invalid data in quite mode it will not show this error message
         //but it will ask user to enter valid data again and again, until not enter
         //a valid data
         if(verbose)
	      {
            std::wcerr << L"Error: neither quadrant nor tree was selected, ";
            std::wcout << L"please try again" << std::endl;
	      }
      }
   } while (cmd == Invalid);

   // get the address of the quadrant or subtree
   std::wstring addr = getAddress(verbose);

   //if the command is tree then it will go inside the first
   //if condition and if the command is quad then it will go
   //in the else part
   if (cmd == Tree) {
      //if it is verbose mode then it will print this message
      //this message is not required according to the project specification
      if (verbose) {
         std::wcout << std::endl << L"Printing tree from \"" << addr << L"\"" << std::endl;
      }

      //printing the content of the print by calling the 
      //function named printTree
      IT->printTree(addr);
   } else if (cmd == Quad) {
      //if it is verbose mode then it will print this message
      //this message is not required according to the project specification
      if (verbose) {
         std::wcout << std::endl << L"Printing quad from \"" << addr << L"\"" << std::endl;
      }

      //printing the content of the quad by calling the function named printquad
      IT->printQuad(addr);
   }
}

// prompt the user (if verbose) and read a line of text into the target
void getString(std::wstring prompt, std::wstring &target, bool verbose)
{
   //if it is verbose mode then it will prompt the message
   if (verbose) {
      std::wcout << prompt;
   }
   std::getline(std::wcin, target);  
}

//if user will enter insert I command then it will grab
//what user have entered and then prompt for another command in sequence
void insertCmd(ItemTree* &IT, bool verbose)
{
   //there is no tree has been created then in the verbose mode
   //it will show the given warning message
   if (!IT) {
      //*************not showing messages in the verbose mode************
      //just only show this warning message in the verbose mode
      //and it will not show this message in the quite mode and if user
      //enters non valid data it will ask until they provide valid data
      if(verbose)
      {
         std::wcerr << L"WARNING: no tree exists to insert to" << std::endl;   
      }
      return;
   }

   //declaring the variables for the title, description
   //lower left address and upper right address
   std::wstring junk, title, desc, LL, UR;
   std::getline(std::wcin, junk); // discard leftover newline from buffer

   //prompting user for entering the title of the tree in the single line
   //this is message only can seeable in the verbose mode
   getString(L"Enter a single line of text for the new item title: >", title, verbose);

   //prompting user for entering the obe line description for the tree
   //and this message only prompts in the verbose mode
   getString(L"Enter a single line of text for the new item description: >", desc, verbose);
   
   //after the description of the tree it will prompt for the lower left address
   //message can only show in the verbose mode
   if (verbose) {
      std::wcout << L"Next provide the address of the item\'s lower left corner";
   }
   //calling the function named getaddress for storing the LL address
   LL = getAddress(verbose);

   //if verbose mode then it will prompt for the upper right address by printing
   //the given message
   if (verbose) {
      std::wcout << L"Next provide the address of the item\'s upper right corner";
   }
   //calling the function named getadress for storing the UR address
   UR = getAddress(verbose);

   //creating a new item by taking the value which is entered by the user
   Item *i = new Item(title, desc, LL, UR);

   //if allocation is failed then it will print the message in the 
   //verbose mode
   //else it will take a look for getting the tree or quad
   if (!i) {
      std::wcerr << L"WARNING: item creation failed" << std::endl;
   } else {
      //declaring the variables accroding to above variables
      std::wstring t_title, d_desc, l_LL, u_UR, q_quad;

      // lookup to get the quadrant
      i->lookup(t_title, d_desc, l_LL, u_UR, q_quad);

      //if the item is not properly inserted into the tree
      //then in verbose mode print the given message
      //and then delete the wrong item
      if (!IT->insert(i)) {
         if (verbose) {
            std::wcerr << L"WARNING: item insert failed" << std::endl;
         }
         delete i;

      }else {
         //******************fixed and will not show message in quite mode****************
         //if it is a verbose mode than only print this message
         //if user inserts correct items in the tree
         //if not a verbose mode it will proceed to the next command 
         //without showing this message
	      if(verbose)
         {
            std::wcout << L"Item inserted in quadrant " << q_quad << std::endl;
         } 
      }   
   } 
}

//in this function we are finding for the item in the tree by the title named
//it will print the diffrent messages in the verbose mode
//and if Item not found then print the message for that
Item* findCmd(ItemTree* IT, bool verbose)
{
   //if tree has not been created then it will print the given message and return null
   if (!IT) {
      if (verbose) {
         std::wcerr << L"WARNING: no tree has been created to search" << std::endl;
      }
      return NULL;
   }

   // see if the user wishes to print a search for the quadrant 
   //or entire tree
   Modifier cmd;

   //checking by the do while loop for the valid command
   //and it will ask user until they provide the valid command
   do {
      //calling the function named getModifier
      //this will basicallly storing the command which is 
      //entered by the user
      cmd = getModifier(verbose);

      //if the command is invalid then show the error message
      if (cmd == Invalid) {
         //*********************show message only in verbose mode***********************
         //if user will enter any other data than (qQtT) it will show the error message
         //in verbose mode whereas in quite mode, it will ask until don't get the valid data
         //it will not print any error message in non-verbose mode
         if(verbose)
         {
            std::wcerr << L"Error: neither quadrant nor tree was selected, ";
            std::wcout << L"please try again" << std::endl;
         }
      }
   } while (cmd == Invalid);

   // get the address of the quadrant or subtree
   std::wstring addr = getAddress(verbose);

   //commenting out not needed variables desc, LL, UR, quad;
   //declaring the variables junk and title 
   std::wstring junk, title; 
   std::getline(std::wcin, junk); // discard leftover newline from buffer

   //prompting for the title of the tree that user want to search and it will 
   //print the message in verbose mode
   getString(L"Enter a single line of text for the title you are searching for: >", title, verbose);

   Item* iptr = NULL;

   //if user want to search for the quad then we are calling 
   //the function named findInQuad
   //and if the user want to search for the entire tree then
   //we will call the function named finInTree
   //we are calling both the functions from the ItemTree.cpp file
   if (cmd == Quad) {
      iptr = IT->findInQuad(title, addr);
   }else {
      iptr = IT->findInTree(title, addr);
   }

   //if the given item is not founded in the tree or quad then it will
   //prompt the messsage in the verbose mode
   //if item is founded then we are calling the function named print from the 
   //Item.cpp file
   if (!iptr) {
      //***************this will print the message only when it is verbose mode***********
      //this message was displaying in the both mode
      //now it will only display in the verbose mode
      //if user enters the wrong command this will ask user to enter the command again
      //and if user will enter right title then it will show the founded item
      if(verbose)
      {
         std::wcerr << L"No matching item found for \"" << title << L"\" in ";
         std::wcerr << addr << std::endl;
      }
   } else {
      std::wcout << L"Found: ";
      //calling the function named print from the Item.cpp file
      iptr->print();
   }

   //and we will return the found item
   return iptr;
}

//this function is for remove command
//this will remove the tree or specific quadrant
//first we will check for the valid or invalid commad
//and then if it will valid command then we will find that tree or quad
//and then we will remove tree or quad
void removeCmd(ItemTree* &IT, bool verbose)
{
   //if tree has not been created then it will print the warning message
   //in the verbose mode
   if (!IT) {
      if (verbose) {
         std::wcerr << L"WARNING: no tree has been created yet" << std::endl;
      }
      return;
   }

   //searching that what user wants to delete tree or quadrant
   //if user will enter invalid command then ask user until they will enter
   //the valid command and we are using the do while loop
   Modifier cmd;
   do {

      //caling the function named getModifier,
      //basically storing the command which is entred by the user
      //invalid command will prompt the message in the verbose mode
      cmd = getModifier(verbose);

      if (cmd == Invalid) {
         //***************show message only in verbose mode**********************
         //if user will enter invalid command rather than qQtT then it will print the 
         //given error message in verbose mode
         //but in quite mode it will not show any message but it will ask until don't get the valid entry
         if(verbose)
         {
            std::wcerr << L"Error: neither quadrant nor tree was selected, ";
            std::wcout << L"please try again" << std::endl;
         }
      }
   } while (cmd == Invalid);

   //extra declared variables are commented out 
   //desc, LL, UR, quad;
   std::wstring junk, title; 
   std::getline(std::wcin, junk); // discard leftover newline from buffer

   //prompting user for entering the title of the tree that they want to remove and 
   //it will prompt in the verbose mode
   getString(L"Enter a single line of text for the title of the item you wish removed >", title, verbose);

   // get the address of the quadrant or subtree
   std::wstring addr = getAddress(verbose);

   //item class pointer intializing with null
   Item* iptr = NULL;

   //if it is quad then we call the function named findinquad from
   //the itemTree.cpp file
   //and if it is tree then we call the functiokn named findintree
   //from the itemtree.cpp file
   if (cmd == Quad) 
   {
      iptr = IT->findInQuad(title, addr);
   }else {
      iptr = IT->findInTree(title, addr);
   }

   //if tree is not foud of the given title then it will prompt message
   //which shows that item doesmot matched
   //else if we found the given title named tree then we will call the
   //remove function from the itemtree.cpp and it will remove that tree
   //and if the tree will not remove suceesfully then it will show the message for that
   if (!iptr) {
      //***************show message only in verbose mode**********************
      //whenever user is in the quite mode it will not tell the user when
      //they enters non valid input, but it will prompt again for 
      //the valid input
      if(verbose)
      {
         std::wcerr << L"No matching item found for \"" << title << L"\" in ";
         std::wcerr << addr << std::endl;
      }
   } else {
       // ************ TRY THE REMOVE HERE ****************
       if (IT->remove(iptr)) {
           std::wcout << L"Removed item \"" << title << L"\"" << std::endl;
       } else {
           std::wcout << L"Remove unsuccessful for item \"" << title << L"\"" << std::endl;
       }
   }
}

