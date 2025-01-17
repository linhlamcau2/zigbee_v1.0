#include"rd_in_out.h"

#define LED_DI GPIO_PC4
#define LED_CI GPIO_PC3
#define RESET_TOUCH_PIN				GPIO_PA1

#define FREQ_LED 25000
#define NUM_LED_DATA (2*NUM_OUTPUT_MAX)
#define NUM_BYTE_FRAME_DATA  (4 * NUM_LED_DATA)

extern output_t rd_output[NUM_OUTPUT_MAX];

static u8 frame_led_data[NUM_BYTE_FRAME_DATA] = {0};

enum
{
	DIM_100 = BIT(4)| BIT(3)| BIT(2)| BIT(1)| BIT(0),
	DIM_50 = BIT(4),
	DIM_20 = BIT(2)| BIT(0),
	DIM_0= 0,
};

void Config_Pin_Led_Lc8823(void){
	gpio_set_func(LED_DI, AS_GPIO);
	gpio_set_output_en(LED_DI,1);
	gpio_set_func(LED_CI, AS_GPIO);
	gpio_set_output_en(LED_CI,1);

	gpio_set_func(RESET_TOUCH_PIN, AS_GPIO);
	gpio_set_output_en(RESET_TOUCH_PIN,1);
	gpio_write(RESET_TOUCH_PIN,0);
}

void start_frame_led(void){
	gpio_write(LED_DI,0);
	for(int i=0;i<32;i++){
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,0);
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,1);
	}
	wd_clear();
}

void end_frame_led(void){
	for(int i=0;i<32;i++){
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,0);
		gpio_write(LED_DI,1);
		sleep_us(500000/FREQ_LED);
		gpio_write(LED_CI,1);
	}
}

static u8 pulse = 0;

void write_clock()
{

	gpio_write(LED_CI,pulse);
	pulse = !pulse;
}
void write_frame_data()
{
	start_frame_led();
	for(u8 i=0; i< NUM_BYTE_FRAME_DATA;i++)
	{
		for(u8 j=0; j< 8; j++)
		{
			sleep_us(500000/FREQ_LED);
			write_clock();
			gpio_write(LED_DI,(frame_led_data[i] & BIT(7-j)));
			sleep_us(500000/FREQ_LED);
			write_clock();
		}
		wd_clear();
	}
	end_frame_led();
}

u8 set_dim(u8 dim)
{
//	u8 stt = 0xE0;
	return 0xE0 | dim;
}

void ctrl_led(u8 id, u8 level)
{
	if(id < NUM_OUTPUT_MAX)
	{
		u8 state = (level > 0) ? DIM_100: DIM_20;
		u8 stt = set_dim(state);

		if( id == 0)
		{
			frame_led_data[0] = stt;
			frame_led_data[4] = stt;
		}
		else if(id == 1)
		{
			frame_led_data[16] = stt;
			frame_led_data[20] = stt;
		}
		else if( id ==2)
		{
			frame_led_data[8] = stt;
			frame_led_data[12] = stt;
		}
		write_frame_data();
	}
}

void ctrl_all(u8 level)
{
	u8 data[4] = {0};
	u8 state = (level > 0) ? DIM_100: DIM_0;
	u8 stt = set_dim(state);
	data[0] = stt;
	data[1] = 0xff;
	for (u8 i=0; i< NUM_LED_DATA; i++)
	{
		memcpy((void *)&frame_led_data[4*i],(void *)data,4 * sizeof(u8));
	}
	write_frame_data();
}

void init_led_data()
{
	for(u8 i =0; i< NUM_OUTPUT_MAX; i++)
	{
		u8 state = (rd_output[i].stt > 0) ? DIM_100: DIM_20;
		u8 stt = set_dim(state);
		u8 data[4] = {0};
		data[0] = stt;
		data[1] = 0xff;
		if( i== 0)
		{
			memcpy((void *)&frame_led_data[0],(void *)data,sizeof(data));
			memcpy((void *)&frame_led_data[4],(void *)data,sizeof(data));
		}
		else if(i == 1)
		{
			memcpy((void *)&frame_led_data[16],(void *)data,sizeof(data));
			memcpy((void *)&frame_led_data[20],(void *)data,sizeof(data));
		}
		else if(i == 2)
		{
			memcpy((void *)&frame_led_data[8],(void *)data,sizeof(data));
			memcpy((void *)&frame_led_data[12],(void *)data,sizeof(data));
		}
	}
	write_frame_data();
}
