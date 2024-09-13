BUILD = /usr/src/linux-6.10.3

all:
	@echo "Copying source files"
	rsync -a ./get_pid_info/get_pid_info.c ${BUILD}/kernel/get_pid_info.c
	rsync -a ./get_pid_info/Makefile ${BUILD}/kernel/Makefile
	rsync -a ./get_pid_info/syscalls.h ${BUILD}/include/linux/syscalls.h
	rsync -a ./get_pid_info/syscall_64.tbl ${BUILD}/arch/x86/entry/syscalls/syscall_64.tbl
	@echo "Compiling..."
	make -j4 -C ${BUILD}	
	make modules_install -C ${BUILD}	
	@echo "Compilation finished successfully."
	@echo "Preparing system..."
	cp ${BUILD}/arch/x86_64/boot/bzImage /boot/vmlinuz-linux-ft_linux
	cp ${BUILD}/System.map /boot/System.map-ft_linux
	cp ${BUILD}/.config /boot/config-ft_linux
	grub-mkconfig -o /boot/grub/grub.cfg
	sed s/linux/linux-ft_linux/g </etc/mkinitcpio.d/linux.preset >/etc/mkinitcpio.d/linux-ft_linux.preset
	mkinitcpio -p linux-ft_linux
	@echo "Rebooting"
	sleep 5
	shutdown -r

