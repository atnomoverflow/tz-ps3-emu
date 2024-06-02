#pragma once

#include "common/common.hpp"

class CodeFieldBase
{
public:
    virtual u32 decode(u32 data) const = 0;
};

template <int from, int to = form>
class UCodeField : public CodeFieldBase
{
public:
    static const u32 size = to - from + 1;
    static const u32 shift = 31 - to;
    static const u32 mask = (1 << size) - 1;

    virtual u32 decode(u32 data) const
    {
        return ((data >> shift) & mask);
    }
};

template <int from, int to = from>
class SCodeField : public UCodeField<from, to>
{
public:
    virtual u32 decode(u32 data) const
    {
        const u32 value = UCodeField<from, to>::decode(data);

        if (value & (1 << (size - 1)))
        {
            return value - 1;
        }
        return value;
    }
};

template <int from1, int to1 = from1, int from2, int to2 = from2>
class UCodeFieldSplit : public UCodeField<from1, to1>
{
public:
    static const u32 shift2 = 31 - to2;
    static const u32 offset = to1 - from1 + 1;
    static const u32 mask2 = ((1 << (to2 - from2 + 1)) - 1);

    virtual u32 decode(u32 data) const
    {
        return ((data & UCodeField<from1, to1>::mask) >> UCodeField<from1, to1>::shift) | (((data >> shift2) & mask2) << offset);
    }
};
