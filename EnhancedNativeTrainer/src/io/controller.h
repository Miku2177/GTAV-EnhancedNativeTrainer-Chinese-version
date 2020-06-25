#pragma once

#include <windows.h>
#include "..\io\config_io.h"

bool IsControllerButtonDown(int inputGroup, int buttonIndex);

bool IsControllerButtonJustUp(int inputGroup, int buttonIndex);