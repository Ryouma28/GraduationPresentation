//=============================================================================
//
// �L�����I������ [characterSelect.cpp]
// Author : Noriyuki Hanzawa
//
//=============================================================================
#include "fade.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "characterSelect.h"
#include "bg.h"
#include "inputController.h"
#include "object.h"
#include "camera.h"
#include "sky.h"
#include "sound.h"
#include "player.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
int CCharacterSelect::m_nCarType = 0;			// �Ԃ̃^�C�v

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCharacterSelect::CCharacterSelect()
{
	for (int nCount = 0; nCount < MAX_CARTYPE; nCount++)
	{
		m_pPlayer[nCount] = NULL;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCharacterSelect::~CCharacterSelect()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCharacterSelect::Init(void)
{
	CCamera *pCamera = CManager::GetCamera();
	m_nCarType = 0;

	for (int nCnt = 0; nCnt < MAX_SELECT_UI; nCnt++)
	{
		pBack[nCnt] = CScene2D::Create(CScene::PRIORITY_UI);
	}
	if (pBack[0] != NULL)//Select�̃��S
	{
		pBack[0]->BindTexture("data/tex/Select.png");
		pBack[0]->SetPosition(D3DXVECTOR3(970, 640, 0.0f));
		pBack[0]->SetSize(D3DXVECTOR3(130, 130, 0.0f));
		pBack[0]->SetTransform();
	}
	if (pBack[1] != NULL)//Decide�̃��S
	{
		pBack[1]->BindTexture("data/tex/Decide.png");
		pBack[1]->SetPosition(D3DXVECTOR3(1150, 640, 0.0f));
		pBack[1]->SetSize(D3DXVECTOR3(130, 130, 0.0f));
		pBack[1]->SetTransform();
	}
	if (pBack[2] != NULL)//�ԂP
	{
		pBack[2]->BindTexture("data/tex/car01.jpg");
		pBack[2]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2 - 240, 520, 0.0f));
		pBack[2]->SetSize(D3DXVECTOR3(150, 130, 0.0f));
		pBack[2]->SetTransform();
	}
	if (pBack[3] != NULL)//�ԂQ
	{
		pBack[3]->BindTexture("data/tex/car02.jpg");
		pBack[3]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2 - 80, 520, 0.0f));
		pBack[3]->SetSize(D3DXVECTOR3(150, 130, 0.0f));
		pBack[3]->SetTransform();
	}
	if (pBack[4] != NULL)//�ԂR
	{
		pBack[4]->BindTexture("data/tex/car03.jpg");
		pBack[4]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2 + 80, 520, 0.0f));
		pBack[4]->SetSize(D3DXVECTOR3(150, 130, 0.0f));
		pBack[4]->SetTransform();
	}
	if (pBack[5] != NULL)//�ԂS
	{
		pBack[5]->BindTexture("data/tex/car04.jpg");
		pBack[5]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2 + 240, 520, 0.0f));
		pBack[5]->SetSize(D3DXVECTOR3(150, 130, 0.0f));
		pBack[5]->SetTransform();
	}

	if (pBack[6] != NULL)//CarSelect�̃��S
	{
		pBack[6]->BindTexture("data/tex/CarSelect.png");
		pBack[6]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, 70, 0.0f));
		pBack[6]->SetSize(D3DXVECTOR3(470, 85, 0.0f));
		pBack[6]->SetTransform();
	}
	if (pBack[7] != NULL)//�Z���N�g����Ƃ��̘g�@����ԍŌ�ɕ`�悷�鎖�I
	{
		pBack[7]->BindTexture("data/tex/SelectFrame.png");
		pBack[7]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2 - 240, 520, 0.0f));
		pBack[7]->SetSize(D3DXVECTOR3(150, 130, 0.0f));
		pBack[7]->SetTransform();
	}

	for (int nCount = 0; nCount < MAX_CARTYPE; nCount++)
	{
		m_pPlayer[nCount] = CPlayer::Create();
		if (m_pPlayer[nCount] == NULL) continue;

		switch (nCount)
		{
		case 0:
			m_pPlayer[nCount]->LoadScript(SCRIPT_CAR01, 6);
			break;
		case 1:
			m_pPlayer[nCount]->LoadScript(SCRIPT_CAR02, 6);
			break;
		case 2:
			m_pPlayer[nCount]->LoadScript(SCRIPT_CAR03, 6);
			break;
		case 3:
			m_pPlayer[nCount]->LoadScript(SCRIPT_CAR04, 6);
			break;
		}

		m_pPlayer[nCount]->SetPosition(D3DXVECTOR3(-3805.42f, -3199.76f, -16055.74f));
	}

	// ���X�e�B�b�N�̓|�ꂽ�󋵏�����
	for (int nCnt = 0; nCnt < STICK_MAX; nCnt++)
	{
		m_bStick[nCnt] = false;
	}

	// ��̍쐬
	CSky::Create();

	// ���f�����̓ǂݍ���
	CObject::LoadModelTest("data/text/model.txt");

	// �J�����̈ʒu�ݒ�
	if (pCamera != NULL)
	{
		pCamera->SetStoker(false);
		pCamera->SetPosCamera(D3DXVECTOR3(-3924.55f, -3300.27f, -15951.08f), D3DXVECTOR3(-0.07f, -0.85f, 0.0f));
	}
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CCharacterSelect::Uninit(void)
{
	//�|���S���̊J��
	CScene::ReleaseAll();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCharacterSelect::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputController *pInputController = CManager::GetInputController();
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (CFade::GetFade() == CFade::FADE_NONE)
	{//�t�F�[�h�����������Ă��Ȃ��Ƃ�
		// �Ԃ̎�ނ�ς��鏈��
		ChangeCarPrev();

		if (pInputKeyboard != NULL)
		{// �L�[�{�[�h�����݂��Ă����Ƃ�
			if (pInputKeyboard->GetTriggerKeyboard(DIK_RETURN))
			{// �w��̃L�[�������ꂽ�Ƃ�
				CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾

				pSound->PlaySoundA(SOUND_LABEL_SE_Decision);			// �_���[�W���̍Đ�
				CFade::SetFade(CManager::MODE_PUZZLE_CUSTOM, CFade::FADETYPE_SLIDE);					//�t�F�[�h������
			}
			//�Ԃ̑I������
			if (pInputKeyboard->GetTriggerKeyboard(DIK_RIGHT) && m_nCarType < 3)
			{
				pos = pBack[7]->GetPosition();
				pBack[7]->SetPosition(D3DXVECTOR3(pos.x + 160, pos.y, pos.z));
				pBack[7]->SetTransform();

				m_nCarType++;
			}
			else if (pInputKeyboard->GetTriggerKeyboard(DIK_LEFT) && m_nCarType > 0)
			{
				pos = pBack[7]->GetPosition();
				pBack[7]->SetPosition(D3DXVECTOR3(pos.x - 160, pos.y, pos.z));
				pBack[7]->SetTransform();

				m_nCarType--;
			}
		}
		if (pInputController != NULL)
		{
			if (pInputController->GetJoypadUse(0))
			{// �R���g���[���[����������Ă���Ƃ�
			 //�Q�[���̑J��
				if (pInputController->GetControllerTrigger(0, JOYPADKEY_A) ||			// �Q�[���p�b�h��A�{�_���������ꂽ�Ƃ�
					pInputController->GetControllerTrigger(0, JOYPADKEY_START))			// �Q�[���p�b�h��START�{�^���������ꂽ�Ƃ�
				{
					CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾
					if (pInputController->GetJoypadUse(0))
					{// �R���g���[���[����������Ă���Ƃ�
						float nValueH = 0;				// �R���g���[���[
						float nValueV = 0;				// �R���g���[���[

						// ���X�e�B�b�N�̏��擾
						pInputController->GetJoypadStickLeft(0, &nValueH, &nValueV);

						// ���X�e�B�b�N���|��Ă��Ȃ��Ƃ�
						if (!m_bStick[STICK_LEFT] && !m_bStick[STICK_RIGHT])
						{
							// ���ɃX�e�B�b�N���|�ꂽ�Ƃ�
							if (nValueH > 0)
							{
								// ���X�e�B�b�N���|�ꂽ
								m_bStick[STICK_LEFT] = true;
							}
							else if (nValueH < 0)
							{// �E�ɃX�e�B�b�N���|�ꂽ�Ƃ�
							 // �E�X�e�B�b�N���|�ꂽ
								m_bStick[STICK_RIGHT] = true;
							}

							// ���X�e�B�b�N�̓|�ꂽ�󋵂�true�̂Ƃ�
							if (m_bStick[STICK_LEFT] && m_nCarType > 0)
							{
								pos = pBack[7]->GetPosition();
								pBack[7]->SetPosition(D3DXVECTOR3(pos.x - 160, pos.y, pos.z));
								pBack[7]->SetTransform();
								m_nCarType--;
							}
							else if (m_bStick[STICK_RIGHT] && m_nCarType < 3)
							{// �E�X�e�B�b�N�̓|�ꂽ�󋵂�true�̂Ƃ�
								pos = pBack[7]->GetPosition();
								pBack[7]->SetPosition(D3DXVECTOR3(pos.x + 160, pos.y, pos.z));
								pBack[7]->SetTransform();
								m_nCarType++;
							}
						}

						// �X�e�B�b�N���|��Ă��Ȃ�
						if (nValueH == 0)
						{
							// ���X�e�B�b�N���|��Ă��Ȃ�
							m_bStick[STICK_LEFT] = false;
							// �E�X�e�B�b�N���|��Ă��Ȃ�
							m_bStick[STICK_RIGHT] = false;
						}

						//�Q�[���̑J��
						if (pInputController->GetControllerTrigger(0, JOYPADKEY_A) ||			// �Q�[���p�b�h��A�{�_���������ꂽ�Ƃ�
							pInputController->GetControllerTrigger(0, JOYPADKEY_START))			// �Q�[���p�b�h��START�{�^���������ꂽ�Ƃ�
						{
							CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾

							pSound->PlaySoundA(SOUND_LABEL_SE_Decision);			// �_���[�W���̍Đ�

							CFade::SetFade(CManager::MODE_PUZZLE_CUSTOM, CFade::FADETYPE_SLIDE);					//�t�F�[�h������
						}

						//�Ԃ̑I������
						if (pInputController->GetControllerTrigger(0, JOYPADKEY_RIGHT) && m_nCarType < 3)
						{
							pos = pBack[7]->GetPosition();
							pBack[7]->SetPosition(D3DXVECTOR3(pos.x + 160, pos.y, pos.z));
							pBack[7]->SetTransform();

							m_nCarType++;
						}
						else if (pInputController->GetControllerTrigger(0, JOYPADKEY_LEFT) && m_nCarType > 0)
						{
							pos = pBack[7]->GetPosition();
							pBack[7]->SetPosition(D3DXVECTOR3(pos.x - 160, pos.y, pos.z));
							pBack[7]->SetTransform();

							m_nCarType--;
						}
					}
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCharacterSelect::Draw(void)
{

}

//=============================================================================
// �A�Z�b�g�̓ǂݍ���
//=============================================================================
void CCharacterSelect::LoadAsset(void)
{
}

//=============================================================================
// �T���v���p�̎Ԃ�ς���
//=============================================================================
void CCharacterSelect::ChangeCarPrev(void)
{
	for (int nCount = 0; nCount < MAX_CARTYPE; nCount++)
	{
		// NULL�`�F�b�N
		if (m_pPlayer[nCount] == NULL) continue;

		if (nCount != m_nCarType)
		{
			if (m_pPlayer[nCount]->GetActive())
			{
				m_pPlayer[nCount]->SetActive(false);
			}
		}
		else
		{
			if (!m_pPlayer[nCount]->GetActive())
			{
				m_pPlayer[nCount]->SetActive(true);
			}
		}
	}
}