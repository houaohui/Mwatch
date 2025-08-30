C:\Keil_v5\MDK\ARM\ARMCC\bin\fromelf.exe  --strip=comment,debug  --elf --output=Bin\app-evasion_.axf Objects\app-evasion.axf
python D:\work\flash_pack_tool\flash_pack_tool.py  Bin\out.bin Bin\app-evasion_.axf 1
C:\Keil_v5\MDK\ARM\ARMCLANG\bin\fromelf.exe --text -c -a Bin\app-evasion_.axf > Bin\disassembly.txt