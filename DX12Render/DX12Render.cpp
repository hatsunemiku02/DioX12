//DX12Render.cpp: 定义应用程序的入口点。
//
#include <functional>
#include "stdafx.h"
#include "DX12Render.h"
#include "RenderSystem/DX12Device.h"

#include "RenderSystem/DX12Shader.h"
#include "RenderSystem/DX12VBO.h"
#include "RenderSystem/DX12CMDList.h"
#include "RenderSystem/DX12PSO.h"
#include "RenderSystem/DX12RootSig.h"
#include "RenderApp/SimplePipeline.h"
#include "RenderApp/RenderItem.h"
#include "RenderApp/Geometry.h"
#include "RenderApp/Material.h"

#include "RenderApp/RenderPipeline.h"
struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

Vertex triangleVertices[] =
{
	{ { 0.0f, 0.25f * 4/3, 0.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
{ { 0.25f, -0.25f * 4 / 3, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
{ { -0.25f, -0.25f * 4 / 3, 0.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } }
};

const char* shadercode = "\
cbuffer objConstantBuffer : register(b0)\
{\
	float4 offset;\
};\
cbuffer matConstantBuffer : register(b1)\
{\
	float4 tricolor;\
};\
struct PSInput\
{\
	float4 position : SV_POSITION;\
	float4 color : COLOR;\
};\
\
PSInput VSMain(float4 position : POSITION, float4 color : COLOR)\
{\
	PSInput result;\
\
	result.position = position+offset;\
	result.color = color*tricolor;\
\
	return result;\
}\
\
float4 PSMain(PSInput input) : SV_TARGET\
{\
	return input.color;\
}\
";


void hanshu(void)
{
	return;
}

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
std::shared_ptr<DX12Device> gDevice;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DX12RENDER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);


	RenderPipeline pipeline;

	//std::function<void(void)> test = std::bind(hanshu);
	
	std::thread renderThread = pipeline.RunInThread();
	renderThread.detach();


	/*

	gDevice = std::make_shared<DX12Device>();
    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	
	
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DX12RENDER));
	
    MSG msg;

	std::shared_ptr<DX12CMDList> mainTreadCMDList = std::make_shared<DX12CMDList>();
	mainTreadCMDList->SetupDirectCMDList(*gDevice);

	//DX12Shader shader;
	std::shared_ptr<DX12Shader> shader = std::make_shared<DX12Shader>();
	shader->CreateShader(gDevice.get(), (void*)shadercode, strlen(shadercode), "testshader", "VSMain", "PSMain");
	
	mainTreadCMDList->Reset(gDevice.get());
	std::shared_ptr<DX12VBO> vbo = std::make_shared<DX12VBO>();
	vbo->CreateFromData(gDevice.get(), *mainTreadCMDList, triangleVertices, sizeof(Vertex), sizeof(triangleVertices));
	gDevice->ExecuteCMDList(*mainTreadCMDList);
	gDevice->WaitForGpu();

	std::shared_ptr<Geometry> geo = std::make_shared<Geometry>();
	geo->Setup(vbo);

	DX12RootSig rootSig;
	rootSig.SetupRootSig(*gDevice,2,0,0,0);

	std::shared_ptr<DX12PSO> pso = std::make_shared<DX12PSO>();
	pso->SetShader(*shader);
	pso->SetVertexLayout(*vbo);
	pso->SetupPSO(*gDevice, rootSig);

	std::shared_ptr<Material> mat = std::make_shared<Material>();
	mat->m_pShader = shader;
	mat->m_pPSO = pso;
 	DirectX::XMFLOAT4 color(1, 0, 0, 0);// = DirectX::XMFLOAT4(1, 1, 0, 0);
 	mat->m_MatContent = (void*)&color;
 	mat->m_ContentSize = sizeof(DirectX::XMFLOAT4);
	RenderItem ritem;
	ritem.m_pGeometry = geo;
	ritem.m_pMaterial = mat;

	SimplePipeline pipeline;
	ritem.m_ObjBuffer.WorldViewProj = std::move( DirectX::XMFLOAT4(0.5, 0, 0, 0));
	pipeline.SetCMDList( mainTreadCMDList );
	pipeline.SetDevice(gDevice);
	pipeline.AddRenderItem(&ritem);
	
    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
		pipeline.BuildConstantBuffer();
		pipeline.UpdateConstantBuffer();
		pipeline.BeforeRender();
		pipeline.Render();
		pipeline.AfterRender();
// 		gDevice.BeforeRender();
// 		gDevice.DrawInstance(3, 1, 0, 0);
// 		gDevice.AfterRender();
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	*/
    return (int)0;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX12RENDER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DX12RENDER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }


 

   ShowWindow(hWnd, nCmdShow);

   gDevice->Init(800,600 , hWnd);

   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
