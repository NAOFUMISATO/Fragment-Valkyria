#pragma once
#include "CreatorBase.h"

namespace FragmentValkyria {

   namespace Create {

      class PoorEnemyMeleeCreator : public CreatorBase
      {
      public:
         PoorEnemyMeleeCreator(Game::GameMain& gameMain);

      private:

         virtual std::unique_ptr<Object::ObjectBase> Create() override;

      };

   }
}