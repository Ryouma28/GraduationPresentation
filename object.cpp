//=============================================================================
//
// �I�u�W�F�N�g���� [object.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "meshField.h"
#include "camera.h"
#include "house.h"
#include "colliderBox.h"
#include "colliderSphere.h"
#include "game.h"
#include "player.h"
#include "distanceNext.h"
#include "finishUi.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define OBJECT_SCRIPT "data/text/manager/manager_model.txt"

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
std::vector<CObject*> CObject::m_vPointObj = {};
CColliderSphere *CObject::m_pSphere = NULL;
unsigned int CObject::m_pointNum = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CObject::CObject(CScene::PRIORITY obj = CScene::PRIORITY_MODEL) : CSceneX(obj)
{
	SetObjType(CScene::PRIORITY_MODEL);

	m_pBox = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObject::~CObject()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CObject::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾

	// ����������
	CSceneX::Init();

	D3DXVECTOR3 pos = D3DXVECTOR3(500.0f, 0.0f, 0.0f);

	// �|�C���g�I�u�W�F�N�g�̂Ƃ�
	if (m_Add == "data/model/point.x")
	{
		// �X�t�B�A��NULL�̂Ƃ�
		if (m_pSphere == NULL)
		{
			// �X�t�B�A�̐�������
			m_pSphere = CColliderSphere::Create(true, 1000.0f);

			// �X�t�B�A������Ƃ�
			if (m_pSphere != NULL)
			{
				m_pSphere->SetScene(this);
				m_pSphere->SetTag("checkpoint");
				m_pSphere->SetPosition(pos);
			}
		}
	}
	SetPosition(pos);			// �ʒu�̐ݒ�
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CObject::Uninit(void)
{
	// �J������
	CSceneX::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CObject::Update(void)
{
	D3DXVECTOR3 pos = GetPosition();				// �ʒu�̎擾

	if (m_Add == "data/model/GoalCircle35.x")
	{
		if (m_pBox == NULL)
		{
			m_pBox = CColliderBox::Create(true, D3DXVECTOR3(400.0f, 800.0f, 1400.0f));

			if (m_pBox != NULL)
			{
				m_pBox->SetScene(this);
				m_pBox->SetTag("goal");
				m_pBox->SetPosition(pos);
				m_pBox->SetOffset(D3DXVECTOR3(0.0f, 500.0f, 0.0f));
			}
		}
	}

	SetPosition(pos);		// �ʒu�̐ݒ�

#ifdef _DEBUG
	Debug();				// �f�o�b�O����
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CObject::Draw(void)
{
	CSceneX::Draw();
}

//=============================================================================
// �N���G�C�g�֐�
//=============================================================================
CObject *CObject::Create(void)
{
	CObject *pObject = NULL;
	pObject = new CObject(CScene::PRIORITY_MODEL);

	if (pObject != NULL)
	{
		pObject->Init();
	}
	return pObject;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CObject::Load(void)
{
	return S_OK;
}

//=============================================================================
// �e�N�X�`���̉���֐�
//=============================================================================
void CObject::Unload(void)
{
}

//=============================================================================
// �I�u�W�F�N�g�A�Z�b�g�̃��[�h����
//=============================================================================
void CObject::LoadScript(void)
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
	pFile = fopen(OBJECT_SCRIPT, "r");

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
						CManager::LoadModel(Add);
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

//=============================================================================
// ���f���̓Ǎ�
//=============================================================================
void CObject::LoadModel(char *add)
{
	FILE *pFile = NULL;											// �t�@�C��
	char cReadText[128] = {};									// ����
	char cHeadText[128] = {};									// ��r
	CObject *pObject = NULL;

	pFile = fopen(add, "r");									// �t�@�C�����J���܂��͍��

	if (pFile != NULL)											// �t�@�C�����ǂݍ��߂��ꍇ
	{
		fgets(cReadText, sizeof(cReadText), pFile);				// �s���΂�
		fgets(cReadText, sizeof(cReadText), pFile);				// �s���΂�
		fgets(cReadText, sizeof(cReadText), pFile);				// �s���΂�

		while (strcmp(cHeadText, "End") != 0)
		{
			std::string Data = cReadText;
			std::vector<std::string> splitData = CManager::split(Data, ',');

			pObject = CObject::Create();
			pObject->BindModel(splitData[LOADTYPE_ADD]);
			pObject->SetPosition(D3DXVECTOR3((float)atof(splitData[LOADTYPE_POS_X].c_str()), (float)atof(splitData[LOADTYPE_POS_Y].c_str()), (float)atof(splitData[LOADTYPE_POS_Z].c_str())));			// �ʒu�̐ݒ�
			pObject->SetRotation(D3DXVECTOR3((float)atof(splitData[LOADTYPE_ROT_X].c_str()), (float)atof(splitData[LOADTYPE_ROT_Y].c_str()), (float)atof(splitData[LOADTYPE_ROT_Z].c_str())));			// ��]�l��ݒ�

			fgets(cReadText, sizeof(cReadText), pFile);									// �s���΂�
			sscanf(cReadText, "%s", &cHeadText);
		}

		fclose(pFile);					// �t�@�C�������
	}
	else
	{
		MessageBox(NULL, "���f�����̃A�N�Z�X���s�I", "WARNING", MB_ICONWARNING);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}

//=============================================================================
// ���f���ǂݍ���(�e�X�g)
//=============================================================================
void CObject::LoadModelTest(char *add)
{
	FILE *pFile = NULL;																	// �t�@�C��
	char cReadText[128];															// ����
	char cHeadText[128];															// ��r
	char cDie[128];

	pFile = fopen(add, "r");				// �t�@�C�����J���܂��͍��

	if (pFile != NULL)						//�t�@�C�����ǂݍ��߂��ꍇ
	{
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
					if (strcmp(cHeadText, "MODELSET") == 0)
					{//�L�����N�^�[�̏������̂Ƃ�
						CObject *pObj = NULL;
						pObj = CObject::Create();										// ���̍쐬

						if (pObj != NULL)
						{
							//�G���h�L�����N�^�[�Z�b�g������܂Ń��[�v
							while (strcmp(cHeadText, "END_MODELSET") != 0)
							{
								fgets(cReadText, sizeof(cReadText), pFile);
								sscanf(cReadText, "%s", &cHeadText);

								if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
								{//�p�[�c�����̂Ƃ�
									char aModelAdd[64];
									memset(&aModelAdd, 0, sizeof(aModelAdd));

									sscanf(cReadText, "%s %s %s", &cDie, &cDie,
										&aModelAdd);

									// �ʒu�̐ݒ�
									pObj->BindModel(aModelAdd);
									pObj->Init();

									// �|�C���g���f���̂Ƃ�
									if (strcmp(aModelAdd, "data/model/point.x") == 0)
									{
										m_vPointObj.push_back(pObj);
									}
								}
								else if (strcmp(cHeadText, "POS") == 0)
								{//�p�[�c�����̂Ƃ�
									D3DXVECTOR3 pos;
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&pos.x,
										&pos.y,
										&pos.z);

									// �ʒu�̐ݒ�
									pObj->SetPosition(pos);
								}
								else if (strcmp(cHeadText, "ROT") == 0)
								{//�p�[�c�����̂Ƃ�
									D3DXVECTOR3 rot;
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&rot.x,
										&rot.y,
										&rot.z);

									// �ʒu�̐ݒ�
									pObj->SetRotation(rot);
								}
								else if (strcmp(cHeadText, "SIZE") == 0)
								{//�p�[�c�����̂Ƃ�
									D3DXVECTOR3 size;
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&size.x,
										&size.y,
										&size.z);

									// �ʒu�̐ݒ�
									pObj->SetSize(size);
								}
							}
						}
					}
				}
			}
		}
		fclose(pFile);				// �t�@�C�������
	}
	else
	{
		MessageBox(NULL, "�n�ʏ��̃A�N�Z�X���s�I", "WARNING", MB_ICONWARNING);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}

//=============================================================================
// ���f�����̓Ǎ�
//=============================================================================
void CObject::BindModel(std::string add)
{
	m_Add = add;

	// ���f���̎擾
	CManager::GetModelResource(add, m_pBuffMat, m_nNumMat, m_pMesh);

	// ���f���̏����Z�b�g
	CSceneX::BindModel(m_pMesh, m_nNumMat, m_pBuffMat);
}

//=============================================================================
// �I�u�W�F�N�g�̓����蔻��
//=============================================================================
void CObject::OnTriggerEnter(CCollider *col)
{
	std::string sTag = col->GetTag();

	if (sTag == "player")
	{
		if (m_pSphere == NULL) { return; }

		if (m_pSphere->GetTag() == "checkpoint")
		{
			if (m_vPointObj.size() < m_pointNum) { return; }		// �z��̃T�C�Y�𒴂��Ȃ��悤�ɂ��鏈��

			m_pointNum++;		// �t���O�̔z������ɂ���

			// �z�񂪍ő�𒴂�����
			if (m_pointNum >= m_vPointObj.size())
			{
				// �����l�ɖ߂�
				m_pointNum = 0;
				CPlayer *pPlayer = CGame::GetPlayer();

				if (pPlayer != NULL)
				{
					CDistanceNext *pDistanceNext = pPlayer->GetDistanceNext();
					if (pDistanceNext != NULL)
					{
						if (pDistanceNext->GetNowRound() < MAX_ROUND - 1)
						{
							pDistanceNext->SetNowRound();
						}
						else
						{
							CNetwork *pNetwork = CManager::GetNetwork();
							pNetwork->SendTCP("GOAL", sizeof("GOAL"));
							pPlayer->SetEvent(true);
							CFinishUi::Create();
						}
					}
				}
			}

			m_pSphere->SetScene(m_vPointObj[m_pointNum]);			// ���̃V�[�����i�[����

			// ���a�̑傫����ς���
			if (m_pointNum == 19 || m_pointNum == 20 || m_pointNum == 22 || m_pointNum == 23 ||
				m_pointNum == 25 || m_pointNum == 26 || m_pointNum == 28 || m_pointNum == 29)
			{
				m_pSphere->SetRadius(1250.0f);
			}
			else if (m_pointNum == 2 || m_pointNum == 18 || m_pointNum == 21 ||
				m_pointNum == 24 || m_pointNum == 25)
			{
				m_pSphere->SetRadius(1500.0f);
			}
			else if (m_pointNum == 25)
			{
				m_pSphere->SetRadius(1700.0f);

			}
			else if (m_pointNum == 27)
			{
				m_pSphere->SetRadius(1850.0f);
			}
			else
			{
				m_pSphere->SetRadius(1000.0f);
			}

			// �|�C���g�ԍ��ݒ�
			SetPointNum(m_pointNum);
		}
	}
}

//=============================================================================
// �C���X�y�N�^�[��\��
//=============================================================================
void CObject::ShowInspector(void)
{
	CSceneX::ShowInspector();
}

//=============================================================================
// �����蔻��(���C)
//=============================================================================
bool CObject::Collide(D3DXVECTOR3 vStart, D3DXVECTOR3 vDir, FLOAT* pfDistance, D3DXVECTOR3* pvNormal, D3DXMATRIX mtx)
{
	BOOL boHit = false;
	D3DXMATRIX mWorld;
	D3DXVec3Normalize(&vDir, &vDir);

	CScene *pSceneNext = NULL;														//����A�b�v�f�[�g�Ώ�
	CScene *pSceneNow = NULL;

	D3DXMATRIX			invmat;							// �t�s����i�[����ϐ�
	D3DXVECTOR3			m_posAfter;						// �t�s��ŏo�����I�_�����i�[����
	D3DXVECTOR3			m_posBefore;					// �I�_�����i�[����
	D3DXVECTOR3			direction;						// �ϊ���̈ʒu�A�������i�[����ϐ�

	CDebugProc::Log("�n�_ : %.2f %.2f %.2f\n", vStart.x, vStart.y, vStart.z);
	CDebugProc::Log("�I�_ : %.2f %.2f %.2f\n", vDir.x, vDir.y, vDir.z);

	pSceneNow = CScene::GetScene(CScene::PRIORITY_MODEL);

	//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, (CScene::PRIORITY_MODEL));							//����A�b�v�f�[�g�Ώۂ��T����

		CObject *pObj = (CObject*)pSceneNow;								// �N���X�`�F���W(��)

		//// ���C�𓖂Ă郁�b�V���������Ă������]���Ă���ꍇ�ł��Ώۂ̃��[���h�s��̋t�s���p����ΐ��������C��������
		//D3DXMatrixInverse(&mWorld, NULL, &pObj->GetMtxWorld());
		//D3DXVec3TransformCoord(&vStart, &vStart, &mWorld);

		//	�t�s��̎擾
		D3DXMatrixInverse(&invmat, NULL, &pObj->GetMtxWorld());

		//	�t�s����g�p���A���C�n�_����ϊ��@�ʒu�ƌ����ŕϊ�����֐����قȂ�̂ŗv����
		D3DXVec3TransformCoord(&m_posBefore, &D3DXVECTOR3(vStart.x, mtx._42, vStart.z), &invmat);

		//	���C�I�_����ϊ�
		D3DXVec3TransformCoord(&m_posAfter, &D3DXVECTOR3(vDir.z, vStart.y, vDir.z), &invmat);

		//	���C��������ϊ�
		D3DXVec3Normalize(&direction, &(m_posAfter - m_posBefore));
		//Ray���΂�


		DWORD dwPolyIndex;
		D3DXIntersect(pObj->GetMesh(), &m_posBefore, &direction, &boHit, &dwPolyIndex, NULL, NULL, pfDistance, NULL, NULL);
		//D3DXIntersect(pObj->m_pMesh, &vStart, &vDir, &boHit, &dwPolyIndex, NULL, NULL, pfDistance, NULL, NULL);
		if (boHit)
		{
			//�������Ă���|���S���̒��_��������
			D3DXVECTOR3 vVertex[3];
			FindVerticesOnPoly(pObj->m_pMesh, dwPolyIndex, vVertex);
			D3DXPLANE p;
			//���̒��_���畽�ʕ������𓾂�
			D3DXPlaneFromPoints(&p, &vVertex[0], &vVertex[1], &vVertex[2]);
			//���ʕ������̌W�����@���̐���
			pvNormal->x = p.a;
			pvNormal->y = p.b;
			pvNormal->z = p.c;

			CDebugProc::Log("���f���̃A�h���X : %s\n", pObj->m_Add.c_str());

			return true;
		}

		pSceneNow = pSceneNext;														//����A�b�v�f�[�g�Ώۂ��i�[
	}

	return false;
}

//
//HRESULT FindVerticesOnPoly(LPD3DXMESH pMesh,DWORD dwPolyIndex,D3DXVECTOR3* pvVertices )
//���̃|���S���̒��_��������
HRESULT CObject::FindVerticesOnPoly(LPD3DXMESH pMesh, DWORD dwPolyIndex, D3DXVECTOR3* pvVertices)
{
	DWORD dwStride = pMesh->GetNumBytesPerVertex();
	DWORD dwVertexNum = pMesh->GetNumVertices();
	DWORD dwPolyNum = pMesh->GetNumFaces();
	WORD* pwPoly = NULL;
	pMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&pwPoly);

	BYTE *pbVertices = NULL;
	FLOAT* pfVetices = NULL;
	LPDIRECT3DVERTEXBUFFER9 VB = NULL;
	pMesh->GetVertexBuffer(&VB);
	if (SUCCEEDED(VB->Lock(0, 0, (VOID**)&pbVertices, 0)))
	{
		pfVetices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3]];
		pvVertices[0].x = pfVetices[0];
		pvVertices[0].y = pfVetices[1];
		pvVertices[0].z = pfVetices[2];

		pfVetices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 1]];
		pvVertices[1].x = pfVetices[0];
		pvVertices[1].y = pfVetices[1];
		pvVertices[1].z = pfVetices[2];

		pfVetices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 2]];
		pvVertices[2].x = pfVetices[0];
		pvVertices[2].y = pfVetices[1];
		pvVertices[2].z = pfVetices[2];

		pMesh->UnlockIndexBuffer();
		VB->Unlock();
		VB->Release();
	}
	return S_OK;
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CObject::Debug(void)
{

}
#endif