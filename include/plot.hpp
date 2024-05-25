#pragma once

#include <Python.h>
#include <vector>
#include <unordered_map>

namespace plt
{
    struct ImageData
    {
        void resize() { data.resize(3 * width * height); }
        std::vector<unsigned char> data;
        int width = 0;
        int height = 0;
    };

    class Plot
    {
    public:
        ~Plot();

        bool status() const { return _status; }
        void setXData(const std::vector<double> &xData) { _xData = xData; }
        void setYData(const std::vector<double> &yData) { _yData = yData; }

        void setParameters(const std::unordered_map<std::string, std::string> parameters);

        const ImageData &image() const { return _plotImage; }

        virtual bool execute() = 0;

    protected:
        Plot(const std::string &scriptName, const std::string &functionName);
        bool _execute(PyObject *pArgs);
        bool _status;
        const std::string _scriptName;
        const std::string _functionName;
        std::vector<double> _xData;
        std::vector<double> _yData;
        std::unordered_map<std::string, std::string> _parameters;
        ImageData _plotImage;

    private:
        bool _buildFigure(PyObject *pFunction, PyObject *pArgs);
    };
}