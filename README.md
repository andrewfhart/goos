goos
====

An experiment with developing low-level Operating System and Bootloader code.


Booting GOOS
------------

The following provides a summarization of the steps necessary to load the GOOS kernel into memory on machine boot. 

### Sections:

1. Overview
2. Stage One
3. Stage Two
4. Kernel Entry Stub

### Section 1: Overview
	The GOOS kernel comes off the disk and into main memory in three stages. Each stage prepares the way for the ones that follow. The rationale behind dividing the task into three stages is two-fold. Firstly, floppy disk sectors are only 512 bytes long and the BIOS, as the last step in its start-up procedure, loads only one sector (see Section 2 for more details). Thus, to begin with, we have just 512 bytes loaded into main memory. Considering the relative simplicity of the tasks performed by the boot code, this might in fact be enough. However, because this is an educational OS, I wanted to be able to get as much feedback from the boot code as possible. As a result, the boot code is verbose, often printing messages to the console. These ASCII strings take up a large amount of space (one byte per character) making the boot code spill over into a second disk sector. The two stages, written in x86 assembly (Intel flavor), together load the kernel and prepare the machine for kernel entry as described in Sections 2 and 3. The kernel entry stub is the first bit of “kernel” code executed. Its job is explained in Section 4.

### Section 2: Stage One

####2.1 File: 
/boot/goosboot.asm
####2.2 Size: 
512 B
####2.3 Purpose: 
To perform hardware checks and load Stage 2
#### 2.4 Description:
Stage One occupies the first sector of the boot disk and is loaded into memory by the BIOS at address 0x07c00. It terminates with the two byte bootsector signature 0xAA55 (1010101001010101) which is required for the BIOS to recognize it as a valid boot sector. 

#### 2.5 Operation: The functionality of Stage One is listed in order executed

##### 2.5.1:  Save Boot Drive: 
The sector saves the value provided by the BIOS indicating whether the sector was loaded from a floppy disk (0x00) or a hard disk (0x80)

##### 2.5.2: Set Data Segment Register: 
We are operating in 0x0000:0x7c00 so the data segment register (ds)  is set to 0x0000

##### 2.5.3: Create a Stack:  
A temporary 512 (0x200) byte stack is initialized at 0x1d00:0x0200 (growing downward, ie: from 0x1d200 to 0x1d000). This allows later operations to save variables while they reuse registers

##### 2.5.4: Verify Hardware: [verify386] 
This routine verifies that the processor executing the code is at least a 386 processor (the target processor family for GOOS).

##### 2.5.5:  Enable Access to Extended Memory: [enableA20] 
This routine communicates with the keyboard controller to enable the A20 line which allows the processor to access memory above 0xFFFFF. 

##### 2.5.6: Load Stage Two: [loadStage2] 
This routine loads the second stage bootloader into memory at 0x08000. As a final step before jumping to the stage two code, the boot drive variable saved in 2.5.1 is placed in dx so that the second stage can use it to load the kernel. 


### Section 3: Stage 2
	
#### 3.1 File: 
/boot/sector2.asm
#### 3.2 Size: 
512 B
#### 3.3 Purpose: 
To load the kernel into memory, switch to PMODE, and call the kernel entry stub
#### 3.4 Description:
Stage 2 occupies the second sector of the boot disk and is loaded into memory by the first stage at address 0x08000. It has two important tasks: to switch the machine from operating in 16-bit real mode to 32-bit protected mode, and to load the kernel itself off the disk and into memory. 
