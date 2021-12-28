#pragma once
/*****************************************************************//**
 * \file   ObjectFactory.h
 * \brief  オブジェクト生成の一括管理クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
#include "CreatorBase.h"
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   // 二重インクルード防止
   namespace Game {
      class GameMain;
   }
   namespace Object {
      class ObjectBase;
   }
   /**
    * \brief オブジェクト生成関係
    */
   namespace Create {
      /**
       * \class オブジェクト生成の一括管理クラス
       * \brief 各オブジェクトの生成管理クラスを登録して使用する
       */
      class ObjectFactory {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         ObjectFactory(Game::GameMain& gameMain);
         /**
          * \brief オブジェクトの生成管理クラスの登録
          * \param type 生成管理クラスを関連付ける際に登録する任意の文字列
          * \param creator 登録する生成管理クラスのインスタンス
          * \return 登録できたならtrue,でなければfalseを返す
          */
         bool Register(std::string_view type, std::unique_ptr<CreatorBase> creator);
         /**
          * \brief 各オブジェクトの生成管理クラスの生成メソッドを呼び出す
          * \param type 生成管理クラスを関連付けた任意の文字列
          * \return 生成したインスタンスのポインタ
          */
         std::unique_ptr<Object::ObjectBase> Create(std::string_view type);
         /**
          * \brief コンテナの全要素削除
          */
         void Clear();

      private:
         Game::GameMain& _gameMain;   //!< ゲーム本体クラスの参照
         std::unordered_map<std::string, std::unique_ptr<CreatorBase>> _creatorMap;   //!< オブジェクト生成管理クラスを登録する連想配列
      };
   }
}
