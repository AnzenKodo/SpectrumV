// For smaller header files
#define WIN32_LEAN_AND_MEAN
#define NOCOMM
#include <Windows.h>

#define STRSAFE_NO_CCH_FUNCTIONS
#define STRSAFE_LOCALE_FUNCTIONS
#include <strsafe.h>

typedef struct compiler_commands
{
    char* ComplerArgs;
    char* LinkerArgs;
    char* DisableLinkerArgs;
    char* NoCrtComplerArgs;
    char* NoCrtLinkerArgs;
    char* FileName;
} compiler_commands;

static char* FILE_LOCATION = "x64/Debug/SpectrumV";
static compiler_commands Compiler = {
    "cl -Wall -Fo%s -Fe%s_cl",
    " -link user32.lib Gdi32.lib -SUBSYSTEM:WINDOWS",
    " -c",
    " -Zi -GF -GS- -GR- -EHsca- -nologo -fp:fast -fp:except- -Oi -Ob3",
    " kernel32.lib -nodefaultlib -incremental:no -opt:ref -manifest:no",
    " main.c",
};
static _Bool ComplerStatus = 1;
static _Bool LinkerStatus  = 0;
static _Bool RunningStatus = 0;
static _Bool ShowHelp      = 0;

#define WORD_BUFFER_SIZE 500
static char     Commands[WORD_BUFFER_SIZE];
static HANDLE   StdOut;

void PrintMessage(const char* Message)
{
    DWORD NumberOfCharsToWrite = (DWORD)lstrlenA(Message);
    if (!WriteConsoleA(StdOut, Message, NumberOfCharsToWrite, 0, 0))
    {
        DWORD ExitCode = GetLastError();
        ExitProcess(ExitCode);
    };
}

char* CommandAppend(char* Source)
{
    HRESULT Result = StringCbCatA(Commands, WORD_BUFFER_SIZE, Source);

    if (Result != S_OK) { FAILED(Result); }
    else { SUCCEEDED(Result); }

    return Commands;
}

int main(void)
{
    StdOut = GetStdHandle(STD_OUTPUT_HANDLE | STD_ERROR_HANDLE);
    if (StdOut == INVALID_HANDLE_VALUE) return GetLastError();

    DWORD ConsoleMode = 0;
    if (!GetConsoleMode(StdOut, &ConsoleMode)) return GetLastError();

    const char* Args = GetCommandLineA();
    const char ArgsLength = lstrlenA(Args);
    
    for (char i = ArgsLength; i >= 0; i--)
    {
        const char Arg = Args[i];
        
		if (Args[ArgsLength - 3] == 'e') break;
        if (Arg == 'h') ShowHelp      = 1;
        if (Arg == 'n') ComplerStatus = 0;
        if (Arg == 'l') LinkerStatus  = 1;
        if (Arg == 'r') RunningStatus = 1;
		if (Arg == ' ') break;
    }
    
    if (Args == "--help" || ShowHelp) {
    	PrintMessage("\n`run.c` a C program that runs C program.\n");
    	PrintMessage("\nUsage: run.c [options]...\n");
    	PrintMessage("\nOptions\n");
    	PrintMessage("    - `h` or `--hlep`: To show help.\n");
    	PrintMessage("    - `n`: No compling.\n");
    	PrintMessage("    - `l`: Linking the object files.\n");
    	PrintMessage("    - `r`: Running the program.\n\n");
    }
    
    if (ComplerStatus) {
        StringCbPrintfA(
        	Commands, 
        	WORD_BUFFER_SIZE, 
        	Compiler.ComplerArgs, 
        	FILE_LOCATION, FILE_LOCATION
        );
        CommandAppend(Compiler.NoCrtComplerArgs);
        CommandAppend(Compiler.FileName);
    
        if (LinkerStatus) {
            CommandAppend(Compiler.LinkerArgs);
            CommandAppend(Compiler.NoCrtLinkerArgs);
        } else {
            CommandAppend(Compiler.DisableLinkerArgs);
        }
        
        PrintMessage(Commands);
        PrintMessage("\n\n");
		WinExec(Commands, 0);
    } else if(LinkerStatus) {
        PrintMessage("❌ Can't link without compiling.\n");
    }
    
    
    if (RunningStatus) {
        if (!ComplerStatus) PrintMessage("⚠️  Only Running.\n");
        else if (!LinkerStatus) PrintMessage("⚠️  Running without Linking.\n");
        
        char RunCommand[10];
        StringCbPrintfA(
        	RunCommand, 
        	sizeof(FILE_LOCATION) + sizeof(RunCommand) + sizeof(" .exe"), 
        	"%s.exe", 
        	FILE_LOCATION
        );
        PrintMessage(RunCommand);
        PrintMessage("\n\n");
        WinExec(RunCommand, 0);
    }

    return 0;
}