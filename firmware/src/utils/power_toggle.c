#include <stdbool.h>
#include "globals.h"

void toggle_power(){
    if(is_system_on){
        is_system_on = false;

    } else {
        is_system_on = true;
    }
}