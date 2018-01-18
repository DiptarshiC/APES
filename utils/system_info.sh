

#the following bash script displays the information as asked in the assignment in the same order


#!/bin/bash

echo 1.user infomration:


id diptarshi #1.user information

echo

echo 2.OS brand/type, 3.OS distribution and 4.OS version:


cat /etc/lsb-release #2.Operating system type/brand, version no.

echo

echo 5.kernel version:

uname -r  #3. kernel version

echo

echo 6.kernel gcc version build :

ls -l /usr/bin/gcc

echo

echo 7.kernel build TIME :

uname -v

echo

echo 8.file system memory:

free -m

echo

echo 9. percentage of disk used

df -h

echo

echo 10. Tasks running user:

top  -bn1

echo
