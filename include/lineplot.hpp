#pragma once

#include "plot.hpp"

namespace plt
{
    class LinePlot : public Plot
    {
    public:
        LinePlot(const std::vector<double> &xData,
                 const std::vector<double> &yData,
                 const std::unordered_map<std::string, std::string> &parameters);
        virtual ~LinePlot() = default;
        LinePlot(LinePlot&) = delete;
        LinePlot& operator=(LinePlot&) = delete;
        LinePlot(LinePlot&&) = delete;
        LinePlot& operator=(LinePlot&&) = delete;
        virtual void execute();
    };
}