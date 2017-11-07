#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <omp.h>

#include "utils.h"

int main(int argc, char **argv)
{
	char *refStr, *tolStr, *hitStr;
	float v;
	refStr = tolStr = hitStr = NULL;
	int ind;
	for (ind = 1; ind < argc; ind++)
	{
		if (strcmp(argv[ind], "-r") == 0) { refStr = argv[ind+1]; ind++; }
		if (strcmp(argv[ind], "-t") == 0) { tolStr = argv[ind+1]; ind++; }
		if (strcmp(argv[ind], "-v") == 0) { v=1; }
	}
	
	if (refStr == NULL || tolStr == NULL)
	{
		fprintf(stderr, "Usage: %s -r ref -t tol\n", argv[0]);
		exit(1);
	}
	struct timeval tim;
	struct tm *local;
	time_t start;
	
	// convert strings to floats
	float ref = strtof(refStr, 0);
	float tol = strtof(tolStr, 0);
	if (tol < 0.0)
	tol *= -1;			/* always positive */

	int rct, cct;
	fscanf(stdin, "%d", &rct);
	fscanf(stdin, "%d", &cct);

	float** rows = (float** ) malloc(rct * sizeof(float *));
	if (rows == 0)
	{
		fprintf(stderr, "Couldn't alocate sufficient space.\n");
		exit(1);
	}
	int i;
	for (i = 0; i < rct; i++)
	{
		float* row = (float* ) malloc(cct * sizeof(float));
		if (row == 0)
	{
		fprintf(stderr, "Couldn't alocate sufficient row space.\n");
		exit(1);
	}
		rows[i] = row;
	}

	int r, c;
	for (r = 0; r < rct; r++)
	for (c = 0; c < cct; c++)
	fscanf(stdin, "%f", &rows[r][c]);

	time (&start);
	local = localtime(&start);
	gettimeofday(&tim,NULL);
	double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
	printf("# Start time and date: %s", asctime(local));

	// data read; now check it
	int count = 0;
	
        #pragma omp parallel for collapse(2)
	for (r = 0; r < rct; r++){
	  for (c = 0; c < cct; c++){
	    if (approxEqual(rows[r][c], ref, tol) == 1)
	      {
		if (v == 1)	
		  fprintf(stdout, "r=%d, c=%d: %.6f\n", r, c, rows[r][c]);
		if (v){
		  fprintf(stdout, "r=%d, cc=%d: %.6f (thread= %d)\n",
			  r, c, rows[r][c], omp_get_thread_num());
		}
		#pragma omp atomic
		count++;
	      }
	  }
	}
	
	gettimeofday(&tim,NULL);
	double t2=tim.tv_sec+(tim.tv_usec/1000000.0); 
	// now output count
	fprintf(stdout, "Found %d approximate matches.\n", count);
	printf("Search time:%.6lf(s) \n", t2-t1);

	
	exit(0);
}
