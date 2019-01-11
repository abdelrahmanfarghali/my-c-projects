#include <Windows.h>
#include <Xinput.h>
#include <iostream>
#include <math.h>
#include <ctime>
#include <cstdlib>

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30
#define INPUT_DEADZONE  ( 0.24f * FLOAT(0x7FFF) )

clock_t startTime = clock();

XINPUT_STATE state = state;

float LX = state.Gamepad.sThumbLX;
float LY = state.Gamepad.sThumbLY;

//determine how far the controller is pushed
float magnitudel = sqrt(LX*LX + LY*LY);

//determine the direction the controller is pushed
float normalizedLX = LX / magnitudel;
float normalizedLY = LY / magnitudel;

float normalizedMagnitude = 0;

float RX = state.Gamepad.sThumbRX;
float RY = state.Gamepad.sThumbRY;

float magnituder = sqrt(RX*RX + RY*RY);

float normalizedRX = RX / magnituder;
float normalizedRY = RY / magnituder;
//repeat for right thumb stick
XINPUT_VIBRATION vibration;
DWORD dwResult;
DWORD I;
void Vibration(){
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = 32000; // use any value between 0-65535 here
	vibration.wRightMotorSpeed = 32000; // use any value between 0-65535 here
	XInputSetState(I, &vibration);
	Sleep(1000);
	vibration.wLeftMotorSpeed = 0; // use any value between 0-65535 here
	vibration.wRightMotorSpeed = 0; // use any value between 0-65535 here
	XInputSetState(I, &vibration);
}
void deadzonecheck(){
	//check if the controller is outside a circular dead zone
	if (magnitudel > INPUT_DEADZONE)
	{
		//clip the magnitude at its expected maximum value
		if (magnitudel > 32767) magnitudel = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitudel -= INPUT_DEADZONE;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitudel / (32767 - INPUT_DEADZONE);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitudel = 0.0;
		normalizedMagnitude = 0.0;
	}
}
int main(){
	for (I = 0; I < XUSER_MAX_COUNT; I++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(I, &state);

		if (dwResult == ERROR_SUCCESS)
		{
			// Controller is connected
				Vibration();
				while (1){
					/*XINPUT_GAMEPAD_DPAD_UP	0x0001
						XINPUT_GAMEPAD_DPAD_DOWN	0x0002
						XINPUT_GAMEPAD_DPAD_LEFT	0x0004
						XINPUT_GAMEPAD_DPAD_RIGHT	0x0008
						XINPUT_GAMEPAD_START	0x0010
						XINPUT_GAMEPAD_BACK	0x0020
						XINPUT_GAMEPAD_LEFT_THUMB	0x0040
						XINPUT_GAMEPAD_RIGHT_THUMB	0x0080
						XINPUT_GAMEPAD_LEFT_SHOULDER	0x0100
						XINPUT_GAMEPAD_RIGHT_SHOULDER	0x0200
						XINPUT_GAMEPAD_A	0x1000
						XINPUT_GAMEPAD_B	0x2000
						XINPUT_GAMEPAD_X	0x4000
						XINPUT_GAMEPAD_Y*/
				}
		}
		else
		{
			// Controller is not connected
			std::cout << "Controller " << I << " not detected!" << std::endl;
		}
	}
	while (1){
		deadzonecheck();
	}
	return 0;
}