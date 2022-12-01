project(aoc-2022)



add_executable(
    01
    src/2022-01.cpp
    )


file(
    COPY
    data
    DESTINATION
    .
    )
