#include "CXBOXController.h"

CXBOXController::CXBOXController(int playerNumber)
{
	// 设置控制器编号
	_controllerNum = playerNumber - 1;
}

XINPUT_STATE CXBOXController::GetState()
{
	// 将状态清零
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	// 获取状态
	XInputGetState(_controllerNum, &_controllerState);

	return _controllerState;
}

bool CXBOXController::IsConnected()
{
	// 将状态清零
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	// 获取状态
	DWORD Result = XInputGetState(_controllerNum, &_controllerState);

	if(Result == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CXBOXController::Vibrate(int leftVal, int rightVal)
{
	// 创建震动状态
	XINPUT_VIBRATION Vibration;

	// 将震动清零
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	// 设置震动值
	Vibration.wLeftMotorSpeed = leftVal;
	Vibration.wRightMotorSpeed = rightVal;

	// 震动控制器
	XInputSetState(_controllerNum, &Vibration);
}