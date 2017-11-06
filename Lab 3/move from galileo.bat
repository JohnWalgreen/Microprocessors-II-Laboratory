SET "local=C:\Users\hansh\Documents\GitHub\Microprocessors-II-Laboratory\Lab 3\from Galileo"
SET "target=/home/root/Documents/to PC"
SET "IP=129.63.205.98"
SET "PASS=Bruins33"

cd "C:\Users\hansh\Documents\SCHOOL UML\Fall 2017\Micro II"
pscp -pw %PASS% "root@%IP%:%target%/*" "%local%"
