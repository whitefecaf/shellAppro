#include "myshell.h"
#include "fctutil.h"

// fonctions externes
void eval(char*cmdline, jobsT *jobs);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);


// pour les 3 interruption les signaux sont masquer car jobs est une variables en section critique

void handler_sigint(int sig)
{

  refusedSignal();
  if(jobsTab.indiceFG != -1)
    {
      
      Kill(jobsTab.jobs[jobsTab.indiceFG].pid, SIGINT);
      delJobInd(jobsTab.indiceFG, &jobsTab);
    }

  autorisedSignal();
}

//traitant de SIGCHLD
void handler_sigchld(int sig)
{
  pid_t pid;
  int status;
	
  refusedSignal();
  

  pid = waitpid(-1, &status, WNOHANG|WUNTRACED);// pid child
  if ((WIFSIGNALED(status) || WIFEXITED(status) || WCOREDUMP(status)) && pid > 0)//le fils est arrêter ?
    {
      int ind  =  searchIndWithPid(pid, &jobsTab) + 1;//le "+1" est la decalage de l'utilisateur
      if (ind != jobsTab.indiceFG + 1) //le "+1" est la decalage de l'utilisateur repercuter de la précédente instruction
	{
	  printf("le job %d s'est terminée ",ind);
	  if(WIFSIGNALED(status))
	    {
	      printf("par le signal %d\n", WTERMSIG(status));
	    }
	  if(WIFEXITED(status))
	    {
	      printf("normalement\n");
	    }
	  if(WCOREDUMP(status))
	    {
	      printf("par un core dump\n");
	    }
	}
      // oui -> delete Job
      delJobPid(pid, &jobsTab);
      
    }

  autorisedSignal();
}

//traitant de SIGTSTP
void handler_sigtstp(int sig)
{
  
  refusedSignal();
  if(jobsTab.indiceFG != -1)
    {

      Kill(jobsTab.jobs[jobsTab.indiceFG].pid, SIGTSTP);
      stoppedJob(jobsTab.indiceFG, &jobsTab);// cette fonction met a jour les jobs
    }
	
 
  autorisedSignal();
}


int main()
{
  setvbuf(stdout, NULL, _IOFBF, 100);
  
  char cmdline[MAXLINE];                 // ligne de commande
  initJobs(&jobsTab);

  
  Signal(SIGINT, handler_sigint); //gestion de SIGINT
  Signal(SIGCHLD, handler_sigchld); //gestion de SIGCHLD
  Signal(SIGTSTP, handler_sigtstp); //gestion de SIGTSTP

  
  while (1)
    {// boucle d'interpretation
      
      fflush(stdout);
      printf("<my_shell> ");     // message d'invite
      fflush(stdout);
      Fgets(cmdline, MAXLINE, stdin);    // lire commande
      if (feof(stdin))                   // fin (control-D)
	exit(0);
      eval(cmdline, &jobsTab);                     // interpreter commande
    }
}
