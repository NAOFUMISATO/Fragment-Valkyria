#pragma once
/*****************************************************************//**
 * \file   ModeMissionFailed.h
 * \brief  モードミッション失敗クラス
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
       * \class モードミッション失敗クラス
       * \brief プレイヤー死亡時に表示する
       */
      class ModeMissionFailed :public AppFrame::Mode::ModeBase {
      public:
         /**
          * \brief コンストラクタ
          */
         ModeMissionFailed();
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
         void Update() override;
         /**
          * \brief 描画処理
          */
         void Render() override;

      private:
         int _animeNo{ 0 };                   //!< アニメーション番号
         int _animeCnt{ 0 };                  //!< フレームカウント保存用
         bool _cntInit{ true };               //!< フレームカウント初期化フラグ
         std::vector<int> _grHandles{ -1 };   //!< 画像ハンドル
      };
   }
}
