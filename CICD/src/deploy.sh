sshpass -p "root" scp cat/s21_cat deploy@172.24.116.8:/usr/local/bin/
sshpass -p "root" scp grep/s21_grep deploy@172.24.116.8:/usr/local/bin/
sshpass -p "root" ssh deploy@172.24.116.8 ls /usr/local/bin/ >> log.txt
