#pragma once

#include <vector>
#include "plot.hpp"

namespace plt
{

    class Figure
    {
    public:
        Figure(int width, int height, int dpi = 50);
        ~Figure() = default;
        int width() const { return _width; }
        int height() const { return _height; }
        int dpi() const { return _dpi; }
        const unsigned char *data() const { return _imageData.data(); }
        void addPlot(PlotType type,
                     const std::vector<double> &xData,
                     const std::vector<double> &yData,
                     const std::unordered_map<std::string, std::string> &parameters = {});
        bool build();

    private:
        bool _initializeInterpreter();
        bool _buildImage(PyObject* data);
        void _setPixel(int i, int r, int g, int b)
            {
                _imageData[i] = r;
                _imageData[i + 1] = g;
                _imageData[i + 2] = b;
            }
        std::vector<std::unique_ptr<Plot>> _plots;
        std::vector<unsigned char> _imageData;
        int _width = 0;
        int _height = 0;
        int _dpi = 50;
    };
}