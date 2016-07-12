/*
 * Drive.cpp - Library for tape-following
 * Created by Rahat Dhande, July 10, 2016.
 */
 

#include <drive.h>
#include <phys253.h>
#include <LiquidCrystal.h>
#include <Arduino.h>

Drive::Drive(uint8_t m1, uint8_t m2, uint8_t qrd1, uint8_t qrd2, uint8_t qrd3, uint8_t qrd4)
{
    _m1 = m1;
    _m2 = m2;
    
    _qrd1 = qrd1;
    _qrd2 = qrd2;
    _qrd3 = qrd3;
    _qrd4 = qrd4;
    
    _speed = 100;
    _turnSpeed = 50;
    
    _threshold = 100;
    
    _kp = 40;
    _kd = 70;
    
    _p = 0;
    _d = 0;
    _correction = 0;
    
    _error = 0;
    _lastError = 0;
    _recentError = 0;
    
    _c = 0;
    _q = 0;
    _m = 0;
}

void Drive::setPD(uint8_t kp, uint8_t kd)
{
    _kp = kp;
    _kd = kd;
}

void Drive::straight()
{
    _left = analogRead(_qrd1);
    _right = analogRead(_qrd2);
    
    if (_left > _threshold) {
        if (_right > _threshold) _error = 0;
        else _error = -1;
    }
    else {
        if (_right > _threshold) _error = 1;
        else if (_lastError > 0) _error = 5;
        else _error = -5;
    }
    
    if (_error != _lastError) {
        _recentError = _lastError;
        _q = _m;
        _m = 1;
    }
    
    _p = _kp * _error;
    _d = _kd * (_error - _recentError) / (float) (_q + _m);
    _correction = _p + _d;
    
    if (_c == 30) {
        LCD.clear();
        LCD.home();
        LCD.print("ol: ");
        LCD.print(analogRead(_qrd3));
        LCD.print("  or: ");
        LCD.print(analogRead(_qrd4));
        LCD.setCursor(0,1);
        LCD.print("l: ");
        LCD.print(_left);
        LCD.print("  r: ");
        LCD.print(_right);
        _c = 0;
    }
    
    ++_c;
    ++_m;
    
    motor.speed(_m1, _speed + _correction);
    motor.speed(_m2, _speed - _correction);
    _lastError = _error;
}

void Drive::left()
{
    while (analogRead(_qrd3) < _threshold) {
        motor.speed(_m1, -_turnSpeed);
        motor.speed(_m2, _turnSpeed);
    }
}

void Drive::right()
{
    while (analogRead(_qrd4) < _threshold) {
        motor.speed(_m1, _turnSpeed);
        motor.speed(_m2, -_turnSpeed);
    }
    
}

void Drive::reverse()
{
    // TODO: test if it works
    _left = analogRead(_qrd1);
    _right = analogRead(_qrd2);
    
    if (_left > _threshold) {
        if (_right > _threshold) _error = 0;
        else _error = -1;
    }
    else {
        if (_right > _threshold) _error = 1;
        else if (_lastError > 0) _error = 5;
        else _error = -5;
    }
    
    if (_error != _lastError) {
        _recentError = _lastError;
        _q = _m;
        _m = 1;
    }
    
    _p = _kp * _error;
    _d = _kd * (_error - _recentError) / (float) (_q + _m);
    _correction = _p + _d;
    
    if (_c == 30) {
        LCD.clear();
        LCD.home();
        LCD.print("kp: ");
        LCD.print(_kp);
        LCD.print("  kd: ");
        LCD.print(_kd);
        LCD.setCursor(0,1);
        LCD.print("l: ");
        LCD.print(_left);
        LCD.print("  r: ");
        LCD.print(_right);
        _c = 0;
    }
    
    ++_c;
    ++_m;
    
    motor.speed(_m1, -1 * _speed - _correction);
    motor.speed(_m2, -1 * _speed + _correction);
    _lastError = _error;
}

boolean Drive::intersection()
{
    return (analogRead(_qrd3) > _threshold || analogRead(_qrd4) > _threshold);
}

void Drive::brake()
{
    motor.stop_all();
}

void Drive::stats()
{
    LCD.clear();
    LCD.home();
    LCD.print("kp: ");
    LCD.print(_kp);
    LCD.print("  kd: ");
    LCD.print(_kd);
    LCD.setCursor(0,1);
    LCD.print("l: ");
    LCD.print(_left);
    LCD.print("  r: ");
    LCD.print(_right);
}