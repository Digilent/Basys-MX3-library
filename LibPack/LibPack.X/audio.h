/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    audio.h

  @Description
    This file groups the declarations of the functions that implement
    the Audio module (defined in audio.c). 
    Include the file in the project when the Audio library is needed.
 */
/* ************************************************************************** */

#ifndef _AUDIO_H    /* Guard against multiple inclusion */
#define _AUDIO_H

void AUDIO_ConfigurePins();
void AUDIO_Init(unsigned char bMode);
void AUDIO_Close();


void AUDIO_InitPlayBack(unsigned short *pPlay_Samples1, int cntBuf1);
void AUDIO_InitRecord(unsigned short *pPlay_Samples1, int cntBuf1);

unsigned char AUDIO_GetAudioMode();


#endif /* _AUDIO_H */

/* *****************************************************************************
 End of File
 */
