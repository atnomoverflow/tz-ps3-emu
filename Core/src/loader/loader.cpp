#include <loader/loader.hpp>
FileType getFileType(const std::string &path)
{

    vfsLocalFile file(path);
    be_t<u32> magic;

    file.Read(&magic, sizeof(magic));

    switch (magic)
    {
    case 'SCE\0':
        return FILETYPE_SELF;
    case '\x7f\x45\x4c\x46':
        return FILETYPE_ELF;
    default:
        return FILETYPE_UNKOWN;
    }
}

std::optional<std::unique_ptr<BaseLoader>> loaderFactory(const std::string &path)
{
    switch (getFileType(path))
    {
    case FILETYPE_ELF:
        return std::make_unique<ElfLoader>(path);
    case FILETYPE_SELF:
        // TODO: return SELF loader
        return {};
    case FILETYPE_UNKOWN:
        return {};
    }
}