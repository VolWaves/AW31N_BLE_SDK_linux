// *INDENT-OFF*
if [ -f $1.bin ];
then rm $1.bin;
fi
if [ -f $1_data.bin ];
then rm $1_data.bin;
fi
if [ -f $1_data1.bin ];
then rm $1_data1.bin;
fi
if [ -f $1.lst ];
then rm $1.lst;
fi

export OBJDUMP=/opt/jieli/pi32/bin/objdump
export OBJCOPY=/opt/jieli/pi32/bin/objcopy
export OBJSIZEDUMP=/opt/jieli/pi32/bin/objsizedump

pwd
cd ../../../../app/post_build/bd47/
pwd

${OBJDUMP} -d -print-imm-hex -print-dbg $1.elf > $1.lst
${OBJSIZEDUMP} -lite -skip-zero -enable-dbg-info ${1}.elf | sort -k 4 -r >  ${1}.txt
${OBJCOPY} -O binary -j .app_code $1.elf  $1.bin
${OBJCOPY} -O binary -j .data $1.elf  data.bin
${OBJCOPY} -O binary -j .lowpower_overlay $1.elf  lowpower_overlay.bin
${OBJCOPY} -O binary -j .update_overlay $1.elf  update_overlay.bin
${OBJDUMP} -section-headers  $1.elf
cat $1.bin data.bin lowpower_overlay.bin update_overlay.bin > app.bin

./isd_download -tonorflash -dev bd47 -boot 0x3f31000 -div8 -wait 300 -uboot uboot.boot -app app.bin

./ufw_maker -fw_to_ufw jl_isd.fw
cp jl_isd.ufw update.ufw

if [ -f "jl_isd.ufw" ]; then rm jl_isd.ufw; fi
//rm jl_isd.ufw

/* #include "app_config.h" */
//host-client -project ${NICKNAME} -mode flash_debug -f app.bin $1.elf isd_config.ini uboot.boot bd49loader.bin
/* host-client -project ${NICKNAME} -mode flash_debug -f app.bin $1.elf uboot.boot uboot.boot_debug bd49loader.bin  bd49loader.uart */
//host-client -project ${NICKNAME} -mode flash_debug -f app.bin $1.elf ota*.bin isd_config.ini *.boot *.boot_debug
