
/*****************************************************************//**
 * \file   SimpleTextureComponent.cpp
 * \brief  âÊëúãyÇ—ÉrÉãÉ{Å[Éhä»à’ï`âÊä÷êîåQ
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "SimpleTextureComponent.h"
#include <DxLib.h>
#include "GameBase.h"
#include "ModeServer.h"
#include "Vector4.h"
#include "DxUtility.h"
namespace {
   constexpr auto DefaultTransFlag = true;
   constexpr auto DefaultTurnFlag = false;
   constexpr auto DefaultCenterX = 0.5f;
   constexpr auto DefaultCenterY = 0.0f;
}
namespace AppFrame {

   namespace Texture {
      SimpleTextureComponent::SimpleTextureComponent(Game::GameBase& gameBase) :_gameBase(gameBase) {
      }

      void SimpleTextureComponent::DrawTexture(int x, int y, double scale, double angle, int handle) {
         int width,height;
         GetGraphSize(handle,&width,&height);
         auto drawX = x + width / 2;
         auto drawY = y + height / 2;
         DrawRotaGraph(drawX, drawY, scale, angle, handle, DefaultTransFlag, DefaultTurnFlag);
      }

      void SimpleTextureComponent::DrawTexture(int x, int y, double scale, double angle, std::vector<int> handles, int animeSpeed) {
         auto count = _gameBase.modeServer().frameCount();
         auto animeMax = handles.size();
         auto animeNo = (count / animeSpeed) % animeMax;
         int width, height;
         GetGraphSize(handles[animeNo], &width, &height);
         auto drawX = x + width / 2;
         auto drawY = y + height / 2;
         DrawRotaGraph(drawX, drawY, scale, angle, handles[animeNo], DefaultTransFlag, DefaultTurnFlag);
      }

      void SimpleTextureComponent::TransDrawTexture(int x, int y, int cx, int cy, double scale, double angle, int handle, bool turnFrag) {
         DrawRotaGraph2(x, y, cx, cy, scale, angle, handle, DefaultTransFlag, turnFrag);
      }

      void SimpleTextureComponent::TransDrawTexture(int x, int y, int cx, int cy, double scale, double angle, std::vector<int> handles, int animeSpeed, bool turnFrag) {
         auto count = _gameBase.modeServer().frameCount();
         auto animeMax = handles.size();
         auto animeNo = (count / animeSpeed) % animeMax;
         DrawRotaGraph2(x, y, cx, cy, scale, angle, handles[animeNo], DefaultTransFlag, turnFrag);
      }

      void SimpleTextureComponent::DrawBillBoard(Math::Vector4 pos, double scale, double angle, int handle) {
         int width, height;
         GetGraphSize(handle, &width, &height);
         auto size = static_cast<float>(width) * scale;
         DrawBillboard3D(Math::ToDX(pos), DefaultCenterX, DefaultCenterY, 
            static_cast<float>(size), static_cast<float>(angle), handle, DefaultTransFlag);
      }

      void SimpleTextureComponent::DrawBillBoard(Math::Vector4 pos,double scale, double angle, std::vector<int> handles, int animeSpeed) {
         auto count = _gameBase.modeServer().frameCount();
         auto animeMax = handles.size();
         auto animeNo = (count / animeSpeed) % animeMax;
         int width, height;
         GetGraphSize(handles[animeNo], &width, &height);
         auto size = static_cast<double>(width) * scale;
         DrawBillboard3D(Math::ToDX(pos), DefaultCenterX, DefaultCenterY,
            static_cast<float>(size), static_cast<float>(angle), handles[animeNo], DefaultTransFlag);
      }

      void SimpleTextureComponent::TransDrawBillBoard(Math::Vector4 pos, double cx, double cy, double scale, double angle, int handle) {
         int width, height;
         GetGraphSize(handle, &width, &height);
         auto size = static_cast<double>(width) * scale;
         DrawBillboard3D(Math::ToDX(pos), static_cast<float>(cx), static_cast<float>(cy),
            static_cast<float>(size), static_cast<float>(angle), handle, DefaultTransFlag);
      }

      void SimpleTextureComponent::TransDrawBillBoard(Math::Vector4 pos, double cx, double cy, double scale, double angle, std::vector<int> handles, int animeSpeed) {
         auto count = _gameBase.modeServer().frameCount();
         auto animeMax = handles.size();
         auto animeNo = (count / animeSpeed) % animeMax;
         int width, height;
         GetGraphSize(handles[animeNo], &width, &height);
         auto size = static_cast<double>(width) * scale;
         DrawBillboard3D(Math::ToDX(pos), static_cast<float>(cx), static_cast<float>(cy),
            static_cast<float>(size), static_cast<float>(angle), handles[animeNo], DefaultTransFlag);
      }


   }
}
