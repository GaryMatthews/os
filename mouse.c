/*
 *  Copyright 2016 Davide Pianca
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <mouse.h>
#include <io.h>
#include <idt.h>
//XXX#include <hal/hal.h>
#include <printf.h>

uint8_t mouse_cycle = 0;
char mouse_byte[3];

extern void mouse_int();

void mouse_wait(uint8_t type) {
    uint32_t timeout = 100000;
    uint32_t expect = (type == 1) ? 1 : 0;
    while(timeout--) {
        if((inportb(MOUSE_STATUS) & type) == expect)
            return;
    }
    return;
}

void mouse_write(uint8_t write) {
    mouse_wait(1);
    outportb(MOUSE_STATUS, MOUSE_WRITE);
    mouse_wait(1);
    outportb(MOUSE_PORT, write);
}

uint8_t mouse_read() {
    mouse_wait(2);
    return inportb(MOUSE_PORT);
}

static mouse_info_t info;

mouse_info_t *get_mouse_info() {
    return &info;
}

extern void set_pixel(uint32_t val, int x, int y);

void mouse_handler() {
    uint8_t status = inportb(MOUSE_STATUS);
    while(status & MOUSE_BBIT) {
        char mouse_in = inportb(MOUSE_PORT);
        if(status & MOUSE_F_BIT) {
            switch(mouse_cycle) {
                case 0:
                    mouse_byte[0] = mouse_in;
                    if(!(mouse_in & MOUSE_V_BIT))
                        goto read_next;
                    mouse_cycle++;
                    break;
                case 1:
                    mouse_byte[1] = mouse_in;
                    mouse_cycle++;
                    break;
                case 2:
                    mouse_byte[2] = mouse_in;
                    if(mouse_byte[0] & 0x80 || mouse_byte[0] & 0x40)
                        goto read_next;
                    info.x += mouse_byte[1];
                    info.y -= mouse_byte[2];
                    mouse_check_bounds();
                    
                    if(mouse_byte[0] & LEFT_CLICK)
                        info.button = LEFT_CLICK;
                    else if(mouse_byte[0] & RIGHT_CLICK)
                        info.button = RIGHT_CLICK;
                    else if(mouse_byte[0] & MIDDLE_CLICK)
                        info.button = MIDDLE_CLICK;
                    mouse_cycle = 0;

                    //printf("mouse_handler x %d y %d\n", info.x, info.y);
                    set_pixel(0xffffff, info.x, info.y);
                    
                    break;
            }
        }
read_next:
        status = inportb(MOUSE_STATUS);
    }
}

void mouse_check_bounds() {
    if(info.x > 640-1)
        info.x = 640-1;
    else if(info.x < 0)
        info.x = 0;
    if(info.y > 480-1)
        info.y = 480-1;
    else if(info.y < 0)
        info.y = 0;
}

void mouse_init() {
    info.x = 0;
    info.y = 0;
    uint8_t status;
    //disable_int();
    mouse_wait(1);
    outportb(MOUSE_STATUS, 0xA8);
    mouse_wait(1);
    outportb(MOUSE_STATUS, 0x20);
    mouse_wait(2);
    status = inportb(0x60) | 2;
    mouse_wait(1);
    outportb(MOUSE_STATUS, 0x60);
    mouse_wait(1);
    outportb(MOUSE_PORT, status);
    mouse_write(0xF6);
    mouse_read();
    mouse_write(0xF4);
    mouse_read();
    install_ir(44, 0x80 | 0x0E, 0x8, &mouse_int);
    //enable_int();
}
