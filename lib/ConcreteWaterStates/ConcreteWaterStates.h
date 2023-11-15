#pragma once
#include "Water.h"
#include "WaterState.h"

class Idle : public WaterState
{
public:
    void enter(Water *water);
    void execute(Water *water);
    void exit(Water *water);

    void buttonShortPress(Water *water);
    void buttonDoublePress(Water *water);
    void buttonLongPress(Water *water);

    void alarm1Interrupt(Water *water);
    void alarm2Interrupt(Water *water);

    static WaterState &getInstance();

private:
    Idle() {}
    Idle(const Idle &other);
};

class Watering : public WaterState
{
public:
    void enter(Water *water);
    void execute(Water *water);
    void exit(Water *water);

    void buttonShortPress(Water *water);
    void buttonDoublePress(Water *water);
    void buttonLongPress(Water *water);

    void alarm1Interrupt(Water *water);
    void alarm2Interrupt(Water *water);

    static WaterState &getInstance();

private:
    Watering() {}
    Watering(const Watering &other);
};

class LengthSetup : public WaterState
{
public:
    void enter(Water *water);
    void execute(Water *water);
    void exit(Water *water);

    void buttonShortPress(Water *water);
    void buttonDoublePress(Water *water);
    void buttonLongPress(Water *water);

    void alarm1Interrupt(Water *water);
    void alarm2Interrupt(Water *water);

    static WaterState &getInstance();

private:
    LengthSetup() {}
    LengthSetup(const LengthSetup &other);
};

class TemperatureSetup : public WaterState
{
public:
    void enter(Water *water);
    void execute(Water *water);
    void exit(Water *water);

    void buttonShortPress(Water *water);
    void buttonDoublePress(Water *water);
    void buttonLongPress(Water *water);

    void alarm1Interrupt(Water *water);
    void alarm2Interrupt(Water *water);

    static WaterState &getInstance();

private:
    TemperatureSetup() {}
    TemperatureSetup(const TemperatureSetup &other);
};

class RainDetected : public WaterState
{
public:
    void enter(Water *water);
    void execute(Water *water);
    void exit(Water *water);

    void buttonShortPress(Water *water);
    void buttonDoublePress(Water *water);
    void buttonLongPress(Water *water);

    void alarm1Interrupt(Water *water);
    void alarm2Interrupt(Water *water);

    static WaterState &getInstance();

private:
    RainDetected() {}
    RainDetected(const RainDetected &other);
};