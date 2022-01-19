#pragma once
/*****************************************************************//**
 * \file   StateServer.h
 * \brief  各状態一括管理クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <string_view>
#include <unordered_map>
#include <memory>
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief 状態関係
    */
    //二重インクルード防止
   namespace Input {
      class InputManager;
   }
   namespace State {
      class StateBaseRoot;
      /**
       * \class 各状態一括管理クラス
       * \brief StateBaseRootを継承し定義した状態クラスの登録、管理を行う
       */
      class StateServer {
      public:
         /**
          * \brief コンストラクタ
          * \param key 最初の状態に関連付ける任意の文字列
          * \param state 最初の状態のインスタンス
          */
         StateServer(std::string_view key, std::shared_ptr<StateBaseRoot> state);
         /**
          * \brief 状態の登録
          * \param key 状態に関連付けるための任意の文字列
          * \param state 状態のインスタンス
          */
         void Register(std::string_view key, std::shared_ptr<StateBaseRoot> state);
         /**
          * \brief リストの一番後ろ(最前面)に状態を追加する
          * \param key 登録済みの状態に関連付けた文字列
          */
         void PushBack(std::string_view key);
         /**
          * \brief リストの一番後ろ(最前面)の状態を除外する
          */
         void PopBack();
         /**
          * \brief 状態の遷移
          * \param key 登録済みの状態に関連付けた文字列
          */
         void GoToState(std::string_view key);
         void InsertBelowBack(std::string_view key);
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         void Input(Input::InputManager& input);
         /**
          * \brief 更新処理
          */
         void Update();
         /**
          * \brief 描画処理
          */
         void Draw() const;

      private:
         std::unordered_map<std::string, std::shared_ptr<StateBaseRoot>> _stateRegistry; //!< 状態を登録する連想配列
         std::list<std::shared_ptr<StateBaseRoot>> _stateList;                           //!< 状態の処理を回す双方向配列
      };
   }
}

