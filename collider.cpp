//==============================================================================
//
// �����蔻��}�l�[�W������ [collider.cpp]
// Author : masayasu wakita
//
//==============================================================================
#include "collider.h"
#include "colliderSphere.h"
#include "colliderBox.h"
#include "debug.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"

//==============================================================================
// �ÓI�����o�ϐ�
//==============================================================================
CCollider *CCollider::m_apTop[COLLISIONTYPE_MAX] = {};			// �擪���̏�����
CCollider *CCollider::m_apCur[COLLISIONTYPE_MAX] = {};			// �Ō�����̏�����
int CCollider::m_nNumAll = 0;									// �����̏�����

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CCollider::CCollider(COLLISIONTYPE Type)
{
	m_offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �I�t�Z�b�g�l

	m_pPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ʒu�̃|�C���^
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �O��ʒu�̏�����
	m_pMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ��ʂ̃|�C���^

	if (Type != COLLISIONTYPE_NONE)
	{
		if (m_apTop[Type] == NULL)
		{
			m_apTop[Type] = this;						// ���� ���g�b�v�ɐݒ�
		}

		if (m_apCur[Type] != NULL)
		{
			m_apCur[Type]->m_pNext[Type] = this;		// �Ō�� �̎� ��������
		}

		m_pPrev[Type] = m_apCur[Type];					// �Ō��������
		m_apCur[Type] = this;							// ���g���Ō���ɂ���
		m_pNext[Type] = NULL;							// �����̎���NULL
		m_Obj = Type;									// �I�u�W�F�N�g�^�C�v���`
		m_nNumAll++;									// �V�[�������v���X����
		m_bDie = false;

		m_mtxWorldParent = NULL;
		m_pScene = NULL;								// �V�[���̏�����
		m_sTag = "none";								// �^�O�̏�����
		m_bUse = true;									// �g�p���ɂ���
		m_bMoving = true;								// �ʒu�C���\�ɂ���
		m_bTrigger = true;								// �g���K�[�̏�����
	}
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CCollider::~CCollider()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CCollider::Init(void)
{
	return S_OK;																									// �� ��Ԃ�
}

//==============================================================================
// �J������
//==============================================================================
void CCollider::Uninit(void)
{

}

//==============================================================================
// �X�V����
//==============================================================================
void CCollider::Update(void)
{
	if (m_pScene != NULL)
	{
		m_pPos = m_pScene->GetPosition();
	}
}

//==============================================================================
// �`�揈��
//==============================================================================
void CCollider::Draw(void)
{

}

//==============================================================================
//�S�Ă̓����蔻���j��
//==============================================================================
void CCollider::ReleaseAll(void)
{
	CCollider *pSceneNext = NULL;																			//����폜�Ώ�
	CCollider *pSceneNow = NULL;

	for (int nCount = 0; nCount < COLLISIONTYPE_MAX; nCount++)
	{
		pSceneNow = m_apTop[nCount];

		//���S�t���O���m�F
		while (pSceneNow != NULL)
		{
			pSceneNext = pSceneNow->m_pNext[nCount];																//����t���O�m�F�Ώۂ��T����

			pSceneNow->Delete();																					//�폜

			pSceneNow = pSceneNext;																					//����t���O�m�F�Ώۂ��i�[
		}
	}
}

//==============================================================================
//�S�Ă̓����蔻����X�V
//==============================================================================
void CCollider::UpdateAll(void)
{
	CCollider *pSceneNext = NULL;																			// ����A�b�v�f�[�g�Ώ�
	CCollider *pSceneNow = NULL;																			// ����A�b�v�f�[�g�Ώ�

	CCollider *pSceneNext2 = NULL;																			// ����A�b�v�f�[�g�Ώ�
	CCollider *pSceneNow2 = NULL;																			// ����A�b�v�f�[�g�Ώ�

	D3DXVECTOR3 first;
	D3DXVECTOR3 second;

	// �J������
	for (int nCount = 0; nCount < COLLISIONTYPE_MAX; nCount++)
	{
		pSceneNow = m_apTop[nCount];

		//���S�t���O���m�F
		while (pSceneNow != NULL)
		{
			pSceneNext = pSceneNow->m_pNext[nCount];		//����t���O�m�F�Ώۂ��T����

			if (pSceneNow->m_bDie)
			{
				pSceneNow->Delete();						//�폜
				pSceneNow->m_pScene = NULL;
				pSceneNow = NULL;
			}

			pSceneNow = pSceneNext;							//����t���O�m�F�Ώۂ��i�[
		}
	}

	// ========================= Sphere of Sphere =========================== //
	pSceneNow = m_apTop[COLLISIONTYPE_SPHERE];

	//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = pSceneNow->m_pNext[COLLISIONTYPE_SPHERE];			//����A�b�v�f�[�g�Ώۂ��T����

		pSceneNow2 = m_apTop[COLLISIONTYPE_SPHERE];

		if (pSceneNow->GetUse())
		{// �����蔻��̌v�Z�Ώۂ��ǂ���

			pSceneNow->Update();

			//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
			while (pSceneNow2 != NULL)
			{
				pSceneNext2 = pSceneNow2->m_pNext[COLLISIONTYPE_SPHERE];		// ����A�b�v�f�[�g�Ώۂ��T����

				// �����蔻��̌v�Z
				if (pSceneNow2->GetUse())
				{// �����蔻��̌v�Z�Ώۂ��ǂ���
					if (pSceneNow != pSceneNow2)
					{// �Ώۂ����ꂶ��Ȃ����ǂ���
						first = pSceneNow->GetPosition();
						second = pSceneNow2->GetPosition();

						first.y = 0.0f;
						second.y = 0.0f;
						if (pSceneNow->GetTrigger() || pSceneNow2->GetTrigger())
						{
							if (SphereHitCollider(pSceneNow, pSceneNow2))
							{
								if (pSceneNow->m_pScene != NULL)
								{
									pSceneNow->m_pScene->OnTriggerEnter(pSceneNow2);
								}
							}
						}
						else
						{
							if (SphereInCollider(pSceneNow, pSceneNow2))
							{
								if (pSceneNow != NULL)
								{
									if (pSceneNow->m_pScene != NULL)
									{
										pSceneNow->m_pScene->OnCollisionEnter(pSceneNow2);
									}
								}
							}
						}
					}
				}

				pSceneNow2 = pSceneNext2;			//����A�b�v�f�[�g�Ώۂ��i�[
			}
		}

		pSceneNow = pSceneNext;				//����A�b�v�f�[�g�Ώۂ��i�[
	}

	// ========================= box of box =========================== //

	pSceneNext = NULL;														// ����A�b�v�f�[�g�Ώ�
	pSceneNow = NULL;														// ����A�b�v�f�[�g�Ώ�

	pSceneNext2 = NULL;														// ����A�b�v�f�[�g�Ώ�
	pSceneNow2 = NULL;														// ����A�b�v�f�[�g�Ώ�

	pSceneNow = m_apTop[COLLISIONTYPE_BOX];

	//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = pSceneNow->m_pNext[COLLISIONTYPE_BOX];					//����A�b�v�f�[�g�Ώۂ��T����

		pSceneNow2 = m_apTop[COLLISIONTYPE_BOX];

		if (pSceneNow->GetUse())
		{// �����蔻��̌v�Z�Ώۂ��ǂ���

			pSceneNow->Update();

			//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
			while (pSceneNow2 != NULL)
			{
				pSceneNext2 = pSceneNow2->m_pNext[COLLISIONTYPE_BOX];		// ����A�b�v�f�[�g�Ώۂ��T����

				// �����蔻��̌v�Z
				if (pSceneNow2->GetUse())
				{// �����蔻��̌v�Z�Ώۂ��ǂ���
					if (pSceneNow != pSceneNow2)
					{// �Ώۂ����ꂶ��Ȃ����ǂ���
						if (pSceneNow->GetTrigger() || pSceneNow2->GetTrigger())
						{
							if (BoxInCollider(pSceneNow, pSceneNow2))
							{
								if (pSceneNow->m_pScene != NULL)
								{
									pSceneNow->m_pScene->OnTriggerEnter(pSceneNow2);
								}
							}
						}
						else
						{
							if (BoxHitCollider(pSceneNow, pSceneNow2))
							{
								if (pSceneNow->m_pScene != NULL)
								{
									pSceneNow->m_pScene->OnCollisionEnter(pSceneNow2);
								}
							}
						}
					}
				}

				pSceneNow2 = pSceneNext2;			//����A�b�v�f�[�g�Ώۂ��i�[
			}
		}

		pSceneNow = pSceneNext;				//����A�b�v�f�[�g�Ώۂ��i�[
	}

	// ========================= box of sphere =========================== //

	pSceneNext = NULL;													// ����A�b�v�f�[�g�Ώ�
	pSceneNow = NULL;													// ����A�b�v�f�[�g�Ώ�

	pSceneNext2 = NULL;													// ����A�b�v�f�[�g�Ώ�
	pSceneNow2 = NULL;													// ����A�b�v�f�[�g�Ώ�

	pSceneNow = m_apTop[COLLISIONTYPE_BOX];

	//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = pSceneNow->m_pNext[COLLISIONTYPE_BOX];				//����A�b�v�f�[�g�Ώۂ��T����

		pSceneNow2 = m_apTop[COLLISIONTYPE_SPHERE];

		if (pSceneNow->GetUse())
		{// �����蔻��̌v�Z�Ώۂ��ǂ���

			pSceneNow->Update();

			//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
			while (pSceneNow2 != NULL)
			{
				pSceneNext2 = pSceneNow2->m_pNext[COLLISIONTYPE_SPHERE];		// ����A�b�v�f�[�g�Ώۂ��T����

				// �����蔻��̌v�Z
				if (pSceneNow2->GetUse())
				{// �����蔻��̌v�Z�Ώۂ��ǂ���
					if (pSceneNow != pSceneNow2)
					{// �Ώۂ����ꂶ��Ȃ����ǂ���

						first = pSceneNow->GetPosition();
						second = pSceneNow2->GetPosition();

						first.y = 0.0f;
						second.y = 0.0f;

						if (pSceneNow->GetTrigger() || pSceneNow2->GetTrigger())
						{
							if (CollisionHitSphereAndBox(pSceneNow, pSceneNow2))
							{
								if (pSceneNow->m_pScene != NULL)
								{
									pSceneNow->m_pScene->OnTriggerEnter(pSceneNow2);
								}
							}
						}
						else
						{
							if (CollisionHitSphereAndBox(pSceneNow, pSceneNow2))
							{
								if (pSceneNow->m_pScene != NULL)
								{
									pSceneNow->m_pScene->OnCollisionEnter(pSceneNow2);
								}
							}
						}

					}
				}

				pSceneNow2 = pSceneNext2;			//����A�b�v�f�[�g�Ώۂ��i�[
			}
		}

		pSceneNow = pSceneNext;				//����A�b�v�f�[�g�Ώۂ��i�[
	}

	//�t��
	pSceneNow = m_apTop[COLLISIONTYPE_SPHERE];

	//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = pSceneNow->m_pNext[COLLISIONTYPE_SPHERE];				//����A�b�v�f�[�g�Ώۂ��T����

		pSceneNow2 = m_apTop[COLLISIONTYPE_BOX];

		if (pSceneNow->GetUse())
		{// �����蔻��̌v�Z�Ώۂ��ǂ���

			pSceneNow->Update();

			//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
			while (pSceneNow2 != NULL)
			{
				pSceneNext2 = pSceneNow2->m_pNext[COLLISIONTYPE_BOX];		// ����A�b�v�f�[�g�Ώۂ��T����

				// �����蔻��̌v�Z
				if (pSceneNow2->GetUse())
				{// �����蔻��̌v�Z�Ώۂ��ǂ���
					if (pSceneNow != pSceneNow2)
					{// �Ώۂ����ꂶ��Ȃ����ǂ���

						first = pSceneNow->GetPosition();
						second = pSceneNow2->GetPosition();

						first.y = 0.0f;
						second.y = 0.0f;

						if (pSceneNow->GetTrigger() || pSceneNow2->GetTrigger())
						{
							if (CollisionHitSphereAndBox(pSceneNow2, pSceneNow))
							{
								if (pSceneNow->m_pScene != NULL)
								{
									pSceneNow->m_pScene->OnTriggerEnter(pSceneNow2);
								}
							}
						}
						else
						{
							if (CollisionHitSphereAndBox(pSceneNow2, pSceneNow))
							{
								if (pSceneNow->m_pScene != NULL)
								{
									pSceneNow->m_pScene->OnCollisionEnter(pSceneNow2);
								}
							}
						}
					}
				}

				pSceneNow2 = pSceneNext2;			//����A�b�v�f�[�g�Ώۂ��i�[
			}
		}

		pSceneNow = pSceneNext;				//����A�b�v�f�[�g�Ώۂ��i�[
	}
}

//==============================================================================
//�S�Ă̓����蔻���`��
//==============================================================================
void CCollider::DrawAll(void)
{
	CCollider *pSceneNext = NULL;										//����`��Ώ�
	CCollider *pSceneNow = NULL;
	for (int nCount = 0; nCount < COLLISIONTYPE_MAX; nCount++)
	{
		pSceneNow = m_apTop[nCount];

		//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
		while (pSceneNow != NULL)
		{
			pSceneNext = pSceneNow->m_pNext[nCount];					//����`��Ώۂ��T����

			if (pSceneNow->GetUse())
			{
				pSceneNow->Draw();										//�`��
			}
			pSceneNow = pSceneNext;										//����`��Ώۂ��i�[
		}
	}
}

//==============================================================================
// �ʒu�̐ݒ�
//==============================================================================
void CCollider::SetPosition(D3DXVECTOR3 pos)
{
	SetPosOld(m_pPos);
	m_pPos = pos;
}

//==============================================================================
// �O��ʒu�̐ݒ�
//==============================================================================
void CCollider::SetPosOld(D3DXVECTOR3 pos)
{
	m_posOld = pos;
}

//==============================================================================
// �X�V�`��Ώېݒ�
//==============================================================================
void CCollider::SetActive(bool bValue)
{
	m_bActive = bValue;					// ���
}

//==============================================================================
// �폜�\��
//==============================================================================
void CCollider::Release(void)
{
	if (!m_bDie)
	{
		m_bDie = true;
	}
}

//==============================================================================
// �^�O�̐ݒ�
//==============================================================================
void CCollider::SetTag(std::string sTag)
{
	m_sTag = sTag;						// �^�O �̑��
}

//==============================================================================
// �����蔻��̎������ݒ�
//==============================================================================
void CCollider::SetScene(CScene *pScene)
{
	m_pScene = pScene;					// ������ �̃|�C���^ ����
}

//==============================================================================
// �}�g���b�N�X�̐ݒ�
//==============================================================================
void CCollider::SetMatrix(D3DXMATRIX *mtx)
{
	m_mtxWorld = *mtx;
}

//==============================================================================
// �����蔻��^�C�v
//==============================================================================
void CCollider::SetTrigger(bool bValue)
{
	m_bTrigger = bValue;				// Trigger�̗L�� ����
}

//=============================================================================
// �����蔻��̎g�p�ݒ�
//==============================================================================
void CCollider::SetUse(bool bValue)
{
	m_bUse = bValue;
}

//=============================================================================
// �ʒu�C���̉ېݒ�
//==============================================================================
void CCollider::SetMoving(bool bValue)
{
	m_bMoving = bValue;
}

//==============================================================================
// �I�t�Z�b�g�̐ݒ�
//==============================================================================
void CCollider::SetOffset(D3DXVECTOR3 pos)
{
	m_offset = pos;
}

//==============================================================================
// �����蔻��`�F�b�N
//==============================================================================
void CCollider::ColliderCheck(void)
{
	COLLISIONTYPE type = this->GetColliderType();

	CCollider *pSceneNext = NULL;				//����`��Ώ�
	CCollider *pSceneNow = NULL;
	bool bJudge = false;

	pSceneNow = m_apTop[type];

	//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = pSceneNow->m_pNext[type];			//����`��Ώۂ��T����

		if (pSceneNow->GetUse())
		{
			if (this != pSceneNow)
			{// �Ώۂ����ꂶ��Ȃ����ǂ���
				if (this->GetTrigger() || pSceneNow->GetTrigger())
				{
					switch (type)
					{
					case COLLISIONTYPE_SPHERE:
						bJudge = SphereHitCollider(this, pSceneNow);
						break;
					case COLLISIONTYPE_BOX:
						bJudge = BoxInCollider(this, pSceneNow);
						break;
					}

					if (bJudge)
					{
						if (this->m_pScene != NULL)
						{
							this->m_pScene->OnTriggerEnter(pSceneNow);
						}
					}
				}
				else
				{
					switch (type)
					{
					case COLLISIONTYPE_SPHERE:
						bJudge = SphereInCollider(this, pSceneNow);
						break;
					case COLLISIONTYPE_BOX:
						bJudge = BoxHitCollider(this, pSceneNow);
						break;
					}

					if (bJudge)
					{
						if (this->m_pScene != NULL)
						{
							this->m_pScene->OnCollisionEnter(pSceneNow);
						}
					}
				}
			}
		}
		pSceneNow = pSceneNext;						//����`��Ώۂ��i�[
	}
}

//==============================================================================
// ���̂Ƌ��̂̓����蔻��(Hit)
//==============================================================================
bool CCollider::SphereHitCollider(CCollider *pFirstTarget, CCollider *pSecondTarget)		// ���̂̓����蔻��
{
	D3DXVECTOR3 save = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								// �ۑ��p
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								// ���K�������f�[�^

	CColliderSphere *pSphere = (CColliderSphere*)pFirstTarget;
	CColliderSphere *pSphere2 = (CColliderSphere*)pSecondTarget;														// �g�����ԂɃL���X�g

	D3DXVECTOR3 Target1_Pos = pSphere->GetPosition() + pSphere->GetOffset();							// Target1�̈ʒu
	D3DXVECTOR3 Target2_Pos = pSphere2->GetPosition() + pSphere2->GetOffset();							// Target2�̈ʒu

	D3DXVECTOR3 Target1_PosOld = pSphere->GetPosOld() + pSphere->GetOffset();							// Target1�̑O�̈ʒu
	D3DXVECTOR3 Target2_PosOld = pSphere2->GetPosOld() + pSphere2->GetOffset();							// Target2�̑O�̈ʒu

	if ((Target1_PosOld.x - Target2_PosOld.x) * (Target1_PosOld.x - Target2_PosOld.x) +
		(Target1_PosOld.y - Target2_PosOld.y) * (Target1_PosOld.y - Target2_PosOld.y) +
		(Target1_PosOld.z - Target2_PosOld.z) * (Target1_PosOld.z - Target2_PosOld.z) >
		(pSphere->GetRadius() + pSphere2->GetRadius()) * (pSphere->GetRadius() + pSphere2->GetRadius()))
	{// �O������������Ă��Ȃ������Ƃ�

		if ((Target1_Pos.x - Target2_Pos.x) * (Target1_Pos.x - Target2_Pos.x) +
			(Target1_Pos.y - Target2_Pos.y) * (Target1_Pos.y - Target2_Pos.y) +
			(Target1_Pos.z - Target2_Pos.z) * (Target1_Pos.z - Target2_Pos.z) <=
			(pSphere->GetRadius() + pSphere2->GetRadius()) * (pSphere->GetRadius() + pSphere2->GetRadius()))
		{// ��������������Ă����Ƃ�
			save = Target1_Pos - Target2_Pos;		//���������߂�(���������߂邽��)

			D3DXVec3Normalize(&vec, &save);			//���K������

			if (!pSphere->GetTrigger() && !pSphere2->GetTrigger())
			{// Trigger�ł͂Ȃ��Ƃ�
				if (pSphere->GetMoving())
				{
					// �H�����񂾕������߂�
					Target1_Pos = Target2_Pos + vec * ((pSphere->GetRadius() + pSphere2->GetRadius()));

					// �H�����񂾕������߂�
					D3DXVECTOR3 offset = pSphere->GetOffset();
					pSphere->m_pScene->SetPosition(Target1_Pos - offset);
					pSphere->m_pScene->SetPosOld(Target1_Pos - offset);
				}
			}

			return true;				// �t���O�𗧂Ă�
		}
	}

	return false;				// �t���O�̏�Ԃ�Ԃ�
}

//==============================================================================
// ���̂Ƌ��̂̓����蔻��(Stay)
//==============================================================================
bool CCollider::SphereInCollider(CCollider *pFirstTarget, CCollider *pSecondTarget)
{
	D3DXVECTOR3 save = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								// �ۑ��p
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								// ���K�������f�[�^

	CColliderSphere *pSphere = (CColliderSphere*)pFirstTarget;
	CColliderSphere *pSphere2 = (CColliderSphere*)pSecondTarget;														// �g�����ԂɃL���X�g

	D3DXVECTOR3 Target1_Pos = pSphere->GetPosition() + pSphere->GetOffset();							// Target1�̈ʒu
	D3DXVECTOR3 Target2_Pos = pSphere2->GetPosition() + pSphere2->GetOffset();							// Target2�̈ʒu
	D3DXVECTOR3 Target1_PosOld = pSphere->GetPosOld() + pSphere->GetOffset();							// Target1�̑O�̈ʒu
	D3DXVECTOR3 Target2_PosOld = pSphere2->GetPosOld() + pSphere2->GetOffset();							// Target2�̑O�̈ʒu

	if ((Target1_Pos.x - Target2_Pos.x) * (Target1_Pos.x - Target2_Pos.x) +
		(Target1_Pos.y - Target2_Pos.y) * (Target1_Pos.y - Target2_Pos.y) +
		(Target1_Pos.z - Target2_Pos.z) * (Target1_Pos.z - Target2_Pos.z) <=
		(pSphere->GetRadius() + pSphere2->GetRadius()) * (pSphere->GetRadius() + pSphere2->GetRadius()))
	{// ��������������Ă����Ƃ�
		save = Target1_Pos - Target2_Pos;		//���������߂�(���������߂邽��)

		D3DXVec3Normalize(&vec, &save);			//���K������

		if (pSphere->GetMoving())
		{
			// �H�����񂾕������߂�
			Target1_Pos = Target2_Pos + vec * ((pSphere->GetRadius() + pSphere2->GetRadius()));

			// �H�����񂾕������߂�
			D3DXVECTOR3 offset = pSphere->GetOffset();
			pSphere->m_pScene->SetPosition(Target1_Pos - offset);
			pSphere->m_pScene->SetPosOld(Target1_Pos - offset);
		}
		return true;				// �t���O�𗧂Ă�
	}

	return false;				// �t���O�̏�Ԃ�Ԃ�
}

//==============================================================================
// �����̂Ɨ����̂̓����蔻��(Hit)
//==============================================================================
bool CCollider::BoxHitCollider(CCollider * pFirstTarget, CCollider * pSecondTarget)
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 offset;
	CColliderBox *pBox = (CColliderBox*)pFirstTarget;
	CColliderBox *pBox2 = (CColliderBox*)pSecondTarget;														// �g�����ԂɃL���X�g

	D3DXVECTOR3 Target1_pos = pBox->m_pScene->GetPosition() + pBox->GetOffset();							// Target1�̈ʒu
	D3DXVECTOR3 Target2_pos = pBox2->m_pScene->GetPosition() + pBox2->GetOffset();							// Target2�̈ʒu
	D3DXVECTOR3 Target1_posOld = pBox->m_pScene->GetPosOld() + pBox->GetOffset();							// Target1�̑O�̈ʒu
	D3DXVECTOR3 Target2_posOld = pBox2->m_pScene->GetPosOld() + pBox2->GetOffset();							// Target2�̑O�̈ʒu
	D3DXVECTOR3 Target1_size = pBox->GetSize();
	D3DXVECTOR3 Target2_size = pBox2->GetSize();

	// ���E�͈͔���
	if ((Target1_posOld.x + Target1_size.x / 2 > Target2_posOld.x - Target2_size.x / 2) &&
		(Target1_posOld.x - Target1_size.x / 2 <= Target2_posOld.x + Target2_size.x / 2))
	{
		// �㉺�͈͔���
		if ((Target1_posOld.y + Target1_size.y / 2 > Target2_posOld.y - Target2_size.y / 2) &&
			(Target1_posOld.y - Target1_size.y / 2 <= Target2_posOld.y + Target2_size.y / 2))
		{
			// ���s������
			if ((Target1_pos.z + Target1_size.z / 2 > Target2_pos.z - Target2_size.z / 2) &&
				(Target1_posOld.z + Target1_size.z / 2 <= Target2_posOld.z - Target2_size.z / 2))
			{// ��O
				//�g���K�[�ł͖����Ƃ�
				if (!pBox->GetTrigger() && !pBox2->GetTrigger())
				{// Trigger�ł͂Ȃ��Ƃ�
					if (pBox->GetMoving())
					{
						//�v���C���[�̈ʒu��␳
						pos = Target1_pos;								// pFirstTarget �̌��݈ʒu���擾
						offset = pBox->GetOffset();
						pos.z = Target2_pos.z - Target2_size.z / 2 - Target1_size.z / 2;		// pSecondTarget �̂Ԃ������ʂ̍ő�l���܂� pFirstTarget ��߂�
						pBox->m_pScene->SetPosition(pos - offset);				// �����蔻��̈ʒu��ς���
					}
				}

				return true;											// �t���O�𗧂Ă�
			}
			else if ((Target1_pos.z - Target1_size.z / 2 < Target2_pos.z + Target2_size.z / 2) &&
				(Target1_posOld.z - Target1_size.z / 2 >= Target2_posOld.z + Target2_size.z / 2))
			{// ��
				//�g���K�[�ł͖����Ƃ�
				if (!pBox->GetTrigger() && !pBox2->GetTrigger())
				{// Trigger�ł͂Ȃ��Ƃ�
					if (pBox->GetMoving())
					{
						//�v���C���[�̈ʒu��␳
						pos = Target1_pos;								// pFirstTarget �̌��݈ʒu���擾
						offset = pBox->GetOffset();
						pos.z = Target2_pos.z + Target2_size.z / 2 + Target1_size.z / 2;		// pSecondTarget �̂Ԃ������ʂ̍ő�l���܂� pFirstTarget ��߂�
						pBox->m_pScene->SetPosition(pos - offset);				// �����蔻��̈ʒu��ς���
					}
				}
				return true;
			}
		}
	}

	//���s���͈͔���
	if ((Target1_posOld.z + Target1_size.z / 2 > Target2_posOld.z - Target2_size.z / 2) &&
		(Target1_posOld.z - Target1_size.z / 2 <= Target2_posOld.z + Target2_size.z / 2))
	{
		//�㉺�͈͔���
		if ((Target1_posOld.y + Target1_size.y / 2 > Target2_posOld.y - Target2_size.y / 2) &&
			(Target1_posOld.y - Target1_size.y / 2 <= Target2_posOld.y + Target2_size.y / 2))
		{
			//�v���C���[�̔���(���E����)
			if (Target1_pos.x + Target1_size.x / 2 > Target2_pos.x - Target2_size.x / 2 &&
				(Target1_posOld.x + Target1_size.x / 2 <= Target2_posOld.x - Target2_size.x / 2))
			{
				//�g���K�[�ł͖����Ƃ�
				if (!pBox->GetTrigger() && !pBox2->GetTrigger())
				{// Trigger�ł͂Ȃ��Ƃ�
					if (pBox->GetMoving())
					{
						//�v���C���[�̈ʒu��␳
						pos = Target1_pos;								// pFirstTarget �̌��݈ʒu���擾
						offset = pBox->GetOffset();
						pos.x = Target2_pos.x - Target2_size.x / 2 - Target1_size.x / 2;		// pSecondTarget �̂Ԃ������ʂ̍ő�l���܂� pFirstTarget ��߂�
						pBox->m_pScene->SetPosition(pos - offset);				// �����蔻��̈ʒu��ς���
					}
				}
				return true;
			}
			else if (Target1_pos.x - Target1_size.x / 2 < Target2_pos.x + Target2_size.x / 2 &&
				(Target1_posOld.x - Target1_size.x / 2 >= Target2_posOld.x + Target2_size.x / 2))
			{
				//�g���K�[�ł͖����Ƃ�
				if (!pBox->GetTrigger() && !pBox2->GetTrigger())
				{// Trigger�ł͂Ȃ��Ƃ�
					if (pBox->GetMoving())
					{
						//�v���C���[�̈ʒu��␳
						pos = Target1_pos;								// pFirstTarget �̌��݈ʒu���擾
						offset = pBox->GetOffset();
						pos.x = Target2_pos.x + Target2_size.x / 2 + Target1_size.x / 2;		// pSecondTarget �̂Ԃ������ʂ̍ő�l���܂� pFirstTarget ��߂�
						pBox->m_pScene->SetPosition(pos - offset);				// �����蔻��̈ʒu��ς���
					}
				}

				return true;
			}
		}
	}
	//���E�͈͔���
	if ((Target1_posOld.x + Target1_size.x / 2 > Target2_posOld.x - Target2_size.x / 2) &&
		(Target1_posOld.x - Target1_size.x / 2 <= Target2_posOld.x + Target2_size.x / 2))
	{
		//���s���͈͔���
		if ((Target1_posOld.z + Target1_size.z / 2 > Target2_posOld.z - Target2_size.z / 2) &&
			(Target1_posOld.z - Target1_size.z / 2 <= Target2_posOld.z + Target2_size.z / 2))
		{
			//�㉺����
			if ((Target1_pos.y + Target1_size.y / 2 > Target2_pos.y - Target2_size.y / 2) &&
				(Target1_posOld.y + Target1_size.y / 2 <= Target2_posOld.y - Target2_size.y / 2))
			{// ��
				//�g���K�[�ł͖����Ƃ�
				if (!pBox->GetTrigger() && !pBox2->GetTrigger())
				{// Trigger�ł͂Ȃ��Ƃ�
					if (pBox->GetMoving())
					{
						//�v���C���[�̈ʒu��␳
						pos = Target1_pos;
						offset = pBox->GetOffset();
						pos.y = Target2_pos.y - Target2_size.y / 2 - Target1_size.y / 2;
						pBox->m_pScene->SetPosition(pos - offset);				// �����蔻��̈ʒu��ς���
					}
				}

				return true;
			}
			else if ((Target1_pos.y - Target1_size.y / 2 < Target2_pos.y + Target2_size.y / 2) &&
				(Target1_posOld.y - Target1_size.y / 2 >= Target2_posOld.y + Target2_size.y / 2))
			{// ��
				//�g���K�[�ł͖����Ƃ�
				if (!pBox->GetTrigger() && !pBox2->GetTrigger())
				{// Trigger�ł͂Ȃ��Ƃ�
					if (pBox->GetMoving())
					{
						//�v���C���[�̈ʒu��␳
						pos = Target1_pos;
						offset = pBox->GetOffset();
						pos.y = Target2_pos.y + Target2_size.y / 2 + Target1_size.y / 2;
						pBox->m_pScene->SetPosition(pos - offset);				// �����蔻��̈ʒu��ς���
					}
				}

				return true;
			}
		}
	}

	//���ʂ�Ԃ�
	return false;
}

//==============================================================================
// �����̂Ɨ����̂̓����蔻��(In)
//==============================================================================
bool CCollider::BoxInCollider(CCollider *pFirstTarget, CCollider *pSecondTarget)
{
	CColliderBox *pBox = (CColliderBox*)pFirstTarget;
	CColliderBox *pBox2 = (CColliderBox*)pSecondTarget;											// �g�����ԂɃL���X�g

	D3DXVECTOR3 Target1_Pos = pBox->GetPosition() + pBox->GetOffset();							// Target1�̈ʒu
	D3DXVECTOR3 Target2_Pos = pBox2->GetPosition() + pBox2->GetOffset();						// Target2�̈ʒu

	D3DXVECTOR3 Target1_Size = pBox->GetSize();
	D3DXVECTOR3 Target2_Size = pBox2->GetSize();

	//���̓����蔻�� (pos1�̉���)
	if (Target1_Pos.z - Target1_Size.z / 2 < Target2_Pos.z + Target2_Size.z / 2)
	{
		if (Target1_Pos.z - Target1_Size.z / 2 >= Target2_Pos.z - Target2_Size.z / 2)
		{
			if (Target1_Pos.y - Target1_Size.y / 2 < Target2_Pos.y + Target2_Size.y / 2)
			{
				if (Target1_Pos.y - Target1_Size.y / 2 >= Target2_Pos.y - Target2_Size.y / 2)
				{
					//���̓����蔻�� (pos1�̉E��)
					if (Target1_Pos.x + Target1_Size.x / 2 < Target2_Pos.x + Target2_Size.x / 2)
					{
						if (Target1_Pos.x + Target1_Size.x / 2 >= Target2_Pos.x - Target2_Size.x / 2)
						{
							return true;
						}
					}
					//���̓����蔻�� (pos1�̍���)
					else if (Target1_Pos.x - Target1_Size.x / 2 < Target2_Pos.x + Target2_Size.x / 2)
					{
						if (Target1_Pos.x - Target1_Size.x / 2 >= Target2_Pos.x - Target2_Size.x / 2)
						{
							return true;
						}
					}
				}
			}
		}
	}
	//���̓����蔻�� (pos1�̏㑤)
	if (Target1_Pos.z - Target1_Size.z / 2 < Target2_Pos.z + Target2_Size.z / 2)
	{
		if (Target1_Pos.z + Target1_Size.z / 2 >= Target2_Pos.z - Target2_Size.z / 2)
		{
			if (Target1_Pos.y - Target1_Size.y / 2 < Target2_Pos.y - Target2_Size.y / 2)
			{
				if (Target1_Pos.y + Target1_Size.y / 2 >= Target2_Pos.y - Target2_Size.y / 2)
				{
					//���̓����蔻�� (pos1�̉E��)
					if (Target1_Pos.x + Target1_Size.x / 2 < Target2_Pos.x + Target2_Size.x / 2)
					{
						if (Target1_Pos.x + Target1_Size.x / 2 >= Target2_Pos.x - Target2_Size.x / 2)
						{
							return true;
						}
					}
					//���̓����蔻�� (pos1�̍���)
					else if (Target1_Pos.x - Target1_Size.x / 2 < Target2_Pos.x + Target2_Size.x / 2)
					{
						if (Target1_Pos.x - Target1_Size.x / 2 >= Target2_Pos.x - Target2_Size.x / 2)
						{
							return true;
						}
					}
				}
			}
		}
	}

	//���ʂ�Ԃ�
	return false;
}

//=============================================================================
// ���Ɨ����̂̏Փ˔���
//=============================================================================
bool CCollider::CollisionHitSphereAndBox(CCollider * pFirstTarget, CCollider * pSecondTarget)
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 offset;
	CColliderBox *pBox = (CColliderBox*)pFirstTarget;
	CColliderSphere *pSphere = (CColliderSphere*)pSecondTarget;														// �g�����ԂɃL���X�g

	if(pBox->m_pScene == NULL || pSphere->m_pScene == NULL) { return false; }

	D3DXVECTOR3 boxPos = pBox->m_pScene->GetPosition() + pBox->GetOffset();							// Target1�̈ʒu
	D3DXVECTOR3 spherePos = pSphere->m_pScene->GetPosition() + pSphere->GetOffset();						// Target2�̈ʒu

	D3DXVECTOR3 boxPosOld = pBox->m_pScene->GetPosOld() + pBox->GetOffset();							// Target1�̈ʒu
	D3DXVECTOR3 spherePosOld = pSphere->m_pScene->GetPosOld() + pSphere->GetOffset();						// Target2�̈ʒu

	D3DXVECTOR3 boxSize = pBox->GetSize();
	float fRadius = pSphere->GetRadius();

	// ���E�͈͔���
	if (((boxPosOld.x + boxSize.x / 2) > spherePosOld.x) &&
		((boxPosOld.x - boxSize.x / 2) < spherePosOld.x))
	{
		// �㉺�͈͔���
		if (((boxPosOld.y + boxSize.y / 2) > spherePosOld.y) &&
			((boxPosOld.y - boxSize.y / 2) < spherePosOld.y))
		{
			// ���s������
			if (((boxPos.z + boxSize.z / 2) + fRadius > spherePos.z) &&
				((boxPosOld.z + boxSize.z / 2) + fRadius <= spherePosOld.z))
			{// ��O
			 //�g���K�[�ł͖����Ƃ�
				if (!pBox->GetTrigger() && !pSphere->GetTrigger())
				{// Trigger�ł͂Ȃ��Ƃ�
					if (pBox->GetMoving())
					{
						//�v���C���[�̈ʒu��␳
						pos = spherePos;
						pos.z = boxPos.z + boxSize.z / 2 + fRadius;
						pSphere->m_pScene->SetPosition(pos - pSphere->GetOffset());
					}
				}

				return true;											// �t���O�𗧂Ă�
			}
			else if (((boxPos.z - boxSize.z / 2) - fRadius < spherePos.z) &&
				((boxPosOld.z - boxSize.z / 2) - fRadius >= spherePosOld.z))
			{// ��
			 //�g���K�[�ł͖����Ƃ�
				if (!pBox->GetTrigger() && !pSphere->GetTrigger())
				{// Trigger�ł͂Ȃ��Ƃ�
					if (pBox->GetMoving())
					{
						//�v���C���[�̈ʒu��␳
						pos = spherePos;
						pos.z = boxPos.z - boxSize.z / 2 - fRadius;
						pSphere->m_pScene->SetPosition(pos - pSphere->GetOffset());
					}
				}
				return true;
			}
		}
	}

	//���s���͈͔���
	if (((boxPosOld.z + boxSize.z / 2) > spherePosOld.z) &&
		((boxPosOld.z - boxSize.z / 2) < spherePosOld.z))
	{
		//�㉺�͈͔���
		if (((boxPosOld.y + boxSize.y / 2) > spherePosOld.y) &&
			((boxPosOld.y - boxSize.y / 2) < spherePosOld.y))
		{
			//�v���C���[�̔���(���E����)
			if (((boxPos.x + boxSize.x / 2) + fRadius > spherePos.x) &&
				((boxPosOld.x + boxSize.x / 2) + fRadius <= spherePosOld.x))
			{
				//�g���K�[�ł͖����Ƃ�
				if (!pBox->GetTrigger() && !pSphere->GetTrigger())
				{// Trigger�ł͂Ȃ��Ƃ�
					if (pBox->GetMoving())
					{
						//�v���C���[�̈ʒu��␳
						pos = spherePos;
						pos.x = boxPos.x + boxSize.x / 2 + fRadius;
						pSphere->m_pScene->SetPosition(pos - pSphere->GetOffset());
					}
				}
				return true;
			}
			else if (((boxPos.x - boxSize.x / 2) - fRadius < spherePos.x) &&
				((boxPosOld.x - boxSize.x / 2) - fRadius >= spherePosOld.x))
			{
				//�g���K�[�ł͖����Ƃ�
				if (!pBox->GetTrigger() && !pSphere->GetTrigger())
				{// Trigger�ł͂Ȃ��Ƃ�
					if (pBox->GetMoving())
					{
						//�v���C���[�̈ʒu��␳
						pos = spherePos;
						pos.x = boxPos.x - boxSize.x / 2 - fRadius;
						pSphere->m_pScene->SetPosition(pos - pSphere->GetOffset());
					}
				}

				return true;
			}
		}
	}
	//���E�͈͔���
	if (((boxPosOld.x + boxSize.x / 2) > spherePosOld.x) &&
		((boxPosOld.x - boxSize.x / 2) < spherePosOld.x))
	{
		//���s���͈͔���
		if (((boxPosOld.z + boxSize.z / 2) > spherePosOld.z) &&
			((boxPosOld.z - boxSize.z / 2) < spherePosOld.z))
		{
			//�㉺����
			if (((boxPos.y + boxSize.y / 2) + fRadius > spherePos.y) &&
				((boxPosOld.y + boxSize.y / 2) + fRadius <= spherePosOld.y))
			{// ��
			 //�g���K�[�ł͖����Ƃ�
				if (!pBox->GetTrigger() && !pSphere->GetTrigger())
				{// Trigger�ł͂Ȃ��Ƃ�
					if (pBox->GetMoving())
					{
						//�v���C���[�̈ʒu��␳
						pos = spherePos;
						pos.y = boxPos.y + boxSize.y / 2 + fRadius;
						pSphere->m_pScene->SetPosition(pos - pSphere->GetOffset());
					}
				}

				return true;
			}
			else if (((boxPos.y - boxSize.y / 2) - fRadius < spherePos.y) &&
				((boxPosOld.y - boxSize.y / 2) - fRadius >= spherePosOld.y))
			{// ��
			 //�g���K�[�ł͖����Ƃ�
				if (!pBox->GetTrigger() && !pSphere->GetTrigger())
				{// Trigger�ł͂Ȃ��Ƃ�
					if (pBox->GetMoving())
					{
						//�v���C���[�̈ʒu��␳
						pos = spherePos;
						pos.y = boxPos.y - boxSize.y / 2 - fRadius;
						pSphere->m_pScene->SetPosition(pos - pSphere->GetOffset());
					}
				}

				return true;
			}
		}
	}

	//���ʂ�Ԃ�
	return false;
}

//=============================================================================
// ���Ɨ����͈͓̂̔�����
//=============================================================================
bool CCollider::CollisionInSphereAndBox(CCollider *pFirstTarget, CCollider *pSecondTarget)
{
	CColliderBox *pBox = (CColliderBox*)pFirstTarget;
	CColliderSphere *pSphere = (CColliderSphere*)pSecondTarget;														// �g�����ԂɃL���X�g

	D3DXVECTOR3 boxPos = pBox->GetPosition() + pBox->GetOffset();							// Target1�̈ʒu
	D3DXVECTOR3 spherePos = pSphere->GetPosition() + pSphere->GetOffset();						// Target2�̈ʒu

	D3DXVECTOR3 boxSize = pBox->GetSize();
	float fRadius = pSphere->GetRadius();

	// ���E�̔��a���̓����蔻��
	if ((boxPos.x - boxSize.x / 2) - fRadius < spherePos.x)
	{// ���Ɠ_
		if ((boxPos.x + boxSize.x / 2) + fRadius > spherePos.x)
		{// �E�Ɠ_
			if ((boxPos.y - boxSize.y / 2) - fRadius < spherePos.y)
			{// ��Ɠ_
				if ((boxPos.y + boxSize.y / 2) + fRadius > spherePos.y)
				{// ���Ɠ_
					if (boxPos.z - boxSize.z / 2 - fRadius < spherePos.z)
					{// ���Ɠ_
						if (boxPos.z + boxSize.z / 2 + fRadius > spherePos.z)
						{// ��O�Ɠ_
							D3DXVECTOR3 pos = spherePos;
							pos.z = boxPos.z + boxSize.z / 2 + fRadius;
							pSphere->m_pScene->SetPosition(pos - pSphere->GetOffset());

							return true;														// �����蔻�� �̃t���O�𗧂Ă�
						}
					}
				}
			}
		}
	}
	// ���s�����a���̓����蔻��
	if (boxPos.z - boxSize.z / 2 - fRadius < spherePos.z)
	{// ���Ɠ_
		if (boxPos.z + boxSize.z / 2 + fRadius > spherePos.z)
		{// �E�Ɠ_
			if ((boxPos.x - boxSize.x / 2) - fRadius < spherePos.x)
			{// ���Ɠ_
				if ((boxPos.x + boxSize.x / 2) + fRadius > spherePos.x)
				{// ��O�Ɠ_
					if ((boxPos.y - boxSize.y / 2) - fRadius < spherePos.y)
					{// ���Ɠ_
						if ((boxPos.y + boxSize.y / 2) + fRadius > spherePos.y)
						{// ��O�Ɠ_
							return true;													// �����蔻�� �̃t���O�𗧂Ă�
						}
					}
				}
			}
		}
	}
	// �p�̉~�Ɠ_�ɂ�铖���蔻��
	if (CollisionHitSphereAndPoint(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pSecondTarget))
	{// �����蔻�� �̃t���O���������Ƃ�
		return true;
	}

	return false;																		// �����蔻�� �̃t���O��Ԃ�Ԃ�
}

//=============================================================================
// ���Ɠ_�̓����蔻��
//=============================================================================
bool CCollider::CollisionHitSphereAndPoint(D3DXVECTOR3 pos, CCollider *pSecondTarget)
{
	CColliderSphere *pSphere = (CColliderSphere*)pSecondTarget;					// �g�����ԂɃL���X�g

	D3DXVECTOR3 save = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// �ۑ��p
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// ���K�������f�[�^

	D3DXVECTOR3 spherePos = pSphere->m_pScene->GetPosition() + pSphere->GetOffset();						// Target2�̈ʒu
	D3DXVECTOR3 spherePosOld = pSphere->m_pScene->GetPosOld() + pSphere->GetOffset();						// Target2�̈ʒu
	float fRadius = pSphere->GetRadius();

	D3DXVECTOR3 point = pos - pSphere->GetOffset();	// �ʒu

	if ((spherePosOld.x - pos.x) * (spherePosOld.x - pos.x) +
		(spherePosOld.z - pos.z) * (spherePosOld.z - pos.z) >
		pSphere->GetRadius() * pSphere->GetRadius())
	{// �O������������Ă��Ȃ������Ƃ�
		if ((spherePos.x - point.x) * (spherePos.x - point.x) +
			(spherePos.z - point.z) * (spherePos.z - point.z) <=
			fRadius * fRadius)
		{// ��������������Ă����Ƃ�
			save = spherePos - point;		//���������߂�(���������߂邽��)

			D3DXVec3Normalize(&vec, &save);			//���K������

			if (!pSphere->GetTrigger())
			{// Trigger�ł͂Ȃ��Ƃ�
				if (pSphere->GetMoving())
				{
					D3DXVECTOR3 offset = pSphere->GetOffset();

					// �H�����񂾕������߂�
					spherePos = point + vec * pSphere->GetRadius();
					spherePos -= offset;
					spherePos.y = 0.0f;
					// �H�����񂾕������߂�
					pSphere->m_pScene->SetPosition(spherePos);
					pSphere->m_pScene->SetPosOld(spherePos);
				}
			}
			return true;				// �t���O�𗧂Ă�
		}
	}

	return false;
}

//==============================================================================
// �폜����
//==============================================================================
void CCollider::Delete(void)
{
	if (this != NULL)
	{
		//�I�������y�у��������
		this->Uninit();

		if (this->m_pPrev[m_Obj] == NULL)
		{//�������擪�������Ƃ�

			if (this->m_pNext[m_Obj] != NULL)
			{//�����̎��̃I�u�W�F�N�g������Ƃ�
				m_apTop[m_Obj] = this->m_pNext[m_Obj];																//�擪�����̃I�u�W�F�N�g�ɏ��n����

				if (m_apTop[m_Obj]->m_pPrev[m_Obj] != NULL)
				{//�O�̐l�̏�񂪂���ꍇ
					m_apTop[m_Obj]->m_pPrev[m_Obj] = NULL;															//�O�ɂ���I�u�W�F�N�g���� NULL �ɂ���
				}
			}
			else
			{//�����̎��̃I�u�W�F�N�g���Ȃ������Ƃ�
				m_apTop[m_Obj] = NULL;
				m_apCur[m_Obj] = NULL;
			}
		}
		else if (this->m_pNext[m_Obj] == NULL)
		{//�������Ō���������Ƃ�
			m_apCur[m_Obj] = m_pPrev[m_Obj];													//�Ō����O�̃I�u�W�F�N�g�ɏ��n����

			if (m_apCur[m_Obj]->m_pNext[m_Obj] != NULL)
			{
				m_apCur[m_Obj]->m_pNext[m_Obj] = NULL;											//���ɂ���I�u�W�F�N�g���� NULL �ɂ���
			}
		}
		else
		{//�ǂ���ł��Ȃ��ꍇ
			m_pNext[m_Obj]->m_pPrev[m_Obj] = m_pPrev[m_Obj];									//���̃I�u�W�F�N�g�̑O�̃I�u�W�F�N�g���Ɏ����̑O�̃I�u�W�F�N�g�����i�[����
			m_pPrev[m_Obj]->m_pNext[m_Obj] = m_pNext[m_Obj];									//�O�̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g���Ɏ����̎��̃I�u�W�F�N�g�����i�[����
		}

		this->m_pScene = NULL;
		delete this;
		m_nNumAll--;																			//������������炷
	}
}



//======================================================================================================================
// ���C�̔���
//======================================================================================================================
bool CCollider::RayBlockCollision(D3DXVECTOR3 &pos, D3DXMATRIX *pMat, float fOffset, float fLength, VERTEX_PLANE &plane)
{
	// �n�`���� �ϐ��錾
	BOOL				bHitFlag = false;		// ���肪�o�����̃t���O
	bool				bLand = false;			// ���肪�o�����̃t���O
	float				fLandDistance = 0;		// ����
	DWORD				dwHitIndex = -1;		// �C���f�b�N�X
	float				fHitU = 0;		// U
	float				fHitV = 0;		// V
	D3DXMATRIX			invmat;							// �t�s����i�[����ϐ�
	D3DXVECTOR3			m_posAfter;						// �t�s��ŏo�����I�_�����i�[����
	D3DXVECTOR3			m_posBefore;					// �I�_�����i�[����
	D3DXVECTOR3			direction;						// �ϊ���̈ʒu�A�������i�[����ϐ�
	std::vector<VERTEX_PLANE>	vPlane;
	std::vector<float>	vDistance;						// �����̔z��ۑ�
	float				fData = 0.0f;		// �f�[�^
	float				fDistanceMin = 100000.0f;		// �ł��߂���������

	CScene *pSceneNext = NULL;														//����A�b�v�f�[�g�Ώ�
	CScene *pSceneNow = NULL;

	// model�̃I�u�W�F�N�g�̃|�W�V�������Q��
	pSceneNow = CScene::GetScene(CScene::PRIORITY_MODEL);

	//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, (CScene::PRIORITY_MODEL));							//����A�b�v�f�[�g�Ώۂ��T����
		CObject *pObj = (CObject*)pSceneNow;

		//	�t�s��̎擾
		D3DXMatrixInverse(&invmat, NULL, &pObj->GetMtxWorld());
		//	�t�s����g�p���A���C�n�_����ϊ��@�ʒu�ƌ����ŕϊ�����֐����قȂ�̂ŗv����
		D3DXVec3TransformCoord(&m_posBefore, &D3DXVECTOR3(pos.x, pMat->_42+ fLength, pos.z), &invmat);
		//	���C�I�_����ϊ�
		D3DXVec3TransformCoord(&m_posAfter, &D3DXVECTOR3(pos.x, pos.y - 1, pos.z), &invmat);
		//	���C��������ϊ�
		D3DXVec3Normalize(&direction, &(m_posAfter - m_posBefore));

		//Ray���΂�
		D3DXIntersect(pObj->GetMesh(), &m_posBefore, &direction, &bHitFlag, &dwHitIndex, &fHitU, &fHitV, &fLandDistance, NULL, NULL);
		if (bHitFlag == TRUE)
		{
			//�����̕ۑ��ǉ�
			vDistance.emplace_back(fLandDistance);

			CObject *pObj = (CObject*)pSceneNow;

			pos.y = pos.y - fData - fOffset + fLength;
			DWORD num = pObj->GetMesh()->GetNumVertices();

			LPD3DXMESH pMesh = pObj->GetMesh();

			VERTEX_3D *pVtx;										//���_���ւ̃|�C���^
			WORD *pIdx;									//�C���f�b�N�X�f�[�^�փ|�C���^

			pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIdx);
			pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtx);

			D3DXVECTOR3 p0, p1, p2;
			int IndexBuff = pIdx[dwHitIndex];
			IndexBuff *= 3;

			p0 = pVtx[IndexBuff].pos;
			p1 = pVtx[IndexBuff + 1].pos;
			p2 = pVtx[IndexBuff + 2].pos;

			D3DXVECTOR3 AB = p0 - p1;
			D3DXVECTOR3 BC = p1 - p2;

			D3DXVECTOR3 norwork;

			D3DXVec3Cross(&norwork, &BC, &AB);
			D3DXVec3Normalize(&norwork, &norwork);

			CDebugProc::Log("�@�� : %.2f, %.2f, %.2f\n", norwork.x, norwork.y, norwork.z);

			pMesh->UnlockVertexBuffer();
			pMesh->UnlockIndexBuffer();

			VERTEX_PLANE planeWork;
			planeWork.a = p0;
			planeWork.b = p1;
			planeWork.c = p2;
			vPlane.emplace_back(planeWork);
		}

		pSceneNow = pSceneNext;													//����A�b�v�f�[�g�Ώۂ��i�[
	}

	//Ray�̃q�b�g���������������Ƃ�
	if (!vDistance.empty())
	{
		//�ŏ��̔�r�Ώ�
		fData = vDistance[0];
		VERTEX_PLANE planeAns = vPlane[0];
		for (unsigned int nCnt = 0; vDistance.size() > nCnt; nCnt++)
		{
			if (vDistance[nCnt] < fData)
			{
				//��r�Ώۂ���������������
				planeAns = vPlane[nCnt];
				fData = vDistance[nCnt];
			}
		}
		if (fData < 30000)//Ray�̒����̎w�����
		{
			pos.y = pos.y - fData - fOffset+ fLength;
			plane = planeAns;
		}
	}
	//Ray�ɔ��肪�Ȃ�������W�����v�ł��Ȃ�
	else
	{
		bLand = false;
	}


	//�z�����ɂ��Ă���
	vDistance.clear();

	// ����t���O��Ԃ�
	return bLand;
}

D3DXVECTOR3 CCollider::RayLeftWallCollision(float &fLeftLength, D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, D3DXVECTOR3 &move, D3DXMATRIX * pMat)
{
	// �n�`���� �ϐ��錾
	BOOL				bHitFlag = false;		// ���肪�o�����̃t���O
	float				fLandDistance = 0;		// ����
	DWORD				dwHitIndex = -1;		// �C���f�b�N�X
	float				fHitU = 0;		// U
	float				fHitV = 0;		// V
	D3DXMATRIX			invmat;							// �t�s����i�[����ϐ�
	D3DXVECTOR3			m_posAfter;						// �t�s��ŏo�����I�_�����i�[����
	D3DXVECTOR3			m_posBefore;					// �I�_�����i�[����
	D3DXVECTOR3			direction;						// �ϊ���̈ʒu�A�������i�[����ϐ�
	D3DXVECTOR3			endPoint;						// �I���n�_
	std::vector<float>	vDistance;						// �����̔z��ۑ�
	float				fData = 0.0f;		// �f�[�^

	float				fLeft;		// �ǂ̌��E�l

	CScene *pSceneNext = NULL;														//����A�b�v�f�[�g�Ώ�
	CScene *pSceneNow = NULL;

	fLeft = 1000.0f;

	endPoint.x = sinf(D3DX_PI * 0.5f + rot.y);
	endPoint.z = cosf(D3DX_PI * 0.5f + rot.y);

	// model�̃I�u�W�F�N�g�̃|�W�V�������Q��
	pSceneNow = CScene::GetScene(CScene::PRIORITY_MODEL);

	//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, (CScene::PRIORITY_MODEL));							//����A�b�v�f�[�g�Ώۂ��T����
		CObject *pObj = (CObject*)pSceneNow;

		//	�t�s��̎擾
		D3DXMatrixInverse(&invmat, NULL, &pObj->GetMtxWorld());
		//	�t�s����g�p���A���C�n�_����ϊ��@�ʒu�ƌ����ŕϊ�����֐����قȂ�̂ŗv����
		D3DXVec3TransformCoord(&m_posBefore, &D3DXVECTOR3(pos.x, pos.y - 100.0f, pos.z), &invmat);
		//	���C�I�_����ϊ�
		D3DXVec3TransformCoord(&m_posAfter, &D3DXVECTOR3(pos.x + endPoint.x, pos.y - 100.0f, pos.z + endPoint.z), &invmat);
		//	���C��������ϊ�
		D3DXVec3Normalize(&direction, &(m_posAfter - m_posBefore));
		//Ray���΂�
		D3DXIntersect(pObj->GetMesh(), &m_posBefore, &direction, &bHitFlag, &dwHitIndex, &fHitU, &fHitV, &fLandDistance, NULL, NULL);
		if (bHitFlag == TRUE)
		{
			//�����̕ۑ��ǉ�
			vDistance.emplace_back(fLandDistance);
		}

		pSceneNow = pSceneNext;													//����A�b�v�f�[�g�Ώۂ��i�[
	}

	//Ray�̃q�b�g���������������Ƃ�
	if (!vDistance.empty())
	{
		//�ŏ��̔�r�Ώ�
		fData = vDistance[0];
		for (unsigned int nCnt = 0; vDistance.size() > nCnt; nCnt++)
		{
			if (vDistance[nCnt] < fData)
			{
				//��r�Ώۂ���������������
				fData = vDistance[nCnt];
			}
		}
		if (fData < 1500.0f)//Ray�̒����̎w�����
		{
			fLeft = fData;
		}
	}

	//�z�����ɂ��Ă���
	vDistance.clear();

	D3DXVECTOR3 testPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	fLeftLength = fLeft;
	testPos = pos + endPoint * (fLeft - 20.0f);
	testPos.y = pos.y;

#ifdef _DEBUG
	CDebugProc::Log("�ǂ̌��E�l : Left = %.2f %.2f %.2f\n", testPos.x, testPos.y, testPos.z);
#endif

	// ����t���O��Ԃ�
	return testPos;
}

D3DXVECTOR3 CCollider::RayRightWallCollision(float &fRightLength, D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, D3DXVECTOR3 &move, D3DXMATRIX * pMat)
{
	// �n�`���� �ϐ��錾
	BOOL				bHitFlag = false;		// ���肪�o�����̃t���O
	float				fLandDistance = 0;		// ����
	DWORD				dwHitIndex = -1;		// �C���f�b�N�X
	float				fHitU = 0;		// U
	float				fHitV = 0;		// V
	D3DXMATRIX			invmat;							// �t�s����i�[����ϐ�
	D3DXVECTOR3			m_posAfter;						// �t�s��ŏo�����I�_�����i�[����
	D3DXVECTOR3			m_posBefore;					// �I�_�����i�[����
	D3DXVECTOR3			direction;						// �ϊ���̈ʒu�A�������i�[����ϐ�
	D3DXVECTOR3			endPoint;						// �I���n�_

	std::vector<float>	vDistance;						// �����̔z��ۑ�
	float				fData = 0.0f;		// �f�[�^

	float				fRight = 1000.0f;		// �ǂ̌��E�l

	CScene *pSceneNext = NULL;														//����A�b�v�f�[�g�Ώ�
	CScene *pSceneNow = NULL;

	endPoint.x = sinf(-D3DX_PI * 0.5f + rot.y);
	endPoint.z = cosf(-D3DX_PI * 0.5f + rot.y);

	// model�̃I�u�W�F�N�g�̃|�W�V�������Q��
	pSceneNow = CScene::GetScene(CScene::PRIORITY_MODEL);

	//�����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, (CScene::PRIORITY_MODEL));							//����A�b�v�f�[�g�Ώۂ��T����
		CObject *pObj = (CObject*)pSceneNow;

		//	�t�s��̎擾
		D3DXMatrixInverse(&invmat, NULL, &pObj->GetMtxWorld());
		//	�t�s����g�p���A���C�n�_����ϊ��@�ʒu�ƌ����ŕϊ�����֐����قȂ�̂ŗv����
		D3DXVec3TransformCoord(&m_posBefore, &D3DXVECTOR3(pos.x, pos.y - 100.0f, pos.z), &invmat);
		//	���C�I�_����ϊ�
		D3DXVec3TransformCoord(&m_posAfter, &D3DXVECTOR3(pos.x + endPoint.x, pos.y - 100.0f, pos.z + endPoint.z), &invmat);
		//	���C��������ϊ�
		D3DXVec3Normalize(&direction, &(m_posAfter - m_posBefore));
		//Ray���΂�
		D3DXIntersect(pObj->GetMesh(), &m_posBefore, &direction, &bHitFlag, &dwHitIndex, &fHitU, &fHitV, &fLandDistance, NULL, NULL);
		if (bHitFlag == TRUE)
		{
			//�����̕ۑ��ǉ�
			vDistance.emplace_back(fLandDistance);
		}

		pSceneNow = pSceneNext;													//����A�b�v�f�[�g�Ώۂ��i�[
	}

	//Ray�̃q�b�g���������������Ƃ�
	if (!vDistance.empty())
	{
		//�ŏ��̔�r�Ώ�
		fData = vDistance[0];
		for (unsigned int nCnt = 0; vDistance.size() > nCnt; nCnt++)
		{
			if (vDistance[nCnt] < fData)
			{
				//��r�Ώۂ���������������
				fData = vDistance[nCnt];
			}
		}
		if (fData < 1500.0f)//Ray�̒����̎w�����
		{
			fRight = fData;
		}
	}

	//�z�����ɂ��Ă���
	vDistance.clear();

	D3DXVECTOR3 testPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	fRightLength = fRight;
	testPos = pos + endPoint * (fRight - 20.0f);
	testPos.y = pos.y;

#ifdef _DEBUG
	CDebugProc::Log("�ǂ̌��E�l : Right = %.2f %.2f %.2f\n", testPos.x, testPos.y, testPos.z);
#endif

	// ����t���O��Ԃ�
	return testPos;
}

bool CCollider::IsInside(D3DXVECTOR3 * pvI, D3DXVECTOR3 * pvA, D3DXVECTOR3 * pvB, D3DXVECTOR3 * pvC)
{
	//�Ӄx�N�g��
	D3DXVECTOR3 vAB, vBC, vCA;
	vAB = *pvB - *pvA;
	vBC = *pvC - *pvB;
	vCA = *pvA - *pvC;
	//�Ӄx�N�g���Ɓu���_�����_�֌������x�N�g���v�Ƃ́A���ꂼ��̊O�ϗp
	D3DXVECTOR3 vCrossAB, vCrossBC, vCrossCA;
	//�u�O�ό��ʂ̃x�N�g���v�ƕ��ʖ@���x�N�g���Ƃ́A���ꂼ��̓��ϗp
	FLOAT fAB, fBC, fCA;
	// �@���p
	D3DXVECTOR3 vNormal;
	// �܂��A3���_���畽�ʕ����������߂�B����́A�����ɕ��ʂ̖@�������߂邱�Ƃł�����
	D3DXPLANE pln;
	D3DXPlaneFromPoints(&pln, pvA, pvB, pvC);
	vNormal.x = pln.a;//�@����x�����͕��ʕ�������x�W��
	vNormal.y = pln.b;//�@����y�����͕��ʕ�������y�W��
	vNormal.z = pln.c;//�@����z�����͕��ʕ�������z�W��
	D3DXVec3Normalize(&vNormal, &vNormal);

	// �e���_�����_I�Ɍ������x�N�g����vV�Ƃ���
	D3DXVECTOR3 vV;
	// ��AB�x�N�g���i���_B�x�N�g��-���_A�x�N�g��)�ƁA���_A�����_I�֌������x�N�g���A�̊O�ς����߂�
	vV = *pvI - *pvA;
	D3DXVec3Cross(&vCrossAB, &vAB, &vV);
	// ��BC�x�N�g���i���_C�x�N�g��-���_B�x�N�g��)�ƁA���_B�����_I�֌������x�N�g���A�̊O�ς����߂�
	vV = *pvI - *pvB;
	D3DXVec3Cross(&vCrossBC, &vBC, &vV);
	// ��CA�x�N�g���i���_A�x�N�g��-���_C�x�N�g��)�ƁA���_C�����_I�֌������x�N�g���A�̊O�ς����߂�
	vV = *pvI - *pvC;
	D3DXVec3Cross(&vCrossCA, &vCA, &vV);
	// ���ꂼ��́A�O�σx�N�g���Ƃ̓��ς��v�Z����
	fAB = D3DXVec3Dot(&vNormal, &vCrossAB);
	fBC = D3DXVec3Dot(&vNormal, &vCrossBC);
	fCA = D3DXVec3Dot(&vNormal, &vCrossCA);

	// �R�̓��ό��ʂ̂����A��ł��}�C�i�X�����̂��̂�����΁A��_�͊O�ɂ���B
	if (fAB >= 0 && fBC >= 0 && fCA >= 0)
	{
		// ��_�́A�ʂ̓��ɂ���
		return true;
	}
	// ��_�͖ʂ̊O�ɂ���
	return false;
}
