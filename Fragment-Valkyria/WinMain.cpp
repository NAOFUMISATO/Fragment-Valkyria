
/*****************************************************************//**
 * \file   WinMain.cpp
 * \brief  アプリケーションのエントリーポイント
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
   auto& gameInstance = FragmentValkyria::Game::Game::GetInstance();
   auto success = gameInstance.Initialize(hInstance);
   // メインループ
   if (success) {
      gameInstance.Run();
   }
   gameInstance.ShutDown();
   return 0;
}