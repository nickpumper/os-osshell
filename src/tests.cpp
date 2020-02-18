#include <string>
#include <string.h>
#include <unistd.h>

using namespace std; 

bool testFileExists();
bool fileExists( string full_path, bool* executable);


int test(){ 

    bool fileExistsResult; 

    printf("Begining Tests! ==========================\n");
    fileExistsResult = testFileExists();

    if (!fileExists){
        printf("fileExists actual: %d ----> Expected: 1\n", fileExistsResult); 
    }


    printf("Pased all tests! =========================\n");
    return 0; 
}



/*
 * Tests that the fileExists() works.  
 *  expected 1 : true,  exec = false  
 *  expected 2 : false, exec = false 
 *  expected 3 : true,  exec = true
 *
 */
bool testFileExists(){

    bool actual1, actual2, actual3; 
    bool exec1, exec2, exec3; 

    char exec_path[2048]; 

    getcwd( exec_path, 2000);
    strcat( exec_path, "../tests/executable");

    actual1 = fileExists( "/home/", &exec1 );
    actual2 = fileExists( "/abc", &exec2 ); 
    actual3 = fileExists( exec_path, &exec3 ); 


    return (actual1 && !actual2 && actual3 && !exec1 && !exec2 && !exec3 );
}

