add_executable (AOC2023
  "day1.ixx"
  "day2.ixx"
  "day3.ixx"
  "day4.ixx"
  "day5.ixx"
  "day6.ixx"
  "day7.ixx"
  "main.cpp"
  "day8.ixx"
  "day9.ixx"
  "day10.ixx"
  "utility.ixx"
)

set_property(TARGET AOC2023 PROPERTY CXX_STANDARD 23)
set_property(TARGET AOC2023 PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(AOC2023 PUBLIC OpenMP::OpenMP_CXX)

if(MSVC)
  target_compile_options(AOC2023 PUBLIC "/openmp:llvm")
endif()