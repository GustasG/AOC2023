add_executable (AOC2023
  "main.cpp"
  "day1.cpp" "day1.hpp"
  "day2.cpp" "day2.hpp"
  "utility.cpp" "utility.hpp"
)

set_property(TARGET AOC2023 PROPERTY CXX_STANDARD 20)
set_property(TARGET AOC2023 PROPERTY CXX_STANDARD_REQUIRED ON)