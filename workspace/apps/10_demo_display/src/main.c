#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(display);

#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>
#include <lvgl_zephyr.h> // for lvgl_init
#include <string.h>

// Settings
static const int32_t sleep_time_ms = 50;        // Target 20 FPS

#if 0
const struct device *display;

#define DISPLAY_BUFFER_PITCH 128

int main(void)
{
    int ret = 0;

    // Initialize the display
    display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (!device_is_ready(display)) {
        printk("Error: display not ready\r\n");
        return 0;
    }

    struct display_capabilities capabilities;
    display_get_capabilities(display, &capabilities);

    const uint16_t x_res = capabilities.x_resolution;
    const uint16_t y_res = capabilities.y_resolution;

    LOG_INF("x_resolution: %d", x_res);
    LOG_INF("y_resolution: %d", y_res);
    LOG_INF("supported pixel formats: %d", capabilities.supported_pixel_formats);
    LOG_INF("screen_info: %d", capabilities.screen_info);
    LOG_INF("current_pixel_format: %d", capabilities.current_pixel_format);
    LOG_INF("current_orientation: %d", capabilities.current_orientation);
	 
    const struct display_buffer_descriptor buf_desc = {
        .width = x_res,
        .height = y_res,
        .buf_size = x_res * y_res,
        .pitch = DISPLAY_BUFFER_PITCH
    };

    uint8_t buf[1024] = {0};

    // Fill the screen with line strips of 4 pixels in height
    for(int i = 0; i < sizeof(buf); i++) {
        buf[i] = 0x0f;
    }

    ret = display_write(display, 0, 0, &buf_desc, buf);
    if (ret != 0) {
        LOG_ERR("could not write to display: %d", ret);
        return ret;
    }

    ret = display_set_contrast(display, 255);
    if (ret != 0) {
        LOG_ERR("could not set display contrast, %d", ret);
        return ret;
    }

    while (1) {
        k_msleep(sleep_time_ms);
    }
}

#else

int main(void)
{
    int ret = lvgl_init();
    if (ret < 0) {
        LOG_ERR("could initialize LVGL, %d", ret);
    }

    LOG_INF("LVGL initialized");

    uint32_t count = 0;
    char buf[11] = {0};
    const struct device *display;
    lv_obj_t *hello_label;
    lv_obj_t *counter_label;
    lv_obj_t *rect;
    lv_obj_t *circle;
    lv_style_t rect_style;
    lv_style_t circle_style;
    lv_point_precise_t rect_points[5] = { {20, 20}, {60, 20}, {60, 60}, {20, 60}, {20, 20} };
    const uint32_t circle_radius = 30;

    // Initialize the display
    display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (!device_is_ready(display)) {
        printk("Error: display not ready\r\n");
        return 0;
    }

    // Create a static label widget
    // hello_label = lv_label_create(lv_scr_act());
    // lv_label_set_text(hello_label, "Hello, World!");
    // lv_obj_align(hello_label, LV_ALIGN_TOP_MID, 0, 5);

    // Create a dynamic label widget
    // counter_label = lv_label_create(lv_scr_act());
    // lv_obj_align(counter_label, LV_ALIGN_BOTTOM_MID, 0, 0);

    // Set line style
    lv_style_init(&rect_style);
    lv_style_set_line_color(&rect_style, lv_color_white()); // lv_color_hex(0x0000FF));
    lv_style_set_line_width(&rect_style, 3);
    lv_style_set_bg_color(&rect_style, lv_color_black());

    // Create a rectangle out of lines
    rect = lv_line_create(lv_scr_act());
    lv_obj_add_style(rect, &rect_style, 0);
    lv_line_set_points(rect,
                       rect_points,
                       sizeof(rect_points) / sizeof(rect_points[0]));
    lv_obj_align(rect, LV_ALIGN_TOP_MID, 0, 0);

    // Set circle style
    lv_style_init(&circle_style);
    lv_style_set_radius(&circle_style, circle_radius);
    lv_style_set_border_width(&circle_style, 10);
    lv_style_set_border_color(&circle_style, lv_color_white());
    lv_style_set_bg_opa(&circle_style, LV_OPA_100);
    lv_style_set_bg_color(&circle_style, lv_color_black()); // lv_color_hex(0xFF0000));

    // Create an object with the new style
    circle = lv_obj_create(lv_scr_act());
    lv_obj_set_size(circle, circle_radius * 2, circle_radius * 2);
    lv_obj_add_style(circle, &circle_style, 0);
    lv_obj_align(circle, LV_ALIGN_CENTER, 0, 5);

    // Disable display blanking
    // display_blanking_off(display);

    // Do forever
    while (1) {

        // Update counter label every second
        // count++;
        // if ((count % (1000 / sleep_time_ms)) == 0) {
        //     sprintf(buf, "%d", count / (1000 / sleep_time_ms));
        //     lv_label_set_text(counter_label, buf);
        // }

        // Must be called periodically
        lv_task_handler();

        // Sleep
        k_msleep(sleep_time_ms);
    }
}

#endif