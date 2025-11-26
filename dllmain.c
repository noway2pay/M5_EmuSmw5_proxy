#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <stdio.h>
#include <time.h>

//----------------------------------------------------------------------------------------------------

const char ORIGINAL_DLL[] = "original-M5_EmuSmw5.dll";
HMODULE h_originalDll = NULL;
static DWORD timeStamp = 0;
static char fileName[128];
CRITICAL_SECTION CriticalSection;

//----------------------------------------------------------------------------------------------------

void putToTextFile(char* dbgString);
void putToTextFileV4(char* dbgString, INT32 ret, BYTE arg1, BYTE arg2, BYTE arg3, BYTE arg4, char* comment);
void putToBinaryFile(char* prefix, BYTE* buffer, DWORD len);
char* deviceControl(INT32 arg);
char* soundControl(INT32 arg);

//----------------------------------------------------------------------------------------------------
typedef void (*T_pf_func)(void);
typedef INT32 (*T_pf_v4_func)(BYTE, BYTE, BYTE, BYTE);
typedef INT32(*T_pf_v5_func)(INT32, INT32, BYTE, void*, void*);


#define EXPORTED_FUNCTION __declspec(dllexport, naked)
#define EXPORTED_V4_FUNCTION __declspec(dllexport)
#define EXPORTED_V5_FUNCTION __declspec(dllexport)

#define MAKE_ENTRY(NAME) T_pf_func pf_ ## NAME; \
EXPORTED_FUNCTION void NAME(void) { \
  putToTextFile(#NAME); \
  __asm { jmp [ pf_ ## NAME ] } ; \
}; 

#define MAKE_V4_ENTRY(NAME) T_pf_v4_func pf_ ## NAME; \
EXPORTED_V4_FUNCTION INT32 NAME(BYTE arg1, BYTE arg2, BYTE arg3, BYTE arg4) { \
  INT32 out = pf_ ## NAME (arg1, arg2, arg3, arg4); \
  char* comment = deviceControl(arg1); \
  putToTextFileV4(#NAME, out, arg1, arg2, arg3, arg4, comment); \
  return out; \
}; 

#define MAKE_V5_ENTRY(NAME) T_pf_v5_func pf_ ## NAME; \
EXPORTED_V5_FUNCTION INT32 NAME(INT32 arg1, INT32 arg2, BYTE arg3, void* arg4, void* arg5) { \
  INT32 out = pf_ ## NAME (arg1, arg2, arg3, arg4, arg5); \
  char* comment = soundControl(arg1); \
  putToTextFileV4(#NAME, out, arg3, 0, 0, 0, comment); \
  return out; \
}; 

//----------------------------------------------------------------------------------------------------

char* deviceControl(INT32 arg) {
  switch (arg) {
  default:
    return "???";
  case 0:
    return "Pwr mng dig";
  case 1:
    return "Pwr mng ana";
  case 2:
    return "EQ vol";
  case 3:
    return "HP vol";
  case 4:
    return "SP vol";
  case 5:
    return "LED master";
  case 6:
    return "LED blink";
  case 7:
    return "LED direct";
  case 8:
    return "MTR master";
  case 9:
    return "MTR blink";
  case 10:
    return "MTR direct";
  case 11:
    return "GET PLL";
  case 13:
    return "HW init";
  case 17:
    return "WR imreg";
  case 18:
    return "RD imreg";
  case 20:
    return "GET dev type";
  }
}

char* soundControl(INT32 arg) {
  switch (arg) {
  default:
    return "???";
  case 0:
    return "SET vol";
  case 1:
    return "SET speed";
  case 2:
    return "SET keyctrl";
  case 12:
    return "SET start";
  case 13:
    return "SET end";
  case 14:
    return "SET pan";
  case 17:
    return "SET evt note";
  case 20:
    return "SET load info";
  case 22:
    return "SET HV voice";
  case 23:
    return "SET HV prog nr";
  case 26:
    return "SET speed W";
  case 31:
    return "SET HV vol";
  }
}


//----------------------------------------------------------------------------------------------------

#define IMPORT_ENTRY(NAME) pf_ ## NAME = (T_pf_func)GetProcAddress(h_originalDll, #NAME);
#define IMPORT_V4_ENTRY(NAME) pf_ ## NAME = (T_pf_v4_func)GetProcAddress(h_originalDll, #NAME);
#define IMPORT_V5_ENTRY(NAME) pf_ ## NAME = (T_pf_v5_func)GetProcAddress(h_originalDll, #NAME);

//----------------------------------------------------------------------------------------------------

MAKE_ENTRY(MaSound_GetEmuInfo)
MAKE_ENTRY(MaSound_EmuInitialize)
MAKE_ENTRY(MaSound_Generate)
MAKE_ENTRY(MaSound_EmuTerminate)
MAKE_ENTRY(MaSound_Initialize)
MAKE_ENTRY(MaSound_Terminate)
MAKE_V4_ENTRY(MaSound_DeviceControl)
MAKE_ENTRY(MaSound_Create)
MAKE_ENTRY(MaSound_Load)
MAKE_ENTRY(MaSound_Open)
MAKE_V5_ENTRY(MaSound_Control)
MAKE_ENTRY(MaSound_Standby)
MAKE_ENTRY(MaSound_Seek)
MAKE_ENTRY(MaSound_Start)
MAKE_ENTRY(MaSound_Stop)
MAKE_ENTRY(MaSound_Pause)
MAKE_ENTRY(MaSound_Restart)
MAKE_ENTRY(MaSound_Close)
MAKE_ENTRY(MaSound_Unload)
MAKE_ENTRY(MaSound_Delete)
MAKE_ENTRY(Phrase_Initialize)
MAKE_ENTRY(Phrase_GetInfo)
MAKE_ENTRY(Phrase_CheckData)
MAKE_ENTRY(Phrase_SetData)
MAKE_ENTRY(Phrase_Seek)
MAKE_ENTRY(Phrase_Play)
MAKE_ENTRY(Phrase_Pause)
MAKE_ENTRY(Phrase_Restart)
MAKE_ENTRY(Phrase_Stop)
MAKE_ENTRY(Phrase_RemoveData)
MAKE_ENTRY(Phrase_Kill)
MAKE_ENTRY(Phrase_GetStatus)
MAKE_ENTRY(Phrase_GetPosition)
MAKE_ENTRY(Phrase_GetLength)
MAKE_ENTRY(Phrase_GetVolume)
MAKE_ENTRY(Phrase_GetPanpot)
MAKE_ENTRY(Phrase_GetLink)
MAKE_ENTRY(Phrase_SetVolume)
MAKE_ENTRY(Phrase_SetPanpot)
MAKE_ENTRY(Phrase_SetLink)
MAKE_ENTRY(Phrase_SetEvHandler)
MAKE_ENTRY(Phrase_Terminate)
MAKE_ENTRY(SetMidiMsg)
MAKE_ENTRY(GetMidiMsg)
MAKE_ENTRY(malib_MakeCRC)
MAKE_ENTRY(malib_Decode_7bitData)
MAKE_ENTRY(MaSound_SetEmuPath)
MAKE_ENTRY(malib_smafphrase_checker)
MAKE_ENTRY(malib_smafaudio_checker)
MAKE_ENTRY(malib_hvscript_checker)
MAKE_ENTRY(MaSound_DeviceControlEx)

//----------------------------------------------------------------------------------------------------

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
#ifdef _DEBUG
      //OutputDebugStringA("[DBG]DLL_PROCESS_ATTACH\n");
#endif
      if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400)) {
        return FALSE;
      }

      h_originalDll = LoadLibrary(ORIGINAL_DLL);

      if (h_originalDll == NULL)
      {
        return FALSE;
      }
      else
      {
        time_t timer;
        time(&timer);
        timeStamp = (DWORD) timer;
        //sprintf(fileName, "d:\\M5_EmuSmw5_%d.txt", timeStamp);
        sprintf(fileName, "d:\\MA5-dbg.txt");
        
        IMPORT_ENTRY(MaSound_GetEmuInfo)
        IMPORT_ENTRY(MaSound_EmuInitialize)
        IMPORT_ENTRY(MaSound_Generate)
        IMPORT_ENTRY(MaSound_EmuTerminate)
        IMPORT_ENTRY(MaSound_Initialize)
        IMPORT_ENTRY(MaSound_Terminate)
        IMPORT_V4_ENTRY(MaSound_DeviceControl)
        IMPORT_ENTRY(MaSound_Create)
        IMPORT_ENTRY(MaSound_Load)
        IMPORT_ENTRY(MaSound_Open)
        IMPORT_V5_ENTRY(MaSound_Control)
        IMPORT_ENTRY(MaSound_Standby)
        IMPORT_ENTRY(MaSound_Seek)
        IMPORT_ENTRY(MaSound_Start)
        IMPORT_ENTRY(MaSound_Stop)
        IMPORT_ENTRY(MaSound_Pause)
        IMPORT_ENTRY(MaSound_Restart)
        IMPORT_ENTRY(MaSound_Close)
        IMPORT_ENTRY(MaSound_Unload)
        IMPORT_ENTRY(MaSound_Delete)
        IMPORT_ENTRY(Phrase_Initialize)
        IMPORT_ENTRY(Phrase_GetInfo)
        IMPORT_ENTRY(Phrase_CheckData)
        IMPORT_ENTRY(Phrase_SetData)
        IMPORT_ENTRY(Phrase_Seek)
        IMPORT_ENTRY(Phrase_Play)
        IMPORT_ENTRY(Phrase_Pause)
        IMPORT_ENTRY(Phrase_Restart)
        IMPORT_ENTRY(Phrase_Stop)
        IMPORT_ENTRY(Phrase_RemoveData)
        IMPORT_ENTRY(Phrase_Kill)
        IMPORT_ENTRY(Phrase_GetStatus)
        IMPORT_ENTRY(Phrase_GetPosition)
        IMPORT_ENTRY(Phrase_GetLength)
        IMPORT_ENTRY(Phrase_GetVolume)
        IMPORT_ENTRY(Phrase_GetPanpot)
        IMPORT_ENTRY(Phrase_GetLink)
        IMPORT_ENTRY(Phrase_SetVolume)
        IMPORT_ENTRY(Phrase_SetPanpot)
        IMPORT_ENTRY(Phrase_SetLink)
        IMPORT_ENTRY(Phrase_SetEvHandler)
        IMPORT_ENTRY(Phrase_Terminate)
        IMPORT_ENTRY(SetMidiMsg)
        IMPORT_ENTRY(GetMidiMsg)
        IMPORT_ENTRY(malib_MakeCRC)
        IMPORT_ENTRY(malib_Decode_7bitData)
        IMPORT_ENTRY(MaSound_SetEmuPath)
        IMPORT_ENTRY(malib_smafphrase_checker)
        IMPORT_ENTRY(malib_smafaudio_checker)
        IMPORT_ENTRY(malib_hvscript_checker)
        IMPORT_ENTRY(MaSound_DeviceControlEx)
      }
      break;


    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

//----------------------------------------------------------------------------------------------------

static char newLine[] = { "\n" };

void putToTextFile(char* dbgString)
{
  EnterCriticalSection(&CriticalSection);

  HANDLE hFile;
  DWORD dwBytesToWrite = (DWORD)strlen(dbgString);
  DWORD dwBytesWritten = 0;
  BOOL bErrorFlag = FALSE;

  hFile = CreateFile(fileName,
    GENERIC_WRITE,
    0,
    NULL,
    OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    NULL);

  if (hFile != INVALID_HANDLE_VALUE)
  {
    SetFilePointer(hFile, 0, NULL, FILE_END);

    /*bErrorFlag =*/ WriteFile(
      hFile,
      dbgString,
      dwBytesToWrite,
      &dwBytesWritten,
      NULL);

    /*bErrorFlag =*/ WriteFile(
      hFile,
      newLine,
      1,
      &dwBytesWritten,
      NULL);

    CloseHandle(hFile);
  }

  LeaveCriticalSection(&CriticalSection);
}

void putToTextFileV4(char* dbgString, INT32 ret, BYTE arg1, BYTE arg2, BYTE arg3, BYTE arg4, char* comment)
{
  EnterCriticalSection(&CriticalSection);

  HANDLE hFile;
  DWORD dwBytesToWrite = (DWORD)strlen(dbgString);
  DWORD dwBytesWritten = 0;
  BOOL bErrorFlag = FALSE;

  hFile = CreateFile(fileName, // name of the write
    GENERIC_WRITE,          // open for writing
    0,                      // do not share
    NULL,                   // default security
    OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,  // normal file
    NULL);                  // no attr. template

  if (hFile != INVALID_HANDLE_VALUE)
  {
    SetFilePointer(hFile, 0, NULL, FILE_END);

    /*bErrorFlag =*/ WriteFile(
      hFile,            // open file handle
      dbgString,        // start of data to write
      dwBytesToWrite,   // number of bytes to write
      &dwBytesWritten,  // number of bytes that were written
      NULL);            // no overlapped structure

    char buff[64];
    int len = sprintf(buff, ":%02X %02X %02X %02X=%X (%s)\n", arg1, arg2, arg3, arg4, ret, comment);

    /*bErrorFlag =*/ WriteFile(
      hFile,
      buff,
      len,
      &dwBytesWritten,
      NULL);

    CloseHandle(hFile);
  }

  LeaveCriticalSection(&CriticalSection);
}



void putToBinaryFile(char* prefix, BYTE* buffer, DWORD len)
{
  HANDLE hFile;
  DWORD dwBytesWritten = 0;
  BOOL bErrorFlag = FALSE;

  time_t timer;
  time(&timer);
  DWORD localTimeStamp = (DWORD)timer;

#if 1
  char fileName[32];
  sprintf_s(fileName, sizeof(fileName), "d:\\%s_%d.bin", prefix, localTimeStamp);

  hFile = CreateFile(fileName,
    GENERIC_WRITE,
    0,
    NULL,
    OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    NULL);
#else
  hFile = CreateFile(prefix, // name of the write
    GENERIC_WRITE,          // open for writing
    0,                      // do not share
    NULL,                   // default security
    OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,  // normal file
    NULL);                  // no attr. template
#endif

  if (hFile == INVALID_HANDLE_VALUE)
  {
    return;
  }

  bErrorFlag = WriteFile(
    hFile,
    buffer,
    len,
    &dwBytesWritten,
    NULL);

  CloseHandle(hFile);
}


