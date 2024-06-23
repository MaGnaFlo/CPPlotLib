#include "figure.hpp"
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

    bool Figure::_initializeInterpreter() const
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
        return true;
    }

    bool Figure::build()
    {
        // initialize interpreter

        if (!_initializeInterpreter())
            return false;
        // automatic dpi adjustement. careful not to throw in some primes
        while (_width % _dpi != 0 || _height % _dpi != 0)
            _dpi--;

        // imports and figure (imports are greedy : ~400 ms)
        std::ostringstream oss;
        oss << "import matplotlib.pyplot as plt\n"
            << "import numpy as np\n"
            << "fig, ax = plt.subplots(figsize=("
            << _width / _dpi << ", "
            << _height / _dpi << "), dpi=" << _dpi << ")\n";
        PyRun_SimpleString(oss.str().c_str());

        // execute all plots
        for (const auto &plot : _plots)
            plot->execute();

        // retrieve image
        oss.str("");
        oss << "ax.grid(" << static_cast<int>(_grid) << ")\n"
            << "ax.set_xlabel(\"" << _xlabel << "\")\n"
            << "ax.set_ylabel(\"" << _ylabel << "\")\n"
            << "ax.set_title(\"" << _title << "\")\n"
            << "fig.set_dpi(" << _dpi << ")\n"
            << "fig.canvas.draw()\n"
            << "img = np.frombuffer(fig.canvas.renderer.buffer_rgba(), dtype='uint8').reshape(" << _width << "," << _height << ", 4)\n";
        PyRun_SimpleString(oss.str().c_str());
        PyObject *mainModule = PyImport_AddModule("__main__");
        PyObject *mainDict = PyModule_GetDict(mainModule);
        PyObject *pData = PyDict_GetItemString(mainDict, "img");

        // build the image
        bool rc = _buildImage(pData);

        // close interpreter
        Py_Finalize();
        return rc;
    }

    void Figure::_setPixel(int i, int r, int g, int b)
    {
        _imageData[i] = r;
        _imageData[i + 1] = g;
        _imageData[i + 2] = b;
    }

    bool Figure::_buildImage(PyObject *pData)
    {
        bool rc{true};
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