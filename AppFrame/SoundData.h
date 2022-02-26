#pragma once
#include <string_view>
#include <vector>
#include <tuple>
/**
 * \brief アプリケーションフレーム
 */
namespace AppFrame {
   namespace Resource {
      class SoundData {
      public:
         SoundData(std::string_view fileName, int initVolume, bool is3Dsound, float radius) :
            _fileName{ fileName }, _volume{ initVolume }, _is3Dsound{ is3Dsound }, _radius{radius}{
         };
         SoundData() {};

         inline std::string_view fileName() { return _fileName; }
         inline std::tuple<int, bool, float> GetSoundParams() { return { _volume ,_is3Dsound,_radius }; }

      private:
         std::string _fileName{ "" };
         int _volume{ 0 };
         bool _is3Dsound{ false };
         float _radius{ 0.0f };
      };
   }
}
