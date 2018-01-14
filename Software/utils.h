/*
 * utils.h
 *
 *  Created on: 14 août 2017
 *      Author: Jerome
 */

#ifndef UTILS_H_
#define UTILS_H_

#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

//maximum delta +-59mn
void addInterval(int delta_mn, int *hours, int *minutes);


#endif /* UTILS_H_ */
