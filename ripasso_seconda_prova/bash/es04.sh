#!/bin/bash

# Si scriva uno script che
# visualizzi il messaggio "Enter password: ", legga una stringa
# e la confronti con la stringa
# "secret" sino all'introduzione della stringa corretta

correct_pass="secret"

read -p "Enter pasword: " password

while [[ "$password" != "$correct_pass" ]]; do
    read -p "Enter pasword: " password
done
