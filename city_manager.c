#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include "utils/inputValidation.h"
#include "utils/fileFunctions.h"
#include "utils/commands.h"

int main(int argc, char **argv){
    if (argc < 4){
        fprintf(stderr, "Not enough command line arguments.\n");
        fprintf(stderr, "2nd argument: --role\n3rd argument: Role Name\n4th argument: Command\n");
        exit(-1);
    }

    // printf("Current Role = %d\n", getRole(argv));
    displayRole(argv);
    printf("\n");
    selectCommand(argv);

    return 0;
}
