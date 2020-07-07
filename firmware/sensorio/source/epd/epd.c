/**
 * @file epd.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "epd.h"
#include "epd_comm.h"
#include "epd_1_9.h"

#include <string.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static uint8_t EPD_readCogId(void)
{
  uint8_t rid = 0x71;
  uint8_t id  = 0;

  EPD_SpiSelect();
  EPD_SpiSend(&rid, 1);
  EPD_SpiReceive(&id, 1);
  EPD_SpiUnselect();

  return id;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void EPD_Write(uint8_t cmd, const uint8_t data[], size_t length)
{
  uint8_t cmdheader  = 0x70;
  uint8_t dataheader = 0x72;

  EPD_SpiSelect();
  EPD_SpiSend(&cmdheader, 1);
  EPD_SpiSend(&cmd, 1);
  EPD_SpiUnselect();

  // chThdSleepMicroseconds(1);

  EPD_SpiSelect();
  EPD_SpiSend(&dataheader, 1);
  EPD_SpiSend(data, length);
  EPD_SpiUnselect();
}

void EPD_Read(uint8_t cmd, uint8_t data[], size_t length)
{
  uint8_t cmdheader  = 0x70;
  uint8_t dataheader = 0x73;

  EPD_SpiSelect();
  EPD_SpiSend(&cmdheader, 1);
  EPD_SpiSend(&cmd, 1);
  EPD_SpiUnselect();

  //chThdSleepMicroseconds(1);

  EPD_SpiSelect();
  EPD_SpiSend(&dataheader, 1);
  EPD_SpiReceive(data, length);
  EPD_SpiUnselect();
}

void EPD_PowerOn(void)
{
  // Power ON
  EPD_DischargeLow();
  EPD_VccOff();
  EPD_SleepMilliseconds(100);

  EPD_VccOn();
  EPD_SpiUnselect();

  EPD_ResetOff();
  EPD_SleepMilliseconds(5);

  EPD_ResetOn();
  EPD_SleepMilliseconds(5);

  EPD_ResetOff();
  EPD_SleepMilliseconds(5);
}

void EPD_Initialize(void)
{
  bool busy = true;

  while (busy) {
    busy = EPD_IsBusy();
    EPD_SleepMilliseconds(1);
  }

  // Read COG ID
  uint8_t id = EPD_readCogId();
  if (0x12 != id) {
    EPD_Abort("wrong chip-id");
  }

  uint8_t cmd;
  uint8_t wrdata[10];
  uint8_t rddata[10];

  // Disable OE
  cmd       = 0x02;
  wrdata[0] = 0x40;
  EPD_Write(cmd, wrdata, 1);

  // Breakage detection
  cmd = 0x0f;
  EPD_Read(cmd, rddata, 1);

  if (0x80 != (rddata[0] & 0x80))
    EPD_Abort("panel broken");

  // Power saving mode
  cmd       = 0x0b;
  wrdata[0] = 0x02;
  EPD_Write(cmd, wrdata, 1);

  // Channel select
  // (0x0000,0003,FC00,00FF)
  cmd       = 0x01;
  wrdata[0] = 0x00;
  wrdata[1] = 0x00;
  wrdata[2] = 0x00;
  wrdata[3] = 0x03;
  wrdata[4] = 0xfc;
  wrdata[5] = 0x00;
  wrdata[6] = 0x00;
  wrdata[7] = 0xff;
  EPD_Write(cmd, wrdata, 8);

  // High power mode
  cmd       = 0x07;
  wrdata[0] = 0xd1;
  EPD_Write(cmd, wrdata, 1);

  // Power setting
  cmd       = 0x08;
  wrdata[0] = 0x02;
  EPD_Write(cmd, wrdata, 1);

  // Set VCOM level
  cmd       = 0x09;
  wrdata[0] = 0xc2;
  EPD_Write(cmd, wrdata, 1);

  // Power setting
  cmd       = 0x04;
  wrdata[0] = 0x03;
  EPD_Write(cmd, wrdata, 1);

  // Driver latch on
  cmd       = 0x03;
  wrdata[0] = 0x01;
  EPD_Write(cmd, wrdata, 1);

  // Driver latch off
  cmd       = 0x03;
  wrdata[0] = 0x00;
  EPD_Write(cmd, wrdata, 1);

  EPD_SleepMilliseconds(5);

  bool ready = false;
  size_t i   = 0;

  for (i = 1; !ready; ++i) {
    if (4 <= i)
      EPD_Abort("charge pump failed");

    // Start charge pump positive voltage
    cmd       = 0x05;
    wrdata[0] = 0x01;
    EPD_Write(cmd, wrdata, 1);

    EPD_SleepMilliseconds(170);

    // Start charge pump negative voltage
    cmd       = 0x05;
    wrdata[0] = 0x03;
    EPD_Write(cmd, wrdata, 1);

    EPD_SleepMilliseconds(110);

    // Set charge pump VCOM on
    cmd       = 0x05;
    wrdata[0] = 0x0f;
    EPD_Write(cmd, wrdata, 1);

    EPD_SleepMilliseconds(60);

    cmd = 0x0f;
    EPD_Read(cmd, rddata, 1);

    if (0x40 == (rddata[0] & 0x40))
      ready = true;
  }

  // Output enable to disable
  cmd       = 0x02;
  wrdata[0] = 0x06;
  EPD_Write(cmd, wrdata, 1);
}

void EPD_UpdateDisplay(const uint8_t oldimage[128][144/8], const uint8_t newimage[128][144/8])
{
  size_t i = 0;
  for(i = 0; i < 4; ++i)
    EPD_WriteFrame(oldimage, EPD_STAGE_1);

  for(i = 0; i < 4; ++i)
    EPD_WriteFrame(oldimage, EPD_STAGE_2);

  for(i = 0; i < 4; ++i)
    EPD_WriteFrame(newimage, EPD_STAGE_3);   

  for(i = 0; i < 4; ++i)
    EPD_WriteFrame(newimage, EPD_STAGE_4);          
}
void EPD_PartialUpdate(const uint8_t oldimage[128][144/8], const uint8_t newimage[128][144/8])
{
  size_t i = 0;
  for(i = 0; i < 4; ++i)
    EPD_WritePartialUpdateFrame(oldimage, newimage);
}

void EPD_PowerOff(void)
{
  EPD_WriteNothingFrame();

  // TODO distinguish different display sizes here

  EPD_WriteBorderDummyLine();

  EPD_SleepMilliseconds(200);

  uint8_t cmd = 0x0B;
  uint8_t data = 0x00;
  EPD_Write(cmd, &data, 1);

  // Latch reset turn on
  cmd = 0x03;
  data = 0x01;
  EPD_Write(cmd, &data, 1);

  // Power off chargepump Vcom off
  cmd = 0x05;
  data = 0x03;
  EPD_Write(cmd, &data, 1);

  //Power off chargepump negative voltage VGL & VDL off
  cmd = 0x05;
  data = 0x01;
  EPD_Write(cmd, &data, 1);

  // Discharge internal
  cmd = 0x04;
  data = 0x80;
  EPD_Write(cmd, &data, 1);

  // Power off chargepump positive voltage VGH & VDH off
  cmd = 0x05;
  data = 0x00;
  EPD_Write(cmd, &data, 1);

  // Turn off osc
  cmd = 0x07;
  data = 0x01;
  EPD_Write(cmd, &data, 1);

  // Set Signals = 0 ( SI, SCLK, /BORDER_CONTROL *3 )
  EPD_MosiLow();
  EPD_SclkLow();

  EPD_VccOff();
  EPD_SleepMilliseconds(10);

  EPD_ResetOn();
  EPD_SpiUnselect();

  EPD_DischargeHigh();
  EPD_SleepMilliseconds(200);
  EPD_DischargeLow();
}

/****************************** END OF FILE **********************************/
