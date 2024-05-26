#include "lineplot.hpp"
#include <iostream>
#include <filesystem>
#include <sstream>

namespace plt
{
    LinePlot::LinePlot(int imgWidth,
                       int imgHeight,
                       int imgDpi,
                       const std::vector<double> &xData,
                       const std::vector<double> &yData,
                       const std::unordered_map<std::string, std::string> &parameters)
        : Plot("script", "plot", imgWidth, imgHeight, imgDpi, xData, yData, parameters)
    {
        // default parameters
        const std::unordered_map<std::string, std::string> defaultParameters = {{"linecolor", "r"},
                                                                                {"linewidth", "1"},
                                                                                {"linestyle", "-"},
                                                                                {"pointcolor", "r"},
                                                                                {"pointsize", "0"},
                                                                                {"grid", "true"},
                                                                                {"title", ""},
                                                                                {"xlabel", ""},
                                                                                {"ylabel", ""}};
        _parameters.insert(defaultParameters.begin(), defaultParameters.end());
    }

    LinePlot::~LinePlot()
    {
    }

    bool LinePlot::execute()
    {
        // format data
        PyObject *x_ = PyList_New(_xData.size());
        for (int i = 0; i < _xData.size(); ++i)
        {
            PyList_SetItem(x_, i, PyFloat_FromDouble(_xData[i]));
        }
        PyObject *y_ = PyList_New(_yData.size());
        for (int i = 0; i < _yData.size(); ++i)
        {
            PyList_SetItem(y_, i, PyFloat_FromDouble(_yData[i]));
        }
        // parameters
        int dpi{_imgData.dpi};
        while (_imgData.width % dpi != 0 || _imgData.height % dpi != 0)
        {
            dpi--; // automatic dpi adjustement. careful not to throw in some primes
        }
        PyObject *dpi_ = PyUnicode_FromString(std::to_string(dpi).c_str());
        PyObject *figwidth_ = PyUnicode_FromString(std::to_string(_imgData.width / dpi).c_str());
        PyObject *figheight_ = PyUnicode_FromString(std::to_string(_imgData.height / dpi).c_str());
        PyObject *linecolor_ = PyUnicode_FromString(_parameters.at("linecolor").c_str());
        PyObject *linewidth_ = PyUnicode_FromString(_parameters.at("linewidth").c_str());
        PyObject *linestyle_ = PyUnicode_FromString(_parameters.at("linestyle").c_str());
        PyObject *pointcolor_ = PyUnicode_FromString(_parameters.at("pointcolor").c_str());
        PyObject *pointsize_ = PyUnicode_FromString(_parameters.at("pointsize").c_str());
        PyObject *grid_ = PyUnicode_FromString(_parameters.at("grid").c_str());
        PyObject *title_ = PyUnicode_FromString(_parameters.at("title").c_str());
        PyObject *xlabel_ = PyUnicode_FromString(_parameters.at("xlabel").c_str());
        PyObject *ylabel_ = PyUnicode_FromString(_parameters.at("ylabel").c_str());
        PyObject *pArgs = Py_BuildValue("(OOOOOOOOOOOOOO)", x_,
                                        y_,
                                        dpi_,
                                        figwidth_,
                                        figheight_,
                                        linecolor_,
                                        linewidth_,
                                        linestyle_,
                                        pointcolor_,
                                        pointsize_,
                                        grid_,
                                        title_,
                                        xlabel_,
                                        ylabel_);

        Py_DECREF(x_);
        Py_DECREF(y_);
        return _execute(pArgs);
    }
}
