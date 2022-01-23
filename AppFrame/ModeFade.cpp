
/*****************************************************************//**
 * \file   ModeFade.cpp
 * \brief  モードのフェード管理
 *
 * \author NAOFUMISATO
 * \date   October 2021
 *********************************************************************/
#include <DxLib.h>
#include "ModeFadeBase.h"
#include "ModeFadeIn.h"
#include "ModeFadeOut.h"
#include "ModeServer.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief モード関係名前空間
    */
   namespace Mode {
      namespace {
         constexpr auto AlphaMin = 0.f;       //!< 透明度の最小値
         constexpr auto AlphaMax = 255.f;     //!< 透明度の最大値
         constexpr auto ShortFadeTime = 30.f; //!< フェード時間（小）
         constexpr auto MiddleFadeTime = 60.f;//!< フェード時間（中）
         constexpr auto LongFadeTime = 90.f;  //!< フェード時間（大）
      }

      ModeFadeBase::ModeFadeBase(Game::GameBase& appBase) : ModeBaseRoot(appBase),
         _color(GetColor(0, 0, 0)) {
      }

      ModeFadeBase::~ModeFadeBase() {
      }

      void ModeFadeBase::Init() {
         _color = GetColor(0, 0, 0);
      }

      void ModeFadeBase::Enter() {
      }

      void ModeFadeBase::Update() {
      }

      void ModeFadeBase::Render() {
         SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(_alpha));
         DrawBox(0, 0, _width, _height, _color, TRUE);
         SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
      }

      ModeFadeIn::ModeFadeIn(Game::GameBase& gameBase) : ModeFadeBase(gameBase) {
      }

      void ModeFadeIn::Enter() {
         //初期透明度の設定（フェードインしていくため、黒画像の透明度MAXで初期化）
         _alpha = AlphaMax;
         //フェードタイプの文字毎にフェード時間の設定を可能にする
         if (_fadeType == 'S') {
            _deltaAlpha = -(AlphaMax / ShortFadeTime);
         }
         else if (_fadeType == 'M') {
            _deltaAlpha = -(AlphaMax / MiddleFadeTime);
         }
         else if (_fadeType == 'L') {
            _deltaAlpha = -(AlphaMax / LongFadeTime);
         }
      }
      void ModeFadeIn::Update() {
         //透明度を徐々に減少させ、最小値に達したならこのモードをポップバック
         _alpha += _deltaAlpha;
         if (_alpha <= AlphaMin) {
            _alpha = AlphaMin;
            GetModeServer().PopBack();
         }
      }

      ModeFadeOut::ModeFadeOut(Game::GameBase& gameBase) : ModeFadeBase(gameBase) {
      }

      void ModeFadeOut::Enter() {
         //初期透明度の設定（フェードアウトしていくため、黒画像の透明度0で初期化）
         _alpha = AlphaMin;
         //フェードタイプの文字毎にフェード時間の設定を可能にする
         if (_fadeType == 'S') {
            _deltaAlpha = (AlphaMax / ShortFadeTime);
         }
         else if (_fadeType == 'M') {
            _deltaAlpha = (AlphaMax / MiddleFadeTime);
         }
         else if (_fadeType == 'L') {
            _deltaAlpha = (AlphaMax / LongFadeTime);
         }
      }

      void ModeFadeOut::Update() {
         //透明度を徐々に増加させ、最大値に達したならこのモードをポップバックし、下のシーンも一緒にポップバックする
         _alpha += _deltaAlpha;
         if (_alpha >= AlphaMax) {
            _alpha = AlphaMax;
            GetModeServer().PopBack();
            GetModeServer().PopBack();
         }
      }

   }
}