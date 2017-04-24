/** Descriptive File Name

  @Company
    Digilent

  @File Name
    mic.h

  @Description
        This file groups the declarations of the functions that implement
        the MIC library (defined in mic.c). 
        Include the file in the project when the MIC library is needed.
 */

#ifndef MIC_H
#define	MIC_H

void MIC_Init();
void MIC_ConfigurePins();
unsigned int MIC_Val();

#endif	/* MIC_H */

