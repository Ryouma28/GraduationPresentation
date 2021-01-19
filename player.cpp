//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "player.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "inputController.h"
#include "scene3D.h"
#include "meshField.h"
#include "model.h"
#include "meshCapsule.h"
#include "colliderSphere.h"
#include "meshCube.h"
#include "enemy.h"
#include "meshOrbit.h"
#include "camera.h"
#include "colliderBox.h"
#include "stage.h"
#include "sound.h"
#include "scene2D.h"
#include "effect.h"
#include "result.h"
#include "fade.h"
#include "takaseiLibrary.h"
#include "sceneX.h"
#include "speed.h"
#include "wall.h"
#include "object.h"
#include "puzzle.h"
#include "number.h"
#include "network.h"
#include "distanceNext.h"
#include "ui.h"
#include "shadow.h"
#include "characterSelect.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define ROT_AMOUNT 0.1f								// ��]�̍������炵�Ă�����
#define ROT_SPEED_DRIFT 0.5f						// �h���t�g����]���x
#define MODEL_FRONT 2								// ���f���O�֔ԍ�
#define MODEL_REAR 1								// ���f����֔ԍ�
#define MODEL_TIRE 2								// �^�C�����f���̐�
#define CAMERA_ROT_SPEED 0.4f						// �J�����̉�]���x
#define TIRE_ROT_SPEED 0.1f							// �^�C���̉�]���x
#define ACCEKERATION 3.0f							// �h���t�g�����x�����l
#define ACCEKERATION_ADDITION 0.02f					// �h���t�g�����x���Z��
#define DRIFT_DECREACE 0.6f							// �h���t�g�����x����
#define DRIFT_DEST 0.25f							// �h���t�g���^�C���̌���
#define	INIT_ROT 1.38f
#define SLOPE_SPEED 0.3f							// ��̑��x

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(CScene::PRIORITY obj = CScene::PRIORITY_PLAYER) : CCharacter(obj)
{
	// �D��x�̐ݒ�
	SetObjType(CScene::PRIORITY_PLAYER);				// �I�u�W�F�N�g�^�C�v

	m_nLife = 100;										// �̗͂̏�����
	m_rot = D3DXVECTOR3(0.0f, INIT_ROT, 0.0f);			// ��]�̏�����
	m_fSpeed = 0;										// �X�s�[�h�̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ��ʂ̏�����
	m_dest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ���̏�����
	m_difference = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���̏�����
	m_vectorOld = ZeroVector3;							// �O��̃x�N�g������
	m_cameraRot = D3DXVECTOR3(0, D3DX_PI, 0);			// �J�����̉�]��񏉊���
	m_pColPlayerSphere = NULL;							// �v���C���[�����蔻��|�C���^�̏�����
	m_pDistanceNext = NULL;								// ���̃v���C���[�Ƃ̋�����UI
	m_bHit = false;										// �����蔻��t���O�̏�������
	m_bJump = false;									// �W�����v�t���O�̏�����
	m_nActionCount = 0;									// �A�N�V�����J�E���^�̏�����
	m_nParticleCount = 0;								// �p�[�e�B�N���J�E���^�̏�����
	m_nPointNum = 0;									// �|�C���g�ԍ�������
	m_fDeathblow = 0.0f;								// �K�E�Z�|�C���g�̏�����
	m_fAcceleration = ACCEKERATION;						// �����x
	m_fSlopeSpeed = 0.0f;								// ��̑��x
	m_bEvent = true;									// �C�x���g�����t���O�̏�����
	m_bMove = false;									// ���ݓ����Ă��邩�̃t���O
	m_bAccel = false;									// �A�N�Z���������Ă��邩�̃t���O
	m_bColliderWithWall = true;							// �ǂ̓����蔻��
	m_bGoal = false;									// �S�[���t���O
	m_bSlope = false;									// ��ɂ��邩�ǂ����̃t���O

	m_pRank = NULL;

	m_nRound = 0;			// ���݂̎����

	for (int nCnt = 0; nCnt < DRIFT_MAX; nCnt++)
	{
		m_bDrift[nCnt] = false;							// �h���t�g�t���O����
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer *pRenderer = CManager::GetRenderer();
	CNetwork *pNetwork = CManager::GetNetwork();

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();
	m_fPuzzleMaxSPeed = 0;
	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 pos = GetPosition();							// �v���C���[�̈ʒu�擾

	switch (pNetwork->GetId())
	{
	case 0:
		pos = D3DXVECTOR3(1286.00f, -4113.86f, 15932.30f);			// �v���C���[�̈ʒu�ݒ�
		break;
	case 1:
		pos = D3DXVECTOR3(1390.30f, -4113.86f, 15296.58f);			// �v���C���[�̈ʒu�ݒ�
		break;
	case 2:
		pos = D3DXVECTOR3(1191.18f, -4113.86f, 16327.53f);			// �v���C���[�̈ʒu�ݒ�
		break;
	case 3:
		pos = D3DXVECTOR3(1105.84f, -4113.86f, 16769.35f);			// �v���C���[�̈ʒu�ݒ�
		break;
	}

	if (pCamera != NULL)
	{
		pCamera->SetPosCamera(pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	}

	// �L�����N�^�[�̏���������
	CCharacter::Init();

	// �A�j���[�V�����̐ݒ�
	AnimationSwitch(ANIMATIONTYPE_NONE);

	//// �v���C���[���f�����̓ǂݍ���
	//switch (CCharacterSelect::GetCarType())
	//{
	//case 0:
	//	LoadScript(SCRIPT_CAR01, ANIMATIONTYPE_MAX);
	//	break;
	//case 1:
	//	LoadScript(SCRIPT_CAR02, ANIMATIONTYPE_MAX);
	//	break;
	//case 2:
	//	LoadScript(SCRIPT_CAR03, ANIMATIONTYPE_MAX);
	//	break;
	//case 3:
	//	LoadScript(SCRIPT_CAR04, ANIMATIONTYPE_MAX);
	//	break;
	//}

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		// �v���C���[�̓����蔻��𐶐�
		m_pColPlayerSphere = CColliderSphere::Create(false, 50.0f);

		if (m_pColPlayerSphere != NULL)
		{ //���̂̃|�C���^��NULL�ł͂Ȃ��Ƃ�
			m_pColPlayerSphere->SetScene(this);
			m_pColPlayerSphere->SetTag("player");										// �^�O �̐ݒ�
			m_pColPlayerSphere->SetPosition(pos);										// �ʒu �̐ݒ�
			m_pColPlayerSphere->SetOffset(D3DXVECTOR3(0.0f, 20.0f, 0.0f));
		}
	}

	// �ʒu�̐ݒ�
	SetPosition(pos);

	int nCntPiece = CPuzzle::GetPieceNum();

	for (int nCnt = 0; nCnt < nCntPiece; nCnt++)
	{
		m_fPuzzleSpeed[nCnt] = CPuzzle::GetSpeed(nCnt);
		m_fPuzzleDecay[nCnt] = CPuzzle::GetDecay(nCnt);
		m_fPuzzlePower[nCnt] = CPuzzle::GetPower(nCnt);
		m_fPuzzleRate[nCnt] = CPuzzle::GetRate(nCnt);
		m_fPuzzleTurning[nCnt] = CPuzzle::GetTurning(nCnt);
	}

	SetSpeed(nCntPiece);

	if (m_fPuzzleMaxSPeed <= 0)
	{
		m_fPuzzleMaxSPeed = NORMAL_SPEED;
	}

	for (int nCnt = 0; nCnt < nCntPiece; nCnt++)
	{
		m_bRankingSign[nCnt] = false;
	}
	for (int nCnt = 0; nCnt < nCntPiece; nCnt++)
	{
		m_bRankingSign[nCnt] = CPuzzle::GetRank(nCnt);
		if (m_bRankingSign[nCnt] == true)
		{
			m_bRanking = true;
		}
	}
	if (m_bRanking == true)
	{
		m_pRank = CNumber::Create();

		if (m_pRank != NULL)
		{
			m_pRank->BindTexture("data/tex/number_rank.png");
			m_pRank->SetPosition(D3DXVECTOR3(1110.0f, 75.0f, 0.0f));
			m_pRank->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
			m_pRank->SetTransform();
		}
		CUi *pRankUi = CUi::Create();

		if (pRankUi != NULL)
		{
			pRankUi->LoadScript("data/text/ui/NowRank.txt");
			pRankUi->SetPosition(D3DXVECTOR3(1150.0f, 100.0f, 0.0f));
		}
	}

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		m_pDistanceNext = CDistanceNext::Create();

		if (m_pDistanceNext != NULL)
		{
			m_pDistanceNext->SetPosition(D3DXVECTOR3(200.0f, 80.0f, 0.0f));
			m_pDistanceNext->SetDistance(D3DXVECTOR3(-10.0f, -8.0f, 0.0f));
			m_pDistanceNext->SetIntervalNum(D3DXVECTOR3(45.0f, 0.0f, 0.0f));
			m_pDistanceNext->SetNumber(256);
		}
	}

	// �e�̐���
	m_pShadow = CShadow::Create();

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CPlayer::Uninit(void)
{
	if (m_pColPlayerSphere != NULL)
	{// ����̓����蔻�肪���݂��Ă����Ƃ�
		m_pColPlayerSphere->Release();
	}

	if (m_pDistanceNext != NULL)
	{
		m_pDistanceNext->Uninit();
		delete m_pDistanceNext;
		m_pDistanceNext = NULL;
	}

	CCharacter::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	D3DXVECTOR3 pos;
	CSound *pSound = CManager::GetSound();
	CNetwork *pNetwork = CManager::GetNetwork();
	float fHeight = 0.0f;
	CModel *pModel = GetModel();

	// �A�j���[�V�������̎擾
	ANIMATIONTYPE animType = (ANIMATIONTYPE)GetAnimType();
	int currentKey = GetCurrentKey();
	int currentFrame = GetCurrentFrame();
	bool bAnimPlayState = GetAnimPlayState();

	// ��ł̃v���C���[����
	//m_bSlope = SlopeMove();

	// ���͏���
	if (!m_bHit)
	{// �������Ă��Ȃ������Ƃ�
		CCamera *pCamera = CManager::GetCamera();
		if (pCamera->GetStalker())
		{
			// ���͏���
			Input();
		}
	}

	pos = GetPosition();				// �ʒu�̎擾

	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	VERTEX_PLANE plane = {};

	CCollider::RayBlockCollision(pos, &pModel[0].GetMtxWorld(), 110, 250.0f, plane);

	D3DXVECTOR3 AB = plane.a - plane.b;
	D3DXVECTOR3 BC = plane.b - plane.c;

	D3DXVECTOR3 norwork;

	D3DXVec3Cross(&norwork, &BC, &AB);
	D3DXVec3Normalize(&norwork, &norwork);

	//CDebugProc::Log("a�n�_ : %f, %f, %f\n", plane.a.x, plane.a.y, plane.a.z);
	//CDebugProc::Log("b�n�_ : %f, %f, %f\n", plane.b.x, plane.b.y, plane.b.z);
	//CDebugProc::Log("c�n�_ : %f, %f, %f\n", plane.c.x, plane.c.y, plane.c.z);

	//���̍������擾����
	CScene *pSceneNext = NULL;														// ������
	CScene *pSceneNow = GetScene(PRIORITY_FLOOR);									// �V�[���̐擪�A�h���X���擾

	CScene *pScene = NowFloor(pos);													// ���݂���t�B�[���h���擾

	if (pScene != NULL)
	{// �������Ă��鏰�����݂����Ƃ�
		CMeshField *pFloor = (CMeshField*)pScene;									// �L���X�g
		fHeight = pFloor->GetHeight(pos);

		RANDTYPE Type = pFloor->GetRandType();

		if (animType == ANIMATIONTYPE_RUN)
		{
			if (currentKey == 5 || currentKey == 0)
			{
				if (currentFrame == 0)
				{
					if (Type == RANDTYPE_GRASS)
					{
						//	pSound->PlaySoundA((SOUND_LABEL)(CManager::GetRand(3) + (int)SOUND_LABEL_SE_GRASS_1));
					}
					else if (Type == RANDTYPE_SAND)
					{
						//	pSound->PlaySoundA((SOUND_LABEL)(CManager::GetRand(3) + (int)SOUND_LABEL_SE_SAND_1));
					}
				}
			}
		}
	}

	// �ʒu�X�V
	pos += m_move;

	// �W�����v���Ă��Ȃ��Ƃ�
	if (!m_bJump)
	{
		// ����
		m_move.x += (0 - m_move.x) * CManager::GetSpeedDampingRate();
		m_move.z += (0 - m_move.z) * CManager::GetSpeedDampingRate();
	}

	////�d�͏���
	//if (m_bJump)
	//{// �W�����v���Ă����Ƃ�
	//	if (animType != ANIMATIONTYPE_JUMP_1)
	//	{
	//		if (pos.y > fHeight)
	//		{// ���݂̍���������荂�������Ƃ�
	//			m_move.y += -0.7f;
	//		}
	//		else
	//		{// ���݂̍����������Ⴉ������
	//			pos.y = fHeight;											// ���̍��������߂�
	//			m_move.y = 0.0f;
	//			if (m_bJump)
	//			{
	//				// �����̃G�t�F�N�g�\��
	//				CEffect::SandSmokeEffect(pos);
	//				m_bJump = false;										// �W�����v�����ς���
	//				SetAnimPlayState(true);									// �A�j���[�V�����̍ĊJ
	//				AnimationSwitch(ANIMATIONTYPE_JUMP_5);					// �A�j���[�V�����̕ύX
	//				pSound->PlaySoundA(SOUND_LABEL_SE_LANDING);				// ���n���̍Đ�
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	pos.y = fHeight;											// ���̍��������߂�
	//}

	// �ʒu�ݒ�
	SetPosition(pos);

	if (m_bColliderWithWall)
	{
		// �����蔻��Ǘ�����
		Collision();
	}

	if (m_pColPlayerSphere != NULL)
	{// ����̓����蔻�肪���݂��Ă����Ƃ�
		m_pColPlayerSphere->SetPosition(pos);
	}

	// �L�����N�^�[�̍X�V����
	CCharacter::Update();

	if (m_pRank != NULL)
	{
		m_pRank->SetNumber(pNetwork->GetRank(pNetwork->GetId()));
	}

	if (m_pDistanceNext != NULL)
	{
		m_pDistanceNext->Update();
	}

	// �h���t�g���Ă����Ƃ�
	if (m_bDrift[DRIFT_LEFT] || m_bDrift[DRIFT_RIGHT])
	{
		// �p�[�e�B�N���S�̂̈ʒu�v�Z
		D3DXVECTOR3 vecPos = D3DXVECTOR3(-25.0f, 0.0f, -5.0f);
		D3DXMATRIX mtxMeshRot, mtxMeshTrans;				// �v�Z�p�}�g���b�N�X
		D3DXMATRIX mtx;										// ����̃}�g���b�N�X
		D3DXMATRIX mtxPlayer;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtx);

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxPlayer);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, vecPos.x, vecPos.y, vecPos.z);
		D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshTrans);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		D3DXMatrixMultiply(&mtx, &mtx, &mtxPlayer);

		for (int nCount = 0; nCount < 2; nCount++)
		{
			CEffect::CreateEffect("spark", D3DXVECTOR3(mtx._41, mtx._42, mtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}

		// �p�[�e�B�N���S�̂̈ʒu�v�Z
		vecPos = D3DXVECTOR3(25.0f, 0.0f, -5.0f);

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtx);

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxPlayer);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, vecPos.x, vecPos.y, vecPos.z);
		D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshTrans);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		D3DXMatrixMultiply(&mtx, &mtx, &mtxPlayer);

		for (int nCount = 0; nCount < 2; nCount++)
		{
			CEffect::CreateEffect("spark", D3DXVECTOR3(mtx._41, mtx._42, mtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}

	if (CSpeed::GetSpeed() > 10)
	{
		CEffect::SandSmoke(pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	}

#ifdef _DEBUG
	CDebugProc::Log("���x : %.2f", CSpeed::GetSpeed());

	Debug();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	CCharacter::Draw();
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CPlayer *CPlayer::Create(void)
{
	CPlayer *pPlayer;
	pPlayer = new CPlayer(CScene::PRIORITY_PLAYER);
	pPlayer->Init();
	return pPlayer;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CPlayer::Load(void)
{
	return S_OK;
}

//=============================================================================
// �K�E�Z�|�C���g���̐ݒ�
//=============================================================================
void CPlayer::SetDeathblow(float nValue)
{
	m_fDeathblow = nValue;
}

//=============================================================================
// �C�x���g�t���O�̐ݒ�
//=============================================================================
void CPlayer::SetEvent(bool bValue)
{
	m_bEvent = bValue;
}

//=============================================================================
// �S�[���t���O�̐ݒ�
//=============================================================================
void CPlayer::SetGoalState(bool bValue)
{
	m_bGoal = bValue;
}

//=============================================================================
// �����蔻��(trigger)
//=============================================================================
void CPlayer::OnTriggerEnter(CCollider *col)
{
	std::string sTag = col->GetTag();
	CModel *pModel = GetModel();
	std::vector<CObject*> pointObj = CObject::GetPointObj();

	if (col->GetScene()->GetObjType() == PRIORITY_ENEMY)
	{
		if (sTag == "weapon")
		{
			CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾
		//	pSound->PlaySoundA(SOUND_LABEL_SE_PUNCH);			// �_���[�W���̍Đ�
			m_nLife -= 5;										// �̗͂����
			AnimationSwitch(ANIMATIONTYPE_DAMAGE);				// �A�j���[�V������ύX

			D3DXVECTOR3 vec;

			vec = GetPosition() - col->GetPosition();			//���������߂�(���������߂邽��)
			D3DXVec3Normalize(&vec, &vec);						//���K������

			m_move.x = vec.x * 25;
			m_move.z = vec.z * 25;

			if (m_nLife < 0)
			{
				CResult::SetIdxKill(CEnemy::GetEnemyKill());			// Kill�������U���g�ɓn��
				CFade::SetFade(CManager::MODE_RESULT, CFade::FADETYPE_SLIDE);					// ���U���g�ɑJ��
			}
		}
	}
	if (sTag == "house")
	{

	}
	if (sTag == "goal")
	{
		CNetwork *pNetwork = CManager::GetNetwork();
		pNetwork->SendTCP("GOAL", sizeof("GOAL"));
		m_bEvent = true;

		//if (CFade::GetFade() == CFade::FADE_NONE)
		//{//�t�F�[�h�����������Ă��Ȃ��Ƃ�
		//	CFade::SetFade(CManager::MODE_RESULT, CFade::FADETYPE_NORMAL);					//�t�F�[�h������
		//}
	}
}

//=============================================================================
// �����蔻��(collider)
//=============================================================================
void CPlayer::OnCollisionEnter(CCollider *col)
{
	std::string sTag = col->GetTag();

	if (sTag == "enemy")
	{
		 //�������ԂȂ̂ŁA���点��
		//D3DXVECTOR3 move;
		//CManager::calcReflectVector(&move, m_move, normal);
		//m_move = move * 20;

		// �p�[�e�B�N���̍Č�
		for (int nCount = 0; nCount < 5; nCount++)
		{
			CEffect::CreateEffect("star", GetPosition(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
}

//========================================================================================
// �A�j���[�V�����t���[���̍ő吔�ɓ��B�����Ƃ��̏���
//========================================================================================
void CPlayer::BehaviorForMaxFrame(void)
{

}

//========================================================================================
// �A�j���[�V�����L�[�̍ő吔�ɓ��B�����Ƃ��̏���
//========================================================================================
void CPlayer::BehaviorForMaxKey(void)
{
	CModel *pModel = GetModel();
	ANIMATIONTYPE animType = (ANIMATIONTYPE)GetAnimType();
	D3DXVECTOR3 rot = pModel[0].GetRotation();		// ��]�ʎ擾

}

//=============================================================================
// �߂��ɂ���G�̋߂��܂ňړ����鏈��
//=============================================================================
void CPlayer::MoveNearEnemy(void)
{
	bool bTarget = false;				// �͈͓��ɓG�����݂��Ă����Ƃ�
	float fMinDistance = 750000.0f;		// ���݂̍ŒZ����
	float fDistance = 0.0f;				// ���݂̋���
	D3DXVECTOR3 enemyPos;				// �ŒZ�����ɂ���G�̈ʒu
	CScene *pSceneNext = NULL;														// ������
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_ENEMY);					// �擪�A�h���X�̎擾

	// �����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_ENEMY);		//����A�b�v�f�[�g�Ώۂ��T����
		fDistance = CManager::GetDistance(GetPosition(), pSceneNow->GetPosition());		// ���������߂�

		if (fDistance < fMinDistance)
		{
			if (!bTarget)
			{// �^�[�Q�b�g�̑��݂��m�F����Ă��Ȃ������Ƃ�
				bTarget = true;				// �ʒu�̈ړ���������
			}

			enemyPos = pSceneNow->GetPosition();			// �G�̈ʒu���L�^����
		}

		pSceneNow = pSceneNext;								//����A�b�v�f�[�g�Ώۂ��i�[
	}

	if (bTarget)
	{// �ړ��̋����o�Ă����Ƃ�
		SetPosition(enemyPos);			// �ʒu�̈ړ�
	}
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
void CPlayer::Collision(void)
{
	// �ǂ̓����蔻��
	m_bHit = CollisionWall();
}

//=============================================================================
// �L�[���͏�񏈗�
//=============================================================================
void CPlayer::Input(void)
{
	ANIMATIONTYPE animType = (ANIMATIONTYPE)GetAnimType();

	// �L�[�{�[�h�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h�̎擾
	CInputController *pGamepad = CManager::GetInputController();

	// �J�����̎擾
	CCamera *pCamera = CManager::GetCamera();

	// ���f���̎擾
	CModel *pModel = GetModel();

	D3DXVECTOR3 rot = pCamera->GetRotation();
	D3DXVECTOR3 Diff;	// �v�Z�p�i�[�ϐ�
	float nValueH = 0;									//�R���g���[���[
	float nValueV = 0;									//�R���g���[���[

	D3DXVECTOR3 modelFrontDiff = D3DXVECTOR3(0, 0, 0);			// ���f���O�֌v�Z�p�}�g���b�N�X
	D3DXVECTOR3 aVec = D3DXVECTOR3(0, 0, 0);					// �v���C���[�����x�x�N�g��
	D3DXVECTOR3 cameraVec = D3DXVECTOR3(0, 0, 0);				// �J�����̕����x�N�g��
	D3DXVECTOR3 moveVec = D3DXVECTOR3(0, 0, 0);					// �v���C���[�^���x�N�g��
	D3DXVECTOR3 fModelRot = pModel[MODEL_FRONT].GetRotation();	// ���f���O�։�]���
	D3DXVECTOR3 fModelRotRear = pModel[MODEL_REAR].GetRotation();// ���f����։�]���
	float fTireRotSpeed = 0.0f;									// �^�C����]���x
	m_fSpeed = 0;

	// ====================== �R���g���[���[ ====================== //
	if (!m_bEvent)
	{// �C�x���g���������Ă��Ȃ������Ƃ�
		if (pGamepad != NULL)
		{// �Q�[���p�b�h�����݂����Ƃ�
			if (pGamepad->GetJoypadUse(0))
			{// �g�p�\�������Ƃ�
				pGamepad->GetJoypadStickLeft(0, &nValueH, &nValueV);

				//�㉺����
				if (pGamepad->GetControllerPress(0, JOYPADKEY_A))
				{
					// �O�փ��f���̍ŏI�ړI���W
					m_dest.y = 0.0f;

					// ���x�ݒ�
					m_fSpeed = -m_fPuzzleMaxSPeed;

					// �^�C����]�����ݒ�
					fTireRotSpeed = TIRE_ROT_SPEED;

					// �����Ă���
					m_bMove = true;

					// �A�N�Z���{�^����������
					m_bAccel = true;
				}
				else if (pGamepad->GetControllerPress(0, JOYPADKEY_B))
				{
					// �O�փ��f���̍ŏI�ړI���W
					m_dest.y = 0.0f;

					// ���x�ݒ�
					m_fSpeed = m_fPuzzleMaxSPeed;

					// �^�C����]�����ݒ�
					fTireRotSpeed = -TIRE_ROT_SPEED;

					// �����Ă���
					m_bMove = true;
				}

				// �A�N�Z���{�^���𗣂����Ƃ�
				if (!pGamepad->GetControllerPress(0, JOYPADKEY_A))
				{
					// �A�N�Z���{�^���𗣂���
					m_bAccel = false;
				}

				// �h���t�g�{�^���������Ă��Ȃ��Ƃ�
				if (!pGamepad->GetControllerPress(0, JOYPADKEY_RIGHT_TRIGGER))
				{
					// ���ɃX�e�B�b�N���|�ꂽ�Ƃ�
					if (nValueH <= JOY_MAX_RANGE && nValueH > 0)
					{
						// �O�փ��f���̍ŏI�ړI���W
						m_dest.y = -CManager::GetTurnVelocity();
					}
					else if (nValueH >= -JOY_MAX_RANGE && nValueH < 0)
					{// �E�ɃX�e�B�b�N���|�ꂽ�Ƃ�
					 // �O�փ��f���̍ŏI�ړI���W
						m_dest.y = CManager::GetTurnVelocity();
					}

					// �u���[�L�{�^���������ꂽ�Ƃ�
					if (pGamepad->GetControllerPress(0, JOYPADKEY_B))
					{
						// ���ɃX�e�B�b�N���|�ꂽ�Ƃ�
						if (nValueH <= JOY_MAX_RANGE && nValueH > 0)
						{
							// �O�փ��f���̍ŏI�ړI���W
							m_dest.y = CManager::GetTurnVelocity();
						}
						else if (nValueH >= -JOY_MAX_RANGE && nValueH < 0)
						{// �E�ɃX�e�B�b�N���|�ꂽ�Ƃ�
						 // �O�փ��f���̍ŏI�ړI���W
							m_dest.y = -CManager::GetTurnVelocity();
						}
					}
				}

				// �A�N�Z����Ԃ̂Ƃ�
				if (m_bAccel)
				{
					// �h���t�g���Ă��Ȃ��Ƃ�
					if (!m_bDrift[DRIFT_RIGHT] && !m_bDrift[DRIFT_LEFT])
					{
						// �h���t�g�{�^�����������Ƃ�
						if (pGamepad->GetControllerPress(0, JOYPADKEY_RIGHT_TRIGGER))
						{
							// ���ɃX�e�B�b�N���|�ꂽ�Ƃ�
							if (nValueH <= JOY_MAX_RANGE && nValueH > 0)
							{
								// �h���t�g���Ă����Ԃɂ���
								m_bDrift[DRIFT_LEFT] = true;
							}
							else if (nValueH >= -JOY_MAX_RANGE && nValueH < 0)
							{// �E�ɃX�e�B�b�N���|�ꂽ�Ƃ�
								// �h���t�g���Ă����Ԃɂ���
								m_bDrift[DRIFT_RIGHT] = true;
							}
						}
					}

					// �E�h���t�g���Ă���Ƃ�
					if (m_bDrift[DRIFT_RIGHT])
					{
						// �O�փ��f���̍ŏI�ړI�n���W
						m_dest.y = DRIFT_DEST;

						// ���ɃX�e�B�b�N���|�ꂽ�Ƃ�
						if (nValueH <= JOY_MAX_RANGE && nValueH > 0)
						{
							// �O�փ��f���̍ŏI�ړI�n���W
							m_dest.y = 0.0f;

							// �����x
							m_fAcceleration -= ACCEKERATION_ADDITION;
						}
						else if (nValueH >= -JOY_MAX_RANGE && nValueH < 0)
						{// �E�ɃX�e�B�b�N���|�ꂽ�Ƃ�
						 // �O�փ��f���̍ŏI�ړI�n���W
							m_dest.y = ROT_SPEED_DRIFT;

							// �����x
							m_fAcceleration += ACCEKERATION_ADDITION;
						}

						// �����x�x�N�g���ݒ�
						aVec.x = sinf(m_rot.y + m_dest.y + D3DX_PI / 2) * m_fAcceleration + m_fSlopeSpeed;
						aVec.z = cosf(m_rot.y + m_dest.y + D3DX_PI / 2) * m_fAcceleration + m_fSlopeSpeed;

						// �h���t�g�{�^���𗣂����Ƃ�
						if (!pGamepad->GetControllerPress(0, JOYPADKEY_RIGHT_TRIGGER))
						{
							// �h���t�g�ő�܂ŃJ�E���g
							for (int nCnt = 0; nCnt < DRIFT_MAX; nCnt++)
							{
								// �h���t�g���Ă��Ȃ���Ԃɂ���
								m_bDrift[nCnt] = false;

								// �����x������
								m_fAcceleration = ACCEKERATION;
							}
						}
					}
					else if (m_bDrift[DRIFT_LEFT])
					{// ���h���t�g�̂Ƃ�
						// �O�փ��f���̍ŏI�ړI�n���W
						m_dest.y = -DRIFT_DEST;

						// ���ɃX�e�B�b�N���|�ꂽ�Ƃ�
						if (nValueH <= JOY_MAX_RANGE && nValueH > 0)
						{
							// �O�փ��f���̍ŏI�ړI�n���W
							m_dest.y = -ROT_SPEED_DRIFT;

							// �����x
							m_fAcceleration += ACCEKERATION_ADDITION;
						}
						else if (nValueH >= -JOY_MAX_RANGE && nValueH < 0)
						{// �E�ɃX�e�B�b�N���|�ꂽ�Ƃ�
							// �O�փ��f���̍ŏI�ړI�n���W
							m_dest.y = 0.0f;

							// �����x
							m_fAcceleration -= ACCEKERATION_ADDITION;
						}

						// �����x�x�N�g���ݒ�
						aVec.x = sinf(m_rot.y + m_dest.y - D3DX_PI / 2) * m_fAcceleration + m_fSlopeSpeed;
						aVec.z = cosf(m_rot.y + m_dest.y - D3DX_PI / 2) * m_fAcceleration + m_fSlopeSpeed;

						// �h���t�g�{�^���𗣂����Ƃ�
						if (!pGamepad->GetControllerPress(0, JOYPADKEY_RIGHT_TRIGGER))
						{
							// �h���t�g�ő�܂ŃJ�E���g
							for (int nCnt = 0; nCnt < DRIFT_MAX; nCnt++)
							{
								// �h���t�g���Ă��Ȃ���Ԃɂ���
								m_bDrift[nCnt] = false;

								// �����x������
								m_fAcceleration = ACCEKERATION;
							}
						}
					}
				}

#ifdef _DEBUG
				CDebugProc::Log("�ړ��� : %.2f %.2f %.2f", m_move.x, m_move.y, m_move.z);

				if (pGamepad->GetControllerPress(0, JOYPADKEY_A))
				{
					CDebugProc::Log("�R���g���[���[ : 0��\n");
				}
				if (pGamepad->GetControllerPress(1, JOYPADKEY_A))
				{
					CDebugProc::Log("�R���g���[���[ : 1��\n");
				}

				CDebugProc::Log("�R���g���[���[H : %f\n", nValueH);
				CDebugProc::Log("�R���g���[���[V : %f\n", nValueV);
#endif
			}
		}

		// ====================== �L�[�{�[�h ====================== //

		if (pKeyboard->GetTriggerKeyboard(DIK_1))
		{
			for (int nCount = 0; nCount < 20; nCount++)
			{
				float fAngle = float(CManager::GetRand(314)) - float(CManager::GetRand(314));
				float fAngle_x = float(CManager::GetRand(314)) - float(CManager::GetRand(314));

				D3DXVECTOR3 particlePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				particlePos.x = cosf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);
				particlePos.y = sinf(D3DX_PI + fAngle_x);
				particlePos.z = sinf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);

				fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;
				D3DXVECTOR3 rot;
				rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);

				CEffect::SetEffect("data/tex/effect/rose_01.png",			// �p�[�e�B�N���̃^�C�v
					GetPosition(),											// �����ʒu
					D3DXVECTOR3(8.0f, 8.0f, 0.0f),							// �T�C�Y
					particlePos * 5.0f,										// �����x�N�g��
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXVECTOR3(0.02f, 0.02f, 0.0f),						// ��]�̕ω���
					D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),						// �F�̕ω���
					EASINGTYPE_NONE,
					rot,													// �e�N�X�`���̉�]
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
					200,													// �p�[�e�B�N���̐����J�E���g��
					-0.98f,													// �d��
					0,														// ��R
					true,													// �r���{�[�h
					0,														// �\������ӏ�(��)
					0,														// �\������ӏ�(�c)
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					0,
					0,
					0,
					D3DXVECTOR2(1.0f, 1.0f),								// �摜�̕�����
					false,													// ���Z�����̗L��
					false,													// Z�o�b�t�@�̔�r�L��
					false													// �t�F�[�h
				);
			}

		}

		if (pKeyboard->GetTriggerKeyboard(DIK_2))
		{
			CEffect::PetalCluster(GetPosition(), D3DXVECTOR3(D3DX_PI / 2, m_rot.y, m_rot.z));
		}
	}

	//static int nCntAura = 0;
	//nCntAura++;

	//CEffect::Aura(nCntAura, false, GetPosition());

	// ���f��������Ƃ�
	if (pModel != NULL)
	{
		if (!m_bEvent)
		{// �C�x���g���������Ă��Ȃ������Ƃ�

		//�㉺����
			if (pKeyboard->GetPressKeyboard(MOVE_ACCEL))
			{
				// �O�փ��f���̍ŏI�ړI���W
				m_dest.y = 0.0f;

				// ���x�ݒ�
				m_fSpeed = -m_fPuzzleMaxSPeed;

				// �^�C����]�����ݒ�
				fTireRotSpeed = TIRE_ROT_SPEED;

				// �����Ă���
				m_bMove = true;

				// �A�N�Z���{�^����������
				m_bAccel = true;
			}
			else if (pKeyboard->GetPressKeyboard(MOVE_BRAKE))
			{
				// �O�փ��f���̍ŏI�ړI���W
				m_dest.y = 0.0f;

				// ���x�ݒ�
				m_fSpeed = m_fPuzzleMaxSPeed;

				// �^�C����]�����ݒ�
				fTireRotSpeed = -TIRE_ROT_SPEED;

				// �����Ă���
				m_bMove = true;
			}

			// �A�N�Z���{�^���𗣂����Ƃ�
			if (!pKeyboard->GetPressKeyboard(MOVE_ACCEL))
			{
				// �A�N�Z���{�^���𗣂���
				m_bAccel = false;
			}

			// �h���t�g�{�^���������Ă��Ȃ��Ƃ�
			if (!pKeyboard->GetPressKeyboard(MOVE_DRIFT))
			{
				//���E����
				if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
				{
					// �O�փ��f���̍ŏI�ړI���W
					m_dest.y = -CManager::GetTurnVelocity();
				}
				else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
				{
					// �O�փ��f���̍ŏI�ړI���W
					m_dest.y = CManager::GetTurnVelocity();
				}

				// �u���[�L�{�^���������ꂽ�Ƃ�
				if (pKeyboard->GetPressKeyboard(MOVE_BRAKE))
				{
					//���E����
					if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
					{
						// �O�փ��f���̍ŏI�ړI���W
						m_dest.y = CManager::GetTurnVelocity();
					}
					else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
					{
						// �O�փ��f���̍ŏI�ړI���W
						m_dest.y = -CManager::GetTurnVelocity();
					}
				}
			}
		}

		// �A�N�Z����Ԃ̂Ƃ�
		if (m_bAccel)
		{
			// �h���t�g���Ă��Ȃ��Ƃ�
			if (!m_bDrift[DRIFT_RIGHT] && !m_bDrift[DRIFT_LEFT])
			{
				// �h���t�g�{�^�����������Ƃ�
				if (pKeyboard->GetPressKeyboard(MOVE_DRIFT))
				{
					//���E����
					if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
					{
						// �h���t�g���Ă����Ԃɂ���
						m_bDrift[DRIFT_LEFT] = true;
					}
					else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
					{
						// �h���t�g���Ă����Ԃɂ���
						m_bDrift[DRIFT_RIGHT] = true;
					}
				}
			}

			// �E�h���t�g���Ă���Ƃ�
			if (m_bDrift[DRIFT_RIGHT])
			{
				// �O�փ��f���̍ŏI�ړI�n���W
				m_dest.y = DRIFT_DEST;

				//���E����
				if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
				{
					// �O�փ��f���̍ŏI�ړI�n���W
					m_dest.y = 0.0f;

					// �����x
					m_fAcceleration -= ACCEKERATION_ADDITION;
				}
				else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
				{
					// �O�փ��f���̍ŏI�ړI�n���W
					m_dest.y = ROT_SPEED_DRIFT;

					// �����x
					m_fAcceleration += ACCEKERATION_ADDITION;
				}

				// �����x�x�N�g���ݒ�
				aVec.x = sinf(m_rot.y + m_dest.y + D3DX_PI / 2) * m_fAcceleration + m_fSlopeSpeed;
				aVec.z = cosf(m_rot.y + m_dest.y + D3DX_PI / 2) * m_fAcceleration + m_fSlopeSpeed;

				// �h���t�g�{�^���𗣂����Ƃ�
				if (!pKeyboard->GetPressKeyboard(MOVE_DRIFT))
				{
					// �h���t�g�ő�܂ŃJ�E���g
					for (int nCnt = 0; nCnt < DRIFT_MAX; nCnt++)
					{
						// �h���t�g���Ă��Ȃ���Ԃɂ���
						m_bDrift[nCnt] = false;

						// �����x������
						m_fAcceleration = ACCEKERATION;
					}
				}
			}
			else if (m_bDrift[DRIFT_LEFT])
			{// ���h���t�g�̂Ƃ�
			 // �O�փ��f���̍ŏI�ړI�n���W
				m_dest.y = -DRIFT_DEST;

				//���E����
				if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
				{
					// �O�փ��f���̍ŏI�ړI�n���W
					m_dest.y = -ROT_SPEED_DRIFT;

					// �����x
					m_fAcceleration += ACCEKERATION_ADDITION;
				}
				else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
				{
					// �O�փ��f���̍ŏI�ړI�n���W
					m_dest.y = 0.0f;

					// �����x
					m_fAcceleration -= ACCEKERATION_ADDITION;
				}

				// �����x�x�N�g���ݒ�
				aVec.x = sinf(m_rot.y + m_dest.y - D3DX_PI / 2) * m_fAcceleration + m_fSlopeSpeed;
				aVec.z = cosf(m_rot.y + m_dest.y - D3DX_PI / 2) * m_fAcceleration + m_fSlopeSpeed;

				// �h���t�g�{�^���𗣂����Ƃ�
				if (!pKeyboard->GetPressKeyboard(MOVE_DRIFT))
				{
					// �h���t�g�ő�܂ŃJ�E���g
					for (int nCnt = 0; nCnt < DRIFT_MAX; nCnt++)
					{
						// �h���t�g���Ă��Ȃ���Ԃɂ���
						m_bDrift[nCnt] = false;

						// �����x������
						m_fAcceleration = ACCEKERATION;
					}
				}
			}
		}

		{// �O�֐ݒ� & �^�C���ݒ�
			// ���f���̉�]�ƖڕW�n�_�̍����i�[
			modelFrontDiff.y = fModelRot.y - m_dest.y;

			// ��]�̕␳
			CTakaseiLibrary::RotRevision(&modelFrontDiff);

			// ���f�������X�ɉ�]�����Ă���
			fModelRot.y -= modelFrontDiff.y * ROT_AMOUNT;
		}

		{// �J�����ݒ�
			// �^�C���̉�]�̔��������Ɗi�[
			cameraVec.y = m_cameraRot.y - (m_rot.y + (m_dest.y * 0.85f));

			// ��]�̕␳
			CTakaseiLibrary::RotRevision(&cameraVec);

			// �J���������X�ɉ�]�����Ă���
			m_cameraRot.y -= cameraVec.y * CAMERA_ROT_SPEED;

			// ��]�̕␳
			CTakaseiLibrary::RotRevision(&m_cameraRot);

			// �J������]�ݒ�
			SetCameraRot(m_cameraRot);
		}

		// �ړ����Ă����Ƃ�
		if (m_bMove)
		{
			// �^�C������
			fModelRot.x -= fTireRotSpeed;
			fModelRotRear.x -= fTireRotSpeed;

			// ��]�̕␳
			CTakaseiLibrary::RotRevision(&fModelRot);
			// ��]�̕␳
			CTakaseiLibrary::RotRevision(&fModelRotRear);

			// ���f���̉�]�̐ݒ�
			pModel[MODEL_FRONT].SetRotation(fModelRot);
			// ���f���̉�]�̐ݒ�
			pModel[MODEL_REAR].SetRotation(fModelRotRear);

			// �h���t�g���Ă���Ƃ�
			if (m_bDrift[DRIFT_RIGHT] || m_bDrift[DRIFT_LEFT])
			{
				// �^���x�N�g���v�Z
				moveVec.x += sinf(m_rot.y) * m_fSpeed * DRIFT_DECREACE;
				moveVec.z += cosf(m_rot.y) * m_fSpeed * DRIFT_DECREACE;
			}
			else
			{
				// �^���x�N�g���v�Z
				moveVec.x += sinf(m_rot.y) * m_fSpeed;
				moveVec.z += cosf(m_rot.y) * m_fSpeed;
			}

			// ��]�̕␳
			CTakaseiLibrary::RotRevision(&m_dest);

			// �v���C���[�����X�ɉ�]�����Ă���
			m_rot.y += m_dest.y * ROT_AMOUNT;
		}

		// �A�N�Z���{�^���ƃu���[�L�{�^���𗣂����Ƃ�
		if (!pGamepad->GetControllerPress(0, JOYPADKEY_A) && !pGamepad->GetControllerPress(0, JOYPADKEY_B) &&
			!pKeyboard->GetPressKeyboard(MOVE_ACCEL) && !pKeyboard->GetPressKeyboard(MOVE_BRAKE))
		{
			// �ړ��s��
			m_bMove = false;
		}

		// �ړ��ʐݒ�
		m_move.x += (moveVec.x + aVec.x) / 2;
		m_move.z += (moveVec.z + aVec.z) / 2;

		// ��]�̕␳
		CTakaseiLibrary::RotRevision(&m_rot);

		// ��]�̐ݒ�
		SetRotation(m_rot);
	}

	CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾
	if (pKeyboard->GetTriggerKeyboard(DIK_5))
	{

		pSound->PlaySoundA(SOUND_LABEL_SE_kyuin);			// �_���[�W���̍Đ�

	}
	if (pKeyboard->GetTriggerKeyboard(DIK_4))
	{

		pSound->PlaySoundA(SOUND_LABEL_SE_Cheers);			// �_���[�W���̍Đ�

	}
	if (pKeyboard->GetTriggerKeyboard(DIK_6))
	{

		pSound->PlaySoundA(SOUND_LABEL_SE_Horn);			// �_���[�W���̍Đ�

	}

#ifdef _DEBUG
	if (pKeyboard->GetTriggerKeyboard(DIK_K))
	{// K�������ꂽ�Ƃ�
		m_fDeathblow = 50.0f;				// �K�E�Z�|�C���g���ő�l�܂ŏグ��
	}
	{// K�������ꂽ�Ƃ�
		m_fDeathblow = 50.0f;				// �K�E�Z�|�C���g���ő�l�܂ŏグ��
	}

#endif
}

//=============================================================================
// �ǂ̓����蔻��
//=============================================================================
bool CPlayer::CollisionWall(void)
{
	CScene *pSceneNext = NULL;														// ������
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_WALL);					// �擪�A�h���X�̎擾
	D3DXVECTOR3 normal = D3DXVECTOR3_ZERO;

	// �����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_WALL);		//����A�b�v�f�[�g�Ώۂ��T����
		CMeshWall *pMeshWall = (CMeshWall*)pSceneNow;

		if (pMeshWall->GetWallTest(this, normal, m_rot))
		{
			D3DXVECTOR3 playerPos = GetPosition();

			OutputDebugString("��������");

			// //�������ԂȂ̂ŁA���点��
			D3DXVECTOR3 move;
			CManager::calcReflectVector(&move, m_move, normal);
			m_move = move * 20;
			return true;
		}

		pSceneNow = pSceneNext;								//����A�b�v�f�[�g�Ώۂ��i�[
	}

	return false;
}

//=============================================================================
// ���C�ɂ��ǂ̓����蔻��
//=============================================================================
bool CPlayer::CollisionWallWithRay(void)
{
	FLOAT fDistance = 0;
	D3DXVECTOR3 vNormal;

	D3DXVECTOR3 rot = CManager::GetCamera()->GetRotation();
	D3DXVECTOR3 endPoint;
	endPoint.x = sinf(D3DX_PI * 1.0f + rot.y);
	endPoint.y = 0.0f;
	endPoint.z = cosf(D3DX_PI * 1.0f + rot.y);

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 playerPos = pos;
	CModel *pModel = GetModel();
	playerPos.y += 15.0f;

	CDebugProc::Log("�����x : %.2f %.2f %.2f\n", m_move.x, m_move.y, m_move.z);
	CDebugProc::Log("�n�_ : %.2f %.2f %.2f\n", pos.x, pos.y, pos.z);
	CDebugProc::Log("�I�_ : %.2f %.2f %.2f\n", playerPos.x + m_move.x, playerPos.y + m_move.y, playerPos.z + m_move.z);

	if (CObject::Collide(playerPos, m_move, &fDistance, &vNormal, pModel[0].GetMtxWorld()) && fDistance <= 30.0f)
	{
		// �������ԂȂ̂ŁA���点��
		D3DXVECTOR3 move = CManager::Slip(m_move, vNormal);// ����x�N�g�����v�Z
		//D3DXVec3Normalize(&move, &move);
		//move *= m_fSpeed;
		m_move = move;

		// ����x�N�g����̕ǂƂ̃��C���� �Q�d�ɔ���
		if (CObject::Collide(playerPos, playerPos + endPoint, &fDistance, &vNormal, pModel[0].GetMtxWorld()) && fDistance <= 20.0f)
		{
			m_move = D3DXVECTOR3_ZERO;//�~�߂�
		}

		return true;
	}

	CDebugProc::Log("���� : %.2f", fDistance);

	return false;
}

//=============================================================================
// ��̏���
//=============================================================================
bool CPlayer::SlopeMove(void)
{
	bool bSlope = false;
	D3DXVECTOR3 pos = GetPosition();	// ���݂̈ʒu
	D3DXVECTOR3 posOld = GetPosOld();	// �ߋ��̈ʒu

	// �ߋ��̈ʒu�̂ق������������Ƃ�
	if (posOld.y > pos.y)
	{
		bSlope = true;
		m_fSlopeSpeed -= SLOPE_SPEED;
	}
	else if (posOld.y < pos.y)
	{// �ߋ��ʒu�̂ق����Ⴉ�����Ƃ�
		m_fSlopeSpeed += SLOPE_SPEED;
	}

	return bSlope;
}

//=============================================================================
// �X�s�[�h�ݒ�
//=============================================================================
void CPlayer::SetSpeed(int nCntSpeed)
{
	switch (nCntSpeed)
	{
	case 0:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0];
		break;
	case 1:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0];
		break;
	case 2:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0] + m_fPuzzleSpeed[1];
		break;
	case 3:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0] + m_fPuzzleSpeed[1] + m_fPuzzleSpeed[2];
		break;
	case 4:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0] + m_fPuzzleSpeed[1] + m_fPuzzleSpeed[2] + m_fPuzzleSpeed[3];
		break;
	case 5:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0] + m_fPuzzleSpeed[1] + m_fPuzzleSpeed[2] + m_fPuzzleSpeed[3] + m_fPuzzleSpeed[4];
		break;
	case 6:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0] + m_fPuzzleSpeed[1] + m_fPuzzleSpeed[2] + m_fPuzzleSpeed[3] + m_fPuzzleSpeed[4] +
			m_fPuzzleSpeed[5];
		break;
	case 7:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0] + m_fPuzzleSpeed[1] + m_fPuzzleSpeed[2] + m_fPuzzleSpeed[3] + m_fPuzzleSpeed[4] +
			m_fPuzzleSpeed[5] + m_fPuzzleSpeed[6];
		break;
	case 8:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0] + m_fPuzzleSpeed[1] + m_fPuzzleSpeed[2] + m_fPuzzleSpeed[3] + m_fPuzzleSpeed[4] +
			m_fPuzzleSpeed[5] + m_fPuzzleSpeed[6] + m_fPuzzleSpeed[7];
		break;
	case 9:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0] + m_fPuzzleSpeed[1] + m_fPuzzleSpeed[2] + m_fPuzzleSpeed[3] + m_fPuzzleSpeed[4] +
			m_fPuzzleSpeed[5] + m_fPuzzleSpeed[6] + m_fPuzzleSpeed[7] + m_fPuzzleSpeed[8];
		break;
	case 10:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0] + m_fPuzzleSpeed[1] + m_fPuzzleSpeed[2] + m_fPuzzleSpeed[3] + m_fPuzzleSpeed[4] +
			m_fPuzzleSpeed[5] + m_fPuzzleSpeed[6] + m_fPuzzleSpeed[7] + m_fPuzzleSpeed[8] + m_fPuzzleSpeed[9];
		break;
	case 11:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0] + m_fPuzzleSpeed[1] + m_fPuzzleSpeed[2] + m_fPuzzleSpeed[3] + m_fPuzzleSpeed[4] +
			m_fPuzzleSpeed[5] + m_fPuzzleSpeed[6] + m_fPuzzleSpeed[7] + m_fPuzzleSpeed[8] + m_fPuzzleSpeed[9] + m_fPuzzleSpeed[10];
		break;
	case 12:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0] + m_fPuzzleSpeed[1] + m_fPuzzleSpeed[2] + m_fPuzzleSpeed[3] + m_fPuzzleSpeed[4] +
			m_fPuzzleSpeed[5] + m_fPuzzleSpeed[6] + m_fPuzzleSpeed[7] + m_fPuzzleSpeed[8] + m_fPuzzleSpeed[9] + m_fPuzzleSpeed[10] +
			m_fPuzzleSpeed[11];
		break;
	case 13:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0] + m_fPuzzleSpeed[1] + m_fPuzzleSpeed[2] + m_fPuzzleSpeed[3] + m_fPuzzleSpeed[4] +
			m_fPuzzleSpeed[5] + m_fPuzzleSpeed[6] + m_fPuzzleSpeed[7] + m_fPuzzleSpeed[8] + m_fPuzzleSpeed[9] + m_fPuzzleSpeed[10] +
			m_fPuzzleSpeed[11] + m_fPuzzleSpeed[12];
		break;
	case 14:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0] + m_fPuzzleSpeed[1] + m_fPuzzleSpeed[2] + m_fPuzzleSpeed[3] + m_fPuzzleSpeed[4] +
			m_fPuzzleSpeed[5] + m_fPuzzleSpeed[6] + m_fPuzzleSpeed[7] + m_fPuzzleSpeed[8] + m_fPuzzleSpeed[9] + m_fPuzzleSpeed[10] +
			m_fPuzzleSpeed[11] + m_fPuzzleSpeed[12] + m_fPuzzleSpeed[13];
		break;
	case 15:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0] + m_fPuzzleSpeed[1] + m_fPuzzleSpeed[2] + m_fPuzzleSpeed[3] + m_fPuzzleSpeed[4] +
			m_fPuzzleSpeed[5] + m_fPuzzleSpeed[6] + m_fPuzzleSpeed[7] + m_fPuzzleSpeed[8] + m_fPuzzleSpeed[9] + m_fPuzzleSpeed[10] +
			m_fPuzzleSpeed[11] + m_fPuzzleSpeed[12] + m_fPuzzleSpeed[13] + m_fPuzzleSpeed[14];
		break;
	case 16:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0] + m_fPuzzleSpeed[1] + m_fPuzzleSpeed[2] + m_fPuzzleSpeed[3] + m_fPuzzleSpeed[4] +
			m_fPuzzleSpeed[5] + m_fPuzzleSpeed[6] + m_fPuzzleSpeed[7] + m_fPuzzleSpeed[8] + m_fPuzzleSpeed[9] + m_fPuzzleSpeed[10] +
			m_fPuzzleSpeed[11] + m_fPuzzleSpeed[12] + m_fPuzzleSpeed[13] + m_fPuzzleSpeed[14] + m_fPuzzleSpeed[15];
		break;
	case 17:
		m_fPuzzleMaxSPeed = m_fPuzzleSpeed[0] + m_fPuzzleSpeed[1] + m_fPuzzleSpeed[2] + m_fPuzzleSpeed[3] + m_fPuzzleSpeed[4] +
			m_fPuzzleSpeed[5] + m_fPuzzleSpeed[6] + m_fPuzzleSpeed[7] + m_fPuzzleSpeed[8] + m_fPuzzleSpeed[9] + m_fPuzzleSpeed[10] +
			m_fPuzzleSpeed[11] + m_fPuzzleSpeed[12] + m_fPuzzleSpeed[13] + m_fPuzzleSpeed[14] + m_fPuzzleSpeed[15] + m_fPuzzleSpeed[16];
		break;
	}
	m_fPuzzleMaxSPeed += NORMAL_SPEED;
}

//=============================================================================
// ���񑬓x�ݒ�
//=============================================================================
void CPlayer::SetTurning(int nCntTurning)
{
	switch (nCntTurning)
	{
	case 0:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0];
		break;
	case 1:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0];
		break;
	case 2:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0] + m_fPuzzleTurning[1];
		break;
	case 3:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0] + m_fPuzzleTurning[1] + m_fPuzzleTurning[2];
		break;
	case 4:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0] + m_fPuzzleTurning[1] + m_fPuzzleTurning[2] + m_fPuzzleTurning[3];
		break;
	case 5:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0] + m_fPuzzleTurning[1] + m_fPuzzleTurning[2] + m_fPuzzleTurning[3] + m_fPuzzleTurning[4];
		break;
	case 6:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0] + m_fPuzzleTurning[1] + m_fPuzzleTurning[2] + m_fPuzzleTurning[3] + m_fPuzzleTurning[4] +
			m_fPuzzleTurning[5];
		break;
	case 7:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0] + m_fPuzzleTurning[1] + m_fPuzzleTurning[2] + m_fPuzzleTurning[3] + m_fPuzzleTurning[4] +
			m_fPuzzleTurning[5] + m_fPuzzleTurning[6];
		break;
	case 8:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0] + m_fPuzzleTurning[1] + m_fPuzzleTurning[2] + m_fPuzzleTurning[3] + m_fPuzzleTurning[4] +
			m_fPuzzleTurning[5] + m_fPuzzleTurning[6] + m_fPuzzleTurning[7];
		break;
	case 9:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0] + m_fPuzzleTurning[1] + m_fPuzzleTurning[2] + m_fPuzzleTurning[3] + m_fPuzzleTurning[4] +
			m_fPuzzleTurning[5] + m_fPuzzleTurning[6] + m_fPuzzleTurning[7] + m_fPuzzleTurning[8];
		break;
	case 10:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0] + m_fPuzzleTurning[1] + m_fPuzzleTurning[2] + m_fPuzzleTurning[3] + m_fPuzzleTurning[4] +
			m_fPuzzleTurning[5] + m_fPuzzleTurning[6] + m_fPuzzleTurning[7] + m_fPuzzleTurning[8] + m_fPuzzleTurning[9];
		break;
	case 11:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0] + m_fPuzzleTurning[1] + m_fPuzzleTurning[2] + m_fPuzzleTurning[3] + m_fPuzzleTurning[4] +
			m_fPuzzleTurning[5] + m_fPuzzleTurning[6] + m_fPuzzleTurning[7] + m_fPuzzleTurning[8] + m_fPuzzleTurning[9] + m_fPuzzleTurning[10];
		break;
	case 12:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0] + m_fPuzzleTurning[1] + m_fPuzzleTurning[2] + m_fPuzzleTurning[3] + m_fPuzzleTurning[4] +
			m_fPuzzleTurning[5] + m_fPuzzleTurning[6] + m_fPuzzleTurning[7] + m_fPuzzleTurning[8] + m_fPuzzleTurning[9] + m_fPuzzleTurning[10] +
			m_fPuzzleTurning[11];
		break;
	case 13:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0] + m_fPuzzleTurning[1] + m_fPuzzleTurning[2] + m_fPuzzleTurning[3] + m_fPuzzleTurning[4] +
			m_fPuzzleTurning[5] + m_fPuzzleTurning[6] + m_fPuzzleTurning[7] + m_fPuzzleTurning[8] + m_fPuzzleTurning[9] + m_fPuzzleTurning[10] +
			m_fPuzzleTurning[11] + m_fPuzzleTurning[12];
		break;
	case 14:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0] + m_fPuzzleTurning[1] + m_fPuzzleTurning[2] + m_fPuzzleTurning[3] + m_fPuzzleTurning[4] +
			m_fPuzzleTurning[5] + m_fPuzzleTurning[6] + m_fPuzzleTurning[7] + m_fPuzzleTurning[8] + m_fPuzzleTurning[9] + m_fPuzzleTurning[10] +
			m_fPuzzleTurning[11] + m_fPuzzleTurning[12] + m_fPuzzleTurning[13];
		break;
	case 15:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0] + m_fPuzzleTurning[1] + m_fPuzzleTurning[2] + m_fPuzzleTurning[3] + m_fPuzzleTurning[4] +
			m_fPuzzleTurning[5] + m_fPuzzleTurning[6] + m_fPuzzleTurning[7] + m_fPuzzleTurning[8] + m_fPuzzleTurning[9] + m_fPuzzleTurning[10] +
			m_fPuzzleTurning[11] + m_fPuzzleTurning[12] + m_fPuzzleTurning[13] + m_fPuzzleTurning[14];
		break;
	case 16:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0] + m_fPuzzleTurning[1] + m_fPuzzleTurning[2] + m_fPuzzleTurning[3] + m_fPuzzleTurning[4] +
			m_fPuzzleTurning[5] + m_fPuzzleTurning[6] + m_fPuzzleTurning[7] + m_fPuzzleTurning[8] + m_fPuzzleTurning[9] + m_fPuzzleTurning[10] +
			m_fPuzzleTurning[11] + m_fPuzzleTurning[12] + m_fPuzzleTurning[13] + m_fPuzzleTurning[14] + m_fPuzzleTurning[15];
		break;
	case 17:
		m_fPuzzleMaxTurning = m_fPuzzleTurning[0] + m_fPuzzleTurning[1] + m_fPuzzleTurning[2] + m_fPuzzleTurning[3] + m_fPuzzleTurning[4] +
			m_fPuzzleTurning[5] + m_fPuzzleTurning[6] + m_fPuzzleTurning[7] + m_fPuzzleTurning[8] + m_fPuzzleTurning[9] + m_fPuzzleTurning[10] +
			m_fPuzzleTurning[11] + m_fPuzzleTurning[12] + m_fPuzzleTurning[13] + m_fPuzzleTurning[14] + m_fPuzzleTurning[15] + m_fPuzzleTurning[16];
		break;
	}
	m_fPuzzleMaxTurning += NORMAL_SPEED;
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CPlayer::Debug(void)
{
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posOld = GetPosOld();
	ImGui::Begin("System");													// �uSystem�v�E�B���h�E�ɒǉ����܂��B�Ȃ���΍쐬���܂��B

	if (ImGui::CollapsingHeader("player"))
	{
		ImGui::Text("pos = %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);								// �v���C���[�̌��݈ʒu��\��
		ImGui::Text("posOld = %.2f, %.2f, %.2f", posOld.x, posOld.y, posOld.z);								// �v���C���[�̌��݈ʒu��\��
		ImGui::Text("rot = %.2f, %.2f, %.2f", m_rot.x, m_rot.y, m_rot.z);								// �v���C���[�̉�]��\��
		ImGui::Text("move = %.2f, %.2f, %.2f", m_move.x, m_move.y, m_move.z);								// �v���C���[�̌��݈ʒu��\��
		ImGui::Text("HP = %d", m_nLife);				// �v���C���[�̗̑͂�\��

		ImGui::Checkbox("ColliderWithWall", &m_bColliderWithWall);

		if (ImGui::Button("BOSS"))
		{
			pos = D3DXVECTOR3(5972.14f, 100.0f, 8000.62f);
			SetPosition(pos);
		}
	}

	//�f�o�b�O�������I��
	ImGui::End();
}
#endif