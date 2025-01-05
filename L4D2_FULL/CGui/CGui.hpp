#pragma once

#pragma comment(lib, "d3d9.lib")

#include <d3d9.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx9.h"

typedef LRESULT(WINAPI* t_Wndproc)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
typedef HRESULT(__stdcall* t_EndScene)(IDirect3DDevice9* this_);

class CGui
{
public:
	[[nodiscard]] static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
	[[nodiscard]] static LRESULT WINAPI hkWndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	[[nodiscard]] static int* GetDirectDeviceVTable();
	[[nodiscard]] static HRESULT __stdcall hkEndScene(IDirect3DDevice9* this_);
	[[nodiscard]] static HRESULT __stdcall hkPresent(IDirect3DDevice9* this_, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion); //Не воркает.

public:
	[[nodiscard]] static inline HWND GetWindow() noexcept
	{
		return window;
	}
	
	[[nodiscard]] static inline t_Wndproc GetOWndProc() noexcept
	{
		return oWndproc;
	}

private:
	static inline HWND window			 = nullptr;
	static inline t_Wndproc oWndproc	 = nullptr;
	static inline bool bMenuOpen		 = false;
};