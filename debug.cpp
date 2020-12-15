//=============================================================================
//
// �f�o�b�O���� [debug.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "inputMouse.h"
#include "camera.h"
#include "scene.h"
#include "circle.h"
#include "inputKeyboard.h"
#include "meshField.h"
#include "game.h"
#include "object.h"
#include "sceneX.h"
#include "collider.h"
#include "enemy.h"
#include "inputMouse.h"
#include "effect.h"
#include "player.h"
#include "wall.h"
#include "ui.h"
#include "effect.h"
#include "write.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define UIMANAGER_SCRIPT "data/text/manager/manager_ui.txt"
#define CONTINUE_MAX 10													// �Čv�Z��

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
char CDebugProc::m_aStr[1024] = {};										// ������
D3DXVECTOR3 CDebugProc::m_mouseOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �}�E�X�̑O�̃��[���h���W
CCircle	*CDebugProc::m_pCircle = NULL;									// �~�̃|�C���^
CObject	*CDebugProc::m_pSample = NULL;									// �؂̃|�C���^
CScene *CDebugProc::m_pSelect = NULL;									// �I�������V�[���̃|�C���^
CEnemy *CDebugProc::m_pEnemy = NULL;									// �G�̃|�C���^
float CDebugProc::m_fPaintSize = 1;										// �n�`�ҏW�u���V�̑傫��
int CDebugProc::m_nCreateIndex = 0;										// 1�t���[���̐�����
bool CDebugProc::m_bDebug = false;										// �f�o�b�O���[�h�̐ؑ�
bool CDebugProc::m_bInspectorWind = false;								// �C���X�y�N�^�[�E�B���h�E�̕\���ؑ�
bool CDebugProc::m_bMouseCursorPosition = false;						// �}�E�X�J�[�\�����W�̎g�p�t���O
int CDebugProc::m_nCntGeneration = 0;									// ������
int	CDebugProc::m_nMode = 0;											// ���[�h�I��
int CDebugProc::m_nParticleShape = PARTICLESHAPE_CONE;					// �p�[�e�B�N���`��
int	CDebugProc::m_nCntContinue = 0;										// �Ċm�F��
D3DXVECTOR2 CDebugProc::m_CreateRand = D3DXVECTOR2(0.0f, 0.0f);			// ���̗�
D3DXVECTOR2 CDebugProc::m_CreateRandOld = D3DXVECTOR2(0.0f, 0.0f);		// ���̗�
CMeshField *CDebugProc::m_apMeshField[FLOOR_LIMIT * FLOOR_LIMIT] = {};
D3DXVECTOR3 CDebugProc::m_createPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
float CDebugProc::m_fSliderPow = 1.0f;

std::string CDebugProc::m_currentModel = {};		// ���f���̃A�h���X
std::string CDebugProc::m_currentTexture = {};		// �e�N�X�`���̃A�h���X
std::string CDebugProc::m_currentUi = {};			// UI�̃A�h���X

bool CDebugProc::m_bHeightCalculation = false;
HWND CDebugProc::m_hWnd = NULL;								// �E�B���h�E�n���h��

// �G�t�F�N�g�쐬�֘A
int CDebugProc::m_particleLife = 0;							// �p�[�e�B�N���̐�������
int CDebugProc::m_nCreate = 0;								// ������
int CDebugProc::m_nInterval = 0;							// �C���^�[�o��
int CDebugProc::m_nEmissionType = 0;

float CDebugProc::m_fStartRadius = 0.0f;					// �n�܂��
float CDebugProc::m_fRadius = 0.0f;							// ���a
float CDebugProc::m_fMinSpeed = 0.0f;						// �Œ�X�s�[�h
float CDebugProc::m_fSpeed = 0.0f;							// �X�s�[�h
float CDebugProc::m_fResistance = 0.0f;						// ��R�l
float CDebugProc::m_fGravity = 0.0f;

D3DXVECTOR3	CDebugProc::m_createRot = D3DXVECTOR3_ZERO;		// ��]��
D3DXVECTOR3 CDebugProc::m_size = D3DXVECTOR3_ZERO;			// �T�C�Y
D3DXVECTOR3 CDebugProc::m_moveSize = D3DXVECTOR3_ZERO;		// �傫���̕ω���
D3DXVECTOR3 CDebugProc::m_moveRot = D3DXVECTOR3_ZERO;		// ��]�̕ω���
D3DXVECTOR3 CDebugProc::m_centerPos = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// ���S�ʒu
D3DXVECTOR2 CDebugProc::m_sprite = D3DXVECTOR2(1.0f, 1.0f);	// ������
D3DXCOLOR	CDebugProc::m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
D3DXCOLOR	CDebugProc::m_moveCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

float CDebugProc::m_fAngle = 0.0f;							// �p�x
float CDebugProc::m_fDistance = 0.0f;						// ����
float CDebugProc::m_fRotationSpeed = 0.0f;					// ��]���x
float CDebugProc::m_fMaxSpeed = 0.0;						// �X�s�[�h�̍ő�l

bool CDebugProc::m_bLoop = false;							// �������J��Ԃ�
bool CDebugProc::m_bRandomSpeed = false;					// �X�s�[�h�����_�����̗L��
bool CDebugProc::m_bAlpha = false;							// �A���t�@�u�����f�B���O�̗L��
bool CDebugProc::m_bZBuffer = false;						// Z�o�b�t�@�̗L��
bool CDebugProc::m_bFadeOut = false;						// �t�F�[�h�A�E�g�̗L��
bool CDebugProc::m_bBillboard = true;						// �r���{�[�h�̗L��
bool CDebugProc::m_bRandAngle = false;						// �p�x�̃����_��

char CDebugProc::m_effectTag[NAME_SIZE] = {};				// �^�O�̏�����

CUi *CDebugProc::m_pCreateUi = NULL;						// UI�̏�����
char CDebugProc::m_CreateName[NAME_SIZE] = {};				// ������
std::vector<std::string> CDebugProc::m_AddUi = {};			// �A�h���X�̔z��(UI)


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDebugProc::CDebugProc()
{
	m_pSelect = NULL;
	m_bInspectorWind = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDebugProc::~CDebugProc()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CDebugProc::Init(HWND hWnd)
{
	CRenderer *Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = Renderer->GetDevice();

	// �E�B���h�E�n���h����ۊ�
	m_hWnd = hWnd;

	// �f�o�b�O�\���p�t�H���g��ݒ�
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "�l�r �S�V�b�N", &m_pFont);

	//IMGUI�̏�����
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.Fonts->AddFontFromFileTTF("data\\fonts\\Meiryo.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);

	// �f�o�b�O�p���|�C���^�̏�����
	for (int nCount = 0; nCount < FLOOR_LIMIT * FLOOR_LIMIT; nCount++)
	{
		m_apMeshField[nCount] = NULL;
	}

	// �~�̍쐬
	m_pCircle = CCircle::Create();

	LoadAddWithUI();
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CDebugProc::Uninit(void)
{
	if (m_pCircle != NULL)
	{// �~�̃|�C���^���J������Ă��Ȃ��Ƃ�
		m_pCircle->Uninit();														// �J������
		delete m_pCircle;															// �~�̊J��
		m_pCircle = NULL;															// �~�̃|�C���^��NULL��
	}

	// ���|�C���^�̊J������
	for (int nCount = 0; nCount < FLOOR_LIMIT * FLOOR_LIMIT; nCount++)
	{
		if (m_apMeshField[nCount] != NULL)
		{// �f�o�b�O�p�̏������݂��Ă����Ƃ�
			m_apMeshField[nCount] = NULL;
		}
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (m_pFont != NULL)
	{// �f�o�b�O�\���p�t�H���g�̊J��
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CDebugProc::Update(void)
{
	// �t���[���J�n
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

#ifdef _DEBUG
	// �����ō쐬�����ȒP�ȃE�B���h�E��\�����܂��B Begin / End�y�A���g�p���āA���O�t���E�B���h�E���쐬���܂��B
	ImGui::Begin("System");                          // �uSystem�v�Ƃ����E�B���h�E���쐬���܂�����ɒǉ����܂��B

	ImGui::SameLine();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::Text("rand = %d", CManager::GetRand(10));								// �v���C���[�̌��݈ʒu��\��

	if (m_pSelect != NULL)
	{
		if (m_bInspectorWind)
		{
			ShowInspector();
		}
	}

	CInputMouse *pMouse = CManager::GetInputMouse();
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	if (pKeyboard->GetPressKeyboard(DIK_F1))
	{
		if (pMouse->GetTriggerMouse(MOUSE_LEFT))
		{
			SelectModel();
		}
	}

	ImGui::End();
#endif

	Debug();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CDebugProc::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nTime = 0;

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// �~�̕`��
	if (m_bDebug && m_nMode != DEBUGMODE_INDIVIDUAL)
	{
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// ����(�����)���J�����O����
		m_pCircle->Draw();
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ����(�����)���J�����O����
	}

	//ImGui�̕`��
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	// �e�L�X�g�`��
	m_pFont->DrawText(NULL, &m_aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	*m_aStr = NULL;
}

//=============================================================================
// ���O�\������
//=============================================================================
void CDebugProc::Log(char* frm, ...)
{
	va_list args;			// ���X�g�̎擾
	char* c = "\n";			// ���s�p

	va_start(args, frm);		// ���X�g�̐擪���擾

	vsprintf(&m_aStr[strlen(m_aStr)], frm, args);		// �����ɓ��Ă͂߂�

	va_end(args);						// ���X�g���J������
}

//=============================================================================
// �f�o�b�O����
//=============================================================================
void CDebugProc::SelectModel(void)
{
	CScene *pSceneNext = NULL;														//����A�b�v�f�[�g�Ώ�
	CScene *pSceneNow = NULL;
	CScene *pSelect = NULL;
	float fDistance = 10000000.0f;

	//for (int nCount = 0; nCount < CScene::PRIORITY_MODEL; nCount++)
	{
		// tree�̃I�u�W�F�N�g�̃|�W�V�������Q��
		pSceneNow = CScene::GetScene(CScene::PRIORITY_MODEL);

		//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
		while (pSceneNow != NULL)
		{
			pSceneNext = CScene::GetSceneNext(pSceneNow, (CScene::PRIORITY_MODEL));							//����A�b�v�f�[�g�Ώۂ��T����
			CObject *pObj = (CObject*)pSceneNow;

			D3DXMATRIX mWorldView;
			D3DXMATRIX m;
			D3DXVECTOR3 vRayDir;
			D3DXVECTOR3 vStartPoint;
			D3DXVECTOR3 v = CManager::GetCursorPosWithCenter();
			D3DXMATRIX mtxWorld, view;
			D3DXVECTOR3 pos = pObj->GetPosition();

			CRenderer *pRenderer = CManager::GetRenderer();
			LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾
			D3DXMatrixTranslation(&mtxWorld, pos.x, pos.y, pos.z);

			pDevice->GetTransform(D3DTS_VIEW, &view);
			mWorldView = mtxWorld * view;
			D3DXMatrixInverse(&m, NULL, &mWorldView);

			//��RayDir�̓��C���΂������x�N�g��
			vRayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
			vRayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
			vRayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;

			//vStartPoint�̓X�N���[����̂QD���W�i�N���b�N�ʒu�j��3D��ԍ��W�ɕϊ���������
			vStartPoint.x = m._41;
			vStartPoint.y = m._42;
			vStartPoint.z = m._43;

			//���C��������
			BOOL bHit;
			FLOAT fDist;
			LPD3DXMESH mesh = pObj->GetMesh();
			D3DXIntersect(mesh, &vStartPoint, &vRayDir, &bHit, NULL, NULL, NULL, &fDist, NULL, NULL);

			if (bHit)
			{
				float fWork = CManager::GetDistance(vStartPoint, pObj->GetPosition());
				if (fDistance > fWork)
				{
					// �I�������I�u�W�F�N�g���i�[
					m_pSelect = pObj;
					m_bInspectorWind = true;
					fDistance = fWork;
				}
			}

			pSceneNow = pSceneNext;													//����A�b�v�f�[�g�Ώۂ��i�[
		}
	}
}

//=============================================================================
// �f�o�b�O����
//=============================================================================
void CDebugProc::Debug(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();		// �L�[�{�[�h�̎擾
	CCamera *pCamera = CManager::GetCamera();		// �J���� �̎擾

#ifdef _DEBUG
	ImGui::Begin("System");			// System�E�B���h�E �̐����܂��̓A�N�Z�X

	//ImGui::BeginChild("Scrolling");
	//for (int n = 0; n < 50; n++)
	//	ImGui::Text("%04d: Some text", n);
	//ImGui::EndChild();

	//const char* listbox_items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
	//static int listbox_item_current = 1;
	//ImGui::ListBox("listbox\n(single select)", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4);

	if (ImGui::Checkbox("DebugMode", &m_bDebug))		// �n�`�ҏW���[�h�؂�ւ�
	{
		for (int nCount = 0; nCount < FLOOR_LIMIT * FLOOR_LIMIT; nCount++)
		{
			if (m_apMeshField[nCount] == NULL)
			{// �������݂��Ă����Ƃ�
				m_apMeshField[nCount] = CMeshField::Create();
			}

			if (m_apMeshField[nCount] != NULL)
			{
				if (m_apMeshField[nCount]->GetActive())
				{// �\�����Ă����Ƃ�
					m_apMeshField[nCount]->SetDebugRand(true);
					m_apMeshField[nCount]->SetActive(false);
				}
			}
		}

		// �I�u�W�F�N�g�}�b�v
		std::map<std::string, MODEL_INFO> modelMap = CManager::GetModelMap();
		auto itr = modelMap.begin();
		m_currentModel = itr->first.c_str();
	}

	//�f�o�b�O�������I��
	ImGui::End();
#endif

	// �f�o�b�O�E�B���h�E�̐���
	if (m_bDebug)
	{
		D3DXVECTOR3 worldPos = D3DXVECTOR3_ZERO;

		if (pCamera != NULL)
		{
			worldPos = pCamera->GetWorldPos();			// �}�E�X�̃��[���h���W���擾
		}

		m_pCircle->SetPosition(worldPos);						// �����͈͂̈ʒu��ݒ�
		m_pCircle->SetRadius(m_fPaintSize);						// �����͈͂̑傫����ݒ�
		m_pCircle->MakeVertex();								// �~�̒��_����

		ImGui::Begin("Debug", &m_bDebug, ImGuiWindowFlags_MenuBar);   // �f�o�b�O�E�B���h�E����

		CRenderer *pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾

		pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

		if (ImGui::BeginMenuBar())
		{// ���j���[�o�[�̐���
			MenuBar();					// ���j���[�o�[����
			ImGui::EndMenuBar();		// ���j���[�o�[�̍X�V�I��
		}

		ImGui::Text("[R]Press is Create");			// �f�o�b�O���[�h�e�L�X�g�\��

		ImGui::BeginTabBar("General");
		TabBar(worldPos);				// �^�u����
		ImGui::EndTabBar();				// �^�u�̏I������

		if (pKeyboard->GetTriggerKeyboard(DIK_RCONTROL))
		{
			m_nMode = DEBUGMODE_NONE;
		}

		ImGui::Text("Rand DebugMode");			// �f�o�b�O���[�h�e�L�X�g�\��

		CDebugProc::Log("�}�E�X�̃��[���h���W : %f, %f, %f\n",			// �}�E�X�̃��[���h���W���o��
			worldPos.x,
			worldPos.y,
			worldPos.z);

		if (m_nMode != DEBUGMODE_INDIVIDUAL)
		{// �X�z�u���[�h����Ȃ��Ƃ�
			ImGui::SliderFloat("paintSize", &m_fPaintSize, 1.0f, 10000.0f);			// �X���C�_�[���g�p����1�̃t���[�g��ҏW���܂�
		}

		if (m_nMode != DEBUGMODE_RAND && m_nMode != DEBUGMODE_UI)
		{// �n�`�ҏW���[�h����Ȃ��Ƃ�
			SelectAssetWithModel();
		}

		if (m_nMode == DEBUGMODE_UI)
		{// UI�쐬���[�h�̂Ƃ�
		}

		if (m_nMode != DEBUGMODE_INDIVIDUAL)
		{// �X�z�u���[�h�ł͂Ȃ������Ƃ�
			if (m_pSample != NULL)
			{// �|�C���^�����݂��Ă����Ƃ�
				m_pSample->Release();				// �J���\��
				m_pSample = NULL;
			}
		}

		if (m_nMode != DEBUGMODE_UI)
		{// UI�������[�h�ł͂Ȃ������Ƃ�
			if (m_pCreateUi != NULL)
			{// UI�����݂��Ă����Ƃ�
				m_pCreateUi->Uninit();				// �J������
				delete m_pCreateUi;					// �폜
				m_pCreateUi = NULL;					// NULL����
			}
		}

		if (m_nMode != DEBUGMODE_ENEMY)
		{// �G�l�~�[�z�u���[�h�ł͂Ȃ������Ƃ�
			if (m_pEnemy != NULL)
			{// �|�C���^�����݂��Ă����Ƃ�
				m_pEnemy->Release();
				m_pEnemy = NULL;
			}
		}

		if (ImGui::Button("Reset"))
		{// Reset �{�^���������ꂽ�Ƃ�
			CScene::ResetFloor();					// ���̍�������Z�b�g
		}

		if (m_nMode == DEBUGMODE_NONE)
		{// �������Ȃ����[�h�������Ƃ�
			ImGui::Text("None");

			// �f�o�b�O�I���{�^��
			if (ImGui::Button("Debug End"))
			{
				m_bDebug = false;

				if (m_pSample != NULL)
				{// ���{�p�̃��f�����������Ƃ�
					m_pSample->Uninit();		// �I������
					m_pSample->Release();		// �������̊J��
					m_pSample = NULL;			// NULL����
				}

				// �f�o�b�O�p���̊J������
				for (int nCount = 0; nCount < FLOOR_LIMIT * FLOOR_LIMIT; nCount++)
				{
					if (m_apMeshField[nCount] != NULL)
					{// �f�o�b�O�p�̏������݂��Ă����Ƃ�
						m_apMeshField[nCount]->Release();
						m_apMeshField[nCount] = NULL;
					}
				}
			}
		}
		else if (m_nMode == DEBUGMODE_RAND)
		{// �n�`�ҏW���[�h�������Ƃ�
		}
		else if (m_nMode == DEBUGMODE_MANY)
		{// �͈͓������������[�h�������Ƃ�
		}
		else if (m_nMode == DEBUGMODE_INDIVIDUAL)
		{// �X�z�u���[�h�������Ƃ�
		}
		else if (m_nMode == DEBUGMODE_DELETE)
		{// �폜���[�h�������Ƃ�
		}
		else if (m_nMode == DEBUGMODE_PAINT)
		{// �n�`�ҏW���[�h�������Ƃ�
		}
		else if (m_nMode == DEBUGMODE_ENEMY)
		{
		}
		// �X�V�I��
		ImGui::End();
	}
}

//=============================================================================
// ���j���[�o�[����
//=============================================================================
void CDebugProc::MenuBar(void)
{
	if (ImGui::BeginMenu("File"))
	{// �t�@�C���^�u�̐���
		if (ImGui::BeginMenu("Load"))
		{// �Z�[�u�^�u�̐���
			if (ImGui::MenuItem("Rand"))
			{// ���[�h
			 // �����̓ǂݍ���
				CMeshField::LoadRand("data/stage/rand.txt", false);
			}
			if (ImGui::MenuItem("Model"))
			{// ���[�h
			 // ���f�����̓ǂݍ���
				CObject::LoadModelTest("data/text/model.txt");
			}
			if (ImGui::MenuItem("wall"))
			{// ���[�h
			 // ���f�����̓ǂݍ���
				CMeshWall::LoadWall("data/text/wall.txt", false);
			}
			if (ImGui::MenuItem("All"))
			{// �Z�[�u
			 // �����̓ǂݍ���
				CMeshField::LoadRand("data/stage/rand.txt", false);

				// ���f�����̓ǂݍ���
				CObject::LoadModelTest("data/text/model.txt");
			}
			ImGui::EndMenu();			// ���j���[�̍X�V�I��
		}

		if (ImGui::BeginMenu("Save"))
		{// �Z�[�u�^�u�̐���
			if (ImGui::MenuItem("Rand"))
			{// ���[�h
			 // �����̏�������
				CScene::SaveRand();
			}
			if (ImGui::MenuItem("Model"))
			{// ���[�h
			 // ���f�����̏�������
				CScene::SaveModel();
			}
			if (ImGui::MenuItem("wall"))
			{// ���[�h
			 // ���f�����̏�������
				CScene::SaveWall();
			}
			if (ImGui::MenuItem("All"))
			{// �Z�[�u
			 // �����̏�������
				CScene::SaveRand();

				// ���f�����̏�������
				CScene::SaveModel();
			}
			ImGui::EndMenu();			// ���j���[�̍X�V�I��
		}

		ImGui::EndMenu();			// ���j���[�̍X�V�I��
	}
}

//=============================================================================
// �^�u����
//=============================================================================
void CDebugProc::TabBar(D3DXVECTOR3 &worldPos)
{
	if (ImGui::BeginTabItem(u8"�����h�X�P�[�v"))
	{// �����h�X�P�[�v���[�h
		if (m_nMode != DEBUGMODE_RAND)
		{
			m_nMode = DEBUGMODE_RAND;
		}

		LandScape(worldPos);
		ImGui::Text(u8"�����h�X�P�[�v");				// ���݂̃f�o�b�O���[�h�̕\��
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem(u8"�ǂ̒��_�ҏW"))
	{// �ǂ̒��_�ҏW���[�h
		if (m_nMode != DEBUGMODE_WALL)
		{
			m_nMode = DEBUGMODE_WALL;
		}

		EditWallVertex();
		ImGui::Text(u8"�ǂ̒��_�ҏW");				// ���݂̃f�o�b�O���[�h�̕\��
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem(u8"�t�H���b�W"))
	{// �t�H���b�W���[�h
		if (m_nMode != DEBUGMODE_MANY)
		{
			m_nMode = DEBUGMODE_MANY;
		}

		ImGui::SliderInt("Generation", &m_nCntGeneration, 0, 50);
		CreateObject(worldPos);					// �����z�u���[�h�̎��s
		ImGui::Text("Many Debug");
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem(u8"�X�z�u"))
	{// �X�z�u���[�h
		if (m_nMode != DEBUGMODE_INDIVIDUAL)
		{
			m_nMode = DEBUGMODE_INDIVIDUAL;
		}

		CreateIndividual(worldPos);				// �X�z�u���[�h�̎��s

		if (m_pSample == NULL)
		{
			m_pSample = CObject::Create();						// ���{�p�I�u�W�F�N�g���쐬

			if (m_pSample != NULL)
			{
				m_pSample->BindModel(m_currentModel);
				m_pSample->SetPosition(worldPos);								// �ʒu���}�E�X�̃��[���h���W�ɐݒ�
				m_pSample->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));			// �F�̕ύX
			}
		}
		ImGui::SameLine();															// ���s���
		ImGui::RadioButton("delete", &m_nMode, DEBUGMODE_DELETE);					// �I���� �͈͓������������[�h ��ǉ�

		// ���݂̃f�o�b�O�^�C�v��\��
		ImGui::Text("individual Debug");
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem(u8"UI�쐬���[�h"))
	{// UI�쐬���[�h
		if (m_nMode != DEBUGMODE_UI)
		{
			m_nMode = DEBUGMODE_UI;
		}

		if (m_pCreateUi != NULL)
		{
			D3DXVECTOR3 pos = m_pCreateUi->GetPosition();
			ImGui::DragFloat3("pos_ui", (float*)&pos);
			m_pCreateUi->SetPosition(pos);

			if (ImGui::CollapsingHeader("OpenFile"))
			{// UI���J��
			 // UI�̃X�N���v�g�I��
				SelectAssetWithUI();

				if (ImGui::Button("Open"))
				{// �J��
					m_pCreateUi->LoadScript(m_currentUi);				// �X�N���v�g�̓ǂݍ���
				}
			}

			if (ImGui::CollapsingHeader("CreateTexture"))
			{// �e�N�X�`������
				// UI�̃X�N���v�g�I��
				SelectAssetWithTexture();

				if (ImGui::Button("AssetCreate"))
				{// �A�Z�b�g�쐬
					m_pCreateUi->CreateTexture(m_currentTexture);		// �A�Z�b�g�̍쐬
				}
			}

			m_pCreateUi->SceneDebug();

			// �����o������
			if (ImGui::CollapsingHeader("System"))
			{// �e�N�X�`������
				ImGui::Text(u8"�����o������");
				ImGui::InputText("name", m_CreateName, NAME_SIZE);

				if (ImGui::Button("Export"))
				{// �o�̓{�^��
					m_pCreateUi->SaveScript(m_CreateName);		// �X�N���v�g�ɏ������ݏ���
				}

				ImGui::Text(u8"�I������");
				if (ImGui::Button("End"))
				{// �I���{�^��
					m_pCreateUi->Uninit();				// �\�����̃A�Z�b�g���J��
					m_pCreateUi->Release();
					m_pCreateUi = NULL;
				}
			}
		}
		else
		{
			m_pCreateUi = CUi::Create();
		}

		// ���݂̃f�o�b�O�^�C�v��\��
		ImGui::Text(u8"UI�쐬���[�h");
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("delete"))
	{// �I�u�W�F�N�g�폜���[�h
		if (m_nMode != DEBUGMODE_DELETE)
		{
			m_nMode = DEBUGMODE_DELETE;
		}

		DeleteObject(worldPos);					// �폜���[�h�̎��s
		// ���݂̃f�o�b�O�^�C�v��\��
		ImGui::Text(u8"�폜���[�h");
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Randpaint"))
	{// ���_�J���[�̕ύX���[�h
		if (m_nMode != DEBUGMODE_PAINT)
		{
			m_nMode = DEBUGMODE_PAINT;
		}

		Paint(worldPos);
		ImGui::Text("Paint Debug");				// ���݂̃f�o�b�O���[�h�̕\��
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Enemy"))
	{// ���_�J���[�̕ύX���[�h
		if (m_nMode != DEBUGMODE_ENEMY)
		{
			m_nMode = DEBUGMODE_ENEMY;
		}

		if (m_pEnemy == NULL)
		{
			m_pEnemy = CEnemy::Create();			// ���{�p�I�u�W�F�N�g���쐬

			if (m_pEnemy != NULL)
			{
				m_pEnemy->SetPosition(worldPos);	// �ʒu���}�E�X�̃��[���h���W�ɐݒ�
			}
		}

		CreateEnemy(worldPos);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Particle"))
	{// ���_�J���[�̕ύX���[�h
		if (m_nMode != DEBUGMODE_PARTICLE)
		{
			m_nMode = DEBUGMODE_PARTICLE;
		}

		// �^�O�̐ݒ�
		ImGui::InputText("Tag", m_effectTag, NAME_SIZE);

		// UI�̃X�N���v�g�I��
		SelectAssetWithTexture();

		// �G�t�F�N�g�̕��o�^�C�v�I��
		ImGui::Combo("ParticleType", &m_nEmissionType, "Cone\0Sphere\0Box\0");

		ImGui::DragInt(u8"������", &m_nCreate);		// ������
		ImGui::DragFloat3(u8"�ʒu", (float*)&m_createPos);
		ImGui::DragFloat3(u8"��]��", (float*)&m_createRot);
		ImGui::DragFloat3(u8"�傫��", (float*)m_size);
		ImGui::DragFloat3(u8"�傫���̕ω���", (float*)&m_moveSize);
		ImGui::DragFloat3(u8"��]�̕ω���", (float*)&m_moveRot);
		ImGui::DragFloat4(u8"�J���[", (float*)&m_col);
		ImGui::DragFloat4(u8"�F�̕ω���", (float*)&m_moveCol);
		ImGui::DragFloat2(u8"�摜������", (float*)&m_sprite);

		ImGui::DragInt(u8"��������", &m_particleLife);
		ImGui::DragInt(u8"�`��", &m_nParticleShape);

		ImGui::Checkbox(u8"�p�x�̃����_��", &m_bRandAngle);

		ImGui::DragFloat(u8"�X�s�[�h�̍ő�l", &m_fMaxSpeed);

		if (m_bRandAngle)
		{
			// �p�x
			m_fAngle = CEffect::GetRandomAngle();		// �����_���Ɏ擾
		}
		else { ImGui::DragFloat(u8"�p�x", &m_fAngle); }

		float fSpeed = 0.0f;
		if (!m_bRandomSpeed)
		{// �X�s�[�h�������_������Ȃ������Ƃ�
			fSpeed = m_fMaxSpeed;
		}

		ImGui::DragFloat(u8"����", &m_fDistance);
		ImGui::DragFloat(u8"��]���x", &m_fRotationSpeed);
		ImGui::DragFloat(u8"�d�͉����x", &m_fGravity);

		ImGui::Checkbox(u8"�J��Ԃ�", &m_bLoop);
		ImGui::Checkbox(u8"�����_���X�s�[�h", &m_bRandomSpeed);
		ImGui::Checkbox(u8"�t�F�[�h�A�E�g", &m_bFadeOut);
		ImGui::Checkbox(u8"���Z����", &m_bAlpha);

		switch ((PARTICLESHAPE)m_nEmissionType)
		{
		case PARTICLESHAPE_CONE:
			ImGui::DragFloat(u8"���a", &m_fRadius);
			break;
		case PARTICLESHAPE_SPHERE:
			break;
		case PARTICLESHAPE_BOX:
			break;
		}

		if (ImGui::Button(u8"����"))
		{
			for (int nCount = 0; nCount < m_nCreate; nCount++)
			{
				float fAngle = float(CManager::GetRand(314)) - float(CManager::GetRand(314));
				float fAngle_x = float(CManager::GetRand(314)) - float(CManager::GetRand(314));

				D3DXVECTOR3 particlePos;
				float fRadius;

				switch ((PARTICLESHAPE)m_nEmissionType)
				{
				case PARTICLESHAPE_CONE:
					fRadius = float(CManager::GetRand(10 * 100)) / 100.0f - float(CManager::GetRand(10 * 100)) / 100.0f;

					particlePos = CEffect::GetRandomPosWithCone(m_fRadius);

					// �ʒu�̌v�Z
					particlePos.x = sinf(D3DX_PI * fAngle) * (fRadius);
					particlePos.y = m_fMaxSpeed;
					particlePos.z = cosf(D3DX_PI * fAngle) * (fRadius);
					break;
				case PARTICLESHAPE_SPHERE:
					particlePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					particlePos.x = cosf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);
					particlePos.y = sinf(D3DX_PI + fAngle_x);
					particlePos.z = sinf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);

					fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;
					break;
				case PARTICLESHAPE_BOX:
					break;
				}

				D3DXVECTOR3 rot = D3DXVECTOR3_ZERO;
				//rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);

				// �p�[�e�B�N���S�̂̈ʒu�v�Z
				D3DXVECTOR3 vecPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXMATRIX mtxMeshRot, mtxMeshTrans;				// �v�Z�p�}�g���b�N�X
				D3DXMATRIX mtx;										// ����̃}�g���b�N�X
				D3DXMATRIX mtxPlayer;

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&mtx);

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&mtxPlayer);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
				D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshRot);

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxMeshTrans, vecPos.x, vecPos.y, vecPos.z);
				D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshTrans);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
				D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxMeshTrans, particlePos.x, particlePos.y, particlePos.z);
				D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

				D3DXMatrixMultiply(&mtx, &mtx, &mtxPlayer);

				CEffect::SetEffect
				(
					m_currentTexture,		// �p�[�e�B�N���̃^�C�v
					m_createPos,			// �����ʒu
					m_size,					// �T�C�Y
					D3DXVECTOR3(mtx._41, mtx._42, mtx._43) * fSpeed,	// �����x�N�g��
					m_moveSize,				// �傫���̕ω���
					m_moveRot,				// ��]�̕ω���
					m_moveCol,				// �F�̕ω���
					EASINGTYPE_NONE,
					rot,					// �e�N�X�`���̉�]
					m_col,					// �J���[
					m_particleLife,			// �p�[�e�B�N���̐����J�E���g��
					m_fGravity,				// �d��
					m_fResistance,			// ��R
					m_bBillboard,			// �r���{�[�h
					0,						// �\������ӏ�(��)
					0,						// �\������ӏ�(�c)
					m_centerPos,			// ���S�ʒu
					m_fAngle,				// �p�x
					m_fDistance,			// ����
					m_fRotationSpeed,		// ��]���x
					m_sprite,				// �摜�̕�����
					m_bAlpha,				// ���Z�����̗L��
					m_bZBuffer,				// Z�o�b�t�@�̔�r�L��
					m_bFadeOut				// �t�F�[�h
				);
			}
		}

		if (ImGui::Button("Export"))
		{
			SaveParticle();
		}

		ImGui::EndTabItem();
	}
}

//=============================================================================
// �����h�X�P�[�v����
//=============================================================================
void CDebugProc::LandScape(D3DXVECTOR3 &worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();								// �}�E�X�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();						// �L�[�{�[�h�̎擾

	// ���݃}�E�X�̍��W�͈͓̔��ɂ��鏰���擾����
	CScene *pSceneNext = NULL;														// ������
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_FLOOR);					// ���̐擪�A�h���X�̎擾

	//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_FLOOR);		//����A�b�v�f�[�g�Ώۂ��T����
		CMeshField *pField = (CMeshField*)pSceneNow;
		if (!pField->GetDebugRand())
		{
			if (pField->CollisionRangeToCircle(worldPos, m_fPaintSize))					// �~�����͈͓̔��ɓ����Ă��邩�ǂ���
			{
				if (m_mouseOld != worldPos)
				{// �O�̍��W�ƍ���̍��W���Ⴄ�Ƃ�
					pField->CreateRand(worldPos, m_fPaintSize);
				}
			}
		}
		pSceneNow = pSceneNext;														//����A�b�v�f�[�g�Ώۂ��i�[
	}

	m_CreateRandOld = m_CreateRand;						// �O�̃f�[�^����

	// �e��X���C�_�[�̐ݒ�
	ImGui::DragFloat(u8"�����ʒu�̃X���C�h�p���[", &m_fSliderPow, 0.5f, 0.1f, 50.0f, "%.1f", 0.5f);

	if (ImGui::DragFloat3(u8"�����ʒu", (float*)&m_createPos, m_fSliderPow))
	{
		DebugFloorCreate((int)m_CreateRand.x, (int)m_CreateRand.y, m_createPos);		// �f�o�b�O�p�̏��̍Ĕz�u
	}

	if (ImGui::DragFloat2(u8"��������", (float*)&m_CreateRand, 1.0f, 0.0f, 20.0f, "%.0f", 1.0f))
	{
		DebugFloorCreate((int)m_CreateRand.x, (int)m_CreateRand.y, m_createPos);		// �f�o�b�O�p�̏��̍Ĕz�u
	}

	if (ImGui::Button(u8"����"))
	{// �����{�^���������ꂽ�Ƃ�
		CMeshField::CreateRand((int)m_CreateRand.x, (int)m_CreateRand.y, m_createPos);	// �����Ƃ������𐶐�
	}

	if (!pKeyboard->GetPressKeyboard(DIK_LALT))
	{// ��Alt�L�[��������Ă��Ȃ��Ƃ�
		if (pKeyboard->GetPressKeyboard(DIK_R))
		{// R�L�[��������Ă���Ƃ�
			if (pMouse->GetReleaseMouse(MOUSE_RIGHT))
			{// �}�E�X�̉E�{�^���������ꂽ�Ƃ�
				pSceneNow = CScene::GetScene(CScene::PRIORITY_FLOOR);					// ���̐擪�A�h���X�̎擾

				//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
				while (pSceneNow != NULL)
				{
					pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_FLOOR);		//����A�b�v�f�[�g�Ώۂ��T����
					CMeshField *pField = (CMeshField*)pSceneNow;

					if (!pField->GetDebugRand())
					{
						pField->CalculationNormalize();
					}

					pSceneNow = pSceneNext;														//����A�b�v�f�[�g�Ώۂ��i�[
				}
			}
			else if (pMouse->GetReleaseMouse(MOUSE_LEFT))
			{// �}�E�X�̍��{�^���������ꂽ�Ƃ�
				pSceneNow = CScene::GetScene(CScene::PRIORITY_FLOOR);					// ���̐擪�A�h���X�̎擾

				//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
				while (pSceneNow != NULL)
				{
					pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_FLOOR);		//����A�b�v�f�[�g�Ώۂ��T����
					CMeshField *pField = (CMeshField*)pSceneNow;

					if (!pField->GetDebugRand())
					{
						pField->CalculationNormalize();
					}

					pSceneNow = pSceneNext;														//����A�b�v�f�[�g�Ώۂ��i�[
				}
			}
		}
	}

	m_mouseOld = worldPos;
}

//=============================================================================
// �ǂ̒��_�ҏW����
//=============================================================================
void CDebugProc::EditWallVertex(void)
{
	CScene *pSceneNext = NULL;	//����A�b�v�f�[�g�Ώ�
	CScene *pSceneNow = NULL;

	// �ǂ̏�� //
	// tree�̃I�u�W�F�N�g�̃|�W�V�������Q��
	pSceneNow = CScene::GetScene(CScene::PRIORITY_WALL);

	//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_WALL);						//����A�b�v�f�[�g�Ώۂ��T����
		CMeshWall *pMeshWall = (CMeshWall*)pSceneNow;
		pMeshWall->EditWallVertex();			// �ǂ̒��_���ҏW����
		pSceneNow = pSceneNext;													// ����A�b�v�f�[�g�Ώۂ��i�[
	}
}

//=============================================================================
// ���_�F�ύX����
//=============================================================================
void CDebugProc::Paint(D3DXVECTOR3 & worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();								// �}�E�X�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();						// �L�[�{�[�h�̎擾

	// ���݃}�E�X�̍��W�͈͓̔��ɂ��鏰���擾����
	CScene *pSceneNext = NULL;														// ������
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_FLOOR);					// ���̐擪�A�h���X�̎擾

	// �����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_FLOOR);		// ����A�b�v�f�[�g�Ώۂ��T����
		CMeshField *pField = (CMeshField*)pSceneNow;
		if (pField->CollisionRangeToCircle(worldPos, m_fPaintSize))					// �~�����͈͓̔��ɓ����Ă��邩�ǂ���
		{
			if (m_mouseOld != worldPos)
			{// �O�̍��W�ƍ���̍��W���Ⴄ�Ƃ�
				pField->Paint(worldPos, m_fPaintSize);
			}
		}
		pSceneNow = pSceneNext;														// ����A�b�v�f�[�g�Ώۂ��i�[
	}

	m_mouseOld = worldPos;
}

//=============================================================================
// �G�z�u����
//=============================================================================
void CDebugProc::CreateEnemy(D3DXVECTOR3 &worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();			// �}�E�X�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CScene *pScene = CScene::NowFloor(worldPos);				// ���݂���ꏊ�̃t�B�[���h���擾

	D3DXVECTOR3 pos = worldPos;

	if (pScene != NULL)
	{// �������݂��Ă����Ƃ�
		CMeshField *pMeshField = (CMeshField*)pScene;			// ���̎擾
		pos.y = pMeshField->GetHeight(worldPos);				// ���̍������擾
	}

	if (m_pEnemy != NULL)
	{
		m_pEnemy->SetPosition(pos);								// ���{�p���f���̈ʒu�����݂̃}�E�X���W�ɐݒ�
	}

	if (!pKeyboard->GetPressKeyboard(DIK_LALT))
	{// Alt�L�[ ��������Ă��Ȃ��Ƃ�
		if (pKeyboard->GetPressKeyboard(DIK_R))
		{
			if (pMouse->GetTriggerMouse(MOUSE_LEFT))
			{// �}�E�X�̍��{�^���������ꂽ�Ƃ�
				CEnemy *pEnemy = CEnemy::Create();				// ���݂̌��{���쐬
				pEnemy->SetPosition(worldPos);					// ���{�̏ꏊ�ɐݒu
			}
		}
	}
}

//=============================================================================
// �����z�u����
//=============================================================================
void CDebugProc::CreateObject(D3DXVECTOR3 &worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();				// �}�E�X�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();		// �L�[�{�[�h�̎擾
	CCamera *pCamera = CManager::GetCamera();						// �J�����̎擾
	CMeshField *pMeshField = CGame::GetMesh();
	CObject *pObject = NULL;

	if (!pKeyboard->GetPressKeyboard(DIK_LALT))
	{// ��Alt�L�[��������Ă��Ȃ��Ƃ�
		if (pKeyboard->GetPressKeyboard(DIK_R))
		{
			if (pMouse->GetPressMouse(MOUSE_LEFT))
			{// �}�E�X�̍��{�^����������Ă���Ƃ�
				D3DXVECTOR3 Difference;				// ����
				float fDistance;					// ����

				// �O�̈ʒu�ƌ��݂̈ʒu�Ƃ̋������Z�o
				Difference.x = m_mouseOld.x - worldPos.x;
				Difference.z = m_mouseOld.z - worldPos.z;
				fDistance = (float)sqrt(Difference.x * Difference.x + Difference.z * Difference.z);

				if (fDistance > m_fPaintSize)
				{// �O�̍��W�ƍ���̍��W���Ⴄ�Ƃ�
					// ���f���͈͔̔z�u
					for (int nCount = 0; nCount < m_nCntGeneration; nCount++)
					{
						float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
						D3DXVECTOR3 pos = D3DXVECTOR3(sinf(D3DX_PI + 6.28f * fAngle) * ((float)CManager::GetRand((int)m_fPaintSize * 10) * 0.1f) + worldPos.x,
							0.0f,
							cosf(D3DX_PI + 6.28f * fAngle) * ((float)CManager::GetRand((int)m_fPaintSize * 10) * 0.1f) + worldPos.z);

						//���̍������擾����
						CScene *pSceneNext = NULL;			// ������
						CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_MODEL);			// �V�[���̐擪�A�h���X���擾

						//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
						while (pSceneNow != NULL)
						{
							pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_MODEL);		// ����A�b�v�f�[�g�Ώۂ��T����
							CMeshField *pField = (CMeshField*)pSceneNow;								// �N���X�`�F���W(��)

							if (m_nCntContinue > CONTINUE_MAX)
							{// �Čv�Z�񐔂�10��𒴂����Ƃ�
								break;
							}
							else if (50.0f > CManager::GetDistance(pSceneNow->GetPosition(), pos))
							{
								// �l�̍Ď擾
								float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
								D3DXVECTOR3 pos = D3DXVECTOR3(sinf(D3DX_PI + 6.28f * fAngle) * ((float)CManager::GetRand((int)m_fPaintSize * 10) * 0.1f) + worldPos.x,
									0.0f,
									cosf(D3DX_PI + 6.28f * fAngle) * ((float)CManager::GetRand((int)m_fPaintSize * 10) * 0.1f) + worldPos.z);

								pSceneNext = CScene::GetScene(CScene::PRIORITY_MODEL);			// �V�[���̐擪�A�h���X���擾
								m_nCntContinue++;					// �Ċm�F�񐔂Ƀv���X����
							}

							pSceneNow = pSceneNext;				//����A�b�v�f�[�g�Ώۂ��i�[
						}

						if (m_nCntContinue < CONTINUE_MAX)
						{
							// �I�u�W�F�N�g�̍쐬
							pObject = CObject::Create();
							if (pObject != NULL)
							{
								pObject->BindModel(m_currentModel);
							}
						}

						m_nCntContinue = 0;

						if (pObject != NULL)
						{
							//���̍������擾����
							CScene *pSceneNext = NULL;			// ������
							CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_FLOOR);			// �V�[���̐擪�A�h���X���擾

							//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
							while (pSceneNow != NULL)
							{
								pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_FLOOR);		// ����A�b�v�f�[�g�Ώۂ��T����
								CMeshField *pField = (CMeshField*)pSceneNow;								// �N���X�`�F���W(��)

								if (pField->CollisionRange(pos))
								{// �I�u�W�F�N�g �����ɏ���Ă����Ƃ�
									pos.y = pField->GetHeight(pos);										// ���̍��������߂�
									pObject->SetPosition(pos);
									break;
								}

								pSceneNow = pSceneNext;				//����A�b�v�f�[�g�Ώۂ��i�[
							}
						}
					}

					m_mouseOld = worldPos;					// �Ō�̉��H�ʒu��ۑ�
				}
			}
		}
	}
}

//=============================================================================
// �P�̔z�u����
//=============================================================================
void CDebugProc::CreateIndividual(D3DXVECTOR3 &worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();								// �}�E�X�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();						// �L�[�{�[�h�̎擾
	CScene *pScene = CScene::NowFloor(worldPos);									// ���݂���ꏊ�̃t�B�[���h���擾
	CPlayer *pPlayer = CGame::GetPlayer();

	D3DXVECTOR3 pos = D3DXVECTOR3_ZERO;
	D3DXVECTOR3 rot = D3DXVECTOR3_ZERO;
	D3DXVECTOR3 size = D3DXVECTOR3_ZERO;

	if (m_pSample != NULL)
	{// ���{�p�I�u�W�F�N�g�����݂��Ă����Ƃ�
		if (!m_bMouseCursorPosition)
		{
			pos = m_pSample->GetPosition();				// �ʒu�̎擾
		}
		else
		{
			if (pPlayer != NULL)
			{
				pos = pPlayer->GetPosition();
			}
		}

		rot = m_pSample->GetRotation();					// ��]�l�̎擾
		size = m_pSample->GetSize();					// �T�C�Y�̎擾
	}

	ImGui::Checkbox("MousePos", &m_bMouseCursorPosition);
	ImGui::DragFloat3("pos", (float*)&pos);
	ImGui::DragFloat3("rot", (float*)&rot, 0.01f);
	ImGui::DragFloat3("size", (float*)&size, 0.01f);

	if (pScene != NULL)
	{// �������݂��Ă����Ƃ�
		CMeshField *pMeshField = (CMeshField*)pScene;			// ���̎擾
		if (pMeshField != NULL)
		{
			pos.y = pMeshField->GetHeight(worldPos);			// ���̍������擾
		}
	}

	if (m_pSample != NULL)
	{
		if (strcmp(m_pSample->GetAdd().c_str(), m_currentModel.c_str()) != 0)
		{// ���݂̃��f���^�C�v���O��̃��f���ƈႤ�Ƃ�
			m_pSample->Uninit();										// �O�̃��f�����J��
			m_pSample->Release();										// �O�̃��f���̊J���t���O�𗧂Ă�
			m_pSample = NULL;											// �O�̃��f���̃A�h���X���̂Ă�

			m_pSample = CObject::Create();								// �V�������f���𐶐�

			if (m_pSample != NULL)
			{
				m_pSample->BindModel(m_currentModel);
			}
		}
	}

	if (m_pSample != NULL)
	{// ���{�p�I�u�W�F�N�g�����݂��Ă����Ƃ�
		m_pSample->SetPosition(pos);									// ���{�p���f���̈ʒu�����݂̃}�E�X���W�ɐݒ�
		m_pSample->SetRotation(rot);									// ��]�l�̎擾
		m_pSample->SetSize(size);
	}

	if (!pKeyboard->GetPressKeyboard(DIK_LALT))
	{// Alt�L�[ ��������Ă��Ȃ��Ƃ�
		if (pKeyboard->GetPressKeyboard(DIK_R))
		{
			if (pMouse->GetTriggerMouse(MOUSE_LEFT))
			{// �}�E�X�̍��{�^���������ꂽ�Ƃ�
				CObject *pObject = CObject::Create();					// ���݂̌��{���쐬

				if (pObject != NULL)
				{
					pObject->BindModel(m_currentModel);
					pObject->SetPosition(pos);							// ���{�̏ꏊ�ɐݒu
					pObject->SetRotation(rot);							// ��]�ʂ̐ݒ�
					pObject->SetSize(size);								// �T�C�Y�̐ݒ�
				}
			}
		}
	}
}

//=============================================================================
// �I�u�W�F�N�g�͈͓��폜����
//=============================================================================
void CDebugProc::DeleteObject(D3DXVECTOR3 &worldPos)
{
	CInputMouse *pMouse = CManager::GetInputMouse();								// �}�E�X�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();						// �L�[�{�[�h�̎擾

	// ���݃}�E�X�̍��W�͈͓̔��ɂ��鏰���擾����
	CScene *pSceneNext = NULL;														// ������
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_MODEL);					// ���̐擪�A�h���X�̎擾

	if (pKeyboard->GetPressKeyboard(DIK_R))
	{
		if (pMouse->GetPressMouse(MOUSE_LEFT))
		{// �}�E�X�̉E�{�^���������ꂽ�Ƃ�
		// �����Ȃ��Ȃ�܂ŌJ��Ԃ�
			while (pSceneNow != NULL)
			{
				pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_MODEL);		//����A�b�v�f�[�g�Ώۂ��T����
				CSceneX *pObject = (CSceneX*)pSceneNow;

				if (CMeshField::SphereModel(pObject->GetPosition(), worldPos, m_fPaintSize))					// �~�����͈͓̔��ɓ����Ă��邩�ǂ���
				{
					pObject->Release();
				}
				pSceneNow = pSceneNext;														//����A�b�v�f�[�g�Ώۂ��i�[
			}
		}
	}
}

//=============================================================================
// �p�[�e�B�N���쐬
//=============================================================================
void CDebugProc::CreateParticle(void)
{

}

//=============================================================================
// �f�o�b�O�p����������
//=============================================================================
void CDebugProc::DebugFloorCreate(const int &nWide, const int &nDepth, D3DXVECTOR3 &createPos)
{
	// �f�o�b�O�p���|�C���^�̏�����
	for (int nCount = 0; nCount < FLOOR_LIMIT * FLOOR_LIMIT; nCount++)
	{
		if (m_apMeshField[nCount] == NULL)
		{// �������݂��Ă����Ƃ�
			continue;
		}

		if (m_apMeshField[nCount]->GetActive())
		{// �\�����Ă����Ƃ�
			m_apMeshField[nCount]->SetActive(false);
		}
	}

	for (int nCntDepth = 0; nCntDepth < nDepth; nCntDepth++)
	{
		for (int nCntWide = 0; nCntWide < nWide; nCntWide++)
		{
			D3DXVECTOR3 pos;

			pos.x = (WIDE_FIELD * SIZE) * nCntWide - (((WIDE_FIELD * SIZE) * nWide) / 2) + ((WIDE_FIELD * SIZE) / 2) + createPos.x;
			pos.y = createPos.y;
			pos.z = (DEPTH_FIELD * SIZE) * nCntDepth - (((DEPTH_FIELD * SIZE) * nDepth) / 2) + ((DEPTH_FIELD * SIZE) / 2) + createPos.z;

			for (int nCount = 0; nCount < FLOOR_LIMIT * FLOOR_LIMIT; nCount++)
			{
				if (m_apMeshField[nCount] == NULL)
				{
					continue;
				}

				if (m_apMeshField[nCount]->GetActive())
				{
					continue;
				}

				m_apMeshField[nCount]->SetPosition(pos);
				m_apMeshField[nCount]->MakeVertex(NULL, true);
				m_apMeshField[nCount]->SetActive(true);
				break;
			}
		}
	}
}

//=============================================================================
// �C���X�y�N�^�[��\��
//=============================================================================
void CDebugProc::ShowInspector(void)
{
	// �����ō쐬�����ȒP�ȃE�B���h�E��\�����܂��B Begin / End�y�A���g�p���āA���O�t���E�B���h�E���쐬���܂��B
	ImGui::Begin("Inspector", &m_bInspectorWind, ImGuiWindowFlags_MenuBar);   // �C���X�y�N�^�[�E�B���h�E����
	m_pSelect->ShowInspector();							// �C���X�y�N�^�[��\��
	ImGui::End();
}

//=============================================================================
// ���f���I������
//=============================================================================
void CDebugProc::SelectAssetWithModel(void)
{
	if (ImGui::BeginCombo(u8"�I�u�W�F�N�g�̎��", m_currentModel.c_str()))
	{
		// �I�u�W�F�N�g�}�b�v
		std::map<std::string, MODEL_INFO> modelMap = CManager::GetModelMap();
		for (auto itr = modelMap.begin(); itr != modelMap.end(); itr++)
		{
			bool is_selected = (m_currentModel == itr->first.c_str());
			if (ImGui::Selectable(itr->first.c_str()))
			{
				m_currentModel = itr->first.c_str();
			}
		}

		ImGui::EndCombo();
	}
}

//=============================================================================
// �e�N�X�`���I������
//=============================================================================
void CDebugProc::SelectAssetWithTexture(void)
{
	if (ImGui::BeginCombo(u8"�e�N�X�`���̎��", m_currentTexture.c_str()))
	{
		// �e�N�X�`���}�b�v
		std::map<std::string, LPDIRECT3DTEXTURE9> modelMap = CManager::GetTextureMap();
		for (auto itr = modelMap.begin(); itr != modelMap.end(); itr++)
		{
			bool is_selected = (m_currentTexture == itr->first.c_str());
			if (ImGui::Selectable(itr->first.c_str()))
			{
				m_currentTexture = itr->first.c_str();
			}
		}

		ImGui::EndCombo();
	}
}

//=============================================================================
// UI�I������
//=============================================================================
void CDebugProc::SelectAssetWithUI(void)
{
	if (ImGui::BeginCombo(u8"UI�̎��", m_currentUi.c_str()))
	{
		for (auto itr = m_AddUi.begin(); itr != m_AddUi.end(); itr++)
		{
			bool is_selected = (m_currentUi == itr->c_str());
			if (ImGui::Selectable(itr->c_str()))
			{
				m_currentUi = itr->c_str();
			}
		}

		ImGui::EndCombo();
	}
}

//=============================================================================
// �p�[�e�B�N���p�����[�^�̏�������
//=============================================================================
void CDebugProc::SaveParticle(void)
{
	CWrite *pWrite = new CWrite;
	if (pWrite == NULL) return;

	//�ϐ��錾
	int nCntLoad = 0;			//���[�h�J�E���g
	char text[64];				// �e�L�X�g
	char cEqual[8] = { "=" };	//�C�R�[���p
	CScene *pSceneNext = NULL;	//����A�b�v�f�[�g�Ώ�
	CScene *pSceneNow = NULL;


	//�J����
	if (pWrite->Open("data/text/particle_Export.txt"))
	{
		strcpy(text, "# �V�[���f�[�^�X�N���v�g\n");
		strcat(text, "# Author : masayasu wakita\n");

		pWrite->TitleWrite(text);				// �^�C�g���̌`���ŏ�������
		pWrite->Write("SCRIPT\n");				// �X�N���v�g�̏I���錾

		pWrite->IndexWrite(" �G�t�F�N�g�̐ݒ�");				// �^�C�g���̌`���ŏ�������
		pWrite->Write("EFFECTSET\n");			// ���_���̏������݊J�n�錾
		pWrite->Write("	TAG = %s\n", m_effectTag);		// �^�O�̏�������
		pWrite->Write("	EFFECT_FILENAME = %s\n", m_currentTexture.c_str());		// �A�h���X������������

		if (m_bBillboard) { pWrite->Write("	BILLBOARD\n"); }			// �r���{�[�h�̗L��
		if (m_bAlpha) { pWrite->Write("	ALPHABLENDING\n"); }			// �A���t�@�u�����f�B���O�̗L��
		if (m_bZBuffer) { pWrite->Write("	ZBUFFER\n"); }			// Z�o�b�t�@�̗L��
		if (m_bFadeOut) { pWrite->Write("	FADE_OUT\n"); }			// �t�F�[�h�A�E�g�̗L��

		pWrite->Write("	SIZE = %.2f %.2f %.2f\n", m_size.x, m_size.y, m_size.z);		// ���S�ʒu�̏�������
		pWrite->Write("	COL = %.2f %.2f %.2f %.2f\n", m_col.r, m_col.g, m_col.b, m_col.a);		// �F�̕ω���

		pWrite->Write("	MOVE_SIZE = %.2f %.2f %.2f\n", m_moveSize.x, m_moveSize.y, m_moveSize.z);		// �T�C�Y�̕ω���
		pWrite->Write("	MOVE_ROT = %.2f %.2f %.2f\n", m_moveSize.x, m_moveSize.y, m_moveSize.z);		// ��]�̕ω���
		pWrite->Write("	MOVE_COL = %.2f %.2f %.2f %.2f\n", m_moveCol.r, m_moveCol.g, m_moveCol.b, m_moveCol.a);		// �F�̕ω���
		pWrite->Write("	LIFE = %d\n", m_particleLife);		// �����J�E���g

		pWrite->Write("	GRAVITY = %.2f\n", m_fGravity);		// �d��
		pWrite->Write("	RESISTANCE = %.2f\n", m_fResistance);		// �����J�E���g
		pWrite->Write("	SPEED = %.2f\n", m_fMaxSpeed);

		pWrite->Write("	CENTER_POS = %.2f %.2f %.2f\n", m_centerPos.x, m_centerPos.y, m_centerPos.z);		// ���S�ʒu
		pWrite->Write("	ANGLE = %.2f\n", m_fAngle);		// �p�x
		pWrite->Write("	DISTANCE = %.2f\n", m_fDistance);		// ����
		pWrite->Write("	ROTATION_SPEED = %.2f\n", m_fRotationSpeed);		// ��]���x
		pWrite->Write("	SPRITE = %.2f %.2f %.2f\n", m_sprite.x, m_sprite.y);		// �e�N�X�`��������

		pWrite->Write("END_EFFECTSET\n");			// ���_���̏������ݏI���錾

		pWrite->Write("END_SCRIPT\n");			// �X�N���v�g�̏I���錾
		pWrite->End();
	}
}

//=============================================================================
// Ui�̃A�h���X�ǂݍ���
//=============================================================================
void CDebugProc::LoadAddWithUI(void)
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
	pFile = fopen(UIMANAGER_SCRIPT, "r");

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
					if (strcmp(cHeadText, "UI_FILENAME") == 0)
					{//�p�[�c���f���̃A�h���X���̂Ƃ�
						sscanf(cReadText, "%s %s %s", &cDie, &cDie, &sAdd[0]);						//�A�h���X�̎擾
						Add = sAdd;
						m_AddUi.push_back(Add);
					}
				}
			}
		}

		//�t�@�C����
		fclose(pFile);
	}
	else
	{

	}
}