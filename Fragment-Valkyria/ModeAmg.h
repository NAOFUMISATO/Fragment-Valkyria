#pragma once
/*****************************************************************//**
 * \file   ModeAmg.h
 * \brief  モードAMGロゴクラス
 * 
 * \author NAOFUMISATO
 * \date   March 2022
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
       * \class モードAMGロゴクラス
       * \brief AMGロゴの描画を行う
       */
      class ModeAmg :public ModeBase {
      public:
         /**
          * \brief コンストラクタ
          */
         ModeAmg();
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 入口処理
          */
         void Enter()override;
         /**
          * \brief 更新処理
          */
         void Update() override;
         /**
          * \brief 描画処理
          */
         void Render() override;

      private:
         int _grHandle{ -1 };    //!< 画像ハンドル
         int _fadeCnt{ -1 };     //!< フレームカウント保存用
         bool _cntInit{ false }; //!< フレームカウント初期化フラグ
      };
   }
}
