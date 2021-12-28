#pragma once
/*****************************************************************//**
 * \file   ObjectServer.h
 * \brief  オブジェクトの一括管理クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
#include "ObjectBase.h"

namespace FragmentValkyria {
   namespace Object {
      class ObjectServer {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         void Add(std::unique_ptr<ObjectBase> object);
         void Input(AppFrame::Input::InputManager& input);
         void Update();
         void Render();
         void Clear();
         void Register(std::string_view key, Vector4 vec);
         std::vector<std::unique_ptr<ObjectBase>>& runObjects() { return _runObjects; };
         Vector4 GetPosition(std::string_view key);
         Vector4 GetForward(std::string_view key);
      private:
         bool 	_updating{ false };
         std::vector<std::unique_ptr<ObjectBase>> _runObjects;
         std::vector<std::unique_ptr<ObjectBase>> _pendingObjects;
         std::unordered_map<std::string, Vector4> _objRegistry;
      };
   }
}
