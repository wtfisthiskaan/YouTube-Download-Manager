# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/YDM_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/YDM_autogen.dir/ParseCache.txt"
  "YDM_autogen"
  )
endif()
