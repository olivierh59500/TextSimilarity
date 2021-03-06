//
//  sim.c
//  Document_Similarity
//
//  Created by He11o_Liu on 2017/5/6.
//  Copyright © 2017年 He11o_Liu. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <unistd.h>

#define Max_file        20
#define Max_filename    100
#define Max_word        20
#define N_word          1
#define d 65536     //d-Dimensions
#define Python_trans "py_tmp_trans"
//#define debug

void get_doc_profile(FILE *fp,unsigned int *profile);
double get_sim_by_profile(unsigned int *profile1,unsigned int *profile2);
int naive_hash(char *str);



int main(int argc, const char * argv[]) {
    FILE *fp1;
    FILE *fp2;
    
    int i = 0, j = 0;
    unsigned int profile1[d];
    unsigned int profile2[d];
    unsigned int profile[Max_file][d];
    double similarity = 0;
    DIR *dir;
    struct dirent *ptr;
    int file_count = 0;
    double result[Max_file][Max_file];
    char filename[Max_file][Max_filename];
    char tmp_filename[100];
    char tmp_word[Max_word];
    
    if(argc == 2){
        printf("Open path: %s\n",argv[1]);
        dir = opendir(argv[1]);
        if(dir == NULL) {printf("Cannot open path!\n"); exit(0);}
        while((ptr = readdir(dir)) != NULL){
            //check DT_REG and not start with '.'
            //check file is not Microsoft Word
            if(ptr->d_type==DT_REG && ptr->d_name[0] != '.' &&
               strstr(ptr->d_name,".doc") == 0){
                if(strstr(ptr->d_name,Python_trans)){
                    strncpy(filename[file_count],ptr->d_name,(int)(strstr(ptr->d_name,Python_trans)-ptr->d_name)/sizeof(char));
                    strcat(filename[file_count],"doc");
                }
                else{
                    strcpy(filename[file_count],ptr->d_name);
                }
                strcpy(tmp_filename,argv[1]);
                //检查末尾是否为/
                for(i = 0; tmp_filename[i+1]!='\0';i++);
                if(tmp_filename[i]!='/') strcat(tmp_filename,"/");
                
                strcat(tmp_filename,ptr->d_name);
                printf("Read file %s.....", tmp_filename);
                fp1 = fopen(tmp_filename,"r");
                if(fp1 == NULL) {printf("Cannot open %s\n",ptr->d_name);continue;}
                
                for(i = 0;i < d;i++) profile[file_count][i] = 0;
                for(i = 0; i< N_word; i++){
                    fseek(fp1, 0, SEEK_SET);
                    for(j = 0; j < i; j++){
                        fscanf(fp1,"%10s",tmp_word);
                    }
                    get_doc_profile(fp1, profile[file_count]);
                }
                
                printf("Generate profile success!\n");
                if(file_count == Max_file){printf("Too many file in this path! Max = 100\n"); exit(0);}
                fclose(fp1);
                file_count ++ ;
            }
        }
        closedir(dir);
        for(i = 0; i< file_count;i++){
            for(j = i; j<file_count;j++){
                result[i][j] = get_sim_by_profile(profile[i],profile[j]);
                result[j][i] = result[i][j];
            }
        }
        
        
        printf("%15s","");
        for(i = 0; i< file_count;i++){
            printf("%15s",filename[i]);
        }
        printf("\n");
        for(i = 0; i< file_count;i++){
            printf("%15s",filename[i]);
            for(j = 0; j<file_count;j++){
                if(result[i][j]>0.3){
                    printf("\033[31m%15lf\033[0m",result[i][j]);
                }
                else{
                    printf("%15lf",result[i][j]);
                }
            }
            printf("\n");
        }
    }
    else if(argc == 3){
        fp1 = fopen(argv[1],"r");
        fp2 = fopen(argv[2],"r");
        if(fp1 == NULL||fp2 == NULL) {printf("Cannot open file!\n");exit(0);}
        //clear profile
        for(i = 0;i < d;i++) profile1[i] = 0;
        for(i = 0;i < d;i++) profile2[i] = 0;
        for(i = 0; i< N_word; i++){
            fseek(fp1, 0, SEEK_SET);
            fseek(fp2, 0, SEEK_SET);
            for(j = 0; j < i; j++){
                fscanf(fp1,"%10s",tmp_word);
                fscanf(fp2,"%10s",tmp_word);
            }
            get_doc_profile(fp1, profile1);
            get_doc_profile(fp2, profile2);
        }
        
        if(strstr(argv[1],Python_trans)){
            strncpy(filename[0],argv[1],(int)(strstr(argv[1],Python_trans)-argv[1])/sizeof(char));
            strcat(filename[0],"doc");
        }
        else{
            strcpy(filename[0],argv[1]);
        }
        
        if(strstr(argv[2],Python_trans)){
            strncpy(filename[1],argv[2],(int)(strstr(argv[2],Python_trans)-argv[2])/sizeof(char));
            strcat(filename[1],"doc");
        }
        else{
            strcpy(filename[1],argv[2]);
        }
        similarity = get_sim_by_profile(profile1, profile2);
        printf("Similarity between %s and %s is %lf\n",filename[0],filename[1],similarity);
        fclose(fp1);
        fclose(fp2);
    }
    else {
        printf("Usage:./sim [path] or ./sim [file1] [file2]\n");
    }
    return 0;
}


void get_doc_profile(FILE *fp,unsigned int *profile){
    int i = 0;
    char word[Max_word];
    char word_set[Max_word*N_word];
    unsigned int hash = 0;
    while(!feof(fp)){
        word_set[0] = '\0';
        for(i = 0; i<N_word;i++){
            fscanf(fp,"%10s",word);
            if(feof(fp)) break;
            strcat(word_set,word);
        }
        hash = naive_hash(word_set);
        profile[hash%d] += 1;
        if(profile[hash%d]>=0xFFFFFFFF) {printf("Overflow");exit(0);}
#ifdef debug
        printf("%s %d\n",word_set,hash%d);
#endif
    }
}

int naive_hash(char *str){
    int hash = 0,i = 0;
    for(i = 0; str[i] != '\0';i++)
        hash = str[i] + 31*hash;
    return hash;
}


double get_sim_by_profile(unsigned int *profile1,unsigned int *profile2){
    int i = 0;
    double mul,sum_A,sum_B;
    double similarity = 0;
    mul = 0;
    sum_A = 0;
    sum_B = 0;
    for(i = 0; i < d;i++){
        mul += profile1[i]*profile2[i];
        sum_A += profile1[i]*profile1[i];
        sum_B += profile2[i]*profile2[i];
    }
#ifdef debug
    printf("mul = %lf sum_A = %lf sum_B = %lf\n",mul,sum_A,sum_B );
#endif
    if(sum_A == 0 || sum_B == 0)
        similarity = 0;
    else
        similarity = mul/(sqrt(sum_A)*sqrt(sum_B));
    return similarity;
}



