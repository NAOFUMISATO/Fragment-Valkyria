#pragma once
#include <tuple>
#include <string>
#include "Vector4.h"
namespace FragmentValkyria {
   namespace Stage {
      class StageData {
         using Vector4 = AppFrame::Math::Vector4;
         public:
            StageData(std::string_view fileName, Vector4 position, Vector4 rotation, Vector4 scale) :
            _stageName(fileName),_position(position),_rotation(rotation),_scale(scale){
            };
            StageData() {};
            ~StageData() = default;
            std::string_view stageName() const { return _stageName; }
            std::tuple<Vector4, Vector4, Vector4> GetStageParams() const { return { _position,_rotation,_scale }; }

      private:
         std::string _stageName;
         Vector4 _position;
         Vector4 _rotation;
         Vector4 _scale;
      };
   }
}
