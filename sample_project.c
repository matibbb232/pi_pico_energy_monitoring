#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/interp.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/uart.h"
#include "pico/binary_info.h"
#include "pico/multicore.h"



// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9


int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}

#define ADC_NUM 0
#define ADC_PIN (26 + ADC_NUM)
#define ADC_VREF 3.3
#define ADC_RANGE (1 << 12)
#define ADC_CONVERT (ADC_VREF / (ADC_RANGE - 1))


void core1_main()
{
    while (1) {
        int data = multicore_fifo_pop_blocking();
        printf("hello world from second core\n");
        printf("This is data sent to other core:\n");
        //printf("%f\n", data);
        printf("%d\n", data);
        // sleep is unnecessary when used with fifo blocking
        // core will wait for data to be pushed
        //sleep_ms(1000);

        /// TODO: SEND THIS DATA TO SERVER 
    }
}

int main()
{
    stdio_init_all();

    // reading analog voltage
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(ADC_NUM);

    uint adc_raw;
    float i_max1 = 0, i_max2 = 0, i_max3 = 0;
    const float conversion_factor = 3.3f / (1 << 12);

    multicore_launch_core1(core1_main);
    printf("hello wow\n");

    uint32_t i = 0;


    while (1)
    {

        for (int i = 0; i < 10; ++i) {
            sleep_ms(100);
            uint16_t result = adc_read();

            //printf("Raw value: 0x%03x, voltage: %f V\n", result, result * conversion_factor);
            // offset 2.5 V   mV per Amp 100

            // converting voltage to amps read by current sensor
            float amps = (((result * conversion_factor) - 2.5) / 0.1); // needs to be tested

            // get the abs val of amps
            if (amps < 0) { 
                amps = (-1) * amps;
            }

            // stores 3 biggest values
            if (amps > i_max3) {
                if (amps > i_max2) {
                    if (amps > i_max1) {
                        i_max1 = amps;
                    } else {
                        i_max2 = amps;
                    }
                } else {
                    i_max3 = amps;
                }
            }

            printf("\namps: ");
            printf("%f\n", amps);

            printf("biggest amp values:\n");
            printf("%f\n", i_max1);
            printf("%f\n", i_max2);
            printf("%f\n", i_max3);
        }

        // average of 3 biggest values
        float i_max = (i_max1 + i_max2 + i_max3) / 3; 
        printf("\n****************\n");
        printf("i_max: ");
        printf("%f\n", i_max);
        printf("hello nice %u\n", i++);
        printf("****************\n");

        // pushing i_max to other core
        int data = i_max * 1e6; // fifo supports only intiger values
        multicore_fifo_push_blocking(data);
        sleep_ms(1000);
    }
}