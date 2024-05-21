#pragma once
#include <Python.h>
#include <vector>
#include <QImage>
#include <memory>
#include <unordered_map>

class PythonWrapper
{
public:
    PythonWrapper(const std::string& modulesRelativePath);
    ~PythonWrapper();

    bool status() const {return _status;}

    void setScript(const std::string& scriptName) {_scriptName = scriptName;}
    void setFunction(const std::string& functionName) {_functionName = functionName;}
    void setXData(const std::vector<double>& xData) {_xData = xData;}
    void setYData(const std::vector<double>& yData) {_yData = yData;}
    void setParameters(const std::unordered_map<std::string, std::string> parameters);
    std::unique_ptr<QImage> execute();

private:
    bool _status;
    std::string _scriptName;
    std::string _functionName;
    std::vector<double> _xData;
    std::vector<double> _yData;
    std::unordered_map<std::string, std::string> _parameters;
};