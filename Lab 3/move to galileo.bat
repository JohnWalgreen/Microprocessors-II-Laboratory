SET "local=.\to Galileo"
SET "target=/home/root/Documents/from PC"
SET "IP=129.63.205.98"
SET "PASS=Bruins33"

..\pscp -pw %PASS% "%LOCAL%\*" "root@%IP%:%target%"
