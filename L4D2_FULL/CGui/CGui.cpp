#include "CGui.hpp"
#include "mh_wrap.hpp"
#include "CLogger.hpp"
#include "CCheat.hpp"
#include "IClientEntityList.hpp"
#include "CInterfaceWrapper.hpp"
#include "IClientEntity.hpp"
#include "CClientClass.hpp"

BOOL CALLBACK CGui::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD procId = 0;
	GetWindowThreadProcessId(hwnd, &procId);
	if (procId == GetCurrentProcessId() && hwnd != GetConsoleWindow())
	{
		char title[256];
		GetWindowTextA(hwnd, title, sizeof(title));
		title[64] = '\0';
		CLogger::Log(eLogLevel::Dev, "Window title -> %s <-.", title);

		CGui::window = hwnd;
		return false;
	}
	return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI CGui::hkWndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYUP && wParam == VK_INSERT)
	{
		CGui::bMenuOpen = !CGui::bMenuOpen;
		return false;
	}

	if (CGui::bMenuOpen && !ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	return CallWindowProc(oWndproc, hWnd, uMsg, wParam, lParam);
}

int* CGui::GetDirectDeviceVTable()
{
	int* vTable					= nullptr;
	D3DPRESENT_PARAMETERS d3dpp = {};

	d3dpp.Windowed				= true;
	d3dpp.SwapEffect			= D3DSWAPEFFECT_COPY;

	IDirect3DDevice9* device	= nullptr;
	IDirect3D9* d3d9			= Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d9)
	{
		CLogger::Log(eLogLevel::Error, "Failed to create IDirect3D9 object.");
		MessageBoxA(nullptr, "Critical error. Game will be closed.", "Cheat", MB_OK);

		return nullptr;
	}

	HRESULT deviceCreated = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device);

	if (deviceCreated == D3D_OK)
	{
		CLogger::Log(eLogLevel::Info, "Device created for window mode.");

		vTable = *reinterpret_cast<int**>(device);

		//device->Release();
	}
	else
	{
		d3dpp.Windowed = false;
		deviceCreated = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device);

		if (deviceCreated == D3D_OK)
		{
			CLogger::Log(eLogLevel::Info, "Device created for fullscreen mode.");

			vTable = *reinterpret_cast<int**>(device);

			//device->Release();
		}
	}

	//d3d9->Release(); //TODO: »з-за release адреса в пам€ти мен€ютс€?

	CLogger::Log(eLogLevel::Dev, "vTable -> %p <-.", vTable);

	return vTable;
}

static inline void InitImgui(IDirect3DDevice9* device)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::GetIO().IniFilename			= NULL;

	ImGui::GetStyle().AntiAliasedFill	= true;
	ImGui::GetStyle().AntiAliasedLines	= true;

	D3DDEVICE_CREATION_PARAMETERS params = {};

	if (device->GetCreationParameters(&params) != S_OK)
		return;

	if (!(ImGui_ImplWin32_Init(params.hFocusWindow) && ImGui_ImplDX9_Init(device) && params.hFocusWindow != 0))
		return;
}

HRESULT __stdcall CGui::hkEndScene(IDirect3DDevice9* this_)
{
	const auto original = mh_wrap::instance().original<decltype(&CGui::hkEndScene)>("hkEndScene");
	static bool bInit = false;

	if (!bInit)
	{
		//EnumWindows(EnumWindowsProc, 0);
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX9_Init(this_);
		InitImgui(this_);

		oWndproc = reinterpret_cast<t_Wndproc>(SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hkWndproc))); //TODO: UnHook + normal view for hook
		CLogger::Log(eLogLevel::Debug, "OWndproc -> %p <-.", oWndproc);

		CLogger::Log(eLogLevel::Dev, "Init ImGui end.");
		bInit = true;
	}

	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX9_NewFrame();
	ImGui::NewFrame();

	if (bMenuOpen)
	{
		ImGui::Begin("Main");

		ImGui::Checkbox("ESP", &config::bEnableEsp);
		ImGui::Indent();

		//Esp settings here

		ImGui::Checkbox("Show Death Zombie", &config::espSettings::bShowDeathEnemy);
		ImGui::Checkbox("Show Health", &config::espSettings::bShowHealth);
		ImGui::Checkbox("Show Survivors", &config::espSettings::bShowSurvivors);

		//

		ImGui::Separator();

		ImGui::End();
	}

	if (config::bEnableEsp)
	{
		CCheat::RenderEsp();

		//Draw esp box and e.t.c.
		//static bool t = false;
		//static const auto icel = reinterpret_cast<IClientEntityList*>(CInterfaceWrapper::GetInterface("client.dll", "VClientEntityList003"));

		//if (!t)
		//{
		//	CCheat::RenderEsp();

		//	for (int i = 0; i < icel->GetHighestEntityIndexInUsed(); ++i)
		//	{
		//		const auto pCurrEntity = icel->GetClientEntity(i);
		//		if (pCurrEntity)
		//		{	
		//			//CLogger::Log(eLogLevel::Dev, "%s", IClientNetworkable::GetClientClass(pCurrEntity)->GetName());
		//			const auto szClassName = IClientNetworkable::GetClientClass(pCurrEntity)->GetName();

		//			
		//		}
		//	}
		//	t = true;
		//}
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return original(this_);
}

HRESULT __stdcall CGui::hkPresent(IDirect3DDevice9* this_, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	const auto original = mh_wrap::instance().original<decltype(&CGui::hkPresent)>("hkPresent");
	static bool bInit = false;
	CLogger::Log(eLogLevel::Debug, "Hello from hkPresent");
	if (!bInit)
	{
		//EnumWindows(EnumWindowsProc, 0);
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX9_Init(this_);
		InitImgui(this_);

		oWndproc = reinterpret_cast<t_Wndproc>(SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hkWndproc))); //TODO: UnHook + normal view for hook
		CLogger::Log(eLogLevel::Debug, "OWndproc -> %p <-.", oWndproc);
		CLogger::Log(eLogLevel::Dev, "Init ImGui end.");
		bInit = true;
	}

	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX9_NewFrame();
	ImGui::NewFrame();

	if (bMenuOpen)
	{
		ImGui::Begin("Main");

		ImGui::Checkbox("ESP", &config::bEnableEsp);
		ImGui::Indent();

		//Esp settings here

		ImGui::Checkbox("Show Death Zombie", &config::espSettings::bShowDeathEnemy);
		ImGui::Checkbox("Show Health", &config::espSettings::bShowHealth);
		ImGui::Checkbox("Show Survivors", &config::espSettings::bShowSurvivors);

		//

		ImGui::Separator();

		ImGui::End();
	}

	if (config::bEnableEsp)
	{
		CCheat::RenderEsp();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return original(this_, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}
