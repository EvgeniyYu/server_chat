cmake_minimum_required(VERSION 3.8)

set(NAME_EXECUTABLE "chat_client")

if ($ENV{GITHUB_RUN_NUMBER})
	project(${NAME_EXECUTABLE} VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
else()
	project(${NAME_EXECUTABLE} VERSION 0.0.1)
endif()


find_package(Boost COMPONENTS unit_test_framework REQUIRED)


add_executable(${NAME_EXECUTABLE} ${SRC} ./main.cpp)

set_target_properties(${NAME_EXECUTABLE} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_include_directories(${NAME_EXECUTABLE} 
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(${NAME_EXECUTABLE} PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${NAME_EXECUTABLE} PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${NAME_EXECUTABLE} PRIVATE pthread)

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

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT evgeniy.yurevich@gmail.com)

include(CPack)

