#pragma once

#include "plot.hpp"

namespace plt
{
    /// @brief Pie plot class
    template <class V>
    class PiePlot : public Plot<double, V>
    {
    public:
        /// @brief Pie plot constructor
        /// @param xData x data vector
        /// @param yData bar heights
        /// @param parameters unordered map of parameters
        PiePlot(const std::vector<V> &xData,
                const std::unordered_map<std::string, std::string> &parameters)
            : Plot<double, V>({}, xData, parameters)
        {
            // default parameters
            const std::unordered_map<std::string, std::string> defaultParameters = {{"autopct","'%1.1f%%'"},
                                                                                    {"pctdistance", "0.5"},
                                                                                    {"labeldistance", "1.25"},
                                                                                    {"shadow", "False"},
                                                                                    {"startangle", "90"},
                                                                                    {"radius", "1"}};
            this->_parameters.insert(defaultParameters.begin(), defaultParameters.end());
            this->_setParameters(parameters);
        }
        /// @brief Pie plot destructor
        virtual ~PiePlot() = default;
        /// @brief Deleted copy constructor
        PiePlot(PiePlot &) = delete;
        /// @brief Deleted copy assignment operator
        PiePlot &operator=(PiePlot &) = delete;
        /// @brief Deleted move constructor
        PiePlot(PiePlot &&) = delete;
        /// @brief Deleted move assignment operator
        PiePlot &operator=(PiePlot &&) = delete;
        /// @brief Sets the pie plot parameters(override Plot's)
        /// @param parameters unordered map of strings of the parameters
        virtual void _setParameters(const std::unordered_map<std::string, std::string> &parameters) override
        {
            for (const auto &[name, value] : parameters)
            {
                if (this->_parameters.find(name) == this->_parameters.end())
                {
                    std::cerr << "Unknown parameter \"" << name << "\"\n"
                              << std::endl;
                    continue;
                }
                this->_parameters[name] = value;
            }
        }
        /// @brief Calls pie plot Python script
        virtual void execute() override
        {
            const auto args{this->_buildArgs()};
            std::ostringstream oss_script;
            oss_script << "ax.pie(";
            int i = 0;
            for (const auto &arg : args) {
                if (i++ == 0) continue; // hackish; forgo the first argument
                oss_script << arg.c_str() << ",";
            }
            oss_script << ")\n";
            PyRun_SimpleString(oss_script.str().c_str());
        }
    };

}
