// OpenGlCppCode.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include <exception>
#include <map>
#include <memory>
#include <math.h>

#include "D3DRender.h"

#pragma comment(lib, "D3d11.lib")
#pragma comment(lib, "D2d1.lib")
#pragma comment(lib, "Dwrite.lib")
// Если надо порутить то всегда возьми ПИ
#define M_PI 3.141592653589783

#define CPP_EXPORTS_API 
#ifdef CPP_EXPORTS_API 
#define CPP_API extern "C" __declspec(dllexport) 
#else 
#define CPP_API __declspec(dllimport) 
#endif 
using namespace std;
extern map<HWND, shared_ptr<DX::Directx11>>* device = NULL;
CPP_API void InitDirectX(int handle)
{
	(*device)[(HWND)handle] = make_shared<DX::Directx11>((HWND)handle);
}

CPP_API void PrepareScene(int handle, int w, int h)
{
	auto finded = device->find((HWND)handle);
	if (finded != device->end())
	{
		finded->second->CreateTarget(w, h);
		finded->second->ClearAll();

		vector<array<array<int, 3>, 2>> triangles(12);
		// первое лицо
		triangles[0][0] = { 0, 2, 1 };
		triangles[0][1] = { 152, 251, 152 };
		triangles[0][1] = { 152, 251, 152 };
		triangles[1][0] = { 1, 2, 3 };
		triangles[1][1] = { 152, 251, 152 };
		// второе лицо
		triangles[2][0] = { 4, 5, 6 };
		triangles[2][1] = { 255, 165, 0 };
		triangles[3][0] = { 5, 7, 6 };
		triangles[3][1] = { 255, 165, 0 };
		// третье лицо
		triangles[4][0] = { 6,7,8 };
		triangles[4][1] = { 255, 0, 0 };
		triangles[5][0] = { 7,9,8 };
		triangles[5][1] = { 255, 0, 0 };
		// четвертое лицо
		triangles[6][0] = { 1, 3, 4 };
		triangles[6][1] = { 238, 130, 238 };
		triangles[7][0] = { 3, 5, 4 };
		triangles[7][1] = { 238, 130, 238 };
		// пятое лицо
		triangles[8][0] = { 1, 11, 10 };
		triangles[8][1] = { 255, 228, 181 };
		triangles[9][0] = { 1,4, 11 };
		triangles[9][1] = { 255, 228, 181 };
		// шестое лицо
		triangles[10][0] = { 3, 12, 5 };
		triangles[10][1] = { 148, 0, 211 };
		triangles[11][0] = { 5, 12, 13 };
		triangles[11][1] = { 148, 0, 211 };
		vector<array<double, 5>> xyz;
		xyz.resize(14);
		xyz[0] = { -1,  1, -1, 0.f,   0.66f };
		xyz[1] = { -1, -1, -1, 0.25f, 0.66f };
		xyz[2] = { 1,  1, -1, 0.f,   0.33f };
		xyz[3] = { 1, -1, -1, 0.25f, 0.33f };
		xyz[4] = { -1, -1,  1, 0.5f,  0.66f };
		xyz[5] = { 1, -1,  1, 0.5f,  0.33f };
		xyz[6] = { -1,  1,  1, 0.75f, 0.66f };
		xyz[7] = { 1,  1,  1, 0.75f, 0.33f };

		xyz[8] = { -1, 1, -1, 1,    0.66 };
		xyz[9] = { 1, 1, -1, 1,    0.33 };
		xyz[10] = { -1, 1, -1, 0.25, 1 };
		xyz[11] = { -1, 1,  1, 0.5,  1 };
		xyz[12] = { 1, 1, -1, 0.25, 0 };
		xyz[13] = { 1, 1,  1, 0.5,  0 };

		vector<array<double, 3>> normals;
		normals.resize(3);
		finded->second->RenderStart();
		auto unit = finded->second->CreateTriangleColorUnit(triangles, xyz, normals);
		finded->second->AddToSaved(unit);
		finded->second->RenderSavedData();
		finded->second->EndRender();
	}
}

CPP_API void RenderScene(int handle, float angle)
{
	auto finded = device->find((HWND)handle);
	if (finded != device->end())
	{
		// Вот тут унас приколы с матрицами преобразования.                                    x     y     z. размеры 
		DirectX::XMStoreFloat4x4(&(finded->second->ModelViewMatrix), DirectX::XMMatrixScaling(0.25, 0.25, 0.25) *
			//                                                x    y    z  . Это все чтобы поворачивалось
			DirectX::XMMatrixRotationAxis(DirectX::XMVECTOR{ 1.0, 1.0, 0.0 }, angle) *
			DirectX::XMMatrixRotationAxis(DirectX::XMVECTOR{ 0.0, 1.0, 1.0 }, angle) *
			DirectX::XMMatrixRotationAxis(DirectX::XMVECTOR{ 1.0, 0.0, 1.0 }, angle) *
			//                            x    y  не торгать. Это чтобы эта штука текстрки накладывала прям на грани и было поцентру
			DirectX::XMMatrixTranslation(0.0, 0.0, -1));
		//DirectX::XMStoreFloat4x4(&(finded->second->ProjectionMatrix), DirectX::XMMatrixIdentity());
		finded->second->RenderStart();
		finded->second->RenderSavedData();
		finded->second->EndRender();
	}
}
