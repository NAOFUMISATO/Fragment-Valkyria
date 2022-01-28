#pragma once
#include <tuple>
namespace AppFrame{
   namespace Data {
      class Rect {
      public:
         Rect(double left, double top,double right, double bottom) :
            _left(left),_top(top),_right(right),_bottom(bottom){
         };
         Rect() {};
         void SetRectParams(double left, double top, double right, double bottom){
            _left = left, _top = top, _right = right, _bottom = bottom;
         }
         std::tuple<double, double, double, double> GetRectParams() { return {_left,_top,_right,_bottom}; }
         void right(double right) { _right = right; }
      private:
         double _left{ 0 };
         double _top{ 0 };
         double _right{ 0 };
         double _bottom{ 0 };
      };

}
}
