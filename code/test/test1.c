#include"syscall.h"
int main(){
    SpaceId prog = Exec("../test/yield.noff");
    Join(prog);
    Exit(0);
}