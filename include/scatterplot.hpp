#pragma once

#include "plot.hpp"

namespace plt
{
    /// @brief Scatter plot class
    class ScatterPlot : public Plot
    {
    public:
        /// @brief Scatter plot constructor
        /// @param xData x data vector
        /// @param yData y data vector
        /// @param parameters unordered map of parameters
        ScatterPlot(const std::vector<double> &xData,
                    const std::vector<double> &yData,
                    const std::unordered_map<std::string, std::string> &parameters);
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
        /// @brief Calls scatter plot Python script
        virtual void execute() override;
    };
}