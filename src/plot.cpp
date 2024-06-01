#include "plot.hpp"
#include <iostream>
#include <string>
#include <sstream>

namespace plt
{
    Plot::Plot(const std::vector<double> &xData,
               const std::vector<double> &yData,
               const std::unordered_map<std::string, std::string> &parameters)
        : _xData(xData),
          _yData(yData),
          _parameters(parameters)
    {

    }

    void Plot::_setParameters(const std::unordered_map<std::string, std::string>& parameters)
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

    std::vector<std::string> Plot::_buildArgs() const
    {
        std::vector<std::string> args;
        std::ostringstream oss_x, oss_y;
        oss_x << "[";
        for (const auto x : _xData) oss_x << x << ",";
        oss_x << "]";
        oss_y << "[";
        for (const auto y : _yData) oss_y << y << ",";
        oss_y << "]";

        args.push_back(oss_x.str());
        args.push_back(oss_y.str());
        for (const auto& [key, value] : _parameters)
        {
            std::ostringstream oss_param;
            oss_param << key << "=" << value;
            args.push_back(oss_param.str());
        }
        return args;

    }
}
