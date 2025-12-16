savedcmd_/home/duguseo/02_auto_device/chardev.mod := printf '%s\n'   chardev.o | awk '!x[$$0]++ { print("/home/duguseo/02_auto_device/"$$0) }' > /home/duguseo/02_auto_device/chardev.mod
