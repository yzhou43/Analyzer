  /*****************************************************
This is the program for the extra credit. It contains the
Lexical Analyzer, Syntax Analyzer, and Semantic Analyzer 
for my programming language.
More detailed information shown in the Specification Document 
*****************************************************/

#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
#define KEY_NUM 34
//Declaration
void emit(char * result,char *ag1,char *op,char *ag2);
char * newtemp();
int lrparser();
int stateList();
int statement();
char * expression();
char * term();
char * factor();
struct 
{
	char result[8];
	char ag1[8];
	char op[8];
	char ag2[8];
}quad[20];
char prog[80],token[32],str[100];
char ch;
int syn,p,index,const_num,kk,row,kt,outc;
FILE *fp_in,*fp_out;
char* keyWord[KEY_NUM] = {"auto","break","case","char","const",
						"continue","default","do","double","else",
						"enum","extern","float","for","goto",
						"if","int","long","register","return",
						"short","signed","sizeof","static","struct",
						"switch","typedef","union","unsigned","void",
						"volatile","while","begin","end"};
						
char *constant[100];
int constValue[100];

/*****************************************************
File Operation 
*****************************************************/				
/*Open the file*/
void fileOpen(char fileInput[],char fileOuput[])
{
	if((fp_in=fopen(fileInput,"r"))==NULL)
	{
		printf("Fail to open the file£¡\n");
		return;
	}
	if((fp_out=fopen(fileOuput,"w"))==NULL)
	{
		printf("Fail to open the output file£¡\n");
		return;
	}
}
/*Close the file*/
void fileClose()
{
	if(fp_in!=NULL)
		fclose(fp_in);  
	if(fp_out!=NULL)
		fclose(fp_out);
}
/*File output*/
void output(int a,char tk[])
{
	printf("(%d,%s)\n",a,tk);
	fprintf(fp_out,"(%d,%s)\n",a,tk);
}
/*****************************************************
Lexical Analyzer
*****************************************************/

/*Error warning*/
void error(char msg[])
{
	printf("%s\n",msg);
	return;
}

/*Get a char*/
void getch()
{
	ch=fgetc(fp_in);
} 

/*Get the blank*/
void getbc()
{
	while(ch==' ' || ch=='\r' || ch=='\t' || ch=='\n')
	{
		if(ch=='\n')
		{
			row++;	
		}
		getch();
	}
}

void geten()
{
	while(ch=='\r')
		getch();
}

/*Get the tab*/
void gettab()
{
	while(ch=='\t')
		getch();
}

/*Get the next line*/
void getline()
{
	while(ch=='\n')
		getch();
}

/*Change to Next line*/
void nextline()
{
	while(ch!='\n')
		getch();
}

/*String concatenates*/
void concat()
{
	int len;
	len=strlen(token);
	token[len]=ch;
	token[len+1]='\0';
 
}

/*Check if letters*/
bool letter(char c)
{
	if((c>='a'&&c<='z')||(c>='A'&&c<='Z'))
		return true;
	if(c=='_')
		return true; 
	return false;
}

/*Check if digits*/
bool digit(char c)
{
	if(c>='0'&&c<='9')
		return true;
	return false;
}

/*Check if keywords*/
int reserve()
{
	int i;
	for(i=0;i<KEY_NUM;i++)
	{
		if(strcmp(token,keyWord[i])==0)
			return (i+100);
	}
	return (10);
}

void retract()
{
	fseek(fp_in,-1,SEEK_CUR);
}


/*Scanner from left to right*/
void scaner()
{
	//printf("%d\t%s\n",syn,token);
	memset(token,0,sizeof(token));
	memset(str,0,sizeof(str));
	getch();
	getbc();
	if(letter(ch))
	{
		while(letter(ch)||digit(ch))
		{
			concat();
			getch();
		}
		retract();
		syn=reserve();
		//output(syn,token);
	}
	else if(digit(ch))
	{
		while(digit(ch))
		{
			concat();
			getch();
			if(ch=='.')
			{
				concat();
				getch();
			}
		}
		retract();
		syn=11;
		//output(syn,token);
	} 
    else
	{
		switch(ch)
		{
			case'+':syn=12;
					concat();
					getch();
					if(ch=='+')
					{
						concat();
						syn=13;
					}
					else
					{
						retract();
					}
					break;
			case'-':syn=14;
					concat();
					getch();
					if(ch=='-')
					{
						concat();
						syn=15;
					}
					else
					{
						retract();
					}
					break;
			case'*':syn=16;concat();break;
			case'/':concat();
					getch();
					if(ch=='/')
					{
						getch();
						nextline();
						syn=-2;
					}
					else if(ch=='*')
					{
						getch();
						bool finish=false;
						while(!finish)
						{
							if(feof(fp_in))
								break;
							while(ch!='*')
							{
								getch();
							}
							getch();
							if(ch=='/')
							{
								syn=-3;
								finish=true;
								break;
							}
							getch();
						}
					}
					else
					{
						syn=17;
						retract();
					}
					break;
			case':':concat();
					getch();
					if(ch=='=')
					{
						concat();
						syn=19;
					}
					else
					{
						syn=18;
						retract();
					}
					break;
			case'<':concat();
					getch();
					if(ch=='>')
					{
						syn=21;
						concat();
					}
					else if(ch=='=')
					{
						syn=22;
						concat();
					}
					else
					{
						syn=20;
						retract();
					}
					break;
			case'>':concat();
					getch();
					if(ch=='=')
					{
						concat();
						syn=24;
					}
					else
					{
						syn=23;
						retract();
					}
					break;
			case'=':syn=25;
					concat();
					getch();
					if(ch=='=')
					{
						concat();
						syn=44;
					}
					else
					{
						retract();
					}
					break;
			case';':syn=26;concat();break;
			case',':syn=27;concat();break;
			case'(':syn=28;concat();break;
			case')':syn=29;concat();break;
			case'{':syn=30;concat();break;
			case'}':syn=31;concat();break;
			case'[':syn=32;concat();break;
			case']':syn=33;concat();break;
			case'\'':syn=34;concat();break;
			case'\"':syn=35;concat();break;
			case'\\':syn=36;concat();break;
			case'&':syn=37;
					concat();
					getch();
					if(ch=='&')
					{
						concat();
						syn=38;
					}
					else
					{
						retract();
					}
					break;
			case'|':syn=39;
					concat();
					getch();
					if(ch=='|')
					{
						concat();
						syn=40;
					}
					else
					{
						retract();
					}
					break;
			case'!':syn=41;
					concat();
					getch();
					if(ch=='=')
					{
						concat();
						syn=45;
					}
					else
					{
						retract();
					}
					break;
			case'#':syn=42;concat();break;
			case'.':syn=43;concat();break;
			case' ':syn=-4;break;
			case'\t':syn=-4;break;
			case'\n':syn=-4;break;
			case'\r':syn=-4;break;
			case EOF:syn=0;break;
			default:syn=-1;break;
		}
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
}
/*****************************************************
Syntax Analyzer and Semantic Analyzer
*****************************************************/

void emit(char * result,char *ag1,char *op,char *ag2)
{
	strcpy(quad[outc].result,result);
	strcpy(quad[outc].ag1,ag1);
	strcpy(quad[outc].op,op);
	strcpy(quad[outc].ag2,ag2);
	outc++;
	printf("%s=%s%c%s\n",result,ag1,op[0],ag2);
}

//Save the results
void save()
{
	for(int i=0;i<outc;i++)
	{
		fprintf(fp_out,"(%d)%s=%s%c%s\n",i,quad[i].result,quad[i].ag1,quad[i].op[0],quad[i].ag2);
	}
} 

//Create a temporary variable
char * newtemp()
{
    char * p;
    char m[8];
    p=(char *)malloc(8);
    kt++;
    itoa(kt,m,10);
    strcpy(p+1,m);
    p[0]='t';
    return(p);
}
//The LR parser
int lrparser()
{
	int schain=0;
	kk=0;
	if(syn==132)//begin
	{
		scaner();
		schain=stateList();
		if(syn==133)//end
		{	
			scaner();
			if((syn==42)&&(kk==0))
			{
				printf("success!\n");
			}
			else
			{
				printf("%d\tend error!\n",row);
				kk=1;
			}
		}
	}
	else
	{
		printf("%d\tno begin error!\n",row);
		kk=1;
	}
	return(schain);
}

//Analyze the statement
int stateList()
{
	int schain=0;
	schain=statement();
	while(syn==26)
	{
		scaner();
		schain=statement();
	}
	return(schain);
}

//Analyze the assignment
int statement()
{
	char tt[8],eplace[8];
	int schain=0;
	if(syn==10)//letter
	{
		strcpy(tt,token);
		scaner();
		if(syn==25)//:=
		{
			scaner();
			strcpy(eplace,expression());
			emit(tt,eplace,"","");
			schain=0;
		}
		else
		{
			printf("expression error!\n");
			kk=1;
		}
	}
	return(schain);
}

//Analyze the expression
char * expression()
{
	char *tp,*ep2,*eplace,*tt;
	tp=(char*)malloc(12);
	ep2=(char*)malloc(12);
	eplace=(char*)malloc(12);
	tt=(char*)malloc(12);
	strcpy(eplace,term());
	while(syn==12 || syn==14)//+ or -
	{
		if(syn==12)
		{
			tt[0]='+';
		}
		if(syn==14)
		{
			tt[0]='-';
		}
		scaner();
		strcpy(ep2,term());
		strcpy(tp,newtemp());
		emit(tp,eplace,tt,ep2);
		strcpy(eplace,tp);
	}
	return(eplace);
}

//Analyze the term
char * term()
{
	char *tp,*ep2,*tplace,*tt;
	tp=(char*)malloc(12);
	ep2=(char*)malloc(12);
	tplace=(char*)malloc(12);
	tt=(char*)malloc(12);
	strcpy(tplace,factor());
	while(syn==16 || syn==17)//* or /
	{
		if(syn==16)
		{
			tt[0]='*';
		}
		else
		{
			tt[0]='/';
		}
		scaner();
		strcpy(ep2,factor());
		strcpy(tp,newtemp());
		emit(tp,tplace,tt,ep2);
		strcpy(tplace,tp);
	}
	return(tplace);
}

//Analyze the factor
char * factor()
{
	char * fplace;
	fplace=(char *)malloc(12);
	strcpy(fplace," ");
	if(syn==10)//letter
	{
		strcpy(fplace,token);
		scaner();
	}
	else if(syn==11)//digit
	{
		strcpy(fplace,token);
		scaner();
	}
	else if(syn==28)
	{
		scaner();
		fplace=expression();
		if(syn==29)
		{
			scaner();
		}
		else
		{
			printf("no ) error!\n");
			kk=1;
		}
	}
	else
	{
		printf("no ( error!\n");
		kk=1;
	}
	return(fplace);
}

//The main funciton
int main()
{
    fileOpen("test.txt","out.txt");
	kk=0;
	row=1;
	scaner();
	lrparser();
	save();
	fileClose();
	getchar();
	return 0;
}
