SET local="C:\Users\hansh\Documents\GitHub\Microprocessors-II-Laboratory\Lab 3\galileo code"
SET target="/home/root/Documents"
SET IP="129.63.205.98"
SET PASS="Bruins33"

cd "C:\Users\hansh\Documents\SCHOOL UML\Fall 2017\Micro II"
pscp -pw %PASS% -r %LOCAL% root@%IP%:%target%