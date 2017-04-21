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

#ifndef _SWT_H    /* Guard against multiple inclusion */
#define _SWT_H

void SWT_Init();
void SWT_ConfigurePins();
unsigned char SWT_GetValue(unsigned char bNo);
unsigned char SWT_GetGroupValue();

#endif /* _SWT_H */

/* *****************************************************************************
 End of File
 */
