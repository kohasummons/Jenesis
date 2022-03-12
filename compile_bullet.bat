




rem compile all cpp files in bullet
g++ -I./deps/include/bullet -c  deps/include/bullet/LinearMath/TaskScheduler/*.cpp

rem move all compiled files to one directory
cmd /k move *.o objs/bullet