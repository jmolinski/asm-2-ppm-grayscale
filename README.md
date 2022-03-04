
SETUP 

Wszystko powinno być robione w dokładnie takiej kolejności jak jest napisane

a) Do wykonania na hoście:

1. sh download_deps.sh
wymaga brew
2. sh download_vm.sh
3. sh add_to_ssh_cfg.sh
4. sh run_vm.sh
login: root
password: root

b) Do wykonania na vm:

1. echo 'deb http://archive.debian.org/debian/ lenny allow-insecure=yes main' > /etc/apt/sources.list && apt-get --allow-unauthenticated upgrade && apt-get install openssh-server
2. cd ~ && mkdir src

c) Do wykonania na hoście:
vm powinna być włączona

1. ssh-copy-id -p 5555 root@localhost
password: root
2. scp -r ./src pwa-arm-dev:~

d) Do wykonania na vm: 
sprawdzenie czy wszystko działa

1. cd ~/src && gcc -o hello hello.c && ./hello
powinno wypisać napis
2. cd ~/src/examples && make && ./first
3. echo $?
powinno wypisać 2

--------------------------------------------------

Po wykonaniu tego setupu mozna usunąć wszystkie skrypty poza run_vm.sh i run_fswatch.sh

VMkę zatrzymuje się poleceniem halt

--------------------------------------------------

Workflow:

1. uruchomienie VMki poleceniem sh run_vm.sh
2. uruchomienie na hoście bash run_fswatch.sh
zmiany z katalogu src na hoscie będą automatycznie synchronizowały się na vmkę
3. wejscie do katalogu src/grayscale na VMce

loop:
    3. modyfikowanie kodu na hoście
    4. make i test na VMce
