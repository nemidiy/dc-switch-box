#include <Homie.h>
#include <string>
#include <switch_controller.h>
#include <homie_handler.h>

dc::utils::SwitchController switch_controller;

std::string s5("S5");
std::string s6("S6");
std::string s7("S7");
std::string s8("S8");

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;

  switch_controller.add_switch(15, 26, s5);
  switch_controller.add_switch(2,  25, s6);
  switch_controller.add_switch(4,  33, s7);
  switch_controller.add_switch(18, 32, s8);

  Homie.onEvent(on_event);
  Homie_setFirmware("switch-box", "1.0.0");
  Homie.setup();

}

void loop() {
  Homie.loop();
  switch_controller.loop();
}
