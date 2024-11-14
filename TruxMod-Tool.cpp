#include "stdafx.h"

#include "Utility.h"

#include "loaders/WADHLoader.h"


int wmain(int argc, wchar_t** argv)
{
    // Set console code page to UTF-8
    SetConsoleOutputCP(CP_UTF8);


    if (argc < 2)
    {
        TM::Log("Insufficient arguments. Please supply a command (e.g. \"unpack\").\n");
        return 0;
    }

    if (wcscmp(argv[1], L"unpack") == 0)
    {
        if (argc < 4)
        {
            TM::Log("Insufficient arguments. Usage: \"truxmod_tool.py unpack[wad file][output directory]\".\n");
            return 0;
        }

        TM::WADHLoader loader(argv[2]);
        loader.Unpack(argv[3]);
    }
    else
    {
        TM::Log("Unknown command. Please check your spelling.");
        return 0;
    }

    return 0;
}