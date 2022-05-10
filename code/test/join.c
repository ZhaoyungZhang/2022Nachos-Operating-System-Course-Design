#include "syscall.h"

int main(){
    SpaceId joinee = Exec("../test/exit.noff");
    Join(joinee);
    Exit(0);
}