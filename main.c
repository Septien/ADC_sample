#include <stdio.h>
#include <string.h>

#include "periph/adc.h"
#include "analog_util.h"
#include "ztimer.h"
#include "periph/gpio.h"
// lcd driver
#include "hd44780.h"

#define LM35_PIN ADC_LINE(10) // Use gpio 32
#define LS_PIN   ADC_LINE(11) // Use gpio 33
#define POT_PIN  ADC_LINE(12) // Use gpio 34

#define ADC_VREF_mV 3300.0
#define ADC_RES ADC_RES_12BIT

int main(void)
{
    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    printf("This board features a(n) %s MCU", RIOT_MCU);

    if (adc_init(LM35_PIN) < 0) {
        printf("Failed to initialize ADC for temperature sensor.\n");
        return -1;
    }
    if (adc_init(LS_PIN) < 0) {
        printf("Failed to initialized ADC for light sensor.\n");
        return -1;
    }
    if (adc_init(POT_PIN) < 0) {
        printf("Failed to initialized ADC for potentiometer.\n");
        return -1;
    }

    // Configure lcd
    hd44780_params_t params = {
        .cols = 16,
        .rows = 2,
        .rs = GPIO19,
        .rw = GPIO_UNDEF,
        .enable = GPIO23,
        .data = {GPIO18, GPIO17, GPIO16, GPIO15, GPIO_UNDEF, GPIO_UNDEF, GPIO_UNDEF, GPIO_UNDEF}
    };
    hd44780_t _lcd, *lcd;
    lcd = &_lcd;

    hd44780_init(lcd, &params);
    hd44780_clear(lcd);
    hd44780_home(lcd);

    int32_t sample_lm35 = 0;
    int32_t sample_light = 0;
    int32_t sample_pot = 0;
    while (1) {
        sample_lm35 = adc_sample(LM35_PIN, ADC_RES);
        float temp = adc_util_mapf(sample_lm35, ADC_RES, 0, 3.3);
        sample_light = adc_sample(LS_PIN, ADC_RES);
        float v = adc_util_mapf(sample_light, ADC_RES, 0, 3.3);
        sample_pot = adc_sample(POT_PIN, ADC_RES);
        float val = adc_util_mapf(sample_pot, ADC_RES, 0, 3.3);

        printf("Temperature: %f.\n", temp);
        printf("Ligh sensor: %f.\n", v);
        printf("Potentiometer voltage: %f.\n", val);

        char str[16] = {0};
        hd44780_set_cursor(lcd, 0, 0);
        sprintf(str, "T:%2.2f P:%.2f", temp, val);
        hd44780_print(lcd, str);
        hd44780_set_cursor(lcd, 0, 1);
        memset(str, 0, 16);
        sprintf(str, "L:%2.2f RIOT OS!", v);
        hd44780_print(lcd, str);

        ztimer_sleep(ZTIMER_MSEC, 2000);
    }
    return 0;
}
