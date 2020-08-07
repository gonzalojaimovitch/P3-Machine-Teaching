
/*
  ============================================================================
  Name        : learnp3.c
  Author      : 
  Version     :
  Copyright   : Your copyright notice
  Description : receives 1 parameters in the call ,tuple formed by pairs(input,output), the program finds the P3 solution to that problem and returns the output program
  ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
char prg[256]; // bad solution, but easy one, it stores the solution program
long int numsteps =1;

#define DEBUG_MP 0
#define VERBOSE 0

#define TAMIO 16
#define NUMPAIRS 12
#define SIZEIO 3

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))


#define MEM_SIZE 200
#define PROG_SIZE 50

#define LEFT_PADDING_DOTS  MEM_SIZE/3
#define LEFT_LIMIT 1 // LEFT_PADDING_DOTS  //CESAR If I left to 0 it does not work properly, it does not include a . on the left
#define RIGHT_LIMIT MEM_SIZE - LEFT_PADDING_DOTS - 1
#define NUM_INSTRUCTIONS  7
#define MAXPRG 10000000000000000


#define LIMIT_STEPS  200  // Has to be smaller than the size of the output ( long)2000
#define OUTPUT_SIZE  250 // (long) 2500

char *Instructions = "><+-o[]";



char *Alphabet = ".01";
int lenAlphabet = 3;
#define last 2
#define beflast 1




void removeChar(char *str, char c) {
  int i = 0;
  int j = 0;

  while (str[i]) {
    if (str[i] != c) {
      str[j++] = str[i];
    }
    i++;
  }
  str[j]=0;
}

// Given a sring in the form (('', '0'), ('0', '')), it splits the string into two arrays with the inputs and outputs
void splitpairs(char *stri, char* pinput[], char * poutput[], int *posi)
{
  char token1[256],token2[256], resp[1000];
  char *str,*tk;
  int tams;

  str=&stri[1];
  removeChar(str,' ');
  removeChar(str,'\'');
  
  tams=strlen(str);
  str[tams-1]='\0';
  //  printf("string %s %d \n",str,tams);

  int k=0,pos=0;
  while(1)
    {
      pos=k/2;
      if(k==0)
	{ //first input
	  char* tk1 = strtok(str, ",");
	  strcpy(token1,tk1);
          
	  tk=&token1[1];
	  // printf("t2:%s \n",tk);
	  pinput[pos]= (char*)malloc(TAMIO * sizeof(char));
	  strcpy( pinput[pos],tk); 
	}
      else
	{
	  
	  //output
          if ( k%2 == 1 )
	    {
	      char* tk2 = strtok(NULL, ",");
	      //if (tk2==NULL) return 1;
	      // tk2=strtok(tk2, "\n");
	      strcpy(token2,tk2);
	      tams=strlen(token2);
	      token2[tams-1]='\0';
	      //           printf("t1:%s \n",token2);
	      poutput[pos]= (char*)malloc(TAMIO * sizeof(char));
	      strcpy(poutput[pos],token2);
	      // printf("%s %d\n",poutput[pos],pos);
	    }
	  else
	    {
	      //input 
	      char* tk2 = strtok(NULL, ",");
	      if (tk2==NULL) break;
	      // tk2=strtok(tk2, "\n");
	      strcpy(token2,tk2);
	      tk=&token2[1];
	      // printf("t2:%s \n",tk);
	      pinput[pos]= (char*)malloc(TAMIO * sizeof(char));
	      strcpy( pinput[pos],tk); 
	    }	  
	}
      k=k+1;
      // return 1;
      *posi=(pos+1);
    }

 

}



int which(char c)
{
  int i=0;
  for (i=0;i<NUM_INSTRUCTIONS;i++)
    if (Instructions[i]==c) return i;
  return -1;
}



// translates program to ID program
int prgtoint(char *mprg)
{
  int i,num=0,tam,d=0,p=1;
 
  tam=strlen(mprg);
  for (i=0;i<tam;i++)
    {
      d=which(mprg[(tam-i)-1]);
      num=num+(d+1)*p;
      p=p*NUM_INSTRUCTIONS;
      // printf("num %d\n", num) ;
    }
  return num;
}



// check whether the ins o is in the program
int isoinprg (char *mprg)
{
  int i,iso=0,tam;
  tam=strlen(mprg);
  for (i=0;i<tam;i++)
    {   if (mprg[i]=='o')
	{
	  iso=1;
	  return iso;
	}
    }
  return iso;
}

// check for balanced loops
int isbalanced( char *string)
{
  int counter = 0,lns;
  lns=strlen(string);
for(int i = 0; i <lns ; i++){
    if(string[i] == '[' )
       counter++;
    else if(string[i] == ']' )    // string is a char array
       counter--;
    if(counter < 0)
       return -1; // error because the ')' should come AFTER '('
}
if(counter == 0)
     return 1; // the string is OK
else
     return -1; // unbalanced
}
 
// given a program, return the following in the lexicographic order
void nextprog (char *currentp, char *nextp)
{
  int lns,i,j;
  
  lns=strlen(currentp);
  // printf("jj %d",lns);
  strcpy(nextp,currentp);
  for (i=(lns-1);i>=0;i--)
    {
      
      if (currentp[i]==Instructions[(NUM_INSTRUCTIONS-1)])
	{
	  nextp[i]=Instructions[0];
	  // printf("i %d",i);
	  continue;
	}
      else
	{
	  nextp[i]=Instructions[(which(currentp[i]) +1)];
	  
	  // printf("%c \n", nextp[i]);
	  //	  printf("%c %d \n", currentp[i],i);
	  return;
	}
    }
  for (i=(lns);i>=0;i--)   nextp[i]=Instructions[0];
  nextp[lns+1]='\0';
}

/*************************************/


long powi(long x, unsigned n)
{
  long  p;
  long  r;

  p = x;
  r = 1.0;
  while (n > 0)
    {
      if (n % 2 == 1)
	r *= p;
      p *= p;
      n /= 2;
    }

  return(r);
}

void runBF(char *ip, char *program, ssize_t prg_size, char *ptr) {
  int count = 0;
  while (ip < (program + prg_size)) {
    switch(*ip) {
    case '>': ++ptr; break;
    case '<': --ptr; break;
    case '+': ++(*ptr); break;
    case '-': --(*ptr); break;
    case '.': putchar(*ptr);
      fflush(stdout);
      break;
    case ',': *ptr = getchar();
      if (*ptr == EOF) exit(0);
      break;
    case '[':
      if (!*ptr) {
	count = 1;
	while (count) {
	  ++ip;
	  if (*ip == '[') ++count;
	  if (*ip == ']') --count;
	}
      }
      break;
    case ']':
      if (*ptr) {
	count = 1;
	while (count) {
	  --ip;
	  if (*ip == ']') ++count;
	  if (*ip == '[') --count;
	}
      }
      break;
    }
    ++ip;
  }
}


void text2alphabet(char *text, int count) {
  while(count--) {
    switch(*text) {
    case '.': *text='\0'; break;
    case '0': *text=beflast; break;
    case '1': *text=last; break;
    default : *text -= ('A'-1); break;
    }
    text++;
  }
}


void alphabet2text(char *text,int counto) {
  while(counto--) {
    switch(*text) {
    case '\0': *text='.'; break;
    case beflast: *text='0'; break;
    case last: *text='1'; break;
    default : *text += ('A'-1); break;
    }
    text++;
  }
}

int runMP(char *ip, char *program, ssize_t prg_size, char *ptr, char *output) {
  int count = 0, counto=0, counts=0, countj=0;
  char *ptr0 = ptr;
  int openloops = 0;
  int abort = 0;
  while (ip < (program + prg_size)) {
    if (DEBUG_MP)
      printf("%d,%d,%d,%lu ", counto, counts, countj, ip-program);
    if (counts == LIMIT_STEPS) {
      if (VERBOSE)
	printf("\nReached maximum number of steps\n");
      break;
    }
    counts++;  // num of steps
    switch(*ip) {
    case '>': // ++ptr;  // We can go right freely (the input is usually padded right with '.' so will stop in loops, but just in case there is a liit
      if (!(ptr==ptr0+RIGHT_LIMIT)) { // We cannot go right beyond the start + RIGHT_LIMIT
	++ptr;
      } else if (openloops > 0) {	  // We get out of a loop if we're in one
	count = 1;
	while (count && (ip < (program + prg_size)) ) {  // We need to check we don't go out of the program
	  ++ip;
	  if (*ip == '[') ++count;
	  if (*ip == ']') --count;
	  countj++;
	}
	openloops--;
      }
      break;
    case '<': // --ptr;  // We can go left freely (the input is usually padded left with '.' so will stop in loops
      if (!(ptr==ptr0-LEFT_LIMIT)) { // We cannot go left beyond the start - LEFT_LIMIT
	--ptr;
      } else if (openloops > 0) {	  // We get out of a loop if we're in one
	count = 1;
	while (count && (ip < (program + prg_size)) ) {  // We need to check we don't go out of the program
	  ++ip;
	  if (*ip == '[') ++count;
	  if (*ip == ']') --count;
	  countj++;
	}
	openloops--;
      }
      break;
    case '+': *ptr=(*ptr + 1)%lenAlphabet;
      break;
    case '-': if (*ptr) {
	--(*ptr);
      } else {
	(*ptr)=lenAlphabet-1;
      }
      break;
    case 'o': *(output+counto) = *ptr;
      counto++;
      break;
    case '[':
      if (!*ptr) {
	count = 1;
	while (count && (ip < (program + prg_size)) ) {  // We need to check we don't go out of the program
	  ++ip;
	  if (*ip == '[') ++count;
	  if (*ip == ']') --count;
	  countj++;
	}
      } else {
	openloops++;
      }
      break;
    case ']':
      if (openloops <= 0) {  // Might crash here
	if (VERBOSE) printf(" ERROR (UNMATCHED CLOSING BRACKET) ");  // Module definition
	if (VERBOSE) fflush(stdout);
	abort = 1;
	break; // exit (note that the break only exits the switch, we need the abort)
      } else {
	if (*ptr) {
	  count = 1;
	  while (count) {
	    --ip;
	    if (*ip == ']') ++count;
	    if (*ip == '[') --count;
	    countj++;
	  }
	} else {
	  openloops--;
	}
      }
      break;
    default:
      printf(" ERROR (UNKNOWN INSTRUCTION) ");
      break;
    }
    if (abort) break;
    ++ip;
  }

  *(output+counto) = '\0';

  if (DEBUG_MP) printf("\n");
  if (VERBOSE) printf("Num. of steps: %d. Num. of jump steps: %d\n", counts, countj);
  numsteps=counts;// we store in a global variable the number of steps
  return counto;
}


#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

long TRIED_PROGRAMS;
/* Args: aux mem (with left dots), Input, aux mem (pointer to initial pos after dots), aux mem for output, output, length of output, flag to show if we run only programs with o
 */
int generate_and_try_all3(  char memory[NUMPAIRS][MEM_SIZE+1],  char right_output [NUMPAIRS] [OUTPUT_SIZE], int len_output[NUMPAIRS], int npairs) {
  //	char programU[PROG_SIZE] = {0};  // Dangerous: can exhaust the stack
  int tok=0,k=0;
  long int mk=0;
  char prgo[256],np[256];
  int i,ret=0,ind=0;
  int counto,tam;
  int inst, iso=0;
  float j,tmp=MAXPRG;
  char  outputU[OUTPUT_SIZE] = {0} , memoryU[MEM_SIZE+1] = {0}, *memU = memoryU+LEFT_PADDING_DOTS;;

  strcpy(prgo,">");// initial program
  for (j=0;j<MAXPRG;j++)
    {
    /*  if (k==1000000) 
        {
         printf("%u  %s  \n: ",strlen(prgo),prgo);
         mk++;
         k=0;
         } */
      iso=isoinprg(prgo); //check if the o instruction is there
      if ( iso==1) iso=isbalanced(prgo);
      tam=strlen(prgo);
       
      if ( iso==1 && strstr("[]",prgo) == NULL) {

        for (i=0;i<npairs;i++)
	  {
	    // Recovers the memory (input) fresh
	    memcpy(memoryU,memory[i],MEM_SIZE+1);  // We are going to use memoryU to have it fresh everytime
	    numsteps=0;
	    counto= runMP(prgo,prgo,tam,memU,outputU);
	    if (numsteps==LIMIT_STEPS) break; //we reach the limit
	    TRIED_PROGRAMS++;
	
     //  if(tok==1)	//TEST
            //        {//TEST
                //      printf("%s,%s,%s\n",prgo,memU,outputU);//TEST
                    //  printf("%d\n",memcmp(right_output[i],outputU,MAX(len_output[i],counto)));   //TEST   
               //       alphabet2text(outputU,counto); //counto);//TEST
	         //     puts(outputU);//TEST

   //		}//TEST
	
	    //	printf("%s,%s,%s\n",prgo,memU,outputU);
	    // Must check equivalence for the maximum of the length (if they have different length they are not the same)
	    if (memcmp(right_output[i],outputU,MAX(len_output[i],counto))!=0) break; //Not covered try a different prog
	    if (i==(npairs-1))
	      {
		strcpy(prg,prgo);
		return 1;
	      }
	  }
	
      }
      nextprog(prgo,np);
   //   if (!strcmp(np,"[>]<[o<]")) //[>]<[o<] //TEST
        //  {//TEST
        //   printf("%s %d \n",np,j);//TEST
        //   tok=1;//TEST
       //   }//TEST
        // else tok=0;//TEST

      strcpy(prgo,np);
     // k++;
        
    }

  return ret;

}





void generate_and_try_all( char *m [NUMPAIRS], char *right_output [NUMPAIRS], int npairs) {
  
  char memory [NUMPAIRS] [MEM_SIZE+1] = {0}, right_output2[NUMPAIRS] [OUTPUT_SIZE]  = {0};
  char *mem [NUMPAIRS]; //; = memory+LEFT_PADDING_DOTS;
  int ret, len_output[NUMPAIRS],i;


  //prepare the pairs
  for (i=0;i<npairs;i++)
    {
      // Prepare the memory (output)
      strcpy(right_output2[i],right_output[i]);
      len_output[i] = strlen(right_output2[i]);
      text2alphabet(right_output2[i],len_output[i]);
             

      mem[i]= memory[i]+LEFT_PADDING_DOTS;
             
      // Prepare the memory (input)
      memset(memory[i], '.', MEM_SIZE);  // It was already full of 0s,
      strncpy(mem[i], m[i], strlen(m[i]));
     // printf("mem %s, \n",memory[i]);
      text2alphabet(memory[i],MEM_SIZE);

	    

      //printf("Input (%lu characters):\n", strlen(m));
      //puts(m);
      // printf("Whole input (pointer at appropriate position):\n");
      // puts(memory);
      //    puts(mem);


      //    puts(mem);
      // printf("True output:\n");
      // puts(right_output);
      // printf("Running...");
    }
 

  clock_t begin = clock();
  TRIED_PROGRAMS= 0;
  numsteps=0;
  /* here, do your time-consuming job */

  int outputinst = 0; // Use 1 if we want to try all programs. Use 0 if we only want to use those programs that have instruction 'o'
  ret = generate_and_try_all3( memory, right_output2, len_output,npairs);
  if (ret) {
    // printf("\nFOUND!!!\n");
  } else {
    //  printf("\nNOT FOUND :-(\n");
    strcpy(prg,"fail");
  }


  // printf("Tried programs: %lu from a total of %d^%d + %d^%d + ... = %lu\n", TRIED_PROGRAMS, NUM_INSTRUCTIONS, n, NUM_INSTRUCTIONS, n-1, max_programs);
  //  printf("Tried programs: %lu, Steps of solution :%lu \n", TRIED_PROGRAMS,numsteps);
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  //printf("Time spent: %.2f seconds", time_spent);


}


char *learnp3(char *pairs)
{
    char *pinputs[NUMPAIRS], *poutputs[NUMPAIRS];
    int i=1,pos;
    
  splitpairs(pairs,pinputs,poutputs,&pos);
  // printf(" kk%d \n",pos);
  for (i=0;i<pos;i++)
    strcat(pinputs[i],".");
  generate_and_try_all(pinputs,poutputs,pos);
  // strcpy(mprg,prg);
  return prg;
}


