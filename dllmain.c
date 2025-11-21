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
void putToBinaryFile(char* prefix, BYTE* buffer, DWORD len);

//----------------------------------------------------------------------------------------------------
typedef void (*T_pf_func)(void);
#define EXPORTED_FUNCTION __declspec(dllexport, naked)

#define MAKE_POINTER_ONLY(NAME) T_pf_func pf_ ## NAME;

#define MAKE_ENTRY(NAME) T_pf_func pf_ ## NAME; \
EXPORTED_FUNCTION void NAME(void) { putToTextFile(#NAME); __asm { jmp [ pf_ ## NAME ] } }; 

//----------------------------------------------------------------------------------------------------

#define IMPORT_ENTRY(NAME) pf_ ## NAME = (T_pf_func)GetProcAddress(h_originalDll, #NAME);

//----------------------------------------------------------------------------------------------------

MAKE_ENTRY(MaSound_GetEmuInfo)
MAKE_ENTRY(MaSound_EmuInitialize)
MAKE_ENTRY(MaSound_Generate)
MAKE_ENTRY(MaSound_EmuTerminate)
MAKE_ENTRY(MaSound_Initialize)
MAKE_ENTRY(MaSound_Terminate)
MAKE_ENTRY(MaSound_DeviceControl)
MAKE_ENTRY(MaSound_Create)
MAKE_ENTRY(MaSound_Load)
MAKE_ENTRY(MaSound_Open)
MAKE_ENTRY(MaSound_Control)
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
        IMPORT_ENTRY(MaSound_DeviceControl)
        IMPORT_ENTRY(MaSound_Create)
        IMPORT_ENTRY(MaSound_Load)
        IMPORT_ENTRY(MaSound_Open)
        IMPORT_ENTRY(MaSound_Control)
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
  HANDLE hFile;
  DWORD dwBytesToWrite = (DWORD)strlen(dbgString);
  DWORD dwBytesWritten = 0;
  BOOL bErrorFlag = FALSE;

  EnterCriticalSection(&CriticalSection);

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


