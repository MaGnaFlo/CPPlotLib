#include "image_provider.hpp"
#include <iostream>
#include <chrono>
#include <cmath>
#include <ranges>
#include <vector>
#include "lineplot.hpp"
#include "figure.hpp"

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{

    // parameters
    const std::string linecolor{"\"r\""};
    constexpr int linewidth{1};
    const std::string linestyle{"\"-\""};
    const std::string pointcolor{"b"};
    constexpr int pointsize{10};
    constexpr bool grid{false};
    const std::string title{"Awesome plot"};
    const std::string xlabel{"The X axis"};
    const std::string ylabel{"The Y axis"};

    const std::unordered_map<std::string, std::string> parameters{{"color", linecolor},
                                                                  {"lw", std::to_string(linewidth)},
                                                                  {"ls", linestyle}};

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
    figure.addPlot(plt::PlotType::LINE, x, y, parameters);
    figure.addPlot(plt::PlotType::LINE, x, y2, {{"color", "\"b\""}});

    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

    // build image
    if (!figure.build())
    {
        return QPixmap();
    }
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    auto dt1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << " Script execution time: " << dt1 << " ms" << std::endl;

    QImage image{figure.data(), figure.width(), figure.height(), figure.width() * 3, QImage::Format_RGB888};

    return QPixmap::fromImage(image);
}