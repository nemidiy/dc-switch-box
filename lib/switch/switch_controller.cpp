/*
Copyright 2019 Nicolas Emiliani

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "switch_controller.h"
#include <sstream>

using namespace dc::utils;

SwitchController::SwitchController(){
}

SwitchController::~SwitchController(){
}

bool SwitchController::add_switch(
        uint8_t toggle_pin,
        uint8_t relay_pin,
        const std::string& name){

    pinMode(toggle_pin, INPUT);
    pinMode(relay_pin, OUTPUT);
    digitalWrite(relay_pin, HIGH);

    auto conf = new switch_config;
    conf->relay_pin = relay_pin;
    conf->toggle_pin = toggle_pin;
    conf->on = false;
    conf->bounce.attach(toggle_pin);
    conf->bounce.interval(50);

    switches.insert(std::make_pair(name, conf));

    
    HomieNode* node = new HomieNode(name.c_str(), name.c_str(), "switch");
    nodes.insert(make_pair(name, node));

    auto handler = [conf, this, node](
            const HomieRange& range, const String& value) -> bool{
        Serial.print("switch handler ");
        uint8_t state = HIGH;
        if (value != "true" && value != "false")
          return false;
        bool on = (value == "true");
        if(on){
          Serial.println("\t-> true");
          conf->on = true;
          state = LOW;
        }else{
          Serial.println("\t-> false");
          conf->on = false;
          state = HIGH;
        }
        digitalWrite(conf->relay_pin, state);
        node->setProperty("on").send(value);
        return true;
    };

    node->advertise("on").settable(handler);
    
    return true;
}


void SwitchController::loop(){

    for (auto& kv: switches){
        auto conf = kv.second;
        conf->bounce.update();
        if(conf->bounce.rose()){
          conf->on = !conf->on;
          digitalWrite(
                  conf->relay_pin,
                  bool_to_state(conf->on));
          Serial.println(conf->relay_pin);
          nodes[kv.first]->setProperty("on") \
                  .send(bool_to_string(conf->on));
        }
    }

    // advertise status
    if (millis() - this->last_sent >= this->INTERVAL * 1000UL
            || this->last_sent == 0) {
        for(auto& kv: nodes){
            String status = "false";
            if(switches[kv.first]->on)
                status = "true";
            kv.second->setProperty("on").send(status);
        }
        this->last_sent = millis();
    }
    
}

String SwitchController::bool_to_string(bool state){
    if(state)
        return "true";
    return "false";
}

uint8_t SwitchController::bool_to_state(bool on){
    if(on)
        return LOW;
    return HIGH;
}