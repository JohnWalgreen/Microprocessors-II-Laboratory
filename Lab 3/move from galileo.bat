SET "local=.\from Galileo"
SET "target=/home/root/Documents/to PC"
SET "IP=129.63.205.98"
SET "PASS=Bruins33"

..\pscp -pw %PASS% "root@%IP%:%target%/*" "%local%"
