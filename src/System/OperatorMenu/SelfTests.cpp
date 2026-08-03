#include "SelfTests.h"

#include "OperatorMenu.h"
#include "System/Lamps/LampsHelper.h"
#include "System/RPU.h"
#include "System/Sound/Sound.h"

static OperatorMenu* operatorMenu = nullptr;
static uint8_t       switchHit    = 0;

void testLamps() {
    static uint8_t lampInTest = 0xFF;

    if (operatorMenu->hasSubMenuChanged()) {
        lampInTest = 0xFF;
        operatorMenu->setDisplays(lampInTest);
        LampsHelper::showAllLamps(500);
    }

    if (switchHit == operatorMenu->enterSwitch) {
        lampInTest++;
        if (lampInTest >= RPU_MAX_LAMPS) lampInTest = 0;
        operatorMenu->setDisplays(lampInTest);
        LampsHelper::hideAllLamps();
        LampsHelper::setLampState(lampInTest, true, 0, 255);
    }
}
void testDisplays() {
    static Time::time_t lastDisplayTestTime = 0;
    static uint8_t      digitInTest         = 0;
    static uint8_t      numberToDisplay     = 0;

    if (operatorMenu->hasSubMenuChanged()) {
        lastDisplayTestTime = Time::getCurrentTime();
        digitInTest         = 0xFF;
    }

    if (switchHit == operatorMenu->enterSwitch) {
        digitInTest++;
        if (digitInTest > (RPU_OS_NUMBER_OF_DISPLAYS + 1) * RPU_OS_NUM_DIGITS) digitInTest = 0xFF;
    }

    if (Time::getCurrentTime() - lastDisplayTestTime > 500) {
        lastDisplayTestTime = Time::getCurrentTime();
        numberToDisplay++;
        if (numberToDisplay >= 10) numberToDisplay = 0;

        if (digitInTest == 0xFF) {
            for (byte count = 0; count < RPU_OS_NUMBER_OF_DISPLAYS; count++) {
                // TODO : 7 Digit display Test
                RPU_SetDisplay(count, (RPU_OS_NUM_DIGITS == 7 ? 1111111 : 111111) * numberToDisplay, false, RPU_OS_NUM_DIGITS);
                RPU_SetDisplayBlank(count, 0xFF);
            }
        } else {
            uint8_t displayNumber      = (RPU_OS_NUMBER_OF_DISPLAYS * RPU_OS_NUM_DIGITS) / digitInTest;
            uint8_t displayDigitInTest = (RPU_OS_NUMBER_OF_DISPLAYS * RPU_OS_NUM_DIGITS) % digitInTest;
            for (byte i = 0; i < RPU_OS_NUMBER_OF_DISPLAYS; i++) {
                if (i == displayNumber) {
                    RPU_SetDisplay(displayNumber, numberToDisplay * pow(10, displayDigitInTest), false, 0);
                    RPU_SetDisplayBlank(displayNumber, 1 << displayDigitInTest);
                } else RPU_SetDisplayBlank(i, 0);
            }
        }
    }
}
void testSolenoids() {
    static uint8_t      solenoidInTest       = 0xFF;
    static bool         keepSolenoidIndex    = false;
    static Time::time_t lastSolenoidTestTime = 0;

    if (operatorMenu->hasSubMenuChanged()) {
        solenoidInTest       = 0xFF;
        keepSolenoidIndex    = false;
        lastSolenoidTestTime = Time::getCurrentTime();
    }

    if (switchHit == operatorMenu->enterSwitch) keepSolenoidIndex = !keepSolenoidIndex;

    if (Time::getCurrentTime() - lastSolenoidTestTime > 1000) {
        lastSolenoidTestTime = Time::getCurrentTime();

        if (!keepSolenoidIndex) {
            solenoidInTest++;
            if (solenoidInTest > (RPU_MPU_ARCHITECTURE < 10 ? 15 : 22)) solenoidInTest = 0;
        }

        operatorMenu->setDisplays(solenoidInTest);
        RPU_PushToSolenoidStack(solenoidInTest, operatorMenu->solenoidTestStrength, true);
    }
}
void testSwitches() {
    uint8_t switchTestDisplay = 0;
    for (byte switchCount = 0; switchCount < 64 && switchTestDisplay < 4; switchCount++) {
        if (RPU_ReadSingleSwitchState(switchCount)) {
            RPU_SetDisplay(switchTestDisplay, switchCount, true);
            switchTestDisplay += 1;
        }
    }
    if (switchTestDisplay < 4) {
        for (int count = switchTestDisplay; count < 4; count++) {
            RPU_SetDisplayBlank(count, 0x00);
        }
    }
}
void testSound() {
    static uint8_t soundIndex = 0;

    if (operatorMenu->hasSubMenuChanged()) {
        soundIndex = 0xFF;
        operatorMenu->setDisplays(0);
    }

    if (switchHit == operatorMenu->enterSwitch) {
        soundIndex++;
        operatorMenu->setDisplays(soundIndex);
        SoundHelper::playSoundEffect(soundIndex, AUDIO_DASH51);
    }
}

void SelfTests::init(OperatorMenu* _operatorMenu) {
    operatorMenu = _operatorMenu;
}
void SelfTests::run() {
    switchHit = SWITCH_STACK_EMPTY;

    switch (operatorMenu->getSubMenu()) {
    case 0:
        testLamps();
        break;
    case 1:
        testDisplays();
        break;
    case 2:
        testSolenoids();
        break;
    case 3:
        testSwitches();
        break;
    case 4:
        testSound();
        break;
    default:
        break;
    }
}
void SelfTests::handleSwitchHit(const uint8_t _switchHit) {
    switchHit = _switchHit;
    switch (operatorMenu->getSubMenu()) {
    case 0:
        testLamps();
        break;
    case 1:
        testDisplays();
        break;
    case 2:
        testSolenoids();
        break;
    case 3:
        testSwitches();
        break;
    case 4:
        testSound();
        break;
    default:
        break;
    }
}
