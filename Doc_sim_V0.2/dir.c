#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>   
#include <unistd.h>

int main(){
    DIR *dir;
    struct dirent *ptr;
    dir = opendir("./");
    if(dir == NULL) {printf("Cannot open path!\n"); exit(0);}
    while((ptr = readdir(dir)) != NULL){
        if(ptr->d_type==DT_REG && ptr->d_name[0] != '.'){
            printf("d_name: %s\n", ptr->d_name);
            printf("d_type: %d\n", ptr->d_type);
        }
    }
    closedir(dir);
    return 0;
}
