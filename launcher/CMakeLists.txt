# Copyright (C)  Johannes Donath <johannesd@torchmind.com>
# and other copyright owners as documented in the project's IP log.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
project(launcher CXX)

add_executable(nucleus WIN32 src/main.cxx)

target_include_directories(nucleus PRIVATE ${EasyHook_INCLUDE_DIR})
target_link_libraries(nucleus ${EasyHook_LIBRARY})

install(TARGETS nucleus nucleus-patch RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/dist)
install(FILES ${LICENSES} ${EasyHook_BINARIES} DESTINATION ${CMAKE_BINARY_DIR}/dist)
install(FILES ${EasyHook_LICENSES} RENAME EasyHook_LICENSE DESTINATION ${CMAKE_BINARY_DIR}/dist)

add_custom_target(create_zip COMMAND
    ${CMAKE_COMMAND} -E tar "cfv" "nucleus.zip" --format=zip
    "${CMAKE_BINARY_DIR}/dist/")