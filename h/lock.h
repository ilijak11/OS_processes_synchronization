/*
 * lock.h
 *
 *  Created on: Aug 8, 2021
 *      Author: OS1
 */

#ifndef LOCK_H_
#define LOCK_H_

/*
#define lock asm cli

#define unlock asm sti
*/


#define lock asm{\
	pushf;\
	cli;\
}

#define unlock asm popf

#endif /* LOCK_H_ */
