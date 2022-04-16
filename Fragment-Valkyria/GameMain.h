#pragma once
/*****************************************************************//**
 * \file   GameMain.h
 * \brief  ゲーム本体クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   constexpr auto ScreenWidth = 1920;   //!< 横画面サイズ
   constexpr auto ScreenHeight = 1080;  //!< 縦画面サイズ
   constexpr auto ScreenDepth = 32;     //!< ビット数
   // 二重インクルード防止
   namespace Create {
      class ObjectFactory;
   }
   namespace Object {
      class ObjectServer;
   }
   namespace Sprite {
      class SpriteServer;
   }
   namespace Stage {
      class LoadStageFromJson;
   }
   namespace Effect {
      class EffectServer;
   }
   /**
    * \brief ゲーム本体
    */
   namespace Game {
      /**
       * \class ゲーム本体クラス
       * \brief ゲームのメイン処理を回す
       */
      class GameMain : public AppFrame::Game::GameBase {
         using GameBase = AppFrame::Game::GameBase;
      public:
         /**
          * \brief 初期化処理
          * \param hInstance WinMainの第一引数
          * \return 初期化に成功したらtrue、失敗したらfalseを返す
          */
         bool Initialize(HINSTANCE hInstance)override;
         /**
          * \brief 実行
          */
         void Run()override;
         /**
          * \brief 停止
          */
         void ShutDown()override;
         /**
          * \brief 入力処理
          */
         void Input()override;
         /**
          * \brief 更新処理
          */
         void Update()override;
         /**
          * \brief 描画処理
          */
         void Render()override;

         /**
          * \brief 雑魚戦をクリアしたかの判定を返す
          * \return クリアしていればtrue,でなければfalse
          */
         inline bool isPoorClear() { return _isPoorClear; }
         /**
          * \brief 雑魚戦をクリアしたかの判定を設定
          * \param poorClear 雑魚戦をクリアしたかの判定
          */
         inline void isPoorClear(bool poorClear) { _isPoorClear = poorClear; }
         /**
          * \brief チュートリアルのクリア判定を返す
          * \return クリアしていればtrue、でなければfalse
          */
         inline bool isTutorialClear() { return _isTutorialClear; }
         /**
          * \brief チュートリアルのクリア判定を設定する
          * \param tutorialClear 設定するクリア判定
          */
         inline void isTutorialClear(bool tutorialClear) { _isTutorialClear = tutorialClear; }
         /**
          * \brief 計測時間の取得
          * \return インゲームで計測している時間
          */
         inline unsigned int ingameTimer() const { return _ingameTimer; }
         /**
          * \brief 計測時間の設定
          * \param timer 設定する時間
          */
         inline void ingameTimer(unsigned int timer) { _ingameTimer = timer; }
         /**
          * \brief 計測時間を進める
          */
         inline void IngameTimerCount() { _ingameTimer++; }
         /**
          * \brief ゲーム内感度の取得
          * \return ゲーム内感度
          */
         inline std::tuple<double, double, int> sensitivity() const { return _sensitivity; }
         /**
          * \brief ゲーム内感度の設定
          * \param cameraSens カメラ感度
          * \param aimSens エイム感度
          * \param deadZone デッドゾーン
          */
         inline void sensitivity(double cameraSens, double aimSens, int deadZone) { _sensitivity = std::make_tuple(cameraSens, aimSens, deadZone); }
         /**
          * \brief プレイヤーのステータスの取得
          * \return プレイヤーのステータス
          */
         inline std::tuple<double, int, int> playerStatus() const { return _playerStatus; }
         /**
          * \brief プレイヤーのステータス設定
          * \param hp 体力
          * \param bullet 残弾
          * \param portion 残回復アイテム数
          */
         inline void playerStatus(double hp, int bullet, int portion) { _playerStatus = std::make_tuple(hp, bullet, portion); }
         /**
          * \brief プレイヤー体力の取得
          * \return プレイヤー体力
          */
         inline double playerHp()const { return std::get<0>(_playerStatus); }
         /**
          * \brief プレイヤー体力の設定
          * \param hp 設定する体力
          */
         inline void playerHp(double hp) {
            auto [oldHp, bullet, portion] = _playerStatus;
            _playerStatus = std::make_tuple(hp, bullet, portion);
         }
         /**
          * \brief プレイヤーの残弾数の取得
          * \return プレイヤーの残弾数
          */
         inline int playerBullet() const { return std::get<1>(_playerStatus); }
         /**
          * \brief プレイヤーの残弾数の設定
          * \param bullet 設定するプレイヤーの残弾数
          */
         inline void playerBullet(int bullet) {
            auto [hp, oldBullet, portion] = _playerStatus;
            _playerStatus = std::make_tuple(hp, bullet, portion);
         }
         /**
          * \brief プレイヤーの残回復アイテム数の取得
          * \return プレイヤーの残回復アイテム数
          */
         inline int playerPortion()const { return std::get<2>(_playerStatus); }
         /**
          * \brief プレイヤーの残回復アイテム数の設定
          * \param portion 設定するプレイヤーの残回復アイテム数
          */
         inline void playerPortion(int portion) {
            auto [hp, bullet, oldPortion] = _playerStatus;
            _playerStatus = std::make_tuple(hp, bullet, portion);
         }
         /**
          * \brief 画面設定の値を返す
          * \return 画面横サイズ、画面縦サイズ、画面ビット数
          */
         inline virtual std::tuple<int, int, int>GraphSize() { return { ScreenWidth,ScreenHeight,ScreenDepth }; }
         /**
          * \brief オブジェクトの生成一括管理クラスの取得
          * \return オブジェクトの生成一括管理クラスの参照
          */
         Create::ObjectFactory& objFactory() const { return *_objFactory; }
         /**
          * \brief ステージ情報管理クラスの取得
          * \return ステージ情報管理クラスの参照
          */
         Stage::LoadStageFromJson& loadStage() const { return *_loadStage; }
         /**
          * \brief オブジェクトサーバーの参照を取得
          * \return オブジェクトサーバーの参照
          */
         inline Object::ObjectServer& objServer() const { return *_objServer; }
         /**
          * \brief スプライトサーバーの参照を取得
          * \return スプライトサーバーの参照
          */
         inline Sprite::SpriteServer& sprServer() const { return *_sprServer; }
         /**
          * \brief エフェクトサーバーの参照を取得
          * \return エフェクトサーバーのポインタ
          */
         inline Effect::EffectServer& efcServer() const { return *_efcServer; }

      private:
         /**
          * \brief ファイルへのカレントパスを登録する
          */
         void CurrentPathRegist();
         /**
          * \brief モードを登録する
          */
         void ModeRegist();

         bool _isPoorClear{ false };                            //!< 雑魚戦をクリアしたか
         bool _isTutorialClear{ false };                          //!< チュートリアルを終了またはスキップしたか
         unsigned int _ingameTimer{ 0 };                        //!< ゲーム内タイマー
         std::tuple<double, double, int> _sensitivity;          //!< ゲーム内感度及びデッドゾーン値のTuple型
         std::tuple<double, int, int> _playerStatus;            //!< プレイヤーのステータスのTuple型
         std::unique_ptr<Object::ObjectServer> _objServer;      //!< オブジェクトの一括管理クラス
         std::unique_ptr<Sprite::SpriteServer> _sprServer;      //!< スプライトの一括管理クラス
         std::unique_ptr<Effect::EffectServer> _efcServer;      //!< エフェクトの一括管理クラス
         std::unique_ptr<Create::ObjectFactory> _objFactory;    //!< オブジェクト生成一括管理クラスのユニークポインタ
         std::unique_ptr<Stage::LoadStageFromJson> _loadStage;  //!< ステージ情報管理クラスのユニークポインタ
      };
   }
}
