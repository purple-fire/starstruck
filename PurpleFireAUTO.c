#pragma config(Sensor, in1,    POT,            sensorPotentiometer)
#pragma config(Sensor, in2,    POTCLAW,        sensorPotentiometer)
#pragma config(Motor,  port1,           BL,            tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           RARMA,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           RARMB,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           LARMA,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           LARMB,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           FR,            tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           FL,            tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           CLAW1,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           CLAW2,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          BR,            tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// Controller 1/2, Stick L/R, Axis X/Y
#define C1LX vexRT[Ch4]
#define C1LY vexRT[Ch3]
#define C1RX vexRT[Ch1]
#define ARMUP vexRT[Btn6U]
#define ARMDOWN vexRT[ChBtn6D]

#define downPOS 4095
#define upPOS 3483
#define chuckPOS 2355
#define pushPOS 3150

#define clawOPEN 1548
#define clawCLOSE 270

int autoPilotActive = 0;
int armMode = 0;
int clawMode = 0;
float armTarget;
float clawTarget;

//Task to Manage Claw Position
task claw()
{
	clawTarget = clawCLOSE;
	float proportionalCoefficient = 0.5;
	float error,
	motorPower,
	proportional; //what you set the motors to
	while (true)
	{
		//ENABLE PID
		if(clawMode==1){
			error = clawTarget - SensorValue[POTCLAW];
			proportional = error * proportionalCoefficient;
			motorPower = proportional;
			motor[CLAW1] = -motorPower;
			motor[CLAW2] = -motorPower;
		}
		//DISABLE PID
		else if(clawMode==0){
			//Claw Open
			if(vexRT[Btn8U] == 1)
			{
				motor[CLAW1] = 75;
				motor[CLAW2] = 75;
			}
			//Claw Close
			else if(vexRT[Btn8D] == 1)
			{
				motor[CLAW1] = -75;
				motor[CLAW2] = -75;
			}
			//Claw Stop
			else{
				clawTarget = SensorValue[POTCLAW];
				clawMode = 1;
			}
			// Motor values can only be updated every 20ms
			wait1Msec(20);
		}
	}
}

//Task to Manage Arm Position
task runArm()
{
	armTarget = downPOS;
	float proportionalCoefficient = 0.5;
	float error,
	motorPower,
	proportional; //what you set the motors to
	while (true)
	{
		//ENABLE PID
		if(armMode==1){
			error = armTarget - SensorValue[POT];
			proportional = error * proportionalCoefficient;
			motorPower = proportional;
			motor[RARMA] = motorPower;
			motor[RARMB] = motorPower;
			motor[LARMA] = motorPower;
			motor[LARMB] = motorPower;
		}
		//DISABLE PID
		else if(armMode==0){
			//Arm UP
			if(vexRT[Btn5U] == 1)
			{
				//Soft Limit For Going UP
				if(SensorValue[POT]>chuckPOS){
					motor[RARMA] = -127;
					motor[RARMB] = -127;
					motor[LARMA] = -127;
					motor[LARMB] = -127;
				}
				else{
					motor[RARMA] = 0;
					motor[RARMB] = 0;
					motor[LARMA] = 0;
					motor[LARMB] = 0;
				}
			}
			//Arm Down
			else if(vexRT[Btn5D] == 1)
			{
				//Soft Limit For Going DOWN
				if(SensorValue[POT]<downPOS){
					motor[RARMA] = 50;
					motor[RARMB] = 50;
					motor[LARMA] = 50;
					motor[LARMB] = 50;
				}
				else{
					motor[RARMA] = 0;
					motor[RARMB] = 0;
					motor[LARMA] = 0;
					motor[LARMB] = 0;
				}
			}
			//Arm Stop
			else{
				armTarget = SensorValue[POT];
				armMode = 1;
			}
			// Motor values can only be updated every 20ms
			wait1Msec(20);
		}
	}
}

//Holonomic Drive Forward using speed Input
void holonomicForward(int speed, float seconds)
{
	// Y component, X component, Rotation
	motor[FL] = -speed;
	motor[FR] =  speed;
	motor[BR] =  speed;
	motor[BL] = -speed;
	wait1Msec(seconds*1000);
}

//Holonomic Drive Backward using speed Input
void holonomicBackward(int speed, float seconds)
{
	// Y component, X component, Rotation
	motor[FL] =  speed;
	motor[FR] = -speed;
	motor[BR] = -speed;
	motor[BL] =  speed;
	wait1Msec(seconds*1000);
}

//Holonomic Drive Left using speed Input
void holonomicRight(int speed, float seconds)
{
	// Y component, X component, Rotation
	motor[FL] = -speed;
	motor[FR] = -speed;
	motor[BR] = speed;
	motor[BL] = speed;
	wait1Msec(seconds*1000);
}

//Holonomic Drive Right using speed Input
void holonomicLeft(int speed, float seconds)
{
	// Y component, X component, Rotation
	motor[FL] = speed;
	motor[FR] = speed;
	motor[BR] = -speed;
	motor[BL] = -speed;
	wait1Msec(seconds*1000);
}

//Holonomic Drive Rotate Left using speed Input
void holonomicRotateLeft(int speed,float seconds)
{
	// Y component, X component, Rotation
	motor[FL] = speed;
	motor[FR] = speed;
	motor[BR] = speed;
	motor[BL] = speed;
	wait1Msec(seconds*1000);
}

//Holonomic Drive Rotate Right using speed Input
void holonomicRotateRight(int speed, float seconds)
{
	// Y component, X component, Rotation
	motor[FL] = -speed;
	motor[FR] = -speed;
	motor[BR] = -speed;
	motor[BL] = -speed;
	wait1Msec(seconds*1000);
}

//Stop All Motors
void holonomicStop()
{
	motor[FL] = 0;
	motor[FR] = 0;
	motor[BR] = 0;
	motor[BL] = 0;
}

task main()
{
	armMode = 1;
	clawMode = 1;
	//Start Arm/Claw Management Task
	startTask(runArm);
	startTask(claw);

	//Push Off Right Stars
	armTarget = downPOS;
	clawTarget = clawCLOSE+75;
	wait1Msec(800);
	armTarget = pushPOS;
	clawTarget = clawOPEN;
	wait1Msec(1000);
	holonomicForward(110,1);
	holonomicRight(40,.7);
	holonomicRotateLeft(25,.25);
	holonomicForward(115,.55);
	armTarget = pushPOS-400;

	//Go Get Stars
	wait1Msec(500);
	holonomicBackward(110,2);
	holonomicStop();
	wait1Msec(1500);
	clawTarget = clawCLOSE;
	wait1Msec(500);
	holonomicForward(110,1.45);
	clawTarget = clawOPEN;

	//Go Get Cube 1
	wait1Msec(1000);
	holonomicBackward(110,2);
	holonomicStop();
	wait1Msec(1000);
	clawTarget = clawCLOSE;
	wait1Msec(500);
	holonomicForward(110,1.45);
	clawTarget = clawOPEN;

	//Go Get Cube 2
	wait1Msec(500);
	holonomicBackward(110,2);
	holonomicStop();
	wait1Msec(1000);
	clawTarget = clawCLOSE;
	wait1Msec(500);
	holonomicForward(110,1.45);
	clawTarget = clawOPEN;

	//Push second fence jacks
	holonomicBackward(110, 0.5);
	holonomicStop();
	wait1Msec(1000);
	holonomicRotateLeft(25,0.5);
	holonomicLeft(110,1.5);
	holonomicRotateRight(25,0.25);

	//Stop Holonomic
	holonomicStop();

	//DONE!
}
