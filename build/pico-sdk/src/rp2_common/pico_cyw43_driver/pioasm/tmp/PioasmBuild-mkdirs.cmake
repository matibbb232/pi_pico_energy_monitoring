# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/usr/share/pico-sdk/tools/pioasm"
  "/home/mateusz/Documents/pico/sample_project/build/pioasm"
  "/home/mateusz/Documents/pico/sample_project/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm"
  "/home/mateusz/Documents/pico/sample_project/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/home/mateusz/Documents/pico/sample_project/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
  "/home/mateusz/Documents/pico/sample_project/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/home/mateusz/Documents/pico/sample_project/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/mateusz/Documents/pico/sample_project/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/mateusz/Documents/pico/sample_project/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
