#include "tft.h"

uint8_t		FRAME[FRAME_BUF_SIZE];			// frame buffer
uint16_t	CUR_FRAME_POS = 0;				// index of left side of current frame
extern	uint8_t	ADC_BUFFER_STATUS;			// status of ADC buffer
extern	int16_t data_adc_buff[2][STEP];

extern uint8_t DMA_STATUS_FLAG;

uint8_t	Background_buf[480];		// buffer of background column colors
uint8_t MainLine_buf[480];			// buffer of main line column colors
uint8_t COLOR_FLAG = 0;				// flag for swapping color buffers


/* Initialization of color buffers */
void tft_ColorBuffers_init(){
	uint8_t color_array[8];
	color_array[0] = (uint8_t)((MainLineColor >> 8) & 0xFF) ;
	color_array[1] = (uint8_t)(MainLineColor & 0xFF);
	color_array[2] = (uint8_t)((BackgroundColor >> 8) & 0xFF) ;
	color_array[3] = (uint8_t)(BackgroundColor & 0xFF);
	color_array[4] = (uint8_t)((VoltageColor >> 8) & 0xFF) ;
	color_array[5] = (uint8_t)(VoltageColor & 0xFF);
	for (int i = 0; i<240; i++){
		if ((i == 21) || (i == 94) || (i == 167)){	// voltage lines
			Background_buf[i*2] = color_array[4];
			Background_buf[i*2+1] = color_array[5];
		}
		else{
			Background_buf[i*2] = color_array[2];
			Background_buf[i*2+1] = color_array[3];
		}
		MainLine_buf[i*2] = color_array[0];
		MainLine_buf[i*2+1] = color_array[1];
	}
}

/* preset FRAME buffer to zeros to avoid noise at start of plotting */
void tft_ZeroFrame(void){
	for (int i=0; i<FRAME_BUF_SIZE; i++)
		FRAME[i] = 239;
}

void tft_ADCtoFrame(void){
	if(ADC_BUFFER_STATUS == BUF_IS_FULL){
		ADC_BUFFER_STATUS = BUF_IN_PROCESS;
		uint8_t current_buffer_indx = DMA2_Stream0->CR & DMA_SxCR_CT ? 0 : 1;
		for(int i = 0; i < STEP; i++)
			FRAME[(CUR_FRAME_POS  + 320 + i)%FRAME_BUF_SIZE] = 239-(int)((double)(data_adc_buff[current_buffer_indx][i])/4096*240);
		tft_UpdateFrame();
	}
}


void TFT_init(){
	SPI1_Master_init(); // SPI1 initialization
	LED_on(); //turn on the led

	// Hardware reset
	RESET_ACTIVE();
	tft_MicrosecDelay(1);
	RESET_IDLE();
	tft_MicrosecDelay(40);

	// pack of initialization commands
	tft_write_cmd(0x01); // software reset
	tft_display_normal_mode();
	tft_pixel_format();
	tft_sleep_out();
	tft_display_on();

	// buffers initializations
	tft_ColorBuffers_init();
	tft_ZeroFrame();
}

/* display preview */
void Greetings(void){
	tft_color_All(BLACK);
	char str1[] = "powered by";
	tft_MicrosecDelay(100);
	Draw_String(125, 20, WHITE, BLACK, &str1[0], 1);
	char str2[] = "Melnichuk M.A.";
	Draw_String(45, 60, WHITE, BLACK, &str2[0], 2);
	char str3[] = "RL6-71";
	Draw_String(120, 100, WHITE, BLACK, &str3[0], 2);
	tft_MicrosecDelay(300000);
}

/* overlay current frame with background buffer colors and draw new one */
void tft_UpdateFrame(){
	COLOR_FLAG = 0;	// select Background color buffer
	for (int cur_col = 1; cur_col < 320; cur_col++)
		tft_ColorColumn(cur_col, FRAME[(CUR_FRAME_POS+cur_col-1)%FRAME_BUF_SIZE], FRAME[(CUR_FRAME_POS+cur_col)%FRAME_BUF_SIZE]);
	CUR_FRAME_POS = (CUR_FRAME_POS+STEP) % FRAME_BUF_SIZE;
	COLOR_FLAG = 1;	// select MainLine color buffer
	for (int cur_col = 1; cur_col < 320; cur_col++)
		tft_ColorColumn(cur_col,FRAME[(CUR_FRAME_POS+cur_col-1)%FRAME_BUF_SIZE], FRAME[(CUR_FRAME_POS+cur_col)%FRAME_BUF_SIZE]);
}

/* delay for a few us */
void tft_MicrosecDelay(uint32_t tick){
	tick*=168;
	while (--tick);
}

/* delay for a few ticks */
void TickDelay(uint32_t tick){
	while(tick--);
}

/* send command to display without data */
void tft_write_cmd(uint8_t cmd){
	DC_COMMAND(); // set DC channel to command mode
	spi1_SendDataDMA_1Byte(&cmd, 1);
	tft_MicrosecDelay(1);
	DC_DATA(); // set DC channel to data mode
}

/* send command to display with data */
void tft_write_cmd_data(uint8_t cmd, uint8_t *data, uint8_t size){
	if (size == 0)
		return;
	DC_COMMAND(); // set DC channel to command mode
	spi1_SendDataDMA_1Byte(&cmd, 1);
	tft_MicrosecDelay(1);
	DC_DATA(); // set DC channel to data mode
	spi1_SendDataDMA_1Byte(&data[0], size);
	tft_MicrosecDelay(8);
}

/* send Sleep_out command to display */
void tft_sleep_out(){
	DC_COMMAND(); // set DC channel to command mode
	uint8_t cmd=Sleep_out;
	spi1_SendDataDMA_1Byte(&cmd, 1);
	tft_MicrosecDelay(3100);
}

/* send Display_off command to display */
void tft_display_off(){
	tft_write_cmd(Display_OFF);
}

/* send Display_on command to display */
void tft_display_on(){
	uint8_t data=ROTATION;
	tft_write_cmd_data(Display_ON, &data, 1);
}

/* send Normal_Display_mode command to display */
void tft_display_normal_mode(){
	tft_write_cmd(Normal_Display_mode);
}

/* send Pixel_16_bits command to display */
void tft_pixel_format(){
	uint8_t data = Pixel_16_bits;
	tft_write_cmd_data(Pixel_Format_Set, &data, 1);
}

/* set left and right columns of region */
void tft_set_column(uint16_t col_start, uint16_t col_end){
	uint8_t data_column[4] = {	(uint8_t)((col_start >> 8) & 0xFF),
								(uint8_t)(col_start & 0xFF),
								(uint8_t)((col_end >> 8) & 0xFF),
								(uint8_t)(col_end & 0xFF)};
	tft_write_cmd_data(Column_Address_Set, &data_column[0], 4);
}

/* set top and bottom rows of region */
void tft_set_row(uint16_t row_start, uint16_t row_end){
	uint8_t data_row[4] = {	(uint8_t)(row_start >> 8),
							(uint8_t)(row_start & 0xFF),
							(uint8_t)(row_end >> 8),
							(uint8_t)(row_end & 0xFF)};
	tft_write_cmd_data(Page_Adress_Set, &data_row[0], 4);
}

/* set region position for coloring */
void tft_set_region(uint16_t row_start, uint16_t row_end, uint16_t col_start, uint16_t col_end){
	uint8_t data = 0x0;
	tft_write_cmd_data(Memory_Access_Control, &data, 1);
	tft_set_column(col_start, col_end);
	tft_set_row(row_start, row_end);
}

/* fill full screen with selected color */
void tft_color_All(uint16_t color){
	tft_set_region(0,X_SIZE,0,Y_SIZE);
	uint8_t	Color_buf[480]; // create temporary column color buffer for fastest coloring
	uint8_t color_array[2]; // parse color
	color_array[0] = (uint8_t)((color >> 8) & 0xFF);
	color_array[1] = (uint8_t)(color & 0xFF);
	for (int i = 0; i<240; i++){ // fill Color_buf
		Color_buf[i*2] = color_array[0];
		Color_buf[i*2+1] = color_array[1];
	}
	tft_write_cmd(Memory_Write); // send Memory_write command
	uint16_t i = 0;
	while(i<=320){
		if (DMA_STATUS_FLAG == 1){ // check if DMA transfer completed
			DMA_STATUS_FLAG = 0; // refresh flag DMA status flag state
			spi1_SendDataDMA_Column(Color_buf,480); // send column color buffer
			++i;
		}
	}
}

/* fill full screen with Background_buf combination */
void tft_color_Background(void){
	tft_set_region(0,X_SIZE,0,Y_SIZE);
	tft_write_cmd(Memory_Write); // send Memory_write command
	uint16_t i = 0;
	while(i<320){
		if (DMA_STATUS_FLAG == 1){ // check if DMA transfer completed
			DMA_STATUS_FLAG = 0; // refresh flag DMA status flag state
			spi1_SendDataDMA_Column(Background_buf,480);  // send column color buffer
			++i;
		}
	}
}

/* fill selected region with selected color */
void tft_ColorXY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color){
	if (x1>X_SIZE || y1>Y_SIZE || x2>X_SIZE || y2>Y_SIZE)
		return;
	if (x1>x2) swap(&x1,&x2);
	if (y1>y2) swap(&y1,&y2);
	tft_set_region(x1,x2,y1,y2);
	int count_pixels = (x2-x1+1)*(y2-y1+1);
	uint8_t color_array[2];
	color_array[0] = (uint8_t)((color >> 8) & 0xFF) ;
	color_array[1] = (uint8_t)(color & 0xFF);
	tft_write_cmd(Memory_Write); // send Memory_write command
	DC_DATA();
	for(uint32_t i = 0; i < count_pixels+1; i++)
		spi1_SendDataDMA_1Byte(&color_array[0],2); // send 2 byte packages of pixel colors
}

/* fill selected part of selected column with any color buffer combination according to y1 and y2 positions */
void tft_ColorColumn(uint16_t x, uint16_t y1, uint16_t y2){
	uint8_t *current_buffer;
	if (x>X_SIZE || y1>Y_SIZE || y2>Y_SIZE)
		return;
	if (y1>y2)
		swap(&y1,&y2);
	tft_set_region(x,x,y1,y2); // set region
	tft_write_cmd(Memory_Write); // send Memory_write command
	current_buffer = COLOR_FLAG ? MainLine_buf : Background_buf; // detect which color buffer should be used
	uint16_t i = 0;
	while(i<1){
		if (DMA_STATUS_FLAG == 1){ // check if DMA transfer completed
			DMA_STATUS_FLAG = 0; // refresh flag DMA status flag state
			spi1_SendDataDMA_Column(&current_buffer[y1*2],(y2-y1+1)*2);
			++i;
			TickDelay(16*(y2-y1+1)); // dynamic delay 16 ticks/pixel
		}
	}
}

/* swap variables */
void swap(uint16_t *a, uint16_t *b){
	uint16_t t;
	t=*a;
	*a=*b;
	*b=t;
}

/* set display to RGB mode */
void tft_RGB(){
	uint8_t data[2];
	data[0] = 0x00;
	data[1] = 0x18;
	tft_write_cmd_data(0xB1, &data[0], 2);
}



