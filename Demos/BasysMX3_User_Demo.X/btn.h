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

#ifndef _BTN_H    /* Guard against multiple inclusion */
#define _BTN_H

void BTN_Init();
void BTN_ConfigurePins();
void BTN_InitChangeNotification();
unsigned char BTN_GetValue(unsigned char btn);
unsigned char BTN_GetGroupValue();

#endif /* _BTN_H */

/* *****************************************************************************
 End of File
 */
