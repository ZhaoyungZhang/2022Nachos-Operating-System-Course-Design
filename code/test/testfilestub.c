#include"syscall.h"
int main(){
    OpenFileId fp;
    char buffer[50];
    int size;
    //create 
    Create("FileStubTest");
    // open file
    fp = Open("FileStubTest");
    //write
    Write("Test System Call base on FileStub!!!",34,fp);
    // read
    size = Read(buffer,16,fp);
    // close
    Close(fp);
    // exit
    Exit(0);
}