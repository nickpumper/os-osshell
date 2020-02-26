#include "osshell.h"
#define HISTORY_PATH "./history.txt"
#define HISTORY_LIMIT 128 
#define COMMAND_LIMIT 100 

using namespace std;

int test();
bool exitFlag;


/*
 * Copyright (C) Tyler Beverley, Nick Pumper. 
 * No use without explicit permission.  
 *
 * This program is a Homework assignment where we created created a terminal shell 
 * for the user to run linux programs in. It is not intented to be used. 
 *
 */

 

int main (int argc, char** argv){

    // main vars
    std::cout << "Welcome to OSShell! Please enter your commands ('exit' to quit)." << std::endl;

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
    while (!exitFlag) {
        vector<string> user_input; 
        vector<string> env_path; 
        string full_path; 
        
        user_input = splitString( getUserInput(), 32 );   
        char* argv[ user_input.size() ];
        convToCharArray(user_input, argv );

        env_path = splitString( getenv("PATH"), ':' );
        full_path = getFullPath( argv[0], env_path );  

        execute( user_input.size(), argv, (char*) full_path.c_str() ); 
    } // while !exit
    return 0;
} // main



/*
 * Takes an input command with arguments then 
 * forks off a new process to run that command.
 *
 *
 * Custom Commands: 
 *  history [x] -> show the last x commands typed. 128 default. 
 *  quit -> exit out of the shell. 
 *
 */
void execute( int argc, char** argv, char* full_path){

    char* cmd = argv[0]; 

    if( strcmp( cmd, "history" ) == 0 ){
        
        if( argc > 2 ){
            //erro
            int quantity = atoi( argv[1] );
            printHistory( quantity );
            return;  
        }
        printHistory( 128 );  
        return;
    } 
    else if( strcmp( cmd, "exit" ) == 0 ) {
        exitFlag = true; 
    }
    else if( *cmd == 4 ){ //ctrl-d
        exitFlag = true; 
    }
    else {
        int cid = fork(); 
        if( cid == 0 ){

            printf("%s\n", full_path); 
            int err = execv(full_path, argv);
            if( err == -1 ){
                printError( string( argv[0] ));   
            }
            exit(0); 
        }
        else{
            int result; 
            wait( &result );
        }
    }
}

string getUserInput() {
    string input;

    cout << "osshell> "; 
    cin.clear(); 
    getline(cin, input); 

    addToHistory( input ); 

    return input;
} //getUserInput


// looks back on prev user input and prints out what was up (up to 128 commands)
void printHistory( int quantity ){
   
    FILE* f = fopen( HISTORY_PATH, "r"); 
    char line[1024];  //not sure what the command limit is. 

    if( quantity < 0 ){
        printf("Error: history expects an integer > 0 (or 'clear')\n");
    }

    if( quantity > 128 ){
        quantity = 128; 
    }
    
    int i = 0; 
    while( fgets(line, sizeof(line), f ) && i <= quantity) {
            printf("  %d: %s\n", i+1, line ); 
            i++;
    } // for

    fclose(f); 

} // historyPrintAll


void addToHistory(string input) {

    //dont add empty string 
    if( input.empty() ){
        return; 
    }

    FILE* f = fopen( HISTORY_PATH, "a"); 
    fputs( input.c_str(), f );

    fclose(f); 
} // addToHistory


void clearHistory() {
    char cmd[128];
    sprintf(cmd, "rm %s", HISTORY_PATH );
    system(cmd); 
}

// Returns vector of strings created by splitting `text` on every occurance of `d`
vector<string> splitString( string text, const char d ){

    vector<string> result;
    char* tok = strtok( (char*)text.c_str(), &d);

    while( tok != 0 ) {
        result.push_back(tok);
        tok = strtok(0, &d);
    } // while

    return result;
} // splitString


void convToCharArray( vector<string> vec, char** res  ){

    for( int i = 0; i < vec.size(); i++){
        res[i] = (char*)vec[i].c_str(); 
        //printf("arg%d: %s\n", i, res[i]); 
    }
}


// Returns a string for the full path of a command if it is found in PATH, otherwise simply return ""
string getFullPath(string cmd, const vector<string>& os_path_list) {

    string result = "";

    for (string s: os_path_list) {
        result.append(s);
    } // for

    result.append("/");
    result.append(cmd);

        std::cout << "getFullPath returns:" << result << "\n";
    return result;

} // getFullPath

// Returns whether a file exists or not; should also set *executable to true/false 
// depending on if the user has permission to execute the file
bool fileExists(std::string full_path, bool* executable){

    struct stat stat_buff; 
    int err; 

    err = stat( full_path.c_str(), &stat_buff);

    //check if file exists. 
    if( err > 0 ){

        if( stat_buff.st_mode && S_IXUSR != 0 ) {
            *executable = true; 
        } else { 
            *executable = false; 
        }
        return true;
    } 

    *executable = false;
    return false;
} // fileExists





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

// prints an error statement
void printError (string badCommand) {
    cout << badCommand << ": Error running command\n";
} // printError



/*
 * djb2 hash func by Dan Bernstein. 
 * could be used instead of strcomp to figure out command. 
 *
 */
unsigned long hash(unsigned char *str) {

    unsigned long hash = 5381;
    int c = *str;

    while( c != '\0' )
        hash = ((hash << 5) + hash) + c; 
        c = *str++;

    return hash;
}


