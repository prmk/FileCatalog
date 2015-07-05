Read Me for FileCatalogue Project
Author: Pramukh Shenoy

1. The input parameters can be in any order, including the path.
	../TestCases *.h /s /D /f "test" *.cpp is same as  
	*.h /s /D ../TestCases /f "test" *.cpp

2. All the options are case-insensitive;  i.e. Both /D and /d will search for duplicates.

3. The search option needs a space between /f and search text.

4. For the requirement 8 if no search pattern is provided, the pattern already provided during the Catalogue build is taken.

5. For the requirement 8, the patterns should be provided in the format: *.h *.cpp

6. Use the compile.bat to compile the Project.

7. Use the run.bat to run the application. run.bat provides demo of all the requirements of the project.