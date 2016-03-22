/* name:               Lane Scobie   Dylan Waters
 * ONE Card number:    1448158       1343144
 * Unix id:            scobie        dwaters
 *
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

char M; //I for independent, B for burst
int A;  //feedback time is 50 bit time units
int K;  //Number of blocks. F should be a multiple of K.
int F;  //Size of frame in number of bits. Can assume is 4000 bits.
double e;//Probability that a bit is in erro.
int B;  //0 for Independent model. For burst, set to 50 and 500 bit times
int N;  //0 for Independent. For Burst, set to 5000 and 1000 bit times
int R;  //Length of simulation in bit time units.
int T; //Number of trials
int t[1];
int GoodFrames[5];
double globalBT;

int main(int argc, char* argv[]){
  srand(time(NULL));
  /*
  FILE * fpointer;
  fpointer=fopen("esimData.txt","a+");
  */
  // READS USER INPUT
  double randoms = ( (double)rand()/(double)RAND_MAX  );
  for(int iter=1; iter<argc; iter+=2 ){
    if (strncmp(argv[iter],"-m",2)==0 ) {
      // fprintf(fpointer,"%s %s \n",argv[iter],argv[iter+1]);
	printf("%s\n",argv[iter+1] );
	M=*(argv[iter+1]);
      }
    else if (strncmp(argv[iter],"-f",2)==0 ) {
      //fprintf(fpointer,"%s %s \n",argv[iter],argv[iter+1]);
      printf("%s\n",argv[iter+1] );
      F=atoi(argv[iter+1]);

      }
   else if (strncmp(argv[iter],"-a",2)==0 ) {
     //fprintf(fpointer,"%s %s \n",argv[iter],argv[iter+1]);
     printf("%s\n",argv[iter+1] );
     A=atoi(argv[iter+1]);
      }
    else if (strncmp(argv[iter],"-k",2)==0 ) {
      //fprintf(fpointer,"%s %s \n",argv[iter],argv[iter+1]);
      printf("%s\n",argv[iter+1] );

      K=atoi(argv[iter+1]);
      }
    else if (strncmp(argv[iter],"-e",2)==0 ) {
      //fprintf(fpointer,"%s %s \n",argv[iter],argv[iter+1]);
	printf("%s\n",argv[iter+1] );
	e=atof(argv[iter+1]);
      }
    else if (strncmp(argv[iter],"-r",2)==0 ) {
      //fprintf(fpointer,"%s %s \n",argv[iter],argv[iter+1]);
      printf("%s\n",argv[iter+1] );
      R=atoi(argv[iter+1]);
      }
    else if (strncmp(argv[iter],"-t",2)==0 ) {
      //fprintf(fpointer,"%s ",argv[iter]);
      printf("%s ",argv[iter+1] );
      //fprintf(fpointer,"%s ",argv[iter+1] );
      T=atoi(argv[iter+1]);
      int t[T];
      for(int i =0; i<T;i++){
	t[i]=atoi(argv[iter+2]);
	printf("%d ",t[i]);
	//fprintf(fpointer,"%d ",t[i]);
	iter+=1;
      }
      printf("\n");
      //fprintf(fpointer,"\n");
    }
    else if (strncmp(argv[iter],"-b",2)==0 ) {
      //fprintf(fpointer,"%s %s \n",argv[iter],argv[iter+1]);
      printf("%s\n",argv[iter+1] );
      B=atoi(argv[iter+1]);
      }
    else if (strncmp(argv[iter],"-n",2)==0 ) {
      //fprintf(fpointer,"%s %s \n",argv[iter],argv[iter+1]);
      printf("%s\n",argv[iter+1] );
      N=atoi(argv[iter+1]);
      }
    else{
      fprintf(stderr,"line:0 Error\n");
      return 0;
    }
  } //END OF USER INPUT
  //Calc. the new error prob. if burst is enabled
  if( toupper(M)=='B' ){
    e=e*(N+B)/B;
  } 
  int checkbit =0;
  int errorfixing=1;
  if (K!=0){//if error correction is disabled
    while((pow(2,checkbit))<(F/K)){
      checkbit++;
    } 
  }else{ 
    errorfixing=0;
    K=1;
  }
  //
  //This is the start of the simulations
  //
  double totalFrames=0;
  int correctFrames=0;
  for(int trial =0; trial<T; trial++){
    int tempGood=0;//good frames
    int errCNT = 0;//number of errors per block
    int totalBT =0;//the BT for the frame
    int burstCLK=0;//burst timer
    int NburstCLK=0;//non burst timer
    srand(t[trial]);
    while(totalBT<R){
      int notSucessful=1;
      while (notSucessful){//a frame
	int BadBlock=0;
	tempGood++;//assume every frame will be good
	notSucessful=0;
	totalFrames++;
	for(int i=0; i<K; i++){//Blocks
	  for(int j =0; j<( (F/K)+checkbit ); j++){//Bits
	    randoms = ( (double)rand()/(double)RAND_MAX  );
	    if( toupper(M)=='B' ){//starts the if statement
	      if(burstCLK<B){
		burstCLK++;
		if(randoms<e){
		  errCNT+=1;
		}
	      }else{
		NburstCLK++;
		if(NburstCLK>N){
		  burstCLK=0;
		  NburstCLK=0;
		}
	      }
	    }else{//If == i
	      if(randoms<e){
		errCNT+=1;
	      }
	    }//This ends the if statement
	  }
	  if (errCNT>1){
	    notSucessful=1;
	    if(!BadBlock){
	      tempGood--;//Frame is bad
	      BadBlock=1;
	    }
	  }else if(!errorfixing && errCNT==1){
	    notSucessful=1;
	    if(!BadBlock){
	      tempGood--;//Frame is bad
	      BadBlock=1;
	    }
	  }
	  errCNT=0;
	}
	//updating the BT
	totalBT+=A;
	totalBT+=F;
	totalBT+=(checkbit*K);
	if (totalBT>R){
	  globalBT+=totalBT;
	  notSucessful=0;
	}
      }//end of a transmission

    }//end of a BT while loop
    GoodFrames[trial]=tempGood;
    correctFrames+=tempGood;
    tempGood=0;
  }//end of a trials
  
  //Start of Calc.
  double STDev;
  double tempV;
  double C1;
  double C2;
  double C11;
  double C12;
  for(int i=0; i<5;i++){
    tempV=GoodFrames[i]- (correctFrames/5);
    STDev+= pow( tempV, 2) ;
  }
  STDev=STDev/5;
  STDev=pow(STDev,0.5 );
  C2 = (correctFrames/5)-(2.776*STDev/pow(5,0.5));
  C1 = (correctFrames/5)+(2.776*STDev/pow(5,0.5));
  //Average Frames
  printf("%lf %lf %lf \n",totalFrames/correctFrames,
	 totalFrames/(5*C1),totalFrames/(5*C2));
  /*
  fprintf(fpointer,"%lf  %lf %lf \n",totalFrames/correctFrames,
	 totalFrames/(5*C1),totalFrames/(5*C2));
  */
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  C11=F*(C1*5)/globalBT;
  C12=F*(C2*5)/globalBT;
  //Throughput
  printf("%lg %lg %lg \n", F*correctFrames/globalBT,C12,C11  );
  //fprintf(fpointer,"%lg %lg %lg \n\n", F*correctFrames/globalBT,C12,C11  );
  //fclose(fpointer);
  return 0;
}

