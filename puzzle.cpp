//=============================================================================
//
// puzzle処理 [puzzle.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "puzzle.h"
#include "fade.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "inputController.h"
#include "box.h"
#include "network.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CPuzzle::CPuzzle()
{
	pBox = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPuzzle::~CPuzzle()
{

}

//=============================================================================
//Init処理
//=============================================================================
HRESULT CPuzzle::Init(void)
{
	LoadAsset();
	pBox = CBox::Create();
	return S_OK;
}

//=============================================================================
// Update関数
//=============================================================================
void CPuzzle::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputController *pInputController = CManager::GetInputController();
	CNetwork *pNetwork = CManager::GetNetwork();

	if (CFade::GetFade() == CFade::FADE_NONE)
	{//フェードが処理をしていないとき
		if (pInputKeyboard != NULL)
		{// キーボードが存在していたとき
			if (pInputKeyboard->GetTriggerKeyboard(DIK_RETURN))
			{// 指定のキーが押されたとき
				if (pNetwork != NULL)
				{
					if (pNetwork->Connect() == S_OK)
					{
						CFade::SetFade(CManager::MODE_GAME);					//フェードを入れる
					}
				}
			}
		}
		if (pInputController->GetJoypadUse(0))
		{// コントローラーが生成されているとき
		 //ゲームの遷移
			if (pInputController->GetControllerTrigger(0, JOYPADKEY_A) ||			// ゲームパッドのAボダンが押されたとき
				pInputController->GetControllerTrigger(0, JOYPADKEY_START))			// ゲームパッドのSTARTボタンが押されたとき
			{
				CFade::SetFade(CManager::MODE_GAME);					//フェードを入れる
			}
		}
	}
}

//=============================================================================
// Draw関数
//=============================================================================
void CPuzzle::Draw(void)
{

}

//=============================================================================
// Uninit関数
//=============================================================================
void CPuzzle::Uninit(void)
{
	if (pBox != NULL)
	{
		pBox->Uninit();
		pBox->Release();
		pBox = NULL;
	}

	// ポリゴンの開放
	CScene::ReleaseAll();
}

//=============================================================================
// アセットの読み込み
//=============================================================================
void CPuzzle::LoadAsset(void)
{
	CBox::Load();
}