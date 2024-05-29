#include "plot.hpp"
#include <iostream>

namespace plt
{
    Plot::Plot(const std::string &scriptName,
               const std::string &functionName,
               const std::vector<double> &xData,
               const std::vector<double> &yData,
               const std::unordered_map<std::string, std::string> &parameters)
        : _scriptName(scriptName),
          _functionName(functionName),
          _xData(xData),
          _yData(yData),
          _parameters(parameters)
    {

    }

    Plot::~Plot()
    {

    }

    void Plot::setParameters(const std::unordered_map<std::string, std::string> parameters)
    {
        for (const auto &[name, value] : parameters)
        {
            if (_parameters.find(name) == _parameters.end())
            {
                std::cout << "Unknown parameter \"" << name << "\"\n"
                          << std::endl;
                continue;
            }
            _parameters[name] = value;
        }
    }

    bool Plot::_execute(PyObject *pArgs)
    {
        // Get a reference to the function
        PyObject* scriptName = PyUnicode_FromString(_scriptName.c_str());
        PyObject* scriptModule = PyImport_Import(scriptName);
        Py_DECREF(scriptName);

        PyObject* pFunction = PyObject_GetAttrString(scriptModule, _functionName.c_str());
        PyObject* result = PyObject_CallObject(pFunction, pArgs);

        // Clean up
        Py_DECREF(pArgs);
        Py_DECREF(pFunction);
        Py_DECREF(scriptModule);

        if (!result) {
            PyErr_Print();
            return false;
        }
        Py_DECREF(result);
        return true;
    }
}
