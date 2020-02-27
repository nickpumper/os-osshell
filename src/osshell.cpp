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
        string input = getUserInput(); 
        vector<string> argv = splitString( input, ' ');   

        execute(argv ); 
        addToHistory( input ); 

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
void execute(vector<string>& argv){

    vector<string> env_path; 
    string full_path; 
    string cmd; 

    if( argv.empty() ){
        return; 
    }

    size_t argc = argv.size();  
    char** args; 


    cmd = argv[0];
    env_path = splitString( getenv("PATH"), ':' );


    if( cmd.compare("history") == 0 ){
        
        if (argc == 1) {
            printHistory( 128 );  
        }
        else if( argc == 2 ){
            if( checkIfNumerical( argv[1].c_str() ) == 1 ){

                int quantity = atoi( argv[1].c_str() );
                printHistory( quantity );

            } else if( argv[1].compare( "clear" ) == 0)  {
                clearHistory(); 
            } else {
                //printError
                printHistory(-1);
            }
        } else {
            printHistory( 128 );  
        }
    } 
    else if(  cmd.compare( "exit" ) == 0 ) {
        exitFlag = true; 
    }
    else if( cmd.compare( "quit" ) == 0 ){
        exitFlag = true; 
    }
    //other programs on the machine. 
    else {

        full_path = getFullPath( cmd, env_path );  

        if( full_path == "" ){
            //if there are no results do nothing. 
            printError( cmd );   
            return; 
        }

        const char** args = new const char* [argv.size()+1];

        for (int i = 0;  i < argv.size();  i++) {     // copy args
            args[i] = argv[i].c_str();
        }
        args[argv.size()] = NULL;  


        int cid = fork(); 
        if( cid == 0 ){

            int err = execv(full_path.c_str(), (char**) args);

            if( err == -1 ){
                printError( cmd );   
                //cout << errno << endl; 
            }
            exit(0); 
        }
        else {
            int result; 
            wait( &result );

            if( result != 0 ){
                //cout << "Did not exit safely!" << endl;
                //exit(1);
            }
        }
    }
}

string getUserInput() {
    string input;

    cout << "osshell> "; 
    cin.clear(); 
    getline(cin, input); 


    return input;
} //getUserInput


// looks back on prev user input and prints out what was up (up to 128 commands)
void printHistory( int quantity ){
   
    FILE* f = fopen( HISTORY_PATH, "r"); 
    char line[2048]; 
    unsigned long long pos; 
    int line_count = 0;  


    if( quantity < 0 ){
        printf("Error: history expects an integer > 0 (or 'clear')\n");
    }

    if( quantity > 128 ){
        quantity = 128; 
    }

    //count lines 
    while( fgets(line, sizeof(line), f ) != NULL ){
        line_count++; 
    }
    rewind( f ); 

    // print the history
    int i = 0; 
    while( fgets(line, sizeof(line), f ) && i <= line_count) {

        if(i >= line_count - quantity){
            printf("  %d: %s", i+1, line ); 
        }
        i++;
    }
    fclose(f); 

} // historyPrintAll


void addToHistory(string input) {

    //dont add empty string 
    if( input.empty() ){
        return; 
    }
    if(input.compare( "history clear" ) == 0){
        return;
    }

    FILE* f = fopen( HISTORY_PATH, "a"); 
    fputs( input.c_str(), f );
    fputs( "\n", f); 

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
    if( text.empty() ) {
        return result; 
    }

    char* tok = strtok( (char*)text.c_str(), &d);

    while( tok != 0 ) {
        result.push_back(tok);
        tok = strtok(0, &d);
    } // while

    return result;
} // splitString


// Returns a string for the full path of a command if it is found in PATH, otherwise simply return ""
string getFullPath(string cmd, const vector<string>& os_path_list) {
    
    string result = "";
    bool executable; 
    bool exists; 
    char buff[2048];  

    //localfile
    if( cmd.find( "/" ) != -1){

        string path = string( getcwd( buff, sizeof(buff) ) ); 
        path = path + "/"  + cmd;

        exists = fileExists( path, &executable);
        if( executable && exists ){
            return path; 
        }
    } 
    else {

        for (string s: os_path_list) {

            string full_path = s + "/" + cmd ; 
            exists = fileExists( full_path, &executable  );

            if( executable && exists ){
                return full_path;
            }
        } // for
    }
    return result;   
} // getFullPath



// Returns whether a file exists or not; should also set *executable to true/false 
// depending on if the user has permission to execute the file
bool fileExists(std::string full_path, bool* executable){

    struct stat stat_buff; 
    int err; 

    err = stat( full_path.c_str(), &stat_buff);

    //check if file exists. 
    if( err > -1 ){

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
int checkIfNumerical( const char* c ){

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
    cout << badCommand << ": Error running command "<< endl;
} // printError

