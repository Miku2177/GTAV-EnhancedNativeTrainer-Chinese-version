#ifndef _XBOX_CONTROLLER_H_
#define _XBOX_CONTROLLER_H_

// 需要Windows头文件和XInput头文件
#include <windows.h>
#include <Xinput.h>

// 现在，是XInput库
// 注意：如果你使用的编译器不支持这种链接库的方法，请注释掉这一行
#pragma comment(lib, "XInput9_1_0.lib")

// XBOX 控制器类定义
class CXBOXController
{
private:
	XINPUT_STATE _controllerState;
	int _controllerNum;
public:
	CXBOXController(int playerNumber);
	XINPUT_STATE GetState();
	bool IsConnected();
	void Vibrate(int leftVal = 0, int rightVal = 0);
};

#endif