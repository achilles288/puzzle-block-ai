set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/puzzleblock/$<0:>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/puzzleblock/$<0:>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/puzzleblock/$<0:>)


find_package(Java REQUIRED)
find_package(JNI REQUIRED)

include(UseJava)

include_directories(${JNI_INCLUDE_DIRS})




# 
# C++ route-finding library
# 
add_library(puzzleblock SHARED
    puzzleblock/c++/puzzleblock.cpp
    puzzleblock/c++/puzzleblock.h
    puzzleblock/c++/routefinding.cpp
    puzzleblock/c++/routefinding.h
)

set_target_properties(puzzleblock PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_link_libraries(puzzleblock PUBLIC ${JNI_LIBRARIES})




# 
# Java program
# 
add_jar(app
    puzzleblock/Block.java
    puzzleblock/Board.java
    puzzleblock/Effect.java
    puzzleblock/PuzzleBlock.java
    puzzleblock/PuzzleBlock_AI.java
    RESOURCES
        NAMESPACE puzzleblock puzzleblock/images/cm_0.jpg
        NAMESPACE puzzleblock puzzleblock/images/cm_1.jpg
        NAMESPACE puzzleblock puzzleblock/images/cm_2.jpg
        NAMESPACE puzzleblock puzzleblock/images/cm_3.jpg
        NAMESPACE puzzleblock puzzleblock/images/cm_4.jpg
        NAMESPACE puzzleblock puzzleblock/images/cm_5.jpg
        NAMESPACE puzzleblock puzzleblock/images/cm_6.jpg
        NAMESPACE puzzleblock puzzleblock/images/cm_7.jpg
        NAMESPACE puzzleblock puzzleblock/images/cm_8.jpg
        NAMESPACE puzzleblock puzzleblock/images/cm_9.jpg
        NAMESPACE puzzleblock puzzleblock/images/cm_10.jpg
        NAMESPACE puzzleblock puzzleblock/images/cm_11.jpg
        NAMESPACE puzzleblock puzzleblock/images/cm_12.jpg
        NAMESPACE puzzleblock puzzleblock/images/cm_13.jpg
        NAMESPACE puzzleblock puzzleblock/images/cm_14.jpg
    MANIFEST META-INF/MANIFEST.MF
    OUTPUT_DIR ${CMAKE_BINARY_DIR}/puzzleblock
)