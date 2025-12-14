#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#define GPIO_CHIP "/dev/gpiochip0"
#define NUM_PINS 3

int gpio_pins[NUM_PINS] = {17, 27, 22};

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line_settings *settings;
    struct gpiod_line_config *line_cfg;
    struct gpiod_request_config *req_cfg;
    struct gpiod_line_request *request;
    int values[NUM_PINS];
    int i;

    chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        perror("gpiod_chip_open");
        return 1;
    }

    settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);

    line_cfg = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(
        line_cfg, gpio_pins, NUM_PINS, settings);

    req_cfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_cfg, "multi-gpio");

    request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
    if (!request) {
        perror("gpiod_chip_request_lines");
        return 1;
    }

    while (1) {
        // All ON
        for (i = 0; i < NUM_PINS; i++) values[i] = 1;
        gpiod_line_request_set_values(request, values);
        sleep(1);

        // All OFF
        for (i = 0; i < NUM_PINS; i++) values[i] = 0;
        gpiod_line_request_set_values(request, values);
        sleep(1);

        // Chase
        for (i = 0; i < NUM_PINS; i++) {
            for (int j = 0; j < NUM_PINS; j++) values[j] = 0;
            values[i] = 1;
            gpiod_line_request_set_values(request, values);
            usleep(300000);
        }
    }

    gpiod_line_request_release(request);
    gpiod_chip_close(chip);
    return 0;
}
