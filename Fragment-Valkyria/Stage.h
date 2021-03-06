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
          */
         Stage();
         /**
          * \brief 描画処理
          */
         void Draw();
         /**
          * \brief オブジェクトの種別を返す
          * \return ステージ
          */
         ObjectType GetObjType() const override { return ObjectType::Stage; }
         /**
          * \brief ステージ描画一括管理クラスの参照を取得
          * \return ステージ描画一括管理クラスの参照
          */
         StageModelComponent& stageComponent() { return *_stageComponent; }

      private:
         std::unique_ptr<StageModelComponent> _stageComponent;       //!< ステージのポインタ
      };
   }
}
