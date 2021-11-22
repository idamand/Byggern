/*
 * menu_oled.h
 *
 * Created: 12.10.2021 10:45:32
 *  Author: raffaelv
 */ 

 
#ifndef MENU_OLED_H_
#define MENU_OLED_H_ 


//this is the most important function, it will both update the menu and write it to screen (or putty in first iteration)
void update_current_menu();

void menu_oled_clear_screen();
void oled_write_menu_to_screen();


void menu_go_down();
void menu_go_up();
void menu_go_in();
void menu_go_out();
int menu_loop(); //returns the value of selected item, if a menu item was selected, else it returns -1



#endif /* MENU_OLED_H_ */