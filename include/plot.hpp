#pragma once

#include <Python.h>
#include <vector>
#include <unordered_map>
#include "figure.hpp"

namespace plt
{

    class Plot
    {
    public:
        ~Plot();

        bool status() const { return _status; }
        void setXData(const std::vector<double> &xData) { _xData = xData; }
        void setYData(const std::vector<double> &yData) { _yData = yData; }
        void setParameters(const std::unordered_map<std::string, std::string> parameters);

        virtual bool execute() = 0;

    protected:
        Plot(Figure& figure, const std::string &scriptName, const std::string &functionName);
        bool _execute(PyObject *pArgs);
        bool _status;
        const std::string _scriptName;
        const std::string _functionName;
        std::vector<double> _xData;
        std::vector<double> _yData;
        std::unordered_map<std::string, std::string> _parameters;
        Figure& _figure;

    private:
        bool _buildFigure(PyObject *pFunction, PyObject *pArgs);
    };
}