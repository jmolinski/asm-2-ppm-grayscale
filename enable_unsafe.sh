#! sh

echo 'deb http://archive.debian.org/debian/ lenny allow-insecure=yes main' > /etc/apt/sources.list
apt-get --allow-unauthenticated upgrade
