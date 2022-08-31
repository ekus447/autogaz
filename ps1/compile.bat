@echo off
color f
cls
psymake
if EXIST "MAIN.EXE" ("./tools/ePSXe.lnk" -nogui %cd%\MAIN.EXE)