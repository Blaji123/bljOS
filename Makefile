
GPPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/kernel.o \
	  obj/loader.o \
	  obj/gdt.o \
	  obj/memorymanagement.o \
	  obj/multitasking.o \
	  obj/syscalls.o \
	  obj/spinlock.o \
	  obj/hardwarecommunication/interrupts.o \
	  obj/hardwarecommunication/port.o \
	  obj/hardwarecommunication/interruptstubs.o \
	  obj/hardwarecommunication/pci.o \
	  obj/drivers/amd_am79c973.o \
	  obj/drivers/ata.o \
	  obj/drivers/keyboard.o \
	  obj/drivers/mouse.o \
	  obj/drivers/driver.o \
	  obj/drivers/vga.o \
	  obj/drivers/rtc.o \
	  obj/drivers/ahci.o \
	  obj/drivers/disk.o \
	  obj/gui/desktop.o \
	  obj/gui/widget.o \
	  obj/gui/window.o \
	  obj/gui/button.o \
	  obj/gui/toolbar.o \
	  obj/net/etherframe.o \
	  obj/net/arp.o \
	  obj/net/ipv4.o \
	  obj/net/icmp.o \
	  obj/net/udp.o \
	  obj/net/tcp.o \
	  obj/filesystem/fat32.o \
	  obj/filesystem/msdospart.o \
	  obj/filesystem/vfs.o \
	  obj/datastructures/bitmap.o \

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(GPPPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'set gfxmode=1024x768x32' >> iso/boot/grub/grub.cfg
	echo 'set gfxpayload=keep' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "bljOS" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

run: mykernel.iso
	(killall VirtualBox && sleep 1) || true
	VBoxManage startvm "bljOS" &

.PHONY: clean
clean:
	rm -rf obj mykernel.bin mykernel.iso
