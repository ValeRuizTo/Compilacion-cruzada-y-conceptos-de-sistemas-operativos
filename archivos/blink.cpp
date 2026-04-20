#include <gpiod.h>
#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
    const char *chip_path = "/dev/gpiochip0";
    unsigned int line_num = 17;

    gpiod_chip *chip;
    gpiod_line_request *request;
    gpiod_line_settings *settings;
    gpiod_line_config *line_cfg;
    gpiod_request_config *req_cfg;

    // Abrir chip
    chip = gpiod_chip_open(chip_path);
    if (!chip) {
        perror("Error abriendo chip");
        return 1;
    }

    // Configuración de línea
    settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);

    line_cfg = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(line_cfg, &line_num, 1, settings);

    req_cfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_cfg, "blink");

    // Solicitar línea
    request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
    if (!request) {
        perror("Error solicitando línea");
        return 1;
    }

    // Loop
    while (true) {
        gpiod_line_request_set_value(request, line_num, GPIOD_LINE_VALUE_ACTIVE);
        cout << "ON" << endl;
        sleep(1);

        gpiod_line_request_set_value(request, line_num, GPIOD_LINE_VALUE_INACTIVE);
        cout << "OFF" << endl;
        sleep(1);
    }

    // Liberar recursos
    gpiod_line_request_release(request);
    gpiod_chip_close(chip);

    return 0;
}
