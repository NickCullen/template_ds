#Install
To build the project you must have cmake installed as there are no project solutions available (i.e. cmake will generate these on the given platform)

###Command line install
- cd Path/of/project/root (the directory where the include and src folders are)
- mkdir build
- cd build
- cmake ..

Once you have run cmake the project files will have been built in the build directory. Simply open up TemplateDatastructure.sln (windows) and right click TemplateDatastructure project in the project explorer to set it as the startup project.

#Description
A git repo containing a bunch of template datastructures I am creating out of the engine workspace so I can perform unit and stress tests on them. 
As all the code is written in the header files you should only need to #include "tds.h" and be good to go. This is basically a header which which includes all the types of template data structures.

Feel free to use this :)

#Support Platorms
Thanks to cmake this should be cross platform as it doesn't rely on any platform specfic code. Although I have not yet tested it on may platforms. Here are a list of the ones I have tested the project on:
*Windows
*Mac