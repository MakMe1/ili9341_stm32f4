#include "tft.h"


//int CURRENT_FRAME_ADDRESS = FRAME_ADDRESS;
uint8_t		FRAME[FRAME_BUF_SIZE];
uint16_t	CUR_FRAME_POS = 0;
uint8_t		DATA_OFFSET = 0;
extern	uint8_t	ADC_BUFFER_STATUS;
extern	int16_t data_adc_buff[2][STEP];

void ZeroFrame(void)
{
	for (int i=0; i<FRAME_BUF_SIZE; i++)
	{
		FRAME[i] = 240;
	}
//	for (int i = 0; i < 640; i++)
//	{
//		FRAME[i] = 120 - (int)(120 * sin((double)i/8/3.14));
//	}
}

void ADC_Get_Data(void)
{
	if(ADC_BUFFER_STATUS == BUF_IS_FULL)
	{
		ADC_BUFFER_STATUS = BUF_IN_PROCESS;
		uint8_t current_buffer_indx = DMA2_Stream0->CR & DMA_SxCR_CT ? 0 : 1;
		for(int i = 0; i < STEP; i++)
		{
			FRAME[(CUR_FRAME_POS  + 320 + i)%FRAME_BUF_SIZE] = 240-(int)((double)(data_adc_buff[current_buffer_indx][i])/4096*240);
		}
		UpdateFrame();
	}
}


void TFT_init(){
	spi1_master_init();
	LED_on();

	RESET_ACTIVE();
	Micro_tick_delay(1);
//	TickDelay(707); // experimental value
	RESET_IDLE();
	Micro_tick_delay(40);
//	TickDelay(8141); // experimental value

	tft_write_cmd(0x01,1,1); // software reset
// necessary to wait 5msec before sending new command
// it works without delay at all

	tft_display_normal_mode();
// necessary to wait 5msec before sending new command
// it works without delay at all
	tft_pixel_format();
//	not necessary to wait

	tft_sleep_out();
// necessary to wait 5msec before sending new command
// it works without delay at all
	tft_display_on();
//	not necessary to wait

	Greetings();
	ZeroFrame();
}

void Greetings(void)
{
	tft_color_ALL(BLACK);
	char str1[] = "powered by";
	Draw_String(125, 20, WHITE, BLACK, &str1[0], 1);
	char str2[] = "Melnichuk M.A.";
	Draw_String(45, 60, WHITE, BLACK, &str2[0], 2);
	char str3[] = "RL6-71";
	Draw_String(120, 100, WHITE, BLACK, &str3[0], 2);
	Micro_tick_delay(300000);
	tft_color_ALL(BLACK);
}

void UpdateFrame()
{
	for (int cur_col = 1; cur_col < 320; cur_col++)
		tft_color_XY(cur_col, FRAME[(CUR_FRAME_POS+cur_col-1)%FRAME_BUF_SIZE], cur_col, FRAME[(CUR_FRAME_POS+cur_col)%FRAME_BUF_SIZE], BLACK);
	CUR_FRAME_POS = (CUR_FRAME_POS+STEP) % FRAME_BUF_SIZE;
	for (int cur_col = 1; cur_col < 320; cur_col++)
		tft_color_XY(cur_col,FRAME[(CUR_FRAME_POS+cur_col-1)%FRAME_BUF_SIZE], cur_col, FRAME[(CUR_FRAME_POS+cur_col)%FRAME_BUF_SIZE], GREEN);
}

void Micro_tick_delay(uint32_t tick){
	tick*=168;
	while (tick--);
}

void TickDelay(uint32_t tick){
	while(tick--);
}

void tft_write_cmd(uint8_t cmd, uint8_t *data, uint8_t size) {
	if (size == 0) {
		return;
	}

	DC_COMMAND();
	spi1_SendDataDMA_1Byte(&cmd, 1);
	Micro_tick_delay(1);

	DC_DATA();
	spi1_SendDataDMA_1Byte(&data[0], size);
	Micro_tick_delay(8); // experimental value
}

void tft_sleep_out() {
	DC_COMMAND();
	uint8_t data=Sleep_out;
	spi1_SendDataDMA_1Byte(&data, 1);
	Micro_tick_delay(3100); // experimental value
}

void tft_display_off() {
	tft_write_cmd(Display_OFF, 0, 0);
}

void tft_display_on() {
	uint8_t data=ROTATION;
	tft_write_cmd(Display_ON, &data, 1);
}

void tft_display_normal_mode() {
	DC_COMMAND();
	uint8_t data=Normal_Display_mode;
	spi1_SendDataDMA_1Byte(&data, 1);
}

void tft_pixel_format() {
	uint8_t data = Pixel_16_bits;
	tft_write_cmd(Pixel_Format_Set, &data, 1);
}

//column - y
void tft_set_column(uint16_t col_start, uint16_t col_end) {
	uint8_t data_column[4] = {	(uint8_t)((col_start >> 8) & 0xFF),
								(uint8_t)(col_start & 0xFF),
								(uint8_t)((col_end >> 8) & 0xFF),
								(uint8_t)(col_end & 0xFF)};

	tft_write_cmd(Column_Address_Set, &data_column[0], 4);
}

//row - x
void tft_set_row(uint16_t row_start, uint16_t row_end) {
	uint8_t data_row[4] = {	(uint8_t)(row_start >> 8),
							(uint8_t)(row_start & 0xFF),
							(uint8_t)(row_end >> 8),
							(uint8_t)(row_end & 0xFF)};
	tft_write_cmd(Page_Adress_Set, &data_row[0], 4);
}

void tft_ram_write(uint8_t *data, uint8_t size) {
	tft_write_cmd(Memory_Write, &data[0], size);
}

void tft_set_region(uint16_t row_start, uint16_t row_end, uint16_t col_start, uint16_t col_end) { // выбор области
	uint8_t data = 0x0;
	tft_write_cmd(Memory_Access_Control, &data, 1);

	tft_set_column(col_start, col_end);
	tft_set_row(row_start, row_end);
	tft_ram_write(&data, 1);
}

void tft_color_ALL(uint16_t color) {
	tft_set_region(0,X_SIZE,0,Y_SIZE);
	uint8_t color_array[4];
	color_array[0] = (uint8_t)((color >> 8) & 0xFF) ;
	color_array[1] = (uint8_t)(color & 0xFF);
	DC_DATA();
	for (int i = 0; i<76800; i++)
	{
		spi1_SendDataDMA_1Byte(&color_array[0],2);
	}
}

// For FLASH frame onely
//void Send_Frame_Color(uint8_t *color_array)
//{
//	uint32_t byte_index;
//	uint8_t pix_info;
//	for (int i = 0; i < PIX_AMOUNT; i++)
//	{
//		byte_index = i/8;
//		pix_info = *(__IO uint8_t*)(CURRENT_FRAME_ADDRESS + byte_index);
//		if ((pix_info >> (i%8)) & 0x1)
//		{
//			spi1_SendDataDMA_1Byte(&color_array[0],2);
//		}
//		else
//			spi1_SendDataDMA_1Byte(&color_array[2],2);
//	}
//}

void tft_color_XY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color) {
	if (x1>X_SIZE || y1>Y_SIZE || x2>X_SIZE || y2>Y_SIZE)
		return;
	if (x1>x2) swap(&x1,&x2);
	if (y1>y2) swap(&y1,&y2);
	tft_set_region(x1,x2,y1,y2);
	int count_pixels = (x2-x1+1)*(y2-y1+1);
	uint8_t color_array[2];
	color_array[0] = (uint8_t)((color >> 8) & 0xFF) ;
	color_array[1] = (uint8_t)(color & 0xFF);
	DC_DATA();
	for(uint32_t i = 0; i < count_pixels+1; i++){
		spi1_SendDataDMA_1Byte(&color_array[0],2);
	}
}

//void tft_draw_pixel(uint16_t x, uint16_t y, uint16_t color){
//	RESET_CS();
//	tft_set_region(x, x+1, y, y+1);
//	DC_DATA();
//	uint8_t color_array[2];
//	color_array[0] = (uint8_t)((color >> 8) & 0xFF) ;
//	color_array[1] = (uint8_t)(color & 0xFF);
//	DC_DATA();
//	spi1_SendDataDMA_1Byte(&color_array[0],2);
//}

//void tft_SetRotation(uint8_t r){
//	uint8_t data;
//	switch(r){
//		case 0:
//			data= 0x48;
//			tft_write_cmd(Memory_Access_Control, &data, 1);
//
//			#define X_SIZE 240
//			#define Y_SIZE 320
//			break;
//		case 1:
//			data= 0x28;
//			tft_write_cmd(Memory_Access_Control, &data, 1);
//			#define X_SIZE 320
//			#define Y_SIZE 240
//			break;
//		case 2:
//			data=0x88;
//			tft_write_cmd(Memory_Access_Control, &data, 1);
//			#define X_SIZE 240;
//			#define Y_SIZE 320;
//			break;
//		case 3:
//			data=0xE8;
//			tft_write_cmd(Memory_Access_Control, &data, 1);
//			#define X_SIZE 320
//			#define Y_SIZE 240
//			break;
//	}
//
//}

void swap(uint16_t *a, uint16_t *b) {
	uint16_t t;
	t=*a;
	*a=*b;
	*b=t;
}

void tft_RGB(){
	uint8_t data[2];
	data[0] = 0x00;
	data[1] = 0x18;
	tft_write_cmd(0xB1, &data[0], 2);
}



