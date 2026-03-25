# ── Bibliotheque principale ───────────────────────────────────────────────────
add_library(telesimcore_lib STATIC)

target_sources(telesimcore_lib
  PRIVATE
    # Les fichiers sources seront ajoutés ici phase par phase
    core/modulation/BpskDemodulator.cpp
    core/modulation/BpskModulator.cpp
    core/channel/AwgnChannel.cpp
    core/TransmissionPipeline.cpp
    core/modulation/QpskModulator.cpp
    core/modulation/QpskDemodulator.cpp
)

target_include_directories(telesimcore_lib
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ── Executable principal ──────────────────────────────────────────────────────
add_executable(telesimcore_app
  app/main.cpp
)

target_link_libraries(telesimcore_app
  PRIVATE
    telesimcore_lib
)
