rm DISK
./nachos -f
./nachos -cp ../test/shell.noff shell
./nachos -cp ../test/halt.noff exit.noff
./nachos -cp ../test/echo.noff echo.noff
./nachos -cp ../test/touch.noff touch.noff
./nachos -x shell

