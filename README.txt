# Modulo LCD I2C PCF8574
Biblioteca do Módulo LCD I2C 16x2 Atmega328p

# Manual de instrução(antes de usar a biblioteca):
- Verifique qual o modelo do PCF8574 ou PCF8574A
- Verifique se os pinos A0, A1 e A2 estão soldadas ou não.
- Consulte a tabela abaixo depois de verificar os pinos.(Obs. Com solda é VSS e sem solda é VDD)

  PCF8574:
  
  ![adress_PCF8574](https://github.com/Dark1809/Modulo_LCD_I2C_PCF8574/assets/149520927/24ed71cf-e9d1-409f-a271-510542b14c67)

  PCF8574A:
  
  ![adress_PCF8574A](https://github.com/Dark1809/Modulo_LCD_I2C_PCF8574/assets/149520927/9985a118-b14b-464b-b05a-f1f6080aed1b)

  
- Após de consultar a tabela utilize o Adrress byte value -> write
  # Exemplo
  Modelo: PCF8574: A0=sem solda, A1=com solda, A2=sem solda.
  A partir da tabela temos 44h esse é o endereçamento que utilizamremos na biblioteca.
# Manual de instrução de utilizar a biblioteca:
-  Temos as seguintes funções:
    - void lcd_i2c_init(char x);  // Inicialização da função LCD e do I2C
    - void lcd_cmd(char v2);      // Função para enviar o comando para o LCD
    - void lcd_msg(char *c);      // Função para enviar strings para o LCD
    - void lcd_set_backlight(char r);   // Acionamento da luz de fundo
A função lcd_i2c_init(char x) é para inicializar o I2C e o LCD, cujo este colocaremos o endereço do módulo.
A função lcd_cmd(char v2) coloca os comandos aceitos pelo LCD. Ex:0x01 limpa display com retorno de curso.
A função lcd_msg(char *c) coloca a string desejado 
A função lcd_set_backlight(char r) liga(1) ou desliga(0) o led do LCD
# Exemplo do uso da Biblioteca:
Abaixo temos um exemplo de código usando a biblioteca.

    #define  F_CPU 16000000UL 
    #include <avr/io.h>
    #include "LCD_MODULO_I2C.h"
    #include <util/delay.h>
    
    int main(void) {
        char msg1[] = "Hello World!";
        char msg2[] = "Aqui esta o I2C!";  
    
        lcd_i2c_init(0x44);     //Envia o endereço em hexadecimal do I2C. Inicia o LCD e o I2C.
        lcd_set_backlight(1);   //Liga o led
        lcd_cmd(0x80);          //A posição que será colocada a string
        lcd_msg(msg1);          //Recebe a Strings
        lcd_cmd(0xC0);          //A posição que será colocada a string
        lcd_msg(msg2);          //Recebe a Strings
        _delay_ms(10000);
        lcd_set_backlight(0);   //Desliga o led

        while (1) {
        }
    }
