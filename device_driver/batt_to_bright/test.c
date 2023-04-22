#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>

int main(int argc, char *argv[]){
	int fp = open("/dev/brightness_control",O_RDWR);
	write(fp,"50",strlen("50"));
	printf("done");
return 0;
}
