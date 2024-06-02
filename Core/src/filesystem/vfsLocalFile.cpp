#include <iostream>
#include <filesystem/vfsLocalFile.hpp>

static const char *getOpenMode(vfsOpenMode mode)
{
    switch (mode)
    {
    case vfsRead:
        return "rb";
    case vfsWrite:
        return "wb";
    case vfsReadWrite:
        return "r+b";
    default:
        return "r";
    }
}
static const int getSeekMode(vfsSeekMode mode)
{
    switch (mode)
    {
    case vfsSeekSet:
        return SEEK_SET;
    case vfsSeekCur:
        return SEEK_CUR;
    case vfsSeekEnd:
        return SEEK_END;
    }

    return SEEK_SET;
}

vfsLocalFile::vfsLocalFile()
{
    m_file = nullptr;
}

vfsLocalFile::vfsLocalFile(const std::string &path, vfsOpenMode mode )
{
    Open(path, mode);
}

vfsLocalFile::~vfsLocalFile()
{
    Close();
}
bool vfsLocalFile::IsOpen() const
{
    return m_file != nullptr;
}

bool vfsLocalFile::Close()
{
    if (!IsOpen() || std::fclose(m_file) != 0)
    {
        m_file = nullptr;
        return false;
    }
    return true;
}

u64 vfsLocalFile::Tell() const
{
    if (!IsOpen())
        return -1;
#ifdef TZ_PS3_EMU_WIN
    return _ftelli64(m_file);
#else
    return ftello64(m_file);
#endif
}

bool vfsLocalFile::Seek(s64 offset, vfsSeekMode mode)
{
#ifdef TZ_PS3_EMU_WIN
    int pos = _fseeki64(m_file, offset, getSeekMode(mode));
#else
    int pos = fseeko64(m_file, offset, getSeekMode(mode));
#endif
    if (!IsOpen() || pos != 0)
    {
        return false;
    }
    return true;
}

u64 vfsLocalFile::GetSize()
{
    u64 size = Tell();
    if (!Seek(0, vfsSeekEnd))
    {
        std::cerr << "vfsLocalFile::GetSize: failed to seek" << std::endl;
        return 0;
    }
    return size;
}

bool vfsLocalFile::Read(void *dst, u64 size)
{
    if (!IsOpen() || std::fread(dst, 1, size, m_file) != size)
        return false;
    return true;
}

bool vfsLocalFile::Write(const void *src, u64 size)
{
    if (!IsOpen() || std::fwrite(src, 1, size, m_file) != size)
        return false;
    return true;
}

bool vfsLocalFile::Open(const std::string &path, vfsOpenMode mode)
{
    Close();
#if defined(_WIN32) && defined(UNICODE)
    _wfopen_s(&m_file, ConvertUTF8ToWString(path).c_str(), ConvertUTF8ToWString(getOpenMode(mode)).c_str());
#elif defined(_WIN32)
    fopen_s(&m_file, path.c_str(), getOpenMode(mode));
#else
    m_file = fopen(path.c_str(), getOpenMode(mode));
#endif
    return IsOpen();
}
