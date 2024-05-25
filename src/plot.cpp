#include "plot.hpp"
#include <filesystem>
#include <sstream>
#include <iostream>

namespace plt
{
    Plot::Plot(Figure& figure, const std::string &scriptName, const std::string &functionName)
        : _scriptName(scriptName), _functionName(functionName), _figure(figure)
    {
        _status = true;
        // initialize interpreter
        Py_Initialize();

        // append the directory containing modules (including numpy and matplotlib) to the Python path
        PyObject *sysPath = PySys_GetObject("path");
        if (!sysPath)
        {
            std::cerr << "Failed to get sys.path\n";
            _status = false;
            return;
        }
        std::stringstream ss;
        ss << std::filesystem::current_path().string();
        ss << "/../python/lib/site-packages";
        const auto dirPath = ss.str();
        PyObject *modulesPath = PyUnicode_FromString(dirPath.c_str());
        if (!modulesPath)
        {
            std::cerr << "Failed to create modules path\n";
            _status = false;
            return;
        }
        int result = PyList_Append(sysPath, modulesPath);
        Py_DECREF(modulesPath);
        if (result == -1)
        {
            std::cerr << "Failed to append modules path to sys.path\n";
            _status = false;
            return;
        }

        // append the directory containing the scripts
        const std::string scripts_path{"../scripts/"};
        PyObject *scriptsPath{PyUnicode_FromString(scripts_path.c_str())};
        if (!scriptsPath)
        {
            std::cerr << "Failed to create scripts path\n";
            _status = false;
            return;
        }
        result = PyList_Append(sysPath, scriptsPath);
        Py_DECREF(scriptsPath);
        if (result == -1)
        {
            std::cerr << "Failed to append modules path to sys.path\n";
            _status = false;
            return;
        }
    }

    Plot::~Plot()
    {
        // close interpreter
        if (Py_FinalizeEx() < 0)
        {
            std::cerr << "Failed to shutdown the Python interpreter.\n";
        }
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
        // load module
        PyObject *pName = PyUnicode_DecodeFSDefault(_scriptName.c_str());
        if (!pName)
        {
            std::cerr << "Could not decode script file name.\n";
            return false;
        }
        PyObject *pScript = PyImport_Import(pName);
        Py_DECREF(pName);
        if (!pScript)
        {
            std::cerr << "Failed to load script.\n";
            return false;
        }
        // load function
        PyObject *pFunction = PyObject_GetAttrString(pScript, _functionName.c_str());
        // the function has to exist and be callable
        if (!pFunction || !PyCallable_Check(pFunction))
        {
            if (PyErr_Occurred())
                PyErr_Print();
            std::cerr << "Could not load function.\n";
            return false;
        }

        return _buildFigure(pFunction, pArgs);
    }

    bool Plot::_buildFigure(PyObject *pFunction, PyObject *pArgs)
    {
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

        bool rc{true};
        PyObject *pResult = PyObject_CallObject(pFunction, pArgs);
        Py_DECREF(pArgs);
        if (pResult && PySequence_Check(pResult))
        {
            if (!PySequence_GetItem(pResult, 0))
            {
                std::cerr << "The returned sequence is empty.\n";
                rc = false;
            }
            else
            {
                for (int i = 0; i < _figure.height(); i++)
                {
                    PyObject *pRow = PySequence_GetItem(pResult, i);
                    if (pRow)
                    {
                        for (int j = 0; j < _figure.width(); j++)
                        {
                            PyObject *pPixel = PySequence_GetItem(pRow, j);
                            if (pPixel)
                            {
                                PyObject *pR = PySequence_GetItem(pPixel, 0);
                                PyObject *pG = PySequence_GetItem(pPixel, 1);
                                PyObject *pB = PySequence_GetItem(pPixel, 2);
                                if (pR && pG && pB)
                                {
                                    int index{i * _figure.width() * 3 + j * 3};
                                    int r = static_cast<unsigned char>(PyLong_AsLong(pR));
                                    int g = static_cast<unsigned char>(PyLong_AsLong(pG));
                                    int b = static_cast<unsigned char>(PyLong_AsLong(pB));
                                    _figure.setPixel(index, r, g, b);
                                }
                                Py_XDECREF(pR);
                                Py_XDECREF(pG);
                                Py_XDECREF(pB);
                                Py_DECREF(pPixel);
                            }
                        }
                        Py_DECREF(pRow);
                    }
                }
                Py_DECREF(pResult);
            }
        }
        else
        {
            PyErr_Print();
            std::cerr << "Call failed.\n";
            rc = false;
        }
        Py_XDECREF(pFunction);
        return rc;
    }
}
