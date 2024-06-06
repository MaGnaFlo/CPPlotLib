#pragma once

#include "plot.hpp"

namespace plt
{
    /// @brief Line plot class
    class LinePlot : public Plot
    {
    public:
        /// @brief Line plot constructor
        /// @param xData x data vector
        /// @param yData y data vector
        /// @param parameters unordered map of parameters
        LinePlot(const std::vector<double> &xData,
                 const std::vector<double> &yData,
                 const std::unordered_map<std::string, std::string> &parameters);
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
        virtual void execute() override;
    };
}