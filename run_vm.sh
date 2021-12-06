#! sh

qemu-system-arm \
-M versatilepb \
-kernel vm/vmlinuz-2.6.26-2-versatile \
-initrd vm/initrd.img-2.6.26-2-versatile \
-hda vm/debian_lenny_arm_standard.qcow2 \
-append "root=/dev/sda1" \
-net nic,model=rtl8139 \
-net user,hostfwd=tcp::5555-:22 \
-nographic
