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

#ifndef __DC_SWITCH_CONTROLLER_H__
#define __DC_SWITCH_CONTROLLER_H__

#include <FS.h>
#include <Homie.h>
#include <map>
#include <string>

namespace dc {

namespace utils {

struct SwitchController {

    SwitchController();

    virtual ~SwitchController();

    bool add_switch(
        uint8_t toggle_pin,
        uint8_t relay_pin,
        const std::string& name);

    void toggle_switch(const std::string& name);

    void loop();

protected :

    static String bool_to_string(bool state);
    static uint8_t bool_to_state(bool on);


    typedef struct {
        uint8_t toggle_pin; 
        uint8_t relay_pin; 
        bool on;
        Bounce bounce;
    } switch_config;

    std::map<std::string, HomieNode*> nodes;
    std::map<std::string, switch_config*> switches;

    unsigned long int last_sent = 0;
    unsigned long int INTERVAL = 10;
};

} // namespace utils

} // namespace dc
#endif
