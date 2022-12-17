cmake_minimum_required(VERSION 3.24)

project(aoc22 CXX)

find_package(fmt REQUIRED)

function(initialise_target target_name target_sources)
    add_executable(${target_name} ${target_sources})
    set_target_properties(${target_name}
            PROPERTIES
                CXX_STANDARD 23
                CXX_STANDARD_REQUIRED ON)

    target_link_libraries(${target_name}
            fmt::fmt)
endfunction()

function(use_supplementary_files file_names)
    foreach(file_name ${file_names})
        message("input file: ${file_name}")
        configure_file(
                ${file_name}
                ${file_name}
                COPYONLY)
    endforeach()
endfunction()

initialise_target(day1 source/day1.cpp)
initialise_target(day2 source/day2.cpp)
initialise_target(day3 source/day3.cpp)

set(supplementary_files
        source/day1.txt
        source/day2.txt
        source/day3.txt)

use_supplementary_files("${supplementary_files}")
