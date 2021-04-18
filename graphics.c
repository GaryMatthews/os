#include <graphics.h>

#include <video.h>
#include <mouse.h>
#include <kheap.h>
#include <rtc.h>

#include <microui.h>
#include <renderer.h>
#include <printf.h>
#include <string.h>
#include <keyboard.h>
#include <pcspk.h>
#include <commands.h>

short mouse_icon[] =  {
        1,0,0,0,0,0,0,0,0,0,0,
        1,1,0,0,0,0,0,0,0,0,0,
        1,2,1,0,0,0,0,0,0,0,0,
        1,2,2,1,0,0,0,0,0,0,0,
        1,2,2,2,1,0,0,0,0,0,0,
        1,2,2,2,2,1,0,0,0,0,0,
        1,2,2,2,2,2,1,0,0,0,0,
        1,2,2,2,2,2,2,1,0,0,0,
        1,2,2,2,2,2,2,2,1,0,0,
        1,2,2,2,2,2,1,1,1,1,0,
        1,2,2,2,2,2,2,1,0,0,0,
        1,2,2,1,2,2,2,1,0,0,0,
        1,2,1,0,1,2,2,2,1,0,0,
        1,1,0,0,0,1,2,2,2,1,0,
        0,0,0,0,0,0,1,2,2,1,0,
        0,0,0,0,0,0,0,1,1,0,0,
};

uint32_t mouse_color_mapping[] = {0, 0, 0xFFFFFFFF};

void paint_mouse() {
    short* buf = mouse_icon;
    int mouse_x = get_mouse_info()->x;
    int mouse_y = get_mouse_info()->y;
    for (int i=0; i<16; i++) {
        for (int j=0; j<11; j++) {
            if (*buf) {
                uint32_t color = mouse_color_mapping[*buf];
                if (mouse_x + j >= 0 && mouse_x + j < 640 &&
                    mouse_y + i >= 0 && mouse_y + i < 480) {
                    draw_pixel(mouse_x + j, mouse_y + i, color);
                }
            }
            buf++;
        }
    }
}

mu_Context ctx;

static int text_width(mu_Font font, const char *text, int len) {
  if (len == -1) { len = strlen(text); }
  return r_get_text_width(text, len);
}

static int text_height(mu_Font font) {
  return r_get_text_height();
}

static  char logbuf[64000];
static   int logbuf_updated = 0;

void write_log(const char *text) {
    strcat(logbuf, text);
    logbuf_updated = 1;
}

void mu_2() {
    mu_begin(&ctx);
    if (mu_begin_window(&ctx, "hello, world", mu_rect(10, 10, 240, 56))) {
        mu_layout_row(&ctx, 2, (int[]) { 100, 100 }, 0);
        if (mu_button(&ctx, "beep on")) {
            beep(400);
        }
        if (mu_button(&ctx, "beep off")) {
            beep_off();
        }
        mu_end_window(&ctx);
    }

    if (mu_begin_window(&ctx, "console", mu_rect(50, 100, 500, 300))) {
        mu_layout_row(&ctx, 1, (int[]) { -1 }, -25);
        mu_begin_panel(&ctx, "Log Output");
        mu_Container *panel = mu_get_current_container(&ctx);
        mu_layout_row(&ctx, 1, (int[]) { -1 }, -1);
        mu_text(&ctx, logbuf);
        mu_end_panel(&ctx);
        if (logbuf_updated) {
            panel->scroll.y = panel->content_size.y;
            logbuf_updated = 0;
        }
        /* input textbox + submit button */
        static char buf[128];
        int submitted = 0;
        mu_layout_row(&ctx, 2, (int[]) { -70, -1 }, 0);
        if (mu_textbox(&ctx, buf, sizeof(buf)) & MU_RES_SUBMIT) {
            mu_set_focus(&ctx, ctx.last_id);
            submitted = 1;
        }
        if (mu_button(&ctx, "Submit")) { submitted = 1; }
        if (submitted) {
            printf("%s\n", buf);
            console_exec(buf);
            buf[0] = '\0';
        }
        mu_end_window(&ctx);
    }

    mu_end(&ctx);
}

void mu() {
    mu_init(&ctx);
    ctx.text_width = text_width;
    ctx.text_height = text_height;
}

void paint_desktop() {
    draw_rect(0, 0, 640, 480, 0x2D);

    rtc_read_datetime();
    char* dt = get_current_datetime_str();
    draw_string(8*65, 0, dt);
    kfree(dt);

    if (mouse_left_button_down()) {
        mu_input_mousedown(&ctx, get_mouse_info()->x, get_mouse_info()->y, MU_MOUSE_LEFT);
    }
    if (mouse_left_button_up()) {
        mu_input_mouseup(&ctx, get_mouse_info()->x, get_mouse_info()->y, MU_MOUSE_LEFT);
    }

    if (mouse_right_button_down()) {
        mu_input_mousedown(&ctx, get_mouse_info()->x, get_mouse_info()->y, MU_MOUSE_RIGHT);
    }
    if (mouse_right_button_up()) {
        mu_input_mouseup(&ctx, get_mouse_info()->x, get_mouse_info()->y, MU_MOUSE_RIGHT);
    }

    mouse_info.prev_button = mouse_info.curr_button;
    
    mu_input_mousemove(&ctx, get_mouse_info()->x, get_mouse_info()->y);

    char c = keyboard_get_lastkey();
    if (c == 10) {
        mu_input_keydown(&ctx, MU_KEY_RETURN);
        mu_input_keyup(&ctx, MU_KEY_RETURN);
        keyboard_invalidate_lastkey();
    } else if (c == 8) {
        mu_input_keydown(&ctx, MU_KEY_BACKSPACE);
        mu_input_keyup(&ctx, MU_KEY_BACKSPACE);
        keyboard_invalidate_lastkey();
    } else if (c != 0) {
        char buf[2];
        buf[0] = c;
        buf[1] = 0;
        keyboard_invalidate_lastkey();
        mu_input_text(&ctx, buf);
        // printf("key: %d\n", c);
    }

    mu_2();

    mu_Command *cmd = NULL;
    while (mu_next_command(&ctx, &cmd)) {
        if (cmd->type == MU_COMMAND_TEXT) {
            r_draw_text(cmd->text.str, cmd->text.pos, cmd->text.color);
        }
        if (cmd->type == MU_COMMAND_RECT) {
            r_draw_rect(cmd->rect.rect, cmd->rect.color);
        }
        if (cmd->type == MU_COMMAND_ICON) {
            r_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color);
        }
        if (cmd->type == MU_COMMAND_CLIP) {
            r_set_clip_rect(cmd->clip.rect);
        }
    }
    
    paint_mouse();
}
