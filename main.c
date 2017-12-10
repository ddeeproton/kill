#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

char * searchProcess = "";

char * strtolower(char *str){
    for(int i = 0; str[i]; i++){
      str[i] = tolower(str[i]);
    }
    return str;
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
   int mlength = 1024;
   char strWindowTitle[mlength];
   char title[mlength];
   char cpid[mlength];
   DWORD pid = 0;
   SendMessage(hWnd, WM_GETTEXT, mlength, (LPARAM)strWindowTitle);
   if(strlen(strWindowTitle) > 0) {
       GetWindowThreadProcessId(hWnd, &pid);
       sprintf(cpid, "%d", (int)pid); // convert (int) to (char *)
       strtolower(searchProcess);
       strcpy(title, strWindowTitle);
       strtolower(title);
       if(pid != getpid() && !strstr(title, "kill.exe") && !strstr(title, "default ime") && !strstr(title, "msctfime ui")){
           if(strlen(searchProcess) == 0 || strstr(title, searchProcess) || strstr(cpid, searchProcess)) {
               printf("%d",(int)pid);
               printf("\t");
               printf(strWindowTitle);
               printf("\n ");
           }
        }
   }
   return 1;
}


BOOL CALLBACK CloseWindowsProc(HWND hWnd, LPARAM lParam)
{
   int mlength = 1024;
   char strWindowTitle[mlength];
   char title[mlength];
   char cpid[mlength];
   DWORD pid = 0;
   SendMessage(hWnd, WM_GETTEXT, mlength, (LPARAM)strWindowTitle);
   if(strlen(strWindowTitle) > 0 && strlen(searchProcess) > 0) {
       GetWindowThreadProcessId(hWnd, &pid);
       sprintf(cpid, "%d", (int)pid); // convert (int) to (char *)
       strtolower(searchProcess);
       strcpy(title, strWindowTitle);
       strtolower(title);
       if(pid != getpid() && !strstr(title, "kill.exe") && !strstr(title, "default ime") && !strstr(title, "msctfime ui")){
           if(strlen(searchProcess) == 0 || strstr(title, searchProcess) || strcmp(cpid, searchProcess) == 0) {
             PostMessage(hWnd, WM_CLOSE, 0, 0);
           }
       }
   }
   return 1;
}

int main(int argc, char *argv[])
{
  BOOL isCommand = 0;
  if(argc > 1){
    if(strcmp(argv[1], "d") == 0 || strcmp(argv[1], "display") == 0) {
      if(argc >= 3) {
        searchProcess = argv[2];
      }
      printf("PID\tWindow Title\n");
      printf("====\t============\n");
      EnumWindows(EnumWindowsProc,0);
      isCommand = 1;
    }else{
        if(strcmp(argv[1], "k") == 0 || strcmp(argv[1], "kill") == 0) {
           if(argc >= 3) {
             searchProcess = argv[2];
             EnumWindows(CloseWindowsProc,0);
             isCommand = 1;
           }
        }
    }
  }
  if(!isCommand) {
    printf("\nkill - Version 0.0.1 ( github.com/ddeeproton )\n\n");
    printf("USAGE:\n\n");
    printf("  Display all processes:\n\n");
    printf("      kill d\n\n");
    printf("      kill display\n\n");
    printf("      kill d \"Window name or pid\"\n\n");
    printf("  Kill a process:\n\n");
    printf("      kill k \"Window name or pid\" \n\n");
    printf("      kill kill \"Window name or pid\" \n\n");
  }
  //HWND wh = FindWindow(NULL, "Windows Command Processor");
  //PostMessage(wh, WM_CLOSE, 0, 0);
  return 0;
}

