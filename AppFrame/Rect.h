#pragma once
#include <tuple>
namespace AppFrame{
   namespace Data {
      class Rect {
      public:
         Rect(double left, double right, double top, double bottom) :
            _left(left),_right(right),_top(top),_bottom(bottom){
         };
         Rect() {};

         std::tuple<double, double, double, double> GetRectParams() { return {_left,_right,_top,_bottom}; }

      private:
         double _left;
         double _right;
         double _top;
         double _bottom;
      };

}
}
