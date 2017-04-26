cmake_minimum_required(VERSION 2.8)

project(fsig)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11)
endif()

aux_source_directory(./source SRC_LIST)

include_directories(${BOOST_INCLUDE_PATH}
                    ${CMAKE_CURRENT_SOURCE_DIR}/../common/)
link_directories(${BOOST_LIBRARY_PATH})

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME}
    boost_system
    boost_date_time
    boost_regex
    boost_thread
    boost_filesystem
    pthread
)
