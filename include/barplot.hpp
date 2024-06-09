#pragma once

#include "plot.hpp"

namespace plt
{
    /// @brief Bar plot class
    template <class U, Numerical V>
    class BarPlot : public Plot<U, V>
    {
    public:
        /// @brief Bar plot constructor
        /// @param xData x data vector
        /// @param yData bar heights
        /// @param parameters unordered map of parameters
        BarPlot(const std::vector<U> &xData,
                const std::vector<V> &yData,
                const std::unordered_map<std::string, std::string> &parameters)
            : Plot<U, V>(xData, yData, parameters)
        {
            // default parameters
            const std::unordered_map<std::string, std::string> defaultParameters = {{"width", "0.8"},
                                                                                    {"bottom", "0"},
                                                                                    {"align", "\"center\""},
                                                                                    {"color", "\"r\""},
                                                                                    {"linewidth", "1"},
                                                                                    {"edgecolor", "\"k\""},
                                                                                    {"tick_label", "None"}};
            this->_parameters.insert(defaultParameters.begin(), defaultParameters.end());
            this->_setParameters(parameters);
        }
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
            const auto it_ticklabel{parameters.find("tick_label")};
            if (it_ticklabel != parameters.end())
            {
                const std::string &value{(*it_ticklabel).second};
                std::istringstream iss{value.c_str()};
                size_t count;
                std::string dummy;
                while (std::getline(iss, dummy, ','))
                    count++;
                if (count != this->_xData.size())
                {
                    std::cerr << "Parameter 'tick_label' must have the same length as data" << std::endl;
                    this->_parameters["tick_label"] = "None";
                }
            }
        }
        /// @brief Calls bar plot Python script
        virtual void execute() override
        {
            const auto args{this->_buildArgs()};
            std::ostringstream oss_script;
            oss_script << "ax.bar(";
            for (const auto &arg : args)
                oss_script << arg.c_str() << ",";
            oss_script << ")\n";
            PyRun_SimpleString(oss_script.str().c_str());
        }
    };
}