#include "chickenFlap.h"

void chickenFlap_main(){
	chickenFlap_init();

	while(1)
		chickenFlap_update();
}

void chickenFlap_init(){
	button_init();
	display_init();
	flap_init();
	bms_init();
	stateMachine_init();
	timer_init();
	watch_init();
}

void chickenFlap_update(){
	stateMachine_update();
	if (stateMachine.state> STATE_INIT) {
		bms_update();
		watch_update();
		button_update();
		stateMachine_update();
		display_update();
	}
}
