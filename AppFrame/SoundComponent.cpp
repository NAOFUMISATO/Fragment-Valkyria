
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
         auto&& [soundData, handle] = _gameBase.resServer().GetSoundInfo(key);

         if (handle != -1) {
            // 読み込み有り
            ChangeVolumeSoundMem(changeVolume, handle);
         }
         else {
            // 読み込み無し
            SetVolumeMusic(changeVolume);
         }
      }

      void SoundComponent::Stop(std::string_view key) {
         auto&& [soundData, handle] = _gameBase.resServer().GetSoundInfo(key);
         if (handle != -1) {
            // 読み込み有り
            StopSoundMem(handle);
         }
         else {
            // 読み込み無し
            StopMusic();
         }
      }

      void SoundComponent::Play(std::string_view key, int playType, Math::Vector4 pos) {
         auto&& [soundData, handle] = _gameBase.resServer().GetSoundInfo(key);
         auto fileName = soundData.fileName();
         auto [volume, is3Dsound, radius] = soundData.GetSoundParams();
         if (handle != -1) {
            // 読み込み有り
            if (is3Dsound) {
               // 3Dサウンドならば再生する位置を設定
               Set3DPositionSoundMem(Math::ToDX(pos), handle);
            }
            PlaySoundMem(handle, playType, TRUE);
            ChangeVolumeSoundMem(volume, handle);
         }
         else {
            // 読み込み無し
            PlayMusic(fileName.data(), playType);
            SetVolumeMusic(volume);
         }
      }
   }
}
