#pragma once

#include <Python.h>
#include <vector>
#include <unordered_map>

namespace plt
{
    enum class PlotType
    {
        LINE,
        SCATTER,
        BAR
    };

    class Plot
    {
    public:
        virtual ~Plot() = default;
        void setXData(const std::vector<double> &xData) { _xData = xData; }
        void setYData(const std::vector<double> &yData) { _yData = yData; }
        virtual void execute() = 0;

    protected:
        Plot(const std::vector<double> &xData,
             const std::vector<double> &yData,
             const std::unordered_map<std::string, std::string> &parameters);
        void _setParameters(const std::unordered_map<std::string, std::string> parameters);

        std::vector<std::string> _buildArgs() const;
        std::vector<double> _xData;
        std::vector<double> _yData;
        std::unordered_map<std::string, std::string> _parameters;

    };
}