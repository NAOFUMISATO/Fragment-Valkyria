#pragma once
/*****************************************************************//**
 * \file   ModePoor.h
 * \brief  モード雑魚戦クラス
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ModeInGameBase.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief モード関係
    */
   namespace Mode {
      /**
       * \class モード雑魚戦クラス
       * \brief 雑魚戦の処理を行う
       */
      class ModePoor : public ModeInGameBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         ModePoor(Game::GameMain& gameMain);
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 入口処理
          */
         void Enter()override;
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         void Input(AppFrame::Input::InputManager& input)override;
         /**
          * \brief 更新処理
          */
         void Update()override;
         /**
          * \brief 描画処理
          */
         void Render()override;

      private:
         /**
          * \brief 雑魚戦ウェーブ処理
          */
         void WaveProcess();
         int _wave{ 0 };   //!< 雑魚戦ウェーブ数
      };
   }
}
