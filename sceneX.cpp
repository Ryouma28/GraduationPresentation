//=============================================================================
//
// sceneX���� [sceneX.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "sceneX.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "object.h"
#include "camera.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSceneX::CSceneX(CScene::PRIORITY obj = CScene::PRIORITY_MODEL) : CScene(obj)
{
	SetObjType(CScene::PRIORITY_MODEL);

	m_ShaderType = SHADERTYPE_LAMBERT;
	m_pBuffMat = NULL;
	m_pMesh = NULL;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_bDrawDebug = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSceneX::~CSceneX()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSceneX::Init(void)
{
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CSceneX::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CSceneX::Update(void)
{
#ifdef _DEBUG
	Debug();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSceneX::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	D3DXVECTOR3 pos = GetPosition();

	D3DXMATRIX		mtxRot, mtxTrans, mtxSize;		//�v�Z�p�}�g���b�N�X
	D3DXMATERIAL	*pMat;							//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9	matDef;							//�}�e���A���f�[�^�ւ̃|�C���^

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �T�C�Y�𔽉f
	D3DXMatrixScaling(&mtxSize, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxSize);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);					//���ׂẴf�[�^���^

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	D3DXMATRIX mtxView, mtxProj;

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	LPD3DXEFFECT pShader = NULL;

	switch (m_ShaderType)
	{
	case SHADERTYPE_LAMBERT:
		pShader = CManager::GetShaderResource(SHADERADD_LAMBERT);

		// �V�F�[�_�[����
		if (pShader != NULL)
		{// �V�F�[�_�[�����݂��Ă����Ƃ�
			pShader->SetTechnique("tecLambert");

			//���[���h�E�r���[�E�v���W�F�N�V�����s���n��
			pShader->SetMatrix("WVP", (D3DXMATRIX*)&(m_mtxWorld * mtxView * mtxProj));
			//���[���h�s��̋t�s��̓]�u�s���n��
			D3DXMATRIX mWIT;
			pShader->SetMatrix("WIT", D3DXMatrixTranspose(&mWIT, D3DXMatrixInverse(&mWIT, NULL, &m_mtxWorld)));

			//���C�g�̕����x�N�g����n��
			pShader->SetVector("LightDir", &D3DXVECTOR4(0.22f, -0.87f, 0.44f, 0.0f));

			//���ˌ��i���C�g�j�̋��x��n���@�ڈ�t���邢���F���ɂ��Ă݂�
			pShader->SetVector("LightIntensity", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

			//�g�U���˗���n��
			pShader->SetVector("Diffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

			// ������n��
			pShader->SetVector("Ambient", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

			//pShader->Begin(NULL, 0);
			//pShader->BeginPass(0);
		}
		break;
	case SHADERTYPE_TOON:
		pShader = CManager::GetShaderResource(SHADERADD_TOON);

		// �V�F�[�_�[����
		if (pShader != NULL)
		{
			pShader->SetTechnique("ToonShading");
			D3DXMATRIX mAll = m_mtxWorld * mtxView * mtxProj;
			pShader->SetMatrix("WVP", &mAll);

			pShader->SetMatrix("mProj", &mtxProj);
			pShader->SetMatrix("mView", &mtxView);
			pShader->SetMatrix("mWorld", &m_mtxWorld);

			pShader->SetTexture("ShadeTexture", CManager::GetResource("data/tex/ToonShader/Shade.bmp"));
			pShader->SetTexture("LineTexture", CManager::GetResource("data/tex/ToonShader/Outline.bmp"));

			CCamera *pCamera = CManager::GetCamera();
			CLight *pLight = CManager::GetLight();

			D3DXVECTOR4 lightPos = pLight->GetPos();

			pShader->SetVector("LightPos", (D3DXVECTOR4*)&lightPos);
			pShader->SetVector("EyePos", (D3DXVECTOR4*)&pCamera->GetPosV());

			pShader->Begin(NULL, 0);
		}
		break;
	case SHADERTYPE_BUMP:
		pShader = CManager::GetShaderResource(SHADERADD_BUMP);

		// �V�F�[�_�[����
		if (pShader != NULL)
		{// �V�F�[�_�[�����݂��Ă����Ƃ�
			pShader->SetTechnique("tecBumpMap");

			//���[���h�E�r���[�E�v���W�F�N�V�����s���n��
			pShader->SetMatrix("mWVP", (D3DXMATRIX*)&(m_mtxWorld * mtxView * mtxProj));

			//���C�g�x�N�g��
			D3DXMATRIX m;
			D3DXVECTOR4 vLight;
			D3DXMatrixInverse(&m, NULL, &m_mtxWorld);

			CLight *pLight = CManager::GetLight();
			D3DXVECTOR3 lightPos = pLight->GetPos();

			D3DXVec3Transform(&vLight, &lightPos, &m);
			D3DXVec3Normalize((D3DXVECTOR3*)&vLight, (D3DXVECTOR3*)&vLight);
			pShader->SetVector("vLightDir", &vLight);

			CCamera *pCamera = CManager::GetCamera();

			//�����x�N�g��
			D3DXVECTOR4 vEye;
			m = m_mtxWorld * mtxView;
			D3DXMatrixInverse(&m, NULL, &m);
			D3DXVec3Transform(&vEye, &pCamera->GetPosV(), &m);
			pShader->SetVector("vEyePos", &vEye);

			pShader->SetTexture("NormalMap", CManager::GetResource("data/tex/snow_nor.png"));

			pShader->Begin(NULL, 0);
		}
		break;
	}

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (pShader != NULL)
		{
			switch (m_ShaderType)
			{
			case SHADERTYPE_LAMBERT:
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, CManager::GetResource(pMat[nCntMat].pTextureFilename));

					if (pShader != NULL)
					{
						//���[���h�s��̋t�s��̓]�u�s���n��
						D3DXMATRIX mWIT;
						pShader->SetMatrix("WIT", D3DXMatrixTranspose(&mWIT, D3DXMatrixInverse(&mWIT, NULL, &m_mtxWorld)));

						// �e�N�X�`���̐ݒ�
						pShader->SetTexture("texDecal", CManager::GetResource(pMat[nCntMat].pTextureFilename));
					}
				}
				else
				{
					// �e�N�X�`���̐ݒ�
					pShader->SetTexture("texDecal", CManager::GetResource(TEXTUREADD_DEFAULT));
				}

				pShader->SetVector("Diffuse", (D3DXVECTOR4*)&pMat[nCntMat].MatD3D.Diffuse);
				pShader->SetVector("Ambient", (D3DXVECTOR4*)&pMat[nCntMat].MatD3D.Ambient);
				pShader->CommitChanges();
				break;
			case SHADERTYPE_TOON:
				pShader->SetFloatArray("Diffuse", (FLOAT*)&pMat[nCntMat].MatD3D.Diffuse, 4);

				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̐ݒ�
					pShader->SetTexture("DecalTexture", CManager::GetResource(pMat[nCntMat].pTextureFilename));
				}
				else
				{
					// �e�N�X�`���̐ݒ�
					pShader->SetTexture("DecalTexture", CManager::GetResource(TEXTUREADD_DEFAULT));
				}
				pShader->BeginPass(0);
				break;
			case SHADERTYPE_BUMP:
				pShader->SetFloatArray("Diffuse", (FLOAT*)&pMat[nCntMat].MatD3D.Diffuse, 4);

				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̐ݒ�
					pShader->SetTexture("Decal", CManager::GetResource(pMat[nCntMat].pTextureFilename));
				}
				else
				{
					// �e�N�X�`���̐ݒ�
					pShader->SetTexture("Decal", CManager::GetResource(TEXTUREADD_DEFAULT));
				}
				pShader->BeginPass(0);
				break;
			}
		}

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �`��
		m_pMesh->DrawSubset(nCntMat);

		if (m_ShaderType == SHADERTYPE_TOON || m_ShaderType == SHADERTYPE_BUMP)
		{
			if (pShader != NULL)
			{
				pShader->EndPass();
			}
		}
	}

	if (pShader != NULL)
	{
		switch (m_ShaderType)
		{
		case SHADERTYPE_LAMBERT:
			//pShader->EndPass();
			//pShader->End();
			break;
		case SHADERTYPE_TOON:
			// �e�N�X�`���̐ݒ�
			pShader->SetTexture("DecalTexture", CManager::GetResource("data/tex/default.jpg"));
			pShader->End();
			break;
		case SHADERTYPE_BUMP:
			// �e�N�X�`���̐ݒ�
			pShader->SetTexture("DecalTexture", CManager::GetResource("data/tex/default.jpg"));
			pShader->End();
			break;
		}
	}

	pDevice->SetTexture(0, NULL);

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �N���G�C�g�֐�
//=============================================================================
CSceneX *CSceneX::Create(void)
{
	CSceneX *pSceneX;
	pSceneX = new CSceneX(CScene::PRIORITY_MODEL);

	if (pSceneX != NULL)
	{// �V�[�������݂��Ă����Ƃ�
		pSceneX->Init();				// ����������
	}
	return pSceneX;
}

//=============================================================================
// ���f�����̐ݒ�
//=============================================================================
void CSceneX::BindModel(LPD3DXMESH pMesh, DWORD nNumMat, LPD3DXBUFFER pBuffMat)
{
	m_pMesh = pMesh;
	m_nNumMat = nNumMat;
	m_pBuffMat = pBuffMat;
}

//=============================================================================
// �V�F�[�_�[�̏���������
//=============================================================================
void CSceneX::Load(void)
{
	//�V�F�[�_�[��ǂݍ���
	CManager::LoadShader((std::string)SHADERADD_LAMBERT);
	CManager::LoadShader((std::string)SHADERADD_TOON);
	CManager::LoadShader((std::string)SHADERADD_BUMP);
}

//=============================================================================
// �C���X�y�N�^�[��\��
//=============================================================================
void CSceneX::ShowInspector(void)
{
	D3DXVECTOR3 pos = GetPosition();

	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::DragFloat3("Position", (float*)&pos, 0.1f);
		ImGui::DragFloat3("Rotation", (float*)&m_rot, 0.01f);
		ImGui::Text("");

		SetPosition(pos);
	}

	CScene::ShowInspector();
}

//=============================================================================
// �V�F�[�_�[�̃p�����[�^�ݒ�
//=============================================================================
void CSceneX::SetShaderParameter(LPD3DXEFFECT & pShader)
{
	switch (m_ShaderType)
	{
	case SHADERTYPE_LAMBERT:

		break;
	case SHADERTYPE_TOON:
		break;
	}
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CSceneX::Debug(void)
{

}
#endif