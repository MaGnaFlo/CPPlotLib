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
        virtual ~Plot();

        void setXData(const std::vector<double> &xData) { _xData = xData; }
        void setYData(const std::vector<double> &yData) { _yData = yData; }
        void setParameters(const std::unordered_map<std::string, std::string> parameters);

        virtual bool execute(PyObject* ax) = 0;

    protected:
        Plot(const std::string &scriptName,
             const std::string &functionName,
             const std::vector<double> &xData,
             const std::vector<double> &yData,
             const std::unordered_map<std::string, std::string> &parameters);

        bool _execute(PyObject *pArgs);
        const std::string _scriptName;
        const std::string _functionName;
        std::vector<double> _xData;
        std::vector<double> _yData;
        std::unordered_map<std::string, std::string> _parameters;

    };
}