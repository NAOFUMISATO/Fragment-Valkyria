#pragma once
/*****************************************************************//**
 * \file   TutorialTips.h
 * \brief  チュートリアル表示テキストクラス
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "SpriteBase.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   /**
    * \brief チュートリアル関係
    */
   namespace Tutorial {
      /**
       * \class チュートリアル表示テキストクラス
       * \brief チュートリアルの表示を行う
       */
      class TutorialTips :public Sprite::SpriteBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          * \param tipsName 表示するTIPSの画像に関連付けた文字列
          */
         TutorialTips(Game::GameMain& gameMain,std::string_view tipsName);
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         void Input(AppFrame::Input::InputManager& input) override;
         /**
          * \brief 更新処理
          */
         void Update()override;
         /**
          * \brief 描画処理
          */
         void Draw()override;
         /**
          * \brief スプライト種別の取得
          * \return 種別:チュートリアル表示テキスト
          */
         virtual SpriteType GetSprType() const { return SpriteType::TutorialTips; }
         /**
          * \brief 出現したかどうかを返す
          * \return 出現したならtrue,でないならばfalse
          */
         bool isAppear() { return _isAppear; }
         /**
          * \brief TIPS毎のクリア判定を返す
          * \param key TIPSに関連付けた文字列
          * \return 関連付けた文字列毎のクリア判定
          */
         bool IsTipsClear(std::string_view key) { return _tipsClear[key.data()]; }

      private:
         int _width{ 0 };                                             //!< 画像横サイズ
         bool _isAppear{ false };                                     //!< TIPS出現フラグ
         std::string _tipsName{ "" };                                 //!< TIPSの画像に関連付けた文字列
         std::unordered_map<std::string, int> _tipsNum;               //!< TIPSに関連付ける文字列と番号の連想配列
         std::unordered_map<std::string, bool> _tipsClear;            //!< 関連付けた文字列毎の達成フラグ
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;  //!< 状態一括管理クラスのポインタ

      public:
         /**
          * \class 状態の基底クラス
          * \brief 各状態はこのクラスを派生し定義する
          */
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            /**
             * \brief コンストラクタ
             * \param owner チュートリアル表示テキストクラスの参照
             */
            StateBase(TutorialTips& owner) : _owner{ owner } {};
            /**
             * \brief 描画処理
             */
            void Draw() override;

         protected:
            TutorialTips& _owner;   //!< モードタイトルの参照
         };
         /**
          * \class TIPSのフェードイン状態クラス
          * \brief フェードインさせる
          */
         class StateFadeIn : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner チュートリアル表示テキストクラスの参照
             */
            StateFadeIn(TutorialTips& owner) :StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update()override;
            /**
             * \brief 出口処理
             */
            void Exit()override;
         };
         /**
          * \class クリア判定を行う状態クラス
          * \brief クリア判定を行う
          */
         class StateJudge : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner チュートリアル表示テキストクラスの参照
             */
            StateJudge(TutorialTips& owner) : StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 入力処理
             * \param input 入力一括管理クラスの参照
             */
            void Input(InputManager& input) override;

         private:
            /**
             * \brief 移動のクリア判定を行う
             * \param input 入力一括管理クラスの参照
             */
            void MoveJudge(InputManager& input);
            /**
             * \brief カメラ移動のクリア判定を行う
             * \param input 入力一括管理クラスの参照
             */
            void CameraJudge(InputManager& input);
            /**
             * \brief 弱攻撃のクリア判定を行う
             * \param input 入力一括管理クラスの参照
             */
            void WeakShootJudge(InputManager& input);
            /**
             * \brief リロードのクリア判定を行う
             * \param input 入力一括管理クラスの参照
             */
            void ReloadJudge(InputManager& input);
            /**
             * \brief オブジェクト発射のクリア判定を行う
             * \param input 入力一括管理クラスの参照
             */
            void ObjectShootJudge(InputManager& input);
            /**
             * \brief 回復のクリア判定を行う
             * \param input 入力一括管理クラスの参照
             */
            void HealJudge(InputManager& input);
            bool _isWeakReady{ false };   //!< 弱攻撃構え状態をクリアしたか
            bool _isObjectReady{ false }; //!< オブジェクト発射攻撃構え状態をクリアしたか
         };
         /**
          * \class TIPSのフェードアウト状態クラス
          * \brief フェードアウトさせる
          */
         class StateFadeOut : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner チュートリアル表示テキストクラスの参照
             */
            StateFadeOut(TutorialTips& owner) : StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update()override;
         };
      };
   }
}
