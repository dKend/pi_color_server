#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>
#include "assertion.h"

#define RED_PIN 17
#define GREEN_PIN 22
#define BLUE_PIN 24

int main(){
	int pi = pigpio_start(NULL, NULL);
	int red, green, blue;
	//set all 3 pins to zero
	set_PWM_dutycycle(pi, RED_PIN, 0);
	set_PWM_dutycycle(pi, GREEN_PIN, 0);
	set_PWM_dutycycle(pi, BLUE_PIN, 0);

	red = 
	assertTrue(get_PWM_dutycycle(pi, RED_PIN) == 0, "get_PWM_dutycycle doesn't return zero when the pin is set to zero.");
	assertTrue(get_PWM_dutycycle(pi, GREEN_PIN) == 0, "get_PWM_dutycycle doesn't return zero when the pin is set to zero.");
	assertTrue(get_PWM_dutycycle(pi, BLUE_PIN) == 0, "get_PWM_dutycycle doesn't return zero when the pin is set to zero.");
	
	printf("Red value: %d\n", get_PWM_dutycycle(pi, RED_PIN));
	printf("Green value: %d\n", get_PWM_dutycycle(pi, GREEN_PIN));
	printf("Blue value: %d\n", get_PWM_dutycycle(pi, BLUE_PIN));
	
	set_PWM_dutycycle(pi, RED_PIN, 25);
	set_PWM_dutycycle(pi, GREEN_PIN, 5);
	set_PWM_dutycycle(pi, BLUE_PIN, 255);
	
	assertTrue(get_PWM_dutycycle(pi, RED_PIN) == 25, "get_PWM_dutycycle doesn't return the exact integer assigned to it with set_PWM_dutycycle.");
	assertTrue(get_PWM_dutycycle(pi, GREEN_PIN) == 5, "get_PWM_dutycycle doesn't return the exact integer assigned to it with set_PWM_dutycycle.");
	assertTrue(get_PWM_dutycycle(pi, BLUE_PIN) == 255, "get_PWM_dutycycle doesn't return the exact integer assigned to it with set_PWM_dutycycle.");
	
	printf("Red value: %d\n", get_PWM_dutycycle(pi, RED_PIN));
	printf("Green value: %d\n", get_PWM_dutycycle(pi, GREEN_PIN));
	printf("Blue value: %d\n", get_PWM_dutycycle(pi, BLUE_PIN));
	
	set_PWM_dutycycle(pi, RED_PIN, 0);
	set_PWM_dutycycle(pi, GREEN_PIN, 0);
	set_PWM_dutycycle(pi, BLUE_PIN, 0);
	
	
	pigpio_stop(pi);
	
	return 0;
}