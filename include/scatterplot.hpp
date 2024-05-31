#pragma once

#include "plot.hpp"

namespace plt
{
    class ScatterPlot : public Plot
    {
    public:
        ScatterPlot(const std::vector<double> &xData,
                 const std::vector<double> &yData,
                 const std::unordered_map<std::string, std::string> &parameters);
        virtual ~ScatterPlot() = default;
        ScatterPlot(ScatterPlot&) = delete;
        ScatterPlot& operator=(ScatterPlot&) = delete;
        ScatterPlot(ScatterPlot&&) = delete;
        ScatterPlot& operator=(ScatterPlot&&) = delete;
        virtual void execute();
    };
}