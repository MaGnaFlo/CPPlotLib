#include "lineplot.hpp"
#include <iostream>
#include <filesystem>
#include <sstream>

namespace plt
{
    LinePlot::LinePlot(const std::vector<double> &xData,
                       const std::vector<double> &yData,
                       const std::unordered_map<std::string, std::string> &parameters)
        : Plot("script", "lineplot", xData, yData, parameters)
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

    bool LinePlot::execute(PyObject* ax)
    {
        PyObject* x = PyList_New(_xData.size());
        PyObject* y = PyList_New(_yData.size());
        for (size_t i = 0; i < _xData.size(); ++i) {
            PyList_SetItem(x, i, PyFloat_FromDouble(_xData[i]));
        }
        for (size_t i = 0; i < _yData.size(); ++i) {
            PyList_SetItem(y, i, PyFloat_FromDouble(_yData[i]));
        }

        PyObject* pLinecolor = PyUnicode_FromString(_parameters["linecolor"].c_str());
        PyObject* pLinestyle = PyUnicode_FromString(_parameters["linestyle"].c_str());
        PyObject* pLinewidth = PyLong_FromLong(std::stoi(_parameters["linewidth"]));
        PyObject* pArgs = PyTuple_Pack(6, ax, x, y, pLinecolor, pLinewidth, pLinestyle);

        Py_DECREF(x);
        Py_DECREF(y);
        Py_DECREF(pLinecolor);
        Py_DECREF(pLinestyle);
        Py_DECREF(pLinewidth);

        return _execute(pArgs);
    }
}
