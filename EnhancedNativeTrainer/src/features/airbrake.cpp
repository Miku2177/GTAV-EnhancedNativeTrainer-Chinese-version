/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Sondai Smith and fellow contributors 2015
*/

#include "..\ui_support\menu_functions.h"
#include "airbrake.h"
#include "..\io\keyboard.h"
#include "..\io\config_io.h"
#include "..\utils.h"
#include "script.h"

bool exitFlag = false;

char* AIRBRAKE_ANIM_A = "amb@world_human_stand_impatient@male@no_sign@base";
char* AIRBRAKE_ANIM_B = "base";

int travelSpeed = 0;

Camera AirCam = NULL;

bool in_airbrake_mode = false;

bool frozen_time = false;
bool help_showing = true;
bool mouse_view_control = false;

Vector3 curLocation;
Vector3 curRotation;
float curHeading;

float rotationSpeed;
float forwardPush;

std::string airbrakeStatusLines[20];

DWORD airbrakeStatusTextDrawTicksMax;
bool airbrakeStatusTextGxtEntry;

void exit_airbrake_menu_if_showing()
{
	ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), true);
	CAM::RENDER_SCRIPT_CAMS(false, false, 0, false, false);
	CAM::DETACH_CAM(AirCam);
	CAM::SET_CAM_ACTIVE(AirCam, false);
	CAM::DESTROY_CAM(AirCam, true);
	AirCam = NULL;
	exitFlag = true;
}

//Test for airbrake command.
void process_airbrake_menu()
{
	exitFlag = false;

	const float lineWidth = 250.0;
	const int lineCount = 1;
	bool loadedAnims = false;

	std::string caption = "Airbrake Mode";

	//draw_menu_header_line(caption,350.0f,50.0f,15.0f,0.0f,15.0f,false);

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	bool inVehicle = PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) ? true : false;

	frozen_time = false;
	if (!inVehicle)
	{
		STREAMING::REQUEST_ANIM_DICT(AIRBRAKE_ANIM_A);
		while (!STREAMING::HAS_ANIM_DICT_LOADED(AIRBRAKE_ANIM_A))
		{
			make_periodic_feature_call();
			WAIT(0);
		}
		loadedAnims = true;
	}

	curLocation = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	curRotation = ENTITY::GET_ENTITY_ROTATION(playerPed, 0);
	curHeading = ENTITY::GET_ENTITY_HEADING(playerPed);

	while (true && !exitFlag)
	{
		in_airbrake_mode = true;

		// draw menu
		draw_menu_header_line(caption, 350.0f, 50.0f, 15.0f, 0.0f, 15.0f, false);
		//draw_menu_line(caption, lineWidth, 15.0, 18.0, 0.0, 5.0, false, true);

		make_periodic_feature_call();

		//Disable airbrake on death
		if (ENTITY::IS_ENTITY_DEAD(playerPed))
		{
			exitFlag = true;
		}
		else if (airbrake_switch_pressed())
		{
			menu_beep();
			ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), true);
			CAM::RENDER_SCRIPT_CAMS(false, false, 0, false, false);
			CAM::DETACH_CAM(AirCam);
			CAM::SET_CAM_ACTIVE(AirCam, false);
			CAM::DESTROY_CAM(AirCam, true);
			AirCam = NULL;
			break;
		}

		if (NETWORK::NETWORK_IS_GAME_IN_PROGRESS())
		{
			break;
		}

		airbrake(inVehicle);

		WAIT(0);
	}

	if (!inVehicle)
	{
		AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
	}

	exitFlag = false;
	in_airbrake_mode = false;
}

void update_airbrake_text()
{
	if (GetTickCount() < airbrakeStatusTextDrawTicksMax)
	{
		int numLines = sizeof(airbrakeStatusLines) / sizeof(airbrakeStatusLines[0]);

		float textY = 0.1f;

		int numActualLines = 0;
		for (int i = 0; i < numLines; i++)
		{
			if (!help_showing && i != 19)
			{
				continue;
			}

			numActualLines++;

			UI::SET_TEXT_FONT(0);
			UI::SET_TEXT_SCALE(0.3, 0.3);
			if (i == 0 || i == 9 || i == 15 || i == 19)
			{
				UI::SET_TEXT_OUTLINE();
				UI::SET_TEXT_COLOUR(255, 180, 0, 255);
			}
			else
			{
				UI::SET_TEXT_COLOUR(255, 255, 255, 255);
			}
			UI::SET_TEXT_WRAP(0.0, 1.0);
			UI::SET_TEXT_DROPSHADOW(1, 1, 1, 1, 1);
			UI::SET_TEXT_EDGE(1, 0, 0, 0, 305);
			if (airbrakeStatusTextGxtEntry)
			{
				UI::_SET_TEXT_ENTRY((char *)airbrakeStatusLines[i].c_str());
			}
			else
			{
				UI::_SET_TEXT_ENTRY("STRING");
				UI::_ADD_TEXT_COMPONENT_STRING((char *)airbrakeStatusLines[i].c_str());
			}
			UI::_DRAW_TEXT(0.01, textY);

			textY += 0.025f;
		}

		int screen_w, screen_h;
		GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);
		float rectWidthScaled = 350 / (float)screen_w;
		float rectHeightScaled = (20 + (numActualLines * 18)) / (float)screen_h;
		float rectXScaled = 0 / (float)screen_h;
		float rectYScaled = 65 / (float)screen_h;

		int rect_col[4] = { 128, 128, 128, 75.0f };

		// rect
		draw_rect(rectXScaled, rectYScaled,
			rectWidthScaled, rectHeightScaled,
			rect_col[0], rect_col[1], rect_col[2], rect_col[3]);
	}
}

void create_airbrake_help_text()
{
	//Debug
	std::stringstream ss;
		
	/*ss << "Heading: " << curHeading << " Rotation: " << curRotation.z
	<< "\n xVect: " << xVect << "yVect: " << yVect;*/

	std::string travelSpeedStr;
	switch (travelSpeed)
	{
	case 0:
		travelSpeedStr = "Slow";
		break;
	case 1:
		travelSpeedStr = "Fast";
		break;
	case 2:
		travelSpeedStr = "Very Fast";
		break;
	}

	ss << "Current Travel Speed: ~HUD_COLOUR_WHITE~" << travelSpeedStr;
	
	int index = 0;
	airbrakeStatusLines[index++] = "Default Airbrake Keys (change in XML):";
	airbrakeStatusLines[index++] = "Q/Z - Move Up/Down";
	airbrakeStatusLines[index++] = "A/D - Rotate Left/Right";
	airbrakeStatusLines[index++] = "W/S - Move Forward/Back";
	airbrakeStatusLines[index++] = "Space + A/D - Move Left/Right";
	airbrakeStatusLines[index++] = "Shift - Toggle Move Speed";
	airbrakeStatusLines[index++] = "T - Toggle Frozen Time";
	airbrakeStatusLines[index++] = "H - Toggle This Help";
	airbrakeStatusLines[index++] = " ";
	airbrakeStatusLines[index++] = "Default Controller Input (change in XML):";
	airbrakeStatusLines[index++] = "Triggers - Move Up/Down";
	airbrakeStatusLines[index++] = "Left Stick - Rotate, Move Forward/Back";
	airbrakeStatusLines[index++] = "A - Toggle Move Speed";
	airbrakeStatusLines[index++] = "B - Toggle Frozen Time";
	airbrakeStatusLines[index++] = " ";
	airbrakeStatusLines[index++] = "Mouse / Camera Controls (change in XML):";
	airbrakeStatusLines[index++] = "M - Toggle Mouse Control ON/OFF";
	airbrakeStatusLines[index++] = "Hold Space To Enable 'Move By Camera' Mode";
	airbrakeStatusLines[index++] = " ";
	airbrakeStatusLines[index++] = ss.str();

	airbrakeStatusTextDrawTicksMax = GetTickCount() + 2500;
	airbrakeStatusTextGxtEntry = false;
}

void moveThroughDoor()
{
	// common variables
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		return;
	}

	curLocation = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	curHeading = ENTITY::GET_ENTITY_HEADING(playerPed);

	float forwardPush = 0.6;

	float xVect = forwardPush * sin(degToRad(curHeading)) * -1.0f;
	float yVect = forwardPush * cos(degToRad(curHeading));

	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x + xVect, curLocation.y + yVect, curLocation.z, 1, 1, 1);
}

bool lshiftWasDown = false;

//////////////////////////////////////// CAMERA & MOUSE CODE ////////////////////////
void camera_view()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Entity target_player = playerPed;
	
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		target_player = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	}
	
	if (AirCam == NULL)
	{
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(target_player, true);
	curRotation = ENTITY::GET_ENTITY_ROTATION(target_player, 0);

	AirCam = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_FLY_CAMERA", playerPosition.x, playerPosition.y, playerPosition.z, curRotation.x, curRotation.y, curRotation.z, 50.0, true, 2);

	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		CAM::ATTACH_CAM_TO_ENTITY(AirCam, target_player, 0.0f, -0.01f, 0.0f, true);
		CAM::POINT_CAM_AT_ENTITY(AirCam, target_player, 0.0f, 0.0f, 0.0f, true);
		CAM::RENDER_SCRIPT_CAMS(true, false, 0, true, true);
		CAM::SET_CAM_ACTIVE(AirCam, true);
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), false);
	}
	else
	{
		CAM::DESTROY_CAM(AirCam, true);
		AirCam = NULL;
	}
	}
	else if (exitFlag == true)
	{
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), true);
		CAM::RENDER_SCRIPT_CAMS(false, false, 0, false, false);
		CAM::DETACH_CAM(AirCam);
		CAM::SET_CAM_ACTIVE(AirCam, false);
		CAM::DESTROY_CAM(AirCam, true);
		AirCam = NULL;
	}
}

void mouse_view() // Control By A Mouse
{
	// Mouse control variables
	POINT coord;
	int x_cur_coords;
	int y_cur_coords;
	int screen_width;
	int screen_height;
	int x_prev_coords;
	
	int body_rotation_up = -1;
	int body_rotation_down = -1;
	//		

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Entity target = playerPed;

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		target = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	}

	bool moveForwardKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_FORWARD) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_FORWARD);
	bool moveBackKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_BACK) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_BACK);
	bool SpaceKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_SPACE) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_SPACE);

	GetCursorPos(&coord);
	GRAPHICS::_GET_SCREEN_ACTIVE_RESOLUTION(&screen_width, &screen_height);
	x_cur_coords = coord.x;
	y_cur_coords = coord.y;
	x_prev_coords = screen_width / 2;

	if (x_cur_coords > x_prev_coords)
	{
		curHeading -= rotationSpeed;
		x_cur_coords = screen_width / 2;
		SetCursorPos(screen_width / 2, y_cur_coords);
	}

	if (x_cur_coords < x_prev_coords)
	{
		curHeading += rotationSpeed;
		x_cur_coords = screen_width / 2;
		SetCursorPos(screen_width / 2, y_cur_coords);
	}
	
	curRotation = ENTITY::GET_ENTITY_ROTATION(target, 0);
	
	body_rotation_up = (((screen_height - y_cur_coords) / 6) - 86);
	body_rotation_down = (86 - ((screen_height - y_cur_coords) / 6));

	if (body_rotation_up < 0) body_rotation_up = (body_rotation_up * -1);
	if (body_rotation_down < 0) body_rotation_down = (body_rotation_down * -1);
	
	if (y_cur_coords < (screen_height / 2)) //Head is raised
	{
		ENTITY::SET_ENTITY_ROTATION(target, (curRotation.x + body_rotation_up), (curRotation.y + body_rotation_up), curRotation.z, 2, true);
	}

	if (y_cur_coords > (screen_height / 2)) //Head is lowered
	{
		ENTITY::SET_ENTITY_ROTATION(target, (curRotation.x - body_rotation_down), (curRotation.y - body_rotation_down), curRotation.z, 2, true);
	}

	if ((moveForwardKey) && (y_cur_coords > ((screen_height / 2) + (screen_height / 4)) && !(SpaceKey)))//Head is lowered
	{
		curLocation.z -= (forwardPush * 1.1);
	}

	if ((moveForwardKey) && (y_cur_coords < ((screen_height / 2) - (screen_height / 10))) && !(SpaceKey)) //Head is raised
	{
		curLocation.z += (forwardPush * 1.1);
	}

	if ((moveBackKey) && (y_cur_coords > ((screen_height / 2) + (screen_height / 4)) && !(SpaceKey)))//Head is lowered
	{
		curLocation.z += (forwardPush * 1.1);
	}

	if ((moveBackKey) && (y_cur_coords < ((screen_height / 2) - (screen_height / 10))) && !(SpaceKey)) //Head is raised
	{
		curLocation.z -= (forwardPush * 1.1);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
void airbrake(bool inVehicle)
{
	// common variables
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	//float tmpHeading = curHeading += ;

	switch (travelSpeed)
	{
	case 0:
		rotationSpeed = 3.5f;
		forwardPush = 0.3f;
		break;
	case 1:
		rotationSpeed = 4.5f;
		forwardPush = 1.7f;
		break;
	case 2:
		rotationSpeed = 5.5f;
		forwardPush = 3.5f;
		break;
	}

	float xVect = forwardPush * sin(degToRad(curHeading)) * -1.0f;
	float yVect = forwardPush * cos(degToRad(curHeading));

	KeyInputConfig* keyConfig = get_config()->get_key_config();

	bool moveUpKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_UP) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_UP);
	bool moveDownKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_DOWN) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_DOWN);
	bool moveForwardKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_FORWARD) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_FORWARD);
	bool moveBackKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_BACK) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_BACK);
	bool rotateLeftKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_ROTATE_LEFT) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_ROTATE_LEFT);
	bool rotateRightKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_ROTATE_RIGHT) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_ROTATE_RIGHT);
	bool SpaceKey = IsKeyDown(KeyConfig::KEY_AIRBRAKE_SPACE) || IsControllerButtonDown(KeyConfig::KEY_AIRBRAKE_SPACE);

	//Airbrake controls vehicle if occupied
	Entity target = playerPed;
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		target = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	}

	BOOL xBoolParam = 1;
	BOOL yBoolParam = 1;
	BOOL zBoolParam = 1;

	ENTITY::SET_ENTITY_VELOCITY(target, 0.0f, 0.0f, 0.0f);
	ENTITY::SET_ENTITY_ROTATION(target, 0, 0, 0, 0, false);

	if (!inVehicle)
	{
		AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), AIRBRAKE_ANIM_A, AIRBRAKE_ANIM_B, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
	}

	if (IsKeyJustUp(KeyConfig::KEY_AIRBRAKE_SPEED) || IsControllerButtonJustUp(KeyConfig::KEY_AIRBRAKE_SPEED))
	{
		travelSpeed++;
		if (travelSpeed > 2)
		{
			travelSpeed = 0;
		}
	}

	if (IsKeyJustUp(KeyConfig::KEY_AIRBRAKE_FREEZE_TIME) || IsControllerButtonJustUp(KeyConfig::KEY_AIRBRAKE_FREEZE_TIME))
	{
		frozen_time = !frozen_time;
	}

	if (IsKeyJustUp(KeyConfig::KEY_AIRBRAKE_HELP) || IsControllerButtonJustUp(KeyConfig::KEY_AIRBRAKE_HELP))
	{
		help_showing = !help_showing;
	}

	if (IsKeyJustUp(KeyConfig::KEY_AIRBRAKE_MOUSE_CONTROL) || IsControllerButtonJustUp(KeyConfig::KEY_AIRBRAKE_MOUSE_CONTROL))
	{
		mouse_view_control = !mouse_view_control;
	}

	if (mouse_view_control) 
	{
		camera_view();
		mouse_view();
	}
	else
	{
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), true);
		CAM::RENDER_SCRIPT_CAMS(false, false, 0, false, false);
		CAM::DETACH_CAM(AirCam);
		CAM::SET_CAM_ACTIVE(AirCam, false);
		CAM::DESTROY_CAM(AirCam, true);
		AirCam = NULL;
	}


	create_airbrake_help_text();
	update_airbrake_text();

	if (moveUpKey)
	{
		curLocation.z += (forwardPush / 1.1);
	}
	else if (moveDownKey)
	{
		curLocation.z -= (forwardPush / 1.1);
	}

	if (moveForwardKey)
	{
		curLocation.x += xVect;
		curLocation.y += yVect;
	}
	else if (moveBackKey)
	{
		curLocation.x -= xVect;
		curLocation.y -= yVect;
	}

	if ((rotateLeftKey) && !(SpaceKey))
	{
		curHeading += rotationSpeed;
	}
	else if ((rotateRightKey) && !(SpaceKey))
	{
		curHeading -= rotationSpeed;
	}
	
	if ((rotateLeftKey) && (SpaceKey))
	{
		curLocation.x += (forwardPush * sin(degToRad(curHeading + 90)) * -1.0f);
		curLocation.y += (forwardPush * cos(degToRad(curHeading + 90)));
	}

	if ((rotateRightKey) && (SpaceKey))
	{
		curLocation.x += (forwardPush * sin(degToRad(curHeading - 90)) * -1.0f);
		curLocation.y += (forwardPush * cos(degToRad(curHeading - 90)));
	}

	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(target, curLocation.x, curLocation.y, curLocation.z, xBoolParam, yBoolParam, zBoolParam);
	ENTITY::SET_ENTITY_HEADING(target, curHeading - rotationSpeed);
}

bool is_in_airbrake_mode()
{
	return in_airbrake_mode;
}

bool is_airbrake_frozen_time()
{
	return frozen_time;
}

bool mouse_view_con()
{
	return mouse_view_control;
}

