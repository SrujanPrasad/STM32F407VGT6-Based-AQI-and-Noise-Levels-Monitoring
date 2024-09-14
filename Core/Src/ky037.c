#include "ky037.h"
#include "math.h"

#define REFERENCE_VOLTAGE 3.3
#define ADC_RESOLUTION 4095.0
#define REFERENCE_DECIBEL 30.0  // Reference dB level for a specific voltage (based on calibration)

ADC_HandleTypeDef* hadc_global_ky037;

void KY037_Init(ADC_HandleTypeDef* hadc) {
    hadc_global_ky037 = hadc;
}

float KY037_ReadDecibels(void) {
    // Start ADC Conversion and wait for it to finish
    HAL_ADC_Start(hadc_global_ky037);
    HAL_ADC_PollForConversion(hadc_global_ky037, HAL_MAX_DELAY);
    uint32_t adc_value = HAL_ADC_GetValue(hadc_global_ky037);

    // Convert ADC value to voltage
    float voltage = (adc_value / ADC_RESOLUTION) * REFERENCE_VOLTAGE;

    // Calculate decibels using a logarithmic scale
    // Assumes that the sensor output is logarithmic in nature with respect to the sound level
    // This might require calibration to match the sensor's characteristics
    float decibels = REFERENCE_DECIBEL + 20 * log10(voltage / REFERENCE_VOLTAGE);

    return decibels;
}
