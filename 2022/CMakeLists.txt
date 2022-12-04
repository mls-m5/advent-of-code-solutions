project(aoc-2022)

function(day name)
    add_executable(${name} src/2022-${name}.cpp)
    set_property(TARGET ${name} PROPERTY CXX_STANDARD 20)
    add_test(NAME ${name} COMMAND ${name})
endfunction()

day(01)
day(02)
day(03)
day(04)

file(COPY data DESTINATION .)
