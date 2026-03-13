include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

add_executable(telesimcore_tests
  unit/test_placeholder.cpp
)

target_link_libraries(telesimcore_tests
  PRIVATE
    telesimcore_lib
    GTest::gtest_main
)

add_test(
  NAME telesimcore_tests
  COMMAND telesimcore_tests
)
