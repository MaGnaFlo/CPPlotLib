#pragma once

#include <Python.h>
#include <vector>
#include <unordered_map>

namespace plt
{
    /// @brief Enumerate of types of plot
    enum class PlotType
    {
        LINE,
        SCATTER,
        BAR
    };
    /// @brief Abstract plot superclass
    class Plot
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
        Plot(const std::vector<double> &xData,
             const std::vector<double> &yData,
             const std::unordered_map<std::string, std::string> &parameters);
        /// @brief Sets the plot parameters
        /// @param parameters unordered map of strings of the parameters
        virtual void _setParameters(const std::unordered_map<std::string, std::string> &parameters);
        /// @brief Builds the arguments for the python targetted plot method as strings
        /// @return Vector of strings, each of which is an argument
        std::vector<std::string> _buildArgs() const;
        /// @brief x data vector
        std::vector<double> _xData;
        /// @brief y data vector
        std::vector<double> _yData;
        /// @brief unordered map of parameters
        std::unordered_map<std::string, std::string> _parameters;
    };
}