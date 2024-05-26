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
        int width() const { return _imageData.width; }
        int height() const { return _imageData.height; }
        int dpi() const { return _imageData.dpi; }
        const unsigned char *data() const { return _imageData.data.data(); }
        void addPlot(PlotType type,
                     const std::vector<double> &xData,
                     const std::vector<double> &yData,
                     const std::unordered_map<std::string, std::string> &parameters);
        bool plot();

    private:
        std::vector<std::unique_ptr<Plot>> _plots;
        ImageData _imageData;
        int _dpi = 50;
    };
}