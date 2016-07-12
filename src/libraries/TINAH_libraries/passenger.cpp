/*
 * Passenger.cpp - Library for detecting passengers.
 */

#include <passenger.h>
#include <phys253.h>
#include <Arduino.h>

Passenger::Passenger(uint8_t leftIR1, uint8_t leftIR2, uint8_t rightIR1, uint8_t rightIR2)
{
    _leftIR1 = leftIR1;
    _leftIR2 = leftIR2;
    _rightIR1 = rightIR1;
    _rightIR2 = rightIR2;
    
    _threshold = 100;
    _precise = 300;
}

uint8_t Passenger::detect(void)
{
    _left = analogRead(_leftIR1);
    _right = analogRead(_rightIR1);
    if (_left > _threshold) {
        return 1;
    }
    else if (_right > _threshold) {
        return 2;
    }
    else return 0;
}

uint8_t Passenger::precise(void)
{
    _left = analogRead(_leftIR2);
    _right = analogRead(_rightIR2);
    if (_left > _precise) {
        return 1;
    }
    else if (_right > _precise) {
        return 2;
    }
    else return 0;
}