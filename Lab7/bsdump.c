/* bsdump.c 
 * 
 * Reads the boot sector of an MSDOS floppy disk
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define SIZE 32  /* size of the read buffer */
//define PRINT_HEX // un-comment this to print the values in hex for debugging

struct BootSector
{
    unsigned char  sName[9];            // The name of the volume
    unsigned short iBytesSector;        // Bytes per Sector
    
    unsigned char  iSectorsCluster;     // Sectors per Cluster
    unsigned short iReservedSectors;    // Reserved sectors
    unsigned char  iNumberFATs;         // Number of FATs
    
    unsigned short iRootEntries;        // Number of Root Directory entries
    unsigned short iLogicalSectors;     // Number of logical sectors
    unsigned char  xMediumDescriptor;   // Medium descriptor
    
    unsigned short iSectorsFAT;         // Sectors per FAT
    unsigned short iSectorsTrack;       // Sectors per Track
    unsigned short iHeads;              // Number of heads
    
    unsigned short iHiddenSectors;      // Number of hidden sectors
};


unsigned short endianSwap(unsigned char one, unsigned char two);
// Pre: Two initialized characters
// Post: The characters are swapped (two, one) and returned as a short

void decodeBootSector(struct BootSector * pBootS, unsigned char buffer[]);
// Pre: An initialized BootSector struct and a pointer to an array
//      of characters read from a BootSector
// Post: The BootSector struct is filled out from the buffer data

void printBootSector(struct BootSector * pBootS);
// Pre: A filled BootSector struct
// Post: The information about the boot sector prints to the console


// entry point:
int main(int argc, char * argv[])
{
	int pBootSector = 0;
	unsigned char buffer[SIZE];
	struct BootSector sector;
    
	// Check for argument
	if (argc < 2) {
		printf("Specify boot sector\n");
		exit(1);
	}
    
	// Open the file and read the boot sector
	pBootSector = open(argv[1], O_RDONLY);
	read(pBootSector, buffer, SIZE);
    close(pBootSector);
    
	// Decode the boot Sector
	decodeBootSector(&sector, buffer);
    
	// Print Boot Sector information
	printBootSector(&sector);
    
    return 0;
}  // end main()


// Converts two characters to an unsigned short with two, one
unsigned short endianSwap(unsigned char one, unsigned char two)
{
    // swap
	return (two << 8) | one;
}


// Fills out the BootSector Struct from the buffer
void decodeBootSector(struct BootSector * pBootS, unsigned char buffer[])
{
	int i = 3;  // Skip the first 3 bytes
	unsigned char first;
	unsigned char second;
	unsigned short value;
    
	// Pull the name and put it in the struct (remember to null-terminate)
	memcpy(pBootS->sName, &(buffer[i]), 8);
	pBootS->sName[9] = '\0';
    
	// Read bytes/sector and convert to big endian
	i = 11; //offset for bytes/sector
	first = buffer[i++];
	second = buffer[i++];
	pBootS->iBytesSector = endianSwap(first, second);
    
	// Read sectors/cluster, Reserved sectors and Number of Fats
	first = buffer[i++];
	second = 0;
	pBootS->iSectorsCluster = endianSwap(first, second);
	
	first = buffer[i++];
	second = buffer[i++];
	pBootS->iReservedSectors = endianSwap(first, second);
	
	first = buffer[i++];
	second = 0;
	pBootS->iNumberFATs = endianSwap(first, second);
    
	// Read root entries, logicical sectors and medium descriptor
	first = buffer[i++];
	second = buffer[i++];
	pBootS->iRootEntries = endianSwap(first, second);
	
	first = buffer[i++];
	second = buffer[i++];
	pBootS->iLogicalSectors = endianSwap(first, second);
	
	first = buffer[i++];
	second = 0;
	pBootS->xMediumDescriptor = endianSwap(first, second);	
    
	// Read and covert sectors/fat, sectors/track, and number of heads
	first = buffer[i++];
	second = buffer[i++];
	pBootS->iSectorsFAT = endianSwap(first, second);
	
	first = buffer[i++];
	second = buffer[i++];
	pBootS->iSectorsTrack = endianSwap(first, second);
	
	first = buffer[i++];
	second = buffer[i++];
	pBootS->iHeads = endianSwap(first, second);
    
	// Read hidden sectors
	first = buffer[i++];
	second = buffer[i++];
	pBootS->iHiddenSectors = endianSwap(first, second);
    
	return;
}


// Displays the BootSector information to the console
void printBootSector(struct BootSector * pBootS)
{
#ifndef PRINT_HEX
	printf("                    Name:   %s\n", pBootS->sName);
	printf("            Bytes/Sector:   %i\n", pBootS->iBytesSector);
	printf("         Sectors/Cluster:   %i\n", pBootS->iSectorsCluster);
	printf("        Reserved Sectors:   %i\n", pBootS->iReservedSectors);
	printf("          Number of FATs:   %i\n", pBootS->iNumberFATs);
	printf("  Root Directory entries:   %i\n", pBootS->iRootEntries);
	printf("         Logical sectors:   %i\n", pBootS->iLogicalSectors);
	printf("       Medium descriptor:   0x%04x\n", pBootS->xMediumDescriptor);
	printf("             Sectors/FAT:   %i\n", pBootS->iSectorsFAT);
	printf("           Sectors/Track:   %i\n", pBootS->iSectorsTrack);
	printf("         Number of heads:   %i\n", pBootS->iHeads);
	printf("Number of Hidden Sectors:   %i\n", pBootS->iHiddenSectors);
#else
	printf("                    Name:   %s\n",     pBootS->sName);
	printf("            Bytes/Sector:   0x%04x\n", pBootS->iBytesSector);
	printf("         Sectors/Cluster:   0x%02x\n", pBootS->iSectorsCluster);
	printf("        Reserved Sectors:   0x%04x\n", pBootS->iReservedSectors);
	printf("          Number of FATs:   0x%02x\n", pBootS->iNumberFATs);
	printf("  Root Directory entries:   0x%04x\n", pBootS->iRootEntries);
	printf("         Logical sectors:   0x%04x\n", pBootS->iLogicalSectors);
	printf("       Medium descriptor:   0x%02x\n", pBootS->xMediumDescriptor);
	printf("             Sectors/FAT:   0x%04x\n", pBootS->iSectorsFAT);
	printf("           Sectors/Track:   0x%04x\n", pBootS->iSectorsTrack);
	printf("         Number of heads:   0x%04x\n", pBootS->iHeads);
	printf("Number of Hidden Sectors:   0x%04x\n", pBootS->iHiddenSectors);
#endif
	return;
}
