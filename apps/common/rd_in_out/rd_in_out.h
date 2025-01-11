#ifndef RD_IN_OUT_H
#define RD_IN_OUT_H


#define OUTPUT_1					GPIO_PB1
#define OUTPUT_2					GPIO_PC1



void rd_init_gpio();
void rd_blink_led();



void rd_handle_input();
#endif
