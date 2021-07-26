include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(BigIntCpp_test
               tests.cpp
               arithmatic-test.cpp
               utility-test.cpp
               karatsuba-calibration-test.cpp)

target_include_directories(BigIntCpp_test PUBLIC
                          ${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_link_libraries(BigIntCpp_test
                      gtest_main
)

include(GoogleTest)
gtest_discover_tests(BigIntCpp_test)
