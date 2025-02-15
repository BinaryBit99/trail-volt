/**
 * Copyright (C) PlatformIO <contact@platformio.org>
 * See LICENSE for details.
 */

 #include <avr/io.h>

 int main (void)
 {
     /* Configure SW0 as input */
     PORTC.DIRCLR = PIN1_bm;
     /* Configure LED0 pin as output */
     PORTB.DIRSET = PIN4_bm;
 
     while (1)
     {
         /* Check the status of SW0 */
         /* 0: Pressed */
         if (!(PORTC.IN & (PIN1_bm)))
         {
             /* LED0 on */
             PORTB.OUTSET = PIN4_bm;
         }
         /* 1: Released */
         else
         {
             /* LED0 off */
             PORTB.OUTCLR = PIN4_bm;
         }
     }
 }
 