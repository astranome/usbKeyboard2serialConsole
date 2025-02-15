/*
 * The MIT License (MIT)
 * Astra Researh edition ******************************
 * Copyright (c) 2022 No0ne (https://github.com/No0ne)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#define DEBUG 1 

#include "hardware/gpio.h"
#include "bsp/board.h"
#include "tusb.h"

#if DEBUG
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 28
#endif

#define HID_A 0x04
#define HID_Z 0x1D

uint8_t kbd_addr = 0;
uint8_t kbd_inst = 0;
/*
const char CSI_7BIT[] = "\e[";
const char CSI_8BIT[] = "\x9B";
const char DCS_7BIT[] = "\eP";
const char DCS_8BIT[] = "\x90";
const char SS2_7BIT[] = "\eN";
const char SS2_8BIT[] = "\x8E";
const char SS3_7BIT[] = "\eO";
const char SS3_8BIT[] = "\x8F";
const char ST_7BIT[]  = "\e\\";
const char ST_8BIT[]  = "\x9C";
const char OSC_7BIT[] = "\e]";
const char OSC_8BIT[] = "\x9D";
*/


uint8_t const hid2ascii[] = {
  0x00, 	//0-3 empty
  0x00, 
  0x00, 
  0x00, 
  'a', 		//a..z 1..0
  'b',
  'c',
  'd',
  'e',
  'f',
  'g',
  'h',
  'i',
  'j',
  'k',
  'l',
  'm',
  'n',
  'o',
  'p',
  'q',
  'r',
  's',
  't',
  'u',
  'v',
  'w',
  'x',
  'y',
  'z',
  '1',
  '2',
  '3',
  '4',
  '5',
  '6',
  '7',
  '8',
  '9',
  '0',
  0x0D, 	//enter
  0x1B,		//esc
  0x08,		//BS
  0x09,		//TAB
  ' ',
  '-',
  '=',
  '[',
  ']',
  0x5C,		// backslash
  0x5C, 		// ???
  ';',
  0x27,		//single quote
  '`',
  ',',
  '.',
  '/',
  0x00,		//CAPS del 04
  'P',		//F1 3B = 58
  'Q',		//F2
  'R',		//F3
  'S',		//F4
  't',		//F5
  'u',		//F6
  'v',		//F7
  'l',		//F8
  'w',		//F9
  'x',		//F10    1A STOPPED
  0x6B,		//F11
  0x6C,		//F12
  0x00,		//PrtScr
  0x02,		//ScrlLock
  0x1A,		//Pause/Break STopped ,return - fg !!!
  0x19,		//Ins
  0x11,		//Home
  0x13,		//PgUp
  0x04,		//Del
  0x12,		//End
  0x14,		//PgDn
  0x06,		//Right-Arr
  0x02,		//Left-Arr
  0x0E,		//Down-Arr
  0x10,		//Up-Arr
  0x01,		//NUMPAD ----NumLock but prints left<<<<<<
  0x2F,		// /
  0x2A,		// * (times?)
  0x2D,		// -
  0x2B,		// + (, = 2c)
  0x0D,		// ??? Enter?
  0x21,		// 1 !
  0x22,		// 2 "
  0x23,		// 3 #
  0x24,		// 4 $
  0x25,		// 5 %
  0x26,		// 6 &
  0x27,		// no 7 single quota '
  0x28,		// 8 (
  0x29,		// 9 )	
  0x20,		// Ins prints "space"
  0x2E,		// Del dot .2a
  0x0D		// not Enter? (+= 2b)
};

uint8_t const hid2asciiCtrl[] = {
  0x00, 	//0-3 empty
  0x00, 
  0x00, 
  0x00, 
  0x01, 	//^A <<<<--
  0x02, 	//^B <---
  0x03, 	//^C
  0x04, 	//^D del
  0x05, 	//^E
  0x06,		//^F --->
  0x07,
  0x08,		//^H BACKSPACE <-
  0x09,		//^I TAB
  0x0A,
  0x0B,
  0x0C,
  0x0D,		//^M CARRIAGE RETURN
  0x0E,		//^N down arrow
  0x0F,		//^O lock/unlock input
  0x10,		//^P up ^ arrow ^^^
  0x11,
  0x12,
  0x13,
  0x14,
  0x15,
  0x16,		//^V page down
  0x17,
  0x18,		//^X
  0x19,
  0x1A		//^Z
};

uint8_t const hid2asciiShift[] = {
  0x00, 	//0-3 empty
  0x00, 
  0x00, 
  0x00, 
  'A', 		//a..z 1..0
  'B',
  'C',
  'D',
  'E',
  'F',
  'G',
  'H',
  'I',
  'J',
  'K',
  'L',
  'M',
  'N',
  'O',
  'P',
  'Q',
  'R',
  'S',
  'T',
  'U',
  'V',
  'W',
  'X',
  'Y',
  'Z',
  '!',
  '@',
  '#',
  '$',
  '%',
  '^',
  '&',
  '*',
  '(',
  ')',
  0x0D, 	//enter
  0x1B,		//esc
  0x08,		//BS
  0x09,		//TAB
  ' ',
  '_',
  '+',
  '{',
  '}',
  '|',		// backslash
  '|',
  ':',
  '"',		// quote mark 22h
  '~',
  '<',
  '>',
  '?',
  0x00,		//CAPS
  0x00,		//F1
  0x00,		//F2
  0x00,		//F3
  0x00,		//F4
  0x00,		//F5
  0x00,		//F6
  0x00,		//F7
  0x00,		//F8
  0x00,		//F9
  0x00,		//F10
  0x00,		//F11
  0x00,		//F12
  0x00,		//PrtScr
  0x00,		//ScrlLock
  0x00,		//Pause/Break
  0x00,		//Ins
  0x00,		//Home
  0x00,		//PgUp
  0x00,		//Del
  0x00,		//End
  0x00,		//PgDn
  0x00,		//Right-Arr
  0x00,		//Left-Arr
  0x00,		//Down-Arr
  0x00,		//Up-Arr
  0x00,		//NUMPAD ----NumLock
  0x00,		// /
  0x00,		// *
  0x00,		// -
  0x00,		// +
  0x00,		// ???
  0x00,		// 1
  0x00,		// 2
  0x00,		// 3
  0x00,		// 4
  0x00,		// 5
  0x00,		// 6
  0x00,		// 7
  0x00,		// 8
  0x00,		// 9	
  0x00,		// Ins
  0x00,		// Del
  '>'
};


void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len) {
  printf("HID device address = %d, instance = %d is mounted\n", dev_addr, instance);

  switch(tuh_hid_interface_protocol(dev_addr, instance)) {
    case HID_ITF_PROTOCOL_KEYBOARD:
      #if DEBUG
      printf("HID Interface Protocol = Keyboard\n");
      #endif
      kbd_addr = dev_addr;
      kbd_inst = instance;
      tuh_hid_receive_report(dev_addr, instance);
    break;
    
    case HID_ITF_PROTOCOL_MOUSE:
      #if DEBUG
      printf("HID Interface Protocol = Mouse\n");
      #endif
      tuh_hid_receive_report(dev_addr, instance);
    break;
  }
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
  #if DEBUG
  printf("HID device address = %d, instance = %d is unmounted\r\n", dev_addr, instance);
  #endif
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) {

  uint8_t ch;
  const char fn[] = "\eO";

  
  switch(tuh_hid_interface_protocol(dev_addr, instance)) {
    case HID_ITF_PROTOCOL_KEYBOARD:  
      tuh_hid_receive_report(dev_addr, instance);
      if ((report[2]<=0x64) && (report[2]>=4)) {	//if valid HID code
          if (report[0] & 0x22) {			//if Shift
            ch = hid2asciiShift[report[2]];
          } else if (report[0] & 0x11) {		//if Ctrl
              if (report[2] <= 0x1A)
                ch = hid2asciiCtrl[report[2]];
              else
                ch = hid2ascii[report[2]];
            } else 
                ch = hid2ascii[report[2]];		//no Ctrl no Shift 
		
        };

      #if DEBUG
      //if ((report[2]<=0x45) && (report[2]>=0x3A)) printf("%c",0);
      if (ch) {
	if ((report[2]<=0x45) && (report[2]>=0x3A)) printf("%s %c", fn, ch);
	else	
	printf("%c",ch);
	};
      //printf("keyboard %02x %02x %02x  \n", report[0], report[1], report[2]);
      #endif
      tuh_hid_receive_report(dev_addr, instance);
    break;
    
    case HID_ITF_PROTOCOL_MOUSE:
      tuh_hid_receive_report(dev_addr, instance);
      #if DEBUG
      printf("mouse %02x %02x %02x \n", report[0], report[1], report[2]);
      #endif
      tuh_hid_receive_report(dev_addr, instance);
    break;
  }
}

void main() {

  board_init();

  #if DEBUG
  uart_init(UART_ID, BAUD_RATE);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  printf("\n usb2serial 0.1\n");
  #endif

  tusb_init();
  
  while(true) {
    tuh_task();
  }
}
