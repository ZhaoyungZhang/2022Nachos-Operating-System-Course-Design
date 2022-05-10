#include"syscall.h"
int main(){
    OpenFileId fp;
    char buffer[50];
    int sz;
    //create 
    Create("FTest");
    // open file
    fp = Open("FTest");
    //write
    Write("Test System Call base on FileSystem!!!",39,fp);
    // read
    sz = Read(buffer,16,fp);
    // close
    Close(fp);
    // exit
    Exit(0);
}