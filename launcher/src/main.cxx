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

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPSTR lpCmdLine, int nShowCmd) {
  int argc;
  auto argv = CommandLineToArgvW(GetCommandLineW(), &argc);

  if (argc < 2) {
    std::wcout << "Usage: nucleus <executable> [args]" << std::endl;
    return 1;
  }

  STARTUPINFOW startupInfo;
  PROCESS_INFORMATION processInfo;

  ZeroMemory(&startupInfo, sizeof startupInfo);
  ZeroMemory(&processInfo, sizeof processInfo);

  startupInfo.cb = sizeof startupInfo;
  startupInfo.hStdOutput = GetStdHandle(STD_INPUT_HANDLE);
  startupInfo.hStdError = GetStdHandle(STD_OUTPUT_HANDLE);

  LPCWSTR imagePath = argv[1];
  std::wstring imageArgs;

  if (argc > 2) {
    for (auto i = 2; i < argc; ++i) {
      if (i != 2) {
        imageArgs += L" ";
      }

      imageArgs += argv[i];
    }
  }

  auto result =
      CreateProcessW(imagePath,
                     (LPWSTR) imageArgs.c_str(),
                     nullptr,
                     nullptr,
                     FALSE,
                     0,
                     nullptr,
                     nullptr,
                     &startupInfo,
                     &processInfo);
  if (!result) {
    std::wcout << "Failed to launch process" << std::endl;
  }

  auto* targetLibrary = (WCHAR*) L"nucleus-patch.dll";

  std::wcout << "Injecting into process " << processInfo.dwProcessId << std::endl;
  auto nt = RhInjectLibrary(
      processInfo.dwProcessId,   // The process to inject into
      0,           // ThreadId to wake up upon injection
      EASYHOOK_INJECT_DEFAULT,
      targetLibrary, // 32-bit
      nullptr,         // 64-bit not provided
      nullptr,
      0
  );

  if (nt != 0) {
    auto err = RtlGetLastErrorString();
    std::wcout << "Failed to inject process: " << err << std::endl;
  } else {
    std::wcout << L"Library injected successfully" << std::endl;
  }

  std::wcout << "Waiting for process to exit" << std::endl;
  WaitForSingleObject(processInfo.hProcess, 0);

  std::wcout << "Shutting down" << std::endl;
  return 0;
}