savedcmd_my_module.mod := printf '%s\n'   my_module.o | awk '!x[$$0]++ { print("./"$$0) }' > my_module.mod
