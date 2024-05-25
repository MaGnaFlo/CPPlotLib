#include "image_provider.hpp"
#include <iostream>
#include <chrono>
#include <cmath>
#include <ranges>
#include <vector>
#include "lineplot.hpp"

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    plt::LinePlot plot;
    if (!plot.status())
    {
        return QPixmap();
    }

    // data
    constexpr int n{40};
    std::vector<double> x(n);
    std::iota(x.begin(), x.end(), 0);
    auto range = x | std::views::transform([](double val)
                                           { return sin(2 * M_PI * val / n); });
    std::vector<double> y(range.begin(), range.end());

    // parameters
    constexpr int dpi{50};
    constexpr int figwidth{15};
    constexpr int figheight{7};
    const std::string linecolor{"r"};
    constexpr int linewidth{1};
    const std::string linestyle{"-"};
    const std::string pointcolor{"b"};
    constexpr int pointsize{10};
    constexpr bool grid{false};
    const std::string title{"Awesome plot"};
    const std::string xlabel{"The X axis"};
    const std::string ylabel{"The Y axis"};

    plot.setXData(x);
    plot.setYData(y);
    plot.setParameters({{"dpi", std::to_string(dpi)},
                        {"figwidth", std::to_string(figwidth)},
                        {"figheight", std::to_string(figheight)},
                        {"linecolor", linecolor},
                        {"linewidth", std::to_string(linewidth)},
                        {"linestyle", linestyle},
                        {"pointcolor", pointcolor},
                        {"pointsize", std::to_string(pointsize)},
                        {"grid", std::to_string(grid)},
                        {"title", title},
                        {"xlabel", xlabel},
                        {"ylabel", ylabel}});

    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

    // build image
    if (!plot.execute())
    {
        return QPixmap();
    }
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    auto dt1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << " Script execution time: " << dt1 << " ms" << std::endl;

    const auto &imgData{plot.image()};
    QImage image{imgData.data.data(), imgData.width, imgData.height, imgData.width * 3, QImage::Format_RGB888};

    return QPixmap::fromImage(image);
}