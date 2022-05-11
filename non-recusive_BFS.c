#include <stdio.h>
#include <dirent.h> 
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <error.h>
#include <sys/stat.h>  

#define MAX_FILE_NAME_LEN 256

typedef struct foldernode_t {
  char *path;                   //point to foldername or filename path
  struct foldernode_t *next;    //next directory
} foldernode;

static void travel_files(char *path)
{
    DIR *d;
    struct dirent *dp;
    char foldername[MAX_FILE_NAME_LEN] = {0};
    char folderpath[MAX_FILE_NAME_LEN] = {0};

    foldernode *folderstart;
    folderstart = calloc(1, sizeof(foldernode));
    folderstart->path = calloc(1, MAX_FILE_NAME_LEN + 1); 
    strncpy(folderstart->path, path, MAX_FILE_NAME_LEN);
    folderstart->next = NULL;

    foldernode *folderfirst = folderstart; 
    foldernode *folderlast = folderstart; 
    foldernode *oldfirst = NULL;

    while(folderfirst != NULL) {
        printf("\nd=%s\n", folderfirst->path);
        if ((d = opendir(folderfirst->path)) != NULL) {
            while ((dp = readdir(d)) != NULL) {
                if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
                    continue;  
                } else if (dp->d_type == DT_REG) {
                    printf("%s\n", dp->d_name);
                } else if (dp->d_type == DT_DIR) {
                    bzero(foldername, sizeof(foldername));
                    bzero(folderpath, sizeof(folderpath));
                    strncpy(foldername, dp->d_name, sizeof(foldername));
                    snprintf(folderpath, sizeof(folderpath), "%s/%s", folderfirst->path , foldername);
                    printf("%s\n", folderpath);
                    
                    foldernode *foldernew;
                    foldernew = calloc(1, sizeof(foldernode));
                    foldernew->path = calloc(1, MAX_FILE_NAME_LEN + 1); 
                    strncpy(foldernew->path, folderpath, MAX_FILE_NAME_LEN);
                    foldernew->next = ((void *)0);
                    folderlast->next = foldernew;
                    folderlast = foldernew;
                }
            }
        } else {
            printf("opendir[%s] error: %m\n", folderfirst->path);
            return;
        }
        oldfirst = folderfirst;
        folderfirst = folderfirst->next; // change folderfirst pointer to next foldernode
        if (oldfirst) {
            if (oldfirst->path) {
                free(oldfirst->path);
                oldfirst->path = NULL;
            }
            free(oldfirst);
            oldfirst = NULL;
        }
        closedir(d);
    }
}

int main(int argc, char **argv)
{
    char *path = NULL;
    if (argc != 2) {
        //type ./b.out +specific directory to traverse the directory you typed
        //or use the default directory ../
        printf("the directory to travese is: %s \n\n", argv[0]);
        path = "../";
    } else {
        path = argv[1];
        printf("the directory to travese is: %s \n\n", argv[1]);
    }
    travel_files(path);

    return 0;
}

