#include <msp430.h>
#include "comp_e.h"
void initCOMP()
{

	CECTL0 =CEIPEN | CEIPSEL_4;
	CECTL2 = CEREFL1 |CEREFL0 | CERS1 | CEREF0_3 | CEREF1_2 | CERSEL;
	CECTL3=CEPD3;
	CECTL1 = CEON;
}
