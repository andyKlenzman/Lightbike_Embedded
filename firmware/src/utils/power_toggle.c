#include <stdbool.h>
#include "filter_handler//filter_handler.h"



void toggle_power(){
    static bool power_on = true;

    if(power_on){
        select_state(MODE_OFF);

    } else {
        select_state(MODE_BASIC);
    }
    power_on = !power_on;



}