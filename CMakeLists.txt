cmake_minimum_required(VERSION 3.8)


set(SRC ./engine/src/server_engine.cpp ./engine/src/user_chat.cpp ./engine/src/json_parser.cpp ./engine/src/request_executor.cpp ./engine/src/cmd_line_processor.cpp)
set(PATH_SOCK socketapi/include)
set(PATH_DB dbapi/include)

set(PATH_MONGODB /usr/include/libmongoc-1.0/)
set(PATH_BSON /usr/include/libbson-1.0/)
set(LIB_MONGODB -lmongoc-1.0)
set(LIB_BSON -lbson-1.0)

include_directories (socketapi/include)
include_directories (dbapi/include)
include_directories (engine/include)

set(NAME_EXECUTABLE "chat_server")
set(NAME_TEST "chat_server_test")
set(NAME_SOCK_LIBRARY "socketapi")
set(NAME_DB_LIBRARY "dbapi")
set(PROJECT "chat_server")

if ($ENV{GITHUB_RUN_NUMBER})
	project(${PROJECT} VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
else()
	project(${PROJECT} VERSION 0.0.1)
endif()


find_package(Boost COMPONENTS program_options unit_test_framework REQUIRED)

add_library(${NAME_SOCK_LIBRARY} STATIC ./socketapi/src/server.cpp ./socketapi/src/session.cpp ./socketapi/src/session_manager.cpp ./socketapi/src/socketapi.cpp)

add_library(${NAME_DB_LIBRARY} STATIC ./dbapi/src/db.cpp)

add_executable(${NAME_EXECUTABLE} ${SRC} ./engine/src/main.cpp)


set_target_properties(${NAME_EXECUTABLE}  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

#[[
set_target_properties(${NAME_TEST} PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)
]]

target_include_directories(${NAME_EXECUTABLE} 
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_include_directories(${NAME_EXECUTABLE} 
    PRIVATE "${PATH_SOCK}" "${PATH_DB}"
)


target_include_directories(${NAME_EXECUTABLE} 
    PRIVATE "${PATH_DB}"
)

target_include_directories(${NAME_EXECUTABLE} 
    PRIVATE "${PATH_BSON}"
)

target_include_directories(${NAME_EXECUTABLE} 
    PRIVATE "${PATH_MONGODB}"
)


target_include_directories(${NAME_SOCK_LIBRARY} PRIVATE "${CMAKE_BINARY_DIR}")
target_include_directories(${NAME_SOCK_LIBRARY} PRIVATE "${PATH_SOCK}")

target_include_directories(${NAME_DB_LIBRARY} PRIVATE "${CMAKE_BINARY_DIR}")
target_include_directories(${NAME_DB_LIBRARY} PRIVATE "${PATH_DB}")

target_include_directories(${NAME_DB_LIBRARY} 
    PRIVATE ${PATH_MONGODB}
)
target_include_directories(${NAME_DB_LIBRARY} 
    PRIVATE ${PATH_BSON}
)



target_link_libraries(${NAME_EXECUTABLE} PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${NAME_EXECUTABLE} PRIVATE ${NAME_SOCK_LIBRARY})
target_link_libraries(${NAME_EXECUTABLE} PRIVATE ${NAME_DB_LIBRARY})
target_link_libraries(${NAME_EXECUTABLE} PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${NAME_EXECUTABLE} PRIVATE pthread)

target_link_libraries(${NAME_SOCK_LIBRARY} PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${NAME_SOCK_LIBRARY} PRIVATE pthread)

target_link_libraries(${NAME_DB_LIBRARY} PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${NAME_DB_LIBRARY} PRIVATE pthread)

target_link_libraries(${NAME_DB_LIBRARY} PRIVATE ${LIB_MONGODB})
target_link_libraries(${NAME_DB_LIBRARY} PRIVATE ${LIB_BSON})


if (MSVC)
    target_compile_options(${NAME_EXECUTABLE} PRIVATE
        /W4
    )
else ()
    target_compile_options(${NAME_EXECUTABLE} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS ${NAME_EXECUTABLE} RUNTIME DESTINATION bin)
install(TARGETS ${NAME_SOCK_LIBRARY} LIBRARY DESTINATION lib)
install(TARGETS ${NAME_DB_LIBRARY} LIBRARY DESTINATION lib)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT evgeniy.yurevich@gmail.com)

include(CPack)
