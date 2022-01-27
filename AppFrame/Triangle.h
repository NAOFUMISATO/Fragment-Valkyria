#pragma once
#include <tuple>

namespace AppFrame {

   namespace Data {
      class Triangle {
         using Apex = std::pair<double, double>;
      public:
         Triangle(Apex firstPos, Apex secondPos, Apex thirdPos) :
            _firstPos{ firstPos }, _secondPos{ secondPos }, _thirdPos{ thirdPos }{
         };
         Triangle() {};

         std::tuple<Apex, Apex, Apex> GetTriangleParams() { return { _firstPos ,_secondPos,_thirdPos }; }

      private:
         Apex _firstPos{ 0.0,0.0 };
         Apex _secondPos{ 0.0,0.0 };
         Apex _thirdPos{ 0.0,0.0 };
      };

   }
}
