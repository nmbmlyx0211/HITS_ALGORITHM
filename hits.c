/* YINXIA LI cs610 2705 prp */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>


void initValue2705(int val, int vertexNo, float* authPrev, float* hubPrev);
float eRate2705(int iteration);
void printFirstLine2705(int vertexNo, float *authPrev, float *hubPrev);
void printForLessThanTen2705(int vertexNo, int iterNo, float *authCur, float *hubCur);
void printForMoreThanTen2705(int vertexNo, int iterNo, float *authCur, float *hubCur);
int** Matrix2705(int vertexNo);
int** transMat2705(int** mat,int vertexNo);
void calAuthAndHub2705(int vertexNo, float* authCur, float* authPrev, float* hubPrev, float* hubCur,  int** mat, int** transMat);


int main(int argc, char *argv[])
{
    int cnt, iteration, initialValue, iterNo;
    int i, j, v1, v2, vertexNo, edgeNo;
    int** mat, ** transMat;
	float errorRate = 0;
	float* authPrev, * authCur, * hubPrev, * hubCur, * diffAuth, * diffHub;

	if (argc == 4)
    {
        iteration = atoi(argv[1]);
        initialValue = atoi(argv[2]);
        //printf("\nIteration: %d  IntitialValue: %d  File: %s\n", iteration, initialValue, argv[3]);
	}
	else
	{
	    printf("Error: wrong paramters!!!");
	    exit(0);
	}
	//read file and initalize value and matrix
    FILE * openF;
	openF = fopen(argv[3], "r");
	if (openF == NULL)
    {
        printf("\n Could not open the file\n");
        exit(0);
	}

	fscanf(openF, "%d %d", &vertexNo, &edgeNo);

    mat = Matrix2705(vertexNo);

    for(j = 0; j < edgeNo; j ++)
    {
        fscanf(openF,"%d %d", &v1, &v2);
        mat[v1][v2] = 1;
    }
    transMat = transMat2705(mat, vertexNo);
	authPrev = (float *) malloc (vertexNo *sizeof(float));
	authCur = (float *) malloc (vertexNo *sizeof(float));
	hubPrev =  (float *) malloc (vertexNo *sizeof(float));
	hubCur =  (float *) malloc (vertexNo *sizeof(float));
	diffAuth =  (float *) malloc (vertexNo *sizeof(float));
	diffHub = (float *) malloc (vertexNo *sizeof(float));

// if vertex nuber > 10, reinitialize iteration and iterationval
	if(vertexNo > 10)
    {
        iteration = 0;
        initialValue = -1;
    }
    //initialize vector
    initValue2705(initialValue, vertexNo, authPrev,hubPrev);
    if (vertexNo < 10){
       printFirstLine2705(vertexNo, authPrev, hubPrev);
    }
    if (iteration < 1){
        errorRate = eRate2705(iteration);
        iterNo = 1; cnt = 0;

        while (cnt != vertexNo)
        {
            cnt = 0;
            calAuthAndHub2705(vertexNo, authCur, authPrev, hubPrev, hubCur,  mat, transMat);

            for(i = 0; i < vertexNo; i++){
                diffHub[i] = hubPrev[i] - hubCur[i];
                diffAuth[i] = authPrev[i] - authCur[i];
                if(diffHub[i] < errorRate && diffAuth[i] < errorRate){
					cnt ++;
				}
                hubPrev[i] = hubCur[i];
                authPrev[i] = authCur[i];
            }

            if(vertexNo <= 10){
                printForLessThanTen2705(vertexNo, iterNo, authCur, hubCur);
            }
            iterNo ++;
        }
        if(vertexNo > 10){
             printForMoreThanTen2705(vertexNo, iterNo, authCur, hubCur);
        }
        printf("\n");
    }
    else
    {
		for(int n = 0; n < iteration; n++){
            calAuthAndHub2705(vertexNo, authCur, authPrev, hubPrev, hubCur,  mat, transMat);

            for(i = 0; i < vertexNo; i++){
                hubPrev[i] = hubCur[i];
                authPrev[i] = authCur[i];
            }
            printf("\n Iter : %2d :", n+1);
			for(i = 0; i < vertexNo; i++){
				printf(" A/H[%2d]=%.7f/%.7f ", i, authCur[i], hubCur[i]);
			}
		}
        printf("\n");
	}
	fclose(openF);
}

//The following are functions.
int** Matrix2705(int vertexNo)
{
	int** mat = (int **) malloc (sizeof(int **) * vertexNo);
  	for (int i = 0; i < vertexNo; i++) {
  	     mat[i] = (int *) malloc(sizeof(int) * vertexNo);
  	}
    for(int i = 0; i< vertexNo ; i++){
        for(int j = 0; j < vertexNo; j ++){
            mat[i][j]= 0;
        }
    }
  	return mat;
}

int** transMat2705(int** mat,int vertexNo){
	int** transMat = (int **) malloc (sizeof(int **) * vertexNo);
  	for (int i = 0; i < vertexNo; i++) {
  	     transMat[i] = (int *) malloc(sizeof(int) * vertexNo);
  	}
  	for(int i = 0; i < vertexNo; i ++){
		for(int j = 0; j < vertexNo;j ++){
		    transMat[j][i] = mat[i][j];
		}
	}
	return transMat;
}

void initValue2705(int val, int vertexNo, float* authPrev, float* hubPrev)
{
    for(int i = 0; i < vertexNo; i++){
        if (val == 0){
            authPrev[i] =0; hubPrev[i] = 0;
        }
        else if(val == 1){
            authPrev[i] =1; hubPrev[i] = 1;
        }
        else if (val == (-1)){
            authPrev[i] = 1/(float)vertexNo; hubPrev[i] = 1/(float)vertexNo;
        }
        else if (val == (-2)){
            authPrev[i] = 1/sqrt(vertexNo); hubPrev[i] = 1/sqrt(vertexNo);
        }
        else{
            printf ("Error! Please enter a number from 0, 1, -1, -2\n");
            exit(0);
        }
    }
}

//iteration number
float eRate2705(int iteration)
{
    float errorRate;
    if (iteration == 0)
    {
        errorRate = 0.00001;
    }
    else
    {
        errorRate= pow(10, iteration);
    }
    return errorRate;
}


void printFirstLine2705(int vertexNo, float *authPrev, float *hubPrev){

    printf("\n Base : %2d :", 0);
    for(int i = 0; i < vertexNo; i ++){
        printf(" A/H[%2d]=%.7f/%.7f ", i, authPrev[i], hubPrev[i]);
    }
}

void printForLessThanTen2705(int vertexNo, int iterNo, float *authCur, float *hubCur){

    printf("\n Iter : %2d :", iterNo);
    for(int i=0;i < vertexNo; i++){
        printf(" A/H[%2d]=%.7f/%.7f ", i, authCur[i], hubCur[i]);
    }
}

void printForMoreThanTen2705(int vertexNo, int iterNo, float *authCur, float *hubCur){
    printf("\nIter : %2d", iterNo -1);
    for(int i = 0; i < vertexNo; i++){
        printf("\nA/H[%2d]=%.7f/%.7f ", i, authCur[i], hubCur[i]);
        if(i==4){
            printf("\n   ... Other vertices omitted \n");
            break;
        }
    }
}

void calAuthAndHub2705(int vertexNo, float* authCur, float* authPrev, float* hubPrev, float* hubCur,  int** mat, int** transMat)
{
    float authSum = 0; float hubSum = 0;
    for(int i = 0; i < vertexNo; i++){
        authCur[i] = 0; hubCur[i] = 0;
    }

    for(int i = 0; i< vertexNo; i++){
        for(int j = 0; j < vertexNo; j ++){
            authCur[i] = authCur[i] + transMat[i][j] * hubPrev[j];
        }
    }
    for(int i = 0; i< vertexNo; i++){
        for(int j = 0;j < vertexNo;j ++){
            hubCur[i] = hubCur[i] + mat[i][j] * authCur[j];
        }
        authSum = authSum + (authCur[i] * authCur[i]);
        hubSum = hubSum + (hubCur[i] * hubCur[i]);
    }

    for(int i = 0; i < vertexNo; i++){
        authCur[i] = authCur[i]/sqrt(authSum);
        hubCur[i] = hubCur[i]/sqrt(hubSum);
    }
}



