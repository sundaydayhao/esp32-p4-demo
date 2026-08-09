#include "bsp/esp-bsp.h"
#include "esp_log.h"
#include "lvgl.h"

static const char *TAG = "sb_display";

static void add_bar(lv_obj_t *parent, int32_t x, int32_t y, int32_t w, int32_t h)
{
    lv_obj_t *bar = lv_obj_create(parent);
    lv_obj_set_pos(bar, x, y);
    lv_obj_set_size(bar, w, h);
    lv_obj_set_style_bg_color(bar, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(bar, 0, 0);
    lv_obj_set_style_radius(bar, 0, 0);
    lv_obj_clear_flag(bar, LV_OBJ_FLAG_SCROLLABLE);
}

static void draw_block_s(lv_obj_t *parent, int32_t x, int32_t y, int32_t w, int32_t h, int32_t t)
{
    add_bar(parent, x, y, w, t);
    add_bar(parent, x, y, t, h / 2);
    add_bar(parent, x, y + h / 2 - t / 2, w, t);
    add_bar(parent, x + w - t, y + h / 2, t, h / 2);
    add_bar(parent, x, y + h - t, w, t);
}

static void draw_block_b(lv_obj_t *parent, int32_t x, int32_t y, int32_t w, int32_t h, int32_t t)
{
    add_bar(parent, x, y, t, h);
    add_bar(parent, x, y, w - t, t);
    add_bar(parent, x, y + h / 2 - t / 2, w - t, t);
    add_bar(parent, x, y + h - t, w - t, t);
    add_bar(parent, x + w - t, y + t, t, h / 2 - t);
    add_bar(parent, x + w - t, y + h / 2 + t / 2, t, h / 2 - t - t / 2);
}

static void show_sb(lv_obj_t *scr)
{
    lv_obj_clean(scr);
    lv_obj_set_style_bg_color(scr, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
    lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

    const int32_t screen_w = lv_disp_get_hor_res(NULL);
    const int32_t screen_h = lv_disp_get_ver_res(NULL);
    const int32_t margin_x = screen_w / 14;
    const int32_t letter_gap = screen_w / 18;
    const int32_t top = screen_h / 9;
    const int32_t height = screen_h - top * 2;
    const int32_t thickness = height / 7;
    const int32_t letter_w = (screen_w - margin_x * 2 - letter_gap) / 2;

    draw_block_s(scr, margin_x, top, letter_w, height, thickness);
    draw_block_b(scr, margin_x + letter_w + letter_gap, top, letter_w, height, thickness);
}

void app_main(void)
{
    bsp_display_start();

    ESP_LOGI(TAG, "Show big SB on ESP32-P4 Function EV Board LCD");
    bsp_display_lock(0);
    lv_obj_t *scr = lv_disp_get_scr_act(NULL);
    show_sb(scr);
    bsp_display_unlock();

    bsp_display_backlight_on();
}