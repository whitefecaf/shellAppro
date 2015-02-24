/* eval : interprete une ligne de commande passee en parametre       */
#include "myshell.h"
#include "jobs.h"

// fonctions externes
int parseline(char *buf, char **argv);
int builtin_command(char **argv);

void eval(char *cmdline, jobsT *jobs)
{
  char *argv[MAXARGS]; // argv pour execve()
  char buf[MAXLINE];   // contient ligne commande modifiee
  int bg;              // arriere-plan ou premier plan ?
  pid_t pid;           // process id

  strcpy(buf, cmdline);
  bg = parseline(buf, argv);
  if (argv[0] == NULL)
    return;                      // ignorer lignes vides

  if (!builtin_command(argv))  // commande interne ?
    { // si non, executee par un fils

      
      if ((pid = Fork()) == 0)
	{   

	  setpgid(pid,pid);/*
	  char *buf = malloc(strlen(argv[1]) -1);
	  int i;
	  for(i = 1; i < strlen(argv[1]); i++)
	    buf[i-1] = argv[1][i];
	  // un  décalage de l'indice existe entre l'affichage et l'implémentation d'où la présence du -1
	  pid = searchPIDWithInd(atoi(buf)-1, jobs);
	  char *str;
	  itoa(pid, str, 10);
	  argv[1] = malloc(sizeof(strlen(str)));
	  strcpy(argv[1], str);	    */
	  if (execvp(argv[0], argv) < 0)
	    {
	      printf("%s: Command not found.\n", argv[0]);
	      exit(0);
	    }
	}

      if (!bg)
	{ // le pere attend fin du travail de premier plan
	  state etat = FG;
	  jobT job = initJob(pid, etat, cmdline);
	  addJob(job, jobs);
	  int status;
	  while (jobs->indiceFG != -1 || waitpid(pid, &status, WNOHANG|WUNTRACED) == 0);
	    
	}
      else       // travail d'arriere-plan, on imprime le pid
	{
	  state etat = BG;
	  jobT job = initJob(pid, etat, cmdline);
	  addJob(job, jobs);
	  printf("%d %s", pid, cmdline);
	}
    }
  else // si oui, executee directement
    {
      
      if(!strcmp(argv[0], "setenv"))
	{
	  if(argv[1] != NULL)
	    {
	      /* char *var = strtok(argv[1],"=");
		 if(setenv(var[0], var[1], 1) == -1)
		 printf("setenv a été mal utilisé(vérifier que votre variable d'environement existe");*/
	      putenv(argv[1]);
	    }
	}
      else if(!strcmp(argv[0], "unsetenv"))
	{
	  if(argv[1] != NULL)
	    {
	      unsetenv(argv[1]);
	    }
	}

      if(!strcmp(argv[0], "jobs"))
	printJob(*jobs);
      else if(!strcmp(argv[0], "fg") || !strcmp(argv[0], "bg")|| !strcmp(argv[0], "stop"))
	{
	  
	  int pid;
	  if(argv[1] == NULL)
	    pid = jobs->jobs[jobs->taille-1].pid;
	  else
	    {
	      
	      char *buf = malloc(strlen(argv[1]) -1);
	      int i;
	      for(i = 1; i < strlen(argv[1]); i++)
		buf[i-1] = argv[1][i];
	      // un  décalage de l'indice existe entre l'affichage et l'implémentation d'où la présence du -1
	      pid = searchPIDWithInd(atoi(buf)-1, jobs);
	    }
	  
	  if(!strcmp(argv[0], "fg"))
	    putJobInFG(pid, jobs);
	  else if(!strcmp(argv[0], "stop"))
	    {
	      kill(pid, SIGTSTP);
	      stoppedJob(searchIndWithPid(pid,jobs), jobs);
	    }
	  else
	    putJobInBG(pid, jobs);
	}
      else if(!strcmp(argv[0], "wait"))
	{

	  while (jobs->taille > 0 || allStopped(jobs) == 0)
	    sleep(1);
	  fflush(stdout);
	  
	  if(jobs->taille > 0)
	    printJob(*jobs);
	  fflush(stdout);
	}
      
      
    }
  return;
}

// si le premier parametre est une commande integree,
// l'executer et renvoyer "vrai"
int builtin_command(char **argv)
{
  if (!strcmp(argv[0], "quit")) // commande "quitter"
    {
      exit(0);
    }
  if (!strcmp(argv[0], "&"))    // ignorer & tout seul
    return 1;
  if (!strcmp(argv[0], "jobs") || !strcmp(argv[0], "fg") || !strcmp(argv[0], "bg") || !strcmp(argv[0], "stop")|| !strcmp(argv[0], "wait") 
      || !strcmp(argv[0], "setenv")|| !strcmp(argv[0], "unsetenv"))
    return 1; // commande interne

  return 0;                     // ce n'est pas une commande integree
}
