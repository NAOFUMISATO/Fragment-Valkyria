#pragma once
/*****************************************************************//**
 * \file   GameBase.h
 * \brief  ゲーム本体の基底クラス
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include <memory>
#include <tuple>
#include <DxLib.h>
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   namespace {
      constexpr auto ScreenWidth = 1920;   //!< 横画面サイズ
      constexpr auto ScreenHeight = 1080;  //!< 縦画面サイズ
      constexpr auto ScreenDepth = 32;     //!< ビット数
   }
   //二重インクルード防止
   namespace Mode {
      class ModeServer;
   }
   namespace Resource {
      class ResourceServer;
      class LoadJson;
   }
   namespace Input {
      class InputManager;
   }
   namespace Path {
      class PathServer;
   }
   namespace Sound {
      class SoundServer;
   }
   namespace Effect {
      class EffectServer;
   }
   namespace Object {
      class ObjectServer;
   }
   /**
    * \brief ゲームベース
    */
   namespace Game {
      /**
       * \class アプリケーションの基底クラス
       * \brief ゲームごとに継承して定義する
       */
      class GameBase {
      public:
         /**
          * \brief ゲームの状態列挙
          */
         enum class GameState {
            Play,    //!< 実行中
            Paused,  //!< 一時停止
            Quit     //!< 終了
         };
         /**
          * \brief コンストラクタ
          */
         GameBase();
         /**
          * \brief デストラクタ
          */
         virtual ~GameBase();
         /**
          * \brief 初期化処理
          * \param hInstance WinMainの第一引数
          * \return 初期化成功したか
          */
         virtual bool Initialize(HINSTANCE hInstance);
         /**
          * \brief メインループを実行する
          */
         virtual void Run();
         /**
          * \brief メインループを終了する
          */
         virtual void ShutDown();
         /**
          * \brief メインループ内で実行する入力処理
          */
         virtual void Input();
         /**
          * \brief メインループ内で実行する更新処理
          */
         virtual void Update();
         /**
          * \brief メインループ内で実行する描画処理
          */
         virtual void Render();

         /**
          * \brief ゲームの基底クラスのインスタンスを取得
          * \return ゲームの基底クラスのインスタンス
          */
         inline static GameBase* gameInstance() { return _gameInstance; }
         /**
          * \brief モードサーバーのポインタを取得
          * \return モードサーバーのポインタ
          */
         inline Mode::ModeServer& modeServer() const { return *_modeServer; }
         /**
          * \brief リソースサーバーのポインタを取得
          * \return リソースサーバーのポインタ
          */
         inline Resource::ResourceServer& resServer() const { return *_resServer; }
         /**
          * \brief 入力の一括管理クラスのポインタを取得
          * \return 入力の一括管理クラスのポインタ
          */
         inline Input::InputManager& inputManager() const { return *_inputManager; }
         /**
          * \brief パスサーバーのポインタを取得
          * \return パスサーバーのポインタ
          */
         inline Path::PathServer& pathServer() const { return *_pathServer; }
         /**
          * \brief サウンド管理サーバーのポインタを取得
          * \return サウンド管理サーバーのポインタ
          */
         inline Sound::SoundServer& soundServer() const { return *_soundServer; }
         /**
          * \brief jsonファイル管理のポインタを取得
          * \return jsonファイル管理のポインタ
          */
         inline Resource::LoadJson& loadJson() const { return *_loadJson; }
         /**
          * \brief エフェクトサーバーのポインタを取得
          * \return エフェクトサーバーのポインタ
          */
         inline Effect::EffectServer& efcServer() const { return *_efcServer; }
         /**
          * \brief オブジェクトサーバーのポインタを取得
          * \return オブジェクトサーバーのポインタ
          */
         inline Object::ObjectServer& objServer() const { return *_objServer; }
         /**
          * \brief ウィンドウモードに設定するか
          * \param _window_mode TRUEでウィンドウモードで実行、でなければフルスクリーンモードで実行
          */
         inline virtual void AppWindowMode(bool windowMode) { ChangeWindowMode(windowMode); }
         /**
          * \brief デバッグログを出力するか
          * \param _window_mode TRUEでデバッグログ出力
          */
         inline virtual void OutputDebugLog(bool windowMode) { SetOutApplicationLogValidFlag(windowMode); };
         /**
          * \brief 画面設定の値を返す
          * \return 画面横サイズ、画面縦サイズ、画面ビット数
          */
         inline virtual std::tuple<int, int, int> GraphSize() { return { ScreenWidth,ScreenHeight,ScreenDepth }; }

      protected:
         static GameBase* _gameInstance;        //!< ゲームのインスタンス
         GameState _gState{ GameState::Play };                  //!< ゲーム状態
         std::unique_ptr<Mode::ModeServer> _modeServer;         //!< モードの一括管理クラス
         std::unique_ptr<Resource::ResourceServer> _resServer;  //!< リソースの一括管理クラス
         std::unique_ptr<Input::InputManager> _inputManager;    //!< 入力の一括管理クラス
         std::unique_ptr<Path::PathServer> _pathServer;         //!< パスの一括管理クラス
         std::unique_ptr<Sound::SoundServer> _soundServer;      //!< サウンドの一括管理クラス
         std::unique_ptr<Resource::LoadJson> _loadJson;         //!< jsonファイル管理クラス
         std::unique_ptr<Effect::EffectServer> _efcServer;      //!< エフェクトの一括管理クラス
         std::unique_ptr<Object::ObjectServer> _objServer;
      };
   }
}