// Copyright (C)  Johannes Donath <johannesd@torchmind.com>
// and other copyright owners as documented in the project's IP log.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <easyhook.h>
#include <iostream>
#include <windows.h>
#include <winuser.h>
#include "main.hxx"

/**
 * Replaces the functionality of {@code GetLogicalProcessorInformation} with a function which will
 * always fail.
 *
 * @param Buffer a pointer to a buffer consisting of SYSTEM_LOGICAL_PROCESSOR_INFORMATION.
 * @param ReturnedLength a pointer to an integer which identifies the number of returned results.
 * @return true if successful.
 */
BOOL WINAPI GetLogicalProcessorInformationHook(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION Buffer,
                                               PDWORD ReturnedLength) {
  // simply fail the call - most games do not seem to have a problem with this and will just
  // continue without setting affinity in this case
  *ReturnedLength = 0;
  return FALSE;
}

void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo) {
  std::cout << "Injected by process Id: " << inRemoteInfo->HostPID << std::endl;

  HOOK_TRACE_INFO hHook = {nullptr};
  auto result = LhInstallHook(
      reinterpret_cast<void*>(GetProcAddress(GetModuleHandle(TEXT("kernel32")),
                                             "GetLogicalProcessorInformation")),
      reinterpret_cast<void*>(GetLogicalProcessorInformationHook),
      nullptr,
      &hHook);

  if (FAILED(result)) {
    std::wstring s;
    s += L"Hook installation failed with error message: ";
    s += RtlGetLastErrorString();

    MessageBoxW(nullptr,
                (LPCWSTR) s.c_str(),
                (LPCWSTR) L"Patch Error",
                MB_APPLMODAL | MB_ICONERROR | MB_OK);
    std::wcout << "Failed to install hook: " << s << std::endl;
  } else {
    std::cout << "Hook installed successfully." << std::endl;
  }

  // enable the hook on all threads but the current one (this is required even if we do not call
  // the original function from our thread)
  ULONG ACLEntries[1] = {0};
  LhSetExclusiveACL(ACLEntries, 1, &hHook);
}