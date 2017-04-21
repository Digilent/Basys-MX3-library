/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _AUDIO_H    /* Guard against multiple inclusion */
#define _AUDIO_H

void AUDIO_ConfigurePins();
void AUDIO_Init(unsigned char bMode);
void AUDIO_Close();


void AUDIO_InitPlayBack(unsigned short *pPlay_Samples1, int cntBuf1);
void AUDIO_InitRecord(unsigned short *pPlay_Samples1, int cntBuf1);

unsigned char GetAudioMode();


#endif /* _AUDIO_H */

/* *****************************************************************************
 End of File
 */
