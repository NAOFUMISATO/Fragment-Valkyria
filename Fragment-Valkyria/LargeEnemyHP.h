#pragma once
#include "SpriteBase.h"

namespace FragmentValkyria {
   namespace Enemy {
      class LargeEnemyHP :public Sprite::SpriteBase {
         using Color = AppFrame::Data::Color;
         using Rect = AppFrame::Data::Rect;
      public:
         LargeEnemyHP(Game::GameMain& gameMain);
         void Init()override;
         void Update()override;
         void Draw()override;
         virtual SpriteType GetSprType() const { return SpriteType::Sprite; }

         private:
            double _hp{ 1000.0 };
            double _hpRange{ 10000.0 };
            double _rate{ 0.0 };              //!< �w�ʃo�[�̑������x
            double _oldFrontHP{ 0.0 };        //!< 1�t���[���O�̑O�ʃo�[�̃v���C���[HP
            double _oldBackHP{ 0.0 };         //!< 1�t���[���O�̔w�ʃo�[�̃v���C���[HP
            bool _rateReset{ true };          //!< �w�ʃo�[�̑������x�����Z�b�g���邩
            bool _shake{ false };             //!< HP�o�[��U�������邩
            Color _frontColor{ 255,255,255 }; //!< �O�ʃo�[�̃J���[
            Rect _frontBar{ 0,0,0,0 };        //!< �O�ʃo�[��`
            Color _backColor{ 255,255,255 };  //!< �w�ʃo�[�̃J���[
            Rect _backBar{ 0,0,0,0 };         //!< �w�ʃo�[��`
            Rect _offSet{ 0,0,0,0 };          //!< ��`�`��̃I�t�Z�b�g
      };
   }
}
