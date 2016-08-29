#ifndef __FONTLIB_H__
#define __FONTLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "string.h"

/* Exported types ------------------------------------------------------------*/
typedef struct tFont
{
	const uint8_t* table;
	uint8_t Width;
	uint8_t Height;
}FONT_t;

extern FONT_t FONT_ARL_8;
extern FONT_t FONT_ARL_12;
extern FONT_t FONT_ARL_16;
extern FONT_t FONT_ARL_24;
extern FONT_t FONT_ARLBLK_8;
extern FONT_t FONT_ARLBLK_12;
extern FONT_t FONT_ARLBLK_16;
extern FONT_t FONT_ARLBLK_24;
extern FONT_t FONT_CLBR_8;
extern FONT_t FONT_CLBR_12;
extern FONT_t FONT_CLBR_16;
extern FONT_t FONT_CLBR_24;
extern FONT_t FONT_CMC_8;
extern FONT_t FONT_CMC_12;
extern FONT_t FONT_CMC_16;
extern FONT_t FONT_CMC_24;
extern FONT_t FONT_GRM_8;
extern FONT_t FONT_GRM_12;
extern FONT_t FONT_GRM_16;
extern FONT_t FONT_GRM_24;
extern FONT_t FONT_LCD_8;
extern FONT_t FONT_LCD_12;
extern FONT_t FONT_LCD_16;
extern FONT_t FONT_LCD_24;
extern FONT_t FONT_MSS_8;
extern FONT_t FONT_MSS_12;
extern FONT_t FONT_MSS_16;
extern FONT_t FONT_MSS_24;
extern FONT_t FONT_TMS_8;
extern FONT_t FONT_TMS_12;
extern FONT_t FONT_TMS_16;
extern FONT_t FONT_TMS_24;
extern FONT_t FONT_TMSBLD_8;
extern FONT_t FONT_TMSBLD_12;
extern FONT_t FONT_TMSBLD_16;
extern FONT_t FONT_TMSBLD_24;

/* Private function prototypes -----------------------------------------------*/
void GetASCIICode(uint8_t* pBuffer, uint8_t ASCII, struct tFont FONTx);

#ifdef __cplusplus
}
#endif

#endif

