#ifndef __FCTUTIL_H__
#define __FCTUTIL_H__

#include "csapp.h"
#include "jobs.h"

//refuse tous les signaux (utilités : lors d'une interuption on veut eviter qu'une autre interuption intervienne)
void refusedSignal();

//réautaurise les signaux (a la fin de l'interruption)
void autorisedSignal();




#endif
