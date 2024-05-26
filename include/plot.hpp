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

    struct ImageData
    {
        int width = 0;
        int height = 0;
        int dpi = 100;
        std::vector<unsigned char> data;
        void setPixel(int i, int r, int g, int b)
        {
            data[i] = r;
            data[i + 1] = g;
            data[i + 2] = b;
        }
    };

    class Plot
    {
    public:
        virtual ~Plot();

        bool status() const { return _status; }
        void setXData(const std::vector<double> &xData) { _xData = xData; }
        void setYData(const std::vector<double> &yData) { _yData = yData; }
        void setParameters(const std::unordered_map<std::string, std::string> parameters);
        const ImageData &imageData() const { return _imgData; }

        virtual bool execute() = 0;

    protected:
        Plot(const std::string &scriptName,
             const std::string &functionName,
             int imgWidth,
             int imgHeight,
             int imgDpi,
             const std::vector<double> &xData,
             const std::vector<double> &yData,
             const std::unordered_map<std::string, std::string> &parameters);

        bool _execute(PyObject *pArgs);
        bool _status;
        const std::string _scriptName;
        const std::string _functionName;
        std::vector<double> _xData;
        std::vector<double> _yData;
        std::unordered_map<std::string, std::string> _parameters;
        ImageData _imgData;

    private:
        bool _buildImage(PyObject *pFunction, PyObject *pArgs);
    };
}