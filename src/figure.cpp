#include "figure.hpp"
#include "lineplot.hpp"

namespace plt
{

    Figure::Figure(int width, int height, int dpi)
    {
        _imageData.width = width;
        _imageData.height = height;
        _imageData.dpi = dpi;
        _imageData.data.resize(3 * width * height);
    }

    void Figure::addPlot(PlotType type,
                         const std::vector<double> &xData,
                         const std::vector<double> &yData,
                         const std::unordered_map<std::string, std::string> &parameters)
    {
        switch (type)
        {
        case PlotType::LINE:
            _plots.push_back(std::make_unique<LinePlot>(_imageData.width, _imageData.height, _imageData.dpi, xData, yData, parameters));
            break;

        case PlotType::SCATTER:
            break;

        case PlotType::BAR:
            break;
        }
    }

    bool Figure::plot()
    {
        bool rc{true};
        // TODO : launch several processes for each plot
        for (const auto &plot : _plots)
        {
            rc = rc && plot->execute();
        }
        //

        for (const auto &plot : _plots)
        {
            // TODO : add images correctly
            _imageData = plot->imageData();
        }

        return rc;
    }
}