#pragma once
#include "SpriteBaseRoot.h"
#include "Vector4.h"
#include <vector>
#include <unordered_map>

namespace AppFrame {
  
   namespace Sprite {
     
      class SpriteServer {
         using Vector4 = Math::Vector4;
      public:
        
         void Add(std::unique_ptr<SpriteBaseRoot> object);
         
         void Input(AppFrame::Input::InputManager& input);
         
         void Update();
        
         void Render();
         
         void Clear();
        
         void Register(std::string_view key, Vector4 vec);
         
         std::vector<std::unique_ptr<SpriteBaseRoot>>& runSprites() { return _runSprites; };
         
         Vector4 GetVecData(std::string_view key);

      private:
         bool _updating{ false };   //!< 更新しているかのフラグ
         std::vector<std::unique_ptr<SpriteBaseRoot>> _runSprites;
         std::vector<std::unique_ptr<SpriteBaseRoot>> _pendingSprites;
         std::unordered_map<std::string, Vector4> _sprRegistry;
      };
   }
}
