#include "mq135.h"
#include "math.h"

#define RL_VALUE 10.0  // Load resistance in kilo ohms
#define R0 10.0        // Base resistance for calibration

// Constants for different gases (based on the MQ135 datasheet or calibration)
#define PPM_CO2_CONST 116.6020682
#define PPM_SMOKE_CONST 2.3
#define PPM_NOX_CONST 1.5
#define PPM_O3_CONST 0.5

// Exponent values for each gas (based on the MQ135 characteristics)
#define CO2_EXPONENT -2.769034857
#define SMOKE_EXPONENT -1.497
#define NOX_EXPONENT -0.8
#define O3_EXPONENT -1.15

ADC_HandleTypeDef* hadc_global;

void MQ135_Init(ADC_HandleTypeDef* hadc) {
    hadc_global = hadc;
}

float calculateResistance(float adc_value) {
    float voltage = (adc_value / 4095.0) * 3.3;
    return ((3.3 - voltage) / voltage) * RL_VALUE;
}

float calculatePPM(float resistance, float ppm_const, float exponent) {
    return ppm_const * pow((resistance / R0), exponent);
}

float MQ135_ReadPPM(void) {
    // Start ADC Conversion and wait for it to finish
    HAL_ADC_Start(hadc_global);
    HAL_ADC_PollForConversion(hadc_global, HAL_MAX_DELAY);
    uint32_t adc_value = HAL_ADC_GetValue(hadc_global);

    // Calculate resistance
    float resistance = calculateResistance(adc_value);

    // Calculate individual PPM values for different gases
    float ppm_co2 = calculatePPM(resistance, PPM_CO2_CONST, CO2_EXPONENT);
    float ppm_smoke = calculatePPM(resistance, PPM_SMOKE_CONST, SMOKE_EXPONENT);
    float ppm_nox = calculatePPM(resistance, PPM_NOX_CONST, NOX_EXPONENT);
    float ppm_o3 = calculatePPM(resistance, PPM_O3_CONST, O3_EXPONENT);

    // Weighted average or combined PPM (customize the weights as necessary)
    float combined_ppm = (ppm_co2 * 0.4) + (ppm_smoke * 0.3) + (ppm_nox * 0.2) + (ppm_o3 * 0.1);

    return combined_ppm;
}
