
/*****************************************************************//**
 * \file   WinMain.cpp
 * \brief  アプリケーションのエントリーポイント
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "GameBase.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

   auto gameBase = AppFrame::Game::GameBase::gameInstance();
   auto success = gameBase->Initialize(hInstance);
   // メインループ
   if (success) {
      gameBase->Run();
   }
   gameBase->ShutDown();
   return 0;
}
