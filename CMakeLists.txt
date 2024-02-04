cmake_minimum_required(VERSION 3.10)
project(bookshop_management_system VERSION 1.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FindPkgConfig)
pkg_check_modules(LIBMYSQLCLIENT REQUIRED mysqlclient)

link_directories(${LIBMYSQLCLIENT_LIBRARY_DIRS})

add_executable(bookshop_management_system
    src/books.cpp
    src/suppliers.cpp
    src/purchases.cpp
    src/utils.cpp
    src/config.cpp
    main.cpp
)
target_include_directories(bookshop_management_system PRIVATE
    include
    ${LIBMYSQLCLIENT_INCLUDE_DIRS}
)
target_link_libraries(bookshop_management_system PRIVATE 
    ${LIBMYSQLCLIENT_LIBRARIES}
)