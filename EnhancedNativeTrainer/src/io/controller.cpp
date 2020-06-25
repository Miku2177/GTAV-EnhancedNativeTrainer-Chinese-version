#include "..\io\keyboard.h"
#include "..\features\script.h"
#include "..\debug\debuglog.h"
#include "..\io\controller.h"

bool IsControllerButtonDown(int inputGroup, int buttonIndex)
{
	if (is_controller_ignored_in_trainer())
		return false;

	if (CONTROLS::IS_CONTROL_JUST_PRESSED(inputGroup, buttonIndex))
	{
		set_status_text("Control just pressed!!");
		return true;
	}

	return false;
}

bool IsControllerButtonJustUp(int inputGroup, int buttonIndex)
{
	if (is_controller_ignored_in_trainer())
		return false;

	if (CONTROLS::IS_CONTROL_JUST_RELEASED(inputGroup, buttonIndex))
	{
		set_status_text("Control just released!");
		return true;
	}

	return false;
}