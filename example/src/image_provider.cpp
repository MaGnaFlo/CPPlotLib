#include "image_provider.hpp"
#include <iostream>
#include <chrono>
#include <cmath>
#include <ranges>
#include <vector>

#include <cpplotlib/figure.hpp>
#include <cpplotlib/pieplot.hpp>
#include <cpplotlib/scatterplot.hpp>
#include <cpplotlib/lineplot.hpp>
#include <cpplotlib/barplot.hpp>


QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    // data
    constexpr int n{40};
    std::vector<double> x(n);
    std::iota(x.begin(), x.end(), 0);
    auto range = x | std::views::transform([](double val)
                                           { return sin(2 * M_PI * val / n); });
    std::vector<double> y(range.begin(), range.end());
    auto range2 = x | std::views::transform([](double val)
                                            { return cos(2 * M_PI * val / n); });
    std::vector<double> y2(range2.begin(), range2.end());

    // figure
    plt::Figure figure{1200, 600, 100};
    // figure.addPlot(plt::PlotType::LINE, x, y);
    // figure.addPlot(plt::PlotType::LINE, x, y2, {{"color", "\"b\""}});
    // figure.addPlot(plt::PlotType::SCATTER, x, y2, {{"c", "\"yellow\""},
    //                                                {"edgecolor", "\"k\""},
    //                                                {"s", "500"}});
    // std::vector<double> x_bar{{0, 1, 2, 3}};
//    std::vector<std::string> x_bar{{"0", "1", "2", "3"}};
//    std::vector<double> y_bar{{20, 10, 5, 15}};
//    figure.addPlot(plt::PlotType::BAR, x_bar, y_bar);
//    figure.setGrid(true);
//    figure.setXLabel("x label");
//    figure.setYLabel("y label");
//    figure.setTitle("title");

    std::vector<std::string> labels {{"'A'", "'B'", "'C'"}};
    std::vector<double> x_pie {{30, 10, 60}};
    figure.addPlot(plt::PlotType::PIE, x_pie, {{"labels", "['A','B','C']"}});

    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

    // build image
    if (!figure.build())
        return QPixmap();
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    auto dt1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << " Script execution time: " << dt1 << " ms" << std::endl;

    QImage image{figure.data(), figure.width(), figure.height(), figure.width() * 3, QImage::Format_RGB888};

    return QPixmap::fromImage(image);
}
