#include "emulator.hpp"
#include "loader/loader.hpp"
#include <iostream>
bool Emulator::load(const std::string &path)
{

    // init memory
    memory.init();

    // get a loader for the file
    auto loader_optional = loaderFactory(path);
    if (!loader_optional.has_value())
    {
        // failed to get loader invalid type of file
        std::cerr << "the file type or invalid path";
        return false;
    }

    auto loader = loader_optional.value();

    if (loader.get()->open(path))
    {
        // failed to get loader invalid type of file
        std::cerr << "error while reading file";
        return false;
    }
    loader.get()->load()

    // ppu and spu thread ???

    return true;
}
void run();
void pause();
void stop();