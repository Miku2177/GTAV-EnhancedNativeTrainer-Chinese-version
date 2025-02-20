/*
	本文件是 GTA V Script Hook SDK 的一部分
				http://dev-c.com
	(C) Alexander Blade 2015-2016
*/

#pragma once

#include <windows.h>

#define IMPORT __declspec(dllimport)

/* 纹理 */

// 创建纹理
//	texFileName	- 纹理文件名，最好指定完整的纹理路径并使用 PNG 纹理
//	返回	内部纹理 ID
//	纹理删除会在游戏重新加载脚本时自动执行
//	只能在原生函数所在的线程中调用

IMPORT int createTexture(const char *texFileName);

// 绘制纹理
//	id		-	从 createTexture() 获取的纹理 ID
//	index	-	每个纹理在屏幕上最多可以同时存在 64 个不同的实例
//	level	-	绘制层级，用于全局绘制顺序，层级最小的纹理实例最先绘制
//	time	-	纹理实例在屏幕上停留的时间（毫秒），时间应足够长，
//				以确保它在下一次对应的 drawTexture() 调用之前保持在屏幕上
//	sizeX,Y	-	在屏幕空间中的大小，范围应为 0.0 到 1.0，例如设置为 0.2 表示
//				纹理实例将占据屏幕空间的 20%
//	centerX,Y -	在纹理空间中的中心位置，例如 0.5 表示纹理的真实中心
//	posX,Y	-	在屏幕空间中的位置，[0.0, 0.0] 表示左上角，[1.0, 1.0] 表示右下角，
//				纹理实例根据其中心位置定位
//	rotation -	旋转角度，范围应为 0.0 到 1.0
//	screenHeightScaleFactor - 屏幕宽高比，用于纹理大小校正，可以通过原生函数获取
//	r,g,b,a	-	颜色，范围应为 0.0 到 1.0
//
//	每次脚本调用 drawTexture() 时，纹理实例的绘制参数都会更新
//	您应始终检查纹理在 16:9、16:10 和 4:3 屏幕宽高比下的布局，例如在 1280x720、
//	1440x900 和 1024x768 屏幕分辨率下，使用窗口模式进行测试
//	只能在原生函数所在的线程中调用

IMPORT void drawTexture(int id, int index, int level, int time,
						float sizeX, float sizeY, float centerX, float centerY,
						float posX, float posY, float rotation, float screenHeightScaleFactor,
						float r, float g, float b, float a);

					// IDXGISwapChain::Present 回调
					// 在实际的 Present 方法调用之前调用，渲染测试调用不会触发回调
					// 当游戏使用 DX10 时，实际上是使用具有 DX10 功能级别的 DX11
					// 请记住，您不能在回调内部调用原生函数
					// void OnPresent(IDXGISwapChain *swapChain);
typedef void(*PresentCallback)(void *);

// 注册 IDXGISwapChain::Present 回调
// 必须在 DLL 附加时调用
IMPORT void presentCallbackRegister(PresentCallback cb);

// 注销 IDXGISwapChain::Present 回调
// 必须在 DLL 分离时调用
IMPORT void presentCallbackUnregister(PresentCallback cb);

/* 键盘 */

// DWORD 键值, WORD 重复次数, BYTE 扫描码, BOOL 是否为扩展键, BOOL 是否与 Alt 键一起按下, BOOL 之前是否按下, BOOL 当前是否松开
typedef void(*KeyboardHandler)(DWORD, WORD, BYTE, BOOL, BOOL, BOOL, BOOL);

// 注册键盘事件处理程序
// 必须在 DLL 附加时调用
IMPORT void keyboardHandlerRegister(KeyboardHandler handler);

// 注销键盘事件处理程序
// 必须在 DLL 分离时调用
IMPORT void keyboardHandlerUnregister(KeyboardHandler handler);

/* 脚本 */

IMPORT void scriptWait(DWORD time);
IMPORT void scriptRegister(HMODULE module, void(*LP_SCRIPT_MAIN)());
IMPORT void scriptRegisterAdditionalThread(HMODULE module, void(*LP_SCRIPT_MAIN)());
IMPORT void scriptUnregister(HMODULE module);
IMPORT void scriptUnregister(void(*LP_SCRIPT_MAIN)()); // 已弃用

IMPORT void nativeInit(UINT64 hash);
IMPORT void nativePush64(UINT64 val);
IMPORT PUINT64 nativeCall();

static void WAIT(DWORD time){
	scriptWait(time);
}
static void TERMINATE(){
	WAIT(MAXDWORD);
}

// 返回全局变量的指针
// 在访问全局变量之前，请确保检查游戏版本，
// 因为不同补丁之间的 ID 可能不同
IMPORT UINT64 *getGlobalPtr(int globalId);

/* 世界 */

// 从内部池中获取实体
// 返回值表示填充数组元素的数量
// 只能在原生函数所在的线程中调用
IMPORT int worldGetAllVehicles(int *arr, int arrSize);
IMPORT int worldGetAllPeds(int *arr, int arrSize);
IMPORT int worldGetAllObjects(int *arr, int arrSize);
IMPORT int worldGetAllPickups(int *arr, int arrSize);

/* 杂项 */

// 使用脚本句柄返回基础对象指针
// 在访问对象字段之前，请确保检查游戏版本，
// 因为不同补丁之间的偏移量可能不同
IMPORT BYTE *getScriptHandleBaseAddress(int handle);
#

//https://github.com/pnwparksfan/gameconfig/blob/master/versions/2802/gameconfig.xml
enum eGameVersion: int{
	VER_1_0_335_2_STEAM,      // 00
	VER_1_0_335_2_NOSTEAM,    // 01

	VER_1_0_350_1_STEAM,      // 02
	VER_1_0_350_2_NOSTEAM,    // 03

	VER_1_0_372_2_STEAM,      // 04
	VER_1_0_372_2_NOSTEAM,    // 05

	VER_1_0_393_2_STEAM,      // 06
	VER_1_0_393_2_NOSTEAM,    // 07

	VER_1_0_393_4_STEAM,      // 08
	VER_1_0_393_4_NOSTEAM,    // 09

	VER_1_0_463_1_STEAM,      // 10
	VER_1_0_463_1_NOSTEAM,    // 11

	VER_1_0_505_2_STEAM,      // 12
	VER_1_0_505_2_NOSTEAM,    // 13

	VER_1_0_573_1_STEAM,      // 14
	VER_1_0_573_1_NOSTEAM,    // 15

	VER_1_0_617_1_STEAM,      // 16
	VER_1_0_617_1_NOSTEAM,    // 17

	VER_1_0_678_1_STEAM,      // 18
	VER_1_0_678_1_NOSTEAM,    // 19

	VER_1_0_757_2_STEAM,      // 20
	VER_1_0_757_2_NOSTEAM,    // 21

	VER_1_0_757_4_STEAM,      // 22
	VER_1_0_757_4_NOSTEAM,    // 23

	VER_1_0_791_2_STEAM,      // 24
	VER_1_0_791_2_NOSTEAM,    // 25

	VER_1_0_877_1_STEAM,      // 26
	VER_1_0_877_1_NOSTEAM,    // 27

	VER_1_0_944_2_STEAM,      // 28
	VER_1_0_944_2_NOSTEAM,    // 29

	VER_1_0_1011_1_STEAM,     // 30
	VER_1_0_1011_1_NOSTEAM,   // 31

	VER_1_0_1032_1_STEAM,     // 32
	VER_1_0_1032_1_NOSTEAM,   // 33

	VER_1_0_1103_2_STEAM,     // 34
	VER_1_0_1103_2_NOSTEAM,   // 35

	VER_1_0_1180_2_STEAM,     // 36
	VER_1_0_1180_2_NOSTEAM,   // 37

	VER_1_0_1290_1_STEAM,     // 38
	VER_1_0_1290_1_NOSTEAM,   // 39

	VER_1_0_1365_1_STEAM,	  // 40
	VER_1_0_1365_1_NOSTEAM,	  // 41

	VER_1_0_1493_0_STEAM,	  // 42
	VER_1_0_1493_0_NOSTEAM,	  // 43

	VER_1_0_1493_1_STEAM,	  // 44
	VER_1_0_1493_1_NOSTEAM,	  // 45

	VER_1_0_1604_0_STEAM,	  // 46
	VER_1_0_1604_0_NOSTEAM,	  // 47

	VER_1_0_1604_1_STEAM,	  // 48
	VER_1_0_1604_1_NOSTEAM,	  // 49

	VER_1_0_1734_0_STEAM,	  //50
	VER_1_0_1734_0_NOSTEAM,	  //51

	VER_1_0_1737_0_STEAM,	  //52
	VER_1_0_1737_0_NOSTEAM,	  //53

	VER_1_0_1868_0_STEAM,	  //54
	VER_1_0_1868_0_NOSTEAM,	  //55
	VER_1_0_1868_1_STEAM,	  //56
	VER_1_0_1868_1_NOSTEAM,	  //57

	VER_1_0_1868_4_EGS,		  //58
	
	VER_1_0_2060_0_STEAM,	  //59
	VER_1_0_2060_0_NOSTEAM,	  //60
	VER_1_0_2060_0_EGS,		  //61

	VER_1_0_2060_1_STEAM,	  //62
	VER_1_0_2060_1_NOSTEAM,	  //63
	VER_1_0_2060_1_EGS,		  //64

	VER_1_0_2189_0_STEAM,	  //65
	VER_1_0_2189_0_NOSTEAM,	  //66
	VER_1_0_2189_0_EGS,		  //67

	VER_1_0_2245_0_STEAM,	  //68
	VER_1_0_2245_0_NOSTEAM,	  //69
	VER_1_0_2245_0_EGS,		  //70

	VER_1_0_2372_0_STEAM,	  //71
	VER_1_0_2372_0_NOSTEAM,	  //72
	VER_1_0_2372_0_EGS,		  //73

	VER_1_0_2802_0_STEAM = 87,
	VER_1_0_2802_0_NOSTEAM,	  
	VER_1_0_2802_0_EGS,		 

	VER_1_0_2944_0_STEAM,
	VER_1_0_2944_0_NOSTEAM,	  
	VER_1_0_2944_0_EGS,	

	VER_1_0_3095_0_STEAM,
	VER_1_0_3095_0_NOSTEAM,
	VER_1_0_3095_0_EGS,

	VER_SIZE,
	VER_UNK = 255
};

IMPORT eGameVersion getGameVersion();
