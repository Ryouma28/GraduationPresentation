//=============================================================================
//
// �ē���󏈗� [GuideSign.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "GuideSign.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "debug.h"
#include "game.h"
#include "takaseiLibrary.h"
#include "player.h"
#include "object.h"
#include "math.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define GUIDESIGN "data/model/Arrow.x"
#define SIGN_DISTANCE 100.0f	// �J��������̋���
#define SIGN_HEIGHT 150.0f	// �v���C���[����̍���
#define ROT_AMOUNT 0.1f		// ��]�̍������炵�Ă�����

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGuideSign::CGuideSign(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CSceneX(obj)
{
	SetObjType(CScene::PRIORITY_UI);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGuideSign::~CGuideSign()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGuideSign::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾

	// ����������
	CSceneX::Init();

	SetShader(SHADERTYPE_TOON);

	LPD3DXMESH		pMesh;							// ���b�V�����ւ̃|�C���^
	DWORD			nNumMat;						// �}�e���A�����̐�
	LPD3DXBUFFER	pBuffMat;						// �}�e���A�����ւ̃|�C���^

	CManager::GetModelResource(GUIDESIGN, pBuffMat, nNumMat, pMesh);
	BindModel(pMesh, nNumMat, pBuffMat);

	D3DXVECTOR3 pos, size, rot;
	pos = ZeroVector3;
	size = D3DXVECTOR3(1.5f, 1.5f, 1.5f);
	rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

	SetPosition(pos);			// �ʒu�̐ݒ�
	SetSize(size);				// �傫���ݒ�
	SetRotation(rot);			// ��]�ݒ�
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CGuideSign::Uninit(void)
{
	// �J������
	CSceneX::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGuideSign::Update(void)
{
	CCamera *pCamera = CManager::GetCamera();	// �J�������擾
	D3DXVECTOR3 rotCamera = ZeroVector3;		// �J�����̉�]�ϐ�
	D3DXVECTOR3 pos = GetPosition();			// �ʒu�擾
	D3DXVECTOR3 rot = GetRotation();			// ��]�擾
	D3DXVECTOR3 posCamera = ZeroVector3;		// �J�����ʒu�ϐ�
	std::vector<CObject*> pointObj = CObject::GetPointObj();
	int pointNum = CObject::GetPointNum();		// ���݂̃|�C���g�ԍ��擾
	D3DXVECTOR3 distance;						// ��_�Ԃ̍�
	D3DXVECTOR3 dest;							// ��]�̍ŏI�ړI�n���W
	D3DXVECTOR3 Diff;							// �v�Z�ϐ�

	// �J�����̏�񂪂���Ƃ�
	if (pCamera != NULL)
	{
		rotCamera = pCamera->GetRotation();	// ��]�擾
		posCamera = pCamera->GetPosition(); // �ʒu�擾
	}

	// ��_�Ԃ̍��v�Z
	distance = posCamera - pointObj[pointNum]->GetPosition();

	// ���̖ړI�n������
	dest.y = (float)atan2(distance.x, distance.z);

	// ���f���̉�]�ƖڕW�n�_�̍����i�[
	Diff.y = rot.y - dest.y;

	// ��]�̕␳
	CTakaseiLibrary::RotRevision(&Diff);

	// ���f�������X�ɉ�]�����Ă���
	rot.y -= Diff.y * ROT_AMOUNT;

	// ��󃂃f���̈ʒu�ݒ�
	pos.x = posCamera.x + sinf(rotCamera.y - D3DX_PI) * SIGN_DISTANCE;
	pos.y = posCamera.y + SIGN_HEIGHT;
	pos.z = posCamera.z + cosf(rotCamera.y - D3DX_PI) * SIGN_DISTANCE;

	// ��]�ݒ�
	SetRotation(rot);

	// �ʒu�ݒ�
	SetPosition(pos);

#ifdef _DEBUG
	Debug();				// �f�o�b�O����
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGuideSign::Draw(void)
{
	CSceneX::Draw();
}

//=============================================================================
// �N���G�C�g�֐�
//=============================================================================
CGuideSign *CGuideSign::Create(void)
{
	CGuideSign *pGuideSign = NULL;
	pGuideSign = new CGuideSign(CScene::PRIORITY_UI);

	if (pGuideSign != NULL)
	{
		pGuideSign->Init();
	}
	return pGuideSign;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CGuideSign::Load(void)
{
	CManager::LoadModel(GUIDESIGN);
	return S_OK;
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CGuideSign::Debug(void)
{

}
#endif