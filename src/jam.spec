/*
	ROM spec file

	Main memory map

  	0x80000000  exception vectors, ...
  	0x80000400  zbuffer (size 320*240*2)
  	0x80025c00  codesegment
	      :  
  	0x8038F800  cfb 16b 3buffer (size 320*240*2*3)

        Copyright (C) 1997-1999, NINTENDO Co,Ltd.
*/

#include <nusys.h>

/* Use all graphic micro codes */
beginseg
	name	"code"
	flags	BOOT OBJECT
	entry 	nuBoot
	address NU_SPEC_BOOT_ADDR
	stack   NU_SPEC_BOOT_STACK
	include "jam.o"
	include "/usr/lib/n64/PR/rspboot.o"
	include "/usr/lib/n64/PR/aspMain.o"
	include "/usr/lib/n64/PR/n_aspMain.o"
	include "/usr/lib/n64/PR/rspboot.o"
	include "/usr/lib/n64/PR/gspF3DEX2.fifo.o"
	include "/usr/lib/n64/PR/gspL3DEX2.fifo.o"
	include "/usr/lib/n64/PR/gspF3DEX2.Rej.fifo.o"
    include "/usr/lib/n64/PR/gspF3DEX2.NoN.fifo.o"
    include "/usr/lib/n64/PR/gspF3DLX2.Rej.fifo.o"
	include "/usr/lib/n64/PR/gspS2DEX2.fifo.o"
endseg

beginseg
	name "asset_data"
	flags RAW
	include "assets/assets.dat"
endseg

beginwave
	name	"jam"
	include	"code"
	include "asset_data"
endwave
