//wfbot by n7
#include <windows.h>
#include <fstream>
#include <vector>
#include <time.h>
#include <d3d9.h>
//#include <d3dx9.h>
#include "DXSDK\d3dx9.h"
#include "detours.h"

#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "DXSDK/d3dx9.lib") 
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"detours.lib")
using namespace std;

#include "main.h"	//helper funcs here

//=====================================================================================================================

typedef HRESULT(WINAPI* CreateDevice_t)				(LPDIRECT3D9, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS*, LPDIRECT3DDEVICE9*);
typedef HRESULT(WINAPI* DrawIndexedPrimitive_t)		(LPDIRECT3DDEVICE9, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
//typedef HRESULT(WINAPI* EndScene_t)				(LPDIRECT3DDEVICE9);
typedef HRESULT(WINAPI* Present_t)					(LPDIRECT3DDEVICE9, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);
typedef HRESULT(WINAPI* Reset_t)					(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
typedef HRESULT(WINAPI *SetTexture_t)				(LPDIRECT3DDEVICE9, DWORD, IDirect3DBaseTexture9*);
typedef HRESULT(WINAPI *SetVertexShader_t)			(LPDIRECT3DDEVICE9, IDirect3DVertexShader9*);
//typedef HRESULT(WINAPI *SetVertexShaderConstantF_t)	(LPDIRECT3DDEVICE9, UINT, const float*, UINT);
typedef HRESULT(WINAPI *SetPixelShader_t)			(LPDIRECT3DDEVICE9, IDirect3DPixelShader9*);
typedef HRESULT(WINAPI *SetVertexDeclaration_t)		(LPDIRECT3DDEVICE9, IDirect3DVertexDeclaration9 *pDecl);


CreateDevice_t CreateDevice;
DrawIndexedPrimitive_t DrawIndexedPrimitive;
//EndScene_t EndScene;
Present_t Present;
Reset_t Reset;
SetTexture_t SetTexture;
SetVertexShader_t SetVertexShader;
//SetVertexShaderConstantF_t SetVertexShaderConstantF;
SetPixelShader_t SetPixelShader;
SetVertexDeclaration_t SetVertexDeclaration;

//=====================================================================================================================

HRESULT APIENTRY DrawIndexedPrimitiveHook(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	//void* ReturnAddress = _ReturnAddress();
	
	//Aim at TEAM WARFACE
	if ((aimbot == 1||esp==1) && (texCRC == 0x6d656cfc))
	{
		AddAim(Device, 1);
	}

	//Aim at TEAM BLACKWOOD
	if ((aimbot == 2||esp==1) && (texCRC == 0xef209505))
	{
		AddAim(Device, 2);
	}

	//Aim at ALL
	if ((aimbot == 3 || esp == 3) && (decl->Type == 16 && numElements == 7 && vSize == 2012))
	{
		AddAim(Device, 3);
	}

	
	//wallhack
	if((wallhack == 1 && decl->Type == 16 && numElements == 7 && vSize == 2012) && (pSize == 808 || pSize == 640))
	//(wallhack == 1 && ReturnAddress != NULL && ReturnAddress == (void *)WFPlayer))
	{
		//Device->SetTexture(0, texBlackwood);
		Device->SetRenderState(D3DRS_ZENABLE, false);
		DrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		Device->SetRenderState(D3DRS_ZENABLE, true);
		//Device->SetTexture(0, texWarface);
	}

	//smoke?
	//texCRC == 5acb22ec && NumVertices == 44 && primCount == 22 && decl->Type == 2 && numElements == 5 && vSize == 2120 && pSize == 788
	//texCRC == 5acb22ec && NumVertices == 44 && primCount == 22 && decl->Type == 2 && numElements == 5 && vSize == 2120 && pSize == 788
	//texCRC == a5948db9 && NumVertices == 48 && primCount == 24 && decl->Type == 2 && numElements == 5 && vSize == 2184 && pSize == 980
	if(nosmoke == 1)
	if ((decl->Type == 2 && numElements == 5) && (vSize >= 1832))//(vSize == 2120|| vSize == 2184|| vSize == 1832))
	{
		return D3D_OK;
	}
	
	//fog?
	//texCRC == d8092660 && NumVertices == 8 && primCount == 12 && decl->Type == 2 && numElements == 4 && vSize == 224 && pSize == 124
	//texCRC == 5ae5a2cd && NumVertices == 24 && primCount == 12 && decl->Type == 16 && numElements == 10 && vSize == 968 && pSize == 664
	//texCRC == 36ffff4e && NumVertices == 24 && primCount == 12 && decl->Type == 2 && numElements == 4 && vSize == 300 && pSize == 324
	//Stride == 24 && texCRC == a4b0ecf0 && NumVertices == 4 && primCount == 2 && decl->Type == 2 && numElements == 6 && sdesc.Size == 589824
	//Stride == 24 && texCRC == 10200a27 && NumVertices == 1196 && primCount == 2250 && decl->Type == 2 && numElements == 4 && sdesc.Size == 3145728

	
	//small bruteforce logger
	if (logger)
	{
		//hold down P key until a texture changes, press I to log values of those textures
		if (GetAsyncKeyState('O') & 1) //-
			countnum--;
		if (GetAsyncKeyState('P') & 1) //+
			countnum++;
		if ((GetAsyncKeyState(VK_MENU)) && (GetAsyncKeyState('9') & 1)) //reset, set to -1
			countnum = -1;
		if (countnum == vSize/10)
			if (GetAsyncKeyState('I') & 1) //press I to log to log.txt
				if (texCRC != 0)
				Log("texCRC == %x && NumVertices == %d && primCount == %d && decl->Type == %d && numElements == %d && vSize == %d && pSize == %d", texCRC, NumVertices, primCount, decl->Type, numElements, vSize, pSize);
		if (countnum == vSize/10)
		{
			Device->SetPixelShader(NULL);
			//return D3D_OK; //delete texture

		}
	}
	
	
	return DrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

//=====================================================================================================================

bool DoInit = true;
HRESULT APIENTRY PresentHook(LPDIRECT3DDEVICE9 Device, CONST RECT *pSrcRect, CONST RECT *pDestRect, HWND hDestWindow, CONST RGNDATA *pDirtyRegion)
{
	//sprite
	PreClear(Device);

	if (DoInit)
	{
		//get viewport
		Device->GetViewport(&Viewport);
		ScreenCenterX = (float)Viewport.Width / 2.0f;
		ScreenCenterY = (float)Viewport.Height / 2.0f;

		//generate texture once
		//GenerateTexture(Device, &texBlackwood, D3DCOLOR_ARGB(255, 255, 0, 0));
		//GenerateTexture(Device, &texWarface, D3DCOLOR_ARGB(255, 0, 255, 0));
		//GenerateTexture(Device, &texBlue, D3DCOLOR_ARGB(255, 0, 0, 255));
		//GenerateTexture(Device, &texYellow, D3DCOLOR_ARGB(255, 255, 0, 0));

		LoadSettings();

		DoInit = false;
	}

	if (pFont == NULL)
	{
		HRESULT hr = D3DXCreateFont(Device, 13, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &pFont);

		if (FAILED(hr)) {
			Log("D3DXCreateFont failed");
		}
	}

	if (pFont)
	{
		Device->GetViewport(&Viewport);
		BuildMenu(Device);
	}

	//Shift|RMouse|LMouse|Ctrl|Alt|Space|X|C
	if (aimkey == 0) Daimkey = 0;
	if (aimkey == 1) Daimkey = VK_SHIFT;
	if (aimkey == 2) Daimkey = VK_RBUTTON;
	if (aimkey == 3) Daimkey = VK_LBUTTON;
	if (aimkey == 4) Daimkey = VK_CONTROL;
	if (aimkey == 5) Daimkey = VK_MENU;
	if (aimkey == 6) Daimkey = VK_SPACE;
	if (aimkey == 7) Daimkey = 0x58; //X
	if (aimkey == 8) Daimkey = 0x43; //C

	 //esp part 2
	if (esp == 1 || esp == 2 || esp == 3)
		if (AimInfo.size() != NULL)
		{
			for (unsigned int i = 0; i < AimInfo.size(); i++)
			{
				if (esp == AimInfo[i].iTeam && AimInfo[i].vOutX > 1 && AimInfo[i].vOutY > 1)
				{
					//drawpic
					PrePresent(Device, (int)AimInfo[i].vOutX - 32, (int)AimInfo[i].vOutY - 20);
					
					//FillRGB(Device, (int)AimInfo[i].vOutX, (int)AimInfo[i].vOutY, 8, 8, D3DCOLOR_ARGB(255, 0, 255, 0));
					//DrawBorder(Device, (int)AimInfo[i].vOutX - 9, (int)AimInfo[i].vOutY, 20, 30, 1, Green);
					//DrawString(pFont, (int)AimInfo[i].vOutX - 9, (int)AimInfo[i].vOutY, Green, "%.f", RealDistance*2.0f);
				}
			}
		}

	//aimbot part 2
	//if (aimbot > 0 && AimInfo.size() != NULL && GetAsyncKeyState(Daimkey))
	if (aimbot > 0 && AimInfo.size() != NULL)
	{
		UINT BestTarget = -1;
		DOUBLE fClosestPos = 99999;

		for (unsigned int i = 0; i < AimInfo.size(); i++)
		{
			//aimfov
			float radiusx = (aimfov*10.0f) * (ScreenCenterX / 100);
			float radiusy = (aimfov*10.0f) * (ScreenCenterY / 100);

			if (aimfov == 0)
			{
				radiusx = 5.0f * (ScreenCenterX / 100);
				radiusy = 5.0f * (ScreenCenterY / 100);
			}

			//get crosshairdistance
			AimInfo[i].CrosshairDistance = GetDistance(AimInfo[i].vOutX, AimInfo[i].vOutY, ScreenCenterX, ScreenCenterY);

			//aim at team 1, 2 or 3
			if (aimbot == AimInfo[i].iTeam)

				//if in fov
				if (AimInfo[i].vOutX >= ScreenCenterX - radiusx && AimInfo[i].vOutX <= ScreenCenterX + radiusx && AimInfo[i].vOutY >= ScreenCenterY - radiusy && AimInfo[i].vOutY <= ScreenCenterY + radiusy)

					//get closest/nearest target to crosshair
					if (AimInfo[i].CrosshairDistance < fClosestPos)
					{
						fClosestPos = AimInfo[i].CrosshairDistance;
						BestTarget = i;
					}
		}


		//if nearest target to crosshair
		if (BestTarget != -1)
		{
			double DistX = AimInfo[BestTarget].vOutX - ScreenCenterX;
			double DistY = AimInfo[BestTarget].vOutY - ScreenCenterY;

			DistX /= (1+aimsens);
			DistY /= (1+aimsens);

			//aim
			if (GetAsyncKeyState(Daimkey) & 0x8000)
				mouse_event(MOUSEEVENTF_MOVE, (int)DistX, (int)DistY, 0, NULL);

			//autoshoot on
			//if ((!GetAsyncKeyState(VK_LBUTTON) && (autoshoot == 1) && (GetAsyncKeyState(Daimkey) & 0x8000))) //
			if (((!GetAsyncKeyState(VK_LBUTTON) && (autoshoot == 1) && (GetAsyncKeyState(Daimkey) & 0x8000))) || ((!GetAsyncKeyState(VK_LBUTTON) && (autoshoot == 2))))
			{
				if (!IsPressed)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					IsPressed = true;
				}
			}
		}
	}
	AimInfo.clear();

	if (autoshoot > 0 && IsPressed)
	{
		if ((timeGetTime() - frametime >= 99)||(timeGetTime() >= (frametime + CLOCKS_PER_SEC / 2)))
		{
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			IsPressed = false;
			frametime = timeGetTime();
		}
	}

	//logger
	if ((GetAsyncKeyState(VK_MENU)) && (GetAsyncKeyState(VK_CONTROL)) && (GetAsyncKeyState(0x4C) & 1)) //ALT + CTRL + L toggles logger
		logger = !logger;
	if (pFont && logger) //&& countnum >= 0)
	{
		char szString[255];
		sprintf_s(szString, "countnum = %d", countnum);
		DrawString(pFont, 220, 100, White, (char*)&szString[0]);
		DrawString(pFont, 220, 110, Yellow, "hold P to +");
		DrawString(pFont, 220, 120, Yellow, "hold O to -");
		DrawString(pFont, 220, 130, Green, "press I to log");
	}

	return Present(Device, pSrcRect, pDestRect, hDestWindow, pDirtyRegion);
}

//=====================================================================================================================

HRESULT APIENTRY SetVertexDeclarationHook(LPDIRECT3DDEVICE9 Device, IDirect3DVertexDeclaration9* pDecl)
{
	if (pDecl != NULL)
	{
		HRESULT hr = pDecl->GetDeclaration(decl, &numElements);
		if (FAILED(hr))
		{
			//Log("GetDeclaration failed");
		}
		//else if (numElements > 0)
		//{
			
		//}
	}

	return SetVertexDeclaration(Device, pDecl);
}

//=====================================================================================================================

HRESULT APIENTRY SetVertexShaderHook(LPDIRECT3DDEVICE9 Device, IDirect3DVertexShader9 *veShader)
{
	if (veShader != NULL)
	{
		vShader = veShader;
		vShader->GetFunction(NULL, &vSize);
	}

	return SetVertexShader(Device, veShader);
}

//=====================================================================================================================

HRESULT APIENTRY SetPixelShaderHook(LPDIRECT3DDEVICE9 Device, IDirect3DPixelShader9 *piShader)
{
	if (piShader != NULL)
	{
		pShader = piShader;
		pShader->GetFunction(NULL, &pSize);
	}

	return SetPixelShader(Device, piShader);
}

//=====================================================================================================================

//HRESULT APIENTRY SetVertexShaderConstantFHook(LPDIRECT3DDEVICE9 Device, UINT StartRegister, const float *pConstantData, UINT Vector4fCount)
//{
	//if (pConstantData)
	//{
		//mStartRegister = StartRegister;
		//mVector4fCount = Vector4fCount;
	//}

	//return SetVertexShaderConstantF(Device, StartRegister, pConstantData, Vector4fCount);
//}

//=====================================================================================================================

//HRESULT APIENTRY EndSceneHook(LPDIRECT3DDEVICE9 Device)
//{
	
	//return EndScene(Device);
//}

//=====================================================================================================================

HRESULT APIENTRY ResetHook(LPDIRECT3DDEVICE9 Device, D3DPRESENT_PARAMETERS* Params)
{
	DeleteRenderSurfaces();

	if (pFont)
		pFont->OnLostDevice();

	HRESULT ResetReturn = Reset(Device, Params);

	HRESULT cooperativeStat = Device->TestCooperativeLevel();

	switch (cooperativeStat)
	{
	case D3DERR_DEVICELOST:
		//Log("D3DERR_DEVICELOST");
		break;
	case D3DERR_DEVICENOTRESET:
		//Log("D3DERR_DEVICENOTRESET");
		break;
	case D3DERR_DRIVERINTERNALERROR:
		//Log("D3DERR_DRIVERINTERNALERROR");
		break;
	case D3D_OK:
		break;
	}
	
	if (SUCCEEDED(ResetReturn))
	{
		if (pFont)
			pFont->OnResetDevice();

		Device->GetViewport(&Viewport);
		ScreenCenterX = Viewport.Width / 2.0f;
		ScreenCenterY = Viewport.Height / 2.0f;
	}

	return ResetReturn;
}

//=====================================================================================================================

HRESULT APIENTRY SetTextureHook(LPDIRECT3DDEVICE9 Device, DWORD Stage, IDirect3DBaseTexture9 *pTexture)
{
	pCurrentTexture = static_cast<IDirect3DTexture9*>(pTexture);

	if (Stage == 0 && pCurrentTexture)
	{
		if (reinterpret_cast<IDirect3DTexture9 *>(pCurrentTexture)->GetType() == D3DRTYPE_TEXTURE)
		{
			pCurrentTexture->GetLevelDesc(0, &desc);
			if (desc.Pool == D3DPOOL_MANAGED) //warface
			//if (desc.Pool != D3DPOOL_MANAGED) //some other games
			{
				pCurrentTexture->LockRect(0, &pLockedRect, NULL, D3DLOCK_NOOVERWRITE | D3DLOCK_READONLY);
				//pCurrentTexture->LockRect(0, &pLockedRect, NULL, 0); //few other games

				if (pLockedRect.pBits != NULL)
					//get crc
					texCRC = QuickChecksum((DWORD*)pLockedRect.pBits, 12);

				pCurrentTexture->UnlockRect(0);
			}
		}
	}

	return SetTexture(Device, Stage, pTexture);
}

//=====================================================================================================================

HRESULT InitHooks()
{
	HMODULE hD3D9DLL = 0;
	do
	{
		hD3D9DLL = GetModuleHandle("d3d9.dll");
		Sleep(100);
	} while (!hD3D9DLL);
	Sleep(1000);

	DWORD TableAddress = FindPattern((DWORD)GetModuleHandle("d3d9.dll"), 0x128000, (PBYTE)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", "xx????xx????xx");
	DirectX9VTable* VTable = (DirectX9VTable*)(*(DWORD*)(TableAddress + 2));

	if (TableAddress)
	{
		//EndScene = (EndScene_t)DetourFunction((PBYTE)VTable->EndScene, (PBYTE)EndSceneHook);
		Present = (Present_t)DetourFunction((PBYTE)VTable->Present, (PBYTE)PresentHook);
		DrawIndexedPrimitive = (DrawIndexedPrimitive_t)DetourFunction((PBYTE)VTable->DrawIndexedPrimitive, (PBYTE)DrawIndexedPrimitiveHook);
		SetTexture = (SetTexture_t)DetourFunction((PBYTE)VTable->SetTexture, (PBYTE)SetTextureHook);
		SetVertexShader = (SetVertexShader_t)DetourFunction((PBYTE)VTable->SetVertexShader, (PBYTE)SetVertexShaderHook);
		Reset = (Reset_t)DetourFunction((PBYTE)VTable->Reset, (PBYTE)ResetHook);
		//SetVertexShaderConstantF = (SetVertexShaderConstantF_t)DetourFunction((PBYTE)VTable->SetVertexShaderConstantF, (PBYTE)SetVertexShaderConstantFHook);
		SetPixelShader = (SetPixelShader_t)DetourFunction((PBYTE)VTable->SetPixelShader, (PBYTE)SetPixelShaderHook);
		SetVertexDeclaration = (SetVertexDeclaration_t)DetourFunction((PBYTE)VTable->SetVertexDeclaration, (PBYTE)SetVertexDeclarationHook);
		return 0;
	}
	else
	{
		Log("Unable To Find VTable");
		return -1;
	}
}

//=====================================================================================================================

void APIENTRY Init(LPVOID param)
{
	if (SUCCEEDED(InitHooks()))
	{

	}
	else
	{
		Log("Initializing Hooks FAILED");
	}
}

//=====================================================================================================================

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD Reason, _In_opt_ LPVOID Reserved)
{
	switch (Reason)
	{
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hInstance);

			GetModuleFileNameA(hInstance, dlldir, 512);
			for (int i = (int)strlen(dlldir); i > 0; i--)
			{
				if (dlldir[i] == '\\')
				{
					dlldir[i + 1] = 0;
					break;
				}
			}

			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Init, 0, 0, 0);
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
		case DLL_THREAD_DETACH:
			break;
	}
	return TRUE;
}