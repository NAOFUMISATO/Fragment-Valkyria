#pragma once
/*****************************************************************//**
 * \file   BulletCreator.h
 * \brief  ���u��U���̐����Ǘ��N���X
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "CreatorBase.h"

namespace FragmentValkyria {

	namespace Create {

		class BulletCreator : public CreatorBase {
		public:
			BulletCreator(Game::GameMain& gameMain);

		private:
			/**
		    * \brief ���u��U���̈ʒu�A�A�j���[�V�����A��Ԃ̓o�^���s��
		    * \param game �Q�[���{�̃N���X�̎Q��
		    * \return ���u��U���̃C���X�^���X
		    */
			virtual std::unique_ptr<Object::ObjectBase> Create() override;
		};
	}
}
