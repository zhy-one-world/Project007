
Å
eye_proto.proto"F
E2D_OPEN_FILE
nFlag (
	bFullPath (
strFileName (	"!
E2D_READ_FILE
nDataLen ("3
E2D_WRITE_FILE
nDataLen (
content ("/
E2D_SEEK_FILE
bKeep (
nOffset ("@
E2D_GET_PORT_INFO
port (
pid (
serverid ("1
SERVER_ID_MAP
nType (
	nServerID ("6
E2D_SYNC_SERVERID!
	serverids (2.SERVER_ID_MAP"
E2D_SERVER_PID"r
E2D_EXE_SYSCMD
bNeedOutput (
bNeedCN (

strCommand (	
strInputBuffer (	
strMD5 (	"z
E2D_EXE_SYSCMD_LARGE
unNeedOutput (
unNeedCN (

strCommand (
strInputBuffer (	
strMD5 (	"D
E2D_UPDATE_SERVER
strPack (	
strMD5 (	
nCheck ("?
E2D_FILE
	unFileLen (
strPath (	
strData (	"b
E2D_FTP_DOWNLOAD

strFtpHost (	

strFtpUser (	

strFtpPass (	

strFtpPath (	"*

E2S_GM_CMD
gmCode (	
data (	"H
E2S_MAIL_ATTACHMENT
itemType (
itemId (
itemNum ("#
E2S_MAIL_FILTER
conditon ("«
E2S_SEND_MAIL
strTitle (	
	strSender (	

strContent (	
strPlayerGUID (	(

attachment (2.E2S_MAIL_ATTACHMENT 
filter (2.E2S_MAIL_FILTER"7
D2E_GET_SERVERID
nPId (
nGameServerId ("I
D2E_FILE_OPERATION
nFileLen (
nRetCode (
content ("@
D2E_GET_CPUBASEINFO
strCPUVendor (	
strCPUBrand (	"8
D2E_GET_CPULOAD
	nCoreLoad (

nTotalLoad ("K
D2E_GET_GAME_PROCINFO

strCpuRate (	
strVSZ (	
strRSS (	"6
D2E_GET_MEMINFO
	nTotalMem (
nFreeMem ("}
	PROC_INFO
strProcName (	
unPID (
unMemUse (
unVMSize (
	unCPUTime (
unThreadCount ("F
D2E_GET_PROCINFO
unProcCount (
	ProcInfos (2
.PROC_INFO"[
D2E_GAME_PROCINFO
isAlive (
unMemUse (
unVMSize (
	unCPULoad ("K
DISK_BASEINFO
strDiskDesc (	

nTotalSize (
	nFreeSize ("J
D2E_GET_DISKINFO
unDiskCount (!
	DiskInfos (2.DISK_BASEINFO"S
NET_CARDINFO
strCardDesc (	
strIP (	
strMask (	
strMac (	"s
D2E_GET_NETINFO

nCardCount (
strSystemName (	
strHostName (	 
	CardInfos (2.NET_CARDINFO"V
NET_CARDLOAD
unTXSize (
unRXSize (
unTXRate (
unRXRate ("I
D2E_GET_CARDLOAD
unCardCount ( 
	CardLoads (2.NET_CARDLOAD"'
D2E_GET_GUARDDIR
strGuardDir (	"<
D2E_EXE_SYSCMD
nReturnCode (
strOutputBuff ("0
D2E_GET_PORT_INFO
port (
count ("b
D2E_UPDATE_SERVER
nCurrentStep (

nStepCount (
nResult (

strMessage (	"K
D2E_FTP_DOWNLOAD
	nProgress (
nRessult (

strMessage (	"O
S2D_GET_SERVER_INFO
nServerType (
nPid (
nGameServerId ("³
	eye_protoA
s2d_tick_server_connect
	server_id (
server_type (r
s2d_send_server_info_to_daemon
	server_id (
server_type (

process_id (
param_string (	;
d2s_close_process
	server_id (
server_type (_
d2s_gm_order
listen_port (
handle_index (
	server_id (
	gm_string (	y
s2d_gm_order_end
listen_port (
handle_index (
	server_id (
result_code (

result_msg (	V
s2d_send_string_to_eye
	server_id (
server_type (
param_string (	