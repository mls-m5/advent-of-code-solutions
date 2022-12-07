project(aoc-2022)

function(day name)
    add_executable(${name} src/2022-${name}.cpp)
    set_property(TARGET ${name} PROPERTY CXX_STANDARD 20)
    add_test(NAME ${name} COMMAND ${name})
endfunction()


file(COPY data DESTINATION .)

day(01)
day(02)
day(03)
day(04)
day(05)
day(06)
day(07)
