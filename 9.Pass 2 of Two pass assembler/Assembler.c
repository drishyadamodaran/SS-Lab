#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fpr=NULL,*fpw=NULL,*fp1=NULL,*fp2=NULL;

char addr(char ch)
{
    int n;

    n=0;
    n=ch-'0';
    n+=8;

    if(n==8)
        return '8';
    else if(n==9)
        return '9';
    else if(n==10)
        return 'A';
    else if(n==11)
        return 'B';
    else if(n==12)
        return 'C';
    else if(n==13)
        return 'D';
    else if(n==14)
        return 'E';
    else if(n==15)
        return 'F';
}

int main()
{
    char label[10],mnemonic[10] ,operand[10],program_name[10],sym_label[30][10],sym_address[30][10],
    opcodes[1000][3],opcode[10],str[80],st[10],ch,c,chr[10],text[80][80];

    int address=0,line=0,length,x=0,first_address=0,last_address=0,pa=0,obj_code=0,i=0,j=0,k=0,pos1=0,
    pos2=0,flag1=0,flag2=0,p1=0,p2=0,l=0,z=0,n=0,count=0,l1=0,l2=0;

    char op_mnemonic[33][10]={"LDA","STA","JSUB","MUL","STL","DIV","ADD","STCH","TD",
    "JEQ","LDCH","WD","SUB","FIXR","TIX","JLT","COMP","CLR","LOADB","J","RD","COMP","STX","LOADT","LDL","LDX","JGT","OR","AND","RSUB","STSW","STX","TIX"};

    char op_opcode[33][10]={"00","0C","48","20","14","24","18","54","E0","30","50","DC",
    "1C","C8","2C","38","28","C4","78","3C","D8","28","20","84","08","04","3","44","40","4C","E8","10","2C"};


      fpr=fopen("new_input.txt","r");
    fpw=fopen("addressed_file.txt","w");

    fscanf(fpr,"%s%s%X",program_name,mnemonic,&address);  
    line++;
    fprintf(fpw,"%d\t\t%s\t%s\t%.4X\n",line,program_name,mnemonic,address); 
    while(strcmp(mnemonic,"END")!=0)
    {
        if(strcmp(label,".")!=0)        
        {
            fscanf(fpr,"%s%s%s",label,mnemonic,operand);
            line++;

            fprintf(fpw,"%d\t%.4X\t%s\t%s\t%s\n",line,address,label,mnemonic,operand);  

            

            if(strcmp(mnemonic,"BYTE")==0)
            {
                length=0;
                length=strlen(operand);
                length-=3;          

                if(operand[0]=='C')
                        address+=length;
                else
                {
                    length=(length/2);
                    address+=length;
                }
            }
            else if(strcmp(mnemonic,"WORD")==0)
                address+=3;
            else if(strcmp(mnemonic,"RESW")==0)
            {
                x=atoi(operand);   
                address+=(x*3);     
            }
            else if(strcmp(mnemonic,"RESB")==0)
            {
                x=atoi(operand);    
                address+=x;
            }
            else
                address+=3;
        }
        else
            fprintf(fpw,"\n");
    }
    fclose(fpr);
    fclose(fpw);

   

    fpr=fopen("addressed_file.txt","r");
    fpw=fopen("symtab.txt","w");

    fscanf(fpr,"%d%s%s%s",&line,program_name,mnemonic,operand);

    while(strcmp(mnemonic,"END")!=0)
    {
        fscanf(fpr,"%d%X%s%s%s",&line,&address,label,mnemonic,operand);

        if(line==2)
            first_address=address;     

        if(strcmp(label,"--")!=0)
        {
            fprintf(fpw,"%s  %X\n",label,address);
            j++;
        }
    }
    last_address=address;      

    fclose(fpr);
    fclose(fpw);
    
    fp1=fopen("symtab.txt","r");
    i=0;

    while(j--)
    {
        fscanf(fp1,"%s%s",sym_label[i],sym_address[i]);
        i++;
    }

    fclose(fp1);

    fpr=fopen("addressed_file.txt","r");
    fpw=fopen("intermediate.txt","w");

    fscanf(fpr,"%d%s%s%s",&line,program_name,mnemonic,operand);
    fprintf(fpw,"%d\t\t%s\t%s\t%s\t\t--\n",line,program_name,mnemonic,operand);

    while(strcmp(mnemonic,"END")!=0)
    {
        fscanf(fpr,"%d%X%s%s%s",&line,&address,label,mnemonic,operand);
        l=strlen(operand);

        if(strcmp(mnemonic,"WORD")==0)
            fprintf(fpw,"%d\t%.4X\t%s\t%s\t%s\t\t%.6X\n",line,address,label,mnemonic,operand,atoi(operand));

        else if(strcmp(mnemonic,"BYTE")==0)
        {
            l=strlen(operand);
            k=0;

            for(i=2;i<l-1;i++)
                opcode[k++]=operand[i];
            opcode[k]='\0';

            if(operand[0]=='C')
                fprintf(fpw,"%d\t%.4X\t%s\t%s\t%s\t\t454F46\n",line,address,label,mnemonic,operand);

            if(operand[0]=='X')
                fprintf(fpw,"%d\t%.4X\t%s\t%s\t%s\t\t%s\n",line,address,label,mnemonic,operand,opcode);
        }

        else if(strcmp(mnemonic,"RESW")==0 || strcmp(mnemonic,"RESB")==0 || strcmp(mnemonic,"START")==0 || strcmp(mnemonic,"END")==0)
            fprintf(fpw,"%d\t%.4X\t%s\t%s\t%s\t\t--\n",line,address,label,mnemonic,operand);

        else if(strcmp(mnemonic,"SUBR")==0)
            fprintf(fpw,"%d\t%.4X\t%s\t%s\t%s\t\t4C0000\n",line,address,label,mnemonic,operand);

        else if(operand[l-1]=='X' && operand[l-2]==',')
        {
            strcpy(st,operand);
            st[l-2]='\0';

            for(j=0;j<17;j++)
            {
                if(strcmp(st,sym_label[j])==0)
                {
                    pos2=j;
                    break;
                }
            }

            strcpy(str,sym_address[pos2]);
            ch=str[0];
            str[0]=addr(ch);

            fprintf(fpw,"%d\t%.4X\t%s\t%s\t%s\t%s%s\n",line,address,label,mnemonic,operand,op_opcode[pos1],str);
        }

        else
        {
            pos1=pos2=0;
            for(i=0;i<26;i++)
            {
                if(strcmp(mnemonic,op_mnemonic[i])==0)
                {
                    pos1=i;
                    break;
                }
            }
            for(j=0;j<17;j++)
            {
                if(strcmp(operand,sym_label[j])==0)
                {
                    pos2=j;
                    break;
                }
            }
            fprintf(fpw,"%d\t%.4X\t%s\t%s\t%s\t\t%s%s\n",line,address,label,mnemonic,operand,op_opcode[pos1],sym_address[pos2]);
        }
    }

    fclose(fpr);
    fclose(fpw);

    fpr=fopen("intermediate.txt","r");
    fscanf(fpr,"%d%s%s%s%s",&line,program_name,mnemonic,operand,opcode);
    k=count=0;
    fpw=fopen("object_file.txt","w");

    chr[0]='^';
    chr[1]=' ';
    chr[2]='\0';
    i=k=0;

    fprintf(fpw,"H ^ %s ^ %.6X ^ %.6X\n\n",program_name,first_address,last_address-first_address);

    while(strcmp(mnemonic,"END")!=0)
    {
        fscanf(fpr,"%d%X%s%s%s%s",&line,&address,label,mnemonic,operand,opcode);

        if(i<30 && strcmp(opcode,"--")!=0)
        {
            if(i==28 || i==29)
            {
                fprintf(fpw,"T ^ %.6X ^ %.2X %s\n\n",address-i,i,text[k]);
                k++;
                i=0;
                strcat(text[k],chr);
                i+=(strlen(opcode))/2;
                strcat(text[k],opcode);
            }
            else
            {
                strcat(text[k],chr);
                i+=(strlen(opcode))/2;
                strcat(text[k],opcode);
            }
        }
        if(i>=30 || strcmp(opcode,"--")==0)
        {
            if(strlen(text[k])!=0)
            {
                if(i<30)
                    fprintf(fpw,"T ^ %.6X ^ %.2X %s\n\n",address-i,i,text[k]);
                else
                    fprintf(fpw,"T ^ %.6X ^ %.2X %s\n\n",address-i+3,i,text[k]);
            }
            k++;
            i=0;
        }
    }
    fprintf(fpw,"E ^ %.6X",first_address);

    fclose(fpw);
    fclose(fpr);


    fpr=fopen("intermediate.txt","r");

    fscanf(fpr,"%d%s%s%s%s",&line,program_name,mnemonic,operand,opcode);
    k=count=i=0;

    while(strcmp(mnemonic,"END")!=0)
    {
        fscanf(fpr,"%d%X%s%s%s%s",&line,&address,label,mnemonic,operand,opcode);

        if(strcmp(opcode,"--")!=0 && strlen(opcode)==6)
        {
            if(address-pa>500)
            {
                ch=(pa%16)+48;
                opcodes[k][0]='+';
                opcodes[k][1]='+';
                opcodes[k][2]='\0';
                k++;
                opcodes[k][0]='0';
                opcodes[k][1]=ch;
                opcodes[k][2]='\0';
                k++;
            }
            for(i=0;i<6;)
            {
                opcodes[k][0]=opcode[i];i++;
                opcodes[k][1]=opcode[i];
                opcodes[k][2]='\0';i++;
                k++;
            }
        }
        if(strcmp(opcode,"--")==0)
        {}
        if(strcmp(opcode,"--")!=0 && strlen(opcode)==2)
        {
            strcpy(opcodes[k],opcode);
            k++;
        }
        pa=address;
    }
    fclose(fpr);


}
