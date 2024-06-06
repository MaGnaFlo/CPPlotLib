#pragma once

#include "plot.hpp"

namespace plt
{
    /// @brief Bar plot class
    class BarPlot : public Plot
    {
    public:
        /// @brief Bar plot constructor
        /// @param xData x data vector
        /// @param yData bar heights
        /// @param parameters unordered map of parameters
        BarPlot(const std::vector<double> &xData,
                const std::vector<double> &yData,
                const std::unordered_map<std::string, std::string> &parameters);
        /// @brief Bar plot destructor
        virtual ~BarPlot() = default;
        /// @brief Deleted copy constructor 
        BarPlot(BarPlot &) = delete;
        /// @brief Deleted copy assignment operator 
        BarPlot &operator=(BarPlot &) = delete;
        /// @brief Deleted move constructor 
        BarPlot(BarPlot &&) = delete;
        /// @brief Deleted move assignment operator 
        BarPlot &operator=(BarPlot &&) = delete;
        /// @brief Sets the bar plot parameters(override Plot's)
        /// @param parameters unordered map of strings of the parameters
        virtual void _setParameters(const std::unordered_map<std::string, std::string> &parameters) override;
        /// @brief Calls bar plot Python script
        virtual void execute() override;
    };
}