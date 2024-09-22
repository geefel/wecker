/*

* in Funktion void main() oder sonst wo:
#ifdef DEBUGs
initUartHW(baudrate);
#endif

*/
 
#ifndef UART_PRINT_H
#define UART_PRINT_H

//#define DEBUGs

//#ifdef DEBUGs

void pText(const char *txt);
void pTextn(const char *txt);
void pItoA(int16_t x);
void pItoAn(int16_t x);
void pInt(uint8_t x);
void pIntn(uint8_t x);
void pInt16(uint16_t x);
void pInt16n(uint16_t x);

//#endif	//DEBUGs

#endif //UART_PRINT_H
