#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>


void ScanforModules(int file,uint8_t *adresses)
{
    	uint8_t i;
    	uint8_t j=0;
    	uint8_t module_counter=0;
    	uint8_t data[1] = {0};
    	
    	uint8_t first = 0x10, last = 0x30;
    	for (i = first; i < last; i++)
    	{
    		
    		printf("ADRESS A CHECKAR:%x\n",i);
    		ioctl(file, I2C_SLAVE, i);
    	
    		if(read(file, data, 1) != 1) //not found
    		{
        		continue;
    		}		
    		
    		printf("Adding :%x\n",i);
    		adresses[j]=i;
    		module_counter++;
    		j++;
    	}
}



int main(int argc, char *argv[])
{
	int file;
	char *bus = "/dev/i2c-1";
	int i;
	int x;
	uint8_t adresses[30]={}; //change if needed

	if((file = open(bus, O_RDWR)) < 0)
	{
	
	printf("Failed to open the bus. \n");
		exit(1);
	}

	//while(1)
	//{
		scanf(" %d ",&x);
		ScanforModules(file,adresses);
		printf("\nSCAN RESULTS:\n");
		
		for(i=0;i<sizeof(adresses);i++)
		{
			if (adresses[i]==0)
			{ 
				break;
			}	
			printf("%d: %x  |",i , adresses[i]);
		}
	//}
}
