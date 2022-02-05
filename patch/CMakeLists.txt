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
project(patch CXX)

add_library(nucleus-patch SHARED src/main.cxx)
set_target_properties(nucleus-patch PROPERTIES PREFIX "")

target_include_directories(nucleus-patch PRIVATE ${EasyHook_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(nucleus-patch ${EasyHook_LIBRARY})