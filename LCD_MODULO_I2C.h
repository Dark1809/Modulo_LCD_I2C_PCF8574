/* 
 * File: LCD_MODULO_I2C   
 * Author: Dark1809
 * Comments:biblioteca para avrs * 
 */
#include <avr/io.h>


#ifndef F_CPU
/* prevent compiler error by supplying a default */
# warning "F_CPU not defined for "LCD_MODULO_I2C.h""
/** \ingroup util_delay
    \def F_CPU
    \brief CPU frequency in Hz

    The macro F_CPU specifies the CPU frequency to be considered by
    the delay macros.  This macro is normally supplied by the
    environment (e.g. from within a project header, or the project's
    Makefile).  The value 1 MHz here is only provided as a "vanilla"
    fallback if no such user-provided definition could be found.

    In terms of the delay functions, the CPU frequency can be given as
    a floating-point constant (e.g. 3.6864E6 for 3.6864 MHz).
    However, the macros in <util/setbaud.h> require it to be an
    integer value.
 */
# define F_CPU 8000000UL
#endif

//---------Protótipos-das-funções-I2C--------------
char i2c_read();
//---------Protótipos-das-funções-LCD--------------
void lcd_i2c_init(char x);  // Inicialização da função LCD e do I2C
void toggle();              // Função enable do LCD
void lcd_cmd_hf(char v1);   // Função para enviar o comando de meio byte para o LCD
void lcd_cmd(char v2);      // Função para enviar o comando para o LCD
void lcd_dwr(char v3);      // Função para enviar dados para o LCD
void lcd_msg(char *c);      // Função para enviar strings para o LCD
void delay(int ms);         // Função delay-nãi use no codigo principal
void lcd_lef_sh();          // Função de Shifting para a esquerda
void lcd_rig_sh();          // Função de Shifting para a direita
void lcd_set_backlight(char r);   // Acionamento da luz de fundo

//----------Subrotinas-do-I2C----------------------

char i2c_read(){
	TWCR  = (1<<TWEN) | (1<<TWINT);	//Habilitar I2C e interrupção clear
	while (!(TWCR & (1<<TWINT)));	//Leitura bem sucedida com todos os dados recebidos no TWDR
	return TWDR;
}

//----------Subrotinas-do-LCD----------------------

void toggle() {
    TWDR &= ~0x02;                                  //Coloca o PB1 em zero, no caso o R/W  
    TWDR |= 0x04;                                   //Coloca o PB2 em um , no caso o EN-----Envio de dados para o registrador de dados do LCD utilizando o sinal de HIGH para LOW
    TWCR = (1 << TWINT) | (1 << TWEN);              //Habilitar I2C e interrupção clear
    while (!(TWCR & (1 << TWINT)));                 //Verificação de recebimento de dados TWDR
    delay(1);
    TWDR &= ~0x04;                                  //Coloca o PB2 em zero, pino de EN do LCD vai para zero
    TWCR = (1 << TWINT) | (1 << TWEN);              //Habilitar I2C e interrupção clear
    while (!(TWCR & (1 << TWINT)));                 //Verificação de recebimento de dados TWDR
}

void lcd_cmd_hf(char v1) {
    TWDR &= ~0x01;                                  //Coloca o PB0 em zero , no caso o RS do LCD rs = 0----Seleção do registrador como registrador de comando
    TWCR = (1 << TWINT) | (1 << TWEN);              //Habilitar I2C e interrupção clear
    while (!(TWCR & (1 << TWINT)));                 //Verificação de recebimento de dados TWDR
    TWDR &= 0x0F;                                   //Limpeza dos nibbles altos
    TWCR = (1 << TWINT) | (1 << TWEN);              //Habilitar I2C e interrupção clear
    while (!(TWCR & (1 << TWINT)));                 //Verificação de recebimento de dados TWDR
    TWDR |= (v1 & 0xF0);                            //Mascara dos nibbles alto e enviando para o LCD
    TWCR = (1 << TWINT) | (1 << TWEN);              //Habilitar I2C e interrupção clear
    while (!(TWCR & (1 << TWINT)));                 //Verificação de recebimento de dados TWDR
    toggle();
}

void lcd_cmd(char v2) {
    TWDR &= ~0x01;                                  //Coloca o PB0 em zero, rs = 0;----Seleção do registrador como registrador de comando
    TWCR = (1 << TWINT) | (1 << TWEN);              //Habilitar I2C e interrupção clear
    while (!(TWCR & (1 << TWINT)));                 //Verificação de recebimento de dados TWDR
    TWDR &= 0x0F;                                   //limpeza do nibble HIGH
    TWCR = (1 << TWINT) | (1 << TWEN);              //Habilitar I2C e interrupção clear
    while (!(TWCR & (1 << TWINT)));                 //Verificação de recebimento de dados TWDR
    TWDR |= (v2 & 0xF0);                            //Mascara dos nibbles alto e enviando para o LCD
    TWCR = (1 << TWINT) | (1 << TWEN);              //Habilitar I2C e interrupção clear
    while (!(TWCR & (1 << TWINT)));                 //Verificação de recebimento de dados TWDR
    toggle();

    TWDR &= 0x0F;                                   //Limpeza do nibble HIGH
    TWCR = (1 << TWINT) | (1 << TWEN);              //Habilitar I2C e interrupção clear
    while (!(TWCR & (1 << TWINT)));                 //Verificação de recebimento de dados TWDR
    TWDR |= ((v2 & 0x0F) << 4);                     //Mascara dos nibbles baixos e enviando para o LCD
    TWCR = (1 << TWINT) | (1 << TWEN);              //Habilitar I2C e interrupção clear
    while (!(TWCR & (1 << TWINT)));                 //Verificação de recebimento de dados TWDR
    toggle();
}

void lcd_dwr(char v3) {
    TWDR |= 0x01;                                   //Coloca o PB0 em um, rs = 1; ----Seleção do registrador como registrador de comando
    TWCR = (1 << TWINT) | (1 << TWEN);              //Habilitar I2C e interrupção clear
    while (!(TWCR & (1 << TWINT)));                 //Verificação de recebimento de dados TWDR
    TWDR &= 0x0F;                                   //Limpeza do nibble HIGH
    TWCR = (1 << TWINT) | (1 << TWEN);              //Habilitar I2C e interrupção clear
    while (!(TWCR & (1 << TWINT)));                 //Verificação de recebimento de dados TWDR
    TWDR |= (v3 & 0xF0);                            //Mascara dos nibbles alto e enviando para o LCD
    TWCR = (1 << TWINT) | (1 << TWEN);              //Habilitar I2C e interrupção clear
    while (!(TWCR & (1 << TWINT)));                 //Verificação de recebimento de dados TWDR
    toggle();

    TWDR &= 0x0F;                                   //Limpeza do nibble HIGH
    TWCR = (1 << TWINT) | (1 << TWEN);              //Habilitar I2C e interrupção clear
    while (!(TWCR & (1 << TWINT)));                 //Verificação de recebimento de dados TWDR
    TWDR |= ((v3 & 0x0F) << 4);                     //Mascara dos nibbles baixos e enviando para o LCD
    TWCR = (1 << TWINT) | (1 << TWEN);              //Habilitar I2C e interrupção clear
    while (!(TWCR & (1 << TWINT)));                 //Verificação de recebimento de dados TWDR
    toggle();
}

void lcd_i2c_init(char x) {
    //Inicialização do I2C
    TWBR = 0x62;		//Calculo do Baud rate
	TWCR = (1<<TWEN);	//Habilitar I2C
	TWSR = 0x00;		//Prescala de 1
    
    //Condição de inicio do I2C
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);	//Condição de inicio
	while (!(TWCR & (1<<TWINT)));
	
    //Envio do endereçamento do dispositivo
    TWDR = x;						//O endereço do I2C do modulo a ser aplicado em hexadeciaml
	TWCR = (1<<TWINT) | (1<<TWEN);	//Habilitar I2C e interrupção clear
	while  (!(TWCR &(1<<TWINT)));
    // Inicialização do LCD
    lcd_cmd_hf(0x30);                               //Sequencia de inicialização do LCD
    lcd_cmd_hf(0x30);                               //     "            "            "              
    lcd_cmd_hf(0x20);                               //     "            "            "               
    lcd_cmd(0x28);                                  //Seleção 16 x 2 LCD em modo de 4bit
    lcd_cmd(0x0C);                                  //Display ON Cursor OFF
    lcd_cmd(0x01);                                  //Clear display
    lcd_cmd(0x06);                                  //Cursor Auto Increment
    lcd_cmd(0x80);                                  //Primeira line primeira localização do LCD
}

void delay(int ms) {
    int i, j;
    for (i = 0; i <= ms; i++)
        for (j = 0; j <= 120; j++);
}

void lcd_msg(char *c) {
    while (*c != 0)                                 //Aguardar até que todas as Strings sejam passadas para o LCD
        lcd_dwr(*c++);                              //Enviar as strings para o LCD
}

void lcd_rig_sh() {
    lcd_cmd(0x1C);                                  //Comando para shift direita
    delay(400);
}

void lcd_lef_sh() {
    lcd_cmd(0x18);                                  //Comando para shift esquerda
    delay(200);
}

void lcd_set_backlight(char r) {
    
    if (r == 1) {
        TWDR |= 0x08;                               //Coloca o PB3 em um, no caso do LCD liga o backlight
        TWCR = (1 << TWINT) | (1 << TWEN);          //Habilitar I2C e interrupção clear
        while (!(TWCR & (1 << TWINT)));
    } else if (r == 0) {
        TWDR &= ~0x08;                              //Coloca o PB3 em zero, no caso do LCD desliga o backlight
        TWCR = (1 << TWINT) | (1 << TWEN);          //Habilitar I2C e interrupção clear
        while (!(TWCR & (1 << TWINT)));             //Verificação de recebimento de dados TWDR
    }

}



