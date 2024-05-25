#pragma once

#include <Python.h>
#include <vector>
#include <unordered_map>
#include "plot.hpp"

namespace plt
{

    class LinePlot : public Plot
    {
    public:
        LinePlot();
        virtual ~LinePlot();
        virtual bool execute();
    };
}