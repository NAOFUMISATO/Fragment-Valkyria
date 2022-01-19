#pragma once
/*****************************************************************//**
 * \file   StateBaseRoot.h
 * \brief  各状態基底クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief 状態関係
    */
   // 二重インクルード防止
   namespace Input {
      class InputManager;
   }
   namespace State {
      /**
       * \class 各状態基底クラス
       * \brief StateServerに登録する各状態の基底クラス
       *        このクラスを継承して状態を定義する
       */
      class StateBaseRoot {
      public:
         /**
          * \brief コンストラクタ
          */
         StateBaseRoot() {};
         /**
          * \brief 入口処理 1度だけ呼ばれる
          */
         virtual void Enter() {};
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         virtual void Input(Input::InputManager& input) {};
         /**
          * \brief 更新処理
          */
         virtual void Update() {};
         /**
          * \brief 描画処理
          */
         virtual void Draw() {};
         /**
          * \brief 出口処理
          */
         virtual void Exit() {};
      };
   }
}

