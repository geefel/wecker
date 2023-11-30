/*
 * von http://www.eckart-winkler.de/computer/c/varargs.htm
 * 
 * %c = Char
 * %s = String
 * %d = Integer
 * print("test %c, %s, %d", 'c', "String", 123);
 */

#include <stdint.h>
#include "print.h"
#include "uart_attiny.h"

void out(char *form, ...) {
  va_list args;
  char *pt;
  char ch;
  char *str;
  int n;
  char back[10];
	char *ph = back;
	int h;

  va_start(args, form);  //Parameterabfrage initialisieren
  for (pt = form; *pt; pt++) {
    if (*pt != '%') {		//Alle Zeichen ausser % ausgeben 
			sendDat(*pt);
			continue;
		}
    switch (*(++pt)) {	//Formatkennzeichner abarbeiten
      case 'c':
        ch = (char)va_arg(args, int);
        sendDat(ch); 
        break;
      
      case 's':
        str = (char*)va_arg(args, int*);
        while(*str) {
					sendDat(*str);
					str++;
				} 
        break;
      
      case 'd':
        n = (int)va_arg(args, int);
				if(!n) {					//Gleich null?
					sendDat('0'); 
					return; 
				}
				if(n < 0) {				//Vorzeichen?
					sendDat('-'); 
					n = -n; 
				}
				while(n) { 				//Ziffern rueckwaerts nach back schreiben
					h = n % 10; 
					n = n / 10; 
					*ph++ = (char)('0' + h); 
				}
				while(ph > back) 	//Nun Ziffern ausgeben
					sendDat(*(--ph));
        break;
      case 0:
        return;
      default:
        sendDat(*pt); 
        break;
    }
  }
  va_end(args);						//Parameterabfrage beenden und Schluss
  return;
}
