project(MessengerProgram)	
cmake_minimum_required(VERSION 3.16)

find_package(PostgreSQL REQUIRED)
find_package (Threads REQUIRED)

set(SOURCE_EXE MainFunction.c)			
set(SOURCE_LIB AdditionalFunction.c ConectingToPSQL.c)			

add_library(Anotherfunction STATIC ${SOURCE_LIB})	
add_executable(messenger ${SOURCE_EXE})	

target_link_libraries(messenger Anotherfunction PostgreSQL::PostgreSQL ${CMAKE_THREAD_LIBS_INIT})		
