//=============================================================================
//
// �Ǐ��� [wall.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "wall.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "circle.h"
#include "tree.h"
#include "collider.h"
#include "write.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define FLOOR_INCREASE (5.0f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshWall::CMeshWall(CScene::PRIORITY obj = CScene::PRIORITY_WALL) : CScene(obj)
{
	SetObjType(CScene::PRIORITY_WALL);

	m_pVtxBuff = NULL;

	// �ʒu�E��]�̏����ݒ�
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bDebug = false;
	m_bCalculation = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshWall::~CMeshWall()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshWall::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾

	//�����_��
	m_nNumVertex = (WALL_DEPTH_FIELD + 1) * (WALL_WIDE_FIELD + 1);

	//�C���f�b�N�X��
	m_nNumIndex = ((WALL_DEPTH_FIELD + 1) * 2) * WALL_DEPTH_FIELD + ((WALL_WIDE_FIELD - 1) * 2);

	//���|���S����
	m_nNumPolygon = WALL_DEPTH_FIELD * WALL_WIDE_FIELD * 2 + 4 * (WALL_DEPTH_FIELD - 1);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	//�C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);

	int nPolygon = WALL_DEPTH_FIELD * WALL_WIDE_FIELD * 2;

	// ���_���̍쐬
	MakeVertex(NULL, false);

	// ���_�C���f�b�N�X�̍쐬
	MakeIndex();

	// �@���̌v�Z
	CalculationNormalize();

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CMeshWall::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CMeshWall::Update(void)
{
#ifdef _DEBUG
	// �f�o�b�O����
	Debug();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMeshWall::Draw(void)
{
#ifdef _DEBUG
	D3DXVECTOR3 pos = GetPosition();
	CRenderer *pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX	mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, NULL);

	// �|���S���̕`��
	//pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, NULL);
#endif
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CMeshWall *CMeshWall::Create(void)
{
	CMeshWall *pMeshWall;
	pMeshWall = new CMeshWall(CScene::PRIORITY_WALL);

	if (pMeshWall != NULL)
	{// ���b�V���t�B�[���h�����݂��Ă����Ƃ�
		pMeshWall->Init();			// ����������
	}
	return pMeshWall;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CMeshWall::Load(void)
{
	return S_OK;
}

//=============================================================================
// �n�`����Ǎ��֐�
//=============================================================================
void CMeshWall::LoadWall(char *add, bool bDebug)
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
					if (strcmp(cHeadText, "FIELDSET") == 0)
					{//�L�����N�^�[�̏������̂Ƃ�
						CMeshWall *pWall;
						pWall = CMeshWall::Create();										// ���̍쐬

						if (pWall != NULL)
						{
							//�G���h�L�����N�^�[�Z�b�g������܂Ń��[�v
							while (strcmp(cHeadText, "END_FIELDSET") != 0)
							{
								fgets(cReadText, sizeof(cReadText), pFile);
								sscanf(cReadText, "%s", &cHeadText);

								if (strcmp(cHeadText, "POS") == 0)
								{//�p�[�c�����̂Ƃ�
									D3DXVECTOR3 pos;
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&pos.x,
										&pos.y,
										&pos.z);

									// �ʒu�̐ݒ�
									pWall->SetPosition(pos);
								}
								else if (strcmp(cHeadText, "CHECK_HEIGHT") == 0)
								{// �v�Z����K�v�̂��郁�b�V�����ǂ���
									pWall->SetCalculation(true);
								}
								else if (strcmp(cHeadText, "VERTEXINFO") == 0)
								{//�p�[�c�����̂Ƃ�
									pWall->MakeVertex(pFile, bDebug);			// ���_���̍쐬(�t�@�C������)
									pWall->CalculationNormalize();				// �@���̌v�Z
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
// �|���S���̏��̍��������߂�
//=============================================================================
bool CMeshWall::GetWallHit(CScene *pTarget, D3DXVECTOR3 &nol)
{
	VERTEX_3D *pVtx;										//���_���ւ̃|�C���^
	bool bHit = false;			// �q�b�g�t���O

	D3DXVECTOR3 FieldPos = GetPosition();

	D3DXVECTOR3 AB;
	D3DXVECTOR3 BC;

	D3DXVECTOR3 point1;
	D3DXVECTOR3 point2;
	D3DXVECTOR3 point3;

	D3DXVECTOR3 aWork[3];
	D3DXVECTOR3 aPlayer[3];
	float fHeight = 0;

	D3DXVECTOR3 pointA, pointB;

	pointA = pTarget->GetPosition();
	pointB = pTarget->GetPosOld();

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	////�@�����߂�
	//for (int nDepth = 0; nDepth < WALL_DEPTH_FIELD + 1; nDepth++)
	//{
	//	for (int nWide = 0; nWide < WALL_WIDE_FIELD + 1; nWide++)
	//	{
	//		// �@�����ƕ��ʂ̏Փ�
	//		D3DXVECTOR3 v1, v2;
	//		v1 = pTarget->GetPosition() - pVtx[WALL_WIDE_FIELD + nWide + 1 + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;
	//		v2 = pTarget->GetPosOld() - pVtx[WALL_WIDE_FIELD + nWide + 1 + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;

	//		if (D3DXVec3Dot(&v1, &pVtx[(WALL_WIDE_FIELD + 1) * nDepth + nWide].nor) * D3DXVec3Dot(&v2, &pVtx[(WALL_WIDE_FIELD + 1) * nDepth + nWide].nor) <= 0)
	//		{
	//			// �A����_���畽�ʂ܂ł̋���
	//			float d1 = 0.0f, d2 = 0.0f;
	//			d1 = CManager::DistanceForDotAndPlane(pointA, pVtx[WALL_WIDE_FIELD + nWide + 1 + ((WALL_WIDE_FIELD + 1) * nDepth)].pos, pVtx[(WALL_WIDE_FIELD + 1) * nDepth + nWide].nor);
	//			d2 = CManager::DistanceForDotAndPlane(pointB, pVtx[WALL_WIDE_FIELD + nWide + 1 + ((WALL_WIDE_FIELD + 1) * nDepth)].pos, pVtx[(WALL_WIDE_FIELD + 1) * nDepth + nWide].nor);

	//			float a = 0.0f;
	//			if (d1 != 0 && d2 != 0)
	//			{
	//				a = fabs(d1) / (fabs(d1) + fabs(d2));
	//			}

	//			D3DXVECTOR3 v3 = D3DXVECTOR3_ZERO;

	//			D3DXVECTOR3 AB = pTarget->GetPosOld() - pTarget->GetPosition();
	//			v3 = pTarget->GetPosition() + (AB * a);
	//			//v3 = (1 - a) * v1 + a * v2;

	//			pointA = v3;

	//			//�����̌v�Z
	//			aWork[0] = (pVtx[WALL_WIDE_FIELD + nWide + 1 + ((WALL_WIDE_FIELD + 1) * nDepth)].pos + FieldPos) - (pVtx[nWide + ((WALL_WIDE_FIELD + 1) * nDepth)].pos + FieldPos);
	//			aPlayer[0] = v3 - (pVtx[WALL_WIDE_FIELD + nWide + 1 + ((WALL_WIDE_FIELD + 1) * nDepth)].pos + FieldPos);

	//			aWork[1] = (pVtx[WALL_WIDE_FIELD + nWide + 2 + ((WALL_WIDE_FIELD + 1) * nDepth)].pos + FieldPos) - (pVtx[WALL_WIDE_FIELD + nWide + 1 + ((WALL_WIDE_FIELD + 1) * nDepth)].pos + FieldPos);
	//			aPlayer[1] = v3 - (pVtx[WALL_WIDE_FIELD + nWide + 2 + ((WALL_WIDE_FIELD + 1) * nDepth)].pos + FieldPos);

	//			aWork[2] = (pVtx[nWide + ((WALL_WIDE_FIELD + 1) * nDepth)].pos + FieldPos) - (pVtx[WALL_WIDE_FIELD + nWide + 2 + ((WALL_WIDE_FIELD + 1) * nDepth)].pos + FieldPos);
	//			aPlayer[2] = v3 - (pVtx[nWide + ((WALL_WIDE_FIELD + 1) * nDepth)].pos + FieldPos);

	//			float fAnswer[3];

	//			//�@��
	//			fAnswer[0] = aWork[0].x * aPlayer[0].z - aWork[0].z * aPlayer[0].x;
	//			fAnswer[1] = aWork[1].x * aPlayer[1].z - aWork[1].z * aPlayer[1].x;
	//			fAnswer[2] = aWork[2].x * aPlayer[2].z - aWork[2].z * aPlayer[2].x;

	//			if ((fAnswer[0] > 0 && fAnswer[1] > 0 && fAnswer[2] > 0) || (fAnswer[0] <= 0 && fAnswer[1] <= 0 && fAnswer[2] <= 0))
	//			{
	//				OutputDebugString("FinalPhase\n");
	//				pTarget->SetPosition(v3);
	//				pTarget->SetPosOld(v3);
	//				return v3;
	//			}
	//		}
	//	}
	//}

	{
		for (int nDepth = 0; nDepth < WALL_DEPTH_FIELD; nDepth++)
		{
			for (int nWide = 0; nWide < WALL_WIDE_FIELD; nWide++)
			{

				D3DXVECTOR3 P = pVtx[nWide + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;	// ���ʏ�̓_P
				D3DXVECTOR3 A = pTarget->GetPosition();		// �n�_
				D3DXVECTOR3 B = pTarget->GetPosOld();		// �I�_

				P.y = 0.0f;
				A.y = 0.0f;
				B.y = 0.0f;

				// PA PB�x�N�g��
				D3DXVECTOR3 PA = D3DXVECTOR3(P.x - A.x, P.y - A.y, P.z - A.z);
				D3DXVECTOR3 PB = D3DXVECTOR3(P.x - B.x, P.y - B.y, P.z - B.z);

				D3DXVec3Normalize(&PA, &PA);
				D3DXVec3Normalize(&PB, &PB);

				// PA PB���ꂼ��𕽖ʖ@���Ɠ���
				float dot_PA = D3DXVec3Dot(&PA, &apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)]);
				float dot_PB = D3DXVec3Dot(&PB, &apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)]);

				// �덷���z�����邽�߂̔��蕶
				if (fabs(dot_PA) < 0.000001) { dot_PA = 0.0f; }
				if (fabs(dot_PB) < 0.000001) { dot_PB = 0.0f; }

				// ��������
				if (dot_PA == 0.0f && dot_PB == 0.0f)
				{
					//���[�����ʏ�ɂ���A��_���v�Z�ł��Ȃ��B
					continue;
				}
				else
				{
					if ((dot_PA >= 0.0f && dot_PB <= 0.0f))
						//(dot_PA <= 0.0f && dot_PB >= 0.0f))
					{

					}
					else
					{
						//�������Ă��Ȃ�
						continue;
					}
				}

				// �ȉ���_�����߂�

				D3DXVECTOR3 AB = B - A;

				// ��_��A�̋��� : ��_��B�̋��� = dot_PA : dot_PB
				float hiritu = fabs(dot_PA) / (fabs(dot_PA) + fabs(dot_PB));
				D3DXVECTOR3 Ans = A + (AB * hiritu);

				D3DXVECTOR3 aVertex[4];
				aVertex[0] = pVtx[WALL_WIDE_FIELD + nWide + 1 + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;
				aVertex[1] = pVtx[WALL_WIDE_FIELD + nWide + 2 + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;
				aVertex[2] = pVtx[nWide + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;
				aVertex[3] = pVtx[1 + nWide + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;

				//�����̌v�Z
				aWork[0] = (aVertex[0] + FieldPos) - (aVertex[2] + FieldPos);
				aPlayer[0] = Ans - (aVertex[0] + FieldPos);

				aWork[1] = (aVertex[1] + FieldPos) - (aVertex[0] + FieldPos);
				aPlayer[1] = Ans - (aVertex[1] + FieldPos);

				aWork[2] = (aVertex[2] + FieldPos) - (aVertex[1] + FieldPos);
				aPlayer[2] = Ans - (aVertex[2] + FieldPos);

				float fAnswer[3];
				D3DXVECTOR3 clossAns[3];

				//�@��
				fAnswer[0] = aWork[0].x * aPlayer[0].z - aWork[0].z * aPlayer[0].x - aWork[0].y * aPlayer[0].y;
				fAnswer[1] = aWork[1].x * aPlayer[1].z - aWork[1].z * aPlayer[1].x - aWork[1].y * aPlayer[1].y;
				fAnswer[2] = aWork[2].x * aPlayer[2].z - aWork[2].z * aPlayer[2].x - aWork[2].y * aPlayer[2].y;

				D3DXVec3Cross(&clossAns[0], &aWork[0], &aPlayer[0]);
				D3DXVec3Cross(&clossAns[1], &aWork[1], &aPlayer[1]);
				D3DXVec3Cross(&clossAns[2], &aWork[2], &aPlayer[2]);

				float dot_12 = D3DXVec3Dot(&clossAns[0], &clossAns[1]);
				float dot_13 = D3DXVec3Dot(&clossAns[0], &clossAns[3]);

				float fDistance = CManager::GetDistance(pTarget->GetPosOld(), Ans);

				{
					//D3DXPLANE p;
					//D3DXPlaneFromPoints(&p, &aVertex[0], &aVertex[1], &aVertex[2]);
					////�p�����g���b�N�������̔}��ϐ��h t "�������B
					//FLOAT t = -((p.a * vP[1].x) + (p.b*vP[1].y) + (p.c*vP[1].z) + p.d) /
					//	(((p.a*vP[0].x) + (p.b*vP[0].y) + (p.c*vP[0].z)) - ((p.a*vP[1].x) + (p.b*vP[1].y) + (p.c*vP[1].z)));
					//// t ���O����P�̊Ԃł���Ȃ�������Ă��邱�ƂɂȂ�i���̎��_�ł́A�܂����������ʂƂ̌����j
					//if (t >= 0 && t <= 1.0)
					//{
					//	//��_���W�𓾂�@�����������Ă���Η��[�_����ȒP�ɋ��܂�
					//	D3DXVECTOR3 v;
					//	v.x = t*vP[0].x + (1 - t)*vP[1].x;
					//	v.y = t*vP[0].y + (1 - t)*vP[1].y;
					//	v.z = t*vP[0].z + (1 - t)*vP[1].z;
					//	//��_���O�p�`�̓����O���𔻒�@�i�����œ����ƂȂ�΁A���S�Ȍ����ƂȂ�j
					//	if (CCollider::IsInside(&v, &aVertex[0], &aVertex[1], &aVertex[2]))
					//	{
					//		return true;
					//	}
					//	return false;
					//}
				}

				if (CCollider::IsInside(&Ans, &aVertex[0], &aVertex[1], &aVertex[2]))
				{
					OutputDebugString("FinalPhase\n");
					D3DXVECTOR3 old = pTarget->GetPosOld();
					bHit = true;
					pTarget->SetPosition(D3DXVECTOR3(Ans.x, pTarget->GetPosition().y, Ans.z));
					pTarget->SetPosOld(old);
					nol = apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)];
					break;
					break;
				}

				////if ((fAnswer[0] >= 0 && fAnswer[1] >= 0 && fAnswer[2] >= 0) || (fAnswer[0] < 0 && fAnswer[1] < 0 && fAnswer[2] < 0))
				//{
				//	if ((clossAns[0] > 0 && clossAns[1] > 0 && clossAns[2] > 0) || (clossAns[0] <= 0 && clossAns[1] <= 0 && clossAns[2] <= 0))
				//	{
				//		if (dot_12 > 0 && dot_13 > 0)
				//		{
				//			OutputDebugString("FinalPhase\n");
				//			D3DXVECTOR3 old = pTarget->GetPosOld();
				//			bHit = true;
				//			pTarget->SetPosition(Ans);
				//			pTarget->SetPosOld(old);
				//			nol = apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)];
				//			break;
				//			break;
				//		}
				//	}
				//}

				P = pVtx[nWide + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;	// ���ʏ�̓_P
				A = pTarget->GetPosition();		// �n�_
				B = pTarget->GetPosOld();		// �I�_

				// PA PB�x�N�g��
				PA = D3DXVECTOR3(P.x - A.x, P.y - A.y, P.z - A.z);
				PB = D3DXVECTOR3(P.x - B.x, P.y - B.y, P.z - B.z);

				// PA PB���ꂼ��𕽖ʖ@���Ɠ���
				dot_PA = D3DXVec3Dot(&PA, &apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1]);
				dot_PB = D3DXVec3Dot(&PB, &apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1]);

				// �덷���z�����邽�߂̔��蕶
				if (fabs(dot_PA) < 0.000001) { dot_PA = 0.0f; }
				if (fabs(dot_PB) < 0.000001) { dot_PB = 0.0f; }

				// ��������
				if (dot_PA == 0.0f && dot_PB == 0.0f)
				{
					//���[�����ʏ�ɂ���A��_���v�Z�ł��Ȃ��B
					continue;
				}
				else
				{
					if ((dot_PA >= 0.0f && dot_PB <= 0.0f) ||
						(dot_PA <= 0.0f && dot_PB >= 0.0f))
					{

					}
					else
					{
						//�������Ă��Ȃ�
						continue;
					}
				}

				// ��_��A�̋��� : ��_��B�̋��� = dot_PA : dot_PB
				hiritu = fabs(dot_PA) / (fabs(dot_PA) + fabs(dot_PB));
				Ans = A + (AB * hiritu);

				//�����̌v�Z
				aWork[0] = (aVertex[2] + FieldPos) - (aVertex[3] + FieldPos);
				aPlayer[0] = Ans - (aVertex[3] + FieldPos);

				aWork[1] = (aVertex[3] + FieldPos) - (aVertex[1] + FieldPos);
				aPlayer[1] = Ans - (aVertex[1] + FieldPos);

				aWork[2] = (aVertex[1] + FieldPos) - (aVertex[2] + FieldPos);
				aPlayer[2] = Ans - (aVertex[2] + FieldPos);

				//�@��
				fAnswer[0] = aWork[0].x * aPlayer[0].z - aWork[0].z * aPlayer[0].x - aWork[0].y * aPlayer[0].y;
				fAnswer[1] = aWork[1].x * aPlayer[1].z - aWork[1].z * aPlayer[1].x - aWork[1].y * aPlayer[1].y;
				fAnswer[2] = aWork[2].x * aPlayer[2].z - aWork[2].z * aPlayer[2].x - aWork[2].y * aPlayer[2].y;

				D3DXVec3Cross(&clossAns[0], &aWork[0], &aPlayer[0]);
				D3DXVec3Cross(&clossAns[1], &aWork[1], &aPlayer[1]);
				D3DXVec3Cross(&clossAns[2], &aWork[2], &aPlayer[2]);

				dot_12 = D3DXVec3Dot(&clossAns[0], &clossAns[1]);
				dot_13 = D3DXVec3Dot(&clossAns[0], &clossAns[3]);

				if (CCollider::IsInside(&Ans, &aVertex[1], &aVertex[2], &aVertex[3]))
				{
					OutputDebugString("FinalPhase\n");
					D3DXVECTOR3 old = pTarget->GetPosOld();
					bHit = true;
					pTarget->SetPosition(D3DXVECTOR3(Ans.x, pTarget->GetPosition().y, Ans.z));
					pTarget->SetPosOld(old);
					nol = apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)];
					break;
					break;
				}

				////if ((fAnswer[0] >= 0 && fAnswer[1] >= 0 && fAnswer[2] >= 0) || (fAnswer[0] < 0 && fAnswer[1] < 0 && fAnswer[2] < 0))
				//{
				//	if ((clossAns[0] > 0 && clossAns[1] > 0 && clossAns[2] > 0) || (clossAns[0] <= 0 && clossAns[1] <= 0 && clossAns[2] <= 0))
				//	{
				//		if (dot_12 > 0 && dot_13 > 0)
				//		{
				//			//OutputDebugString("FinalPhase\n");
				//			//D3DXVECTOR3 old = pTarget->GetPosOld();
				//			//bHit = true;
				//			//pTarget->SetPosition(Ans);
				//			//pTarget->SetPosOld(old);
				//			//nol = apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)];
				//			break;
				//			break;
				//		}
				//	}
				//}
			}
		}
	}

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
	return bHit;
}

//=============================================================================
// �|���S���̕ǒʉߔ���
//=============================================================================
bool CMeshWall::GetWallTest(CScene * pTarget, D3DXVECTOR3 & nol, D3DXVECTOR3 &rot)
{
	VERTEX_3D *pVtx;										//���_���ւ̃|�C���^
	bool bHit = false;			// �q�b�g�t���O

	D3DXVECTOR3 FieldPos = GetPosition();

	D3DXVECTOR3 AB;
	D3DXVECTOR3 BC;

	D3DXVECTOR3 point1;
	D3DXVECTOR3 point2;
	D3DXVECTOR3 point3;

	D3DXVECTOR3 aWork[3];
	D3DXVECTOR3 aPlayer[3];
	float fHeight = 0;

	D3DXVECTOR3 pointA, pointB;

	pointA = pTarget->GetPosition();
	pointB = pTarget->GetPosOld();

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nDepth = 0; nDepth < WALL_DEPTH_FIELD; nDepth++)
	{
		for (int nWide = 0; nWide < WALL_WIDE_FIELD; nWide++)
		{

			D3DXVECTOR3 P = pVtx[nWide + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;	// ���ʏ�̓_P
			D3DXVECTOR3 A = pTarget->GetPosition();		// �n�_
			D3DXVECTOR3 B = pTarget->GetPosOld();		// �I�_

			P.y = 0.0f;
			A.y = 0.0f;
			B.y = 0.0f;

			// PA PB�x�N�g��
			D3DXVECTOR3 PA = D3DXVECTOR3(P.x - A.x, P.y - A.y, P.z - A.z);
			D3DXVECTOR3 PB = D3DXVECTOR3(P.x - B.x, P.y - B.y, P.z - B.z);

			D3DXVec3Normalize(&PA, &PA);
			D3DXVec3Normalize(&PB, &PB);

			// PA PB���ꂼ��𕽖ʖ@���Ɠ���
			float dot_PA = D3DXVec3Dot(&PA, &apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)]);
			float dot_PB = D3DXVec3Dot(&PB, &apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)]);

			// �덷���z�����邽�߂̔��蕶
			if (fabs(dot_PA) < 0.000001) { dot_PA = 0.0f; }
			if (fabs(dot_PB) < 0.000001) { dot_PB = 0.0f; }

			// ��������
			if (dot_PA == 0.0f && dot_PB == 0.0f)
			{
				//���[�����ʏ�ɂ���A��_���v�Z�ł��Ȃ��B
				continue;
			}
			else
			{
				if ((dot_PA >= 0.0f && dot_PB <= 0.0f))
					//(dot_PA <= 0.0f && dot_PB >= 0.0f))
				{

				}
				else
				{
					//�������Ă��Ȃ�
					continue;
				}
			}

			// �ȉ���_�����߂�

			D3DXVECTOR3 AB = B - A;

			// ��_��A�̋��� : ��_��B�̋��� = dot_PA : dot_PB
			float hiritu = fabs(dot_PA) / (fabs(dot_PA) + fabs(dot_PB));
			D3DXVECTOR3 Ans = A + (AB * hiritu);

			D3DXVECTOR3 aVertex[4];
			aVertex[0] = pVtx[WALL_WIDE_FIELD + nWide + 1 + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;
			aVertex[1] = pVtx[WALL_WIDE_FIELD + nWide + 2 + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;
			aVertex[2] = pVtx[nWide + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;
			aVertex[3] = pVtx[1 + nWide + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;

			//�����̌v�Z
			aWork[0] = (aVertex[0] + FieldPos) - (aVertex[2] + FieldPos);
			aPlayer[0] = Ans - (aVertex[0] + FieldPos);

			aWork[1] = (aVertex[1] + FieldPos) - (aVertex[0] + FieldPos);
			aPlayer[1] = Ans - (aVertex[1] + FieldPos);

			aWork[2] = (aVertex[2] + FieldPos) - (aVertex[1] + FieldPos);
			aPlayer[2] = Ans - (aVertex[2] + FieldPos);

			float fAnswer[3];
			D3DXVECTOR3 clossAns[3];

			//�@��
			fAnswer[0] = aWork[0].x * aPlayer[0].z - aWork[0].z * aPlayer[0].x - aWork[0].y * aPlayer[0].y;
			fAnswer[1] = aWork[1].x * aPlayer[1].z - aWork[1].z * aPlayer[1].x - aWork[1].y * aPlayer[1].y;
			fAnswer[2] = aWork[2].x * aPlayer[2].z - aWork[2].z * aPlayer[2].x - aWork[2].y * aPlayer[2].y;

			D3DXVec3Cross(&clossAns[0], &aWork[0], &aPlayer[0]);
			D3DXVec3Cross(&clossAns[1], &aWork[1], &aPlayer[1]);
			D3DXVec3Cross(&clossAns[2], &aWork[2], &aPlayer[2]);

			float dot_12 = D3DXVec3Dot(&clossAns[0], &clossAns[1]);
			float dot_13 = D3DXVec3Dot(&clossAns[0], &clossAns[3]);

			float fDistance = CManager::GetDistance(pTarget->GetPosOld(), Ans);

			RAY ray;
			ray.vPoint[0] = pTarget->GetPosOld();
			ray.vPoint[1] = pTarget->GetPosition();
			ray.fYaw = 0.0f;
			ray.vPos = D3DXVECTOR3_ZERO;

			POLYGON poly;
			poly.vPoint[0] = aVertex[0];
			poly.vPoint[1] = aVertex[1];
			poly.vPoint[2] = aVertex[2];

			if (Intersect(&ray, &poly))
			{
				OutputDebugString("FinalPhase\n");
				D3DXVECTOR3 old = pTarget->GetPosOld();
				bHit = true;
				pTarget->SetPosition(D3DXVECTOR3(Ans.x, pTarget->GetPosition().y, Ans.z));
				pTarget->SetPosOld(old);
				nol = apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)];
				break;
				break;
			}

			////if ((fAnswer[0] >= 0 && fAnswer[1] >= 0 && fAnswer[2] >= 0) || (fAnswer[0] < 0 && fAnswer[1] < 0 && fAnswer[2] < 0))
			//{
			//	if ((clossAns[0] > 0 && clossAns[1] > 0 && clossAns[2] > 0) || (clossAns[0] <= 0 && clossAns[1] <= 0 && clossAns[2] <= 0))
			//	{
			//		if (dot_12 > 0 && dot_13 > 0)
			//		{
			//			OutputDebugString("FinalPhase\n");
			//			D3DXVECTOR3 old = pTarget->GetPosOld();
			//			bHit = true;
			//			pTarget->SetPosition(Ans);
			//			pTarget->SetPosOld(old);
			//			nol = apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)];
			//			break;
			//			break;
			//		}
			//	}
			//}

			P = pVtx[nWide + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;	// ���ʏ�̓_P
			A = pTarget->GetPosition();		// �n�_
			B = pTarget->GetPosOld();		// �I�_

											// PA PB�x�N�g��
			PA = D3DXVECTOR3(P.x - A.x, P.y - A.y, P.z - A.z);
			PB = D3DXVECTOR3(P.x - B.x, P.y - B.y, P.z - B.z);

			// PA PB���ꂼ��𕽖ʖ@���Ɠ���
			dot_PA = D3DXVec3Dot(&PA, &apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1]);
			dot_PB = D3DXVec3Dot(&PB, &apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1]);

			// �덷���z�����邽�߂̔��蕶
			if (fabs(dot_PA) < 0.000001) { dot_PA = 0.0f; }
			if (fabs(dot_PB) < 0.000001) { dot_PB = 0.0f; }

			// ��������
			if (dot_PA == 0.0f && dot_PB == 0.0f)
			{
				//���[�����ʏ�ɂ���A��_���v�Z�ł��Ȃ��B
				continue;
			}
			else
			{
				if ((dot_PA >= 0.0f && dot_PB <= 0.0f) ||
					(dot_PA <= 0.0f && dot_PB >= 0.0f))
				{

				}
				else
				{
					//�������Ă��Ȃ�
					continue;
				}
			}

			// ��_��A�̋��� : ��_��B�̋��� = dot_PA : dot_PB
			hiritu = fabs(dot_PA) / (fabs(dot_PA) + fabs(dot_PB));
			Ans = A + (AB * hiritu);

			//�����̌v�Z
			aWork[0] = (aVertex[2] + FieldPos) - (aVertex[3] + FieldPos);
			aPlayer[0] = Ans - (aVertex[3] + FieldPos);

			aWork[1] = (aVertex[3] + FieldPos) - (aVertex[1] + FieldPos);
			aPlayer[1] = Ans - (aVertex[1] + FieldPos);

			aWork[2] = (aVertex[1] + FieldPos) - (aVertex[2] + FieldPos);
			aPlayer[2] = Ans - (aVertex[2] + FieldPos);

			//�@��
			fAnswer[0] = aWork[0].x * aPlayer[0].z - aWork[0].z * aPlayer[0].x - aWork[0].y * aPlayer[0].y;
			fAnswer[1] = aWork[1].x * aPlayer[1].z - aWork[1].z * aPlayer[1].x - aWork[1].y * aPlayer[1].y;
			fAnswer[2] = aWork[2].x * aPlayer[2].z - aWork[2].z * aPlayer[2].x - aWork[2].y * aPlayer[2].y;

			D3DXVec3Cross(&clossAns[0], &aWork[0], &aPlayer[0]);
			D3DXVec3Cross(&clossAns[1], &aWork[1], &aPlayer[1]);
			D3DXVec3Cross(&clossAns[2], &aWork[2], &aPlayer[2]);

			dot_12 = D3DXVec3Dot(&clossAns[0], &clossAns[1]);
			dot_13 = D3DXVec3Dot(&clossAns[0], &clossAns[3]);

			ray.vPoint[0] = pTarget->GetPosOld();
			ray.vPoint[1] = pTarget->GetPosition();
			ray.fYaw = 0.0f;
			ray.vPos = D3DXVECTOR3_ZERO;

			poly.vPoint[0] = aVertex[1];
			poly.vPoint[1] = aVertex[2];
			poly.vPoint[2] = aVertex[3];

			if (Intersect(&ray, &poly))
			{
				OutputDebugString("FinalPhase\n");
				D3DXVECTOR3 old = pTarget->GetPosOld();
				bHit = true;
				pTarget->SetPosition(D3DXVECTOR3(Ans.x, pTarget->GetPosition().y, Ans.z));
				pTarget->SetPosOld(old);
				nol = apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)];
				break;
				break;
			}

			////if ((fAnswer[0] >= 0 && fAnswer[1] >= 0 && fAnswer[2] >= 0) || (fAnswer[0] < 0 && fAnswer[1] < 0 && fAnswer[2] < 0))
			//{
			//	if ((clossAns[0] > 0 && clossAns[1] > 0 && clossAns[2] > 0) || (clossAns[0] <= 0 && clossAns[1] <= 0 && clossAns[2] <= 0))
			//	{
			//		if (dot_12 > 0 && dot_13 > 0)
			//		{
			//			//OutputDebugString("FinalPhase\n");
			//			//D3DXVECTOR3 old = pTarget->GetPosOld();
			//			//bHit = true;
			//			//pTarget->SetPosition(Ans);
			//			//pTarget->SetPosOld(old);
			//			//nol = apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)];
			//			break;
			//			break;
			//		}
			//	}
			//}
		}
	}

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
	return bHit;
}

//=============================================================================
// X����Z���̓����蔻��
//=============================================================================
bool CMeshWall::SphereModel(D3DXVECTOR3 C1, D3DXVECTOR3 C2, float R1)
{
	bool bDetection = false;
	//D3DXVECTOR3 C3;							//�v�Z�p
	D3DXVECTOR3 save = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//�ۑ��p
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if ((C1.x - C2.x) * (C1.x - C2.x) + (C1.z - C2.z) * (C1.z - C2.z) <= R1 * R1)
	{
		bDetection = true;
	}
	return bDetection;
}

//====================================================================
// �t�B�[���h�͈͓����ǂ���
//====================================================================
bool CMeshWall::CollisionRange(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 FieldPos = GetPosition();
	bool bIn = false;

	if (FieldPos.x - (WALL_WIDE_FIELD * SIZE) / 2 < pos.x)
	{
		if (FieldPos.x + (WALL_WIDE_FIELD * SIZE) / 2 > pos.x)
		{
			if (FieldPos.z - (WALL_WIDE_FIELD * SIZE) / 2 < pos.z)
			{
				if (FieldPos.z + (WALL_WIDE_FIELD * SIZE) / 2 > pos.z)
				{
					bIn = true;
				}
			}
		}
	}

	return bIn;
}

//=============================================================================
// �~�͈͓̔�����
//=============================================================================
bool CMeshWall::CollisionRangeToCircle(D3DXVECTOR3 pos, float fRadius)
{
	D3DXVECTOR3 FieldPos = GetPosition();
	bool bIn = false;																// �����蔻�� �̏�����

	// ���E�̔��a���̓����蔻��
	if ((FieldPos.x - (WALL_WIDE_FIELD * SIZE) / 2) - fRadius < pos.x)
	{// ���Ɠ_
		if ((FieldPos.x + (WALL_WIDE_FIELD * SIZE) / 2) + fRadius > pos.x)
		{// �E�Ɠ_
			if (FieldPos.z - (WALL_WIDE_FIELD * SIZE) / 2 < pos.z)
			{// ���Ɠ_
				if (FieldPos.z + (WALL_WIDE_FIELD * SIZE) / 2 > pos.z)
				{// ��O�Ɠ_
					bIn = true;														// �����蔻�� �̃t���O�𗧂Ă�
				}
			}
		}
	}
	if (!bIn)
	{// �����蔻�� �̃t���O�������Ă��Ȃ������Ƃ�
	 // ���s�����a���̓����蔻��
		if (FieldPos.x - (WALL_WIDE_FIELD * SIZE) / 2 < pos.x)
		{// ���Ɠ_
			if (FieldPos.x + (WALL_WIDE_FIELD * SIZE) / 2 > pos.x)
			{// �E�Ɠ_
				if ((FieldPos.z - (WALL_WIDE_FIELD * SIZE) / 2) - fRadius < pos.z)
				{// ���Ɠ_
					if ((FieldPos.z + (WALL_WIDE_FIELD * SIZE) / 2) + fRadius > pos.z)
					{// ��O�Ɠ_
						bIn = true;													// �����蔻�� �̃t���O�𗧂Ă�
					}
				}
			}
		}
	}
	if (!bIn)
	{// �����蔻�� �̃t���O�������Ă��Ȃ������Ƃ�
	 // �p�̉~�Ɠ_�ɂ�铖���蔻��
		if (CollisionCircle(pos, fRadius))
		{// �����蔻�� �̃t���O���������Ƃ�
			bIn = true;
		}
	}

	return bIn;																		// �����蔻�� �̃t���O��Ԃ�Ԃ�
}

//=============================================================================
// �_�Ɖ~�̓����蔻��
//=============================================================================
bool CMeshWall::CollisionCircle(D3DXVECTOR3 pos, float fRadius)
{
	D3DXVECTOR3 FieldPos = GetPosition();
	bool bIn = false;		// �����蔻��̏�����

	if (((FieldPos.x - (WALL_WIDE_FIELD * SIZE) / 2 - pos.x) * (FieldPos.x - (WALL_WIDE_FIELD * SIZE) / 2 - pos.x)) +
		((FieldPos.z - (WALL_DEPTH_FIELD * SIZE) / 2 - pos.z) * (FieldPos.z - (WALL_DEPTH_FIELD * SIZE) / 2 - pos.z)) <= fRadius * fRadius)
	{// ����Ɠ_
		bIn = true;			// �����蔻�� �̃t���O�𗧂Ă�
	}
	else if (((FieldPos.x + (WALL_WIDE_FIELD * SIZE) / 2 - pos.x) * (FieldPos.x + (WALL_WIDE_FIELD * SIZE) / 2 - pos.x)) +
		((FieldPos.z - (WALL_DEPTH_FIELD * SIZE) / 2 - pos.z) * (FieldPos.z - (WALL_DEPTH_FIELD * SIZE) / 2 - pos.z)) <= fRadius * fRadius)
	{// �E��Ɠ_
		bIn = true;			// �����蔻�� �̃t���O�𗧂Ă�
	}
	else if (((FieldPos.x - (WALL_WIDE_FIELD * SIZE) / 2 - pos.x) * (FieldPos.x - (WALL_WIDE_FIELD * SIZE) / 2 - pos.x)) +
		((FieldPos.z + (WALL_DEPTH_FIELD * SIZE) / 2 - pos.z) * (FieldPos.z + (WALL_DEPTH_FIELD * SIZE) / 2 - pos.z)) <= fRadius * fRadius)
	{// �����Ɠ_
		bIn = true;			// �����蔻�� �̃t���O�𗧂Ă�
	}
	else if (((FieldPos.x + (WALL_WIDE_FIELD * SIZE) / 2 - pos.x) * (FieldPos.x + (WALL_WIDE_FIELD * SIZE) / 2 - pos.x)) +
		((FieldPos.z + (WALL_DEPTH_FIELD * SIZE) / 2 - pos.z) * (FieldPos.z + (WALL_DEPTH_FIELD * SIZE) / 2 - pos.z)) <= fRadius * fRadius)
	{// �E���Ɠ_
		bIn = true;			// �����蔻�� �̃t���O�𗧂Ă�
	}

	return bIn;			// �����蔻�� �t���O�̏�Ԃ�Ԃ�
}

//=============================================================================
// �@���̌v�Z
//=============================================================================
void CMeshWall::CalculationNormalize(void)
{
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �@�����߂�
	for (int nDepth = 0; nDepth < WALL_DEPTH_FIELD; nDepth++)
	{
		for (int nWide = 0; nWide < WALL_WIDE_FIELD; nWide++)
		{
			D3DXVECTOR3 AB = pVtx[nWide + ((WALL_WIDE_FIELD + 1) * nDepth)].pos - pVtx[WALL_WIDE_FIELD + 1 + nWide + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;
			D3DXVECTOR3 BC = pVtx[nWide + ((WALL_WIDE_FIELD + 1) * nDepth)].pos - pVtx[WALL_WIDE_FIELD + nWide + 2 + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;

			D3DXVec3Cross(&apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)], &BC, &AB);
			D3DXVec3Normalize(&apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)], &apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)]);

			AB = pVtx[nWide + ((WALL_WIDE_FIELD + 1) * nDepth)].pos - pVtx[WALL_WIDE_FIELD + nWide + 2 + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;
			BC = pVtx[nWide + ((WALL_WIDE_FIELD + 1) * nDepth)].pos - pVtx[1 + nWide + ((WALL_WIDE_FIELD + 1) * nDepth)].pos;

			D3DXVec3Cross(&apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1], &BC, &AB);
			D3DXVec3Normalize(&apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1], &apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1]);
		}
	}

	// ���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �@�����ϋ��߂�
	for (int nDepth = 0; nDepth < WALL_DEPTH_FIELD + 1; nDepth++)
	{
		for (int nWide = 0; nWide < WALL_WIDE_FIELD + 1; nWide++)
		{
			// ��ԏ�̒i
			if (nDepth == 0)
			{
				if (nWide != WALL_WIDE_FIELD)
				{//�Ō���ł͂Ȃ��ꍇ
					if (nWide == 0)
					{// ��ԍŏ�
					 // �悱
						pVtx[(WALL_WIDE_FIELD + 1) * nDepth + nWide].nor =
							(apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)] +
								apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1])
							/ 2;
					}
					else if (nWide != WALL_WIDE_FIELD)
					{
						pVtx[(WALL_WIDE_FIELD + 1) * nDepth + nWide].nor =
							(apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide) - 1] +
								apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)] +
								apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1])
							/ 3;
					}
				}
			}
			else if (nDepth != WALL_DEPTH_FIELD)
			{// ��ԉ��̒i�ł͂Ȃ��ꍇ
				if (nWide == 0)
				{// ��Ԓ[�����������Ƃ�
					pVtx[(WALL_WIDE_FIELD + 1) * nDepth + nWide].nor =
						(apNor[(nDepth - 1) * 2 * WALL_WIDE_FIELD + (2 * nWide)] +
							apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)] +
							apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1])
						/ 3;
				}
				else if (nWide == WALL_WIDE_FIELD)
				{// ���Ō�
					pVtx[(WALL_WIDE_FIELD + 1) * nDepth + nWide].nor =
						(apNor[(nDepth - 1) * 2 * WALL_WIDE_FIELD + (2 * nWide) - 2] +
							apNor[(nDepth - 1) * 2 * WALL_WIDE_FIELD + (2 * nWide) - 1] +
							apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide) - 1])
						/ 3;
				}
				else
				{// �^��
					pVtx[(WALL_WIDE_FIELD + 1) * nDepth + nWide].nor =
						(apNor[(nDepth - 1) * 2 * WALL_WIDE_FIELD + (2 * nWide) - 2] +
							apNor[(nDepth - 1) * 2 * WALL_WIDE_FIELD + (2 * nWide) - 1] +
							apNor[(nDepth - 1) * 2 * WALL_WIDE_FIELD + (2 * nWide)] +
							apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide) - 1] +
							apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide)] +
							apNor[((WALL_WIDE_FIELD * 2) * nDepth) + (2 * nWide) + 1])
						/ 6;
				}
			}
			else
			{
				if (nWide == 0)
				{// ��ԍŏ��ł͂Ȃ��ꍇ

				}
				else if (nWide == WALL_WIDE_FIELD)
				{
					pVtx[(WALL_WIDE_FIELD + 1) * nDepth + nWide].nor =
						(apNor[(nDepth - 1) * 2 * WALL_WIDE_FIELD + (2 * nWide) - 2] +
							apNor[(nDepth - 1) * 2 * WALL_WIDE_FIELD + (2 * nWide) - 1])
						/ 2;
				}
				else
				{
					pVtx[(WALL_WIDE_FIELD + 1) * nDepth + nWide].nor =
						(apNor[(nDepth - 1) * 2 * WALL_WIDE_FIELD + (2 * nWide) - 2] +
							apNor[(nDepth - 1) * 2 * WALL_WIDE_FIELD + (2 * nWide) - 1] +
							apNor[(nDepth - 1) * 2 * WALL_WIDE_FIELD + (2 * nWide)])
						/ 3;
				}
			}
		}
	}

	// ���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���̐�������
//=============================================================================
void CMeshWall::CreateRand(const int &nWide, const int &nDepth, const D3DXVECTOR3 &createPos)
{
	for (int nCntDepth = 0; nCntDepth < nDepth; nCntDepth++)
	{
		for (int nCntWide = 0; nCntWide < nWide; nCntWide++)
		{
			CMeshWall *pMeshWall = CMeshWall::Create();

			if (pMeshWall != NULL)
			{
				D3DXVECTOR3 pos;

				pos.x = (WALL_WIDE_FIELD * SIZE) * nCntWide - (((WALL_WIDE_FIELD * SIZE) * nWide) / 2) + ((WALL_WIDE_FIELD * SIZE) / 2) + createPos.x;
				pos.y = createPos.y;
				pos.z = (WALL_DEPTH_FIELD * SIZE) * nCntDepth - (((WALL_DEPTH_FIELD * SIZE) * nDepth) / 2) + ((WALL_DEPTH_FIELD * SIZE) / 2) + createPos.z;

				pMeshWall->SetPosition(pos);
			}
		}
	}
}

//=============================================================================
// �C���X�y�N�^�[�̕\��
//=============================================================================
void CMeshWall::ShowInspector(void)
{

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void CMeshWall::MakeVertex(FILE *pFile, bool bDebug)
{
	VERTEX_3D *pVtx;
	char cReadText[128];															// ����
	char cHeadText[128];															// ��r
	float aData[7];																	// ����

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nDepth = 0; nDepth < WALL_DEPTH_FIELD + 1; nDepth++)
	{
		for (int nWide = 0; nWide < WALL_WIDE_FIELD + 1; nWide++)
		{
			if (pFile != NULL)
			{

				fgets(cReadText, sizeof(cReadText), pFile);							// �f�[�^����1�s�擾
				sscanf(cReadText, "%s", &cHeadText);								// �s��ǂݍ���

				// �f�[�^��������`���ɕ���
				CManager::ConvertStringToFloat(cReadText, ",", aData);

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(aData[0], aData[1], aData[2]);			// �f�[�^�𓖂Ă͂߂�

				D3DXCOLOR col = D3DXCOLOR(aData[3], aData[4], aData[5], aData[6]);

				//���_�J���[
				pVtx[0].col = col;	// �f�[�^�𓖂Ă͂߂�

				if (bDebug)
				{
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �f�[�^�𓖂Ă͂߂�
				}

			}
			else
			{
				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3((-SIZE * WALL_WIDE_FIELD) / 2 + SIZE * nWide, 0.0f, (SIZE * WALL_DEPTH_FIELD) / 2 - SIZE * nDepth);

				if (!bDebug)
				{
					//���_�J���[
					pVtx[0].col = m_col;
				}
				else
				{
					if (nDepth >= WALL_DEPTH_FIELD || nWide >= WALL_WIDE_FIELD)
					{
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// �f�[�^�𓖂Ă͂߂�
					}
					else
					{
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);	// �f�[�^�𓖂Ă͂߂�
					}
				}
			}

			//�@��
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//�e�N�X�`���`�ʂ̈ʒu
			pVtx[0].tex = D3DXVECTOR2(1.0f * nWide, 1.0f * nDepth);

			pVtx++;
		}
	}

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �C���f�b�N�X���蓖��
//=============================================================================
void CMeshWall::MakeIndex(void)
{
	WORD *pIdx;									//�C���f�b�N�X�f�[�^�փ|�C���^
	int nCount = 0;

	//�C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntY = 0; nCntY < WALL_DEPTH_FIELD; nCntY++)
	{
		if (nCntY != 0)
		{
			pIdx[0] = (WALL_WIDE_FIELD + 1) + nCntY * (WALL_WIDE_FIELD + 1);
			pIdx++;
			nCount++;
		}

		for (int nCntX = 0; nCntX < WALL_WIDE_FIELD + 1; nCntX++)
		{
			pIdx[0] = (WALL_WIDE_FIELD + 1) + nCntX + nCntY * (WALL_WIDE_FIELD + 1);
			pIdx[1] = nCntX + nCntY * (WALL_WIDE_FIELD + 1);
			pIdx += 2;
			nCount += 2;
		}

		if (WALL_DEPTH_FIELD > nCntY + 1)
		{
			pIdx[0] = WALL_WIDE_FIELD + nCntY * (WALL_WIDE_FIELD + 1);
			pIdx++;
			nCount++;
		}
	}

	//�C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^
	m_pIdxBuff->Unlock();
}

//=============================================================================
// �n�`����ۑ��֐�
//=============================================================================
void CMeshWall::SaveWall(CWrite *pWrite)
{
	VERTEX_3D *pVtx;				// ���_���ւ̃|�C���^

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nDepth = 0; nDepth < WALL_DEPTH_FIELD + 1; nDepth++)
	{
		for (int nWide = 0; nWide < WALL_WIDE_FIELD + 1; nWide++)
		{
			//���_���W�̎擾
			D3DXVECTOR3 pos = pVtx[0].pos;
			D3DXCOLOR col = pVtx[0].col;

			// ���_���̏�������
			pWrite->Write("		%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n", pos.x, pos.y, pos.z, col.r, col.g, col.b, col.a);
			pVtx++;				// �|�C���^���V�t�g
		}
	}

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �ǂ̒��_���ҏW����
//=============================================================================
void CMeshWall::EditWallVertex(void)
{
	VERTEX_3D *pVtx;				// ���_���ւ̃|�C���^

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nDepth = 0; nDepth < WALL_DEPTH_FIELD + 1; nDepth++)
	{
		for (int nWide = 0; nWide < WALL_WIDE_FIELD + 1; nWide++)
		{
			//���_���W�̎擾
			char label[16];
			sprintf(label, "pos %d", nWide + WALL_DEPTH_FIELD * nDepth);
			D3DXVECTOR3 pos = pVtx[0].pos;
			ImGui::DragFloat3(label, (float*)&pos);
			pVtx[0].pos = pos;
			pVtx++;				// �|�C���^���V�t�g
		}
	}

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//
//BOOL Intersect(RAY* pRay,POLYGON* pPoly)
//���������ʂƐ����̌����𔻒� �i�������Ă���ꍇ��true���A���Ă��Ȃ��ꍇ��false��Ԃ��j
bool CMeshWall::Intersect(RAY* pRay, POLYGON* pPoly)
{
	D3DXVECTOR3 vP[2];

	//�����i���C�j�̒[�_�𓾂�
	{
		vP[0] = pRay->vPoint[0];
		vP[1] = pRay->vPoint[1];

		D3DXMATRIX mRotation;
		D3DXMatrixRotationY(&mRotation, pRay->fYaw);
		D3DXVec3TransformCoord(&vP[0], &vP[0], &mRotation);
		D3DXVec3TransformCoord(&vP[1], &vP[1], &mRotation);
		vP[0] += pRay->vPos;
		vP[1] += pRay->vPos;
	}
	//���蕔��
	{
		D3DXPLANE p;
		D3DXPlaneFromPoints(&p, &pPoly->vPoint[0], &pPoly->vPoint[1], &pPoly->vPoint[2]);
		//�p�����g���b�N�������̔}��ϐ��h t "�������B
		FLOAT t = -((p.a * vP[1].x) + (p.b*vP[1].y) + (p.c*vP[1].z) + p.d) /
			(((p.a*vP[0].x) + (p.b*vP[0].y) + (p.c*vP[0].z)) - ((p.a*vP[1].x) + (p.b*vP[1].y) + (p.c*vP[1].z)));
		// t ���O����P�̊Ԃł���Ȃ�������Ă��邱�ƂɂȂ�i���̎��_�ł́A�܂����������ʂƂ̌����j
		if (t >= 0 && t <= 1.0)
		{
			//��_���W�𓾂�@�����������Ă���Η��[�_����ȒP�ɋ��܂�
			D3DXVECTOR3 v;
			v.x = t*vP[0].x + (1 - t)*vP[1].x;
			v.y = t*vP[0].y + (1 - t)*vP[1].y;
			v.z = t*vP[0].z + (1 - t)*vP[1].z;
			//��_���O�p�`�̓����O���𔻒�@�i�����œ����ƂȂ�΁A���S�Ȍ����ƂȂ�j
			if (CCollider::IsInside(&v, &pPoly->vPoint[0], &pPoly->vPoint[1], &pPoly->vPoint[2]) == true)
			{
				return true;
			}
			return false;
		}
	}
	return false;
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CMeshWall::Debug(void)
{

}
#endif