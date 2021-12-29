
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
         constexpr auto ALPHA_MIN = 0.f;       //!< 透明度の最小値
         constexpr auto ALPHA_MAX = 255.f;     //!< 透明度の最大値
         constexpr auto SHORTFADE_TIME = 30.f; //!< フェード時間（小）
         constexpr auto MIDDLEFADE_TIME = 60.f;//!< フェード時間（中）
         constexpr auto LONGFADE_TIME = 90.f;  //!< フェード時間（大）
      }

      ModeFadeBase::ModeFadeBase(Game::GameBase& appBase) : ModeBase(appBase),
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

      ModeFadeIn::ModeFadeIn(Game::GameBase& appBase) : ModeFadeBase(appBase) {
      }

      void ModeFadeIn::Enter() {
         //初期透明度の設定（フェードインしていくため、黒画像の透明度MAXで初期化）
         _alpha = ALPHA_MAX;
         //フェードタイプの文字毎にフェード時間の設定を可能にする
         if (_fadeType == 'S') {
            _deltaAlpha = -(ALPHA_MAX / SHORTFADE_TIME);
         }
         else if (_fadeType == 'M') {
            _deltaAlpha = -(ALPHA_MAX / MIDDLEFADE_TIME);
         }
         else if (_fadeType == 'L') {
            _deltaAlpha = -(ALPHA_MAX / LONGFADE_TIME);
         }
      }
      void ModeFadeIn::Update() {
         //透明度を徐々に減少させ、最小値に達したならこのモードをポップバック
         _alpha += _deltaAlpha;
         if (_alpha <= ALPHA_MIN) {
            _alpha = ALPHA_MIN;
            GetModeServer().PopBack();
         }
      }

      ModeFadeOut::ModeFadeOut(Game::GameBase& appBase) : ModeFadeBase(appBase) {
      }

      void ModeFadeOut::Enter() {
         //初期透明度の設定（フェードアウトしていくため、黒画像の透明度0で初期化）
         _alpha = ALPHA_MIN;
         //フェードタイプの文字毎にフェード時間の設定を可能にする
         if (_fadeType == 'S') {
            _deltaAlpha = (ALPHA_MAX / SHORTFADE_TIME);
         }
         else if (_fadeType == 'M') {
            _deltaAlpha = (ALPHA_MAX / MIDDLEFADE_TIME);
         }
         else if (_fadeType == 'L') {
            _deltaAlpha = (ALPHA_MAX / LONGFADE_TIME);
         }
      }

      void ModeFadeOut::Update() {
         //透明度を徐々に増加させ、最大値に達したならこのモードをポップバックし、下のシーンも一緒にポップバックする
         _alpha += _deltaAlpha;
         if (_alpha >= ALPHA_MAX) {
            _alpha = ALPHA_MAX;
            GetModeServer().PopBack();
            GetModeServer().PopBack();
         }
      }

   }
}