---------------------------------------------------------------------
				DIGIMON STORY NDS PAK FORMAT __ MSG
---------------------------------------------------------------------

HEADER (16 bytes)
4 bytes: Number of files
4 bytes: Version number
8 bytes: Padding

-POINTER LIST
	POINTER (16 bytes)
	4 bytes: address of file
	4 bytes: size of file (bytes)
	4 bytes: copy of previous 4 bytes
	4 bytes: unknown. Always set to 0x80000000?

-Files
	4 bytes: Junk? all 0s
	4 bytes: Number of messages in partition
	DATA LIST? (4 bytes * Number of messages)
		4 bytes: offset from the partition address to find a message.

---------------------------------------------------------------------
				DIGIMON STORY NDS PAK FORMAT __ NCGR
---------------------------------------------------------------------
HEADER (16 bytes)
4 bytes: Number of files
4 bytes: Version number
8 bytes: Junk or Padding?

-POINTER LIST
	POINTER (16 bytes)
	4 bytes: address of file
	4 bytes: 
	4 bytes: size of file (bytes)
	4 bytes: junk padding. 0x0.