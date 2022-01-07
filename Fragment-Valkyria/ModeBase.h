#pragma once
#include "AppFrame.h"
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
      class ModeBase : public AppFrame::Mode::ModeBaseRoot {
      public:
         
         ModeBase(Game::GameMain& gameMain);
         /**
          * \brief デストラクタ
          */
         ~ModeBase() override;
         /**
          * \brief 初期化処理
          */
         void Init()override {};
         /**
          * \brief 入口処理
          */
         void Enter()override {};
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         void Input(AppFrame::Input::InputManager& input)override {};
         /**
          * \brief 更新処理
          */
         void Update() override {};
         /**
          * \brief 描画処理
          */
         void Render() override {};

      protected:
         Game::GameMain& _gameMain; //!< ゲームクラスの参照
      };
   }
}
