#include <string>
#include <tchar.h>
#include <windows.h>


double toSeconds(FILETIME f) { return *((__int64 *)&f) / 1000000.0; }

int wmain(int argc, wchar_t *argv[]) {
  auto reportErrorAndExit = [](const char *msg, auto... pfArgs) {
    printf(msg, pfArgs...);
    exit(1);
  };
  std::wstring child_cmd;
  for (int i = 1; i < argc; ++i) {
    child_cmd += std::wstring(L"\"") + argv[i] + L"\" ";
  }
  child_cmd += L'\0';
  STARTUPINFO startupInfo;
  PROCESS_INFORMATION processInfo;
  memset(&startupInfo,0,sizeof(startupInfo));
  memset(&processInfo,0,sizeof(processInfo));
  startupInfo.cb = sizeof(startupInfo);
  auto createProcessRet =
      CreateProcess(NULL, &(child_cmd[0]), NULL, NULL, FALSE, 0, NULL, NULL,
                    &startupInfo, &processInfo);
  if (!createProcessRet) {
    reportErrorAndExit("Failed to Create Process(%d)\n", GetLastError());
  }
  WaitForSingleObject(processInfo.hProcess, INFINITE);
  FILETIME lpCreationTime, lpExitTime, lpKernelTime, lpUserTime;
  auto processTimeRet =
      GetProcessTimes(processInfo.hProcess, &lpCreationTime, &lpExitTime,
                      &lpKernelTime, &lpUserTime);
  if (!processTimeRet) {
    reportErrorAndExit("Failed to Get Process Timer(%d)\n", GetLastError());
  }
  printf("kernal time: %lf\nuser time: "
         "%lf\nelapsed time : %lf\n",
         toSeconds(lpKernelTime), toSeconds(lpUserTime),
         toSeconds(lpExitTime) - toSeconds(lpCreationTime));
  CloseHandle(processInfo.hProcess);
  CloseHandle(processInfo.hThread);
}