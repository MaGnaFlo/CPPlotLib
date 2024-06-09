#pragma once

#include <vector>
#include "plot.hpp"
#include "lineplot.hpp"
#include "scatterplot.hpp"
#include "barplot.hpp"
#include <string_view>

namespace plt
{
    /// @brief Main class ruling the figure plot.
    class Figure
    {
    public:
        /// @brief Figure constructor
        /// @param width Figure width, in pixels
        /// @param height Figure height, in pixels
        /// @param dpi Figure dpi
        Figure(int width, int height, int dpi = 50);
        /// @brief Figure destructor
        ~Figure() = default;
        /// @brief Figure width getter
        /// @return Figure width
        int width() const { return _width; }
        /// @brief Figure height getter
        /// @return Figure height
        int height() const { return _height; }
        /// @brief Figure dpi getter
        /// @return Figure dpi
        int dpi() const { return _dpi; }
        /// @brief Image data, as a byte array of size width x height x 3
        /// @return Image data byte array
        const unsigned char *data() const { return _imageData.data(); }
        /// @brief Adds a plot to the figure
        /// @param type Plot type (eg. LinePlot, ScatterPlot, BarPlot)
        /// @param xData Data abscissae
        /// @param yData Data ordinates
        /// @param parameters Plot parameters
        template <Numerical U, Numerical V>
        void addPlot(PlotType type,
                     const std::vector<U> &xData,
                     const std::vector<V> &yData,
                     const std::unordered_map<std::string, std::string> &parameters = {})
        {
            switch (type)
            {
            case PlotType::LINE:
                _plots.push_back(std::make_unique<LinePlot<U,V>>(xData, yData, parameters));
                break;

            case PlotType::SCATTER:
                _plots.push_back(std::make_unique<ScatterPlot<U, V>>(xData, yData, parameters));
                break;

            case PlotType::BAR:
                _plots.push_back(std::make_unique<BarPlot<U,V>>(xData, yData, parameters));
                break;
            }
        }
        
        template <StringLike U, Numerical V>
        void addPlot(PlotType type,
                     const std::vector<U> &xData,
                     const std::vector<V> &yData,
                     const std::unordered_map<std::string, std::string> &parameters = {})
        {
            switch (type)
            {
            case PlotType::LINE: break;
            case PlotType::SCATTER: break;
            case PlotType::BAR:
                _plots.push_back(std::make_unique<BarPlot<U,V>>(xData, yData, parameters));
                break;
            }
        }

        /// @brief Calls the execution of the plots and builds the image
        /// @return true if all went well
        bool build();

    private:
        /// @brief Initializes the Python interpreter, adds the paths to the necessary modules
        /// @return true if all went well
        bool _initializeInterpreter() const;
        /// @brief Builds the data array from Python data
        /// @param data Python object of the matplotlib figure
        /// @return true if all went well
        bool _buildImage(PyObject *data);
        /// @brief Sets value of a pixel data
        /// @param i Pixel index
        /// @param r Red channel
        /// @param g Green channel
        /// @param b Blue channel
        void _setPixel(int i, int r, int g, int b);
        /// @brief Vector of all the plots to be displayed
        std::vector<std::unique_ptr<IPlot>> _plots;
        /// @brief Data array of the image (size width x height x 3)
        std::vector<unsigned char> _imageData;
        /// @brief Figure width
        int _width = 0;
        /// @brief Figure heught
        int _height = 0;
        /// @brief Figure dpi
        int _dpi = 50;
    };
}