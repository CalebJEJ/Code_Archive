#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "omp.h"

//See values of N in assignment instructions.
#define N 100000
//Do not change the seed, or your answer will not be correct
#define SEED 72

//For GPU implementation
#define BLOCKSIZE 1024


struct pointData{
double x;
double y;
};



void generateDataset(struct pointData * data);

using namespace std;
void warmUpGPU();
__global__ void epsilonDist(struct pointData * data, double epsilon, int * numPairsInE);


int main(int argc, char *argv[])
{	
	//WARM UP///////////
	warmUpGPU();
	
	//Read epsilon distance from command line
	if (argc!=2)
	{
	printf("\nIncorrect number of input parameters. Please input an epsilon distance.\n");
	return 0;
	}

	
	char inputEpsilon[20];
	strcpy(inputEpsilon,argv[1]);
	double epsilon=atof(inputEpsilon);
	
	

	//generate dataset:
	struct pointData * data;
	data=(struct pointData*)malloc(sizeof(struct pointData)*N);
	printf("\nSize of dataset (MiB): %f",(2.0*sizeof(double)*N*1.0)/(1024.0*1024.0));
	generateDataset(data);
	

	//Write your code here:
	//The data you need to use is stored in the variable "data", 
	//which is of type pointData
	
//Data Copy Start of Total GPU Time/////
	double tstart=omp_get_wtime();

	cudaError_t errCode = cudaSuccess;
	struct pointData* dev_Data;
	int * numPairsInE = (int*)malloc(sizeof(int)*N);
	for(int i = 0; i<N; i++)
	{
		numPairsInE[i] = 0;
	}
	int * dev_pairs;
	
	
    errCode=cudaMalloc((struct pointData**)&dev_Data, sizeof(struct pointData)*N);	
	if(errCode != cudaSuccess) {
	cout << "\nError: A error with code " << errCode << endl; 
	}
	
	errCode=cudaMalloc((int**)&dev_pairs, sizeof(int)*N);	
	if(errCode != cudaSuccess) {
	cout << "\nError: A error with code " << errCode << endl; 
	}


	errCode=cudaMemcpy( dev_Data, data, sizeof(struct pointData)*N, cudaMemcpyHostToDevice);
	if(errCode != cudaSuccess) {
	cout << "\nError: A memcpy error with code " << errCode << endl; 
	}	
	
	errCode=cudaMemcpy( dev_pairs, numPairsInE, sizeof(int)*N, cudaMemcpyHostToDevice);
	if(errCode != cudaSuccess) {
	cout << "\nError: A memcpy error with code " << errCode << endl; 
	}	



//Data Copy End/////

	//Run Kernel
    const unsigned int totalBlocks=ceil(N*1.0/BLOCKSIZE);
	printf("\ntotal blocks: %d",totalBlocks);

	double tstartKernel =omp_get_wtime();
	epsilonDist<<<totalBlocks,BLOCKSIZE>>>(dev_Data, epsilon, dev_pairs);
	cudaDeviceSynchronize();
	
	//Need to transfer data back for processing, this shouldn't be added to kernel time
	double tstartData2 =omp_get_wtime();
	//Data copy 2
	cudaMemcpy( numPairsInE, dev_pairs, sizeof(int)*N, cudaMemcpyDeviceToHost);
	cudaDeviceSynchronize();
	double tendData2 =omp_get_wtime();
	//End of data transfer 2 -> resume calculation
	
	int totalPairs = 0;
	for(int i = 0; i < N; i++)
	{
		totalPairs = totalPairs + numPairsInE[i];
	}
	totalPairs = totalPairs * 2;
	totalPairs = totalPairs + N;
	
	double tendKernel =omp_get_wtime();

	if(errCode != cudaSuccess){
		cout<<"Error after kernel launch "<<errCode<<endl;
	}

	
	printf("\nNumber of Pairs: %d", totalPairs);
	
	double tend=omp_get_wtime();
	                                            //Remove time used to move data back from kernel time
	printf("\nKernel time (s): %.15f",tendKernel-tstartKernel - (tstartData2 - tendData2));	
	printf("\nTotal time (s): %.15f",tend-tstart);



	free(data);
	printf("\n");
	return 0;
}

__global__ void epsilonDist(struct pointData * data, double epsilon, int * numPairsInE) 
{
	
	unsigned int tid=threadIdx.x + (blockIdx.x*blockDim.x);
	if(tid>=N)
	{
		return;
	}
	
	unsigned int index2;
	double diffX = 0;
    double diffY = 0;
	int count = 0;
	
	__shared__ struct pointData dataShared[BLOCKSIZE];

	dataShared[threadIdx.x] = data[tid];
	
	
	for(index2 = tid + 1; index2 < N; index2++)
	{
		diffX = fabs(dataShared[threadIdx.x].x - data[index2].x);
		diffY = fabs(dataShared[threadIdx.x].y - data[index2].y);
		
		//if the points are clearly too far then don't count them
		if( diffX <= epsilon && diffY <= epsilon )
		{
			if( sqrt( pow( diffX, 2 ) + pow( diffY, 2 ) )
	                    <= epsilon )
			{
				count++;
			}
		}
		
	}
	
	numPairsInE[threadIdx.x] = numPairsInE[threadIdx.x] + count;
	
	return;
}





__global__ void warmup(unsigned int * tmp) {
if (threadIdx.x==0)
*tmp=555;

return;
}



void warmUpGPU(){


printf("\nWarming up GPU for time trialing...\n");	
unsigned int * dev_tmp;
unsigned int * tmp;
tmp=(unsigned int*)malloc(sizeof(unsigned int));
*tmp=0;
cudaError_t errCode=cudaSuccess;
errCode=cudaMalloc((unsigned int**)&dev_tmp, sizeof(unsigned int));	
	if(errCode != cudaSuccess) {
	cout << "\nError: dev_tmp error with code " << errCode << endl; 
	}

warmup<<<1,256>>>(dev_tmp);

//copy data from device to host 
	errCode=cudaMemcpy( tmp, dev_tmp, sizeof(unsigned int), cudaMemcpyDeviceToHost);
	if(errCode != cudaSuccess) {
	cout << "\nError: getting tmp result form GPU error with code " << errCode << endl; 
	}

	cudaDeviceSynchronize();

	printf("\ntmp (changed to 555 on GPU): %d",*tmp);

cudaFree(dev_tmp);

return;
}



//Do not modify the dataset generator or you will get the wrong answer
void generateDataset(struct pointData * data)
{

	//seed RNG
	srand(SEED);


	for (unsigned int i=0; i<N; i++){
		data[i].x=1000.0*((double)(rand()) / RAND_MAX);	
		data[i].y=1000.0*((double)(rand()) / RAND_MAX);	
	}
	

}
