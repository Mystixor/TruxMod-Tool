#include "stdafx.h"

#include "WADHLoader.h"

#include "Utility.h"

namespace TM
{
	WADHLoader::WADHLoader(const std::filesystem::path& filePath)
        :
        m_WADHPath(filePath),
        m_WADHSize(TM::GetFileSize(m_WADHPath)),
        m_Data(nullptr),
        m_Header(WADH_HEADER()), m_FileNameArrayOffset(0),
        m_OutputDir(L""), m_ExtractCount(0U), m_OOBCount(0U)
	{
	}

	WADHLoader::~WADHLoader()
	{
	}

	void WADHLoader::Unpack(const wchar_t* outputDir)
	{
        Log(L"Attempting extraction of archive \"{}\" ot directory \"{}\".", m_WADHPath.c_str(), outputDir);

        m_OutputDir = outputDir;

        unsigned int extractCount = 0;

        std::ifstream file(m_WADHPath, std::ios::binary);

        if (m_WADHSize >= sizeof(WADH_HEADER) && file)
        {
            m_Data = new char[m_WADHSize];
            file.read(m_Data, m_WADHSize);
            file.close();

            m_Header = *(WADH_HEADER*)(m_Data + 0x0);

            if (m_Header.magic == 'HDAW')
            {
                m_FileNameArrayOffset = sizeof(WADH_HEADER) + m_Header.fileCount * sizeof(WADH_FILE);
                HandleFile("", 0);
            }
            else
            {
                Log("Supplied file is not a valid WADH file.");
            }

            Log("Extracted {} files.", m_ExtractCount);

            if (m_OOBCount > 0)
            {
                Log("\nThere {} {} {} referenced in the archive which {} to not be stored inside the archive. The purpose is unknown, it might be a relict from development.",
                    m_OOBCount == 1 ? "was" : "were", m_OOBCount, m_OOBCount == 1 ? "file" : "files", m_OOBCount == 1 ? "seems" : "seem");
            }

            delete[] m_Data;
        }
	}

	unsigned int WADHLoader::HandleFile(const std::filesystem::path& path, uint32_t index)
	{
        uint32_t offset = sizeof(WADH_HEADER) + index * sizeof(WADH_FILE);

        WADH_FILE file = *(WADH_FILE*)(m_Data + offset);

        std::string name = m_Data + m_FileNameArrayOffset + file.nameOffset;

        std::filesystem::path filePath = path / name;

        if (file.fileSize > 0)
        {
            std::ofstream fileStream(m_OutputDir / filePath, std::ios::binary);
            if (fileStream)
            {
                if (m_Header.fileDataArrayOffset + file.dataOffset + file.fileSize <= m_WADHSize)
                {
                    fileStream.write(m_Data + m_Header.fileDataArrayOffset + file.dataOffset, file.fileSize);
                    m_ExtractCount++;
                }
                else
                {
                    m_OOBCount++;
                }
                fileStream.close();
            }
        }

        std::ofstream metaStream(m_OutputDir / "meta" / std::format("{}_{}", index, name), std::ios::binary);
        if (metaStream)
        {
            metaStream.write((char*)&file, sizeof(WADH_FILE));
            metaStream.close();
        }

        if (file.nextSiblingIdx != -1)
        {
            HandleFile(path, file.nextSiblingIdx);
        }

        if (file.firstChildIdx != -1)
        {
            std::filesystem::create_directory(m_OutputDir / filePath);
            std::filesystem::create_directory(m_OutputDir / "meta" / path);
            HandleFile(filePath, file.firstChildIdx);
        }

        return 0;
	}
}