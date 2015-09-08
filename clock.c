/*
 * nazev: hodiny.c
 * popis: vypise cas slovy
 *
 * autor: Josef Busta
 *
 * preklad: gcc -std=c99 -pedantic -g -Wall -o hodiny hodiny.c
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

void chybneZadanyCas(char ** a);
void tiskDoplnujiciCasoveInformace(int h, int m);

int main (int argc, char** argv){
    int hod, min;
    if(argc<2 || argc>3 || (strcmp(argv[1],"cs")!=0 && strcmp(argv[1],"cz")!=0 && strcmp(argv[1],"en")!=0)){
        printf("usage: %s en|cz [hour:minute]\n", argv[0]);
        return 1;
    }
    if(argv[2]==NULL){//zjisteni aktualniho systemoveho casu
        time_t rawtime = time(NULL);
        struct tm * timeinfo;
        timeinfo = localtime(&rawtime);
        hod = timeinfo->tm_hour;
        min = timeinfo->tm_min;
    }else{
        for (int i = 0; i < strlen(argv[2]); i++){//nepovoleny format casu
            if((!isdigit((int) argv[2][i]) && argv[2][i]!=':') || strchr(argv[2],':')==NULL)chybneZadanyCas(argv);
        }
        hod = atoi(argv[2]);
        min = atoi(strchr(argv[2],':') + 1);
        if(hod > 23 || min > 59)chybneZadanyCas(argv);//nepovoleny format casu
    }
    if(strcmp(argv[1],"en")== 0){//anglictina
        char * en[] = {"midnight","one","two","three","four","five","six","seven","eight","nine","ten","eleven",
            "twelve","thirteen","fourteen","quarter","sixteen","seventeen","eighteen","nineteen","twenty",
            "twenty-one","twenty-two","twenty-three","twenty-four","twenty-five","twenty-six","twenty-seven",
            "twenty-eight","twenty-nine","half"};
        char * en1[] = {"minutes","AM","PM","past","to"};
        int p_hod = (hod>12)?(hod-12):hod;//pozice casu v poli
        int p_min = (min>30)?fabs(min-60):min;//pozice casu v poli
        if(min==1 || min==59)en1[0]="minute";
        if(min>0)printf("%s ",en[p_min]);
        if(min==0)printf("%s ",en[p_hod]);
        if(min%15!=0)printf("%s ",en1[0]);
        if(min<=30 && min>0){//po
            printf("%s %s ",en1[3],en[p_hod]);
        }
        if(min<=59 && min>30){//do
            printf("%s ",en1[4]);
            p_hod = (p_hod>=12)?0:p_hod;
            if(hod>=23){
                printf("%s ",en[0]);
            }else{
                printf("%s ",en[p_hod+1]);
            }
        }
        int cislo=hod*100+min;
        if(cislo>30 && cislo<1200)printf("%s ",en1[1]);//AM
        if(cislo>=1200 && cislo<=2330)printf("%s ",en1[2]);//PM
    }else{//cestina
        char * cs1[] = {"jedna","dve","tri","ctyri","pet","sest","sedm","osm","devet","deset","jedenact","dvanact"};
        char * cs2[] = {"jedne","druhe","treti","ctvrte","pate","seste","sedme","osme",
            "devate","desate","jedenacte","dvanacte"};
        char * cs3[] = {"ctvrt na","pul","tri ctvrte na","hodin","minut","za","a"};
        int p_hod = (hod>=12)?(hod-12):hod;//pozice casu v poli
        int p_min = ((min%15)<8)?(min%15)-1:15-(min%15)-1;//pozice casu v poli
        if((p_hod>0 && p_hod<4) || (p_hod==4 && min<8))cs3[3]="hodiny";
        if(p_hod==0 || (p_hod==1 && min<8))cs3[3]="hodina";
        if(min%15==1)cs3[4]="minuta";
        if((min%15>1 && min%15<5) || (min%15>10 && min%15<14))cs3[4]="minuty";
        if((hod==0 || hod==12) && min<8)tiskDoplnujiciCasoveInformace(hod,min);
        if(min%15>7){//do
            printf("%s ",cs3[5]);
            if(min%15==14){
                printf("minutu ");
            }else{
                printf("%s %s ",cs1[p_min],cs3[4]);
            }
        }
        if(!(((hod==0 || hod==12) && min<8) || ((hod==23 || hod==11) && min>52))){//hodiny
            if(min>7 && min<23)printf("%s ",cs3[0]);
            if(min>37 && min<53)printf("%s ",cs3[2]);
            if(min>22 && min<38){
                printf("%s %s ",cs3[1],cs2[p_hod]);
            }else if(min<8){
                printf("%s ",cs1[p_hod-1]);
            }else if(p_hod==0 && min<53){
                printf("jednu ");
            }else{
                printf("%s ",cs1[p_hod]);
            }
            if(min>52 || min<8)printf("%s ",cs3[3]);
        }
        if(min%15<8 && min%15!=0){//po
            printf("%s %s %s ",cs3[6],cs1[p_min],cs3[4]);
        }
        if(!((hod==0 || hod==12) && min<8))tiskDoplnujiciCasoveInformace(hod,min);
    }
    printf("\n");
    return 0;
}

void tiskDoplnujiciCasoveInformace(int h, int m){
    char * cs0[] = {"pulnoc","v noci","rano","dopoledne","poledne","odpoledne","vecer"};
    int cislo = h*100+m;
    if(cislo<8)printf("%s ",cs0[0]);
    else if(cislo<408)printf("%s ",cs0[1]);
    else if(cislo<908)printf("%s ",cs0[2]);
    else if(cislo<1153)printf("%s ",cs0[3]);
    else if(cislo<1208)printf("%s ",cs0[4]);
    else if(cislo<1708)printf("%s ",cs0[5]);
    else if(cislo<2108)printf("%s ",cs0[6]);
    else if(cislo<2353)printf("%s ",cs0[1]);
    else printf("%s ",cs0[0]);
}

void chybneZadanyCas(char ** a){
    if(strcmp(a[1],"en")==0)printf("%s: incorect format of time (HH:MM)\n", a[0]);
    if(strcmp(a[1],"cs")==0 || strcmp(a[1],"cz")==0) printf("%s: spatne zadany cas (HH:MM)\n", a[0]);
    printf("\nusage: %s en|cz [hour:minute]\n", a[0]);
    exit(1);
}

