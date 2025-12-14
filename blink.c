#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#define GPIO_CHIP "/dev/gpiochip0"
#define NUM_PINS 3

int gpio_pins[NUM_PINS] = {17, 27, 22};

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line *lines[NUM_PINS];
    int i;

    chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        perror("Open chip failed");
        return 1;
    }

    // Request each GPIO line as output
    for (i = 0; i < NUM_PINS; i++) {
        lines[i] = gpiod_chip_get_line(chip, gpio_pins[i]);
        if (!lines[i]) {
            perror("Get line failed");
            return 1;
        }

        if (gpiod_line_request_output(lines[i], "multi-gpio", 0) < 0) {
            perror("Request line as output failed");
            return 1;
        }
    }

    // Blink pattern
    while (1) {
        // Turn all ON
        for (i = 0; i < NUM_PINS; i++)
            gpiod_line_set_value(lines[i], 1);

        sleep(1);

        // Turn all OFF
        for (i = 0; i < NUM_PINS; i++)
            gpiod_line_set_value(lines[i], 0);

        sleep(1);

        // Chase pattern
        for (i = 0; i < NUM_PINS; i++) {
            gpiod_line_set_value(lines[i], 1);
            usleep(300000);
            gpiod_line_set_value(lines[i], 0);
        }
    }

    // Cleanup (never reached here, but good practice)
    for (i = 0; i < NUM_PINS; i++)
        gpiod_line_release(lines[i]);

    gpiod_chip_close(chip);
    return 0;
}
