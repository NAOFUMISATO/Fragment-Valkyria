#pragma once
/*****************************************************************//**
 * \file   ModeMissionCompleted.h
 * \brief  モードミッション完遂クラス
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "AppFrame.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief モード関係
    */
   namespace Mode {
      /**
       * \class モードミッション完遂クラス
       * \brief ゲームクリア時に表示する
       */
      class ModeMissionCompleted :public AppFrame::Mode::ModeBase {
      public:
         /**
          * \brief コンストラクタ
          */
         ModeMissionCompleted();
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
          * \brief 描画処理
          */
         void Render() override;

      private:
         std::vector<int> _grHandles{ -1 };   //!< 画像ハンドル
      };
   }
}
