#ifndef __JOBS_H__
#define __JOBS_H__

#include "csapp.h"

#define MAXJOBS 10


typedef enum{FG,BG,ST} state;

typedef struct{
	pid_t pid;
	state etat;
	char cmd[MAXLINE];
} jobT;

typedef struct{
	jobT jobs[MAXJOBS];
	int taille;
	int indiceFG; // si pas de FG, indice = -1
} jobsT;

jobT initJob(pid_t pid, state etat, char *cmd);
void initJobs(jobsT *jobs);
void delJobPid(pid_t pid, jobsT *jobs);
void delJobInd(int i, jobsT *jobs);
void addJob(jobT job, jobsT *jobs);
void printJob(jobsT jobs);
void putJobInBG(int pid, jobsT *jobs);
void putJobInFG(int pid, jobsT *jobs);
int searchPIDWithInd(int ind,jobsT *jobs);
int searchIndWithPid(pid_t pid, jobsT *jobs);

void stoppedJob(int ind,jobsT *jobs);
int allStopped(jobsT *jb);


#endif
