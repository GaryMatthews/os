    global                  modfile_0, modfile_0_size

    section .data

modfile_0:              incbin      "mods/01.mod"
modfile_0_size:         dd $-modfile_0

    
