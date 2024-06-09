#pragma once

#include "plot.hpp"
#include <iostream>
#include <filesystem>
#include <sstream>

namespace plt
{
    /// @brief Scatter plot class
    template <Numerical U, Numerical V>
    class ScatterPlot : public Plot<U, V>
    {
    public:
        /// @brief Scatter plot destructor
        virtual ~ScatterPlot() = default;
        /// @brief Deleted copy constructor
        ScatterPlot(ScatterPlot &) = delete;
        /// @brief Deleted copy assignment operator
        ScatterPlot &operator=(ScatterPlot &) = delete;
        /// @brief Deleted move constructor
        ScatterPlot(ScatterPlot &&) = delete;
        /// @brief Deleted move assignment operator
        ScatterPlot &operator=(ScatterPlot &&) = delete;

        /// @brief Scatter plot constructor
        /// @param xData x data vector
        /// @param yData y data vector
        /// @param parameters unordered map of parameters
        ScatterPlot(const std::vector<U> &xData,
                    const std::vector<V> &yData,
                    const std::unordered_map<std::string, std::string> &parameters)
            : Plot<U, V>(xData, yData, parameters)
        {
            // default parameters
            const std::unordered_map<std::string, std::string> defaultParameters = {{"c", "\"r\""},
                                                                                    {"s", "1"},
                                                                                    {"marker", "\".\""},
                                                                                    {"edgecolor", "\"k\""},
                                                                                    {"linewidth", "2"},
                                                                                    {"alpha", "1"}};
            this->_parameters.insert(defaultParameters.begin(), defaultParameters.end());
            this->_setParameters(parameters);
        }

        /// @brief Calls scatter plot Python script
        void execute()
        {
            const auto args{this->_buildArgs()};
            std::ostringstream oss_script;
            oss_script << "ax.scatter(";
            for (const auto &arg : args)
                oss_script << arg.c_str() << ",";
            oss_script << ")\n";
            PyRun_SimpleString(oss_script.str().c_str());
        }
    };
}