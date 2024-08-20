#pragma once

#include <Python.h>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <iostream>
#include <string>
#include <sstream>

namespace plt
{
    /// @brief Enumerate of types of plot
    enum class PlotType
    {
        LINE,
        SCATTER,
        BAR,
        PIE
    };

    template <class T>
    concept Numerical = std::integral<T> || std::floating_point<T>;
    template <class T>
    concept StringLike = std::is_convertible_v<T, std::string_view>;

    /// @brief Plot interface
    class IPlot
    {
    public:
        virtual ~IPlot() = default;
        virtual void execute() = 0;
    };

    /// @brief Abstract plot superclass
    template <class U, Numerical V>
    class Plot : public IPlot
    {
    public:
        /// @brief Plot destructor
        virtual ~Plot() = default;
        /// @brief Pure virtual execution method: runs the Python method, as dsecribed in derived classes
        virtual void execute() = 0;

    protected:
        /// @brief Plot constructor, sets the data and plot parameters
        /// @param xData x data vector
        /// @param yData y data vector
        /// @param parameters map of parameters (dependent on the type; line width, style, ...)
        Plot(const std::vector<U> &xData,
             const std::vector<V> &yData,
             const std::unordered_map<std::string, std::string> &parameters)
            : _xData(xData),
              _yData(yData),
              _parameters(parameters)
        {
        }

        /// @brief Sets the plot parameters
        /// @param parameters unordered map of strings of the parameters
        virtual void _setParameters(const std::unordered_map<std::string, std::string> &parameters)
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

        /// @brief Builds the arguments for the python targetted plot method as strings
        /// @return Vector of strings, each of which is an argument
        std::vector<std::string> _buildArgs() const
        {
            std::vector<std::string> args;
            std::ostringstream oss_x, oss_y;
            oss_x << "[";
            for (const auto x : _xData)
                oss_x << x << ",";
            oss_x << "]";
            oss_y << "[";
            for (const auto y : _yData)
                oss_y << y << ",";
            oss_y << "]";

            args.push_back(oss_x.str());
            args.push_back(oss_y.str());
            for (const auto &[key, value] : _parameters)
            {
                std::ostringstream oss_param;
                oss_param << key << "=" << value;
                args.push_back(oss_param.str());
            }
            return args;
        }

        /// @brief x data vector
        std::vector<U> _xData;
        /// @brief y data vector
        std::vector<V> _yData;
        /// @brief unordered map of parameters
        std::unordered_map<std::string, std::string> _parameters;
    };
}
