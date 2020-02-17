#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

vector<string> splitString(string text, char d);
string getFullPath(std::string cmd, const vector<string>& os_path_list);
bool fileExists(std::string full_path, bool *executable);


/*
 * Copyright (C) Tyler Beverley, Nick Pumper. 
 * No use without explicit permision.  
 *
 * This program is a Homework assignment where we created created a terminal shell 
 * for the user to run linux programs in. It is not intented to be used. 
 *
 */

 

int main (int argc, char **argv){

    string input;
    char* os_path = getenv("PATH");
    vector<string> os_path_list = splitString(os_path, ':');

    std::cout << "Welcome to OSShell! Please enter your commands ('exit' to quit)." << std::endl;

    // Repeat:
    //  Print prompt for user input: "osshell> " (no newline)
    //  Get user input for next command
    //  If command is `exit` exit loop / quit program
    //  If command is `history` print previous N commands
    //  For all other commands, check if an executable by that name is in one of the PATH directories
    //   If yes, execute it
    //   If no, print error statement: "<command_name>: Error running command" (do include newline)

    return 0;
}

// Returns vector of strings created by splitting `text` on every occurance of `d`
vector<string> splitString(string text, char d){

    vector<string> result;

    return result;
}

// Returns a string for the full path of a command if it is found in PATH, otherwise simply return ""
string getFullPath(string cmd, const vector<string>& os_path_list){

    return "";
}

// Returns whether a file exists or not; should also set *executable to true/false 
// depending on if the user has permission to execute the file
bool fileExists(string full_path, bool *executable){

    *executable = false;
    return false;
}



/*
 * Checks if the char array contains only numerical ASCII. 
 * ASCII Range 48 -> 57 (No decimals)
 *
 * Returns 0 if error
 * Returns 1 otherwise. 
 */
int checkIfNumerical( char* c ){

    //loop through the capactiy, stop if end of string
    //or if there are too many decimals
    while( *c != '\0' ){

        if( (*c < 48 || *c > 57) ){
            return 0;
        }
        if (*c + 1  == '\0'){
            break; 
        }
        else {
            c = c + 1;
        }
    }
    return 1;
}




