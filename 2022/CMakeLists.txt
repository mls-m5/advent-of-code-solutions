project(aoc-2022)



add_executable(
    01
    src/2022-01.cpp
    )

set_property(TARGET 01 PROPERTY CXX_STANDARD 20)

add_executable(
    02
    src/2022-02.cpp
    )

set_property(TARGET 02 PROPERTY CXX_STANDARD 20)


file(
    COPY
    data
    DESTINATION
    .
    )
