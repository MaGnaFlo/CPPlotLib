#pragma once

#include <vector>

namespace plt {

    class Figure
    {
        public:
        Figure(int width, int height, int dpi=50) : _dpi(dpi)
        {
            resize(width, height);
        }
        ~Figure() {}
        void resize(int width, int height)
        {
            _width = width;
            _height = height;
            _data.resize(3 * _width * _height);
        }
        int width() const {return _width;}
        int height() const {return _height;}
        int dpi() const {return _dpi;}

        void setPixel(int i, int r, int g, int b) 
        {
            _data[i] = r;
            _data[i+1] = g;
            _data[i+2] = b;
        }

        const unsigned char* data() const {return _data.data();}

        private:
        std::vector<unsigned char> _data;
        int _width = 0;
        int _height = 0;
        int _dpi = 50;

    };
}