include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

# ── Tests unitaires ──────────────────────────────────────────────────────────
add_executable(unit_tests
  unit/test_bpsk_modulator.cpp
  unit/test_awgn_channel.cpp
  unit/test_bpsk_demodulator.cpp
  unit/test_qpsk_modulator.cpp
  unit/test_qpsk_demodulator.cpp
  unit/test_transmission_pipeline.cpp
)
target_link_libraries(unit_tests
  PRIVATE
    telesimcore_lib
    GTest::gtest_main
)
# Dossier tests/ accessible pour #include "mocks/PerfectChannel.hpp"
target_include_directories(unit_tests
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
add_test(NAME unit_tests COMMAND unit_tests)

# ── Tests d'intégration ──────────────────────────────────────────────────────
add_executable(integration_tests
  #integration/test_qpsk_chain.cpp
  #integration/test_bpsk_chain.cpp
  integration/test_bpsk_qpsk_chain.cpp
)
target_link_libraries(integration_tests
  PRIVATE
    telesimcore_lib
    GTest::gtest_main
)
add_test(NAME integration_tests COMMAND integration_tests)