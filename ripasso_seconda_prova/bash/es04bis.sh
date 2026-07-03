#!/bin/bash

# Si modichi lo script in modo
# da consentire massimo 3 tentativi

correct_pass="secret"
cont=2
read -p "Enter pasword: " password
while [[ "$password" != "$correct_pass" && $cont -gt 0 ]]; do
    read -p "Enter pasword: " password
    cont=$((cont-1))
done
exit 0