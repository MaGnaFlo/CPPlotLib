#pragma once

#include "plot.hpp"

namespace plt
{
    /// @brief Line plot class
    template <Numerical U, Numerical V>
    class LinePlot : public Plot<U, V>
    {
    public:
        /// @brief Line plot constructor
        /// @param xData x data vector
        /// @param yData y data vector
        /// @param parameters unordered map of parameters
        LinePlot(const std::vector<U> &xData,
                 const std::vector<V> &yData,
                 const std::unordered_map<std::string, std::string> &parameters)
            : Plot<U, V>(xData, yData, parameters)
        {
            // default parameters
            const std::unordered_map<std::string, std::string> defaultParameters = {{"color", "\"r\""},
                                                                                    {"lw", "1"},
                                                                                    {"ls", "\"-\""},
                                                                                    {"alpha", "1"}};
            this->_parameters.insert(defaultParameters.begin(), defaultParameters.end());
            this->_setParameters(parameters);
        }
        /// @brief Line plot destructor
        virtual ~LinePlot() = default;
        /// @brief Deleted copy constructor
        LinePlot(LinePlot &) = delete;
        /// @brief Deleted copy assignment operator
        LinePlot &operator=(LinePlot &) = delete;
        /// @brief Deleted move constructor
        LinePlot(LinePlot &&) = delete;
        /// @brief Deleted move assignment operator
        LinePlot &operator=(LinePlot &&) = delete;
        /// @brief Calls line plot Python script
        virtual void execute() override
        {
            const auto args{this->_buildArgs()};
            std::ostringstream oss_script;
            oss_script << "ax.plot(";
            for (const auto &arg : args)
                oss_script << arg.c_str() << ",";
            oss_script << ")\n";
            PyRun_SimpleString(oss_script.str().c_str());
        }
    };
}