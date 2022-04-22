
/*****************************************************************//**
 * \file   SoundConponent.cpp
 * \brief  音源の一括管理
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "SoundComponent.h"
#include <tuple>
#include <DxLib.h>
#include "ResourceServer.h"
#include "GameBase.h"
#include "DxUtility.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief 音源関係
    */
   namespace Sound {

      void SoundComponent::Play(std::string_view key, Math::Vector4 pos) {
         Play(key, DX_PLAYTYPE_BACK,pos);
      }
      void SoundComponent::PlayLoop(std::string_view key, Math::Vector4 pos) {
         Play(key, DX_PLAYTYPE_LOOP, pos);
      }

      void SoundComponent::ChangeVolume(std::string_view key, int changeVolume) {
         // サウンド再生に必要なデータの取得
         auto& gameInstance = AppFrame::Game::GameBase::GetInstance();
         auto&& [soundData, handle] = gameInstance.resServer().GetSoundInfo(key);
         // 事前読み込みによりハンドルを作成しているか
         if (handle != -1) {
            ChangeVolumeSoundMem(changeVolume, handle);  // 音量の変更
         }
         else {
            SetVolumeMusic(changeVolume);                // 音量の変更
         }
      }

      void SoundComponent::Stop(std::string_view key) {
         // サウンド再生に必要なデータの取得
         auto& gameInstance = AppFrame::Game::GameBase::GetInstance();
         auto&& [soundData, handle] = gameInstance.resServer().GetSoundInfo(key);
         // 事前読み込みによりハンドルを作成しているか
         if (handle != -1) {
            StopSoundMem(handle);   // 音源の停止
         }
         else {
            StopMusic();            // 音源の停止
         }
      }

      void SoundComponent::Play(std::string_view key, int playType, Math::Vector4 pos) {
         // サウンド再生に必要なデータの取得
         auto& gameInstance = AppFrame::Game::GameBase::GetInstance();
         auto&& [soundData, handle] = gameInstance.resServer().GetSoundInfo(key);
         auto fileName = soundData.fileName();
         auto [volume, is3Dsound, radius] = soundData.GetSoundParams();
         // 事前読み込みによりハンドルを作成しているか
         if (handle != -1) {
            // 3Dサウンドで読み込んでいるか
            if (is3Dsound) {
               Set3DPositionSoundMem(Math::ToDX(pos), handle);  // 3Dサウンドを再生する位置を設定
               Set3DRadiusSoundMem(radius, handle);             // 3Dサウンドを再生した時の聞こえる距離を設定
            }
            PlaySoundMem(handle, playType, TRUE);               // 音源を再生
            ChangeVolumeSoundMem(volume, handle);               // 音量の変更
         }
         else {
            PlayMusic(fileName.data(), playType);               // 音源を再生
            SetVolumeMusic(volume);                             // 音量の変更
         }
      }
   }
}
