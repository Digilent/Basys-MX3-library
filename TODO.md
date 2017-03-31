# TODO
1. ACL lib
   - ACL_ConfigurePins function – there is one pin configured only, but it does not say what is its functionality – what about the rest?
2. LED lib
   - Init function lits all the LEDs, add a default set value to 0 operation inside the function
   - The argument of setvalue function should not be allowed to exceed 7
3. SWT lib test
   - The argument of getvalue function should not be allowed to exceed the max number
4. BTN lib
   - Btn.c file, comments at the beginning need to be updated
   - The get value function argument is limited to 0 and 4, if exceeds, the return value is ff, the swt functions work differently
   - BTN_GetValue function 0, ?U?, ?u?: BTNU comments to be corrected
   - BTN_GetGroupValue comments : The 5 LSB bits from returned value corresponds to a button: correspond is correct
   - BTN_GetGroupValue comments: (see return value explanation). Replace that with description
5. SSD lib test
   - Update comments in ssd.c and delete the two lines that define 2 digit segments
   - Cleanup the ISR function
6. RGBLed lib test
   - Update comments in the c file, first lines
   - Carry gets often for large values and rarely for small values. Replace gets with occurs
   - // thake the 9'th bit (addition carry) as the PDM. Typo at take
   - Cleanup the isr function
   - Cleanup init function
7. Doc cleanup
   - Convert User Guide to LibGuide.md, place in root folder, only include library overviews.
8. Project ignores
   - Investigate if some files in the MPLAB X Project directories can be added to .gitignore

