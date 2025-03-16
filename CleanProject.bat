@echo off
setlocal

:: Define the folders to delete
set foldersToDelete=Binaries Intermediate Saved

:: Delete specified folders if they exist
for %%F in (%foldersToDelete%) do (
    if exist "%%F" (
        echo Deleting %%F...
        rmdir /s /q "%%F"
    )
)

:: Delete .sln files in the project folder
for %%S in (*.sln) do (
    echo Deleting %%S...
    del "%%S"
)

echo Cleanup complete!
pause
