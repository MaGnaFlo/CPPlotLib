#include "barplot.hpp"
#include <iostream>
#include <filesystem>
#include <sstream>

namespace plt
{
    BarPlot::BarPlot(const std::vector<double> &xData,
                       const std::vector<double> &yData,
                       const std::unordered_map<std::string, std::string> &parameters)
        : Plot(xData, yData, parameters)
    {
        // default parameters
        const std::unordered_map<std::string, std::string> defaultParameters = {{"width", "0.8"},
                                                                                {"bottom", "0"},
                                                                                {"align", "\"center\""},
                                                                                {"color", "\"r\""},
                                                                                {"linewidth", "1"},
                                                                                {"edgecolor", "\"k\""},
                                                                                {"tick_label", "None"}};
        _parameters.insert(defaultParameters.begin(), defaultParameters.end());
        _setParameters(parameters);
    }

    void BarPlot::_setParameters(const std::unordered_map<std::string, std::string>& parameters)
    {
        for (const auto &[name, value] : parameters)
        {
            if (_parameters.find(name) == _parameters.end())
            {
                std::cerr << "Unknown parameter \"" << name << "\"\n"
                          << std::endl;
                continue;
            }
            _parameters[name] = value;
        }
        const auto it_ticklabel {parameters.find("tick_label")};
        if (it_ticklabel != parameters.end())
        {
            const std::string& value {(*it_ticklabel).second};
            std::istringstream iss {value.c_str()};
            size_t count;
            std::string s;
            while (std::getline(iss, s, ',')) count++;
            if (count != _xData.size()) 
            {
                std::cerr << "Parameter 'tick_label' must have the same length as data" << std::endl;
                _parameters["tick_label"] = "None";
            }
        }

    }

    void BarPlot::execute()
    {
        const auto args {_buildArgs()};
        std::ostringstream oss_script;
        oss_script << "ax.bar(";
        for (const auto& arg : args) oss_script << arg.c_str() << ",";
        oss_script << ")\n";
        PyRun_SimpleString(oss_script.str().c_str());
    }
}
