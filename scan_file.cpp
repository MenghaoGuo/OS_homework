#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int scan_file(char *s) {
	DIR *dir;
	struct dirent *ptr;
	dir = opendir(s);
	char path[100];
	if ((dir=opendir(s)) == NULL) {
        perror("没有这个文件夹！");
        exit(1);
    }
	while ((ptr = readdir(dir)) != NULL) {
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0 || ptr->d_name[0] == '.')
			continue;
		else if (ptr->d_type == 8) ///file
			cout << "name" << s << '/' << ptr->d_name << endl;
		else if (ptr->d_type == 10) ///link file
			cout << "name" << s << '/' << ptr->d_name << endl;
		else if (ptr->d_type == 4) {///dir
			memset(path, 0, sizeof(path));
			strcpy(path, s);
			strcat(path, "/");
			strcat(path, ptr->d_name);
			scan_file (path);
		}
	}
	closedir(dir);
}

int main() {
	scan_file("./");
}
