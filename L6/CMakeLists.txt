cmake_minimum_required(VERSION 3.11) 
project(test_wt) 
#Для студии ставим флаг, что кодировка исходников UTF-8
if(MSVC)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()
set(CMAKE_CXX_STANDARD 20) 
find_package(PostgreSQL REQUIRED) 
find_package(wt CONFIG REQUIRED) 
add_executable(${PROJECT_NAME} main.cpp) 
target_link_libraries(${PROJECT_NAME} PRIVATE PostgreSQL::PostgreSQL Wt::Wt Wt::Dbo Wt::HTTP Wt::DboPostgres) 
