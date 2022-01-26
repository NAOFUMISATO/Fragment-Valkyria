#pragma once
#include "ObjectBaseRoot.h"
#include "Vector4.h"
#include <vector>
#include <unordered_map>
/**
 * \brief �A�v���P�[�V�����t���[��
 */
namespace AppFrame {
   /**
    * \brief �I�u�W�F�N�g�֌W
    */
   namespace Object {
      /**
       * \class �I�u�W�F�N�g�̈ꊇ�Ǘ��N���X
       * \brief ObjectBase�œo�^����I�u�W�F�N�g���ꊇ�Ǘ�����
       */
      class ObjectServer {
         using Vector4 = Math::Vector4;
      public:
         /**
          * \brief �I�u�W�F�N�g�𓮓I�z��ɒǉ�����
          * \param object �ǉ�����I�u�W�F�N�g
          */
         void Add(std::unique_ptr<ObjectBaseRoot> object);
         /**
          * \brief _runObjects�ɓo�^����Ă���Active��Ԃ̃I�u�W�F�N�g�̓��͏�������
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(AppFrame::Input::InputManager& input);
         /**
          * \brief _runObjects�ɓo�^����Ă���Active��Ԃ̃I�u�W�F�N�g�̍X�V��������
          */
         void Update();
         /**
          * \brief _runObjects�ɓo�^����Ă���Active��Ԃ̃I�u�W�F�N�g�̕`�揈������
          */
         void Render();
         /**
          * \brief ���I�z��̉��
          */
         void Clear();
         /**
          * \brief �I�u�W�F�N�g��Vector4�f�[�^�����W�X�g���ɓo�^����
          * \param key �I�u�W�F�N�g�Ɋ֘A�t����C�ӂ̕�����
          * \param vec �o�^����Vector4�f�[�^
          */
         void Register(std::string_view key, Vector4 vec);
         /**
          * \brief �I�u�W�F�N�g�̏������񂷓��I�z����擾����
          */
         std::vector<std::unique_ptr<ObjectBaseRoot>>& runObjects() { return _runObjects; };
         /**
          * \brief ���W�X�g���ɓo�^�����I�u�W�F�N�g��Vecto4�f�[�^���擾����
          * \param key �I�u�W�F�N�g��Vecto4�f�[�^�Ɋ֘A�t�����C�ӂ̕�����
          * \return �o�^�����I�u�W�F�N�g�̈ʒu
          */
         Vector4 GetVecData(std::string_view key);

      private:
         bool _updating{ false };   //!< �X�V���Ă��邩�̃t���O
         std::vector<std::unique_ptr<ObjectBaseRoot>> _runObjects;      //!< �I�u�W�F�N�g�̏������񂷗p�̓��I�z��
         std::vector<std::unique_ptr<ObjectBaseRoot>> _pendingObjects;  //!< �ۗ����̃I�u�W�F�N�g��o�^���Ă������I�z��
         std::unordered_map<std::string, Vector4> _objRegistry;         //!< �I�u�W�F�N�g��Vector4�f�[�^��o�^���Ă����A�z�z��
      };
   }
}
