#include "lineplot.hpp"
#include <iostream>
#include <filesystem>
#include <sstream>

namespace plt
{
    LinePlot::LinePlot(const std::vector<double> &xData,
                       const std::vector<double> &yData,
                       const std::unordered_map<std::string, std::string> &parameters)
        : Plot(xData, yData, parameters)
    {
        // default parameters
        const std::unordered_map<std::string, std::string> defaultParameters = {{"color", "\"r\""},
                                                                                {"lw", "1"},
                                                                                {"ls", "\"-\""},
                                                                                {"alpha", "1"}};
        _parameters.insert(defaultParameters.begin(), defaultParameters.end());
        _setParameters(parameters);
    }

    void LinePlot::execute()
    {
        const auto args{_buildArgs()};
        std::ostringstream oss_script;
        oss_script << "ax.plot(";
        for (const auto &arg : args) oss_script << arg.c_str() << ",";
        oss_script << ")\n";
        PyRun_SimpleString(oss_script.str().c_str());
    }
}
