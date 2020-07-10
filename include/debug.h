/*
 * debug.h
 *
 *  Created on: 30 Apr 2020
 *      Author: csqwang
 */

#ifndef __DEBUG__
#define __DEBUG__

#ifdef DEBUG
#define debug(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define debug(fmt, ...) ((void)0)
#endif

#endif /* DEBUG_H_ */
