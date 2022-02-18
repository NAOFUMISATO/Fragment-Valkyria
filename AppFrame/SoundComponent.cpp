
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
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief 音源関係
    */
   namespace Sound {

      void SoundComponent::Play(std::string_view key) {
         Play(key, DX_PLAYTYPE_BACK);
      }
      void SoundComponent::PlayLoop(std::string_view key) {
         Play(key, DX_PLAYTYPE_LOOP);
      }

      void SoundComponent::ChangeVolume(std::string_view key, int changeVolume) {
         auto&& [filename, handle, volume] = _gameBase.resServer().GetSoundInfo(key);

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
         auto&& [filename, handle, volume] = _gameBase.resServer().GetSoundInfo(key);
         if (handle != -1) {
            // 読み込み有り
            StopSoundMem(handle);
         }
         else {
            // 読み込み無し
            StopMusic();
         }
      }

      void SoundComponent::Play(std::string_view key, int playType) {
         auto&& [filename, handle, volume] = _gameBase.resServer().GetSoundInfo(key);

         if (handle != -1) {
            // 読み込み有り
            PlaySoundMem(handle, playType, TRUE);
            ChangeVolumeSoundMem(volume, handle);
         }
         else {
            // 読み込み無し
            PlayMusic(filename.c_str(), playType);
            SetVolumeMusic(volume);
         }
      }
   }
}
