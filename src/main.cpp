#include "stm32g431xx.h"
#include "stm32g4xx_LL_rcc.h"
#include "stm32g4xx_LL_bus.h"
#include "stm32g4xx_LL_spi.h"
#include "stm32g4xx_LL_gpio.h"
#include "stm32g4xx_LL_pwr.h"
#include "stm32g4xx_LL_system.h"
#include "stm32g4xx_LL_tim.h"

int main(){
    //включаем внутрений источник частоты для работы впринципе контроллера
    LL_RCC_HSI_Enable();
    while(LL_RCC_HSI_IsReady() != 1);
    //включаем тактирование на pwr модуль отвечающй за управление питанием и включаем тактирование памяти 
    // по скольку видимо на всех stm32 там есть режимы работы памяти
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    while(LL_APB1_GRP1_IsEnabledClock(LL_APB1_GRP1_PERIPH_PWR) != 1);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_FLASH);
    while(LL_AHB1_GRP1_IsEnabledClock(LL_AHB1_GRP1_PERIPH_FLASH) != 1);
    LL_FLASH_EnablePrefetch();
    LL_FLASH_EnableInstCache();
    LL_FLASH_EnableDataCache();
    //по мануалу необходимо сначала выставить AHB делтель на 2 пред выставлением частоты на 170 мгц
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);
    // включаем буст по питанию 
    LL_PWR_EnableRange1BoostMode();
    while(LL_PWR_IsActiveFlag_VOS() == 1); 
    //включаем другой режим работы памяти
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
    while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_4);
    //включаем внешний источник тактирования и убеждаемся в его работе 
    LL_RCC_HSE_Enable();
    while(LL_RCC_HSE_IsReady() != 1);
    // выключаем PLL настраиваем его
    LL_RCC_PLL_Disable();
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE,LL_RCC_PLLM_DIV_2, 85, LL_RCC_PLLR_DIV_2);
    // выходная частота обязаня делиться минимум на 2 на SYSTEMCLK п этому мы входное HSE 
    // делим на 2 что бы получить 4 и умножаем на 85 получаем 340 при делении на 2 получаем максимальную частоту ядра 
    // 170 МГц
    LL_RCC_PLL_EnableDomain_SYS();
    LL_RCC_PLL_Enable();
    while(LL_RCC_PLL_IsReady() != 1);
    // выбор системного источника PLL
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);
    // выставляем делители шин тактирования но предварительно дожидаемся стабилизации 
    for(volatile int i = 0; i < 1000; i++) {
        __NOP();
    }
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    //включаем тактирования линий 
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    while(LL_AHB2_GRP1_IsEnabledClock(LL_AHB2_GRP1_PERIPH_GPIOA) != 1);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
    while(LL_AHB2_GRP1_IsEnabledClock(LL_AHB2_GRP1_PERIPH_GPIOB) != 1);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
    while(LL_APB1_GRP1_IsEnabledClock(LL_APB1_GRP1_PERIPH_SPI2) != 1);
    //настраиваем все пины для управления экраном 
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_6, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_VERY_HIGH);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_6, LL_GPIO_PULL_DOWN);
    //-----------------------
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_10, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_10, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_VERY_HIGH);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_10, LL_GPIO_PULL_DOWN);
    //-----------------------
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_11, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_11, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_11, LL_GPIO_SPEED_FREQ_VERY_HIGH);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_11, LL_GPIO_PULL_DOWN);
    //-----------------------
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_12, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_12, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_12, LL_GPIO_SPEED_FREQ_VERY_HIGH);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_12, LL_GPIO_PULL_DOWN);
    //-----------------------
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_13, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_13, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_13, LL_GPIO_SPEED_FREQ_VERY_HIGH);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_13, LL_GPIO_PULL_NO);
    LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_13, LL_GPIO_AF_5);
    //-----------------------
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_14, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_14, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_14, LL_GPIO_SPEED_FREQ_VERY_HIGH);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_14, LL_GPIO_PULL_UP);
    LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_14, LL_GPIO_AF_5);
    //-----------------------
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_15, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_15, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_15, LL_GPIO_SPEED_FREQ_VERY_HIGH);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_15, LL_GPIO_PULL_NO);
    LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_15, LL_GPIO_AF_5);
    // выключаем spi для настройки 
    LL_SPI_Disable(SPI2);
    LL_SPI_SetMode(SPI2, LL_SPI_MODE_MASTER);
    LL_SPI_SetStandard(SPI2, LL_SPI_PROTOCOL_MOTOROLA);
    LL_SPI_SetClockPhase(SPI2, LL_SPI_PHASE_1EDGE);
    LL_SPI_SetClockPolarity(SPI2, LL_SPI_POLARITY_LOW);
    LL_SPI_SetBaudRatePrescaler(SPI2, LL_SPI_BAUDRATEPRESCALER_DIV16);
    LL_SPI_SetTransferBitOrder(SPI2, LL_SPI_MSB_FIRST);
    LL_SPI_SetTransferDirection(SPI2, LL_SPI_FULL_DUPLEX);
    LL_SPI_SetDataWidth(SPI2, LL_SPI_DATAWIDTH_8BIT);
    LL_SPI_SetRxFIFOThreshold(SPI2, LL_SPI_RX_FIFO_TH_QUARTER);
    LL_SPI_SetNSSMode(SPI2, LL_SPI_NSS_SOFT);
    LL_SPI_Enable(SPI2);
    while(true){

    }
    //LL_SPI_TransmitData8();
}