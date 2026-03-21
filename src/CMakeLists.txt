# Bibliothèque principale C++ (le coeur du projet)
add_library(telesimcore_lib STATIC)

target_sources(telesimcore_lib
  PRIVATE
    # Les fichiers sources seront ajoutés ici phase par phase
    core/modulation/BpskModulator.cpp
    core/channel/AwgnChannel.cpp
)

target_include_directories(telesimcore_lib
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
