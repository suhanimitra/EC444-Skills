#define SWIPECARD 0		// Define each state
#define PUMPINGGAS 1
#define TRANSACTIONCOMPLETE 2

void StateMachine(){
     int state = SWIPECARD;	// Initial state 
     while (1)
    {
     	switch (state){
		SWIPECARD:
			if (CARD_DECLINED) {state = swipe_card}
			if (SUCCESS) {state = pumping_gas}
            if (TANK_FULL) {state = error_handler}
			break;
		PUMPINGGAS:
            if (CARD_DECLINED) {state = error_handler}
            if (SUCCESS) {state = error_handler}
			if (TANK FULL) {state = transaction_complete}
			break;
		TRANSACTIONCOMPLETE:
			if (CARD_DECLINED) {state = error_handler}
            if (SUCCESS) {state = error_handler}
			if (TANK FULL) {state = error_handler}
			break;
	    }
    }
}