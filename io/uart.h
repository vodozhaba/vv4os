/******************************************************************************
 * File:       /vv4os/io/uart.h                                              *
 * Author:     vodozhaba                                                      *
 * Created on: Jun 16, 2017                                                   *
 * Purpose:    Allows working with serial ports.                              *
 ******************************************************************************/

void UartInit(int freq);
char UartRead();
void UartSend(char c);
