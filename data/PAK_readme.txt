HEADER (16 bytes)
4 bytes: Number of files
4 bytes: Version number
8 bytes: Padding

-POINTER LIST
	POINTER (16 bytes)
	4 bytes: address of file
	4 bytes: Uncompressed Size (bytes)
	4 bytes: Compressed Size
	4 bytes: unknown. Compressed boolean = 0x80000000? Uncompressed boolean = 0x00000000

---------------------------------------------------------------------
				DIGIMON STORY NDS PAK FORMAT __ MSG
---------------------------------------------------------------------
-Files
	4 bytes: Junk? all 0s
	4 bytes: Number of messages in partition
	DATA LIST? (4 bytes * Number of messages)
		4 bytes: offset from the partition address to find a message.
-Notes:
	Message files are flagged as compressed, but clearly not.
---------------------------------------------------------------------
				DIGIMON STORY NDS PAK FORMAT __ NCGR
---------------------------------------------------------------------
