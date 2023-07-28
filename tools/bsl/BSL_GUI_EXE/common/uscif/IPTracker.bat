@echo off

REM This batch file calculates the range of IPs on the network based on IP address and subnet mask specified.It scans through all the IP range and prints the arp table to a text file.
REM This batch file takes 3 parameters. The first two parameters are mandatory and the third one is optional.
REM The first parameter is the ipaddress(ipaddress of the PC you are running the script on).
REM The second parameter is the subnet mask(subnet mask of the network you are connected to).
REM The above two parameters can be found by the command ipconfig /all from the command prompt.
REM
REM The third argument is to distinguish between fast scan and complete scan(fast/complete).The fast scan prints out a quick list of the arp cache table on your local area network.This list is random and quick. This list may not list out the device you are looking for all the time.
REM Inorder for the device you are looking for to always show up on the arpcache table do a complete scan which is little slower and always perfect.
REM The fourth parameter is the full path and name of the arpcache table. If this parameter is not specified the batch file would generate the output file in the local directory with the default name as arpcache.txt
REM
REM !!!!!!!!If the fouth parameter is used and the path has spaces in it, specify the full path under quotation marks.!!!!!!
REM
REM ex: IPTracker.bat 10.0.3.51 255.255.0.0 complete "C:\Program Files\Texas Instruments\ccsv4\common\uscif\arpcache.txt" - this would scan through all the ip address in your network range and print the arp table to arpcache.txt under the specified path.
REM
REM

SETLOCAL

IF "%1"=="" GOTO ipaddress
SET ipaddress=%1

:subnet1
IF "%2"=="" GOTO subnet
SET subnet=%2

:scan1
IF "%3"=="" GOTO scan
SET scan=%3

:start
for /f "tokens=1,2,3,4 delims=. " %%a in ("%ipaddress%") do set ip1=%%a&set ip2=%%b&set ip3=%%c&set ip4=%%d
for /f "tokens=1,2,3,4 delims=. " %%a in ("%subnet%") do set sub1=%%a&set sub2=%%b&set sub3=%%c&set sub4=%%d

set /A temp1 =""%sub1%" ^ "255""
set /A Bcast1=""%ip1%" | "temp1""

set /A temp2 =""%sub2%" ^ "255""
set /A Bcast2=""%ip2%" | "temp2""

set /A temp3 =""%sub3%" ^ "255""
set /A Bcast3=""%ip3%" | "temp3""

set /A temp4 =""%sub4%" ^ "255""
set /A Bcast4=""%ip4%" | "temp4""

set BCAST=%Bcast1%.%Bcast2%.%Bcast3%.%Bcast4%

REM echo The current BCAST address is %BCAST%

if "%scan%"=="fast" goto fastscan

:completescan
set /A Ntwk1="%ip1%"
set /A Ntwk2=""%ip2%" & "%sub2%""
set /A Ntwk3=""%ip3%" & "%sub3%""
set /A Ntwk4=""%ip2%" & "%sub4%""

set NTWRK=%Ntwk1%.%Ntwk2%.%Ntwk3%.%Ntwk4%

REM echo The current Network address is %NTWRK%

set /A IpStart1=%Ntwk1%
set /A IpStart2=%Ntwk2%
set /A IpStart3=%Ntwk3%
set /A IpStart4="%Ntwk4% + "1""

set IPSTART=%IpStart1%.%IpStart2%.%IpStart3%.%IpStart4%

REM echo IP Range Start Adress is %IPSTART%

set /A IpEnd1=%Bcast1%
set /A IpEnd2=%Bcast2%
set /A IpEnd3=%Bcast3%
set /A IpEnd4="%Bcast4% - "1""

set IPEND=%IpEnd1%.%IpEnd2%.%IpEnd3%.%IpEnd4%

REM echo IP Range End Adress is %IPEND%

REM Delete all the values in the arp table.
arp -d

REM PING ALL THE IPS IN THE RANGE

set timeout=1
set startcount=%IpStart4%
set endcount=%IpEnd4%
set teststartip=%ip1%.%ip2%.%ip3%.
for /L %%I in (%startcount%,1,%endcount%) do (ping %teststartip%%%I -n 1 -w %timeout% >nul)

set ARPFILEPATH="%4"
IF "%ARPFILEPATH%"=="""" GOTO default
set ARPPATH=%4
arp -a > %ARPPATH%

goto end

:fastscan
ping -n 2 %BCAST%

set ARPFILEPATH="%4"
IF "%ARPFILEPATH%"=="""" GOTO default
set ARPPATH=%4
arp -a > %ARPPATH%

goto end

:ipaddress
echo Enter IP address in dotted format [ex: 192.168.10.100] :
Set /P ipaddress=
if "%ipaddress%"=="" goto ipaddress
goto subnet1

:subnet
echo Enter subnet mask in dotted format [ex: 255.255.25.0] :
Set /P subnet=
if "%subnet%"=="" goto subnet
goto scan1

:scan
echo Enter Scan Method [fast/complete]:
Set /P scan=
if "%scan%"=="fast" goto start
if "%scan%"=="complete" goto start
goto scan

:default
arp -a > arpcache.txt
goto end

:end
echo ARP CACHE TABLE IS READY!!!!!
ENDLOCAL
