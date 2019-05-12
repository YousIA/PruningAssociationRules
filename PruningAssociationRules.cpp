#include <iostream>
#include <vector>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <sys/time.h>
#include <unistd.h>
#include<math.h>
using namespace std;
/*****Input program******/
int Nb_rules =0; // number of rules
#define max_rules 3000 // maximum number of rules
#define N 16 // number of items
#define M 40// Number of transactions
#define k 1 // le nombre de clusters
#define K 20 // the number of bees 
# define alpha 0.5 // represents the weight of the support in the first fitness function
# define Beta 0.5 // represents the weight of the confidence in the first fitness function
# define minsup 0.5 // represents minimum support thershold  of meta rules
# define minconf 0.5  // represents minimum confidence thershold of meta rules
int  flip=1; // the empirical parametre for the first and the second strategy
# define Distance 7 //  the empirical parametre for the third strategy
int  IMAX=2; // the maximum number of iteration of the BSO algorithm
int IMAX_iter_TS=1; // the  maximum number of iteration of tabu search algorithm
int Sx_union_Sy=0;
# define  List_size 100 // the maximal size of tabu list
int nb_element=0;// the number of current element in the tabu list
typedef struct bbb{
int centre[N];
}center;
typedef struct aaa{
	center obj;
    int cluster;
}objet;
typedef struct rrrr
{
int trans[max_rules]; 
} tr;
typedef struct vvvv
{
int trans[max_rules];
float cost; 
} vr;
typedef struct {int solution[max_rules]; float cost;} bee;
typedef struct {int trans[N]; int nb;} ligne;
vector<bee> T_Dance(K);
bee Sref;
bee T_List[List_size];// the tabu list 
//vector<objet> T_objet (Nb_rules);
objet T_objet[max_rules];
vector<objet> centers(k);
vector<tr> Trans_rules(N);
vector<ligne> dataset(M);
vector<vr> Meta_rules(K*IMAX);
/**************prototype declaration*******/
void download_ruleset();
void read_trans();
void display_ruleset();
void gen_rules();
/******clustering methods**********/
void initialiser_centers();
float Manhattan(objet x, objet y );
float Euclidian(objet x, objet y);
float Euclidian_squared(objet x, objet y);
float Chebychev(objet x, objet y);
float Dist_item(objet x, objet y);
float Dist_aicha(objet x, objet y);
float d(objet x, objet y); // Jaccard coefficient used on Reynolds work.
float n(objet x, objet y); // Jaccard coefficient used on Reynolds work.
void classical_center_computation(int cluster);
void aicha_center_computation(int cluster);
void Uoi(int cluster);
void Iuc(int cluster);
void Uic(int cluster);
void assigned_clusters();
void Kmeans_function();
float MMD(); //Minimize max diameter of clusters
float MMAC(); //Minimize average diameter of clusters
float MARC(); // Minimize average radius of cluster
float MADP();// Minimize average distance between points in cluster
float MMSD(); //Minimize means-squared distance from point to center
float silouhette (objet x);
float Avearage_sl();
/*ARM Methods*/
void ARM_process(); // exctract all possible rules from the dataset. 
void init_trans_rules(); // inialize trans_rules structure for each cluster
void shared_consequent(int cluster); // project and transforms the rule base in trans_rules according to the specified cluster
float support_rule(int s[]); // this function calculates the support of the entire solution s
float support_antecedent(int s[]); // this function computes the support of the antecedent of the solution s
float support_consequent(int s[]); // this function computes the support of the consequent of the solution s
float confidence(int s[]); // it calculates the confidence of the rule
float leverage(int s[]); // it calculates the leverage of the rule
float lift(int s[]);// it calculates the lift of the rule
float fitness1(int s[]); // computes the fitness of a given solution s
void create_Sref(); // here we create the solution reference sref and initialize it with the random way
bee neighborhood_computation(bee S);// this function explores the local region for each bee
bee TS(bee S); // this function explores the local region for each bee using Tabu Search 
bool verify(int t[], int v[]); // verify if the two arrays are the same
void search_area1(bee s, int iteration); //detremines the search area for each bee using the first strategy
void search_area2(bee s, int iteration); //detremines the search area for each bee using the second strategy
void search_area3(bee s, int iteration); //detremines the search area for each bee using the third strategy
int W(int t[]); // indicates the  weight of solution representing by a vector t, this function is used on search_area3()
void copy(int t[], int v[]); // it copies the vector t in the vector v
int best_dance(); // return the best dance after the exploration of search region of each bee
void display_bees(); // display solutions
void display_solution(bee S); // this function display the current solution with its cost
int number_relevant_meta_rules(); // calculates the number of meta rules which theri confidence is 100%
/*********final processing methods**********/
void insert_MR(int iteration); // insert meta rules found into Meta_rules
void display_MR();  // display all meta rules found
void pruning_rules(); // pruning rules using MR and clusters information
void returned_rules_evaluation(); // evaluation of the output rules 
void display_tran(); // display inital transactions
/***************main program*******/
int main(void)
{
    //download_ruleset();
    read_trans(); 
    //display_tran();
    //printf("*************\n");
    ARM_process();
   
    //gen_rules();
     clock_t t1,t2;
     t1=clock();
   //Kmeans_function();
   //display_ruleset();
    printf ("%d\n",Nb_rules);
   t2=clock();
   float diff ((float)t2-(float)t1);   
  float seconds = diff / CLOCKS_PER_SEC;
  //printf("\n%f\n",seconds);
  //printf("\n%f",MMSD());
       /*printf("\n%f",MMD());  
       printf("\n%f",MMAC());  
       printf("\n%f",MARC());  
       printf("\n%f",MMSD()); */  
    //printf("%f",);
   //Kmeans_function();
//printf("%f\n\t",MMSD());
int i,j;bee best;

/*for (int s=0; s<Nb_rules;s++)
   {
   for (int o=0;o<N;o++)
   {
       printf("%d  ",T_objet[s].obj.centre[o]);
   }    
   printf("\n");
   }
   printf("\n\n************\n");*/
//for (int cluster=0; cluster<k;cluster++)
//{
   init_trans_rules();
   shared_consequent(0);  
   create_Sref();
  //display_solution(Sref);
   search_area3(Sref,IMAX);    
     for (i=0; i<IMAX;i++)
	      {
	        int existe=0,l=0,indice=0;			
	        for (j=0;j<K;j++) // neighborhood computation for all the solution in tab
			  		{ 
				     T_Dance[j]=neighborhood_computation(T_Dance[j]);
				  	}
				    j=best_dance();
					copy(T_Dance[j].solution,Sref.solution);
					Sref.cost=T_Dance[j].cost;
					if (Sref.cost > best.cost)//atte o maximisation
			    { 
					 copy(Sref.solution, best.solution);
					 best.cost=Sref.cost;
			    }
      	  insert_MR(i); // insert meta rules found in Meta_rules
      search_area3(Sref,IMAX);

          }// Bso ending
          
          pruning_rules();
          printf("end");
            //printf("K=%d IMAX=%d flip=%d  %f\n",K, IMAX, flip, seconds);
      //printf ("\n%d  %d\n",flip, number_relevant_meta_rules());
      
      //display_bees();
    //display_MR();
// }  // end main loop (of each cluster)
 //printf ("\n%d\n", number_relevant_meta_rules());
    returned_rules_evaluation();

 int z; scanf("%d",&z); 
return 0;     
}
/*******read transactional data bass and insert it in the data set structure********************************/
void read_trans()
{
char c, i=0, l=0;
FILE *f=NULL;
f=fopen("C://Users//Invité//Desktop//Ladjel//transaction_index.txt","r");
if (f!=NULL) {/*printf("the file is succefully opened\n");*/} 
while (c!=EOF)
{
        c=fgetc(f); 
        if (c!=' ' && c!='\n') {dataset[i].trans[l]=atoi(&c);l++;}
        if (c=='\n'){dataset[i].nb=l;l=0;i++;}
}   
fclose(f); 
}
/**************exctract all possible rules from the dataset. ********************/
void ARM_process()
{
     int i,j;
      for (i=0;i<max_rules;i++)
      {
       for (j=0;j<N;j++)
        {
        T_objet[i].obj.centre[j]=0;
        }
      }
 
  for (i=0;i<M;i++)
   {
     for (j=0;j<N;j++)
      {
          if (dataset[i].trans[j]==1)
           {
              
              for (int r=j+1;r<N;r++)
               {
                if (dataset[i].trans[r]==1)
                  {
                  T_objet[Nb_rules].obj.centre[j]=1;
                  T_objet[Nb_rules].obj.centre[r]=1;
                  Nb_rules++;                         
                  }    
               }                       
           }    
      }        
   }
    for (i=0;i<M;i++)
      {
       for (j=0;j<N;j++)
        {
        T_objet[Nb_rules].obj.centre[j]=dataset[i].trans[j];
        }
        Nb_rules++;
      }
    for (i=0;i<M;i++)
      {
       bool exist=false;
       for (j=0;j<N;j++)
        {
        if (dataset[i].trans[j]==1 && exist==false)
        {
        T_objet[Nb_rules].obj.centre[j]=0;
        exist=true;
        }
        else {
              T_objet[Nb_rules].obj.centre[j]=dataset[i].trans[j];
             }
        }
        Nb_rules++;
      }          
}
/*****************************genrates rules from data base************/
void gen_rules()
{
   for (int i=0;i<Nb_rules;i++)
    {              
     for (int j=0;j<N;j++)
     {      
    T_objet[i].obj.centre[j]=rand()%2;
     }
    }
    //printf("\n");
}
/********************recuperates rules base and put it on T_objects table*********/
void download_ruleset()
{
char c ;
int j;
int i=0;
FILE *f=NULL;
f=fopen("C:\\Users\\Invité\\Desktop\\benchmarks\\synthetic rules set\\R_497_1516.txt","r");
if (f!=NULL) {/*printf("the file is succefully opened\n");*/} 
j=0;
while (c!=EOF)
{
   
        c=fgetc(f); 
        if (c=='\n'){i++;j=0;}
        else {
             if (c!=' '){
                      T_objet[i].obj.centre[j]=atoi(&c);j++;
                        }
             }
             
}    
//printf("\n");
fclose(f); 
}
/*******************display dataset***********************/
void display_ruleset()
{
  for (int  i = 0; i < Nb_rules; i++)
    {
              // if (T_objet[i].cluster==cluster)
                //  {   
              for ( int j=0;j<N;j++)
              {
              printf ("%d ",T_objet[i].obj.centre[j]);
              //if (T_objet[i].cluster==0)
              //printf("%d ", T_objet[i].cluster);
              }   
              printf("\n"); 
              //}         
    }    
}
/***************initaliser_ centers**********************/
void initialiser_centers()
{
for (int i=0;i<k;i++)
  {
      for (int j=0;j<N;j++)
      {
	   centers[i].obj.centre[j]=T_objet[i].obj.centre[j];
      }      
    T_objet[i].cluster=i;
  }
}
/**********similarity measures between rules****************/
float Manhattan(objet x, objet y )
{ 
float a=0;
	for (int i=0;i<N;i++)
	{
		int d=(int)(x.obj.centre[i]-y.obj.centre[i]);
		a=a+abs(d);
	}

return a;
}

float Euclidian(objet x, objet y)
{
float a=0;
	for (int i=0;i<N;i++)
	{
		int d=(int)(x.obj.centre[i]-y.obj.centre[i]);
		a=a+(float)pow((double)d,(double)2);
	}
	a=(float)sqrt((double)a);
return a;	
}
float Euclidian_squared(objet x, objet y)
{
float a=0;
	for (int i=0;i<N;i++)
	{
		int d=(int)(x.obj.centre[i]-y.obj.centre[i]);
		a=a+(float)pow((double)d,(double)2);
	}
return a;	
}
float Chebychev(objet x, objet y)
{
float a=0;
	for (int i=0;i<N;i++)
	{
		int d=(int)(x.obj.centre[i]-y.obj.centre[i]);
		if (d>a)
		{
			a=d;
		}
	}
return a;	
}

float Dist_item(objet x, objet y)
{
int nb_item_total=0;
int nb_item_commun=0;
float a;
for (int i=0;i< N;i++)
{	
	
	if ((x.obj.centre[i]!=0) ||(y.obj.centre[i]!=0))
	{
	nb_item_total++;	
	}
	if ((x.obj.centre[i]!=0)&& (y.obj.centre[i]!=0))
	{
		nb_item_commun++;
    }
	
}

a=nb_item_total-nb_item_commun;
return a;	
}

float Dist_aicha(objet x, objet y)
{
	int nb_item_total=0;
	int nb_item_commun=0;
	int nb_item_x=0;
	int nb_item_y=0;
	float b,c;
	float a;
	for (int i=0;i< N;i++)
	{	
		
		if ((x.obj.centre[i]!=0) ||(y.obj.centre[i]!=0))
		{
		nb_item_total++;	
		}
		if ((x.obj.centre[i]!=0)&& (y.obj.centre[i]!=0))
		{
			nb_item_commun++;
	    }
		if (x.obj.centre[i]!=0)
		{
		nb_item_x++;	
		}
		if (y.obj.centre[i]!=0)
		{
		nb_item_y++;	
		}
		
	}
b=nb_item_total-nb_item_commun;
c=(2*(nb_item_y+nb_item_x));
a=b/c;
return a;
}
float d(objet x, objet y)
{
int Sx[M],Sy[M];      
      
      
     for (int i=0;i<M;i++)
	   {
      Sy[i]=0;
      Sx[i]=0;
       }  
	for (int i=0; i<M; i++)
		{
		bool appartient1=true;
		bool appartient2=true;
		int j=1;
		while (j<N)
		{
		 if (x.obj.centre[j]!=0)
		{
			int l=0; 
			bool existe=false;
			while (l< dataset[i].nb && existe==false)
			{
				if (dataset[i].trans[l]==j)
				{existe=true;}
			l++;
			}
			if (existe==false){appartient1=false;}
		}
		if (y.obj.centre[j]!=0)
		{
			int l=0; 
			bool existe=false;
			while (l< dataset[i].nb && existe==false)
			{
				if (dataset[i].trans[l]==j)
				{existe=true;}
			l++;
			}
			if (existe==false){appartient2=false;}
		}
		j++;	
		}
		if (appartient1==true) {Sx[i]=1;}
		if (appartient2==true) {Sy[i]=1;}
    }
		int compt_inter=0;
	   for (int i=0;i<M;i++)
	   {
        if (Sx[i]==1 || Sy[i]==1) {Sx_union_Sy++;}
        if (Sx[i]==1 && Sy[i]==1) {compt_inter++;} 
       }
       float dd=Sx_union_Sy-compt_inter;
	return dd;
}
float n(objet x, objet y)
{
      float term1=d(x,y);
      return(term1/Sx_union_Sy);
}

/***************center computation formulas between rules*******************/
void classical_center_computation(int cluster)
{
	int nbr=0;
	objet obj_temp;
	for (int j=0;j<N;j++)
    {
		obj_temp.obj.centre[j]=0;
    }
for (int i=0;i<Nb_rules; i++)
{
    if (T_objet[i].cluster==cluster)
    {
    	for (int j=0;j<N;j++)
        {
    		obj_temp.obj.centre[j]=obj_temp.obj.centre[j]+T_objet[i].obj.centre[j];
        }
    	nbr++;
    }
}
for (int i=0;i<N;i++)
{
	if (nbr!=0)
centers[cluster].obj.centre[i]=	obj_temp.obj.centre[i]/nbr;
}
}
void aicha_center_computation(int cluster)
{
	int nbr=0;
	int s=0;
	int card_center=0;
	objet obj_temp;
	for (int j=0;j<N;j++)
    {
		obj_temp.obj.centre[j]=0;
		centers[cluster].obj.centre[j]=0;
    }
for (int i=0;i<Nb_rules; i++)
{
	
    if (T_objet[i].cluster==cluster)
    {
    	for (int j=0;j<N;j++)
        {
    		if (T_objet[i].obj.centre[j]==1)
    		{
    			s++;
    			obj_temp.obj.centre[j]++;
    		}
        }
    	nbr++;
    }
}
  if (nbr!=0)
  {
   card_center=s/nbr;
  }
  
for (int i=0;i<card_center;i++)
{	
	int max=obj_temp.obj.centre[0];
    int indice_max=0;
	  for (int j=1;i<N;i++)	
	  {
		  if (max<obj_temp.obj.centre[j])
		  {
			  max=obj_temp.obj.centre[j];
			  indice_max=j;
		  }
		  
	  }	  
   centers[cluster].obj.centre[indice_max]=1;
   obj_temp.obj.centre[indice_max]=0;
}

}

void Uoi(int cluster)
{
objet union_items;
objet intersection_items;
for (int j=0;j<N;j++)
{
	union_items.obj.centre[j]=0;
	intersection_items.obj.centre[j]=1;
}
for (int i=0;i<Nb_rules; i++)
{

    if (T_objet[i].cluster==cluster)
    {
    	for (int j=0;j<N;j++)
        {
    		if (T_objet[i].obj.centre[j]!=0)
    		{
    		union_items.obj.centre[j]=1;
    		}
    		if (T_objet[i].obj.centre[j]==0)
    		{
    		intersection_items.obj.centre[j]=0;
    		}
        }
    }
}
for (int i=0;i<N;i++)
{
centers[cluster].obj.centre[i]=	union_items.obj.centre[i]-intersection_items.obj.centre[i];	
}	
}

void Iuc(int cluster)
{
objet intersection_items;
   for (int j=0;j<N;j++)
   {
	 intersection_items.obj.centre[j]=1;
   }
   for (int i=0;i<Nb_rules; i++)
   {

       if (T_objet[i].cluster==cluster)
       {
       	for (int j=0;j<N;j++)
           {
       		if (T_objet[i].obj.centre[j]==0)
       		{
       		intersection_items.obj.centre[j]=0;
       		}
           }
       }
   }
   for (int i=0;i<N;i++)
   {
    if (intersection_items.obj.centre[i]==1)
     {
     centers[cluster].obj.centre[i]=1;	
     }
    }
}
void Uic(int cluster)
{
	objet union_items;
	for (int j=0;j<N;j++)
	{
		union_items.obj.centre[j]=0;
	}
	for (int i=0;i<Nb_rules; i++)
	{

	    if (T_objet[i].cluster==cluster)
	    {
	    	for (int j=0;j<N;j++)
	        {
	    		if (T_objet[i].obj.centre[j]!=0)
	    		{
	    		union_items.obj.centre[j]=1;
	    		}
	        }
	    }
	}
	for (int i=0;i<N;i++)
	{
	if (union_items.obj.centre[i]==0)	
	centers[cluster].obj.centre[i]=	0;	
	}	
}

/*****************************affectation des clusters************/
void assigned_clusters()
{
for (int i=0;i<Nb_rules;i++)
{
    //float min=Manhattan(T_objet[i], centers[0]);
	//float min=Chebychev(T_objet[i], centers[0]);
//	printf("%f ",min);
    //float min=Euclidian(T_objet[i], centers[0]);
	//float min=Euclidian_squared(T_objet[i], centers[0]);
	//float min=Dist_aicha(T_objet[i], centers[0]);
	float min=Dist_item(T_objet[i], centers[0]);
    int indice=0;
	for (int j=1;j<k;j++)
    {
	//float d=Manhattan(T_objet[i], centers[j]);
	//float d=Chebychev(T_objet[i], centers[j]);
	//printf("%f ",d);
	//float d=Euclidian(T_objet[i], centers[j]);
	//float d=Euclidian_squared(T_objet[i], centers[j]);
	//float d=Dist_aicha(T_objet[i], centers[j]);
	float d=Dist_item(T_objet[i], centers[j]);
		if (d<min){indice=j;}
    }
    //printf("\n");
T_objet[i].cluster=indice;
}
}
/**********************************Main algorithm of kmeans***********/
void Kmeans_function()
{
int iteration=0;
//download_ruleset();
//gen_rules();
initialiser_centers();
while (iteration<1)
{
for (int i=0;i<k;i++)
{
  //classical_center_computation(i);
 //aicha_center_computation(i);
 //Uoi(i);
  //Iuc(i);
 Uic(i);
}
assigned_clusters();
iteration++;
//printf("\n\t%d",iteration);
}
}
/**************************Evaluation Metrics***************/
float MMD() //Minimize max diameter of clusters
{
float measure=0;

    for (int i=0;i<k;i++)
       {
    	
	    for (int j=0;j<Nb_rules;j++)
	    {
	     if (T_objet[j].cluster==i)
	     {	 
	       for (int l=0;l<Nb_rules;l++)
	        {
	    	 if (T_objet[l].cluster==i)
	    	 {
	    	  float d=Manhattan(T_objet[j],T_objet[l]);
	    	  if (d>measure){measure=d;}
	         }
	       }
        }
      }
     }   
return measure;
}
float MMAC() //Minimize average diameter of clusters
{
float measure=0;

    for (int i=0;i<k;i++)
       {
    	float max=0;
	    for (int j=0;j<Nb_rules;j++)
	    {
	     if (T_objet[j].cluster==i)
	     {	 
	       for (int l=0;l<Nb_rules;l++)
	        {
	    	 if (T_objet[l].cluster==i)
	    	 {
	    	  float d=Manhattan(T_objet[j],T_objet[l]);
	    	  if (d>max){max=d;}
	         }
	       }
        }
      }
	    measure=measure+max;
     }   
    measure=measure/k;
return measure;
}
float MARC() // Minimize average radius of cluster
{
float measure=0;

    for (int i=0;i<k;i++)
       {
    	float max=0;
	    for (int j=0;j<Nb_rules;j++)
	    {
	     if (T_objet[j].cluster==i)
	     {	 
	          float d=Manhattan(T_objet[j],centers[i]);
	    	  if (d>measure){max=d;}
	      }
      }
	    measure=measure+max;
     }   
    measure=measure/k;
return measure;
}
float MADP()// Minimize average distance between points in cluster
{
	float measure=0;

    for (int i=0;i<k;i++)
       {
    	float sum=0;
	    for (int j=0;j<Nb_rules;j++)
	    {
	     if (T_objet[j].cluster==i)
	     {	 
	       for (int l=0;l<Nb_rules;l++)
	        {
	    	 if (T_objet[l].cluster==i)
	    	 {
	    	  float d=Manhattan(T_objet[j],T_objet[l]);
	    	  sum=sum+d;
	         }
	       }
        }
      }
	    if (sum>measure){measure=sum;}
     }   
return measure;
}
float MMSD() //Minimize means-squared distance from point to center
{
	float measure=0;

	    for (int i=0;i<Nb_rules;i++)
	    {	 
	        float d=Manhattan(T_objet[i],centers[T_objet[i].cluster]);
	    	measure=measure+d;
       }   
       measure=measure/Nb_rules;
return measure;
}
float silouhette(objet x)
{
   float Sum[k];
   int nb[k];
   for (int i=0;i<k;i++)
   {
   Sum[i]=nb[i]=0;    
   } 
   for (int i=0;i<Nb_rules;i++)
   {
      Sum[T_objet[i].cluster]=Sum[T_objet[i].cluster]+Euclidian(x,T_objet[i]);
      nb[T_objet[i].cluster]++;                                 
   } 
     float min=100000;
     float a;
      for (int i=0;i<k;i++)
       {
       if (i!=x.cluster)
       { 
        if (min >Sum[i]/nb[i])
        {                     
        min=Sum[i]/nb[i];
        }
        else {a=Sum[i]/nb[i];}   
       }
       }
float s=0;
s=min-a;
if (a>min){s=s/a;}
else {s=s/min;} 
printf ("\n%f",s);
return s;
}
float Avearage_sl()
{
      float s=0;
   for (int i=0;i<Nb_rules;i++)
   {
    s=s+silouhette(T_objet[i]);
    }      
    s=s/Nb_rules;
    return s;
}
/********initalize trans _rules structure******/
void init_trans_rules()
{
for (int i=0;i<N;i++)
    {
        for (int j=0;j<Nb_rules;j++)
        Trans_rules[i].trans[j]==0;
    }

}
/**********rules base transformatio to tras_rules*********/
void shared_consequent(int cluster)
{
 for (int i=0;i<Nb_rules;i++)
   {
   //  if (T_objet[i].cluster==cluster)
     //  {
        for (int j=0;j<N;j++)
         {
          if (T_objet[i].obj.centre[j]==1)
            {
            Trans_rules[j].trans[i]=1;                             
            }       
         }
       //}
   }
}
/**********************copry t in v********/
void copy(int t[], int v[])
{
for (int i=0;i<Nb_rules; i++)
{
v[i]=t[i]; 
}     
}
/**************************display_solution*****************/
void display_solution(bee S)
{
for (int i=0;i<Nb_rules;i++)
{
    printf("%d ", S.solution[i]);
}
//printf ("cost is:%f",S.cost);
printf("\n");
}
/*************************compute the support of the solution s**********/
float support_rule(int s[])
{
float compt=0;
		for (int i=0; i<N; i++)
		{
		bool appartient=true;
		
		int j=1;
		while (j<Nb_rules)
		{
		 if (s[j]!=0)
		{
			int l=0; 
			bool existe=false;
			while (l< Nb_rules && existe==false)
			{
				//if (Trans_rules[i].trans[l]==j)
				if (Trans_rules[i].trans[j]==1)
                {existe=true;}
			l++;
			}
			if (existe==false){appartient=false;}
		}
		j++;	
		}
		if (appartient==true) {compt++;}
		}
		//printf("\n%f",compt);
	   compt=compt/N;
	return compt;
}
/*****************************support antecedent computing*****************************/
float support_antecedent(int s[])
	{
		float compt=0;
		
		for (int i=0; i<N; i++)
		{
		bool appartient=true;
		int j=1;
		while (j<Nb_rules)
		{
		 if (s[j]==1)
		{
			int l=0; 
			bool existe=false;
			while (l< Nb_rules&& existe==false)
			{
					//if (Trans_rules[i].trans[l]==j)
				      if (Trans_rules[i].trans[j]==1)
                        {existe=true;}
			l++;
			}
			if (existe==false){appartient=false;}
		}
		j++;	
		}
		if (appartient==true) {compt++;}
		}
	   compt=compt/N;
	//if(compt!=0)System.out.println("antecedent"+compt);
	  // printf("\n%f",compt);
       return compt;
	}
	/*********************the support of the consequent part*************************/	
float support_consequent(int s[])
{
		float compt=0;
		
		for (int i=0; i<N; i++)
		{
		bool appartient=true;
		int j=1;
		while (j<Nb_rules)
		{
		 if (s[j]==2)
		{
			int l=0; 
			bool existe=false;
			while (l< Nb_rules&& existe==false)
			{
					//if (Trans_rules[i].trans[l]==j)
				      if (Trans_rules[i].trans[j]==1)
                        {existe=true;}
			l++;
			}
			if (existe==false){appartient=false;}
		}
		j++;	
		}
		if (appartient==true) {compt++;}
		}
	   compt=compt/N;
	//if(compt!=0)System.out.println("antecedent"+compt);
	  // printf("\n%f",compt);
       return compt;    
}
/****************************condifence computing**************************/
float confidence(int s[])
{
	float conf=0;
	if (support_antecedent(s)!=0)
	conf=support_rule(s)/support_antecedent(s);
	//nf=support_rule(sol);
	return conf;
}
/*************lift computing**********/
float lift (int s[])
{
 float nb1=confidence(s);
 float nb2=support_consequent(s);
 return(nb1/nb2);  
}
//**************the leverage of the rule*******/
float leverage(int s[])
{
float nb1=support_rule(s);
float nb2=support_antecedent(s);
float nb3=support_consequent(s);
return (nb1-(nb2*nb3));    
}
//**************the conviction of the rule*******/
float conviction(int s[])
{
float nb1=support_consequent(s);
float nb2=confidence(s);
return ((1-nb1)/(1-nb2));    
}
/***********************evaluation of the solution s******/
float fitness1(int s[])
	{
 clock_t t1,t2;
  t1=clock();
	float cost=0; 
	//if (support_rule(sol)<Minsup || confidence(sol)<Minconf){cout=-1;}
	float x=alpha*support_rule(s);
	float y=Beta*confidence(s);
	cost=x+y;
	t2=clock();
	float diff ((float)t2-(float)t1);
	float seconds = diff / CLOCKS_PER_SEC;
	return cost;
	}
/*********************create a solution reference Sref******************************************/
void create_Sref()
{
for (int i=0;i<Nb_rules;i++)
	{ 
		if (rand() % 2==1){Sref.solution[i]=0 ;}
		else {
			if (rand() % 2==0){Sref.solution[i]=0;}
			else {
				Sref.solution[i]=rand() % 3; ;
			}
		}
		
	}
	
	Sref.cost=fitness1(Sref.solution);
 }
/***********************************negihborhood computation************************/
bee neighborhood_computation(bee S)
{
bee s;
int indice=0;
int i=0; 
int neighbor[Nb_rules], best_neighbor[Nb_rules];
float current_cost=0;
float best_cost=0;
		copy(S.solution,best_neighbor);
		   while (i<K)
		   {
			   for (int j=0;j<Nb_rules;j++)
			   {   
			   copy(S.solution,neighbor);	    
			   }
	          if (neighbor[indice]==0) 
	          {
	        		  if (rand()%2==0)
	        		  {neighbor[indice]=1;}
	        		  else{neighbor[indice]=2;}
	          }
	          else {
	          if (neighbor[indice]==1) 
	          {
	        	  if (rand()%2==0)
                         neighbor[indice]=0;

	        	  else {
	        	 neighbor[indice]=2;
	        		  }
	          }
	          else {
	          if (neighbor[indice]==2) 
	          {
	        	  	  if (rand()%2==0)
                  neighbor[indice]=0;

	        	  else {
	                neighbor[indice]=1;
	        		 }
	          }
	          }
	          }
	     indice++;
	     if (indice>=Nb_rules){indice=0;}   
		 
		 current_cost=fitness1(neighbor); //evaluates the solution 
		 if (current_cost>best_cost){copy(neighbor,best_neighbor);best_cost=current_cost;}
		 i++;
		 }
copy(best_neighbor, s.solution);
s.cost=best_cost;
return s;
}
bool verify(int t[], int v[])
{
bool exist=true;
int i=0;
while (i<Nb_rules && exist==true)
{
  if (t[i]!=v[i]){exist=false;}
  i++;    
}
return exist;  
}
/**************Tabu Search Metaheuristic***********/
bee TS(bee S)
{
bee neighbors=S;
for (int i=0;i<IMAX_iter_TS;i++)
{
 T_List[nb_element]=neighbors;//insert neighbor in the tabu list
 nb_element++;
 neighbors=neighborhood_computation(neighbors);
 bool exist=false;
 int j=0;
 while  (j<nb_element && exist==false)
  {
    if (verify(neighbors.solution, T_List[j].solution)==true)
    {exist=true;}            
    j++;               
  }
 
 if (exist==true)// if exist neighbor in list then modifier neighbor
 {
   int x=rand()%Nb_rules;
   if (neighbors.solution[x]==0){neighbors.solution[x]=(rand()%2)+1;}
   else {neighbors.solution[x]=0;}  
 }
 
 if (nb_element==List_size)//taille maximal de list est atteint alor delete premier element
  {
  nb_element--;
  for (int l=0;l<nb_element;l++)
  {T_List[l]=T_List[l+1];}                                  
  }
}    
}
/************************determination of search area********************/
void search_area1(bee s, int iteration)
	{
	 
	    int indice=iteration % Nb_rules;
		int i=0;
		   while (i<K)
		   {
			   for (int j=0;j<Nb_rules;j++)
			   {   
			    T_Dance[i].solution[j]=s.solution[j];	    
			   }	
	          if (T_Dance[i].solution[indice]==0) 
	          {
	        	  if (iteration%4==0)
	        		  {T_Dance[i].solution[indice]=1;}
	        		  else{T_Dance[i].solution[indice]=2;}
	        		  
	        	    //           }    
	          }
			
	          else{
	          if (T_Dance[i].solution[indice]==1) 
	          {	  if (iteration%3==0)
			  {T_Dance[i].solution[indice]=0;}
			  else{T_Dance[i].solution[indice]=2;}
	        		 
	        }
	          else{ 
	          
	        	  if (iteration%2==0)
	    		  {T_Dance[i].solution[indice]=1;}
	    		  else{T_Dance[i].solution[indice]=0;}
	        	 }
	          }
	     indice=indice+flip;
	     if (indice>=Nb_rules){indice=0;}   
		 
		 T_Dance[i].cost=fitness1(T_Dance[i].solution);//evaluer solution  
		 i++;
		   }
}
/**************search 2*********************/
void search_area2(bee s, int iteration)
{
int i=0;
int Nb_sol=0;
bool stop=false;
	  while (i<Nb_rules && stop==false)
	  {
		   for (int j=0;j<Nb_rules;j++)
		   {   
			   T_Dance[Nb_sol].solution[j]=s.solution[j];	       
		   }
		   for (int l=i;l<(i+flip)%Nb_rules;l++)
		   {
		  if ( T_Dance[Nb_sol].solution[l]==0) 
	     {
	   	  if (rand()%2==1)
	   		  { T_Dance[Nb_sol].solution[l]=1;}
	   		  else{T_Dance[Nb_sol].solution[l]=2;}
	   		  
	   	}
	     else {
	     if (T_Dance[Nb_sol].solution[l]==1) 
	     {
	    	 if (rand()%2==1)
	  		  {T_Dance[Nb_sol].solution[l]=0;}
	  		  else{T_Dance[Nb_sol].solution[l]=2;}
	     }
	     else {
	       if (T_Dance[Nb_sol].solution[l]==2) 
	        {
	    	 if (rand()%2==0)
	  		  {T_Dance[Nb_sol].solution[l]=0;}
	  		  else{T_Dance[Nb_sol].solution[l]=1;}
	         }
	         }
	     }
		}
	T_Dance[Nb_sol].cost=fitness1(T_Dance[Nb_sol].solution); //evaluates the solution  
	Nb_sol++; 
	if (Nb_sol==K){stop=true;}   
	}
}   
/********search3***************************/
int W(int t[])
{
int w=0;
	for (int i=0;i<Nb_rules; i++)
	{
	w=w+t[i];
	}
return w;
} 
/*******search 3 continued****************************/
void search_area3(bee s, int iteration)
{
int Nb_sol=0;
	  while (Nb_sol!=K)
	  {
		   for (int j=0;j<Nb_rules;j++)
		   {   
			  T_Dance[Nb_sol].solution[j]=s.solution[j];	 	    
		   }
		   int l=0;
		   int cpt=0;
		   while (cpt<Distance)
		   {
		  if (T_Dance[Nb_sol].solution[l]==0) 
	     {
	   	  if (rand()%2==1)
	   		  {T_Dance[Nb_sol].solution[l]=1; cpt++;}
	   		  else{T_Dance[Nb_sol].solution[l]=2;cpt=cpt+2;}
	   		  
	   	}
	     else {
	     if (T_Dance[Nb_sol].solution[l]==1) 
	     {
	    	 if (rand()%2==0)
	  		  {T_Dance[Nb_sol].solution[l]=0;cpt++;}
	  		  else{T_Dance[Nb_sol].solution[l]=2;cpt++;}
	     }
	     else {
	       if (T_Dance[Nb_sol].solution[l]==2) 
	        {
	    	 if (rand()%2==0)
	  		  {T_Dance[Nb_sol].solution[l]=0;cpt=cpt+2;}
	  		  else{T_Dance[Nb_sol].solution[l]=1;cpt=cpt+1;}
	         }
	         }
	     
		   }
		  l=(l+1)%Nb_rules;
		   } //end the small while
	T_Dance[Nb_sol].cost=fitness1(T_Dance[Nb_sol].solution);//assecees the solution  
	Nb_sol++; 
	  } // end the big while
 }
/********************************best dance********************/
int best_dance()
{
	float max=T_Dance[0].cost;
	int indice=0;
	for (int i=1;i<K;i++)	
	{
     	if (T_Dance[i].cost>max)
	     {     
           max=T_Dance[i].cost;
		   indice=i;
         }
	}
return indice;
}
/*****************************display T_dance************/
void display_bees()
{
FILE *f=NULL;
//f=fopen("C://Users//Dzcomp//Desktop//benchmark//dataset.txt","a");
if (f==NULL) {printf("the file is succefully opened\n");} 
for (int i=0;i<K;i++)
{
    for (int j=0;j<Nb_rules;j++)
    {
    printf ("%d ",T_Dance[i].solution[j]);   
    }
    printf("\n********************\n");
}
fclose(f);
}
/*********final processing methods***************/
void insert_MR(int iteration) // insert MR found into Meta_rules structure
{
for (int i=0;i<K;i++)
{
    for (int j=0;j<Nb_rules;j++)
    {
     Meta_rules[(K*iteration)+i].trans[j]=T_Dance[i].solution[j];  
     Meta_rules[(K*iteration)+i].cost=T_Dance[i].cost;  
    }     
}
}
/**************Display all meta rules found************/
void display_MR()
{
for (int i=0;i<(IMAX*K);i++)
{
    for (int j=0;j<Nb_rules;j++)
    {
    printf ("%d ",Meta_rules[i].trans[j]);   
    }
    printf("\n********************\n");    
}     
}
int  number_relevant_meta_rules()
{
     int nb=0;
for (int i=0; i< (IMAX*K); i++)
{
    float s =confidence(Meta_rules[i].trans);
  if (s==1)
  {
  nb++;
  //printf  ("the confidence is %f\n",s);
  } 
     
}
return nb;
}
/***********************pruning rules using MR and clusters information*******************/
void pruning_rules()
{
     //float f=((alpha*minsup)+(Beta*minconf));
          float f=0;
 for (int i=0;i<(IMAX*K);i++)
 { 
   //if (Meta_rules[i].cost>f)
   //{  
    for (int j=0; j<Nb_rules;j++)
      {
       if (Meta_rules[i].trans[j]==1)
         {
          T_objet[j].cluster=-1;                            
         }
      }   
   //}
 }    
}
/***********************count the number of remaining rules ********/
void returned_rules_evaluation()
{
      /*    float alpha=0.5; float beta =0.5;
     float gamma=0.5;
     */
int nb=0;
float rel=0;
for (int i=0;i<Nb_rules;i++)
{
if (T_objet[i].cluster!=-1)
 {  
// float x=lift(T_objet[i].obj.centre); 
 //float y=leverage(T_objet[i].obj.centre);  
 //float z=conviction(T_objet[i].obj.centre); 
 //rel=rel+x+y+z;                         
 nb++;                          
 }
}
float distinct=0;
/*for (int i=0;i<Nb_rules;i++)
{
    if (T_objet[i].cluster!=-1 )
    {
    for (int j=i+1;j<Nb_rules;j++)
    {
    if (T_objet[j].cluster!=-1 )
     {  
     distinct=distinct+Manhattan(T_objet[i], T_objet[j] );                          
     }
    }
   }
}*/
float f=(rel+distinct)/nb;
printf("%d",nb);     
}
/***********display_transactions********/
void display_tran()
{
for (int i=0;i<M;i++)
 {
  printf ("\n");
  for (int j=0;j<N;j++)
    {
    printf ("%d ", dataset[i].trans[j]);
    }     
}
}
