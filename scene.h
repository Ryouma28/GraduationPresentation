//=============================================================================
//
// scene���� [scene.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	MAX_CARTYPE (4)		// �ԗނ̍ő吔
#define	SCRIPT_CAR01 "data/animation/car01.txt"		// ��01�̃��f�����A�h���X
#define	SCRIPT_CAR02 "data/animation/car02.txt"		// ��01�̃��f�����A�h���X
#define	SCRIPT_CAR03 "data/animation/car03.txt"		// ��01�̃��f�����A�h���X
#define	SCRIPT_CAR04 "data/animation/car04.txt"		// ��01�̃��f�����A�h���X

//=============================================================================
// �O���錾
//=============================================================================
class CMeshCube;
class CMeshSphere;
class CCollider;
class CWrite;

//=============================================================================
// �N���X��`
//=============================================================================
class CScene
{
public:
	typedef enum
	{
		PRIORITY_NONE = -1,
		PRIORITY_BG,
		PRIORITY_FLOOR,
		PRIORITY_WALL,
		PRIORITY_SKY,
		PRIORITY_MODEL,
		PRIORITY_PLAYER,
		PRIORITY_ENEMY,
		PRIORITY_CHARACTER,
		PRIORITY_LIFE,
		PRIORITY_TREE,
		PRIORITY_ORBIT,
		PRIORITY_EFFECT,
		PRIORITY_MESH,
		PRIORITY_UI,
		PRIORITY_MAX
	} PRIORITY;

	CScene(PRIORITY Type);						// �R���X�g���N�^
	virtual ~CScene();							// �f�X�g���N�^
	virtual HRESULT Init(void) = 0;				// ����������
	virtual void Uninit(void) = 0;				// �J������
	virtual void Update(void) = 0;				// �X�V����
	virtual void Draw(void) = 0;				// �`�揈��
	static void ReleaseAll(void);				// �S�ẴI�u�W�F�N�g��j�J��
	static void UpdateAll(void);				// �S�ẴI�u�W�F�N�g���X�V
	static void DrawAll(void);					// �S�ẴI�u�W�F�N�g��`��

	void Release(void);							// �폜�\�񏈗�
	void SetObjType(PRIORITY obj);				// �I�u�W�F�N�g�^�C�v�̐ݒ�
	void SetActive(bool bValue);				// �X�V�`��Ώېݒ�
	static void SetShadow(LPDIRECT3DDEVICE9 pDevice, D3DXMATRIX mtxWorld, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPD3DXMESH pMesh, D3DXVECTOR3 pos);		// �e�̐ݒ�												// �e�̐���
	static CScene *NowFloor(D3DXVECTOR3 pos);	// ���ݗ����Ă���ꏊ�̏����擾
	static void ResetFloor(void);				// ���̍����ƐF�����Z�b�g���鏈��
	virtual void SetPosition(D3DXVECTOR3 pos);	// �ʒu�̐ݒ�
	void SetPosOld(D3DXVECTOR3 pos);			// �O�̈ʒu��ݒ�

	static CScene *GetScene(PRIORITY obj);							// �V�[�����擾
	static CScene *GetSceneNext(CScene *pScene, PRIORITY type);		// ���̃V�[�����擾
	D3DXVECTOR3 GetPosition(void) { return m_pos; }					// �ʒu�̎擾
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }				// �O�̈ʒu���擾
	bool GetActive(void) { return m_bActive; }						// �X�V�`��Ώێ擾
	PRIORITY GetObjType(void) { return m_Obj; }						// �I�u�W�F�N�g�^�C�v�̎擾

	virtual void OnTriggerEnter(CCollider *col) = 0;
	virtual void OnCollisionEnter(CCollider *col) = 0;
	virtual void ShowInspector(void);

	static void SaveEnemy(void);					// �G���̏�������
	static void SaveRand(void);						// �����̏�������
	static void SaveModel(void);					// �ؔz�u���̏�������
	static void SaveWall(void);						// �Ǐ��̏�������
	static void SavePoint(CWrite *pWrite, D3DXVECTOR3 &pos);			// �|�C���g���f���̏�������
	void SaveCollider(void);						// �����蔻��̏�������

	static char *GetSceneTypeName(const int &nIndex);

private:
	void Delete(void);								// �J������
	void WriteForSphereCollider(CWrite *pWrite, CCollider *pCollider);				// �X�t�B�A�R���C�_�[�Ǝ��̒l����������
	void WriteForBoxCollider(CWrite *pWrite, CCollider *pCollider);				// �X�t�B�A�R���C�_�[�Ǝ��̒l����������
	static void ShowHierarchy(void);				// �q�G�����L�[�̕\��

	static int m_nNumAll;
	static CScene *m_apCur[PRIORITY_MAX];			// ����(�Ō��)�I�u�W�F�N�g�ւ̃|�C���^
	static CScene *m_apTop[PRIORITY_MAX];			// �擪�I�u�W�F�N�g�ւ̃|�C���^
	CScene *m_pPrev[PRIORITY_MAX];					// �O�̃I�u�W�F�N�g�|�C���^
	CScene *m_pNext[PRIORITY_MAX];					// ���̃I�u�W�F�N�g�|�C���^
	PRIORITY m_Obj;									// �I�u�W�F�N�g�̎��
	bool	m_bActive;								// �`�悷�邩�ǂ���
	bool	m_bDie;									// Release���邩�ǂ���
	std::vector<CCollider*> m_apCollider;			// �����蔻��
	std::string m_Label;							// ���x��

	D3DXVECTOR3 m_pos;								// �ʒu
	D3DXVECTOR3 m_posOld;							// �O��̈ʒu
};
#endif