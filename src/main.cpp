#include <Arduino.h>

int PIN_x_axis_pulse_stepper_motor;
int PIN_x_axis_dir_stepper_motor;

int PIN_y_axis_pulse_stepper_motor;
int PIN_y_axis_dir_stepper_motor;

int time_step_motor_x = 0;
int time_step_motor_y = 0;

int freq_step_motor = 10000; // 10kHz

float pos_target_x, pos_current_x;
float pos_target_y, pos_current_y;

int mult_mmPerRevo = 2;   // ball screw pitch -mm-
int mult_step_main = 200; // step count for pulse/rev -motordriver-
int mult_step_micr = 32;  // MicroStep
int mult_step_total = mult_step_main * mult_step_micr;

bool dir_stepper_motor_x;
bool dir_stepper_motor_y;

void step_target_x(float value_x)
{
  pos_target_x = mult_step_total * value_x / mult_mmPerRevo;
}
void step_target_y(float value_y)
{
  pos_target_y = mult_step_total * value_y / mult_mmPerRevo;
}

bool is_at_target_x_axis_motor()
{
  if (pos_current_x = pos_target_x)
    return true;
  return false;
}
bool is_at_target_y_axis_motor()
{
  if (pos_current_y = pos_target_y)
    return true;
  return false;
}
void set_dir_stepper_motor_x()
{
  if (is_at_target_x_axis_motor)
  {
    dir_stepper_motor_x = true;
    digitalWrite(PIN_x_axis_dir_stepper_motor, HIGH);
  }
  else
  {
    dir_stepper_motor_x = false;
    digitalWrite(PIN_x_axis_dir_stepper_motor, LOW);
  }
}
void set_dir_stepper_motor_y()
{
  if (is_at_target_y_axis_motor)
  {
    dir_stepper_motor_y = true;
    digitalWrite(PIN_y_axis_dir_stepper_motor, HIGH);
  }
  else
  {
    dir_stepper_motor_y = false;
    digitalWrite(PIN_y_axis_dir_stepper_motor, LOW);
  }
}

void stepper_motor_x()
{
  if (dir_stepper_motor_x)
    pos_current_x += 1;
  else
    pos_current_x -= 1;
  digitalWrite(PIN_x_axis_pulse_stepper_motor, HIGH);
  delayMicroseconds(3);
  digitalWrite(PIN_x_axis_pulse_stepper_motor, LOW);
  time_step_motor_x = micros();
}

void stepper_motor_y()
{
  if (dir_stepper_motor_y)
    pos_current_y += 1;
  else
    pos_current_y -= 1;
  digitalWrite(PIN_y_axis_pulse_stepper_motor, HIGH);
  delayMicroseconds(3);
  digitalWrite(PIN_y_axis_pulse_stepper_motor, LOW);
  time_step_motor_y = micros();
}

void move_stepper_motor_x()
{
  if ((micros() - time_step_motor_x) >= (1000000 / freq_step_motor))
  {
    set_dir_stepper_motor_x();
    stepper_motor_x();
  }
}
void move_stepper_motor_y()
{
  if ((micros() - time_step_motor_y) >= (1000000 / freq_step_motor))
  {
    set_dir_stepper_motor_y();
    stepper_motor_y();
  }
}

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  step_target_x(100);
  move_stepper_motor_x();
  step_target_y(100);
  move_stepper_motor_y();
}