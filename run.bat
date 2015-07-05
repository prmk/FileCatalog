:run.bat
:
:runs FileCatalog code

cls
echo "Demo of Req#3 with recursive directory search"
FileCatalog\Debug\FileCatalog.exe FileCatalog/TestCases /s *.h *.cpp

cls
echo "Demo of Req#3 without recursive directory search"
FileCatalog\Debug\FileCatalog.exe FileCatalog/TestCases *.*

cls
echo "Demo of Req#4 by printing the contents of the DataStore"
FileCatalog\Debug\FileCatalog.exe FileCatalog/TestCases *.* /s

cls
echo "Demo of Req#5: List of duplicate files"
FileCatalog\Debug\FileCatalog.exe FileCatalog/TestCases *.h *.cpp /d /s

cls
echo "Demo of Req#6: Search text"
FileCatalog\Debug\FileCatalog.exe FileCatalog/TestCases *.* /f "const string& title"

cls
echo "Demo of Req#7: to print summary"
FileCatalog\Debug\FileCatalog.exe

cls
echo "Demo of Req#8: Accept search text in console"
FileCatalog\Debug\FileCatalog.exe FileCatalog/TestCases *.* /s