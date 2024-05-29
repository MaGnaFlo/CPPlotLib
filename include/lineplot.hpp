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
        LinePlot(const std::vector<double> &xData,
                 const std::vector<double> &yData,
                 const std::unordered_map<std::string, std::string> &parameters);
        virtual ~LinePlot();
        virtual bool execute(PyObject* ax);
    };
}