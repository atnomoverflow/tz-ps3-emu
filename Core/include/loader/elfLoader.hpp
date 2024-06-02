#pragma once

#include "baseLoader.hpp"
#include <cstring>
class ElfLoader : public BaseLoader
{
public:
    ElfLoader();
    ElfLoader(const std::string &path);
    ~ElfLoader();
    virtual bool open(const std::string &path) override;
    virtual void close() override;
    virtual bool load() override;

    u16 getMachine();
    u32 getEntry();
};
