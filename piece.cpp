//==================================================================================================================
//
// �s�[�X[piece.cpp]
// Author:Ryouma Inoue
//
//==================================================================================================================
#include "manager.h"
#include "renderer.h"
#include "piece.h"
#include "inputKeyboard.h"
#include "player.h"
#include "game.h"
#include "scene2D.h"
#include "takaseiLibrary.h"
#include "debug.h"
#include "box.h"
#include "pieceSelect.h"
#include "inputController.h"
#include "sound.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define WhileX 50.0f																// �C�`�}�X�̒�����
#define WhileY 50.0f																// �C�`�}�X�̒�������
#define WhileZ 50.0f																// �C�`�}�X�̒����c
#define Range_X 13																	// ���͈̔�
#define Range_Y 6																	// �c�͈̔�
#define SmallUp 1																	// ���A�b�v
#define MediumUp 2																	// ���A�b�v
#define GreatUp 3																	// ��A�b�v

//==================================================================================================================
// �ÓI�����o�ϐ��̏�����
//==================================================================================================================
LPDIRECT3DTEXTURE9 CPiece::m_pTexture = NULL;			// �e�N�X�`���ϐ�

//==================================================================================================================
// �O���[�o���ϐ�
//==================================================================================================================

//==================================================================================================================
// �R���X�g���N�^
//==================================================================================================================
CPiece::CPiece(PRIORITY type = CScene::PRIORITY_UI) :CScene(type)
{

}

//==================================================================================================================
// �f�X�g���N�^
//==================================================================================================================
CPiece::~CPiece()
{

}

//==================================================================================================================
// ����������
//==================================================================================================================
HRESULT CPiece::Init(void)
{
	// ������
	m_nCntMove_X = 0;
	m_nCntMove_Y = 0;
	m_fSpeed = 0;
	m_fRate = 0;
	m_fTurning = 0;
	m_fDecay = 0;
	m_nPower = 0;
	m_bPlacement = false;
	m_bRelease = false;
	m_bMove = false;
	m_bPut = true;
	m_bMap = false;
	m_bRoute = false;
	m_bRanking = false;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �u���b�N�̏�����
	for (int nCntDepth = 0; nCntDepth < Piece_Depth; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < Piece_Width; nCntWidth++)
		{
			m_bPuzzle[nCntDepth][nCntWidth] = false;
			m_bBox[nCntDepth][nCntWidth] = false;
			m_pBlock[nCntDepth][nCntWidth] = CScene2D::Create(PRIORITY_UI);

			if (m_pBlock[nCntDepth][nCntWidth] != NULL)
			{
				m_pBlock[nCntDepth][nCntWidth]->SetPosition(D3DXVECTOR3(100.0f + nCntWidth * 55.0f,
					100.0f + nCntDepth * 55.0f,
					0.0f));
				m_pBlock[nCntDepth][nCntWidth]->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
				m_pBlock[nCntDepth][nCntWidth]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				m_pBlock[nCntDepth][nCntWidth]->SetTransform();

			}
		}
	}
	return S_OK;
}

//==================================================================================================================
// �I������
//==================================================================================================================
void CPiece::Uninit(void)
{

}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CPiece::Update(void)
{
	// �L�[�{�[�h�擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	// �Q�[���p�b�h�̎擾
	CInputController *pGamepad = CManager::GetInputController();

	float nValueH = 0;									//�R���g���[���[
	float nValueV = 0;									//�R���g���[���[

	// �s�[�X�̐ݒ�
	SetPiece();

	SetStatus();

	// ====================== �R���g���[���[ ====================== //

	if (pGamepad != NULL)
	{// �Q�[���p�b�h�����݂����Ƃ�
		if (pGamepad->GetJoypadUse(0))
		{// �g�p�\�������Ƃ�
			pGamepad->GetJoypadStickLeft(0, &nValueH, &nValueV);

			//// �Q�[���p�b�h����
			//InputGemepad(nValueH, nValueV, fTireRotSpeed, aVec);

			if (pGamepad->GetControllerPress(0, JOYPADKEY_B))
			{
				if (m_bPut == true)
				{
					CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾

					pSound->PlaySoundA(SOUND_LABEL_SE_WithParts);			// �_���[�W���̍Đ�
					pSound->SetVolume(SOUND_LABEL_SE_WithParts, 5.0f);

					m_bPlacement = true;
					m_bMove = true;
				}
			}	// �z�u���Ă��Ȃ�������
			if (m_bPlacement == false)
			{

				// ���ɃX�e�B�b�N���|�ꂽ�Ƃ�
				if (pGamepad->GetControllerTrigger(0, JOYPADKEY_LEFT))
				{
					m_nCntMove_X--;
				}
				else if (pGamepad->GetControllerTrigger(0, JOYPADKEY_RIGHT))
				{// �E�ɃX�e�B�b�N���|�ꂽ�Ƃ�
					m_nCntMove_X++;
				}
				// ���ɃX�e�B�b�N���|�ꂽ�Ƃ�
				if (pGamepad->GetControllerTrigger(0, JOYPADKEY_UP))
				{
					m_nCntMove_Y--;
				}
				else if (pGamepad->GetControllerTrigger(0, JOYPADKEY_DOWN))
				{// ��ɃX�e�B�b�N���|�ꂽ�Ƃ�
					m_nCntMove_Y++;
				}
			}
			else
			{
				// �c���J�E���g
				for (int nDepth = 0; nDepth < Piece_Depth; nDepth++)
				{
					// �����J�E���g
					for (int nWide = 0; nWide < Piece_Width; nWide++)
					{
						// ��Ԋm�F
						if (m_bPuzzle[nDepth][nWide] == true)
						{
							// �F�̕ύX
							m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(m_col));
						}
					}
				}

			}
		}
	}
	// �z�u����
	if (pKeyboard->GetTriggerKeyboard(MOVE_JUMP))
	{
		if (m_bPut == true)
		{
			CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾

			pSound->PlaySoundA(SOUND_LABEL_SE_WithParts);			// �_���[�W���̍Đ�
			pSound->SetVolume(SOUND_LABEL_SE_WithParts, 5.0f);

			m_bPlacement = true;
			m_bMove = true;
		}
	}
	for (int nCntDepth = 0; nCntDepth < Piece_Depth; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < Piece_Width; nCntWidth++)
		{
			if (m_bRelease == true)
			{
				// �u���b�N����
				m_pBlock[nCntDepth][nCntWidth]->Release();
				Release();
			}
		}
	}
	// �z�u���Ă��Ȃ�������
	if (m_bPlacement == false)
	{
		// -----------------------------------------
		// �ړ�����
		// -----------------------------------------
		// ���E����
		if (pKeyboard->GetTriggerKeyboard(MOVE_LEFT))
		{
			m_nCntMove_X--;
		}
		else if (pKeyboard->GetTriggerKeyboard(MOVE_RIGHT))
		{
			m_nCntMove_X++;
		}
		// �㉺����
		else if (pKeyboard->GetTriggerKeyboard(MOVE_ACCEL))
		{
			m_nCntMove_Y--;
		}
		else if (pKeyboard->GetTriggerKeyboard(MOVE_BRAKE))
		{
			m_nCntMove_Y++;
		}
	}
	else
	{
		// �c���J�E���g
		for (int nDepth = 0; nDepth < Piece_Depth; nDepth++)
		{
			// �����J�E���g
			for (int nWide = 0; nWide < Piece_Width; nWide++)
			{
				// ��Ԋm�F
				if (m_bPuzzle[nDepth][nWide] == true)
				{
					// �F�̕ύX
					m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(m_col));
				}
			}
		}

	}
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CPiece::Draw(void)
{

}

//==================================================================================================================
// �|���S������
//==================================================================================================================
CPiece *CPiece::Create(void)
{
	// �V�[�����I�Ɋm��
	CPiece *pPiece = new CPiece(CScene::PRIORITY_UI);

	if (pPiece != NULL)
	{
		// �V�[��������
		pPiece->Init();
	}

	// �l��Ԃ�
	return pPiece;
}

//==================================================================================================================
// �e�N�X�`����񃍁[�h
//==================================================================================================================
HRESULT CPiece::Load(void)
{
	return S_OK;
}

//==================================================================================================================
// �e�N�X�`�����j��
//==================================================================================================================
void CPiece::Unload(void)
{
	// �e�N�X�`���̊J��

}

//==================================================================================================================
// �X�e�[�^�X�̐ݒ�
//==================================================================================================================
void CPiece::SetStatus(void)
{
	// �z�u���Ă���
	if (m_bPlacement == true)
	{
		// �`��ς���
		switch (m_PieceType)
		{
			// ����
			// ����
		case PieceType_Square:
			switch (m_StatusType)
			{
			case StatusType_Route:
				m_fTurning = MediumUp;
				m_bRoute = true;
				break;

			case StatusType_Ranking:
				m_fSpeed = MediumUp;
				m_bRanking = true;
				break;
			}
			break;

			// ����
			// ����
			// ����
			// ����
			// ����
		case PieceType_Rectangle:

			m_fSpeed = MediumUp;
			break;

			// ��
			// ��
			// ��
			// ����
		case PieceType_L_Type:
			m_fTurning = 3.0f;
			break;
			// ��
		case PieceType_Square_1:

			switch (m_StatusType)
			{
			case StatusType_MaxSpeed_SmallUp:
				m_fSpeed = SmallUp;
				break;

			case StatusType_Rate_SmallUp:
				m_fRate = SmallUp;
				break;

			case StatusType_Turning_SmallUp:
				m_fTurning = SmallUp;
				break;
			}
			break;

			// ��
			// ��
		case PieceType_Rectangle_1:
			switch (m_StatusType)
			{
			case StatusType_MaxSpeed_MediumUp:
				m_fSpeed = MediumUp;
				break;

			case StatusType_Rate_MediumUp:
				m_fRate = MediumUp;
				break;

			case StatusType_Turning_MediumUp:
				m_fTurning = MediumUp;
				break;
			}
			break;

			// ��
			// ��
			// ��
			// ��
		case PieceType_Rectangle_2:
			switch (m_StatusType)
			{
			case StatusType_Decay_Down:
				m_fDecay = MediumUp;
				break;
			}

			break;

			// ����
			// ������
			// �@�@����
		case PieceType_Speed:
			m_fSpeed = GreatUp;
			break;

			// ����
			// �@��
			// �@��
			// ����
			// ��
		case PieceType_Speed_1:
			m_fRate = GreatUp;
			break;
		}

	}
}

//==================================================================================================================
// �s�[�X�ݒ�
//==================================================================================================================
void CPiece::SetPiece(void)
{
	// �c���J�E���g
	for (int nDepth = 0; nDepth < Piece_Depth; nDepth++)
	{
		// �����J�E���g
		for (int nWide = 0; nWide < Piece_Width; nWide++)
		{
			m_bBox[nDepth][nWide] = CPieceSelect::GetPuzzle(nDepth, nWide);
			// �`��ς���
			switch (m_PieceType)
			{
			// ����
			// ����
			case PieceType_Square:
				// �z�u
				if (nDepth == m_nCntMove_Y && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y && nWide == m_nCntMove_X + 1)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 1 && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 1 && nWide == m_nCntMove_X + 1)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else
				{
					m_bPuzzle[nDepth][nWide] = false;
				}

				// ��Ԋm�F
				if (m_bPuzzle[nDepth][nWide] == true)
				{
					// �e�N�X�`���ύX
					m_pBlock[nDepth][nWide]->BindTexture("data/tex/grass.jpg");
					// �z�u���Ă��邩�ǂ���
					if (m_bPlacement == false)
					{
						// �F�̕ύX
						if (m_bBox[m_nCntMove_Y][m_nCntMove_X] == false && m_bBox[m_nCntMove_Y][m_nCntMove_X + 1] == false &&
							m_bBox[m_nCntMove_Y + 1][m_nCntMove_X] == false && m_bBox[m_nCntMove_Y + 1][m_nCntMove_X + 1] == false)
						{
							m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
							m_bPut = true;
						}
						else
						{
							// �F�̕ύX
							m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
							m_bPut = false;
						}
					}
					else
					{
						// �F�̕ύX
						m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}

				}
				else
				{
					// �F�ݒ�
					m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}

				// �g�O�ɍs���Ȃ��悤�ɂ���
				if (m_nCntMove_X <= 0)
				{
					m_nCntMove_X = 0;
				}
				else if (m_nCntMove_X >= Range_X)
				{
					m_nCntMove_X = Range_X;
				}

				if (m_nCntMove_Y <= 0)
				{
					m_nCntMove_Y = 0;
				}
				else if (m_nCntMove_Y >= Range_Y)
				{
					m_nCntMove_Y = Range_Y;
				}
				break;

			// ����
			// ����
			// ����
			// ����
			// ����
			case PieceType_Rectangle:

				// �z�u
				if (nDepth == m_nCntMove_Y && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y && nWide == m_nCntMove_X + 1)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 1 && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 1 && nWide == m_nCntMove_X + 1)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 2 && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 2 && nWide == m_nCntMove_X + 1)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 3 && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 3 && nWide == m_nCntMove_X + 1)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 4 && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 4 && nWide == m_nCntMove_X + 1)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else
				{
					m_bPuzzle[nDepth][nWide] = false;
				}

				// ��Ԋm�F
				if (m_bPuzzle[nDepth][nWide] == true)
				{
					// �e�N�X�`���ύX
					m_pBlock[nDepth][nWide]->BindTexture("data/tex/grass.jpg");
					// �z�u���Ă��邩�ǂ���
					if (m_bPlacement == false)
					{
						// �F�̕ύX
						if (m_bBox[m_nCntMove_Y][m_nCntMove_X] == false && m_bBox[m_nCntMove_Y][m_nCntMove_X + 1] == false &&
							m_bBox[m_nCntMove_Y + 1][m_nCntMove_X] == false && m_bBox[m_nCntMove_Y + 1][m_nCntMove_X + 1] == false &&
							m_bBox[m_nCntMove_Y + 2][m_nCntMove_X] == false && m_bBox[m_nCntMove_Y + 2][m_nCntMove_X + 1] == false &&
							m_bBox[m_nCntMove_Y + 3][m_nCntMove_X] == false && m_bBox[m_nCntMove_Y + 3][m_nCntMove_X + 1] == false &&
							m_bBox[m_nCntMove_Y + 4][m_nCntMove_X] == false && m_bBox[m_nCntMove_Y + 4][m_nCntMove_X + 1] == false)
						{
							m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
							m_bPut = true;
						}
						else
						{
							// �F�̕ύX
							m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
							m_bPut = false;
						}
					}
					else
					{
						// �F�̕ύX
						m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}

				}
				else
				{
					// �F�ݒ�
					m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}
				// �g�O�ɍs���Ȃ��悤�ɂ���
				if (m_nCntMove_X <= 0)
				{
					m_nCntMove_X = 0;
				}
				else if (m_nCntMove_X >= Range_X)
				{
					m_nCntMove_X = Range_X;
				}

				if (m_nCntMove_Y <= 0)
				{
					m_nCntMove_Y = 0;
				}
				else if (m_nCntMove_Y >= Range_Y - 3)
				{
					m_nCntMove_Y = Range_Y - 3;
				}
				break;
			// ��
			// ��
			// ��
			// ����
			case PieceType_L_Type:

				// �z�u
				if (nDepth == m_nCntMove_Y && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				// �z�u
				else if (nDepth == m_nCntMove_Y + 1 && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				// �z�u
				else if (nDepth == m_nCntMove_Y + 2 && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				// �z�u
				else if (nDepth == m_nCntMove_Y + 3 && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 3 && nWide == m_nCntMove_X + 1)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else
				{
					m_bPuzzle[nDepth][nWide] = false;
				}

				// ��Ԋm�F
				if (m_bPuzzle[nDepth][nWide] == true)
				{
					// �e�N�X�`���ύX
					m_pBlock[nDepth][nWide]->BindTexture("data/tex/grass.jpg");
					// �z�u���Ă��邩�ǂ���
					if (m_bPlacement == false)
					{
						// �F�̕ύX
						if (m_bBox[m_nCntMove_Y][m_nCntMove_X] == false && m_bBox[m_nCntMove_Y + 1][m_nCntMove_X] == false &&
							m_bBox[m_nCntMove_Y + 2][m_nCntMove_X] == false && m_bBox[m_nCntMove_Y + 3][m_nCntMove_X] == false &&
							m_bBox[m_nCntMove_Y + 3][m_nCntMove_X + 1] == false)
						{
							m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
							m_bPut = true;
						}
						else
						{
							// �F�̕ύX
							m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
							m_bPut = false;
						}
					}
					else
					{
						// �F�̕ύX
						m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}

				}
				else
				{
					// �F�ݒ�
					m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}

				// �g�O�ɍs���Ȃ��悤�ɂ���
				if (m_nCntMove_X <= 0)
				{
					m_nCntMove_X = 0;
				}
				else if (m_nCntMove_X >= Range_X)
				{
					m_nCntMove_X = Range_X;
				}

				if (m_nCntMove_Y <= 0)
				{
					m_nCntMove_Y = 0;
				}
				else if (m_nCntMove_Y >= Range_Y - 2)
				{
					m_nCntMove_Y = Range_Y - 2;
				}

				break;
			// ��
			case PieceType_Square_1:

				// �z�u
				if (nDepth == m_nCntMove_Y && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else
				{
					m_bPuzzle[nDepth][nWide] = false;
				}

				// ��Ԋm�F
				if (m_bPuzzle[nDepth][nWide] == true)
				{
					// �e�N�X�`���ύX
					m_pBlock[nDepth][nWide]->BindTexture("data/tex/grass.jpg");
					// �z�u���Ă��邩�ǂ���
					if (m_bPlacement == false)
					{
						// �F�̕ύX
						if (m_bBox[m_nCntMove_Y][m_nCntMove_X] == false)
						{
							m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
							m_bPut = true;
						}
						else
						{
							// �F�̕ύX
							m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
							m_bPut = false;
						}
					}
					else
					{
						// �F�̕ύX
						m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}

				}
				else
				{
					// �F�ݒ�
					m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}

				// �g�O�ɍs���Ȃ��悤�ɂ���
				if (m_nCntMove_X <= 0)
				{
					m_nCntMove_X = 0;
				}
				else if (m_nCntMove_X >= Range_X + 1)
				{
					m_nCntMove_X = Range_X + 1;
				}

				if (m_nCntMove_Y <= 0)
				{
					m_nCntMove_Y = 0;
				}
				else if (m_nCntMove_Y >= Range_Y + 1)
				{
					m_nCntMove_Y = Range_Y + 1;
				}

				break;

			// ��
			// ��
			case PieceType_Rectangle_1:

				// �z�u
				if (nDepth == m_nCntMove_Y && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				// �z�u
				else if (nDepth == m_nCntMove_Y + 1 && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else
				{
					m_bPuzzle[nDepth][nWide] = false;
				}

				// ��Ԋm�F
				if (m_bPuzzle[nDepth][nWide] == true)
				{
					// �e�N�X�`���ύX
					m_pBlock[nDepth][nWide]->BindTexture("data/tex/grass.jpg");
					//m_pBlock[nDepth][nWide]->SpriteAnimation(D3DXVECTOR2(3.0f, 5.0f), 1, 1);
					// �z�u���Ă��邩�ǂ���
					if (m_bPlacement == false)
					{
						// �F�̕ύX
						if (m_bBox[m_nCntMove_Y][m_nCntMove_X] == false && m_bBox[m_nCntMove_Y + 1][m_nCntMove_X] == false)
						{
							m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
							m_bPut = true;
						}
						else
						{
							// �F�̕ύX
							m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
							m_bPut = false;
						}
					}
					else
					{
						// �F�̕ύX
						m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}

				}
				else
				{
					// �F�ݒ�
					m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}

				// �g�O�ɍs���Ȃ��悤�ɂ���
				if (m_nCntMove_X <= 0)
				{
					m_nCntMove_X = 0;
				}
				else if (m_nCntMove_X >= Range_X + 1)
				{
					m_nCntMove_X = Range_X + 1;
				}

				if (m_nCntMove_Y <= 0)
				{
					m_nCntMove_Y = 0;
				}
				else if (m_nCntMove_Y >= Range_Y)
				{
					m_nCntMove_Y = Range_Y;
				}

				break;

			// ��
			// ��
			// ��
			// ��
			case PieceType_Rectangle_2:

				// �z�u
				if (nDepth == m_nCntMove_Y && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				// �z�u
				else if (nDepth == m_nCntMove_Y + 1 && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				// �z�u
				else if (nDepth == m_nCntMove_Y + 2 && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				// �z�u
				else if (nDepth == m_nCntMove_Y + 3 && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else
				{
					m_bPuzzle[nDepth][nWide] = false;
				}

				// ��Ԋm�F
				if (m_bPuzzle[nDepth][nWide] == true)
				{
					// �e�N�X�`���ύX
					m_pBlock[nDepth][nWide]->BindTexture("data/tex/grass.jpg");
					// �z�u���Ă��邩�ǂ���
					if (m_bPlacement == false)
					{
						// �F�̕ύX
						if (m_bBox[m_nCntMove_Y][m_nCntMove_X] == false && m_bBox[m_nCntMove_Y + 1][m_nCntMove_X] == false &&
							m_bBox[m_nCntMove_Y + 2][m_nCntMove_X] == false && m_bBox[m_nCntMove_Y + 3][m_nCntMove_X] == false)
						{
							m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
							m_bPut = true;
						}
						else
						{
							// �F�̕ύX
							m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
							m_bPut = false;
						}
					}
					else
					{
						// �F�̕ύX
						m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}

				}
				else
				{
					// �F�ݒ�
					m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}

				// �g�O�ɍs���Ȃ��悤�ɂ���
				if (m_nCntMove_X <= 0)
				{
					m_nCntMove_X = 0;
				}
				else if (m_nCntMove_X >= Range_X + 1)
				{
					m_nCntMove_X = Range_X + 1;
				}

				if (m_nCntMove_Y <= 0)
				{
					m_nCntMove_Y = 0;
				}
				else if (m_nCntMove_Y >= Range_Y - 2)
				{
					m_nCntMove_Y = Range_Y - 2;
				}

				break;

				// ����
				// ������
				// �@�@����
			case PieceType_Speed:
				// �z�u
				if (nDepth == m_nCntMove_Y && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y && nWide == m_nCntMove_X + 1)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 1 && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 1 && nWide == m_nCntMove_X + 1)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 1 && nWide == m_nCntMove_X + 2)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 2 && nWide == m_nCntMove_X + 2)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 2 && nWide == m_nCntMove_X + 3)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else
				{
					m_bPuzzle[nDepth][nWide] = false;
				}

				// ��Ԋm�F
				if (m_bPuzzle[nDepth][nWide] == true)
				{
					// �e�N�X�`���ύX
					m_pBlock[nDepth][nWide]->BindTexture("data/tex/grass.jpg");
					// �z�u���Ă��邩�ǂ���
					if (m_bPlacement == false)
					{
						// �F�̕ύX
						if (m_bBox[m_nCntMove_Y][m_nCntMove_X] == false && m_bBox[m_nCntMove_Y][m_nCntMove_X + 1] == false &&
							m_bBox[m_nCntMove_Y + 1][m_nCntMove_X] == false && m_bBox[m_nCntMove_Y + 1][m_nCntMove_X + 1] == false &&
							m_bBox[m_nCntMove_Y + 1][m_nCntMove_X + 2] == false && m_bBox[m_nCntMove_Y + 2][m_nCntMove_X + 2] == false &&
							m_bBox[m_nCntMove_Y + 2][m_nCntMove_X + 3] == false)
						{
							m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
							m_bPut = true;
						}
						else
						{
							// �F�̕ύX
							m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
							m_bPut = false;
						}
					}
					else
					{
						// �F�̕ύX
						m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}

				}
				else
				{
					// �F�ݒ�
					m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}

				// �g�O�ɍs���Ȃ��悤�ɂ���
				if (m_nCntMove_X <= 0)
				{
					m_nCntMove_X = 0;
				}
				else if (m_nCntMove_X >= Range_X - 2)
				{
					m_nCntMove_X = Range_X - 2;
				}

				if (m_nCntMove_Y <= 0)
				{
					m_nCntMove_Y = 0;
				}
				else if (m_nCntMove_Y >= Range_Y - 1)
				{
					m_nCntMove_Y = Range_Y - 1;
				}
				break;

			// ����
			// �@��
			// �@��
			// ����
			// ��
			case PieceType_Speed_1:

				// �z�u
				if (nDepth == m_nCntMove_Y && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				// �z�u
				else if (nDepth == m_nCntMove_Y && nWide == m_nCntMove_X + 1)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				// �z�u
				else if (nDepth == m_nCntMove_Y + 1 && nWide == m_nCntMove_X + 1)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				// �z�u
				else if (nDepth == m_nCntMove_Y + 2 && nWide == m_nCntMove_X + 1)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				// �z�u
				else if (nDepth == m_nCntMove_Y + 3 && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 3 && nWide == m_nCntMove_X + 1)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else if (nDepth == m_nCntMove_Y + 4 && nWide == m_nCntMove_X)
				{
					m_bPuzzle[nDepth][nWide] = true;
				}
				else
				{
					m_bPuzzle[nDepth][nWide] = false;
				}

				// ��Ԋm�F
				if (m_bPuzzle[nDepth][nWide] == true)
				{
					// �e�N�X�`���ύX
					m_pBlock[nDepth][nWide]->BindTexture("data/tex/grass.jpg");
					// �z�u���Ă��邩�ǂ���
					if (m_bPlacement == false)
					{
						// �F�̕ύX
						if (m_bBox[m_nCntMove_Y][m_nCntMove_X] == false && m_bBox[m_nCntMove_Y][m_nCntMove_X + 1] == false &&
							m_bBox[m_nCntMove_Y + 1][m_nCntMove_X + 1] == false && m_bBox[m_nCntMove_Y + 2][m_nCntMove_X + 1] == false &&
							m_bBox[m_nCntMove_Y + 3][m_nCntMove_X] == false && m_bBox[m_nCntMove_Y + 3][m_nCntMove_X + 1] == false &&
							m_bBox[m_nCntMove_Y + 4][m_nCntMove_X] == false)
						{
							m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
							m_bPut = true;
						}
						else
						{
							// �F�̕ύX
							m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
							m_bPut = false;
						}
					}
					else
					{
						// �F�̕ύX
						m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}

				}
				else
				{
					// �F�ݒ�
					m_pBlock[nDepth][nWide]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}

				// �g�O�ɍs���Ȃ��悤�ɂ���
				if (m_nCntMove_X <= 0)
				{
					m_nCntMove_X = 0;
				}
				else if (m_nCntMove_X >= Range_X)
				{
					m_nCntMove_X = Range_X;
				}

				if (m_nCntMove_Y <= 0)
				{
					m_nCntMove_Y = 0;
				}
				else if (m_nCntMove_Y >= Range_Y - 3)
				{
					m_nCntMove_Y = Range_Y - 3;
				}

				break;
			}
		}
	}
}