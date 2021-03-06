;~ Includes
#include <Crypt.au3>
#include <Constants.au3>
#include <Debug.au3>

;~ _DebugSetup("Log window client 2", True)

Global $datafile
if $CmdLine[0] < 1 Then 
   $datafile = "clientdata.ini"
Else 
   $datafile = $CmdLine[1]
EndIf

Global $g_IP = IniRead($datafile, "connections", "serverip", "-1")
Global $g_PORT = IniRead($datafile, "connections", "serverport", "-1")
Global $encrypt_key = "bla"

Global $casino_path = "C:\poker\OngMachine\"
Global $casino = "OngMachine.exe"
Global $run_path = "C:\kutya\hopper\"
Global $run = "run.bat"
Global $casinoname = IniRead($datafile, "startup", "casinoname", "-1")

Global $listening_socket
Global $listening_ip = IniRead($datafile, "connections", "listenip", "-1")
Global $listening_port = IniRead($datafile, "connections", "listenport", "-1")
Global $max_connections = 100
Global $cmd_max_length = 256

Global $dllName = "OpenScrapeDLL.dll"
Global $tableMap1 = IniRead($datafile, "startup", "tableMap1", "-1") 
Global $tableMap2 = IniRead($datafile, "startup", "tableMap2", "-1") 
Global $dll = DllOpen($dllName)
Global $loaded_table_map = ""

Global $MAX_TABLES = IniRead($datafile, "other", "maxtables", "-1") 
Enum $TAB_X = 0, $TAB_Y, $TAB_HWND, $OH_HWND, $TAB_MAX_DATA
Global $myTables[$MAX_TABLES][$TAB_MAX_DATA]

Global $coords

TCPStartup()
Local $socket = TCPConnect($g_IP, $g_PORT)
dbgOut($socket);
If $socket = -1 Then
   MsgBox(4096, "", "Cannot connect to server fool!")
   Exit
EndIf
dbgOut("Connected to the server")

;~ TCPStartup()
;~ $listening_socket = TCPListen($listening_ip, $listening_port, $max_connections)
;~ If $listening_socket = -1 Then
;~ 	MsgBox(0, "Error TCPListen", "Error: Cannot open port for the client - scheduler!")
;~ 	Exit
;~  EndIf
;~  
;~ $connected_socket = -1
;~ while $connected_socket = -1
;~    $connected_socket = TCPAccept($listening_socket)
;~ WEnd

sleep(10000)

Run($run_path & $run, $run_path, @SW_MAXIMIZE)
sleep(1000)
dbgOut("CMD is active");
send("tancoskurva{ENTER}")

dbgOut("Opened OngMachine as administrator");
WinWaitActive("OngMachine")
ClickRegionWithoutScrape(WinGetHandle("OngMachine"), IniRead($datafile, "startup", "coord1x", "-1"), IniRead($datafile, "startup", "coord1y", "-1"), 1)

sleep(1000)
WinWaitActive("OngMachine")
ClickRegionWithoutScrape(WinGetHandle("OngMachine"), IniRead($datafile, "startup", "coord2x", "-1"), IniRead($datafile, "startup", "coord2y", "-1"), 1)

sleep(25000)
WinWaitActive($casinoname)
ClickRegionWithoutScrape(WinGetHandle($casinoname), IniRead($datafile, "startup", "coord3x", "-1"), IniRead($datafile, "startup", "coord3y", "-1"), 1)

sleep(9000)
ClickRegionWithoutScrape(WinGetHandle($casinoname), IniRead($datafile, "startup", "coord4x", "-1"), IniRead($datafile, "startup", "coord4y", "-1"), 1)
sleep(1000)
ClickRegionWithoutScrape(WinGetHandle($casinoname), IniRead($datafile, "startup", "coord5x", "-1"), IniRead($datafile, "startup", "coord5y", "-1"), 1)
ClickRegionWithoutScrape(WinGetHandle($casinoname), IniRead($datafile, "startup", "coord6x", "-1"), IniRead($datafile, "startup", "coord6y", "-1"), 1)

WinWaitActive($casinoname)
WinMove($casinoname, "", 3200, 1300)

For $i = 0 To ($MAX_TABLES - 1)
   $myTables[$i][$TAB_HWND] = -1
Next

dbgOut("Init tables...")

For $i = 0 To ($MAX_TABLES - 1)
    while $myTables[$i][$TAB_HWND] = -1
	  $myTables[$i][$TAB_HWND] = OpenOneTable() 
	  if $myTables[$i][$TAB_HWND] = -1 Then sleep(2000)
    WEnd
	$coords = WinGetPos($myTables[$i][$TAB_HWND])
    $myTables[$i][$TAB_X] = $coords[0]
	$myTables[$i][$TAB_Y] = $coords[1]
	Bring(StringTrimLeft($myTables[$i][$TAB_HWND],2), $myTables[$i][$TAB_X], $myTables[$i][$TAB_Y], $i)
    sleep(2000)
Next
dbgOut("Initialization ended")
dbgOut("Starting keepin' up loop")


while 1
   
;~    $recv = TCPRecv($connected_socket, $cmd_max_length)
;~    dbgOut("Received: " & $recv)
;~    if $recv == "END_SESSION" Then
;~ 	  EndSession()
;~ 	  ExitLoop
;~    EndIf

   sleep(10000)
   
   For $i = 0 To ($MAX_TABLES - 1)
	  if NOT WinExists(WinGetTitle($myTables[$i][$TAB_HWND])) Then
		 $myTables[$i][$TAB_HWND] = -1
		 while $myTables[$i][$TAB_HWND] = -1
			$myTables[$i][$TAB_HWND] = OpenOneTable()
			if $myTables[$i][$TAB_HWND] = -1 Then sleep(10000)
		  WEnd
		  $coords = WinGetPos($myTables[$i][$TAB_HWND])
		  $myTables[$i][$TAB_X] = $coords[0]
		  $myTables[$i][$TAB_Y] = $coords[1]
		  Bring(StringTrimLeft($myTables[$i][$TAB_HWND],2), $myTables[$i][$TAB_X], $myTables[$i][$TAB_Y], $i)
		  ExitLoop
	  EndIf
   Next

;~    MouseClick("left", 30, 30, 1)
;~    MouseMove(20, 30, 10)
WEnd

TCPCloseSocket($connected_socket)

Func dbgOut($str)
;~    $curHwnd = WinGetHandle("")
;~    _DebugOut($str)
;~    WinActivate($curHwnd)
 EndFunc
 
Func openOneTableWithoutScrape()

   ClickRegionWithoutScrape(WinGetHandle("bwin"), 280, 170)
   sleep(3000)
   $table_handle = WinGetHandle("")
   ClickRegionWithoutScrape($table_handle, 380, 280)
   ClickRegionWithoutScrape($table_handle, 250, 280)
   ClickRegionWithoutScrape($table_handle, 120, 280)
   ClickRegionWithoutScrape($table_handle, 40, 255)
   ClickRegionWithoutScrape($table_handle, 40, 100)
   
   return $table_handle

EndFunc

Func ClickRegionWithoutScrape($table_handle, $pos_x, $pos_y, $click_number)
   $coords = WinGetPos($table_handle)
   While(WinActive(WinGetTitle($table_handle)) = 0)
	  WinActivate(WinGetTitle($table_handle))
   WEnd
   MouseClick("left", $coords[0]+$pos_x, $coords[1]+$pos_y, $click_number, 1)
EndFunc

Func ScrapeRegion($tableMap, $title_window, $tmRegion, $offset)
   DbgOut("Scraping region " & $title_window & " " & $tmRegion)
   Local $hWnd = WinGetHandle($title_window)
   Local $res = _DllScrape_scrapeRegion($tableMap, $hWnd, $tmRegion, $dllName, 0)
   DbgOut($title_window & " offset 0 " & $tmRegion & "=" & $res) 
   return $res
EndFunc

Func EndSession()
   Local $i = 0
   while $i < $MAX_TABLES And $myTables[$i][$TAB_HWND] = -1
	  $i = $i + 1
   WEnd
   if ($i < $MAX_TABLES) Then
	  ClickRegionWithoutScrape($myTables[$i][$TAB_HWND], IniRead($datafile, "other", "coordEndx", "-1"), IniRead($datafile, "other", "coordEndy", "-1"), 1)
   EndIf
   
   while $i < $MAX_TABLES
	  For $i = 0 To ($MAX_TABLES - 1)
		 if NOT WinExists(WinGetTitle($myTables[$i][$TAB_HWND])) Then
			$myTables[$i][$TAB_HWND] = -1
		 EndIf
	  Next
	  $i = 0
	  while $i < $MAX_TABLES And $myTables[$i][$TAB_HWND] = -1
		 $i = $i + 1
	  WEnd
   WEnd
   
   EndCommand()
   WinClose($casinoname, "")
   WinClose("OngMachine", "")
   
EndFunc

Func OpenOneTable()
   
   Local $table_title
   Local $table_handle
   
   DbgOut("Opentable begin")
   
;~    RANDOM COORDINATE
   ClickRegionWithoutScrape($casinoname, 160, 400, 1)
   
   DbgOut("Searching for a table")
   Local $selected_table = -1
   For $i = 1 To 9 
	  Local $player_number
	  Local $sitting
	  Local $limit
	  
	  $player_number = ScrapeRegion($tableMap1, "bwin", "players" & $i, 0)
	  $sitting = ScrapeRegion($tableMap1, "bwin", "sitting" & $i, 0)
	  $limit = ScrapeRegion($tableMap1, "bwin", "limit" & $i, 0)
	  if  $player_number > 3 And ($sitting <> "sitting") And ($sitting <> "sittting") And  ($limit == "0.04") Then
		 $selected_table = $i
		 ExitLoop
	  EndIf
   Next
   
   if $selected_table == -1 Then
	  ;WinSetState($casinoname, "", @SW_MINIMIZE)
	  Return -1
   EndIf
   DbgOut("Table found: " & $selected_table)
   

   Local $pos_x
   Local $pos_y
   $pos_x = IniRead($datafile, "startup", "tablesBeginx", "-1") 
   $pos_y = IniRead($datafile, "startup", "tablesBeginy", "-1")  + $selected_table * IniRead($datafile, "startup", "tablesPixels", "-1") 
   
   DbgOut("Clicking to position: " & $pos_x & " " & $pos_y)
   ClickRegionWithoutScrape($casinoname, $pos_x, $pos_y, 2)
   
   sleep(5000)
   
   AutoItSetOption("WinTitleMatchMode", 2)
   $array = WinList("No-limit")
   DbgOut("Size of array of tables : " & $array[0][0]);
   Local $found = -1
   for $i = 1 to $array[0][0]
	  $found = -1
	  for $j = 0 to ($MAX_TABLES - 1)
		 if $array[$i][1] == $myTables[$j][$TAB_HWND] Then
			$found = 1
			ExitLoop
		 EndIf
	  Next
	  if $found = -1 Then
		 $table_handle = $array[$i][1]
		 ExitLoop
	  EndIf
   Next
   
   if $found = 1 Then 
	  Return -1
   EndIf
   
   ;listabol!!
   
   ;WinSetState($casinoname, "", @SW_MINIMIZE)
   
   $table_title = WinGetTitle($table_handle)
   DbgOut("Actual table title: " & $table_title)  
   DbgOut("Actual table handle: " & $table_handle)  
   DbgOut("Table opened")  
   DbgOut("Searching for a place")
   Local $sitdown_seat = -1
   While ScrapeRegion($tableMap2, $table_title, "ami", 0) <> "hi"
	  $sitdown_seat = -1
	  For $i = 1 To 6
		 if ScrapeRegion($tableMap2, $table_title, "sitdown" & $i, 0) = 1 Then
			$sitdown_seat = $i
			ExitLoop
		 EndIf
	  Next
	  
	  DbgOut("First state over")
	  
	  if $sitdown_seat = -1 Then
		 For $i = 1 To 6
			if ScrapeRegion($tableMap2, $table_title, "sitdown" & $i, 0) = 1 Then
			   $sitdown_seat = $i
			   ExitLoop
			EndIf
		 Next
		 
		 DbgOut("Second state over")
		 if $sitdown_seat = -1 Then
			ExitLoop
		 Else
			 DbgOut("Got In")
			_DllScrape_clickRegion($tableMap2, $table_handle, "sitdown" & $sitdown_seat, $dllName, 1)
		 EndIf
	  Else
		 _DllScrape_clickRegion($tableMap2, $table_handle, "sitdown" & $sitdown_seat, $dllName, 1)
		  DbgOut("Got In")
	  EndIf
	  sleep(5000)
	  if ScrapeRegion($tableMap2, $table_title, "ami", 0) == "hi" Then
		 $sitdown_seat = 0
		 ExitLoop
	  EndIf
	  sleep(5000)
   WEnd
   
  if $sitdown_seat = -1 Then
	  DbgOut("FUCKED UP")
	  if ScrapeRegion($tableMap2, $table_title, "ami", 0) <> "hi" Then 
		 For $i = 0 To ($MAX_TABLES - 1)
			if $myTables[$i][$TAB_HWND] == $table_handle Then
			   Return -1
			EndIf
		 Next
		 DbgOut("CLOSING WINDOW")
		 WinClose($table_handle, "")
		 if WinExists($table_handle) Then
			 sleep(1000)
			 DbgOut("Window not closed. Clicking stay in button.")
			 ClickRegionWithoutScrape($table_handle, 250, 300, 1)
		 Else
			Return -1
		 EndIf
	  EndIf
   EndIf
   
   DbgOut("Actual table handle: " & $table_handle)  
   DbgOut("FINISHED sitin")
   return $table_handle
EndFunc

Func WaitForACK()
   $recv_encrypted = ""
   While ($recv_encrypted == "") And Not @error
      $recv_encrypted = TCPRecv($socket, 16)
   WEnd
   If $recv_encrypted == "ACK" Then
      dbgOut("Received ACK")
   Else
      dbgOut("!!! Received NOT ACK !!! : " & $recv_encrypted)
   EndIf
EndFunc

Func MutexLock()
   $command = "MUTEX_LOCK"
   $command_encrypted = _Crypt_EncryptData($command, $encrypt_key, $CALG_USERKEY)
   dbgOut("Command : " & $command)
   dbgOut("Command encrypted : " & $command_encrypted)
   TCPSend($socket, $command)
   WaitForACK()
EndFunc

Func MutexUnlock()
   $command = "MUTEX_UNLOCK"
   $command_encrypted = _Crypt_EncryptData($command, $encrypt_key, $CALG_USERKEY)
   dbgOut("Command : " & $command)
   dbgOut("Command encrypted : " & $command_encrypted)
   TCPSend($socket, $command)
   WaitForACK()
EndFunc

Func Bring($table, $pos_x, $pos_y, $id)
   $command = "BRING @ " & $id & " @ " & $pos_x & " @ "& $pos_y & " @ " &$table
   dbgOut("Command : " & $command)
   TCPSend($socket, $command)
   WaitForACK()
EndFunc

Func EndCommand()
   $command = "END_SESSION"
   dbgOut("Command : " & $command)
   TCPSend($socket, $command)
   WaitForACK()
EndFunc




;------- Functions -------

;Function to load Table Map in Dll
Func LoadTablemap($name)

	Local $res = DllCall($dll, "int:cdecl", "OpenTablemap", "str", $name)

	If (@error <> 0) Then
		ConsoleWrite("ERROR in dllcall(OpenTablemap) " & @error & @CRLF)
		Exit
	EndIf

	If ($res[0] = 0) Then
		ConsoleWrite("ERROR in OpenTablemap with map " & $tableMap & @CRLF)
		Exit
	EndIf
EndFunc   ;==>LoadTablemap

;Function to read a region using a window ($hWnd) and name of region
Func ReadRegion($hWnd, $name)
	Return ReadRegionWithOffset($hWnd, $name, 0)
EndFunc   ;==>ReadRegion

;Function to read a region using a window ($hWnd) and name of region
Func ReadRegionWithOffset($hWnd, $name, $offset)
	Local $res = DllCall($dll, "int:cdecl", "ReadRegion", "hwnd", $hWnd, "str", $name, "str*", "", "int", $offset)

	If (@error <> 0) Then
		ConsoleWrite("ERROR in dllcall(ReadRegion) " & @error & @CRLF)
		Exit
	EndIf

	Return $res[3]

EndFunc   ;==>ReadRegionWithOffset

;Function to get Coord from a region ($name)
Func GetCoordRegion($name)
	Local $posl
	Local $posr
	Local $post
	Local $posb

	Local $res = DllCall($dll, "none:cdecl", "GetRegionPos", "str", $name, "int*", $posl, "int*", $post, "int*", $posr, "int*", $posb)

	if (@error <> 0) Then
		ConsoleWrite("ERROR in dllcall(ReadRegion) " & @error & @CRLF)
		Exit
	EndIf

	Return $res

EndFunc   ;==>GetCoordRegion

Func _DllScrape_Init($nameTableMap)
   	;Opening Dll
	Local $dll = DllOpen($dllName)
	;Loading Table Map
	_DllScrape_LoadTablemap($nameTableMap, $dll)

EndFunc

Func _DllScrape_scrapeRegion($nameTableMap, $hWnd, $regionName, $dllName, $offset)

   if ($loaded_table_map <> $nameTableMap) Then
	  _DllScrape_LoadTablemap($nameTableMap,$dll)
	  $loaded_table_map = $nameTableMap
   EndIf

	Local $text = _DllScrape_ReadRegionWithOffset($hWnd, $regionName, $offset,$dll)

	Return $text

EndFunc

Func _DllScrape_clickRegion($nameTableMap, $hWnd, $regionName, $dllName, $numberClicks)

   if ($loaded_table_map <> $nameTableMap) Then
	  _DllScrape_LoadTablemap($nameTableMap,$dll)
	  $loaded_table_map = $nameTableMap
   EndIf

	;Lets Get coord from the region
	Local $coord = _DllScrape_GetCoordRegion($regionName,$dll)

	Local $pos = WinGetPos ( $hWnd )

	Local $extraX = 4      ;Classic XP
	Local $extraY = 23     ;Classic XP

	$x = Int((($coord[4] - $coord[2]) / 2) + $coord[2] + $pos[0] + $extraX )
	$y = Int((($coord[5] - $coord[3]) / 2) + $coord[3] + $pos[1] + $extraY )

   While(WinActive(WinGetTitle($hWnd)) = 0)
	  WinActivate(WinGetTitle($hWnd))
   WEnd
	MouseClick ( "left" , $x  , $y, $numberClicks, 1)

EndFunc

;Function to load Table Map in Dll
Func _DllScrape_LoadTablemap($name, $dll)

	Local $res = DllCall($dll, "int:cdecl", "OpenTablemap", "str", $name)

	If (@error <> 0) Then
		ConsoleWrite("ERROR in dllcall(OpenTablemap) " & @error & @CRLF)
		Exit
	EndIf

	If ($res[0] = 0) Then
		ConsoleWrite("ERROR in OpenTablemap with map " & $name & @CRLF)
		Exit
	EndIf
EndFunc   ;==>LoadTablemap

;Function to read a region using a window ($hWnd) and name of region
Func _DllScrape_ReadRegion($hWnd, $name,$dll)
	Return _DllScrape_ReadRegionWithOffset($hWnd, $name, 0,$dll)
EndFunc   ;==>ReadRegion

;Function to read a region using a window ($hWnd) and name of region
Func _DllScrape_ReadRegionWithOffset($hWnd, $name, $offset, $dll)
	Local $res = DllCall($dll, "int:cdecl", "ReadRegion", "hwnd", $hWnd, "str", $name, "str*", "", "int", $offset)

	If (@error <> 0) Then
		ConsoleWrite("ERROR in dllcall(ReadRegion) " & @error & @CRLF)
		Exit
	EndIf

	Return $res[3]

EndFunc   ;==>ReadRegionWithOffset

;Function to get Coord from a region ($name)
Func _DllScrape_GetCoordRegion($name,$dll)
	Local $posl
	Local $posr
	Local $post
	Local $posb

	Local $res = DllCall($dll, "none:cdecl", "GetRegionPos", "str", $name, "int*", $posl, "int*", $post, "int*", $posr, "int*", $posb)

	if (@error <> 0) Then
		ConsoleWrite("ERROR in dllcall(ReadRegion) " & @error & @CRLF)
		Exit
	EndIf

	Return $res

EndFunc   ;==>GetCoordRegion


