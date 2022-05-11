// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// AdvancePC
// Write PC Register , one step forward
//----------------------------------------------------------------------
void AdvancePC(){
    machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
    machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
    machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
}

//----------------------------------------------------------------------
// StartProcess(int spaceID)
// 
//----------------------------------------------------------------------
void StartProcess(int spaceId){
    //printf("In new thread%d StartProcess\n",spaceId);
    // 初始化寄存器组
    currentThread->space->InitRegisters();
    // 加载页表
    currentThread->space->RestoreState();
    // 转入用户程序
    machine->Run();
    ASSERT(FALSE);
}

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    if (which == SyscallException) {
        //printf("Syscall = %d\n",type);
        switch (type){
            case SC_Halt:{
                //printf("Execute system call of Halt(),CurrentThreadId: %d Name:%s \n",(currentThread->space)->getSpaceId(),currentThread->getName());
                DEBUG('a',"Shutdown,initiated by user program.\n");
                interrupt->Halt();
                break;
            }
            case SC_Exec:{
                //printf("Execute system call of Exec(),CurrentThreadId: %d Name:%s \n",(currentThread->space)->getSpaceId(),currentThread->getName());
                // read args
                char filename[128];
                int addr = machine->ReadRegister(4);
                int i = 0;
                do{
                    // read filename from mainmemory
                    machine->ReadMem(addr+i,1,(int*)&filename[i]);
                }while(filename[i++] != '\0');
                // open file
                OpenFile *executable = fileSystem->Open(filename);
                if(!executable){
                    //printf("Unable to open file %s\n",filename);
                    return ;
                }
                // Apply for address space
                AddrSpace *space = new AddrSpace(executable);
                // print
                // space->Print();
                delete executable;
                // create kernel process
                Thread *thread = new Thread(filename);
                //printf("New Thread SpaceID: %d, Name: %s\n",space->getSpaceId(),filename);
                thread->Fork(StartProcess,(int)space->getSpaceId());
                // user process -> kernel process
                // printf("user process -> kernel process\n");
                thread->space = space;
                machine->WriteRegister(2,space->getSpaceId());
                AdvancePC();
                // printf("end exec\n");
                break;
            }
            case SC_Join:{
                //printf("Execute system call of Join(), CurrentThreadId: %d Name:%s \n",(currentThread->space)->getSpaceId(),currentThread->getName());
                int SpaceId = machine->ReadRegister(4);
                currentThread->Join(SpaceId);
                machine->WriteRegister(2,currentThread->waitExitCode());
                AdvancePC();
                break;
            }
            case SC_Exit:{
                //printf("Execute system call of Exit(),CurrentThreadId: %d Name:%s \n",(currentThread->space)->getSpaceId(),currentThread->getName());
                int exitCode = machine->ReadRegister(4);
                machine->WriteRegister(2,exitCode);
                currentThread->setExitCode(exitCode);
                // a speical target when father process exit, and clear List
                if(exitCode==99)
                    scheduler->emptyList(scheduler->getTerminatedList());
                delete currentThread->space;
                currentThread->Finish();
                AdvancePC();
                break;
            }
            case SC_Yield:{
                //printf("Execute system call of Yield, CurrentThreadId: %d Name:%s \n",(currentThread->space)->getSpaceId(),currentThread->getName());
                currentThread->Yield();
                AdvancePC();
                break;
            }
            
#ifdef FILESYS_STUB
            case SC_Create:{
                printf("FILESYS_STUB_SC_Create, CurrentThreadId: %d Name:%s \n",(currentThread->space)->getSpaceId(),currentThread->getName());
                int addr = machine->ReadRegister(4);
                char filename[128];
                for(int i=0;i<128;i++){
                    machine->ReadMem(addr+i,1,(int *)&filename[i]);
                    if(filename[i] == '\0') break;
                }
                int fileDescriptor = OpenForWrite(filename);
                if(fileDescriptor == -1)
                    printf("create file %s failed!\n",filename);
                else
                    printf("create file %s succeed,the file id is %d\n",filename,fileDescriptor);
                Close(fileDescriptor);
                AdvancePC();
                break;
            }
            case SC_Open:{
                printf("FILESYS_STUB_SC_Open, CurrentThreadId: %d Name:%s \n",(currentThread->space)->getSpaceId(),currentThread->getName());
                // read address
                int addr = machine->ReadRegister(4);
                char filename[128];
                for(int i=0;i<128;i++){
                    machine->ReadMem(addr+i,1,(int *)&filename[i]);
                    if(filename[i] == '\0') break;
                }

                int fileDescriptor = OpenForWrite(filename);
                if(fileDescriptor == -1) 
                    printf("Open file %s failed!\n",filename);
                else 
                    printf("Open file %s succeed, the file id is %d\n",filename, fileDescriptor);
                machine->WriteRegister(2, fileDescriptor);
                AdvancePC();
                break;
            }
            case SC_Write:{
                printf("FILESYS_STUB_SC_Write, CurrentThreadId: %d Name:%s \n",(currentThread->space)->getSpaceId(),currentThread->getName());
                int addr = machine->ReadRegister(4);
                int size = machine->ReadRegister(5);
                int fileid = machine->ReadRegister(6);
                // openfile
                OpenFile *openfile = new OpenFile(fileid);
                ASSERT(openfile != NULL);
                // read data
                char buffer[128];
                for(int i=0;i<size;i++){
                    machine->ReadMem(addr+i,1,(int*)&buffer[i]);
                    if(buffer[i] == '\0') break;
                }
                buffer[size] = '\0';
                // write data
                int writePosition;
                if(fileid == 1) writePosition = 0;
                else writePosition = openfile->Length();
                // begin at writeposition
                int WrittenBytes = openfile->WriteAt(buffer,size,writePosition);
                if(WrittenBytes == 0) printf("write file failed!\n");
                else printf("\"%s\" has wrote in file %d succeed!\n",buffer,fileid);
                AdvancePC();
                break;
            }
            case SC_Read:{
                printf("FILESYS_STUB_SC_Read, CurrentThreadId: %d Name:%s \n",(currentThread->space)->getSpaceId(),currentThread->getName());
                int addr = machine->ReadRegister(4);
                int size = machine->ReadRegister(5);
                int fileid = machine->ReadRegister(6);

                // open file and read
                char buffer[size+1];
                OpenFile *openfile = new OpenFile(fileid);
                int readbytes = openfile->Read(buffer,size);

                for(int i =0;i<size;i++)
                    if(!machine->WriteMem(addr,1,buffer[i])) printf("This is something Wrong\n");
                buffer[size] = '\0';
                printf("Read succeed,the content is \"%s\",the length is %d \n",buffer,size);
                machine->WriteRegister(2,readbytes);
                AdvancePC();
                break;
            }
            case SC_Close:{
                printf("FILESYS_STUB_SC_Close, CurrentThreadId: %d Name:%s \n",(currentThread->space)->getSpaceId(),currentThread->getName());
                int fileid = machine->ReadRegister(4);
                Close(fileid);
                printf("File %d closed succeed!\n",fileid);
                AdvancePC();
                break;
            }
#else
            case SC_Create:{
                //printf("FILESYS_SC_Create, CurrentThreadId: %d Name:%s \n",(currentThread->space)->getSpaceId(),currentThread->getName());
                int addr = machine->ReadRegister(4);
                char filename[128];
                for(int i=0;i<128;i++){
                    machine->ReadMem(addr+i,1,(int *)&filename[i]);
                    if(filename[i] == '\0') break;
                }
                // differernce in here
                if(!fileSystem->Create(filename,0))
                    printf("Create file %s failed!\n",filename);
                //else 
                    //printf("Create file %s succeed!\n",filename);
                AdvancePC();
                break;
            }case SC_Open:{
                //printf("FILESYS_SC_Open, CurrentThreadId: %d Name:%s \n",(currentThread->space)->getSpaceId(),currentThread->getName());
                // read address
                int addr = machine->ReadRegister(4);
                int fileid;
                char filename[128];
                for(int i=0;i<128;i++){
                    machine->ReadMem(addr+i,1,(int *)&filename[i]);
                    if(filename[i] == '\0') break;
                }
                //different
                OpenFile *openfile = fileSystem->Open(filename);
                if(!openfile){
                    printf("File \"%s\" not exists,could not open!\n",filename);
                    fileid = -1;
                }else{
                    fileid = currentThread->space->getFileDescriptor(openfile);
                    if(fileid < 0)
                        printf("Too many files have been Open\n");
                    // else
                    //     printf("File \"%s\" open succeed!File id = %d.\n",filename,fileid);
                }
                machine->WriteRegister(2, fileid);
                AdvancePC();
                break;
            }
            case SC_Write:{
                //printf("FILESYS_SC_Write, CurrentThreadId: %d Name:%s \n",(currentThread->space)->getSpaceId(),currentThread->getName());
                int addr = machine->ReadRegister(4);
                int size = machine->ReadRegister(5);
                int fileid = machine->ReadRegister(6);

                // create a openfile just like 文件句柄
                OpenFile *openfile = new OpenFile(fileid);
                ASSERT(openfile != NULL);

                // read data
                char buffer[128];
                for(int i=0;i<size;i++){
                    machine->ReadMem(addr+i,1,(int*)&buffer[i]);
                    if(buffer[i] == '\0') break;
                }
                buffer[size] = '\0';

                // open Nachos file
                openfile = currentThread->space->getFileId(fileid);
                if(!openfile){
                    // no file
                    printf("Fail to Open File Fileid = %d\n",fileid);
                    AdvancePC();
                    break;
                }
                if(fileid == 1 || fileid == 2){
                    // stdout or stderr
                    openfile->WriteStdout(buffer,size);
                    AdvancePC();
                    break;
                }

                // write data
                int writePosition = openfile->Length();
                openfile->Seek(writePosition);

                // begin at writeposition
                int WrittenBytes = openfile->Write(buffer,size);
                if(WrittenBytes == 0) printf("write file failed!\n");
                //else if(fileid!=1 || fileid!= 2) 
                //printf("\"%s\" has wrote in file id = %d succeed!Size = %d\n",buffer,fileid,size);
                AdvancePC();
                break;
            }
            case SC_Read:{
                //printf("FILESYS_SC_Read, CurrentThreadId: %d Name:%s \n",(currentThread->space)->getSpaceId(),currentThread->getName());
                int addr = machine->ReadRegister(4);
                int size = machine->ReadRegister(5);
                int fileid = machine->ReadRegister(6);

                // open file and read
                OpenFile *openfile = currentThread->space->getFileId(fileid);
                // read
                char buffer[size+1];
                int readbytes = 0;
                if(fileid == 0) readbytes = openfile->ReadStdin(buffer,size);
                else readbytes = openfile->Read(buffer,size);

                for(int i=0;i<readbytes;i++)
                    machine->WriteMem(addr,1,buffer[i]);
                buffer[readbytes] = '\0';

                for(int i =0;i<readbytes;i++)
                    if(buffer[i]>0 && buffer[i] <= 9) 
                        buffer[i] += 0x30;
                char *bufcopy = buffer;
                if(readbytes > 0){
                    // if(fileid != 0)
                    //     printf("Read file %d succeed! the content is \"%s\", the length = %d\n",fileid,bufcopy,readbytes);
                }
                else printf("Read file failed!\n");
                machine->WriteRegister(2,readbytes);
                AdvancePC();
                break;
            }
            case SC_Close:{
                //printf("FILESYS_SC_Close, CurrentThreadId: %d Name:%s \n",(currentThread->space)->getSpaceId(),currentThread->getName());
                int fileid = machine->ReadRegister(4);
                OpenFile *openfile = currentThread->space->getFileId(fileid);
                if(openfile!=NULL){
                    // write back to disk
                    openfile->WriteBack();
                    delete openfile;
                    currentThread->space->releaseFileDescriptor(fileid);
                    //printf("File %d closed succeed!\n",fileid);
                }else printf("Failed to Close File %d.\n",fileid);
                AdvancePC();
                break;
            }
#endif
            default:{
                printf("Unexpected user mode exception %d %d\n", which, type);
                ASSERT(FALSE);
                break;
            }
        }
    } else {
        printf("Unexpected user mode exception which = %d type = %d\n", which, type);
        ASSERT(FALSE);
    }
}
