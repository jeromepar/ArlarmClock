/*
 * utils.cpp
 *
 *  Created on: 14 août 2017
 *      Author: Jerome
 */




//maximum delta +-59mn
void addInterval(int delta_mn, int *hours, int *minutes){
	*minutes+=delta_mn;
	if (*minutes >= 60){
		*minutes-=60;
		*hours=(*hours+1)%24;
	} else if (*minutes < 0) {
		*minutes+=60;
		//*hours=(*hours-1)%24; -> Not working with negative values
		if ((*hours-1)<0){
			*hours+=23;
		} else {
			*hours-=1;
		}
	}
}
