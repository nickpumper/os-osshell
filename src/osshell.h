#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

using namespace std; 

vector<string> splitString(string text, char d);
string getFullPath(std::string cmd, const vector<string>& os_path_list);
bool fileExists(std::string full_path, bool * executable);
string getUserInput();
void addToHistory (string input);
void clearHistory();
void printHistory(int quantity);
void detectCommand(string input );
void printError (string badCommand);
