//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "meshCapsule.h"
#include "meshSphere.h"
#include "meshCube.h"
#include "camera.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MANAGER_PARTICLE "data/text/manager/manager_particle.txt"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CEffect *CEffect::m_pEffect[MAX_EFFECT] = {};
std::map<std::string, EFFECT> CEffect::m_effectMap = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEffect::CEffect(CScene::PRIORITY obj = CScene::PRIORITY_EFFECT) : CScene(obj)
{
	SetObjType(CScene::PRIORITY_EFFECT);								// �I�u�W�F�N�g�^�C�v�̐ݒ�

	/* =============== �l�̏����� =============== */
	m_pVtxBuff = NULL;													// ���_�o�b�t�@�̏�����
	m_pTexture = NULL;													// �e�N�X�`���̏�����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								// �ʒu�̏�����
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);							// �J���[�̏�����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								// ��]�̏�����
	m_size = D3DXVECTOR3(5.0f, 5.0f, 0.0f);								// �T�C�Y�̏�����
	m_move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);								// �ړ��ʂ̏�����
	m_moveSize = D3DXVECTOR3_ZERO;										// �傫���̕ω���
	m_moveRot = D3DXVECTOR3_ZERO;										// ��]�̕ω���
	m_moveCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);						// �F�̕ω���
	m_nLife = 0;														// �ϋv�l�̏�����
	m_bUse = false;														// �g�p��Ԃ̏�����
	m_fGravity = 0.0f;													// �d�͂̏�����
	m_bZBuffer = true;													// Z�o�b�t�@�̎Q�ƗL��
	m_bAlpha = false;													// ���Z�����̗L��������
	m_AnimCount = 0;													// �A�j���[�V�����J�E���^�[�̏�����
	m_AnimPage = 0;														// �A�j���[�V�����y�[�W�̏�����
	m_nInterval = 0;													// �C���^�[�o���̏�����
	m_fRotationSpeed = 0;												// ��]�X�s�[�h�̏�����
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEffect::Init(void)
{
	//�f�o�C�X���擾����
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	//// �摜���
	//m_aEffect[EFFECTTYPE_ROSE].sprite = D3DXVECTOR2(1.0f, 1.0f);		// ������
	//m_aEffect[EFFECTTYPE_ROSE].bAlpha = false;							// �A���t�@�u�����f�B���O�̗L��

	//m_aEffect[EFFECTTYPE_SMOKE].sprite = D3DXVECTOR2(8.0f, 1.0f);		// ������
	//m_aEffect[EFFECTTYPE_SMOKE].bAlpha = false;							// �A���t�@�u�����f�B���O�̗L��

	//m_aEffect[EFFECTTYPE_HALO].sprite = D3DXVECTOR2(1.0f, 1.0f);		// ������
	//m_aEffect[EFFECTTYPE_HALO].bAlpha = false;							// �A���t�@�u�����f�B���O�̗L��

	//m_aEffect[EFFECTTYPE_SANDSMOKE].sprite = D3DXVECTOR2(8.0f, 1.0f);	// ������
	//m_aEffect[EFFECTTYPE_SANDSMOKE].bAlpha = false;						// �A���t�@�u�����f�B���O�̗L��

	//m_aEffect[EFFECTTYPE_SHOCKWAVE].sprite = D3DXVECTOR2(1.0f, 1.0f);	// ������
	//m_aEffect[EFFECTTYPE_SHOCKWAVE].bAlpha = false;						// �A���t�@�u�����f�B���O�̗L��

	//m_aEffect[EFFECTTYPE_STAR].sprite = D3DXVECTOR2(1.0f, 1.0f);		// ������
	//m_aEffect[EFFECTTYPE_STAR].bAlpha = false;							// �A���t�@�u�����f�B���O�̗L��

	//m_aEffect[EFFECTTYPE_SLASH].sprite = D3DXVECTOR2(9.0f, 1.0f);		// ������
	//m_aEffect[EFFECTTYPE_SLASH].bAlpha = false;							// �A���t�@�u�����f�B���O�̗L��

	//m_aEffect[EFFECTTYPE_SPHERE].sprite = D3DXVECTOR2(1.0f, 1.0f);		// ������
	//m_aEffect[EFFECTTYPE_SPHERE].bAlpha = true;							// �A���t�@�u�����f�B���O�̗L��

	//m_aEffect[EFFECTTYPE_WING].sprite = D3DXVECTOR2(1.0f, 1.0f);		// ������
	//m_aEffect[EFFECTTYPE_WING].bAlpha = true;							// �A���t�@�u�����f�B���O�̗L��

	//m_aEffect[EFFECTTYPE_AURA].sprite = D3DXVECTOR2(1.0f, 1.0f);		// ������
	//m_aEffect[EFFECTTYPE_AURA].bAlpha = true;							// �A���t�@�u�����f�B���O�̗L��

	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								// �T�C�Y�̐ݒ�

	// ���_���̍X�V
	MakeVertex();

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CEffect::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@�����݂��Ă����Ƃ�
		m_pVtxBuff->Release();		// �J��
		m_pVtxBuff = NULL;			// NULL����
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CEffect::Update(void)
{
	if (m_bUse)
	{
		float fSize = 1.0f;
		float fRate = (float)m_nLife / (float)m_nMaxLife;

		// �ʒu�X�V
		m_pos += m_move;

		// �T�C�Y��
		switch (m_easingType)
		{
		case EASINGTYPE_IN:
			fSize = CManager::easeIn(fRate, 0, 1, 1);
			break;
		case EASINGTYPE_OUT:
			fSize = CManager::easeOut(fRate);
			break;
		}

		// ���_���̍X�V
		MakeVertex();

		m_size += m_moveSize * fSize;
		m_rot += m_moveRot;
		m_col += m_moveCol;
		SpriteAnimation(m_sprite, m_AnimPage, 0);

		// ����
		m_move.x += (0 - m_move.x) * m_fResistance;
		m_move.y += (0 - m_move.y) * m_fResistance;
		m_move.z += (0 - m_move.z) * m_fResistance;

		if (fabs(m_fDistance) >= 1.0f)
		{
			m_fAngle += m_fRotationSpeed;

			// �ʏ펞�͂������ǂ�
			m_pos.x = m_centerPos.x + sinf(D3DX_PI * m_fAngle) * m_fDistance;
			m_pos.z = m_centerPos.z + cosf(D3DX_PI * m_fAngle) * m_fDistance;
		}

		// �A�j���[�V��������
		if (m_nLife < m_nMaxLife)
		{
			Animation();
		}

		// �J������
		if (m_nLife < m_nMaxLife)
		{
			m_nLife++;					//���C�t�����

			//�d�͏���
			m_move.y += m_fGravity;
		}
		else
		{
			if (m_bFadeOut)
			{
				m_col.a -= 0.05f;

				if (m_col.a < 0.0f)
				{
					m_bUse = false;
				}
			}
			else
			{
				m_bUse = false;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEffect::Draw(void)
{
	if (m_bUse)
	{
		CRenderer *pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice;

		D3DXMATRIX	mtxRot, mtxTrans, mtxView;				//�v�Z�p�}�g���b�N�X

		//�f�o�C�X���擾����
		pDevice = pRenderer->GetDevice();

		if (m_bAlpha)
		{// ���Z�������g�p�����Ƃ�
			// �����_�[�X�e�[�g(���Z��������)
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// ����(�����)���J�����O����
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		if (!m_bZBuffer)
		{
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		if (m_bBillboard)
		{
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			m_mtxWorld._11 = mtxView._11;
			m_mtxWorld._12 = mtxView._21;
			m_mtxWorld._13 = mtxView._31;
			m_mtxWorld._21 = mtxView._12;
			m_mtxWorld._22 = mtxView._22;
			m_mtxWorld._23 = mtxView._32;
			m_mtxWorld._31 = mtxView._13;
			m_mtxWorld._32 = mtxView._23;
			m_mtxWorld._33 = mtxView._33;
		}

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetTexture(0, m_pTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// �����_�[�X�e�[�g(�ʏ�u�����h����)
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		// �����_�[�X�e�[�g(�ʏ�u�����h����)
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		if (m_bAlpha)
		{// ���Z�������g�p�����Ƃ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ����(�����)���J�����O����

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetTexture(0, NULL);
	}
}

//=============================================================================
// �N���G�C�g�֐�
//=============================================================================
void CEffect::Create(void)
{
	// �G�t�F�N�g�̍쐬
	for (int nCount = 0; nCount < MAX_EFFECT; nCount++)
	{
		m_pEffect[nCount] = new CEffect(PRIORITY_EFFECT);

		if (m_pEffect[nCount] != NULL)
		{// �G�t�F�N�g�����݂��Ă����Ƃ�
			m_pEffect[nCount]->Init();					// ����������
			m_pEffect[nCount]->SetUse(false);			// �g�p��Ԃ̕ύX
		}
	}
}

//=============================================================================
// ���f���̃��[�h�֐�
//=============================================================================
HRESULT CEffect::Load(void)
{
	//m_pTexture[EFFECTTYPE_ROSE] = CManager::GetResource("data/tex/effect/rose_01.png");
	//m_pTexture[EFFECTTYPE_SMOKE] = CManager::GetResource("data/tex/effect/explosion002.png");
	//m_pTexture[EFFECTTYPE_SANDSMOKE] = CManager::GetResource("data/tex/effect/explosion001.png");
	//m_pTexture[EFFECTTYPE_HALO] = CManager::GetResource("data/tex/effect/Halo.png");
	//m_pTexture[EFFECTTYPE_SHOCKWAVE] = CManager::GetResource("data/tex/effect/Shockwave.png");
	//m_pTexture[EFFECTTYPE_STAR] = CManager::GetResource("data/tex/effect/Star.png");
	//m_pTexture[EFFECTTYPE_SLASH] = CManager::GetResource("data/tex/effect/Slashing.png");
	//m_pTexture[EFFECTTYPE_SPHERE] = CManager::GetResource("data/tex/effect/sphere.jpg");
	//m_pTexture[EFFECTTYPE_WING] = CManager::GetResource("data/tex/effect/wing.jpg");
	//m_pTexture[EFFECTTYPE_AURA] = CManager::GetResource("data/tex/effect/aura.png");

	return S_OK;
}

//=============================================================================
// �G�t�F�N�g�̈ʒu��ݒ�
//=============================================================================
void CEffect::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// �G�t�F�N�g�̈ړ��ʂ�ݒ�
//=============================================================================
void CEffect::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=============================================================================
// �g�p�����ǂ����ݒ肷��
//=============================================================================
void CEffect::SetUse(bool bValue)
{
	m_bUse = bValue;
}

//=============================================================================
// �e�N�X�`���`��ʒu�̍X�V
//=============================================================================
void CEffect::SpriteAnimation(D3DXVECTOR2 patternNo, int nPatternAnim, int nHeight)
{// �c�Ɖ��̃p�^�[�����ƌ��݂̃y�[�W�ƍs

	VERTEX_3D *pVtx;										// ���_���ւ̃|�C���^

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̍X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f + nPatternAnim * 1.0f / patternNo.x, 0.0f + (1.0f / patternNo.y) * nHeight);
	pVtx[1].tex = D3DXVECTOR2(1.0f / patternNo.x + nPatternAnim * 1.0f / patternNo.x, 0.0f + (1.0f / patternNo.y) * nHeight);
	pVtx[2].tex = D3DXVECTOR2(0.0f + nPatternAnim * 1.0f / patternNo.x, 1.0f / patternNo.y + (1.0f / patternNo.y) * nHeight);
	pVtx[3].tex = D3DXVECTOR2(1.0f / patternNo.x + nPatternAnim * 1.0f / patternNo.x, 1.0f / patternNo.y + (1.0f / patternNo.y) * nHeight);

	// ���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �G�t�F�N�g�̎g�p
//=============================================================================
void CEffect::SetEffect(const std::string Add, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move,
	D3DXVECTOR3 moveSize, D3DXVECTOR3 moveRot, D3DXCOLOR moveCol, EASINGTYPE easingType, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife,
	float fGravity, float fResistance, bool bBillboard, int nPatternAnim, int nHeight,
	D3DXVECTOR3 centerPos, float fAngle, float fDistance, float fRotationSpeed, D3DXVECTOR2 sprite, bool bAlpha, bool bZBuffer, bool bFadeOut)
{
	for (int nCount = 0; nCount < MAX_EFFECT; nCount++)
	{
		if (!m_pEffect[nCount]->m_bUse)
		{
			m_pEffect[nCount]->m_pTexture = CManager::GetResource(Add);				// �e�N�X�`�����̏�����
			m_pEffect[nCount]->m_pos = pos;											// �ʒu���
			m_pEffect[nCount]->m_rot = rot;											// ��]���
			m_pEffect[nCount]->m_move = move;										// �ړ����
			m_pEffect[nCount]->m_col = col;											// �J���[
			m_pEffect[nCount]->m_nMaxLife = nLife;									// �������Ԃ̍ő吔
			m_pEffect[nCount]->m_nLife = 0;											// ��������
			m_pEffect[nCount]->m_fGravity = fGravity;								// �d�͂̎g�p
			m_pEffect[nCount]->m_AnimPage = nPatternAnim;							// �y�[�W��
			m_pEffect[nCount]->m_size = size;										// �傫��
			m_pEffect[nCount]->MakeVertex();										// ���_���̍쐬
			m_pEffect[nCount]->SpriteAnimation(sprite, nPatternAnim, nHeight);		// �`��̈�̐ݒ�
			m_pEffect[nCount]->m_fAngle = fAngle;									// �p�x
			m_pEffect[nCount]->m_fDistance = fDistance;								// ����
			m_pEffect[nCount]->m_centerPos = centerPos;								// ���S�ʒu�̐ݒ�
			m_pEffect[nCount]->m_fResistance = fResistance;							// ��R
			m_pEffect[nCount]->m_fRotationSpeed = fRotationSpeed;					// ��]���x
			m_pEffect[nCount]->m_moveSize = moveSize;								// �T�C�Y�̉��Z�l
			m_pEffect[nCount]->m_moveRot = moveRot;									// ��]�̕ω���
			m_pEffect[nCount]->m_moveCol = moveCol;									// �F�̕ω���
			m_pEffect[nCount]->m_bBillboard = bBillboard;							// �r���{�[�h�̗L��
			m_pEffect[nCount]->m_easingType = easingType;							// �C�[�W���O�̐ݒ�
			m_pEffect[nCount]->m_sprite = sprite;									// �摜�̕�����
			m_pEffect[nCount]->m_bZBuffer = bZBuffer;								// Z�o�b�t�@�̎Q��
			m_pEffect[nCount]->m_bAlpha = bAlpha;									// ���Z����
			m_pEffect[nCount]->m_bFadeOut = bFadeOut;								// �d�̗͂L��
			m_pEffect[nCount]->m_bUse = true;										// �g�p���ɐݒ�
			break;
		}
	}
}

//=============================================================================
// �����U��Ԃт�̃G�t�F�N�g
//=============================================================================
void CEffect::FallingPetals(bool bGravity, D3DXVECTOR3 pos, int nLife)
{
	float fAngle = 0.0f;
	D3DXVECTOR3 particlePos = D3DXVECTOR3_ZERO;

	// �ʒu�̌v�Z
	particlePos = GetRandomPosWithSphere();
	// �p�x
	fAngle = GetRandomAngle();

	D3DXVECTOR3 rot;
	rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);			// ��]�ʂ̐ݒ�

	CEffect::SetEffect("data/tex/effect/rose_01.png",			// �G�t�F�N�g�^�C�v
		pos,													// �����ʒu
		D3DXVECTOR3(10.0f, 10.0f, 0.0f),						// �T�C�Y
		particlePos * 5.0f,										// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y��1�t���[��������̉��Z�l
		D3DXVECTOR3(0.02f, 0.02f, 0.0f),						// ��]�̕ω���
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),						// �F�̕ω���
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		rot,													// �e�N�X�`���̉�]
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
		nLife,													// ���C�t
		bGravity,												// �d��
		0.025f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0,														// ��]���x
		D3DXVECTOR2(1.0f, 1.0f),								// �摜�̕�����
		false,													// ���Z�����̗L��
		false,													// Z�o�b�t�@�̔�r�L��
		false													// �t�F�[�h
	);
}

//=============================================================================
// �W�܂�Ԃт�̃G�t�F�N�g
//=============================================================================
void CEffect::PetalsGather(D3DXVECTOR3 pos)
{
	float fAngle = 0.0f;

	D3DXVECTOR3 particlePos;

	// �ʒu�̌v�Z
	particlePos = GetRandomPosWithSphere();

	// �p�x
	fAngle = GetRandomAngle();		// �����_���Ɏ擾

	D3DXVECTOR3 rot;
	rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);		// ��]�ʂ̐ݒ�

	CEffect::SetEffect("data/tex/effect/rose_01.png",							// �G�t�F�N�g�^�C�v
		pos + particlePos * 200,								// �����ʒu
		D3DXVECTOR3(10.0f, 10.0f, 0.0f),						// �T�C�Y
		particlePos * -7,										// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y��1�t���[��������̉��Z�l
		D3DXVECTOR3(0.02f, 0.02f, 0.0f),						// ��]�̕ω���
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),						// �F�̕ω���
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		rot,													// �e�N�X�`���̉�]
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
		30,														// ���C�t
		false,													// �d��
		0.025f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0,														// ��]���x
		D3DXVECTOR2(1.0f, 1.0f),								// �摜�̕�����
		false,													// ���Z�����̗L��
		false,													// Z�o�b�t�@�̔�r�L��
		false													// �t�F�[�h
	);
}

//=============================================================================
// �Ԃт炪�����オ��G�t�F�N�g
//=============================================================================
void CEffect::PetalsSoaring(D3DXVECTOR3 pos)
{
	float fAngle = 0.0f;
	D3DXVECTOR3 particlePos;

	// �p�x
	fAngle = GetRandomAngle();	// �����_���Ɏ擾

	// ��]
	D3DXVECTOR3 rot;
	rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);		// ��]�ʂ̐ݒ�

	// ����
	float fDistance = float(CManager::GetRand(200 * 100)) / 100;		// �����_���Ɏ擾
	fDistance = 50.0f + fDistance;										// 50���Z����

	// �ʒu
	particlePos = pos;
	particlePos.x += sinf(D3DX_PI * fAngle) * fDistance;
	particlePos.z += cosf(D3DX_PI * fAngle) * fDistance;

	CEffect::SetEffect("data/tex/effect/rose_01.png",							// �G�t�F�N�g�^�C�v
		particlePos,											// �����ʒu
		D3DXVECTOR3(10.0f, 10.0f, 0.0f),						// �T�C�Y
		D3DXVECTOR3(0.0f, 5.0f, 0.0f),							// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y�̃t���[��������̉��Z�l
		D3DXVECTOR3(0.02f, 0.02f, 0.0f),						// ��]�̕ω���
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),						// �F�̕ω���
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		rot,													// �e�N�X�`���̉�]
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
		250,													// ���C�t
		false,													// �d��
		0.015f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		pos,													// ���S�ʒu
		fAngle,													// �p�x
		fDistance,												// ����
		0.01f,													// ��]���x
		D3DXVECTOR2(1.0f, 1.0f),								// �摜�̕�����
		false,													// ���Z�����̗L��
		false,													// Z�o�b�t�@�̔�r�L��
		false													// �t�F�[�h
	);
}

//=============================================================================
// �H�������G�t�F�N�g
//=============================================================================
void CEffect::WingStay(D3DXVECTOR3 pos)
{
	float fAngle = float(CManager::GetRand(314)) - float(CManager::GetRand(314));
	float fAngle_x = float(CManager::GetRand(314)) - float(CManager::GetRand(314));

	D3DXVECTOR3 particlePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �ʒu�̌v�Z
	particlePos.x = cosf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);
	particlePos.y = sinf(D3DX_PI + fAngle_x);
	particlePos.z = sinf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);

	// �p�x
	fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;		// �����_���Ɏ擾

	// ��]
	D3DXVECTOR3 rot;
	rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);		// �����_���Ɏ擾

	// ����
	float fDistance = float(CManager::GetRand((int)350 * 100)) / 100;	// �����_���Ɏ擾
	fDistance = 50.0f + fDistance;										// 50���Z����

	// �X�s�[�h
	float fSpeed = float(CManager::GetRand((int)350 * 100)) / 100;		// �����_���Ɏ擾
	fSpeed = 50 + fSpeed;												// 50���Z����

	CEffect::SetEffect("data/tex/effect/wing.jpg",							// �G�t�F�N�g�^�C�v
		pos + particlePos * fDistance,							// �����ʒu
		D3DXVECTOR3(5.0f, 5.0f, 0.0f),							// �T�C�Y
		particlePos,											// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y�̃t���[��������̉��Z�l
		D3DXVECTOR3(0.02f, 0.02f, 0.0f),						// ��]�̕ω���
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),						// �F�̕ω���
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		rot,													// �e�N�X�`���̉�]
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
		250,													// ���C�t
		false,													// �d��
		0.025f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0,														// ��]���x
		D3DXVECTOR2(1.0f, 1.0f),								// �摜�̕�����
		true,													// ���Z�����̗L��
		true,													// Z�o�b�t�@�̔�r�L��
		false													// �t�F�[�h
	);
}

//=============================================================================
// �����̃G�t�F�N�g
//=============================================================================
void CEffect::SandSmokeEffect(D3DXVECTOR3 pos)
{
	for (int nCount = 0; nCount < 15; nCount++)
	{
		float fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;
		float fRadius = float(CManager::GetRand(3 * 100)) / 100.0f - float(CManager::GetRand(3 * 100)) / 100.0f;

		D3DXVECTOR3 particlePos;

		// �ʒu�̌v�Z
		particlePos.x = sinf(D3DX_PI * fAngle) * (3.0f);
		particlePos.y = 1.0f;
		particlePos.z = cosf(D3DX_PI * fAngle) * (3.0f);

		D3DXMATRIX mtxMeshRot, mtxMeshTrans;					// �v�Z�p�}�g���b�N�X
		D3DXMATRIX mtx;											// ����̃}�g���b�N�X

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtx);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, particlePos.x, particlePos.y, particlePos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		// �T�C�Y
		float size = CManager::GetRand(100) / 10.0f;			// �����_���Ɏ擾

		CEffect::SetEffect("data/tex/effect/explosion001.png",				// �G�t�F�N�g�^�C�v
			pos,												// �����ʒu
			D3DXVECTOR3(size, size, 0.0f),						// �T�C�Y
			D3DXVECTOR3(mtx._41, mtx._42, mtx._43),				// �ړ�����
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),					// �T�C�Y�̃t���[��������̉��Z�l
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// ��]�̕ω���
			D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.02f),				// �F�̕ω���
			EASINGTYPE_NONE,									// �C�[�W���O�^�C�v
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// �e�N�X�`���̉�]
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f),					// �J���[
			100,												// ���C�t
			false,												// �d��
			0.025f,												// ��R
			true,												// �r���{�[�h�̎g�p
			0,													// �\������ӏ�(��)
			0,													// �\������ӏ�(�c)
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// ���S�ʒu
			0,													// �p�x
			0,													// ����
			0,													// ��]���x
			D3DXVECTOR2(8.0f, 1.0f),							// �摜�̕�����
			false,												// ���Z�����̗L��
			true,												// Z�o�b�t�@�̔�r�L��
			false												// �t�F�[�h
		);
	}
}

//=============================================================================
// ��������
//=============================================================================
void CEffect::SandSmokeExplosion(D3DXVECTOR3 pos)
{
	for (int nCount = 0; nCount < 100; nCount++)
	{
		float fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;
		float fRadius = float(CManager::GetRand(4 * 100)) / 100 - float(CManager::GetRand(4 * 100)) / 100;

		D3DXVECTOR3 particlePos;

		D3DXVECTOR3 vecPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXMATRIX	mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
		D3DXMATRIX mtx;			// ����̃}�g���b�N�X
		D3DXMATRIX mtxPlayer;

		// �X�s�[�h
		float fSpeed = float(CManager::GetRand(15 * 100)) / 100;		// �����_���Ɏ擾
		particlePos.y = 1 + fSpeed;
		particlePos.x = sinf(D3DX_PI * fAngle) * (fSpeed);
		particlePos.z = cosf(D3DX_PI * fAngle) * (fSpeed);

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
		D3DXMatrixTranslation(&mtxMeshTrans, particlePos.x, particlePos.y, particlePos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		// �}�g���b�N�X�̍���
		D3DXMatrixMultiply(&mtx, &mtx, &mtxPlayer);

		D3DXVECTOR3 circlePos = pos;

		// �ʒu�v�Z
		circlePos.x += sinf(D3DX_PI * fAngle) * 30;
		circlePos.z += cosf(D3DX_PI * fAngle) * 30;

		CEffect::SetEffect("data/tex/effect/explosion001.png",				// �G�t�F�N�g�^�C�v
			circlePos,											// �����ʒu
			D3DXVECTOR3(15.0f, 15.0f, 0.0f),					// �T�C�Y
			D3DXVECTOR3(mtx._41, mtx._42, mtx._43),				// �ړ�����
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// �T�C�Y�̃t���[��������̉��Z�l
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// ��]�̕ω���
			D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.02f),				// �F�̕ω���
			EASINGTYPE_IN,										// �C�[�W���O�^�C�v
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// �e�N�X�`���̉�]
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f),					// �J���[
			15,													// ���C�t
			false,												// �d��
			0.025f,												// ��R
			true,												// �r���{�[�h
			0,													// �\������ӏ�(��)
			0,													// �\������ӏ�(�c)
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// ���S�ʒu
			0,													// �p�x
			0,													// ����
			0,													// ��]���x
			D3DXVECTOR2(8.0f, 1.0f),							// �摜�̕�����
			false,												// ���Z�����̗L��
			true,												// Z�o�b�t�@�̔�r�L��
			false												// �t�F�[�h
		);
	}
}

//=============================================================================
// �V���b�N�E�E�F�[�u
//=============================================================================
void CEffect::Shockwave(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != NULL)
	{// �J���������݂��Ă����Ƃ�
		CEffect::SetEffect("data/tex/effect/Shockwave.png",				// �G�t�F�N�g�^�C�v
			pos,												// �����ʒu
			D3DXVECTOR3(15.0f, 15.0f, 0.0f),					// �T�C�Y
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// �ړ�����
			D3DXVECTOR3(10.0f, 10.0f, 0.0f),					// �T�C�Y�̃t���[��������̉��Z�l
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// ��]�̕ω���
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),					// �F�̕ω���
			EASINGTYPE_NONE,									// �C�[�W���O�^�C�v
			D3DXVECTOR3(0.0f, rot.y - pCamera->GetRotation().y, 0.0f),	// �e�N�X�`���̉�]
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f),					// �J���[
			nLife,												// ���C�t
			false,												// �d��
			0.025f,												// ��R
			false,												// �r���{�[�h
			0,													// �\������ӏ�(��)
			0,													// �\������ӏ�(�c)
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// ���S�ʒu
			0,													// �p�x
			0,													// ����
			0,													// ��]���x
			D3DXVECTOR2(1.0f, 1.0f),							// �摜�̕�����
			false,												// ���Z������
			true,												// Z�o�b�t�@�̔�r�L��
			false												// �t�F�[�h
		);
	}
}

//=============================================================================
// �n���[����
//=============================================================================
void CEffect::Halo(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	float fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;

	D3DXVECTOR3 rot;

	// ��]
	rot = D3DXVECTOR3(0.0f, 0.0f, cosf(fAngle) * 10);			// �����_���Ɏ擾

	CEffect::SetEffect("data/tex/effect/Halo.png",				// �G�t�F�N�g�^�C�v
		pos,													// �����ʒu
		D3DXVECTOR3(2.0f * size.x, 2.0f * size.y, 0.0f),		// �T�C�Y
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �ړ�����
		D3DXVECTOR3(-2.0f, -2.0f, 0.0f),						// �T�C�Y�̃t���[��������̉��Z�l
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ��]�̕ω���
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),						// �F�̕ω���
		EASINGTYPE_IN,											// �C�[�W���O�^�C�v
		rot,													// �e�N�X�`���̉�]
		D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.8f),						// �J���[
		15,														// ���C�t
		false,													// �d��
		0.025f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0,														// ��]���x
		D3DXVECTOR2(1.0f, 1.0f),								// �摜�̕�����
		false,													// ���Z�����̗L��
		true,													// Z�o�b�t�@�̔�r�L��
		false													// �t�F�[�h
	);
}

//=============================================================================
// �򉻕��U
//=============================================================================
void CEffect::Purification(D3DXVECTOR3 pos)
{
	float fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;

	D3DXVECTOR3 particlePos;

	D3DXMATRIX mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtx;										// ����̃}�g���b�N�X

	// ���x
	float fSpeed = float(CManager::GetRand(3 * 100)) / 100;
	particlePos.y = 1.0f + fSpeed;

	// �ʒu�̌v�Z
	particlePos.x = sinf(D3DX_PI * fAngle) * (fSpeed);
	particlePos.z = cosf(D3DX_PI * fAngle) * (fSpeed);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtx);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxMeshTrans, particlePos.x, particlePos.y, particlePos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

	D3DXVECTOR3 point = pos;

	// �����ʒu�̌v�Z
	point.x += sinf(D3DX_PI * fAngle) * 80;
	point.z += cosf(D3DX_PI * fAngle) * 80;

	CEffect::SetEffect("data/tex/effect/explosion002.png",						// �G�t�F�N�g�^�C�v
		point,													// �����ʒu
		D3DXVECTOR3(20.0f, 20.0f, 0.0f),						// �T�C�Y
		D3DXVECTOR3(0.0f, mtx._42, 0.0f),						// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y�̃t���[��������̉��Z�l
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ��]�̕ω���
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),						// �F�̕ω���
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �e�N�X�`���̉�]
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),						// �J���[
		50,														// ���C�t
		false,													// �d��
		0.025f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0,														// ��]���x
		D3DXVECTOR2(8.0f, 1.0f),								// �摜�̕�����
		false,													// ���Z�����̗L��
		true,													// Z�o�b�t�@�̔�r�L��
		false													// �t�F�[�h
	);
}

//=============================================================================
// �X�^�[
//=============================================================================
void CEffect::Star(D3DXVECTOR3 pos)
{
	CEffect::SetEffect("data/tex/effect/Star.png",							// �G�t�F�N�g�^�C�v
		pos,													// �����ʒu
		D3DXVECTOR3(50.0f, 50.0f, 0.0f),						// �T�C�Y
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �ړ�����
		D3DXVECTOR3(50.0f / 15, 50.0f / 15, 0.0f),				// �T�C�Y�̃t���[��������̉��Z�l
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ��]�̕ω���
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),						// �F�̕ω���
		EASINGTYPE_OUT,											// �C�[�W���O�^�C�v
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �e�N�X�`���̉�]
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
		15,														// ���C�t
		false,													// �d��
		0.025f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0,														// ��]���x
		D3DXVECTOR2(1.0f, 1.0f),								// �摜�̕�����
		false,													// ���Z�����̗L��
		true,													// Z�o�b�t�@�̔�r�L��
		true													// �t�F�[�h
	);
}

//=============================================================================
// �a��
//=============================================================================
void CEffect::Slashing(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	D3DXMATRIX mtxMeshRot, mtxMeshTrans;				//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtx;										// ����̃}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtx);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

	CEffect::SetEffect("data/tex/effect/Slashing.png",						// �G�t�F�N�g�^�C�v
		pos,													// �����ʒu
		D3DXVECTOR3(50.0f, 150.0f, 0.0f),						// �T�C�Y
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y�̃t���[��������̉��Z�l
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ��]�̕ω���
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),						// �F�̕ω���
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		rot,													// �e�N�X�`���̉�]
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
		15,														// ���C�t
		false,													// �d��
		0.025f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0,														// ��]���x
		D3DXVECTOR2(9.0f, 1.0f),								// �摜�̕�����
		false,													// ���Z�����̗L��
		true,													// Z�o�b�t�@�̔�r�L��
		false													// �t�F�[�h
	);
}

//=============================================================================
// �I�[��
//=============================================================================
void CEffect::Aura(int nCount, bool bJump, D3DXVECTOR3 pos)
{
	float fAngle = float(CManager::GetRand(314)) - float(CManager::GetRand(314));
	float fAngle_x = float(CManager::GetRand(314)) - float(CManager::GetRand(314));

	D3DXVECTOR3 pointPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �p�x
	fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;		// �����_���Ɏ擾

	// ��]
	D3DXVECTOR3 rot;
	rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);			// �����_���Ɏ擾

	// ����
	float fDistance = float(CManager::GetRand(200 * 100)) / 100;			// �����_���Ɏ擾
	fDistance = 50.0f + fDistance;

	pointPos = pos;

	// �ʒu�̌v�Z
	pointPos.x += sinf(D3DX_PI * fAngle) * fDistance;
	pointPos.z += cosf(D3DX_PI * fAngle) * fDistance;

	CEffect::SetEffect("data/tex/effect/sphere.jpg",						// �G�t�F�N�g�^�C�v
		pointPos,												// �����ʒu
		D3DXVECTOR3(5.0f, 5.0f, 0.0f),							// �T�C�Y
		D3DXVECTOR3(0.0f, 5.0f, 0.0f),							// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y�̃t���[��������̉��Z�l
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ��]�̕ω���
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),						// �F�̕ω���
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		rot,													// �e�N�X�`���̉�]
		D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f),						// �J���[
		100,													// ���C�t
		false,													// �d��
		0.005f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		pos,													// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0,														// ��]���x
		D3DXVECTOR2(1.0f, 1.0f),								// �摜�̕�����
		true,													// ���Z�����̗L��
		true,													// Z�o�b�t�@�̔�r�L��
		false													// �t�F�[�h
	);

	// �ʒu�̃��Z�b�g
	pointPos = pos;

	// ��]
	rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);		// �����_���Ɏ擾

	// ����
	fDistance = float(CManager::GetRand(50 * 100)) / 100;				// �����_���Ɏ擾

	// �ʒu�̌v�Z
	pointPos.x += sinf(D3DX_PI * fAngle) * fDistance;
	pointPos.z += cosf(D3DX_PI * fAngle) * fDistance;

	CEffect::SetEffect("data/tex/effect/aura.png",							// �G�t�F�N�g�^�C�v
		pointPos,												// �����ʒu
		D3DXVECTOR3(50.0f, 50.0f, 0.0f),						// �T�C�Y
		D3DXVECTOR3(0.0f, 5.0f, 0.0f),							// �ړ�����
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y�̃t���[��������̉��Z�l
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ��]�̕ω���
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),						// �F�̕ω���
		EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
		rot,													// �e�N�X�`���̉�]
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f),						// �J���[
		5 + CManager::GetRand(50),								// ���C�t
		false,													// �d��
		0.005f,													// ��R
		true,													// �r���{�[�h
		0,														// �\������ӏ�(��)
		0,														// �\������ӏ�(�c)
		pos,													// ���S�ʒu
		0,														// �p�x
		0,														// ����
		0,														// ��]���x
		D3DXVECTOR2(1.0f, 1.0f),								// �摜�̕�����
		true,													// ���Z�����̗L��
		true,													// Z�o�b�t�@�̔�r�L��
		false													// �t�F�[�h
	);

	if (!bJump)
	{// �W�����v���Ă��Ȃ������Ƃ�
		if (nCount % 20 == 0)
		{// ���܂肪0�������Ƃ�
			CEffect::SetEffect("data/tex/effect/sphere.jpg",	// �G�t�F�N�g�^�C�v
				pos,											// �����ʒu
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),					// �T�C�Y
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),					// �ړ�����
				D3DXVECTOR3(5.0f, 5.0f, 0.0f),					// �T�C�Y�̃t���[��������̉��Z�l
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ��]�̕ω���
				D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),						// �F�̕ω���
				EASINGTYPE_OUT,									// �C�[�W���O�^�C�v
				D3DXVECTOR3(1.57f, 0.0f, 0.0f),					// �e�N�X�`���̉�]
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f),				// �J���[
				50,												// ���C�t
				false,											// �d��
				0.025f,											// ��R
				false,											// �r���{�[�h
				0,												// �\������ӏ�(��)
				0,												// �\������ӏ�(�c)
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),					// ���S�ʒu
				0,												// �p�x
				0,												// ����
				0,												// ��]���x
				D3DXVECTOR2(1.0f, 1.0f),						// �摜�̕�����
				true,											// ���Z�����̗L��
				true,											// Z�o�b�t�@�̔�r�L��
				false											// �t�F�[�h
			);
		}

		if (nCount % 15 == 0)
		{// ���܂肪0�������Ƃ�
			CEffect::SetEffect("data/tex/effect/Shockwave.png",			// �G�t�F�N�g�^�C�v
				pos,											// �����ʒu
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),					// �T�C�Y
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),					// �ړ�����
				D3DXVECTOR3(4.0f, 4.0f, 0.0f),					// �T�C�Y�̃t���[��������̉��Z�l
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),					// ��]�̕ω���
				D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),				// �F�̕ω���
				EASINGTYPE_OUT,									// �C�[�W���O�^�C�v
				D3DXVECTOR3(1.57f, 0.0f, 0.0f),					// �e�N�X�`���̉�]
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f),				// �J���[
				50,												// ���C�t
				false,											// �d��
				0.025f,											// ��R
				false,											// �r���{�[�h
				0,												// �\������ӏ�(��)
				0,												// �\������ӏ�(�c)
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),					// ���S�ʒu
				0,												// �p�x
				0,												// ����
				0,												// ��]���x
				D3DXVECTOR2(1.0f, 1.0f),						// ������
				false,													// ���Z�����̗L��
				true,											// Z�o�b�t�@�̔�r�L��
				false											// �t�F�[�h
			);
		}
	}
}

//=============================================================================
// �Ԃт�̏W����
//=============================================================================
void CEffect::PetalCluster(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCount = 0; nCount < 150; nCount++)
	{
		float fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;
		float fRadius = float(CManager::GetRand(10 * 100)) / 100.0f - float(CManager::GetRand(10 * 100)) / 100.0f;

		D3DXVECTOR3 petalPos;

		// �ʒu�̌v�Z
		petalPos.x = sinf(D3DX_PI * fAngle) * (fRadius);
		petalPos.y = 15.0f;
		petalPos.z = cosf(D3DX_PI * fAngle) * (fRadius);

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
		D3DXMatrixTranslation(&mtxMeshTrans, petalPos.x, petalPos.y, petalPos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		D3DXMatrixMultiply(&mtx, &mtx, &mtxPlayer);

		D3DXVECTOR3 petalRot;
		petalRot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);

		CEffect::SetEffect("data/tex/effect/rose_01.png",							// �G�t�F�N�g�^�C�v
			pos,	// �����ʒu										// �����ʒu
			D3DXVECTOR3(10.0f, 10.0f, 0.0f),						// �T�C�Y
			D3DXVECTOR3(mtx._41, mtx._42, mtx._43),					// �ړ�����
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// �T�C�Y�̃t���[��������̉��Z�l
			D3DXVECTOR3(0.02f, 0.02f, 0.0f),						// ��]�̕ω���
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),						// �F�̕ω���
			EASINGTYPE_NONE,										// �C�[�W���O�^�C�v
			petalRot,												// �e�N�X�`���̉�]
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
			50,														// ���C�t
			false,													// �d��
			0,														// ��R
			true,													// �r���{�[�h
			0,														// �\������ӏ�(��)
			0,														// �\������ӏ�(�c)
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// ���S�ʒu
			0,														// �p�x
			0,														// ����
			0,														// ��]���x
			D3DXVECTOR2(1.0f, 1.0f),								// ������
			false,													// ���Z�����̗L��
			false,													// Z�o�b�t�@�̔�r�L��
			false													// �t�F�[�h�̗L��
		);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void CEffect::MakeVertex(void)
{
	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f);

	//�@���x�N�g��
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//�e�N�X�`���`�ʂ̈ʒu
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �A�j���[�V��������
//=============================================================================
void CEffect::Animation(void)
{
	// �A�j���[�V����
	if (m_AnimCount >= m_nMaxLife / m_sprite.x)
	{
		m_AnimPage++;		// �y�[�W��1���Z����

		SpriteAnimation(m_sprite, m_AnimPage, 0);		// �`��͈�
		m_AnimCount = 0;		// ������
	}

	m_AnimCount++;				// �J�E���^��1�v���X����
}

//=============================================================================
// �ʒu�̃����_������
//=============================================================================
D3DXVECTOR3 CEffect::GetRandomPosWithSphere(void)
{
	// �ʒu�̃����_���擾
	float fAngle = float(CManager::GetRand(314)) - float(CManager::GetRand(314));
	float fAngle_x = float(CManager::GetRand(314)) - float(CManager::GetRand(314));

	D3DXVECTOR3 pos;

	// �ʒu�̌v�Z
	pos.x = cosf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);
	pos.y = sinf(D3DX_PI + fAngle_x);
	pos.z = sinf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);

	// �v�Z���ʂ�Ԃ�
	return pos;
}


//=============================================================================
// �ʒu�̃����_������
//=============================================================================
D3DXVECTOR3 CEffect::GetRandomPosWithCone(float &fRadius)
{
	// �ʒu�̃����_���擾
	float fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;
	fRadius = float(CManager::GetRand(10 * 100)) / 100.0f - float(CManager::GetRand(10 * 100)) / 100.0f;

	D3DXVECTOR3 pos;

	// �ʒu�̌v�Z
	pos.x = sinf(D3DX_PI * fAngle) * (fRadius);
	pos.y = 1.0f;

	fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;
	pos.z = cosf(D3DX_PI * fAngle) * (fRadius);

	return pos;
}

//=============================================================================
// �p�x�̃����_������
//=============================================================================
float CEffect::GetRandomAngle(void)
{
	// �p�x�̃����_���擾
	return float(CManager::GetRand(314)) / 100.0f -
		float(CManager::GetRand(314)) / 100.0f;		// �����_���Ɏ擾
}

//=============================================================================
// �G�t�F�N�g���̓ǂݍ���
//=============================================================================
void CEffect::LoadScript(std::string Add)
{
	std::map<std::string, EFFECT>::const_iterator it = m_effectMap.find(Add);

	if (it == m_effectMap.end())
	{// ������Ȃ������Ƃ�
	 // ����������������L�[�Ƃ���
	 // �V�����}�b�v�ɃG�t�F�N�g�f�[�^��ǉ�����
		EFFECT effect;
		effect.bAlpha = false;
		effect.bBillboard = false;
		effect.bFadeOut = false;
		effect.bZBuffer = false;

		FILE *pFile;
		char cReadText[128];		//����
		char cHeadText[128];		//��r
		char cDie[128];
		int nCntPointer = 0;			//�|�C���^�[�̐��l

		char aTag[64];					//���f���̃A�h���X
		char aAdd[64];					//���f���̃A�h���X

		//�e�L�X�g�f�[�^���[�h
		pFile = fopen(Add.c_str(), "r");

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

					if (strcmp(cHeadText, "EFFECTSET") == 0)
					{//�L�����N�^�[�̏������̂Ƃ�
							//�G���h�L�����N�^�[�Z�b�g������܂Ń��[�v
						while (strcmp(cHeadText, "END_EFFECTSET") != 0)
						{
							fgets(cReadText, sizeof(cReadText), pFile);
							sscanf(cReadText, "%s", &cHeadText);

							//���s
							if (strcmp(cReadText, "\n") != 0)
							{
								if (strcmp(cHeadText, "EFFECT_FILENAME") == 0)
								{// �e�N�X�`���A�h���X���
									sscanf(cReadText, "%s %s %s", &cDie, &cDie, &aAdd[0]);						//�A�h���X�̎擾
									effect.texAdd = aAdd;
								}
								else if (strcmp(cHeadText, "TAG") == 0)
								{// �^�O�̏��
									sscanf(cReadText, "%s %s %s", &cDie, &cDie, &aTag[0]);						//�A�h���X�̎擾
								}
								else if (strcmp(cHeadText, "BILLBOARD") == 0)
								{// �r���{�[�h�̗L��
									effect.bBillboard = true;
								}
								else if (strcmp(cHeadText, "ALPHABLENDING") == 0)
								{// �A���t�@�u�����f�B���O�̗L��
									effect.bAlpha = true;
								}
								else if (strcmp(cHeadText, "ZBUFFER") == 0)
								{// Z�o�b�t�@�̗L��
									effect.bZBuffer = true;
								}
								else if (strcmp(cHeadText, "FADE_OUT") == 0)
								{// �t�F�[�h�A�E�g�̗L��
									effect.bFadeOut = true;
								}
								else if (strcmp(cHeadText, "SIZE") == 0)
								{// �T�C�Y
									D3DXVECTOR3 size;
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&size.x,
										&size.y,
										&size.z);

									// �T�C�Y����ݒ肷��
									effect.size = size;
								}
								else if (strcmp(cHeadText, "COL") == 0)
								{// �F
									D3DXCOLOR col;
									sscanf(cReadText, "%s %s %f %f %f %f", &cDie, &cDie,
										&col.r,
										&col.g,
										&col.b,
										&col.a);

									// �F����ݒ肷��
									effect.col = col;
								}
								else if (strcmp(cHeadText, "MOVE_SIZE") == 0)
								{// �T�C�Y�̕ω���
									D3DXVECTOR3 moveSize;
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&moveSize.x,
										&moveSize.y,
										&moveSize.z);

									// �T�C�Y�̕ω��ʏ���ݒ肷��
									effect.moveSize = moveSize;
								}
								else if (strcmp(cHeadText, "MOVE_ROT") == 0)
								{// ��]�̕ω���
									D3DXVECTOR3 moveRot;
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&moveRot.x,
										&moveRot.y,
										&moveRot.z);

									// ��]�̕ω��ʏ���ݒ肷��
									effect.moveRot = moveRot;
								}
								else if (strcmp(cHeadText, "MOVE_COL") == 0)
								{// �F�̕ω���
									D3DXCOLOR moveCol;
									sscanf(cReadText, "%s %s %f %f %f %f", &cDie, &cDie,
										&moveCol.r,
										&moveCol.g,
										&moveCol.b,
										&moveCol.a);

									// �F�̕ω��ʏ���ݒ肷��
									effect.moveCol = moveCol;
								}
								else if (strcmp(cHeadText, "LIFE") == 0)
								{//���C�t
									int nLife;
									sscanf(cReadText, "%s %s %d", &cDie, &cDie,
										&nLife);

									// �T�C�Y����ݒ肷��
									effect.nLife = nLife;
								}
								else if (strcmp(cHeadText, "GRAVITY") == 0)
								{// �d�͉����x
									float fGravity;
									sscanf(cReadText, "%s %s %f", &cDie, &cDie,
										&fGravity);

									// �T�C�Y����ݒ肷��
									effect.fGravity = fGravity;
								}
								else if (strcmp(cHeadText, "RESISTANCE") == 0)
								{// ��R�l
									float fResistance;
									sscanf(cReadText, "%s %s %f", &cDie, &cDie,
										&fResistance);

									// �T�C�Y����ݒ肷��
									effect.fResistance = fResistance;
								}
								else if (strcmp(cHeadText, "SPEED") == 0)
								{// ��R�l
									float fMaxSpeed;
									sscanf(cReadText, "%s %s %f", &cDie, &cDie,
										&fMaxSpeed);

									// �T�C�Y����ݒ肷��
									effect.fMaxSpeed = fMaxSpeed;
								}
								else if (strcmp(cHeadText, "CENTER_POS") == 0)
								{// ��]�̕ω���
									D3DXVECTOR3 centerPos;
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&centerPos.x,
										&centerPos.y,
										&centerPos.z);

									// ��]�̕ω��ʏ���ݒ肷��
									effect.centerPos = centerPos;
								}
								else if (strcmp(cHeadText, "ANGLE") == 0)
								{// �d�͉����x
									float fAngle;
									sscanf(cReadText, "%s %s %f", &cDie, &cDie,
										&fAngle);

									// �T�C�Y����ݒ肷��
									effect.fAngle = fAngle;
								}
								else if (strcmp(cHeadText, "DISTANCE") == 0)
								{// �d�͉����x
									float fDistance;
									sscanf(cReadText, "%s %s %f", &cDie, &cDie,
										&fDistance);

									// �T�C�Y����ݒ肷��
									effect.fDistance = fDistance;
								}
								else if (strcmp(cHeadText, "ROTATION_SPEED") == 0)
								{// �d�͉����x
									float fRotationSpeed;
									sscanf(cReadText, "%s %s %f", &cDie, &cDie,
										&fRotationSpeed);

									// �T�C�Y����ݒ肷��
									effect.fSpeedRot = fRotationSpeed;
								}
								else if (strcmp(cHeadText, "SPRITE") == 0)
								{// �T�C�Y
									D3DXVECTOR2 sprite;
									sscanf(cReadText, "%s %s %f %f", &cDie, &cDie,
										&sprite.x,
										&sprite.y);

									// �T�C�Y����ݒ肷��
									effect.sprite = sprite;
								}
							}
						}
					}
				}
			}

			// �}�b�v�֑}������
			m_effectMap.insert(std::map<std::string, EFFECT>::value_type(aTag, effect));

			//�t�@�C����
			fclose(pFile);
		}
		else
		{

		}

		return;				// ���ʂ�Ԃ�
	}
}

//=============================================================================
// �G�t�F�N�g�f�[�^�̎擾
//=============================================================================
EFFECT CEffect::GetEffectData(std::string Tag)
{
	std::map<std::string, EFFECT>::const_iterator it = m_effectMap.find(Tag);

	EFFECT effect;

	if (it == m_effectMap.end())
	{// ������Ȃ������Ƃ�
	 // ����������������L�[�Ƃ���
	 // �V�����}�b�v�Ƀe�N�X�`����ǉ�����
		return effect;
	}
	else
	{
		return it->second;
	}
}

//=============================================================================
// �G�t�F�N�g�̍쐬����
//=============================================================================
void CEffect::CreateEffect(std::string Tag, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	EFFECT effect;
	effect = GetEffectData(Tag);

	float fAngle = float(CManager::GetRand(314)) - float(CManager::GetRand(314));
	float fAngle_x = float(CManager::GetRand(314)) - float(CManager::GetRand(314));

	D3DXVECTOR3 particlePos;
	float fRadius;

	float fSpeed = 0.0f;
	//if (false)
	{// �X�s�[�h�������_������Ȃ������Ƃ�
		fSpeed = effect.fMaxSpeed;
	}

	switch (PARTICLESHAPE_SPHERE)
	{
	case PARTICLESHAPE_CONE:
		fRadius = float(CManager::GetRand(10 * 100)) / 100.0f - float(CManager::GetRand(10 * 100)) / 100.0f;

		particlePos = CEffect::GetRandomPosWithCone(fRadius);

		// �ʒu�̌v�Z
		particlePos.x = sinf(D3DX_PI * fAngle) * (fRadius);
		particlePos.y = effect.fMaxSpeed;
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

	//D3DXVECTOR3 rot = D3DXVECTOR3_ZERO;
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
		effect.texAdd,		// �p�[�e�B�N���̃^�C�v
		pos,			// �����ʒu
		effect.size,					// �T�C�Y
		D3DXVECTOR3(mtx._41, mtx._42, mtx._43) * fSpeed,	// �����x�N�g��
		effect.moveSize,				// �傫���̕ω���
		effect.moveRot,				// ��]�̕ω���
		effect.moveCol,				// �F�̕ω���
		EASINGTYPE_NONE,
		rot,					// �e�N�X�`���̉�]
		effect.col,					// �J���[
		effect.nLife,			// �p�[�e�B�N���̐����J�E���g��
		effect.fGravity,				// �d��
		effect.fResistance,			// ��R
		effect.bBillboard,			// �r���{�[�h
		0,						// �\������ӏ�(��)
		0,						// �\������ӏ�(�c)
		effect.centerPos,			// ���S�ʒu
		effect.fAngle,				// �p�x
		effect.fDistance,			// ����
		effect.fSpeedRot,		// ��]���x
		effect.sprite,				// �摜�̕�����
		effect.bAlpha,				// ���Z�����̗L��
		effect.bZBuffer,				// Z�o�b�t�@�̔�r�L��
		effect.bFadeOut				// �t�F�[�h
	);

}

//=============================================================================
// �p�[�e�B�N���X�N���v�g�A�Z�b�g�̃��[�h����
//=============================================================================
void CEffect::LoadParticleScript(void)
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
	pFile = fopen(MANAGER_PARTICLE, "r");

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
						CEffect::LoadScript(Add);
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
void CEffect::Debug(void)
{

}
#endif