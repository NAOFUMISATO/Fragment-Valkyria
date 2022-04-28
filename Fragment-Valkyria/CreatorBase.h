#pragma once
/*****************************************************************//**
 * \file   CreatorBase.h
 * \brief  オブジェクト生成管理の基底クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <memory>
#include "AppFrame.h"
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   // 二重インクルード防止
   namespace Game {
      class Game;
   }
   namespace Object {
      class ObjectBase;
   }
   /**
    * \brief オブジェクト生成関係
    */
   namespace Create {
      /**
       * \class オブジェクト生成管理の基底クラス
       * \brief 各生成管理クラスはこのクラスを継承して定義する
       */
      class CreatorBase {
      public:
         /**
          * \brief コンストラクタ
          */
         CreatorBase() {};
         /**
          * \brief デフォルトデストラクタ
          */
         virtual ~CreatorBase() = default;
         /**
          * \brief 派生先でオブジェクト生成を行う
          * \return 派生先で定義
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() = 0;
         /**
          * \brief ゲーム本体クラスからJson読み込み用クラスの参照の取得
          * \return Json読み込み用クラスの参照
          */
         inline AppFrame::Resource::LoadResourceJson& GetLoadJson() const;
      };
   }
}