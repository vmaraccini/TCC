//
//  FileHelper.h
//  Lumiar-Final
//
//  Created by Denis on 21/06/2016.
//
//

#ifndef FileHelper_h
#define FileHelper_h

#include "SizeConstants.h"
#include "../Comum.h"

#include <stdio.h>

int composePath(char *oldpath,char *path,char *newpath);
int transferFile(char *path,int output_fd, char* msgbuffer);
char* getCurrentDir(char *path, int pathsize);

#endif /* FileHelper_h */
