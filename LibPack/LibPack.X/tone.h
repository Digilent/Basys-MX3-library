/* ************************************************************************** */
/** Descriptive File Name

  @Company
 *  University of Delaware

  @File Name
    tone.h

  @Description
    This file groups the declarations of the functions that implement
    the Tune module (defined in tune.c). 
    Include the file in the project when the Tune library is needed.
 *  This library cannot be used at the same time as the Audio library, this
 *  is intended as an alternative.
 */
/* ************************************************************************** */

#ifndef _TONE_H    /* Guard against multiple inclusion */
#define _TONE_H

void TONE_ConfigurePins();
void TONE_Init();
void TONE_Close();

void TONE_Start(int frequency, int ms);

#endif /* _TONE_H */

/* *****************************************************************************
 End of File
 */
