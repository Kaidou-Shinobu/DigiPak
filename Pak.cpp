#include "Pak.h"

Pak::Pak() : extension(".bin"){
}

std::string Pak::hexToStr(std::uint32_t& data) {

	std::string newString;
	char* ptr = reinterpret_cast<char*>(&data);
	newString.push_back(*ptr);
	newString.push_back(*(ptr + 1));
	newString.push_back(*(ptr + 2));
	newString.push_back(*(ptr + 3));
	return newString;
}

std::unique_ptr<char[]> Pak::decompressPrototype(char* compressedBuffer, int uncompressedSize) {
	std::unique_ptr<char[]> buffer = std::make_unique<char[]>(uncompressedSize);
	std::unique_ptr<char[]> buffer2 = std::make_unique<char[]>(0x1000);
	std::uint32_t r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r12, lr;
	char* compressedData = compressedBuffer;

	//STMFD   SP!, {R3-R9,LR} ; r0 - source?
	//                        ; r1 - destination?
	//                        ; r2 - buffer?
	r0 = 0;
	r1 = 0;
	r2 = 0;

	//LDR     R5, =0xFEE
	//MOV     R6, R2
	//MOV     R8, R0
	//MOV     R4, #0
	//MOV     R7, R1
	r5 = 0xFEE;
	r6 = r2;
	r8 = r0;
	r4 = 0;
	r7 = r1;

	//MOV     R0, R6
	//MOV     R1, R4
	//MOV     R2, R5
	//BL      MI_CpuFill8
	//std::memset(buffer2.get() + r4, 0, r5);
	std::fill(buffer2.get() + r4, buffer2.get() + r4 + r5, 0);



	std::copy(compressedData, compressedData + 4, reinterpret_cast<char*>(&r0));
	//std::memcpy(&r0, compressedData, sizeof(std::uint32_t)); //Older alternative. 

	r8 += 4;

	//loc_208A90C:
	//CMP     R0, #0
	//LDMLEFD SP!, {R3-R9,PC}
	while (r0 > 0)
	{
		//MOV     R1, R4,LSL#15
		//MOV     R4, R1,LSR#16
		r1 = r4 << 15;
		r4 = r1 >> 16;

		//TST     R4, #0x100
		//BNE     loc_208A93C
		if ((r4 & 0x100) != 0x100)
		{
			//SUBS    R0, R0, #1
			//LDRB    R1, [R8],#1
			//LDMMIFD SP!, {R3-R9,PC}
			//ORR     R1, R1, #0xFF00
			//MOV     R1, R1,LSL#16
			//MOV     R4, R1,LSR#16
			r0 = r0 - 1;
			r1 = compressedData[r8++];
			if ((int)r0 < 0) break;

			r1 = r1 | 0xFF00;
			r1 = r1 << 16;
			r4 = r1 >> 16;
		}

		//TST     R4, #0x1
		//BEQ     loc_208A96C
		if ((r4 & 0x1) == 0x1)
		{
			//SUBS    R0, R0, #1
			//LDRB    R1, [R8],#1
			//LDMMIFD SP!, {R3-R9,PC}
			//AND     R3, R1, #0xFF
			//STRB    R3, [R7],#1
			//LDR     R1, =0xFFF
			//ADD     R2, R5, #1
			//STRB    R3, [R6,R5]
			//AND     R5, R2, R1
			//B       loc_208A90C

			r0 = r0 - 1;
			r1 = compressedData[r8++];
			if ((int)r0 < 0) break;
			r3 = r1 & 0xFF;
			buffer[r7++] = static_cast<char>(r3);
			r1 = 0xFFF;
			r2 = r5 + 1;
			buffer2[r6 + r5] = static_cast<char>(r3);
			r5 = r2 & r1;
			continue;
		}

		//LDRB    R3, [R8]
		//SUBS    R1, R0, #1
		//LDMMIFD SP!, {R3-R9,PC}
		//LDRB    R1, [R8,#1]
		//ADD     R8, R8, #2
		//SUBS    R0, R0, #2
		//LDMMIFD SP!, {R3-R9,PC}
		//AND     R2, R1, #0xF0
		//AND     R1, R1, #0xF
		//ADD     R9, R1, #2
		//LDR     R1, =0xFFF
		//ORR     R12, R3, R2,LSL#4
		//MOV     LR, #0
		//r3 = compressedData[r8];
		std::copy(compressedData + r8, compressedData + r8 + 1, reinterpret_cast<char*>(&r3));
		r1 = r0 - 1;
		if ((int)r1 < 0) break;
		//r1 = compressedData[r8 + 1];
		std::copy(compressedData + r8 + 1, compressedData + r8 + 1 + 1, reinterpret_cast<char*>(&r1));
		r8 = r8 + 2;
		r0 = r0 - 2;
		if ((int)r0 < 0) break;
		r2 = r1 & 0xF0;
		r1 = r1 & 0xF;
		r9 = r1 + 2;
		r1 = 0xFFF;
		r12 = r3 | (r2 << 4);
		lr = 0;

		do
		{
			//loc_208A9A0:
			//    ADD     R2, R12, LR
			//    AND     R2, R2, R1
			//    LDRB    R3, [R6,R2]
			//    ADD     LR, LR, #1
			//    ADD     R2, R5, #1
			//    STRB    R3, [R7],#1
			//    STRB    R3, [R6,R5]
			//    CMP     LR, R9
			//    AND     R5, R2, R1
			//    BLE     loc_208A9A0
			r2 = r12 + lr;
			r2 = r2 & r1;
			//r3 = buffer2[r6 + r2];
			std::copy(buffer2.get() + r6 + r2, buffer2.get() + r6 + r2 + 1, reinterpret_cast<char*>(&r3));
			lr = lr + 1;
			r2 = r5 + 1;
			buffer[r7++] = static_cast<char>(r3);
			buffer2[r6 + r5] = static_cast<char>(r3);
			r5 = r2 & r1;
		} while (lr <= r9);
	}
	return std::move(buffer);
}