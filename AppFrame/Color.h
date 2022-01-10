#pragma once
#include <algorithm>

namespace AppFrame {
   namespace Color {
      class Color {
      public:
         Color(int r, int g, int b, int a) :
            _red(std::clamp(r,0,255)),
            _green(std::clamp(g,0,255)),
            _blue(std::clamp(b,0,255)),
            _alpha(std::clamp(a,0,255)){
         };
         Color() {};
         ~Color() = default;

         std::tuple<int, int, int, int> GetColor() { return std::make_tuple(_red, _green, _blue, _alpha); }
         void SetColor(int r, int g, int b, int a) { 
            _red = std::clamp(r,0,255); 
            _green = std::clamp(g,0,255); 
            _blue = std::clamp(b,0,255); 
            _alpha = std::clamp(a,0,255); 
         }
         int red() { return _red; }
         void red(int r) { _red = std::clamp(r,0,255); }
         int green() { return _green; }
         void green(int g) { _green = std::clamp(g,0,255); }
         int blue() { return _blue; }
         void blue(int b) { _blue = std::clamp(b,0,255); }
         int alpha() { return _alpha; }
         void alpha(int a) { _alpha = std::clamp(a,0,255); }

      private:
         int _red{ 255 };
         int _green{ 255 };
         int _blue{ 255 };
         int _alpha{ 255 };

      };
   }
}
