#pragma once
/*****************************************************************//**
 * \file   ModeMovie.h
 * \brief  モードムービークラス
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ModeBase.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief モード関係
    */
   namespace Mode {
      /**
       * \class モードムービークラス
       * \brief ムービー再生を行う
       */
      class ModeMovie : public ModeBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         ModeMovie(Game::GameMain& gameMain);
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
         bool _play{ true };  //!< ムービーのオープンフラグ
         int _mvHandle{ -1 }; //!< ムービーハンドル
      };
   }
}