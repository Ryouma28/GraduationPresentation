//=============================================================================
//
// �I�u�W�F�N�g���� [object.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "sceneX.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define DEPTH	20											// ���s��
#define WIDE 20												// ��

//=============================================================================
// �O���錾
//=============================================================================
class CColliderBox;
class CColliderSphere;

//=============================================================================
// �N���X��`
//=============================================================================
class CObject : public CSceneX
{
public:
	// �񋓑̒�`
	typedef enum
	{
		LOADTYPE_ADD = 0,
		LOADTYPE_POS_X,
		LOADTYPE_POS_Y,
		LOADTYPE_POS_Z,
		LOADTYPE_ROT_X,
		LOADTYPE_ROT_Y,
		LOADTYPE_ROT_Z,
		LOADTYPE_MAX
	} LOADTYPE;

	CObject(PRIORITY obj);							// �R���X�g���N�^
	~CObject();										// �f�X�g���N�^
	HRESULT Init(void);								// ����������
	void Uninit(void);								// �J������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��

	static CObject *Create(void);					// �N���G�C�g����
	static HRESULT Load(void);						// ���[�h����
	static void Unload(void);						// �e�N�X�`���̊J������
	static void LoadScript(void);					// �I�u�W�F�N�g�A�Z�b�g�̃��[�h����

	static void LoadModel(char *add);				// ���f���̃��[�h����
	static void LoadModelTest(char *add);			// ���f���̃��[�h����

	void BindModel(std::string add);

	std::string GetAdd(void) { return m_Add; }
	LPD3DXMESH GetMesh(void) { return m_pMesh; }	// ���b�V�����̎擾
	static std::vector<CObject*> GetPointObj(void) { return m_vPointObj; }
	static void SetPointNum(unsigned int pointNum) { m_pointNum = pointNum; }
	static unsigned int GetPointNum(void) { return m_pointNum; }

	void OnTriggerEnter(CCollider *col);
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void);

	static bool Collide(D3DXVECTOR3 vStart, D3DXVECTOR3 vDir, FLOAT* pfDistance, D3DXVECTOR3* pvNormal, D3DXMATRIX mtx);			// �����蔻��
	static HRESULT FindVerticesOnPoly(LPD3DXMESH pMesh, DWORD dwPolyIndex, D3DXVECTOR3* pvVertices);
	static void ReleaseCheckPoint(void);

private:
#ifdef _DEBUG
	void Debug(void);								// �f�o�b�O����
#endif
	void SetAdd(std::string &Add) { m_Add = Add; }

	LPD3DXMESH		m_pMesh;							// ���b�V�����ւ̃|�C���^
	DWORD			m_nNumMat;							// �}�e���A�����̐�
	LPD3DXBUFFER		m_pBuffMat;						// �}�e���A�����ւ̃|�C���^
	CColliderBox *m_pBox;								// �����蔻��{�b�N�X

	static CColliderSphere *m_pSphere;					// �����蔻��X�t�B�A
	static unsigned int m_pointNum;						// �|�C���g�̐�
	static std::vector<CObject*> m_vPointObj;

	std::string m_Add;														// ���f���̃A�h���X
};
#endif