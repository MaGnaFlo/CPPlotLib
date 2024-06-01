#pragma once

#include "plot.hpp"

namespace plt
{
    class BarPlot : public Plot
    {
    public:
        BarPlot(const std::vector<double> &xData,
                 const std::vector<double> &yData,
                 const std::unordered_map<std::string, std::string> &parameters);
        virtual ~BarPlot() = default;
        BarPlot(BarPlot&) = delete;
        BarPlot& operator=(BarPlot&) = delete;
        BarPlot(BarPlot&&) = delete;
        BarPlot& operator=(BarPlot&&) = delete;

        virtual void _setParameters(const std::unordered_map<std::string, std::string>& parameters) override;
        virtual void execute() override;
    };
}