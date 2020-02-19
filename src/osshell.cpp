#include "osshell.h"

using namespace std;

int test(); 
const int HISTORY_LIMIT = 128;

/*
 * Copyright (C) Tyler Beverley, Nick Pumper. 
 * No use without explicit permission.  
 *
 * This program is a Homework assignment where we created created a terminal shell 
 * for the user to run linux programs in. It is not intented to be used. 
 *
 */

 

int main (int argc, char **argv){

    // main vars
    string input;
    char* os_path = getenv("PATH");
    vector<string> os_path_list = splitString(os_path, ':');
    bool exit = false;

    // command vars
    string exitCommand = "exit";
    string historyCommand = "history";

    // history command vars
    string * history = new string[HISTORY_LIMIT]; // to get this to persist across runs of OSShell, should probably move to a text file

    //std::cout << "Welcome to OSShell! Please enter your commands ('exit' to quit)." << std::endl;

    // Repeat:
    //  Print prompt for user input: "osshell> " (no newline)
    //  Get user input for next command
    //  If command is `exit` exit loop / quit program
    //  If command is `history` print previous N commands
    //  For all other commands, check if an executable by that name is in one of the PATH directories
    //   If yes, execute it
    //   If no, print error statement: "<command_name>: Error running command" (do include newline)
    //

    // main loop - exits on "exit" command
    while (!exit) {
        input = getUserInput();

        // --- check for commands ---
        
        // TODO: this is all super crude, implement checks later
        // gonna need to replace the find method calls with something more substantial
        // probably implement a method with a switch that sees if its a valid command?

        //check for exit command
        if (input.find(exitCommand) != std::string::npos) {
            std::exit(1);
        } // if

        // check for history command
        if (input.find(historyCommand) != std::string::npos) {
            historyPrintAll(history);
        } // if
        

        // if it was a valid command, add it to the history of commands
        addToHistory(input, history);
    } // while !exit

    //test();
    return 0;
} // main

// Prompts the user for input, then returns it as a string so we can do things with it.
string getUserInput() {
    string input;

    cout << "osshell> "; // Mirrinan wants this prompt
    cin.clear(); // for safety
    getline(cin, input); // ive found getline to be the safest

    return input;
} //getUserInput

// looks back on prev user input and prints out what was up (up to 128 commands)
void historyPrintAll (string * history) {
    for (int i = 0; i < HISTORY_LIMIT; i++) {
        if (!history[i].empty()) {
            cout << i << ": " << history[i] << "\n";
        }
    } // for
} // historyPrintAll

// adds the given string to the string array that should be the history array
void addToHistory (string input, string * history) {
    bool foundEmpty = false;
    int i = 0;

    // TO DO: error catch if it goes over 128 commands

    while (!foundEmpty) {
        if (history[i].empty()) {
            foundEmpty = true;
            history[i] = input;
        } // if

        i++;
    } // while
} // addToHistory

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

    struct stat stat_buff; 
    int err; 

    err = stat( full_path.c_str(), &stat_buff);

    //check if file exists. 
    if( err > 0 ){

        if( stat_buff.st_mode & S_IXUSR != 0 ) {
            *executable = true; 
        } else { 
            *executable = false; 
        }
        return true;
    } 

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




