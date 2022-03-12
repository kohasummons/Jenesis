

rem compile all cpp files in engine directory
g++ -I.\deps\include -c deps/include/jenesis/*.cpp 

rem move all compiled files to one directory
cmd /k move *.o objs