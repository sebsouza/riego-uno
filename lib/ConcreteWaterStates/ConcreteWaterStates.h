#pragma once
#include "Water.h"
#include "WaterState.h"

class WaterOff : public WaterState
{
public:
    void enter(Water *water);
    void execute(Water *water);
    void exit(Water *water);

    void buttonShortPress(Water *water);
    void buttonLongPress(Water *water);

    static WaterState &getInstance();

private:
    WaterOff() {}
    WaterOff(const WaterOff &other);
};

class WaterOn : public WaterState
{
public:
    void enter(Water *water);
    void execute(Water *water);
    void exit(Water *water);

    void buttonShortPress(Water *water);
    void buttonLongPress(Water *water);

    static WaterState &getInstance();

private:
    WaterOn() {}
    WaterOn(const WaterOn &other);
};

class WaterConfig : public WaterState
{
public:
    void enter(Water *water);
    void execute(Water *water);
    void exit(Water *water);

    void buttonShortPress(Water *water);
    void buttonLongPress(Water *water);

    static WaterState &getInstance();

private:
    WaterConfig() {}
    WaterConfig(const WaterConfig &other);
};