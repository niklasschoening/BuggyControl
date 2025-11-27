#include <Arduino.h>
#include <unity.h>
#include <BuggyControl.h>

// Unit-Tests können hier hinzugefügt werden
void test_motor_initialization() {
    Motor motor(13, 12, 14, 27, 100, 30, 1000, 10000);
    TEST_ASSERT_EQUAL(0, motor.getCurrentDuty());
}

void setup() {
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_motor_initialization);
    UNITY_END();
}

void loop() {
    // Nichts zu tun
}
