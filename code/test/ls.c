#include "syscall.h"

int
main()
{
    char c;
    OpenFileId input = ConsoleInput;
    OpenFileId output = ConsoleOutput;
    // ls
    List();
    Exit(0);
}
