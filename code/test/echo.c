#include "syscall.h"

int
main()
{
    char c;
    OpenFileId input = ConsoleInput;
    OpenFileId output = ConsoleOutput;
    char buffer[60];
    int k = 0;
    int i = 0;
    int h = 0;
    int flag = 0;
    int j = 0;
    OpenFileId fp;
    do {	    
        Read(&c, 1, input);
        buffer[i] = c;
    } while( buffer[i++] != '\n' );
    buffer[i] = '\0';

    for(;j<i;j++){
        if(buffer[j] == '>'){
            flag = 1;
            break;
        }
    }
    if(flag){
        char contents[60];
        char filename[60];
        for(k=0;k<j-1;k++)
            contents[k] = buffer[k];
        contents[j-1] = '\0';
        for(h = j+2;h<i;h++){
            if(h == '\n')
                break;
            if(h!=' ')
                filename[h-j-2] = buffer[h];
        }
        filename[i] ='\0';
        fp = Open(filename);
        Write(contents,j-1,fp);
        Close(fp);
    }
    else
        Write(buffer, i, output);
    Exit(0);
}
