cmake_minimum_required (VERSION 2.6)
project(TemplateDatastructures)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#includes
include_directories("${PROJECT_SOURCE_DIR}/include")

#get module sources
FILE(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.cpp")

#get module headers
FILE(GLOB sources_h "${PROJECT_SOURCE_DIR}/include/*.h*")

message("Source  = ${sources}")
message("Includes = ${sources_h}")

#group them so visual studio will organise them
source_group("Source Files" FILES ${sources})
source_group("Header Files" FILES ${sources_h})

#compile into a executable witht he sources 
#note that headers are included so they get inserted into
#the required folder in visual studio solutions
#note that headers are ignored by cmake in this context
add_executable(TemplateDatastructures ${sources} ${sources_h} main.cpp)



