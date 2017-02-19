include_directories(${StaticMode_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Catch)

add_executable(StaticMode_test StaticMode_test.cpp)

if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))

  # -Wno-exit-time-destructors is because Catch TEST macros introduce exit-time dtors.
  set_target_properties(StaticMode_test PROPERTIES COMPILE_FLAGS "-Wno-exit-time-destructors" )

endif()

# Cause Catch to emit main()
target_compile_definitions(StaticMode_test PUBLIC -DCATCH_CONFIG_MAIN)

add_test(NAME StaticMode_test COMMAND StaticMode_test)
