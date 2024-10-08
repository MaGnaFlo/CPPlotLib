# CPPlotLib

CPPlotlib is a simple C++ API to plot figures using Python's matplotlib. It naturally uses the Python interpreter in background.

## Installation
Assuming creating _build_ in the same folder as the CMakeLists.txt:
```bash
cmake ..
make
sudo make install
```

## Examples
The example provided uses QML to display the resulting figure.

### Line and scatter plots

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
plt::Figure figure{1200, 600, 100}; // 1200x600 with 100 dpi
if (!figure.init()) {
  std::cerr << "Error during initialization" << std::endl;
  // return if needed
}

figure.addPlot(plt::PlotType::LINE, x, y);
figure.addPlot(plt::PlotType::LINE, x, y2, {{"color", "\"b\""}});
figure.addPlot(plt::PlotType::SCATTER, x, y2, {{"c", "\"yellow\""},
                                               {"edgecolor", "\"k\""},
                                               {"s", "500"}});
figure.setGrid(true);
figure.setXLabel("Abscissa");
figure.setYLabel("Ordinate");
figure.setTitle("Example");

if (!figure.build()) {
  std::cerr << "Error during figure computation" << std::endl;
  // return if needed
}
figure.close(); // necessary to ensure the GIL is released

auto figure_data {figure.data()};
int width {figure.width()};
int height {figure.height()};

// ...
```
![alt text](https://github.com/MaGnaFlo/CPPlotLib/blob/master/screenshots/line_scatter.png)

### Bar plot

```cpp
// data
std::vector<std::string> x_bar{{"0", "1", "2", "3"}};
std::vector<double> y_bar{{20, 10, 5, 15}};

// figure
plt::Figure figure{1200, 600, 100};
if (!figure.init()) {
  std::cerr << "Error during initialization" << std::endl;
  // return if needed
}

figure.addPlot(plt::PlotType::BAR, x_bar, y_bar);
figure.setGrid(true);
figure.setXLabel("Abscissa");
figure.setYLabel("Ordinate");
figure.setTitle("Example");

// build image
if (!figure.build()) {
  std::cerr << "Error during figure computation" << std::endl;
  // return if needed
}
figure.close();

auto figure_data {figure.data()};
int width {figure.width()};
int height {figure.height()};

// ...
```

![alt text](https://github.com/MaGnaFlo/CPPlotLib/blob/master/screenshots/barplot.png)
