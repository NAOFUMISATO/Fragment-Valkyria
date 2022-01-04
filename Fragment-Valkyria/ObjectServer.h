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
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief オブジェクト関係
    */
   namespace Object {
      /**
       * \class オブジェクトの一括管理クラス
       * \brief ObjectBaseで登録するオブジェクトを一括管理する
       */
      class ObjectServer {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief オブジェクトを動的配列に追加する
          * \param object 追加するオブジェクト
          */
         void Add(std::unique_ptr<ObjectBase> object);
         /**
          * \brief _runObjectsに登録されているActive状態のオブジェクトの入力処理を回す
          * \param input 入力一括管理クラスの参照
          */
         void Input(AppFrame::Input::InputManager& input);
         /**
          * \brief _runObjectsに登録されているActive状態のオブジェクトの更新処理を回す
          */
         void Update();
         /**
          * \brief _runObjectsに登録されているActive状態のオブジェクトの描画処理を回す
          */
         void Render();
         /**
          * \brief 動的配列の解放
          */
         void Clear();
         /**
          * \brief オブジェクトのVector4データをレジストリに登録する
          * \param key オブジェクトに関連付ける任意の文字列
          * \param vec 登録するVector4データ
          */
         void Register(std::string_view key, Vector4 vec);
         /**
          * \brief オブジェクトの処理を回す動的配列を取得する
          */
         std::vector<std::unique_ptr<ObjectBase>>& runObjects() { return _runObjects; };
         /**
          * \brief レジストリに登録したオブジェクトの位置を取得する
          * \param key オブジェクトに関連付けた任意の文字列
          * \return 登録したオブジェクトの位置
          */
         Vector4 GetPosition(std::string_view key);
         /**
          * \brief レジストリに登録したオブジェクトの回転角を取得する
          * \param key オブジェクトに関連付けた任意の文字列
          * \return 登録したオブジェクトの回転角
          */
         Vector4 GetForward(std::string_view key);

      private:
         bool _updating{ false };   //!< 更新しているかのフラグ
         std::vector<std::unique_ptr<ObjectBase>> _runObjects;      //!< オブジェクトの処理を回す用の動的配列
         std::vector<std::unique_ptr<ObjectBase>> _pendingObjects;  //!< 保留中のオブジェクトを登録しておく動的配列
         std::unordered_map<std::string, Vector4> _objRegistry;     //!< オブジェクトのVector4データを登録しておく連想配列
      };
   }
}
