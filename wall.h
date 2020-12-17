//=============================================================================
//
// �Ǐ��� [wall.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define WALL_DEPTH_FIELD	50							// ���s��
#define WALL_WIDE_FIELD 1								// ��
#define SIZE		50.0f								// �T�C�Y

struct RAY //���C
{
	D3DXVECTOR3 vPoint[2];
	D3DXVECTOR3 vDirection;
	D3DXVECTOR3 vPos;
	FLOAT fYaw;

	RAY()
	{
		ZeroMemory(this, sizeof(RAY));
	}
};

struct POLYGON //���ʁi���ʂƌ����Ă��A���������ʏ��3���_�������Ă���ɉ߂��Ȃ��j
{
	D3DXVECTOR3 vPoint[3];
	POLYGON()
	{
		ZeroMemory(this, sizeof(POLYGON));
	}
};

//=============================================================================
// �N���X��`
//=============================================================================
class CMeshWall : public CScene
{
public:
	CMeshWall(PRIORITY obj);														// �R���X�g���N�^
	~CMeshWall();																	// �f�X�g���N�^
	HRESULT Init(void);																// ����������
	void Uninit(void);																// �J������
	void Update(void);																// �X�V����
	void Draw(void);																// �`�揈��

	static CMeshWall *Create(void);													// �N���G�C�g����
	static HRESULT Load(void);														// ���[�h����

	static void LoadWall(char *add, bool bDebug);									// �n�ʂ̍����ǂݍ���
	bool GetWallHit(CScene *pTarget, D3DXVECTOR3 &nol);								// �|���S���̍��������߂�
	bool GetWallTest(CScene *pTarget, D3DXVECTOR3 &nol, D3DXVECTOR3 &rot);

	static bool SphereModel(D3DXVECTOR3 C1, D3DXVECTOR3 C2, float R1);				// X����Z���̓����蔻��
	bool CollisionRange(D3DXVECTOR3 pos);											// �����͈͓̔��ɂ��邩�ǂ���
	bool CollisionRangeToCircle(D3DXVECTOR3 pos, float fRadius);					// �~�������͈͓̔��ɂ��邩�ǂ���
	bool CollisionCircle(D3DXVECTOR3 pos, float fRadius);							// �~�Ɠ_�̓����蔻��
	void CalculationNormalize(void);												// �@���̌v�Z

	void SetColor(const D3DXCOLOR &col) { m_col = col; }							// �n�ʂ̐F��ݒ�

	void SetDebugRand(const bool &bValue) { m_bDebug = bValue; }					// �f�o�b�O�p�t���O�̐ݒ�
	bool GetDebugRand(void) { return m_bDebug; }									// �f�o�b�O�p�t���O�̎擾

	void SetCalculation(const bool &bValue) { m_bCalculation = bValue; }			// �N���v�Z�t���O�̐ݒ�
	const bool &GetCalculation(void) { return m_bCalculation; }						// �N���v�Z�t���O�̎擾

	static void CreateRand(const int &nWide, const int &nDepth, const D3DXVECTOR3 &createPos);

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void);

	void MakeVertex(FILE *pFile, bool bDebug);										// ���_���̍쐬

	void SaveWall(CWrite *pWrite);			// �Ǐ��ۑ�����
	void EditWallVertex(void);				// �ǂ̒��_���ҏW����

private:
#ifdef _DEBUG
	void Debug(void);																// �f�o�b�O�����֐�
#endif
	static bool Intersect(RAY* pRay, POLYGON* pPoly);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;												// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;												// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	void MakeIndex(void);															// ���_�C���f�b�N�X�̍쐬

	D3DXVECTOR3 apNor[WALL_DEPTH_FIELD * WALL_WIDE_FIELD * 2];								// 1�ʂ��Ƃ̖@���x�N�g���̔z��

	D3DXVECTOR3				m_rot;													// �|���S���̌���(��])
	D3DXCOLOR				m_col;													// ���_�J���[
	D3DXMATRIX				m_mtxWorld;												// ���[���h�}�g���b�N�X

	int m_nNumVertex;																// �����_��
	int m_nNumIndex;																// �C���f�b�N�X��
	int m_nNumPolygon;																// ���|���S����
	bool m_bDebug;																	// �f�o�b�O�Ƃ��Ẵt���O
	bool m_bCalculation;															// �N���v�Z�t���O
};
#endif