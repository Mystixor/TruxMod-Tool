#pragma once

#include <filesystem>

namespace TM
{
    struct WADH_HEADER
    {
        int32_t magic;
        int32_t fileDataArrayOffset;
        int32_t fileCount;
        int32_t fileNameArraySize;
    };

    struct WADH_FILE
    {
        int32_t nameOffset;
        int32_t dat_04;
        int32_t dataOffset;
        int32_t fileSize;
        int32_t fileSize1;
        int32_t dat_14;
        int32_t firstChildIdx;
        int32_t nextSiblingIdx;
    };

    class WADHLoader
    {
    public:
        WADHLoader(const std::filesystem::path& filePath);

        ~WADHLoader();

        void Unpack(const wchar_t* outputDir);

    private:
        unsigned int HandleFile(const std::filesystem::path& path, uint32_t index);

        const std::filesystem::path m_WADHPath;

        unsigned int m_WADHSize;

        char* m_Data;

        WADH_HEADER m_Header;

        int32_t m_FileNameArrayOffset;

        std::filesystem::path m_OutputDir;

        unsigned int m_ExtractCount;

        unsigned int m_OOBCount;
    };
}