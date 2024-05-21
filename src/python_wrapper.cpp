#include "python_wrapper.hpp"
#include <iostream>
#include <QDir>
#include <sstream>

PythonWrapper::PythonWrapper(const std::string& modulesRelativePath)
{
    _status = true;
    // initialize interpreter
    Py_Initialize();

    // append the directory containing modules (including numpy and matplotlib) to the Python path
    PyObject *sysPath = PySys_GetObject("path");
    if (!sysPath) {
        std::cerr << "Failed to get sys.path\n";
        _status = false;
        return;
    }
    std::stringstream ss;
    ss << QDir::currentPath().toStdString(); // < change to standard cpp
    if (!modulesRelativePath.empty() && modulesRelativePath.front() != '/') {
        ss << "/";
    }
    ss << modulesRelativePath;
    const auto dirPath = ss.str();
    PyObject* modulesPath = PyUnicode_FromString(dirPath.c_str());
    if (!modulesPath) {
        std::cerr << "Failed to create modules path\n";
        _status = false;
        return;
    }
    int result = PyList_Append(sysPath, modulesPath);
    Py_DECREF(modulesPath);
    if (result == -1) {
        std::cerr << "Failed to append modules path to sys.path\n";
        _status = false;
        return;
    }

    // append the directory containing the scripts
    const std::string scripts_path {"../scripts/"};
    PyObject* scriptsPath {PyUnicode_FromString(scripts_path.c_str())};
    if (!scriptsPath) {
        std::cerr << "Failed to create scripts path\n";
        _status = false;
        return;
    }
    result = PyList_Append(sysPath, scriptsPath);
    Py_DECREF(scriptsPath);
    if (result == -1) {
        std::cerr << "Failed to append modules path to sys.path\n";
        _status = false;
        return;
    }

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
                   {"ylabel", ""}
    };
}

PythonWrapper::~PythonWrapper()
{
    // close interpreter
    if (Py_FinalizeEx() < 0) {
        std::cerr << "Failed to shutdown the Python interpreter.\n";
    }
}

void PythonWrapper::setParameters(const std::unordered_map<std::string, std::string> parameters)
{
    for (const auto& [name,value]: parameters) {
        if (_parameters.find(name) == _parameters.end()) {
            std::cout << "Unknown parameter \"" << name << "\"\n" << std::endl;
            continue;
        }
        _parameters[name] = value;
    }
}

std::unique_ptr<QImage> PythonWrapper::execute()
{
    // declarations
    std::unique_ptr<QImage> img {nullptr};
    PyObject *pName, *pScript, *pFunc;
    PyObject *pArgs, *pValue;
    // load the module
    pName = PyUnicode_DecodeFSDefault(_scriptName.c_str());
    if (!pName){
        std::cerr << "Could not decode script file name.\n";
        return nullptr;
    }
    pScript = PyImport_Import(pName);
    Py_DECREF(pName);
    if (!pScript) {
       std::cerr << "Failed to load script.\n";
        return nullptr;
    }
    // load function
    pFunc = PyObject_GetAttrString(pScript, _functionName.c_str());
    // the function has to exist and be callable
    if (!pFunc || !PyCallable_Check(pFunc)) {
        if (PyErr_Occurred()) PyErr_Print();
        std::cerr << "Could not load function.\n";
        return nullptr;
    }
    // format data
    PyObject* x_ = PyList_New(_xData.size());
    for (int i = 0; i<_xData.size(); ++i) {
        PyList_SetItem(x_, i, PyFloat_FromDouble(_xData[i]));
    }
    PyObject* y_ = PyList_New(_yData.size());
    for (int i = 0; i<_yData.size(); ++i) {
        PyList_SetItem(y_, i, PyFloat_FromDouble(_yData[i]));
    }
    // parameters
    PyObject* dpi_ = PyUnicode_FromString(_parameters["dpi"].c_str());
    PyObject* figwidth_ = PyUnicode_FromString(_parameters["figwidth"].c_str());
    PyObject* figheight_ = PyUnicode_FromString(_parameters["figheight"].c_str());
    PyObject* linecolor_ = PyUnicode_FromString(_parameters["linecolor"].c_str());
    PyObject* linewidth_ = PyUnicode_FromString(_parameters["linewidth"].c_str());
    PyObject* linestyle_ = PyUnicode_FromString(_parameters["linestyle"].c_str());
    PyObject* pointcolor_ = PyUnicode_FromString(_parameters["pointcolor"].c_str());
    PyObject* pointsize_ = PyUnicode_FromString(_parameters["pointsize"].c_str());
    PyObject* grid_ = PyUnicode_FromString(_parameters["grid"].c_str());
    PyObject* title_ = PyUnicode_FromString(_parameters["title"].c_str());
    PyObject* xlabel_ = PyUnicode_FromString(_parameters["xlabel"].c_str());
    PyObject* ylabel_ = PyUnicode_FromString(_parameters["ylabel"].c_str());
    pArgs = Py_BuildValue("(OOOOOOOOOOOOOO)", x_, 
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

    // call
    pValue = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(x_);
    Py_DECREF(y_);
    Py_DECREF(pArgs);
    if (pValue && PySequence_Check(pValue)) {
        if (!PySequence_GetItem(pValue, 0)) {
            std::cerr << "The returned sequence is empty\n.";
            return nullptr;
        }
        // building image
        int height = PySequence_Size(pValue);
        int width = PySequence_Size(PySequence_GetItem(pValue, 0));
        img = std::make_unique<QImage>(width, height, QImage::Format_RGB32);
        for (int i = 0; i < height; i++) {
            PyObject* row = PySequence_GetItem(pValue, i);
            if (row) {
                for (int j = 0; j < width; j++) {
                    PyObject* pixel = PySequence_GetItem(row, j);
                    if (pixel) {
                        img->setPixel(j, i, qRgb(PyLong_AsLong(PySequence_GetItem(pixel,0)), 
                                                 PyLong_AsLong(PySequence_GetItem(pixel,1)), 
                                                 PyLong_AsLong(PySequence_GetItem(pixel,2))));
                    }
                }
            }
        }
        Py_DECREF(pValue);
    } else {
        Py_DECREF(pFunc);
        Py_DECREF(pScript);
        PyErr_Print();
        std::cerr << "Call failed\n";
        return nullptr;
    }
    Py_XDECREF(pFunc);
    Py_DECREF(pScript);
    return std::move(img);
}