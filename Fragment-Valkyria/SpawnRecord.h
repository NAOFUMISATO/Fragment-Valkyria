#pragma once
#include <string_view>
#include "Vector4.h"

namespace FragmentValkyria {
   namespace Create {
      class SpawnRecord {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         SpawnRecord() {};
         SpawnRecord(unsigned int prograss, std::string type, Vector4 position, Vector4 rotation) :
            _progress{ prograss }, _type{ type }, _position{ position }, _rotation{ rotation }{
         }
         std::tuple<unsigned int, std::string, Vector4, Vector4> GetSpawnParam() const { 
            return { _progress ,_type,_position,_rotation }; 
         }

      private:
         unsigned int _progress{ 0 };
         std::string _type{ "" };
         Vector4 _position{ 0,0,0 };
         Vector4 _rotation{ 0,0,0 };
      };
   }
}
