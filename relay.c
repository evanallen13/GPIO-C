#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#define GPIO_CHIP "/dev/gpiochip0"
#define RELAY_GPIO 17   // BCM GPIO
#define ACTIVE_LOW 1    // set to 0 if your relay is active-high

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line_settings *settings;
    struct gpiod_line_config *line_cfg;
    struct gpiod_request_config *req_cfg;
    struct gpiod_line_request *request;
    int value;

    chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        perror("gpiod_chip_open");
        return 1;
    }

    settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);

    line_cfg = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(
        line_cfg, (int[]){RELAY_GPIO}, 1, settings);

    req_cfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_cfg, "relay-control");

    request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
    if (!request) {
        perror("gpiod_chip_request_lines");
        return 1;
    }

    while (1) {
        // RELAY ON
        value = ACTIVE_LOW ? 0 : 1;
        gpiod_line_request_set_values(request, &value);
        printf("Relay ON\n");
        sleep(2);

        // RELAY OFF
        value = ACTIVE_LOW ? 1 : 0;
        gpiod_line_request_set_values(request, &value);
        printf("Relay OFF\n");
        sleep(2);
    }

    gpiod_line_request_release(request);
    gpiod_chip_close(chip);
    return 0;
}
