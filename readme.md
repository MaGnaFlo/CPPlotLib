# CPPlotLib

CPPlotlib is a simple C++ API to plot figures in the same way Python's matplotlib does.

## Line and scatter plots

```cpp
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
figure.addPlot(plt::PlotType::LINE, x, y);
figure.addPlot(plt::PlotType::LINE, x, y2, {{"color", "\"b\""}});
figure.addPlot(plt::PlotType::SCATTER, x, y2, {{"c", "\"yellow\""},
                                               {"edgecolor", "\"k\""},
                                               {"s", "500"}});
figure.setGrid(true);
figure.setXLabel("Abscissa");
figure.setYLabel("Ordinate");
figure.setTitle("Example");

// build image
figure.build();
auto figure_data {figure.data()};

// ...
```
![alt text](https://github.com/MaGnaFlo/CPPlotLib/blob/master/screenshots/line_scatter.png)

## Bar plot

```cpp
// data
std::vector<std::string> x_bar{{"0", "1", "2", "3"}};
std::vector<double> y_bar{{20, 10, 5, 15}};

// figure
plt::Figure figure{1200, 600, 100};
figure.addPlot(plt::PlotType::BAR, x_bar, y_bar);
figure.setGrid(true);
figure.setXLabel("Abscissa");
figure.setYLabel("Ordinate");
figure.setTitle("Example");

// build image
figure.build();
auto figure_data {figure.data()};

// ...
```

![alt text](https://github.com/MaGnaFlo/CPPlotLib/blob/master/screenshots/barplot.png)
