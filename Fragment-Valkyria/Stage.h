#pragma once
/*****************************************************************//**
 * \file   Stage.h
 * \brief  ステージクラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ObjectBase.h"
#include <memory>
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   // 二重インクルード防止
   namespace Model {
      class ModelComponent;
   }
   /**
    * \brief ステージ関係
    */
   namespace Stage {
      // 二重インクルード防止
      class StageModelComponent;
      /**
       * \class ステージクラス
       * \brief スカイスフィア及びステージの管理を行う
       */
      class Stage :public Object::ObjectBase {
      public:
         /**
          * \brief コンストラクタ
          * \param game ゲーム本体クラスの参照
          */
         Stage(Game::GameMain& game);
         /**
          * \brief 更新処理
          */
         void Update();
         /**
          * \brief 描画処理
          */
         void Draw();
         /**
          * \brief オブジェクトの種別を返す
          * \return ステージ
          */
         ObjectType GetObjType() const override { return ObjectType::Stage; }
         StageModelComponent& stageComponent() { return *_stageComponent; }

      private:
         std::unique_ptr<StageModelComponent> _stageComponent;       //!< ステージのポインタ
      };
   }
}
