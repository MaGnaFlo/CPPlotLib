#include "scatterplot.hpp"
#include <iostream>
#include <filesystem>
#include <sstream>

namespace plt
{
    ScatterPlot::ScatterPlot(const std::vector<double> &xData,
                             const std::vector<double> &yData,
                             const std::unordered_map<std::string, std::string> &parameters)
        : Plot(xData, yData, parameters)
    {
        // default parameters
        const std::unordered_map<std::string, std::string> defaultParameters = {{"c", "\"r\""},
                                                                                {"s", "1"},
                                                                                {"marker", "\".\""},
                                                                                {"edgecolor", "\"k\""},
                                                                                {"linewidth", "2"},
                                                                                {"alpha", "1"}};
        _parameters.insert(defaultParameters.begin(), defaultParameters.end());
        _setParameters(parameters);
    }

    void ScatterPlot::execute()
    {
        const auto args {_buildArgs()};
        std::ostringstream oss_script;
        oss_script << "ax.scatter(";
        for (const auto& arg : args) oss_script << arg.c_str() << ",";
        oss_script << ")\n";
        PyRun_SimpleString(oss_script.str().c_str());
    }
}
