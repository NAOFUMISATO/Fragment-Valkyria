
/*****************************************************************//**
 * \file   GameBase.cpp
 * \brief  ゲーム本体の基底
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "GameBase.h"
#include <DxLib.h>
#include <EffekseerForDxLib.h>
#include "ModeServer.h"
#include "InputManager.h"
#include "ResourceServer.h"
#include "SoundComponent.h"
#include "CurrentPathServer.h"
#include "LoadResourceJson.h"
#include "EffectServer.h"
#include "ObjectServer.h"
#include "TextureComponent.h"
#include "SpriteServer.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief ゲームベース
    */
   namespace Game {
      GameBase* GameBase::_gameInstance = nullptr;

      GameBase::GameBase() {
         _gameInstance = this;
      };

      GameBase::~GameBase() {
      };
      bool GameBase::Initialize(HINSTANCE hinstance) {
         //デバッグモードならウインドウモードに設定及び、デバッグログの出力を行う
#ifndef _DEBUG
         OutputDebugLog(false);
         AppWindowMode(false);
#else
         OutputDebugLog(true);
         AppWindowMode(true);
#endif
         //画面サイズ設定
         auto [width, height, depth] = GraphSize();
         SetGraphMode(width, height, depth);

         // DirectX11を使用可(Effekseerを使用する為)
         SetUseDirect3DVersion(DX_DIRECT3D_11);
        
         //エラーが起きたら直ちに終了
         if (DxLib_Init() == -1) {
            return false;
         }

         SetBackgroundColor(0, 0, 255);
         //描画先を裏画面にセット
         SetDrawScreen(DX_SCREEN_BACK);

         // Effekseerの初期化
         if (Effekseer_Init(8000) == -1)
         {
            DxLib_End();
            return -1;
         }

         // フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ
         SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
         // DXライブラリのデバイスロストした時のコールバックの設定
         Effekseer_SetGraphicsDeviceLostCallbackFunctions();

         // Ｚバッファを有効にする
         SetUseZBuffer3D(TRUE);
         // Ｚバッファへの書き込みを有効にする
         SetWriteZBuffer3D(TRUE);

         _inputManager = std::make_unique<Input::InputManager>();

         _resServer = std::make_unique<Resource::ResourceServer>(*this);

         _soundComponent = std::make_unique<Sound::SoundComponent>(*this);

         _pathServer = std::make_unique<Path::CurrentPathServer>();

         _loadresJson = std::make_unique<Resource::LoadResourceJson>(*this);

         _efcServer = std::make_unique<Effect::EffectServer>();

         _objServer = std::make_unique<Object::ObjectServer>();

         _texComponent = std::make_unique<Texture::TextureComponent>(*this);

         _sprServer = std::make_unique <Sprite::SpriteServer>();

         return true;
      }

      void GameBase::Run() {
         //メインループ
         while (_gState != GameState::Quit) {
            Input();  // 入力
            Update(); // 更新
            Render(); // 描画
         }
      }

      void GameBase::ShutDown() {
         Effkseer_End();
         DxLib_End();
      }


      void GameBase::Input() {
         if (ProcessMessage() == -1) {
            _gState = GameState::Quit;  // -1 が返ってきたのでゲームを終了する
         }
         // 入力状態の更新
         _inputManager->Update();
         if (_inputManager->GetKeyboard().EscClick()) {
            _gState = GameState::Quit;  // ESC押されたのでゲームを終了する
         }
         _modeServer->Input(*_inputManager);    // モードサーバーの入力処理を実行
      }

      void GameBase::Update() {
         //モードの更新処理
         _modeServer->Update();
      }

      void GameBase::Render() {
         //モードの描画処理
         ClearDrawScreen();
         _modeServer->Render();
         ScreenFlip();
      }
   }
}