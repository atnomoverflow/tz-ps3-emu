#pragma once


#include "cpu/decoder.hpp"



static UCodeField<0,31> instruction;
static UCodeField<0,5> op;

static UCodeField<30> aa;
static UCodeField<31> lk;

