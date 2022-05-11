#include"syscall.h"
// create a new file
int main(){
    OpenFileId fp;
    char buffer[50];
    char filename[50];
    int sz;

    // read name
    char c;
    OpenFileId input = ConsoleInput;
    OpenFileId output = ConsoleOutput;
    int k = 0;
    int i = 0;
    int h = 0;
    do {	    
        Read(&c, 1, input);
        filename[i] = c;
    } while(filename[i++] != '\n');
    filename[i] = '\0';
    // read name end

    // hint the user
    Write("Create New File ", 17, output);
    Write(filename, i, output);

    //create 
    Create(filename);
    // open file
    fp = Open(filename);
    Close(fp);
    // exit
    Exit(0);
}