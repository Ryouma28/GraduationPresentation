//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "ranking.h"
#include "characterSelect.h"
#include "inputKeyboard.h"
#include "inputMouse.h"
#include "inputController.h"
#include "scene2D.h"
#include "camera.h"
#include "collider.h"
#include "light.h"
#include "sound.h"
#include "sceneX.h"
#include "puzzle.h"
#include "network.h"
#include "effect.h"
#include "write.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MANAGER_TEX "data/text/manager/manager_texture.txt"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CRenderer *CManager::m_pRenderer = NULL;											// �����_���[ �|�C���^��������
CInputKeyboard *CManager::m_pInputKeyboard = NULL;									// �L�[�{�[�h �|�C���^��������
CInputMouse *CManager::m_pInputMouse = NULL;										// �}�E�X �|�C���^��������
CInputController *CManager::m_pInputController = NULL;								// �R���g���[���[ �|�C���^��������
CManager::MODE CManager::m_mode = CManager::MODE_NONE;								// ���[�h �ϐ��̏�����

CCamera *CManager::m_pCamera = NULL;												// �J���� �|�C���^��������
CLight *CManager::m_pLight = NULL;													// ���C�g �|�C���^��������
CNetwork *CManager::m_pNetwork = NULL;												// �l�b�g���[�N �|�C���^��������

CGame *CManager::m_pGame = NULL;													// �Q�[�� �|�C���^��������
CTitle *CManager::m_pTitle = NULL;													// �^�C�g�� �|�C���^��������
CPuzzle *CManager::m_pPuzzle = NULL;												// �p�Y���@�|�C���^��������
CResult *CManager::m_pResult = NULL;												// ���U���g �|�C���^��������
CRanking *CManager::m_pRanking = NULL;												// �����L���O �|�C���^��������
CCharacterSelect *CManager::m_pCharacterSelect = NULL;								// �L�����N�^�[�I�� �|�C���^��������

CSound *CManager::m_pSound = NULL;													// �T�E���h �|�C���^��������

/* �X�N���v�g�f�[�^ */
float CManager::m_fSpeedDampingRate = 0.0f;				// �X�s�[�h�̌�����
float CManager::m_fTurningVelocity = 0.0f;				// ���񑬓x

std::map<std::string, LPDIRECT3DTEXTURE9> CManager::m_TexMap = {};					// �e�N�X�`���}�b�v�̏�����
std::map<std::string, MODEL_INFO> CManager::m_ModelMap = {};						// ���f�����}�b�v�̏�����
std::map<std::string, LPD3DXEFFECT> CManager::m_ShaderMap = {};						// �V�F�[�_�[�}�b�v�̏�����

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// �l���Z�b�g
	srand((unsigned int)time(NULL));

	m_pRenderer = new CRenderer;																//�����_���[�̃N���G�C�g
	if (m_pRenderer != NULL)
	{// �����_���[�����݂��Ă����Ƃ�
	 // �����_���[�̏�����
		if (FAILED(m_pRenderer->Init(hInstance, hWnd, bWindow)))
		{
			MessageBox(hWnd, "�����_���[�̏������Ɏ��s", "�x��", MB_ICONWARNING);
			return E_FAIL;
		}
	}

	m_pInputKeyboard = new CInputKeyboard;														//�L�[�{�[�h�̃N���G�C�g

	if (m_pInputKeyboard != NULL)
	{// �L�[�{�[�h�����݂��Ă����Ƃ�
	 // �L�[�{�[�h�̏�����
		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{
			MessageBox(hWnd, "�L�[�{�[�h�̏������Ɏ��s", "�x��", MB_ICONWARNING);
			return E_FAIL;
		}
	}

	m_pInputMouse = new CInputMouse;															//�}�E�X�̃N���G�C�g

	if (m_pInputMouse != NULL)
	{// �}�E�X�����݂��Ă����Ƃ�
		// �}�E�X�̏�����
		if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
		{
			MessageBox(hWnd, "�}�E�X�̏������Ɏ��s", "�x��", MB_ICONWARNING);
			return E_FAIL;
		}
	}

	m_pInputController = new CInputController;													// �R���g���[���[�̐���

	if (m_pInputController != NULL)
	{// �R���g���[���[�����݂��Ă����Ƃ�
		// �R���g���[���[�̏�����
		if (FAILED(m_pInputController->Init(hInstance, hWnd)))
		{
			MessageBox(hWnd, "�R���g���[���[�̏������Ɏ��s", "�x��", MB_ICONWARNING);
			return E_FAIL;
		}
	}

	// �l�b�g���[�N�̐ݒ�f�[�^�ǂݍ���
	CNetwork::LoadConfiguration();

	m_pNetwork = new CNetwork;

	// �l�b�g���[�N
	if (!m_pNetwork->Init() == S_OK)
	{
		m_pNetwork->Uninit();
		delete m_pNetwork;
		m_pNetwork = NULL;
		return E_FAIL;
	}

	CSceneX::Load();
	LoadTexScript();
	LoadSystemFile();

	LPDIRECT3DDEVICE9 pDevice;
	pDevice = m_pRenderer->GetDevice();

	m_pCamera = new CCamera;
	m_pCamera->Init();

	m_pLight = new CLight;																		//���C�g�̃N���G�C�g
	m_pLight->Init();

	m_pSound = new CSound;
	m_pSound->Init(hWnd);

	CTitle::LoadAsset();
	CPuzzle::LoadAsset();
	CGame::LoadAsset();
	CResult::LoadAsset();
	CCharacterSelect::LoadAsset();

	SetMode(MODE_TITLE);																		//���[�h�Z���N�g

	CEffect::LoadParticleScript();

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CManager::Uninit(void)
{
	// �e�N�X�`���̊J��
	TexRelease();

	// ���f���f�[�^�̊J��
	ModelRelease();

	// �V�F�[�_�[�f�[�^�̊J��
	ShaderRelease();

	// �L�[�{�[�h�̊J������
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();																	// �L�[�{�[�h�̏I������
		delete m_pInputKeyboard;																	// �L�[�{�[�h�̃��������
		m_pInputKeyboard = NULL;																	// �|�C���^��NULL�ɂ���
	}

	// �}�E�X�̊J������
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Uninit();																	// �}�E�X�̏I������
		delete m_pInputMouse;																		// �}�E�X�̃��������
		m_pInputMouse = NULL;																		// �|�C���^��NULL�ɂ���
	}

	// �R���g���[���[�̊J������
	if (m_pInputController != NULL)
	{
		m_pInputController->Uninit();																// �R���g���[���[�̏I������
		delete m_pInputController;																	// �R���g���[���[�̃��������
		m_pInputController = NULL;																	// �|�C���^��NULL�ɂ���
	}

	// �T�E���h�̊J������
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();																			// Sound�̏I������
		delete m_pSound;																			// Sound�̃��������
		m_pSound = NULL;																			// �|�C���^��NULL�ɂ���
	}

	// �J�����̊J������
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();																		// �J�����̏I������
		delete m_pCamera;																			// �J�����̃��������
		m_pCamera = NULL;																			// �|�C���^��NULL�ɂ���
	}

	// ���C�g�̊J������
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();																			// ���C�g�̏I������
		delete m_pLight;																			// ���C�g�̃��������
		m_pLight = NULL;																			// �|�C���^��NULL�ɂ���
	}

	// �^�C�g���̊J������
	if (m_pTitle != NULL)
	{
		m_pTitle->Uninit();																			// �^�C�g���̏I������
		delete m_pTitle;																			// �^�C�g���̃��������
		m_pTitle = NULL;																			// �|�C���^��NULL�ɂ���
	}

	// �p�Y���̊J������
	if (m_pPuzzle != NULL)
	{
		m_pPuzzle->Uninit();																		// �p�Y���̏I������
		delete m_pPuzzle;																			// �p�Y���̃��������
		m_pPuzzle = NULL;																			// �|�C���^��NULL�ɂ���
	}

	// �Q�[���̊J������
	if (m_pGame != NULL)
	{
		m_pGame->Uninit();																			// �Q�[���̏I������
		delete m_pGame;																				// �Q�[���̃��������
		m_pGame = NULL;																				// �|�C���^��NULL�ɂ���
	}

	// ���U���g�̊J������
	if (m_pResult != NULL)
	{
		m_pResult->Uninit();																		// ���U���g�̏I������
		delete m_pResult;																			// ���U���g�̃��������
		m_pResult = NULL;																			// �|�C���^��NULL�ɂ���
	}

	// �����L���O�̊J������
	if (m_pRanking != NULL)
	{
		m_pRanking->Uninit();																		// �����L���O�̏I������
		delete m_pRanking;																			// �����L���O�̃��������
		m_pRanking = NULL;																			// �|�C���^��NULL�ɂ���
	}

	// �L�����N�^�[�I���̊J������
	if (m_pCharacterSelect != NULL)
	{
		m_pCharacterSelect->Uninit();																// �L�����N�_�[�I���̏I������
		delete m_pCharacterSelect;																	// �L�����N�_�[�I���̃��������
		m_pCharacterSelect = NULL;																	// �|�C���^��NULL�ɂ���
	}

	// Scene�̉������
	CScene::ReleaseAll();

	// �����_���[�̊J������
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();																		// Renderer�̏I������
		delete m_pRenderer;																			// Renderer�̃��������
		m_pRenderer = NULL;																			// �|�C���^��NULL�ɂ���
	}
	// �l�b�g���[�N�̊J������
	if (m_pNetwork != NULL)
	{
		m_pNetwork->Uninit();
		delete m_pNetwork;
		m_pNetwork = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void)
{
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Update();
	}

	if (m_pInputController != NULL)
	{
		m_pInputController->Update();
	}

	switch (m_mode)
	{
	case CManager::MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Update();
		}
		break;
	case CManager::MODE_DEMO_PLAY:

		break;
	case CManager::MODE_CHARACTER_SELECT:
		if (m_pCharacterSelect != NULL)
		{
			m_pCharacterSelect->Update();
		}
		break;
	case CManager::MODE_STAGE_SELECT:

		break;
	case CManager::MODE_PUZZLE_CUSTOM:
		if (m_pPuzzle != NULL)
		{
			m_pPuzzle->Update();
		}
		break;
	case CManager::MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Update();
		}
		break;
	case CManager::MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Update();
		}
		break;
	case CManager::MODE_RANKING:
		if (m_pRanking != NULL)
		{
			m_pRanking->Update();
		}
		break;
	}

	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	if (m_pSound != NULL)
	{
		m_pSound->Update();
	}

#ifdef _DEBUG
	Debug();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
#ifdef _DEBUG
	ImGui::EndFrame();
#endif

	// �`��
	m_pRenderer->Draw();
}

//========================================================================================
// ���[�h�̐ݒ�
//========================================================================================
void CManager::SetMode(MODE mode)
{
	m_pSound->StopSound();
	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;
	case MODE_DEMO_PLAY:

		break;
	case MODE_CHARACTER_SELECT:
		if (m_pCharacterSelect != NULL)
		{
			m_pCharacterSelect->Uninit();
			delete m_pCharacterSelect;
			m_pCharacterSelect = NULL;
		}
		break;
	case MODE_STAGE_SELECT:

		break;
	case MODE_PUZZLE_CUSTOM:
		if (m_pPuzzle != NULL)
		{
			m_pPuzzle->Uninit();
			delete m_pPuzzle;
			m_pPuzzle = NULL;
		}
		break;
	case MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		break;
	case MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
		break;
	case MODE_RANKING:
		if (m_pRanking != NULL)
		{
			m_pRanking->Uninit();
			delete m_pRanking;
			m_pRanking = NULL;
		}
		break;
	}

	//���݂̃��[�h����
	m_mode = mode;

	switch (mode)
	{
	case MODE_TITLE:
		m_pTitle = new CTitle;
		m_pTitle->Init();
		m_pSound->PlaySoundA(SOUND_LABEL_BGM_TiTle);
		break;

	case MODE_DEMO_PLAY:

		break;
	case MODE_CHARACTER_SELECT:
		m_pCharacterSelect = new CCharacterSelect;
		m_pCharacterSelect->Init();
		m_pSound->PlaySoundA(SOUND_LABEL_BGM_Character_Select);
		break;
	case MODE_STAGE_SELECT:

		break;
	case MODE_PUZZLE_CUSTOM:
		m_pPuzzle = new CPuzzle;
		m_pPuzzle->Init();
		m_pSound->PlaySoundA(SOUND_LABEL_BGM_Puzzle);
		break;
	case MODE_GAME:
		m_pGame = new CGame;
		m_pGame->Init();
		m_pSound->PlaySoundA(SOUND_LABEL_BGM_Race);

		break;
	case MODE_RESULT:
		m_pResult = new CResult;
		m_pResult->Init();
		m_pSound->PlaySoundA(SOUND_LABEL_BGM_Result);

		break;
	case MODE_RANKING:
		m_pRanking = new CRanking;
		m_pRanking->Init();
		break;
	}
}

//=============================================================================
// �`���ϊ�(float)�֐�
//=============================================================================
void CManager::ConvertStringToFloat(char* text, char* delimiter, float* pResult)
{
	char* tp;
	char* ctx;
	int nTemp = 1;

	tp = strtok_s(text, delimiter, &ctx);
	pResult[0] = (float)atof(tp);

	while (tp = strtok_s(NULL, ",", &ctx))
	{
		pResult[nTemp++] = (float)atof(tp);
	}
}

//=============================================================================
// 2�_�Ԃ̋����Z�o����
//=============================================================================
float CManager::GetDistance(D3DXVECTOR3 FirstTarget, D3DXVECTOR3 SecondTarget)
{
	return ((FirstTarget.x - SecondTarget.x) * (FirstTarget.x - SecondTarget.x) +
		(FirstTarget.y - SecondTarget.y) * (FirstTarget.y - SecondTarget.y) +
		(FirstTarget.z - SecondTarget.z) * (FirstTarget.z - SecondTarget.z));
}

//=============================================================================
// Rand�֐�
//=============================================================================
int CManager::GetRand(int nValue)
{
	if (nValue <= 0) { return 0; }

	for (int nCount = 0; nCount < 5; nCount++)
	{
		rand();						//rand�̋���s
	}

	//�����_���Ȓl��Ԃ�
	int nAnswer = rand() % (nValue);

	int Wrok = ((int)rand() + 1) / ((int)RAND_MAX + 2);

	//float frand(unsigned v) {
	unsigned res = (1 >> 9) | 0x3f800000;
	float work = (*(float*)&res) - 1.0f;
	//}
	return nAnswer;
}

//=============================================================================
// �_�ƕ��ʂ̋��������߂�
//=============================================================================
float CManager::DistanceForDotAndPlane(const D3DXVECTOR3 &point, const D3DXVECTOR3 &planePoint, const D3DXVECTOR3 &planeNormal)
{
	// �_�ƕ��ʏ�̓_�̃x�N�g��(�_ - ���ʏ�̓_)
	D3DXVECTOR3 pointVector;
	pointVector = point - planePoint;

	// �@���Ɠ_�ƕ��ʂ̓_����ςŌv�Z
	return fabs(D3DXVec3Dot(&planeNormal, &pointVector));
}

//=============================================================================
// �C�[�W���O(Out)
//=============================================================================
float CManager::easeOut(float fValue)
{
	return fValue * (2 - fValue);
}

//=============================================================================
// �C�[�W���O(In)
//=============================================================================
float CManager::easeIn(float fTime, float fStart = 0.0f, float fDifference = 1.0f, float fTotal = 1.0f)
{
	fTime /= fTotal;
	return fDifference * fTime * fTime * fTime + fStart;
}

//=============================================================================
// �C�[�W���O(InAndOut)
//=============================================================================
float CManager::easeInAndOut(float fTime, float fStart, float fDifference, float fTotal)
{
	fTime /= fTotal / 2.0f;

	if (fTime < 1)
	{
		return fDifference / 2.0f * fTime * fTime * fTime + fStart;
	}

	fTime = fTime - 2;
	return fDifference / 2.0f * (fTime * fTime * fTime + 2) + fDifference;
}

//=============================================================================
// �w�肵���e�N�X�`�����g�����Ԃɂ��鏈��
//=============================================================================
HRESULT CManager::Load(std::string Add)
{
	std::map<std::string, LPDIRECT3DTEXTURE9>::const_iterator it = m_TexMap.find(Add);

	if (it == m_TexMap.end())
	{// ������Ȃ������Ƃ�
	 // ����������������L�[�Ƃ���
	 // �V�����}�b�v�Ƀe�N�X�`����ǉ�����
		CRenderer *pRenderer = m_pRenderer;
		LPDIRECT3DDEVICE9 pDevice;
		HRESULT hr;
		LPDIRECT3DTEXTURE9 tex = NULL;

		//�f�o�C�X���擾����
		pDevice = pRenderer->GetDevice();

		hr = D3DXCreateTextureFromFile(pDevice, Add.c_str(), &tex);


		if (FAILED(hr))
		{// �����ł��Ȃ������Ƃ���NULL��Ԃ�
			MessageBox(NULL, Add.c_str(), "Load : LOAD ERROR", MB_OK);
			return hr;			// ���ʂ�Ԃ�
		}

		// �}�b�v�֑}������
		m_TexMap.insert(std::map<std::string, LPDIRECT3DTEXTURE9>::value_type(Add, tex));
		return hr;				// ���ʂ�Ԃ�
	}

	return E_FAIL;
}

//=============================================================================
// �w�肵���e�N�X�`�������炤����
//=============================================================================
LPDIRECT3DTEXTURE9 CManager::GetResource(std::string Add)
{
	std::map<std::string, LPDIRECT3DTEXTURE9>::const_iterator it = m_TexMap.find(Add);

	if (it == m_TexMap.end())
	{// ������Ȃ������Ƃ�
	 // ����������������L�[�Ƃ���
	 // �V�����}�b�v�Ƀe�N�X�`����ǉ�����
		CRenderer *pRenderer = m_pRenderer;
		LPDIRECT3DDEVICE9 pDevice;
		HRESULT hr;
		LPDIRECT3DTEXTURE9 tex = NULL;

		//�f�o�C�X���擾����
		pDevice = pRenderer->GetDevice();

		hr = D3DXCreateTextureFromFile(pDevice, Add.c_str(), &tex);


		if (FAILED(hr))
		{// �����ł��Ȃ������Ƃ���NULL��Ԃ�
			MessageBox(NULL, Add.c_str(), "GetResource : LOAD ERROR", MB_OK);
			return NULL;
		}

		// �}�b�v�֑}������
		m_TexMap.insert(std::map<std::string, LPDIRECT3DTEXTURE9>::value_type(Add, tex));
		return tex;
	}
	else
	{
		return it->second;
	}
}

//=============================================================================
// �w�肵�����f������ǂݍ��ޏ���
//=============================================================================
HRESULT CManager::LoadModel(std::string Add)
{
	std::map<std::string, MODEL_INFO>::const_iterator it = m_ModelMap.find(Add);

	if (it == m_ModelMap.end())
	{// ������Ȃ������Ƃ�
	 // ����������������L�[�Ƃ���
	 // �V�����}�b�v�Ƀe�N�X�`����ǉ�����
		CRenderer *pRenderer = m_pRenderer;
		LPDIRECT3DDEVICE9 pDevice;
		HRESULT hr;
		MODEL_INFO info = {};

		//�f�o�C�X���擾����
		pDevice = pRenderer->GetDevice();

		hr = D3DXLoadMeshFromX(Add.c_str(), D3DXMESH_SYSTEMMEM, pDevice, NULL, &info.pBuffMat, NULL, &info.nNumMat, &info.pMesh);

		if (FAILED(hr))
		{// �����ł��Ȃ������Ƃ���NULL��Ԃ�
			MessageBox(NULL, Add.c_str(), "LoadModel : LOAD ERROR", MB_OK);
			return hr;			// ���ʂ�Ԃ�
		}

		// �}�b�v�֑}������
		m_ModelMap.insert(std::map<std::string, MODEL_INFO>::value_type(Add, info));
		return hr;				// ���ʂ�Ԃ�
	}

	return S_OK;
}

//=============================================================================
// �w�肵�����f�������炤����
//=============================================================================
bool CManager::GetModelResource(std::string Add, LPD3DXBUFFER &pBuffMat, DWORD &nNumMat, LPD3DXMESH &pMesh)
{
	std::map<std::string, MODEL_INFO>::const_iterator it = m_ModelMap.find(Add);

	if (it == m_ModelMap.end())
	{// ������Ȃ������Ƃ�
	 // ����������������L�[�Ƃ���
	 // �V�����}�b�v�Ƀe�N�X�`����ǉ�����
		CRenderer *pRenderer = m_pRenderer;
		LPDIRECT3DDEVICE9 pDevice;
		HRESULT hr;
		MODEL_INFO info = {};

		//�f�o�C�X���擾����
		pDevice = pRenderer->GetDevice();

		const char* add = Add.c_str();

		hr = D3DXLoadMeshFromX(Add.c_str(), D3DXMESH_SYSTEMMEM, pDevice, NULL, &info.pBuffMat, NULL, &info.nNumMat, &info.pMesh);

		if (FAILED(hr))
		{// �����ł��Ȃ������Ƃ���NULL��Ԃ�
			MessageBox(NULL, Add.c_str(), "GetModelResource : LOAD ERROR", MB_OK);
			pBuffMat = NULL;
			nNumMat = NULL;
			pMesh = NULL;
			return false;
		}

		D3DXMATERIAL	*pMat;							//���݂̃}�e���A���ۑ��p
		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)info.pBuffMat->GetBufferPointer();

		// �}�e���A���̃e�N�X�`���ǂݍ���
		for (int nCntMat = 0; nCntMat < (int)info.nNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{// �e�N�X�`�����������Ƃ�
				char *pFileName = NULL;
				pFileName = pMat[nCntMat].pTextureFilename;			// �e�N�X�`���̃A�h���X���擾
				CManager::Load(pFileName);		// �e�N�X�`���̓ǂݍ���
			}
		}


		// �}�b�v�֑}������
		m_ModelMap.insert(std::map<std::string, MODEL_INFO>::value_type(Add, info));
		pBuffMat = info.pBuffMat;
		nNumMat = info.nNumMat;
		pMesh = info.pMesh;
		return true;
	}
	else
	{
		pBuffMat = it->second.pBuffMat;
		nNumMat = it->second.nNumMat;
		pMesh = it->second.pMesh;
		return true;
	}
}

//=============================================================================
// �w�肵�����f������ǂݍ��ޏ���
//=============================================================================
HRESULT CManager::LoadShader(std::string Add)
{
	std::map<std::string, LPD3DXEFFECT>::const_iterator it = m_ShaderMap.find(Add);

	if (it == m_ShaderMap.end())
	{// ������Ȃ������Ƃ�
	 // ����������������L�[�Ƃ���
	 // �V�����}�b�v�ɃV�F�[�_�[��ǉ�����
		CRenderer *pRenderer = m_pRenderer;
		LPDIRECT3DDEVICE9 pDevice;
		HRESULT hr;
		LPD3DXEFFECT shader = NULL;

		//�f�o�C�X���擾����
		pDevice = pRenderer->GetDevice();

		hr = D3DXCreateEffectFromFile(pDevice, Add.c_str(), NULL, NULL, 0, NULL, &shader, NULL);

		if (FAILED(hr))
		{// �����ł��Ȃ������Ƃ���NULL��Ԃ�
			MessageBox(NULL, Add.c_str(), "LoadShader : LOAD ERROR", MB_OK);
			return hr;			// ���ʂ�Ԃ�
		}

		// �}�b�v�֑}������
		m_ShaderMap.insert(std::map<std::string, LPD3DXEFFECT>::value_type(Add, shader));
		return hr;				// ���ʂ�Ԃ�
	}

	return S_OK;
}

//=============================================================================
// �w�肵�����f�������擾���鏈��
//=============================================================================
LPD3DXEFFECT CManager::GetShaderResource(std::string Add)
{
	std::map<std::string, LPD3DXEFFECT>::const_iterator it = m_ShaderMap.find(Add);

	if (it == m_ShaderMap.end())
	{// ������Ȃ������Ƃ�
	 // ����������������L�[�Ƃ���
	 // �V�����}�b�v�Ƀe�N�X�`����ǉ�����
		CRenderer *pRenderer = m_pRenderer;
		LPDIRECT3DDEVICE9 pDevice;
		HRESULT hr;
		LPD3DXEFFECT shader = NULL;

		//�f�o�C�X���擾����
		pDevice = pRenderer->GetDevice();

		hr = D3DXCreateEffectFromFile(pDevice, Add.c_str(), NULL, NULL, 0, NULL, &shader, NULL);

		if (FAILED(hr))
		{// �����ł��Ȃ������Ƃ���NULL��Ԃ�
			MessageBox(NULL, Add.c_str(), "GetShaderResource : LOAD ERROR", MB_OK);
			return NULL;
		}

		// �}�b�v�֑}������
		m_ShaderMap.insert(std::map<std::string, LPD3DXEFFECT>::value_type(Add, shader));
		return shader;
	}
	else
	{
		return it->second;
	}
}

//=============================================================================
// �ǂ���x�N�g��
//=============================================================================
D3DXVECTOR3 *CManager::calcWallScratchVector(D3DXVECTOR3 *out, const D3DXVECTOR3 &front, const D3DXVECTOR3 &normal)
{
	D3DXVECTOR3 normal_n;
	D3DXVec3Normalize(&normal_n, &normal);
	return D3DXVec3Normalize(out, &(front - D3DXVec3Dot(&front, &normal_n) * normal_n));
}

//=============================================================================
// ���˃x�N�g��
//=============================================================================
D3DXVECTOR3 *CManager::calcReflectVector(D3DXVECTOR3 *out, const D3DXVECTOR3 &front, const D3DXVECTOR3 &normal)
{
	D3DXVECTOR3 normal_n;
	D3DXVec3Normalize(&normal_n, &normal);
	return D3DXVec3Normalize(out, &(front - 2.0f * D3DXVec3Dot(&front, &normal_n) * normal_n));
}

//=============================================================================
// �����̌��������߂鏈��
//=============================================================================
int CManager::LengthCalculation(int nValue)
{
	int nLength = 1;

	while (nValue /= 10)
	{
		nLength++;
	}

	return nLength;
}

//=============================================================================
// �E�B���h�E�̒��S�ʒu�Ń}�E�X�J�[�\���̈ʒu���擾
//=============================================================================
D3DXVECTOR3 CManager::GetCursorPosWithCenter(void)
{
	//�}�E�X���W���X�N���[���̒��S�����_�ɂȂ�悤�ɕ␳�i���K���j
	D3DXMATRIX mProj;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	// �����_���[�����݂��Ă��Ȃ��Ƃ�
	if (pRenderer == NULL) return pos;

	// �}�E�X�����݂��Ă��Ȃ��Ƃ�
	if (m_pInputMouse == NULL) return pos;

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	pDevice->GetTransform(D3DTS_PROJECTION, &mProj);					// �v���W�F�N�V�����}�g���b�N�X�̎擾
	pos.x = (((2.0f * m_pInputMouse->GetMouseX()) / (float)SCREEN_WIDTH) - 1) / mProj._11;
	pos.y = -(((2.0f * m_pInputMouse->GetMouseY()) / (float)SCREEN_HEIGHT) - 1) / mProj._22;
	pos.z = 1.0f;

	return pos;
}

//
//D3DXVECTOR3 Slip(D3DXVECTOR3 L,D3DXVECTOR3 N)
// L:���˃x�N�g���i���C�j N:�|���S���̖@��
D3DXVECTOR3 CManager::Slip(D3DXVECTOR3 L, D3DXVECTOR3 N)
{
	D3DXVECTOR3 S; //����x�N�g���i��������j

	//����x�N�g�� S=L-(N * L)/(|N|^2)*N
	S = L - ((D3DXVec3Dot(&N, &L)) / (pow(D3DXVec3Length(&N), 2))) * N;

	return S;
}

//=============================================================================
// ���[���h�}�g���b�N�X�̍쐬
//=============================================================================
D3DXMATRIX CManager::CreateMtxWorld(D3DXVECTOR3 &pos, D3DXVECTOR3 &rot)
{
	D3DXMATRIX	mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtx;							// �}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtx);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	return  mtx;
}

//=============================================================================
// �V�X�e���ݒ�t�@�C���Ǎ�
//=============================================================================
void CManager::LoadSystemFile(void)
{
	FILE *pFile;																	// �t�@�C��
	char cReadText[128];															// ����
	char cHeadText[128];															// ��r
	char cDie[128];

	pFile = fopen("system.ini", "r");											// �t�@�C�����J���܂��͍��

	if (pFile != NULL)
	{
		//�X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}

		// �X�N���v�g��������
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// �G���h�X�N���v�g������܂�
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);

				//���s
				if (strcmp(cReadText, "\n") != 0)
				{
					if (strcmp(cHeadText, "SPEED_ROT") == 0)
					{// ���񑬓x
						sscanf(cReadText, "%s %s %f", &cDie, &cDie,
							&m_fTurningVelocity);
					}
					else if (strcmp(cHeadText, "SPEED_DOWN") == 0)
					{//�p�[�c�����̂Ƃ�
						sscanf(cReadText, "%s %s %f", &cDie, &cDie,
							&m_fSpeedDampingRate);
					}
				}
			}
		}
		fclose(pFile);				// �t�@�C�������

#ifdef _DEBUG
		MessageBox(NULL, "�V�X�e�����̓Ǎ��ɐ����I", "SUCCESS", MB_ICONASTERISK);		// ���b�Z�[�W�{�b�N�X�̐���
#endif
	}
	else
	{
		MessageBox(NULL, "�V�X�e�����̃A�N�Z�X���s�I", "WARNING", MB_ICONWARNING);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}

//=============================================================================
// �V�X�e���ݒ�t�@�C�����o
//=============================================================================
void CManager::SaveSystemFile(void)
{
	CWrite *pWrite = new CWrite;
	if (pWrite == NULL) return;

	//�ϐ��錾
	char text[64];				// �e�L�X�g
	char cEqual[8] = { "=" };	//�C�R�[���p
	CScene *pSceneNext = NULL;	//����A�b�v�f�[�g�Ώ�
	CScene *pSceneNow = NULL;

	//�J����
	if (pWrite->Open("system.ini"))
	{
		strcpy(text, "# �V�X�e���f�[�^�X�N���v�g\n");
		strcat(text, "# Author : masayasu wakita\n");

		pWrite->TitleWrite(text);			// �^�C�g���̌`���ŏ�������
		pWrite->Write("SCRIPT\n");			// �X�N���v�g�̏I���錾
		pWrite->Write("\n");

		// �p�����[�^�̏�� //
		pWrite->IndexWrite("�e��p�����[�^");

		pWrite->Write("# ���񑬓x\n");
		pWrite->Write("SPEED_ROT = %.2f\n", m_fTurningVelocity);

		pWrite->Write("# �X�s�[�h�̌�����\n");
		pWrite->Write("SPEED_DOWN = %.2f\n", m_fSpeedDampingRate);

		pWrite->Write("END_SCRIPT\n");			// �X�N���v�g�̏I���錾
		pWrite->End();

		MessageBox(NULL, "�V�X�e�����̏������ݏI�����܂����I", "WARNING", MB_ICONINFORMATION);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}

//=============================================================================
// �e�N�X�`���̊J��
//=============================================================================
void CManager::TexRelease(void)
{
	for (auto itr = m_TexMap.begin(); itr != m_TexMap.end(); itr++)
	{
		// �e�N�X�`���̊J��
		if (itr->second != NULL)
		{
			itr->second->Release();
			itr->second = NULL;
		}
	}

	m_TexMap.clear();
}

//=============================================================================
// ���f���f�[�^�̊J��
//=============================================================================
void CManager::ModelRelease(void)
{
	for (auto itr = m_ModelMap.begin(); itr != m_ModelMap.end(); itr++)
	{
		// �}�e���A���̊J��
		if (itr->second.pBuffMat != NULL)
		{
			itr->second.pBuffMat->Release();
			itr->second.pBuffMat = NULL;
		}

		// ���b�V�����̊J��
		if (itr->second.pMesh != NULL)
		{
			itr->second.pMesh->Release();
			itr->second.pMesh = NULL;
		}
	}

	m_ModelMap.clear();
}

//=============================================================================
// �V�F�[�_�[�̊J��
//=============================================================================
void CManager::ShaderRelease(void)
{
	for (auto itr = m_ShaderMap.begin(); itr != m_ShaderMap.end(); itr++)
	{
		// �e�N�X�`���̊J��
		if (itr->second != NULL)
		{
			itr->second->Release();
			itr->second = NULL;
		}
	}

	m_ShaderMap.clear();
}

//=============================================================================
// ���������̋�؂育�ƂɎ擾����(1�s����)
//=============================================================================
std::vector<std::string> CManager::split(
	std::string& input,		// 1�s�̃X�g���[��
	char delimiter		// ��؂蕶��
)
{
	// �ϐ��錾
	std::istringstream iss_Line(input);	// ������X�g���[��
	std::string sRead;					// ������ǂݍ��ݗp
	std::vector<std::string> vec_Result;		// 1�s�ǂݍ��ݗp

	// �w�肵����؂肪������胋�[�v����
	while (std::getline(iss_Line, sRead, delimiter))
	{
		// �ǂݎ������������i�[����
		vec_Result.push_back(sRead);
	}
	// �m�ۂ��������񕪕Ԃ�
	return vec_Result;
}

//=============================================================================
// �e�N�X�`���A�Z�b�g�̃��[�h����
//=============================================================================
void CManager::LoadTexScript(void)
{
	FILE *pFile;
	char cReadText[128];		//����
	char cHeadText[128];		//��r
	char cDie[128];
	int nCntPointer = 0;		//�|�C���^�[�̐��l

	char sAdd[64];				//���f���̃A�h���X
	std::string Add;

	int nCntMotion = 0;			//�Q�Ƃ���|�C���^�̒l��������
	int nCntKey = 0;

	int nMaxModel = 0;

	//�e�L�X�g�f�[�^���[�h
	pFile = fopen(MANAGER_TEX, "r");

	if (pFile != NULL)
	{
		//�|�C���^�[�̃��Z�b�g
		nCntPointer = 0;

		//�X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}

		//�X�N���v�g��������
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			//�G���h�X�N���v�g������܂�
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);

				//���s
				if (strcmp(cReadText, "\n") != 0)
				{
					if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
					{//�p�[�c���f���̃A�h���X���̂Ƃ�
						sscanf(cReadText, "%s %s %s", &cDie, &cDie, &sAdd[0]);						//�A�h���X�̎擾
						Add = sAdd;
						CManager::Load(Add);
					}
				}
			}
		}

		//�t�@�C����
		fclose(pFile);
	}
	else
	{
		MessageBox(NULL, "�e�N�X�`���}�l�[�W�����J���܂���ł����I", "WARNING", MB_ICONWARNING);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}



#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CManager::Debug(void)
{
	ImGui::Begin("Manager", NULL, ImGuiWindowFlags_MenuBar);   // �f�o�b�O�E�B���h�E����

	if (ImGui::BeginMenuBar())
	{// ���j���[�o�[�̐���
		if (ImGui::BeginMenu("File"))
		{// �t�@�C���^�u�̐���
			if (ImGui::MenuItem(u8"�ۑ�"))
			{// �ۑ�����
				SaveSystemFile();
			}
			if (ImGui::MenuItem(u8"�Ǎ�"))
			{// �ǂݍ��ݏ���
				LoadSystemFile();
			}

			ImGui::EndMenu();			// ���j���[�̍X�V�I��
		}

		ImGui::EndMenuBar();		// ���j���[�o�[�̍X�V�I��
	}

	ImGui::DragFloat(u8"���񑬓x", &m_fTurningVelocity);
	ImGui::DragFloat(u8"�X�s�[�h�̌�����", &m_fSpeedDampingRate);

	//�f�o�b�O�������I��
	ImGui::End();
}
#endif