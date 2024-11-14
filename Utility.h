#pragma once

#include "stdafx.h"

namespace TM
{
    //	Logs a text encoded as UTF-8 to the console.
    static void Log(const std::string& text)
    {
        std::cout << text << std::endl;
    }

    //	Logs a text encoded as UTF-8 to the console. Text will be formatted in the same way as with std::format.
    template <class... _Types>
    _NODISCARD static void Log(const std::string_view formatText, _Types&&... _Args)
    {
        std::string text = std::vformat(formatText, std::make_format_args(_Args...));

        Log(text);
    }

    //	Logs a wide character text encoded as UTF-8 to the console.
    static void Log(const std::wstring& text)
    {
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, NULL, 0, NULL, NULL);
        char* buffer = new char[bufferSize];
        WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, buffer, bufferSize, NULL, NULL);
        Log(buffer);
        delete[] buffer;
    }

    //	Logs a wide character text encoded as UTF-8 to the console. Text will be formatted in the same way as with std::format.
    template <class... _Types>
    _NODISCARD static void Log(const std::wstring_view formatText, _Types&&... _Args)
    {
        std::wstring text = std::vformat(formatText, std::make_wformat_args(_Args...));

        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, NULL, 0, NULL, NULL);
        char* buffer = new char[bufferSize];
        WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, buffer, bufferSize, NULL, NULL);
        Log(buffer);
        delete[] buffer;
    }

    //	Returns whether a file exists with the specified wide char string file name.
    inline bool IsFileExisting(const std::wstring& name)
    {
        FILE* file = nullptr;
        if (_wfopen_s(&file, name.c_str(), L"r") == 0) {
            if (file != nullptr)
            {
                fclose(file);
                return true;
            }
            return false;
        }
        else {
            return false;
        }
    }

    //	Returns the size of a file located at the specified wide char string path.
    inline std::uintmax_t GetFileSize(const std::wstring& name)
    {
        if (IsFileExisting(name))
        {
            return std::filesystem::file_size(name);
        }
        return 0ULL;
    }
}