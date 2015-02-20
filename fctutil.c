#include "fctutil.h"

void refusedSignal(){
	sigset_t p_ens;
	sigfillset(&p_ens);
	sigprocmask(SIG_SETMASK, &p_ens, NULL);
}

void autorisedSignal(){
	sigset_t p_ens;
	sigemptyset(&p_ens);
	sigprocmask(SIG_SETMASK, &p_ens, NULL);
}




