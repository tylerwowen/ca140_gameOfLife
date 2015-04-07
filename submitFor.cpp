#include "life.h"
#include <time.h>

using namespace std;
//Generate the life matrix any way you want. We would highly recommend that you print the generated
//matrix into a file, so that you can share it with other teams for checking correctness.
void genlife(int *a, unsigned int n)
{
  if (n > 16) {
	// Method 2
	int cell[12*16] = {
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
	  0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
	  0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
	  0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	  0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
	  0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
	  0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
	  0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
	  0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	// set every entry to 0
	for (int i = 0; i < n*n; i++){
	  a[i] = 0;
	}
	
	for (int i = 0; i < 12; i++) {
	  for (int k = 0; k < 16; k++){
		a[i*n + k] = cell[i*16 + k];
	  }
	}
	
	FILE *fp = fopen("generatedFile", "w");
	for (int i = 0; i < n; i++) {
	  for (int k = 0; k < n; k++) {
		
		fprintf(fp, "%d ", a[i*n + k]);
	  }
	  fprintf(fp, "\n");
	}
	fclose(fp);
  }
  else {
	// Initialize random number seed
	srand((unsigned int)time(NULL));
	
	for(int i = 0; i < n*n; i++)
	{
	  // Around 10% of the entries will be set as alive
	  a[i] = (rand()/RAND_MAX > 0.9 ? 1:0);
	}
	
  }
  
}

//Read the life matrix from a file
void readlife(int *a, unsigned int n, char *filename)
{
  FILE *fp = fopen(filename, "r");
  for(int i = 0; i < n*n; i++)
  {
	fscanf(fp, "%d", &a[i]);
  }
  fclose(fp);
}

//Life function
void life(int *a, unsigned int n, unsigned int iter, int *livecount)
{
  int totalLives = 0;
  
  
  for(int k = 0; k < iter; k++) {
	cilk_for(int i = 0; i < n; i++) {
	  for(int j = 0; j < n; j++) {
		int neighbors = 0;
		int iPre = (i - 1 + n) % n * n, iPost = (i + 1 + n) % n * n;
		int jPre = (j - 1 + n) % n, jPost = (j + 1 + n) % n;
		if (a[iPre + jPre] > 0)  neighbors++;
		if (a[iPre + j] > 0)  neighbors++;
		if (a[iPre + jPost] > 0)  neighbors++;
		if (a[i*n + jPre] > 0)  neighbors++;
		if (a[i*n + jPost] > 0)  neighbors++;
		if (a[iPost + jPre] > 0)  neighbors++;
		if (a[iPost + j] > 0)  neighbors++;
		if (a[iPost + jPost] > 0)  neighbors++;

		// if cell is alive
		if(a[i * n + j] == 1) {
		  a[i * n + j] = (neighbors == 2 || neighbors == 3 ? 3 : 2 );
		}
		else {
		  a[i * n + j] = (neighbors == 3 ? -1 : -2 );
		}
	  }
	}
	cilk_for (int i = 0; i < n*n; i++){
	  // if a[i] is even, a[i] is dead, set to 0
	  a[i] = (a[i] % 2 == 0) ? 0 : 1;
	}
#if DEBUG == 1
	if((k+1) % (iter/10) == 0) {
	  totalLives = countlive(a, n);
	  livecount[k / (iter/10)] = (totalLives);
	}
#endif

  }
}
