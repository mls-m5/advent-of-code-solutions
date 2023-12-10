project(aoc-2023)

function(day name)
    add_executable(2023-${name} src/${name}.cpp)
    target_compile_features(
        2023-${name} PRIVATE cxx_std_20)
    add_test(NAME 2023-${name} COMMAND ${name})
endfunction()


file(COPY data DESTINATION .)

day(01)
day(02)
day(03)
day(04)
day(05)
day(06)
day(07)
day(08)
