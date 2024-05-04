#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "neuron.h"

// to calculate sigmoid
double sigmoid(double x)
{
	return (1.0/(1.0+exp(-x)));
}

// absolut value
double bas_val(double x)
{
	if(x<0)
	{
		return -x;
	}
	return x;
}


// init Neuron
Neuron* n_init(Neuron* n, unsigned int list[],size_t le)
{
	//if size_t ==0 or 1
	if(le<=2)
	{
		errx(EXIT_FAILURE,"init Neuron list is incorrect");
	}
	// lent of pointer 
	int sum=0;
	
	// add neuron and link
	for(size_t i=0;i<le-1;i++)
	{
		sum+=list[i];
		sum+=list[i]*list[i+1];
	}
	sum+=list[le-1];
	
	// set value of neuron
	n->lent=le;
	
	// set list len
	for(int i=0; i<(int)le;i++)
	{
		(n->l_len)[i]=list[i];
	}
	(n->l_len)[(int)le]='\0';
	n->val=malloc((size_t)(sum)*sizeof(double));
	// if i can't creat val
	if(n->val==NULL)
		errx(EXIT_FAILURE,"Not enough memory!");
	
	for (int i=0; i<sum;i++)
	{
		(n->val)[i]=0.0;
	}
	return n;
}

// free Neuron
void free_n(Neuron* n)
{
	free(n->val);
}


// val of num Neuron
double* val_neuron(Neuron* n,int num)
{
	// if num is not a natural number
	if(num<0)
		errx(EXIT_FAILURE,"num must be positif");
	
	int place=0;// this is place of element-sum
	int i=0; 

	// if num is not in this ligne of neuron
	while(i<(int)(n->lent) 
			&&(n->l_len)[i]<(unsigned int)num)
	{
		//security for out range
		if((int)(n->lent)-1>i)
		{
			place+=(n->l_len)[i]+(n->l_len)[i]*(n->l_len)[i+1];
		}
		// sub num
		num-=(n->l_len)[i];
		i++;
	}

	// if num is not out of possiblity
	if(i==(int)(n->lent)-1 
			&& (n->l_len)[i]<(unsigned int)num)
	{
		errx(EXIT_FAILURE,"there is too big for send neuron");
	}
	else
	{
		return (n->val)+num+place-1;
	}
}

//first element of num neuron
double* first_link(Neuron* n, int num)
{	
	// if num is not a natural number
	if(num<0)
		errx(EXIT_FAILURE,"num must be positif");
	int place=0;// this is place of element-sum 
	int i=0;
	//if i and num is not in this ligne of neuron
	while(i<(int)(n->lent) 
			&&(n->l_len)[i]<(unsigned int)num)
	{
		//security of out range
		if((int)(n->lent)-1>i)
		{
			place+=(n->l_len)[i]+(n->l_len)[i]*(n->l_len)[i+1];
		}
		num-=(n->l_len)[i];
		i++;
	}
	// if num is not out of possiblity
	if(i==(int)(n->lent)-1)
		errx(EXIT_FAILURE,"there is too big for link");
	else
	{
		num--;
		return (n->val)+place+(n->l_len)[i]+
			(num*(n->l_len[i+1])); 
	}
}

double* ancestor_link(Neuron* n,int num)
{
	// if num is not a natural number
	if(num<0)
		errx(EXIT_FAILURE,"num must be positif for ancestor link");
	int place=0;// this is place of element-sum 
	int i=0;
	if((unsigned int)num<=(n->l_len)[0])
		errx(EXIT_FAILURE,"num must be not a first neuron");
	//if i and num is not in this ligne of neuron
	while(i<(int)(n->lent) 
			&&(n->l_len)[i]<(unsigned int)num)
	{
		//security of out range
		if((int)(n->lent)-1>i)
		{
			place+=(n->l_len)[i]+(n->l_len)[i]*(n->l_len)[i+1];
		}
		num-=(n->l_len)[i];
		i++;
	}	
	// if num is not out of possiblity
	if(i==(int)(n->lent)-1 && (unsigned int)num>(n->l_len)[i])
		errx(EXIT_FAILURE,"there is too big for ancestor link");
	else
	{
		if(i)
		{
			num--;
			return (n->val)+place-((n->l_len[i])*(n->l_len[i-1]))+
				(num); 
		}
		else
		{
			errx(EXIT_FAILURE,"Neuron: i must be positif");
		}
	}
}

void print_n(Neuron* n)
	//print neuron
{
	printf("---------------------------------\n");
	int place = 0;//place of pointer
	for(int i=0;i<(int)(n->lent)-1;i++)
	{
		// print neuron
		for(unsigned int elem=0;elem<(n->l_len)[i];elem++)
		{
			printf("%f,\t",*((n->val)+place));
			place++;
			// this is good presentation
			if((n->l_len)[i+1]!=1)
			{
				if((n->l_len)[i+1]!=2)
				{
					for(unsigned int jump=0;
						jump<(n->l_len)[i+1]
						+(n->l_len)[i+1]/2;jump++)
					{
						printf("\t");
					}
				}
				else
				{
					for(unsigned int jump=0;
						jump<(n->l_len)[i+1];jump++)
					{
						printf("\t");
					}
				}
			}
		}
		printf("\n");
		//print link of neuron
		for(unsigned int elem=0;
				elem<(n->l_len)[i]*(n->l_len)[i+1];elem++)
		{
			printf("%f,\t",*((n->val)+place));
			place++;
		}
		printf("\n\n");
	}
	//print last neuron
	for(unsigned int elem=0;elem<(n->l_len)[(int)(n->lent)-1];elem++)
	{
		printf("%f,\t",*((n->val)+place));
		place++;
	}
	printf("\n");
	printf("---------------------------------\n");
}

//save neuron
void save_Neuron(Neuron* n,char filename[])
{
	// to creat new file
	char result[] = ".neuron";
	
	int lent=0;
	for (;filename[lent];lent++);
	
	char newfilename[lent + 8 + 1];
	
	int i;
	
	for (i = 0; filename[i] != '\0'; i++)
		newfilename[i] = filename[i];
	int j;
	
	for (j = 0; result[j] != '\0'; j++)
		newfilename[i+j] = result[j];
	newfilename[i+j] = '\0';
	
	FILE *f;
	f = fopen(newfilename, "w");
	
	if (f == NULL)
  		errx(1, "Error save file %s ", filename);
	
	unsigned int sum=0;
	fprintf(f, "%ld\n", (n->lent));//print lent in first line
	
	// print the list in second line
	for(int in=0;in<(int)(n->lent)-1;in++)
	{
		fprintf(f,"%d ",(n->l_len)[in]);
		sum+=(n->l_len)[in]+((n->l_len)[in]*(n->l_len)[in+1]);
	}
	fprintf(f,"%d\n",(n->l_len)[(int)(n->lent)-1]);
	sum+=(n->l_len)[(int)(n->lent)-1];
	// print the values of neuron
	for(unsigned int in=0;in<sum;in++)
	{
		fprintf(f,"%f\n",*((n->val)+in));
	}
	fclose(f);
}

// load Neuron
void load_Neuron(Neuron* n,char filename[])
{
	FILE* f;
	f = fopen(filename, "r");

	if (f == NULL)
    		errx(1, "Error loading file %s ", filename);
	
	unsigned int l=0;
	unsigned int values=0;
	unsigned int p=1000000;//for double
	unsigned int in=0;
	char c;
	while ((c = fgetc(f)) != EOF)
	{
    		if (c == '\n')
		{
			// calculat if line is 0 or 1 or more 
			if(l==0)
			{
				(n->lent)=(size_t) values;
			}
			if(l==1)
			{
				(n->l_len)[in]=values;
				(n->l_len)[(int)n->lent]='\0';
				in=0;
				unsigned int sum=0;
				for(;in<(n->lent)-1;in++)
				{
					sum+=(n->l_len)[in]
						+(n->l_len)[in+1]*(n->l_len)[in];
				}
				sum+=(n->l_len)[n->lent-1];
				n->val=malloc((size_t)(sum)*sizeof(double));
			}
			if(l>1)
			{
				*(n->val+l-2)=(double)values/p;
			}
			values=0;
			l++;
		}
		else
		{
			//printf("%c,%i\n",c,l);
			//same  but too short
			if(c==' ')
			{
				(n->l_len)[in]=values;
				in++;
				values=0;
			}
			else
			{
				if(c!='.')
					values= (values*10)+c-'0';
				if(c=='-')
					values=-values;
			}
			/*if(l==0)
			{
				values+= c-'0';
			}
			if(l==1)
			{
				if(c==' ')
				{
					(n->l_len)[in]=values;
					in++;
				}
				else
				{
					values+= c-'0';
				}
			}
			if(l>1)
			{
				values+= c-'0';
			}*/

		}
	}
	fclose(f);
}

/*
int main()
{	
	
	Neuron N1;
	unsigned int list[]={2,3,1};
	n_init(&N1,list,(size_t)3);
	double* ne=val_neuron(&N1,1);
	*(ne)=1.0;
	ne=val_neuron(&N1,2);
	*(ne)=2.0;
	ne=val_neuron(&N1,3);
	*(ne)=1.5;
	ne=val_neuron(&N1,4);
	*(ne)=2.5;
	ne=val_neuron(&N1,5);
	*(ne)=2.3;
	
	ne=first_link(&N1,1);
	*(ne)=1.2;
	*(ne+1)=1.3;
	ne=first_link(&N1,2);
	*ne=1.4;
	*(ne+1)=1.55;
	ne=first_link(&N1,3);
	*ne=1.6;
	ne=first_link(&N1,4);
	*ne=1.7;
	printf("Neuron:\n");
	print_n(&N1);
	//printf("ancestor of 3 is %f\n",*(ancestor_link(&N1,3)));
	//printf("ancestor of 4 is %f\n",*(ancestor_link(&N1,4)));
	//printf("ancestor of 5 is %f\n",*(ancestor_link(&N1,5)));
	//printf("ancestor of 6 is %f\n",*(ancestor_link(&N1,6)));
	
	Neuron N2;
	char t[]={'t','e','s','t','\0'};
	char t2[]={'t','e','s','t','.','n','e','u','r','o','n','\0'};
	save_Neuron(&N1,t);
	load_Neuron(&N2,t2);
	printf("neuron load:\n");
	print_n(&N2);
	
	free_n(&N2);
	free_n(&N1);
	return 0;
}
*/