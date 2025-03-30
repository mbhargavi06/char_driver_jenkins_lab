savedcmd_/home/ubuntu/char_driver/char_driver.mod := printf '%s\n'   char_driver.o | awk '!x[$$0]++ { print("/home/ubuntu/char_driver/"$$0) }' > /home/ubuntu/char_driver/char_driver.mod
