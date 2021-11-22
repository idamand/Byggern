#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//#include <until/delay.h>
#include <time.h>
#include "oled_driver.h"

#include "adc_driver.h" // needed in the menu loop that responds to the joystick

// content of example menu 
// line 1
// -> a, b, c, d
// line 2
// line 3
// -> 1,2,3, 4

//each innermost menu-item has a corresponding identifying number. a->0, ... d->3, 1->4 ... 4->7  
//NB: this is not respected by menu handler at the moment.

//char outer_menu[3][8] = {"line1", "line2","line3"};
char outer_menu[3][8] = {"play", "play","play yes"};
char submenues[3][4][8] = {{"a","b","c","d"},{}, {"1","2","3","4"}};
char current_menu[6][8];  // has to  be large enough to store all other submenues

int menu_depth = 0; // 0 or 1 since we only have implmented one level of submenues
int menu_number =0; // which of the  submenues are we in, if menu_depth == 0 this number is irrelevant 
//that is, we find the current menu by submenues[menu_number][x][x]

int current_pos = 0; // the current selection of the joystick in the submenu 
int previous_pos =0; // in order to not update more lines than neecessary, but that is just a neat extre feauture to  think of later.
int current_submenu = 0; // 0 is main menu, then they are indexed increasingly // should possibly be a pointer to the submenues, and then dereference that

/*
void menu_oled_clear_screen(){
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}*/

// loop for handling the menu, needs to be called at regular intervals for the menu to function
int menu_loop(){
	if (adc_joystick_direction() == DOWN)
	{
		menu_go_down();
	}
	if (adc_joystick_direction() == UP){
		menu_go_up();
	}
	if (button_read(0)){ //read right button 
		if (menu_depth=0){
			menu_go_in();
		}
		else{
			return current_pos; // should be a more correct value that also takes into acccount submenu
		}
	}
	return -1; 
}

// will write the current subemnu to the screen, and highlight the current selection 
void oled_write_menu_to_screen(){
	oled_clear_screen();
	//menu_oled_clear_screen();

	// this is necessary in order to understand how to structure the menu
	// currently it will just output stuff to the console
	//for (int i = 0; i < sizeof(submenues[0])/sizeof(submenues[0][0]); ++i){
		
	for (int i = 0; i < sizeof(current_menu)/sizeof(current_menu[0]); ++i){		
	//for (int i = 0; i < sizeof(outer_menu)/sizeof(outer_menu[0]); ++i){	
		if(i==current_pos){
			printf("- ");
		}
		//printf(current_menu[i]); oled_newline();
		printf("%s", current_menu[i]); oled_newline();
	}
	//printf("%s\r\n", entire_menu[0]);
}

// this will copy the correct array into current_menu
// will also call oled_write_menu_to_secreen
// might bee moree elegant with pointers, but then one would have to handle length of array
// would also be more eleegant to update it with passing the deepth and index as argument
// todo: must check wether the length of submenu is zero?
void update_current_menu(){
	if(menu_depth==0){ // current menu is outermost menu
		for(int i=0; i< sizeof(outer_menu)/sizeof(outer_menu[1]); i++){
			strcpy(current_menu[i], outer_menu[i]);
		}
		
	}
	else if (menu_depth == 1)
	{
		for(int i=0; i< sizeof(submenues[menu_number])/sizeof(submenues[menu_number][0]); i++){
			strcpy(current_menu[i], submenues[menu_number][i]);
		}
	}
	else{
		printf("illegal menu depth in updating menu");
	}
	oled_write_menu_to_screen();
}


// should  have check to see if it is out of bounds
void menu_go_down(){
	current_pos = current_pos +1;
	oled_write_menu_to_screen();
}

// should  have check to see if it is out of bounds
void menu_go_up(){
	current_pos = current_pos - 1;
	oled_write_menu_to_screen();
}

// goes in one submenu
void menu_go_in(){
	if (menu_depth == 0){
		menu_depth = 1;
		update_current_menu(); // will also call oled_write_menu_to_screen();
	}
	else {
		printf("cannot go deeper than one submenu layer");
	}
}

// goes out to the menu outside this submenu
void menu_go_out(){
	if(menu_depth == 1){
		menu_depth = 0;
		//current_pos = 0;
		update_current_menu(); // will also call oled_write_menu_to_screen();
	}
	else {
		printf("cannot go to lower menu deepth than main menu");
	}
}

// this gets called whenever the click button is pressed
//NB: unfinished
void click_button_respond(){
	// if  it is on an item which has a submenu then call menu go in
	// todo: add check to se wether there is a submenu, and then call menu_go in

	// return the current selection?
}

// should be of custom type menu-item
// returns when it gets  
int return_selection(){
	return 0;
}
/*
int main(int argc, char const *argv[])
{
	// todo: make sure that the remnants of the inner menu do not remain as we change to the outer menu
	// todo: add checks that stop illegal actions

	click_button_respond();

	//oled_write_menu_to_screen();
	/*
	update_current_menu();
	menu_go_out();
	menu_go_down();
	menu_go_down();
	menu_go_in();
	

	//calls to menu go up or go down will later be called by the joystik which is polled 

	printf("finished");
	//menu_go_up();
	//oled_write_menu_to_screen();
	return 0;
}

*/