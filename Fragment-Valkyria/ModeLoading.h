#pragma once
/*****************************************************************//**
 * \file   ModeLoading.h
 * \brief  モード読み込み中クラス
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
       * \class モード読み込み中クラス
       * \brief 読み込み中画面を表示させる
       */
      class ModeLoading :public ModeBase {
      public:
         /**
          * \brief コンストラクタ
          */
         ModeLoading();
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 更新処理
          */
         void Update()override;
         /**
          * \brief 描画処理
          */
         void Render()override;

      private:
         int _grHandle{ -1 };                //!< 画像ハンドル
         std::vector<int> _grHandles{ 0 };   //!< 画像ハンドルを格納した動的配列
      };
   }
}
