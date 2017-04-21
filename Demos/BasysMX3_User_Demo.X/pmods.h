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

#ifndef _PMODS_H    /* Guard against multiple inclusion */
#define _PMODS_H

void PMODS_InitPin(unsigned char bPmod, unsigned char bPos, unsigned char bDir, unsigned char pullup, unsigned char pulldown);
unsigned char PMODS_GetValue(unsigned char bPmod, unsigned char bPos);
void PMODS_SetValue(unsigned char bPmod, unsigned char bPos, unsigned char bVal);

#endif /* _PMODS_H */

/* *****************************************************************************
 End of File
 */
