#include "figure.hpp"
#include "lineplot.hpp"
#include <iostream>
#include <sstream>
#include <filesystem>

namespace plt
{

    Figure::Figure(int width, int height, int dpi)
    {
        _width = width;
        _height = height;
        _dpi = dpi;
        _imageData.resize(3 * width * height);
    }

    void Figure::addPlot(PlotType type,
                         const std::vector<double> &xData,
                         const std::vector<double> &yData,
                         const std::unordered_map<std::string, std::string> &parameters)
    {
        switch (type)
        {
        case PlotType::LINE:
            _plots.push_back(std::make_unique<LinePlot>(xData, yData, parameters));
            break;

        case PlotType::SCATTER:
            break;

        case PlotType::BAR:
            break;
        }
    }

    bool Figure::_initializeInterpreter()
    {
        // initialize interpreter
        Py_Initialize();

        // append the directory containing modules (including numpy and matplotlib) to the Python path
        PyObject *sysPath = PySys_GetObject("path");
        if (!sysPath)
        {
            std::cerr << "Failed to get sys.path\n";
            return false;
        }
        std::stringstream ss;
        ss << std::filesystem::current_path().string();
        ss << "/../python/lib/site-packages";
        const auto dirPath = ss.str();
        PyObject *modulesPath = PyUnicode_FromString(dirPath.c_str());
        if (!modulesPath)
        {
            std::cerr << "Failed to create modules path\n";
            return false;
        }
        int result = PyList_Append(sysPath, modulesPath);
        Py_DECREF(modulesPath);
        if (result == -1)
        {
            std::cerr << "Failed to append modules path to sys.path\n";
            return false;
        }

        // append the directory containing the scripts
        const std::string scripts_path{"../scripts/"};
        PyObject *scriptsPath{PyUnicode_FromString(scripts_path.c_str())};
        if (!scriptsPath)
        {
            std::cerr << "Failed to create scripts path\n";
            return false;
        }
        result = PyList_Append(sysPath, scriptsPath);
        Py_DECREF(scriptsPath);
        if (result == -1)
        {
            std::cerr << "Failed to append modules path to sys.path\n";
            return false;
        }
        return true;
    }

    bool Figure::build()
    {
        if (!_initializeInterpreter())
        {
            return false;
        }

        while (_width % _dpi != 0 || _height % _dpi != 0)
        {
            _dpi--; // automatic dpi adjustement. careful not to throw in some primes
        }

        PyObject* mainModule = PyImport_AddModule("__main__");
        PyObject* mainDict = PyModule_GetDict(mainModule);

        std::ostringstream oss;
        oss << "import matplotlib.pyplot as plt\n"
            << "import numpy as np\n"
            << "fig, ax = plt.subplots(figsize=(" 
            << _width/_dpi << ", " 
            << _height/_dpi << "), dpi=" << _dpi << ")\n";
        PyRun_SimpleString(oss.str().c_str());

        
        PyObject* ax = PyDict_GetItemString(mainDict, "ax");

        bool rc {true};
        for (const auto &plot : _plots)
        {
            rc = rc && plot->execute(ax);
        }

        oss.str("");
        oss << "fig.set_dpi(" << _dpi << ")\n"
            << "fig.canvas.draw()\n"
            << "img = np.frombuffer(fig.canvas.renderer.buffer_rgba(), dtype='uint8').reshape(" << _width << "," << _height << ", 4)\n";

        PyRun_SimpleString(oss.str().c_str());
        
        PyObject* pData = PyDict_GetItemString(mainDict, "img");
        
        rc = rc && _buildImage(pData);
        
        Py_Finalize();

        return rc;
    }

    bool Figure::_buildImage(PyObject* pData)
    {
        bool rc {true};
        if (pData && PySequence_Check(pData))
        {
            if (!PySequence_GetItem(pData, 0))
            {
                std::cerr << "The returned sequence is empty.\n";
                rc = false;
            }
            else
            {
                for (int i = 0; i < _width; i++)
                {
                    PyObject *pRow = PySequence_GetItem(pData, i);
                    if (pRow)
                    {
                        for (int j = 0; j < _height; j++)
                        {
                            PyObject *pPixel = PySequence_GetItem(pRow, j);
                            if (pPixel)
                            {
                                PyObject *pR = PySequence_GetItem(pPixel, 0);
                                PyObject *pG = PySequence_GetItem(pPixel, 1);
                                PyObject *pB = PySequence_GetItem(pPixel, 2);
                                if (pR && pG && pB)
                                {
                                    int index{i * _height * 3 + j * 3};
                                    int r = static_cast<unsigned char>(PyLong_AsLong(pR));
                                    int g = static_cast<unsigned char>(PyLong_AsLong(pG));
                                    int b = static_cast<unsigned char>(PyLong_AsLong(pB));
                                    _setPixel(index, r, g, b);
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
                Py_DECREF(pData);
            }
        }
        return rc;
    }
}