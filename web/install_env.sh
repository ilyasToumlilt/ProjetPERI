#!/bin/bash
#install_env.sh

sudo aptitude update
sudo aptitude upgrade
sudo aptitude install apache2
sudo aptitude install php5
sudo aptitude install mysql-server php5-mysql
sudo rm /var/www/index.html
echo "<?php phpinfo(); ?>" > /var/www/index.php
