SET "local=.\from Galileo"
SET "target=/media/card/to PC"
SET "IP=129.63.205.98"
SET "PASS=Bruins33"
..\pscp -pw %PASS% "root@%IP%:%target%/*" "%local%"