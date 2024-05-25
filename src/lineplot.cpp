#include "lineplot.hpp"
#include <iostream>
#include <filesystem>
#include <sstream>

namespace plt
{
    LinePlot::LinePlot() : Plot("script", "plot")
    {
        // default parameters
        _parameters = {{"dpi", "50"},
                       {"linecolor", "r"},
                       {"linewidth", "1"},
                       {"linestyle", "-"},
                       {"pointcolor", "r"},
                       {"pointsize", "0"},
                       {"figwidth", "15"},
                       {"figheight", "7"},
                       {"grid", "true"},
                       {"title", ""},
                       {"xlabel", ""},
                       {"ylabel", ""}};
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
        PyObject *dpi_ = PyUnicode_FromString(_parameters["dpi"].c_str());
        PyObject *figwidth_ = PyUnicode_FromString(_parameters["figwidth"].c_str());
        PyObject *figheight_ = PyUnicode_FromString(_parameters["figheight"].c_str());
        PyObject *linecolor_ = PyUnicode_FromString(_parameters["linecolor"].c_str());
        PyObject *linewidth_ = PyUnicode_FromString(_parameters["linewidth"].c_str());
        PyObject *linestyle_ = PyUnicode_FromString(_parameters["linestyle"].c_str());
        PyObject *pointcolor_ = PyUnicode_FromString(_parameters["pointcolor"].c_str());
        PyObject *pointsize_ = PyUnicode_FromString(_parameters["pointsize"].c_str());
        PyObject *grid_ = PyUnicode_FromString(_parameters["grid"].c_str());
        PyObject *title_ = PyUnicode_FromString(_parameters["title"].c_str());
        PyObject *xlabel_ = PyUnicode_FromString(_parameters["xlabel"].c_str());
        PyObject *ylabel_ = PyUnicode_FromString(_parameters["ylabel"].c_str());
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