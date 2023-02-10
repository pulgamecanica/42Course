-- https://mariadb.com/kb/en/

-- create `LOGIN` user, change password and grant privileges to wordpress database
CREATE USER IF NOT EXISTS '$MARIADB_USER_NAME'@'%' IDENTIFIED BY '$MARIADB_USER_PASS';
GRANT ALL PRIVILEGES ON $WORDPRESS_DB_NAME.* TO '$MARIADB_USER_NAME'@'%' WITH GRANT OPTION;
#DROP TABLE WHERE '%test%'
-- set root password
UPDATE mysql.user SET password=password('$MARIADB_ROOT_PASS') WHERE user='root';

-- reset to take effect
FLUSH PRIVILEGES;
