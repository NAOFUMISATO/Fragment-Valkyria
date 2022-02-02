#pragma once
/*****************************************************************//**
 * \file   ModeTitle.h
 * \brief  モードタイトルクラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
#include "ModeBase.h"
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   // 二重インクルード防止
   namespace Game {
      class GameMain;
   }
   /**
    * \brief モード関係
    */
   namespace Mode {
      /**
       * \class モードタイトルクラス
       * \brief ゲームのタイトル処理を回す
       */
      class ModeTitle : public ModeBase {
         using handles = std::vector<int>;
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体の参照
          */
         ModeTitle(Game::GameMain& gameMain);
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 入口処理
          */
         virtual void Enter()override;
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         void Input(AppFrame::Input::InputManager& input)override;
         /**
          * \brief 更新処理
          */
         void Update() override;
         /**
          * \brief 描画処理
          */
         void Render() override;

      private:
         std::tuple<handles, handles, handles> _grHandles{ 0,0,0 };
         int _alpha{ 255 };           //!< スタートガイドの透明度
      };
   }
}
