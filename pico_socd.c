#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define UP_IN 0
#define DOWN_IN 1
#define LEFT_IN 2
#define RIGHT_IN 3

#define UP_OUT 21
#define DOWN_OUT 20
#define LEFT_OUT 19
#define RIGHT_OUT 18

int first_pressed_y = -1;
int first_pressed_x = -1;

bool up = true;
bool down = true;
bool left = true;
bool right = true;

bool previous_up = false;
bool previous_down = false;
bool previous_left = false;
bool previous_right = false;

void handle_lr_input();
void handle_ud_input();

int main() {
    gpio_init(UP_IN);
    gpio_init(DOWN_IN);
    gpio_init(LEFT_IN);
    gpio_init(RIGHT_IN);
    
    gpio_init(UP_OUT);
    gpio_init(DOWN_OUT);
    gpio_init(LEFT_OUT);
    gpio_init(RIGHT_OUT);

    gpio_set_dir(UP_IN, GPIO_IN);
    gpio_set_dir(DOWN_IN, GPIO_IN);
    gpio_set_dir(LEFT_IN, GPIO_IN);
    gpio_set_dir(RIGHT_IN, GPIO_IN);

    gpio_set_dir(UP_OUT, GPIO_OUT);
    gpio_set_dir(DOWN_OUT, GPIO_OUT);
    gpio_set_dir(LEFT_OUT, GPIO_OUT);
    gpio_set_dir(RIGHT_OUT, GPIO_OUT);

    gpio_pull_up(UP_IN);
    gpio_pull_up(DOWN_IN);
    gpio_pull_up(LEFT_IN);
    gpio_pull_up(RIGHT_IN);

    gpio_set_outover(UP_OUT, GPIO_OVERRIDE_INVERT);
    gpio_set_outover(DOWN_OUT, GPIO_OVERRIDE_INVERT);
    gpio_set_outover(LEFT_OUT, GPIO_OVERRIDE_INVERT);
    gpio_set_outover(RIGHT_OUT, GPIO_OVERRIDE_INVERT);

    while(true) {
        handle_lr_input();
        handle_ud_input();
    }
}


void handle_lr_input() {
    previous_left = left;
    previous_right = right;

    left = gpio_get(LEFT_IN);
    right = gpio_get(RIGHT_IN);

    if (previous_left == left && previous_right == right) {
        return;
    }

    if (!left && right) {
        first_pressed_x = LEFT_IN;
    }
    else if (left && !right) {
        first_pressed_x = RIGHT_IN;
    }
    else if (left && right) {
        first_pressed_x = -1;
    }
    else if (!left && !right && first_pressed_x == -1) {
        first_pressed_x = LEFT_IN;
    }

    if (left && right) {
        gpio_put(LEFT_OUT, 0);
        gpio_put(RIGHT_OUT, 0);
        return;
    }
    if (!left && right) {
        gpio_put(LEFT_OUT, 1);
        gpio_put(RIGHT_OUT, 0);
        return;
    }
    if (left && !right) {
        gpio_put(LEFT_OUT, 0);
        gpio_put(RIGHT_OUT, 1);
        return;
    }
    if (!left && !right && first_pressed_x == RIGHT_IN) {
        gpio_put(LEFT_OUT, 1);
        gpio_put(RIGHT_OUT, 0);
        return;
    }
    gpio_put(LEFT_OUT, 0);
    gpio_put(RIGHT_OUT, 1);
}

void handle_ud_input() {
    previous_up = up;
    previous_down = down;

    up = gpio_get(UP_IN);
    down = gpio_get(DOWN_IN);

    if (previous_up == up && previous_down == down) {
        return;
    }

    if (!up && down) {
        first_pressed_y = UP_IN;
    }
    else if (up && !down) {
        first_pressed_y = DOWN_IN;
    }
    else if (up && down) {
        first_pressed_y = -1;
    }
    else if (!up && !down && first_pressed_y == -1) {
        first_pressed_y = UP_IN;
    }

    if (up && down) {
        gpio_put(UP_OUT, 0);
        gpio_put(DOWN_OUT, 0);
        return;
    }
    if (!up && down) {
        gpio_put(UP_OUT, 1);
        gpio_put(DOWN_OUT, 0);
        return;
    }
    if (up && !down) {
        gpio_put(UP_OUT, 0);
        gpio_put(DOWN_OUT, 1);
        return;
    }
    if (!up && !down && first_pressed_y == DOWN_IN) {
        gpio_put(UP_OUT, 1);
        gpio_put(DOWN_OUT, 0);
        return;
    }
    gpio_put(UP_OUT, 0);
    gpio_put(DOWN_OUT, 1);
}
