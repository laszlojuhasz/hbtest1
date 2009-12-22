/*
 * $Id$
 */

#include "hbclass.ch"
#include "common.ch"
#include "fileio.ch"
#include "error.ch"

#pragma -kM+

/*
  Docs:

  RFC 1945 - Hypertext Transfer Protocol -- HTTP/1.0
  RFC 2616 - Hypertext Transfer Protocol -- HTTP/1.1
  HTTP Made Really Easy (http://www.jmarshall.com/easy/http/)
*/


#define AF_INET                  2
#define THREAD_COUNT_PREALLOC    0
#define THREAD_COUNT_MAX        50
#define SESSION_TIMEOUT        600

#define CR_LF                       (CHR(13)+CHR(10))

THREAD STATIC s_cResult, s_nStatusCode, s_aHeader, s_lSessionDestroy

MEMVAR server, get, post, cookie, session


INIT PROC SocketInit()
  IF socket_init() != 0
    ? "socket_init() error"
  ENDIF
RETURN


EXIT PROC Socketxit()
  socket_exit()
RETURN


CLASS UHttpd
  // Settings
  DATA nPort        INIT 80
  DATA cBindAddress INIT "0.0.0.0"
  DATA cAccessLog   INIT "uhttpd_access.log"
  DATA cErrorLog    INIT "uhttpd_error.log"
  DATA bIdle        INIT {|| NIL}
  DATA aMount       INIT {=>}

  // Results
  DATA cError       INIT ""

  // Private
  DATA hAccessLog
  DATA hErrorLog

  DATA hmtxQueue
  DATA hmtxLog
  DATA hmtxSession

  DATA hListen
  DATA aSession

  DATA lStop

  METHOD Run()
  METHOD Stop()

  // Private
  METHOD LogAccess()
  METHOD LogError()
ENDCLASS


FUNC UHttpdNew()
RETURN UHttpd()


METHOD Run() CLASS UHttpd
LOCAL hSocket, aRemote, nI, aThreads, aI
LOCAL nWaiters

  IF ! HB_MTVM()
    Self:cError := "Multithread support required"
    RETURN .F.
  ENDIF

  IF Self:nPort < 1 .OR. Self:nPort > 65535
    Self:cError := "Invalid port number"
    RETURN .F.
  ENDIF

  IF (Self:hAccessLog := FOPEN(Self:cAccessLog, FO_CREAT + FO_WRITE)) == -1
    Self:cError :=  "Access log file open error " + LTRIM(STR(FERROR()))
    RETURN .F.
  ENDIF
  FSEEK(Self:hAccessLog, 0, FS_END)

  IF (Self:hErrorLog := FOPEN(Self:cErrorLog, FO_CREAT + FO_WRITE)) == -1
    Self:cError :=  "Error log file open error " + LTRIM(STR(FERROR()))
    FCLOSE(Self:hAccessLog)
    RETURN .F.
  ENDIF
  FSEEK(Self:hErrorLog, 0, FS_END)

  Self:hmtxQueue   := hb_mutexCreate()
  Self:hmtxLog     := hb_mutexCreate()
  Self:hmtxSession := hb_mutexCreate()

  IF (Self:hListen := socket_create()) == NIL
    Self:cError :=  "Socket create error " + LTRIM(STR(socket_error()))
    FCLOSE(Self:hErrorLog)
    FCLOSE(Self:hAccessLog)
    RETURN .F.
  ENDIF

  IF socket_bind(Self:hListen, {AF_INET, Self:cBindAddress, Self:nPort}) == -1
    Self:cError :=  "Bind error " + LTRIM(STR(socket_error()))
    socket_close(Self:hListen)
    FCLOSE(Self:hErrorLog)
    FCLOSE(Self:hAccessLog)
    RETURN .F.
  ENDIF

  IF socket_listen(Self:hListen) == -1
    Self:cError :=  "Listen error " + LTRIM(STR(socket_error()))
    socket_close(Self:hListen)
    FCLOSE(Self:hErrorLog)
    FCLOSE(Self:hAccessLog)
    RETURN .F.
  ENDIF

  aThreads := {}
  FOR nI := 1 TO THREAD_COUNT_PREALLOC
    AADD(aThreads, hb_threadStart(@ProcessConnection(), Self))
  NEXT

  Self:lStop := .F.
  Self:aSession := {=>}

  DO WHILE .T.
    IF (nI := socket_select({Self:hListen},,, 1000)) > 0
      hSocket := socket_accept(Self:hListen)
      IF hSocket == NIL
        Self:LogError("[error] Accept error " + LTRIM(STR(socket_error())))
      ELSE
        hb_mutexQueueInfo( Self:hmtxQueue, @nWaiters )
        ? "New connection", hSocket
        ? "Waiters:", nWaiters
        IF nWaiters < 2 .AND. LEN(aThreads) < THREAD_COUNT_MAX
           /*
              We need two threads in worst case. If first thread becomes a sessioned
              thread, the second one will continue to serve sessionless requests for
              the same connection. We create two threads here to avoid free thread count
              check (and aThreads variable sync) in ProcessRequest().
           */
           AADD(aThreads, hb_threadStart(@ProcessConnection(), Self))
           AADD(aThreads, hb_threadStart(@ProcessConnection(), Self))
        ENDIF
        hb_mutexNotify(Self:hmtxQueue, {hSocket, ""})
      ENDIF
    ELSE
      EVAL(Self:bIdle, Self)
      IF Self:lStop;  EXIT
      ENDIF
    ENDIF
  ENDDO
  socket_close(Self:hListen)

  // End child threads
  hb_mutexLock(Self:hmtxSession)
  HB_HEVAL(Self:aSession, {|k,v| hb_mutexNotify(v[2], NIL), HB_SYMBOL_UNUSED(k)})
  hb_mutexUnlock(Self:hmtxSession)
  AEVAL(aThreads, {|| hb_mutexNotify(Self:hmtxQueue, NIL)})
  AEVAL(aThreads, {|h| hb_threadJoin(h)})

  FCLOSE(Self:hErrorLog)
  FCLOSE(Self:hAccessLog)
RETURN .T.


METHOD Stop() CLASS UHttpd
  Self:lStop := .T.
RETURN NIL


METHOD LogError(cError) CLASS UHttpd
  hb_mutexLock(Self:hmtxLog)
  FWRITE(Self:hErrorLog, DTOS(DATE()) + " " + TIME() + " " + cError + " " + HB_OSNewLine())
  hb_mutexUnlock(Self:hmtxLog)
RETURN NIL


METHOD LogAccess() CLASS UHttpd
LOCAL cDate := DTOS(DATE()), cTime := TIME()
  hb_mutexLock(Self:hmtxLog)
  FWRITE(Self:hAccessLog, ;
         server["REMOTE_ADDR"] + " - - [" + RIGHT(cDate, 2) + "/" + ;
         {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"}[VAL(SUBSTR(cDate, 5, 2))] + ;
         "/" + LEFT(cDate, 4) + ":" + cTime + ' +0000] "' + server["REQUEST_ALL"] + '" ' + ;
         LTRIM(STR(s_nStatusCode)) + " " + LTRIM(STR(LEN(s_cResult))) + ;
         ' "' + server["HTTP_REFERER"] + '" "' + server["HTTP_USER_AGENT"] + ;
         '"' + HB_OSNewLine())
  hb_mutexUnlock(Self:hmtxLog)
RETURN NIL


STATIC FUNC ProcessConnection(oServer)
LOCAL hSocket, cRequest, cSend, aI, nLen, nI, nReqLen, cBuf

  PRIVATE server, get, post, cookie

  DO WHILE .T.
    hb_mutexSubscribe(oServer:hmtxQueue,, @aI)
    IF aI == NIL
      EXIT
    ENDIF

    hSocket := aI[1]
    cRequest := aI[2]

    BEGIN SEQUENCE

      /* receive query header */
      cRequest := ""
      nLen := 1
      DO WHILE AT(CR_LF + CR_LF, cRequest) == 0 .AND. nLen > 0
        IF (nI := socket_select({hSocket},,, 10000)) > 0  /* Timeout */
          nLen := socket_recv(hSocket, @cBuf)
          cRequest += cBuf
        ELSE
          nLen := 0
          ? "recv() timeout", hSocket
        ENDIF
      ENDDO

      IF nLen == -1
        ? "recv() error:", socket_error()
      ELSEIF nLen == 0 /* connection closed */
      ELSE

        // PRIVATE
        server := {=>}
        get := {=>}
        post := {=>}
        cookie := {=>}

        s_cResult := ""
        s_aHeader := {}
        s_nStatusCode := 200

        IF socket_getpeername(hSocket, @aI) != -1
          server["REMOTE_ADDR"] := aI[2]
          server["REMOTE_HOST"] := server["REMOTE_ADDR"]  // no reverse DNS
          server["REMOTE_PORT"] := aI[3]
        ENDIF

        IF socket_getsockname(hSocket, @aI) != -1
          server["SERVER_ADDR"] := aI[2]
          server["SERVER_PORT"] := aI[3]
        ENDIF

        ? LEFT(cRequest, AT(CR_LF + CR_LF, cRequest) + 1)

        nReqLen := ParseRequestHeader(@cRequest)
        IF nReqLen == NIL
          USetStatusCode(400)
        ELSE

          /* receive query body */
          DO WHILE LEN(cRequest) < nReqLen .AND. nLen > 0
            nLen :=  socket_recv(hSocket, @cBuf)
            cRequest += cBuf
          ENDDO

          IF nLen == -1
            ? "recv() error:", socket_error()
          ELSEIF nLen == 0 /* connection closed */
          ELSE
            ? cRequest
            ParseRequestBody(LEFT(cRequest, nReqLen))
            cRequest := SUBSTR(cRequest, nReqLen + 1)

            /* Deal with supported protocols and methods */
            IF server["SERVER_PROTOCOL"] $ "HTTP/1.0 HTTP/1.1"
              IF !(server["REQUEST_METHOD"] $ "GET POST")
                USetStatusCode(501)
              ELSE
                IF server["SERVER_PROTOCOL"] == "HTTP/1.1"
                  IF LOWER(server["HTTP_CONNECTION"]) == "close"
                    UAddHeader("Connection", "close")
                  ELSE
                    UAddHeader("Connection", "keep-alive")
                  ENDIF
                ENDIF
                IF ! ProcessRequest(oServer, hSocket);  BREAK
                ENDIF
              ENDIF
            ELSE /* We do not support another protocols */
              USetStatusCode(400)
            ENDIF
          ENDIF
        ENDIF

        SendResponse(oServer, hSocket)

        IF LOWER(UGetHeader("Connection")) == "close" .OR. server["SERVER_PROTOCOL"] == "HTTP/1.0"
        ELSE
          hb_mutexNotify(oServer:hmtxQueue, {hSocket, cRequest})
          BREAK
        ENDIF
      ENDIF
      ? "Close connection1", hSocket
      socket_shutdown(hSocket)
      socket_close(hSocket)
    END SEQUENCE
  ENDDO
  DBCLOSEALL()
RETURN 0


STATIC FUNC ProcessRequest(oServer, hSocket, cBuffer)
LOCAL nI, cMount, cPath, cSID, hmtx, aData, bEval

  PRIVATE session

  // Search mounting table
  cMount := server["SCRIPT_NAME"]
  IF HB_HHasKey(oServer:aMount, cMount)
    cPath := ""
  ELSE
    nI := LEN(cMount)
    DO WHILE (nI := HB_RAT("/", cMount,, nI)) > 0
      IF HB_HHasKey(oServer:aMount, LEFT(cMount, nI) + "*")
        cMount := LEFT(cMount, nI) + "*"
        cPath := SUBSTR(server["SCRIPT_NAME"], nI + 1)
        EXIT
      ENDIF
      nI--
    ENDDO
  ENDIF

  IF cMount != NIL
    bEval := oServer:aMount[cMount, 1]

    IF oServer:aMount[cMount, 2]
      /* sessioned */
      IF HB_HHasKey(cookie, "SESSID");  cSID := cookie["SESSID"]
      ENDIF

      hb_mutexLock(oServer:hmtxSession)
      IF cSID == NIL .OR. ! HB_HHasKey(oServer:aSession, cSID)

        /* create new session */

        cSID := HB_MD5(DTOS(DATE()) + TIME() + STR(HB_RANDOM(), 15, 12))
        hmtx := hb_mutexCreate()
        oServer:aSession[cSID] := {hb_threadSelf(), hmtx, {=>}}

        // PRIVATE
        session := oServer:aSession[cSID, 3]

        hb_mutexUnlock(oServer:hmtxSession)

        DO WHILE .T.
          s_cResult := ""
          s_aHeader := {}
          s_nStatusCode := 200
          s_lSessionDestroy := .F.
          BEGIN SEQUENCE WITH {|oErr| UErrorHandler(oErr, oServer)}
            EVAL(bEval, cPath)
          RECOVER
            USetStatusCode(500)
          END SEQUENCE

          IF s_lSessionDestroy
            UAddHeader("Set-Cookie", "SESSID=" + cSID + "; path=/; Max-Age=0")
          ELSE
            UAddHeader("Set-Cookie", "SESSID=" + cSID + "; path=/")
          ENDIF

          IF server["SERVER_PROTOCOL"] == "HTTP/1.1"
            IF LOWER(server["HTTP_CONNECTION"]) == "close"
              UAddHeader("Connection", "close")
            ELSE
              UAddHeader("Connection", "keep-alive")
            ENDIF
          ENDIF

          SendResponse(oServer, hSocket)

          IF s_lSessionDestroy
            /* Destroy session before closing socket, since graceful close requires some time */
            hb_mutexLock(oServer:hmtxSession)
            HB_HDel(oServer:aSession, cSID)
            hb_mutexUnlock(oServer:hmtxSession)
          ENDIF

          IF LOWER(UGetHeader("Connection")) == "close" .OR. server["SERVER_PROTOCOL"] == "HTTP/1.0"
            ? "Close connection2", hSocket
            socket_shutdown(hSocket)
            socket_close(hSocket)
          ELSE
            /* pass connection to common queue */
            hb_mutexNotify(oServer:hmtxQueue, {hSocket, cBuffer})
          ENDIF

          IF s_lSessionDestroy
            EXIT
          ENDIF

          IF ! hb_mutexSubscribe(hmtx, SESSION_TIMEOUT, @aData) .OR. aData == NIL
            ? "Session exit"
            hb_mutexLock(oServer:hmtxSession)
            HB_HDel(oServer:aSession, cSID)
            hb_mutexUnlock(oServer:hmtxSession)
            EXIT
          ENDIF
          hSocket := aData[1]
          cBuffer := aData[2]
          bEval := aData[3]
          cPath := aData[4]
          server := aData[5]
          get := aData[6]
          post := aData[7]
          cookie := aData[8]
          session := aData[9]
          aData := NIL
        ENDDO

        /* close databases and release variables */
        DBCLOSEALL()
        server := NIL
        get := NIL
        post := NIL
        cookie := NIL
        session := NIL
      ELSE
        /* session already exists */
          ? "session pries", server["SCRIPT_NAME"]
        hb_mutexNotify(oServer:aSession[cSID, 2], {hSocket, cBuffer, oServer:aMount[cMount, 1], cPath, server, get, post, cookie, oServer:aSession[cSID, 3]})
        hb_mutexUnlock(oServer:hmtxSession)
      ENDIF

      RETURN .F.
    ELSE
      /* not sessioned */
      BEGIN SEQUENCE WITH {|oErr| UErrorHandler(oErr, oServer)}
        EVAL(bEval, cPath)
      RECOVER
        USetStatusCode(500)
      END SEQUENCE
    ENDIF
  ELSE
    USetStatusCode(404)
  ENDIF
RETURN .T.


STATIC FUNC ParseRequestHeader(cRequest)
LOCAL aRequest, aLine, nI, nJ, cI, nK, nContentLength := 0

  aRequest := split(CR_LF, cRequest)
  aLine := split(" ", aRequest[1])

  server["REQUEST_ALL"] := aRequest[1]
  IF LEN(aLine) == 3 .AND. LEFT(aLine[3], 5) == "HTTP/"
    server["REQUEST_METHOD"] := aLine[1]
    server["REQUEST_URI"] := aLine[2]
    server["SERVER_PROTOCOL"] := aLine[3]
  ELSE
    server["REQUEST_METHOD"] := aLine[1]
    server["REQUEST_URI"] := IIF(LEN(aLine) >= 2, aLine[2], "")
    server["SERVER_PROTOCOL"] := IIF(LEN(aLine) >= 3, aLine[3], "")
    RETURN 0
  ENDIF

  // Fix invalid queries: bind to root
  IF ! (LEFT(server["REQUEST_URI"], 1) == "/")
    server["REQUEST_URI"] := "/" + server["REQUEST_URI"]
  ENDIF

  IF (nI := AT("?", server["REQUEST_URI"])) > 0
    server["SCRIPT_NAME"] := LEFT(server["REQUEST_URI"], nI - 1)
    server["QUERY_STRING"] := SUBSTR(server["REQUEST_URI"], nI + 1)
  ELSE
    server["SCRIPT_NAME"] := server["REQUEST_URI"]
    server["QUERY_STRING"] := ""
  ENDIF

  server["HTTP_ACCEPT"] := ""
  server["HTTP_ACCEPT_CHARSET"] := ""
  server["HTTP_ACCEPT_ENCODING"] := ""
  server["HTTP_ACCEPT_LANGUAGE"] := ""
  server["HTTP_CONNECTION"] := ""
  server["HTTP_HOST"] := ""
  server["HTTP_KEEP_ALIVE"] := ""
  server["HTTP_REFERER"] := ""
  server["HTTP_USER_AGENT"] := ""
  server["HTTP_CONTENT_TYPE"] := ""

  FOR nI := 2 TO LEN(aRequest)
    IF aRequest[nI] == "";  EXIT
    ELSEIF (nJ := AT(":", aRequest[nI])) > 0
      cI := ALLTRIM(SUBSTR(aRequest[nI], nJ + 1))
      SWITCH UPPER(LEFT(aRequest[nI], nJ - 1))
        CASE "COOKIE"
          IF (nK := AT(";", cI)) == 0
            nK := LEN(TRIM(cI))
          ENDIF
          cI := LEFT(cI, nK)
          IF (nK := AT("=", cI)) > 0
            /* cookie names are case insensitive, uppercase it */
            cookie[UPPER(LEFT(cI, nK - 1))] := SUBSTR(cI, nK + 1)
          ENDIF
          EXIT
        CASE "CONTENT-LENGTH"
          nContentLength := VAL(cI)
          EXIT
        OTHERWISE
          server["HTTP_" + STRTRAN(UPPER(LEFT(aRequest[nI], nJ - 1)), "-", "_")] := cI
          EXIT
      ENDSWITCH
    ENDIF
  NEXT
  IF !(server["QUERY_STRING"] == "")
    FOR EACH cI IN split("&", server["QUERY_STRING"])
      IF (nI := AT("=", cI)) > 0
        get[UUrlDecode(LEFT(cI, nI - 1))] := UUrlDecode(SUBSTR(cI, nI + 1))
      ELSE
        get[UUrlDecode(cI)] := NIL
      ENDIF
    NEXT
  ENDIF
  cRequest := SUBSTR(cRequest, AT(CR_LF + CR_LF, cRequest) + 4)
RETURN nContentLength


STATIC FUNC ParseRequestBody(cRequest)
LOCAL nI, cPart

  IF server["HTTP_CONTENT_TYPE"] == "application/x-www-form-urlencoded"
    FOR EACH cPart IN split("&", cRequest)
      IF (nI := AT("=", cPart)) > 0
        post[UUrlDecode(LEFT(cPart, nI - 1))] := UUrlDecode(SUBSTR(cPart, nI + 1))
      ELSE
        post[UUrlDecode(cPart)] := NIL
      ENDIF
    NEXT
  ENDIF
RETURN NIL


STATIC FUNC MakeResponse()
LOCAL cRet

  IF UGetHeader("Content-Type") == NIL
    UAddHeader("Content-Type", "text/html")
  ENDIF
  UAddHeader("Date", HttpDateFormat(HB_DATETIME()))

  cRet := IIF(server["SERVER_PROTOCOL"] == "HTTP/1.0", "HTTP/1.0 ", "HTTP/1.1 ")
  SWITCH s_nStatusCode
    CASE 200
      cRet += "200 OK"
      EXIT
    CASE 301
      cRet += "301 Moved Permanently"
      s_cResult := "<html><body><h1>301 Moved Permanently</h1></body></html>"
      EXIT
    CASE 302
      cRet += "302 Found"
      s_cResult := "<html><body><h1>302 Found</h1></body></html>"
      EXIT
    CASE 303
      cRet += "303 See Other"
      s_cResult := "<html><body><h1>303 See Other</h1></body></html>"
      EXIT
    CASE 304
      cRet += "304 Not Modified"
      s_cResult := "<html><body><h1>304 Not Modified</h1></body></html>"
      EXIT
    CASE 400
      cRet += "400 Bad Request"
      s_cResult := "<html><body><h1>400 Bad Request</h1></body></html>"
      UAddHeader("Connection", "close")
      EXIT
    CASE 401
      cRet += "401 Unauthorized"
      s_cResult := "<html><body><h1>401 Unauthorized</h1></body></html>"
      EXIT
    CASE 402
      cRet += "402 Payment Required"
      s_cResult := "<html><body><h1>402 Payment Required</h1></body></html>"
      EXIT
    CASE 403
      cRet += "403 Forbidden"
      s_cResult := "<html><body><h1>403 Forbidden</h1></body></html>"
      EXIT
    CASE 404
      cRet += "404 Not Found"
      s_cResult := "<html><body><h1>404 Not Found</h1></body></html>"
      EXIT
    CASE 412
      cRet += "412 Precondition Failed"
      s_cResult := "<html><body><h1>412 Precondition Failed</h1></body></html>"
      EXIT
    CASE 500
      cRet += "500 Internal Server Error"
      s_cResult := "<html><body><h1>500 Internal Server Error</h1></body></html>"
      EXIT
    CASE 501
      cRet += "501 Not Implemented"
      s_cResult := "<html><body><h1>501 Not Implemented</h1></body></html>"
      UAddHeader("Connection", "close")
      EXIT
    OTHERWISE
      cRet += "500 Internal Server Error"
      s_cResult := "<html><body><h1>500 Internal Server Error</h1></body></html>"
      UAddHeader("Connection", "close")
  ENDSWITCH
  cRet += CR_LF
  UAddHeader("Content-Length", LTRIM(STR(LEN(s_cResult))))
  AEVAL(s_aHeader, {|x| cRet += x[1] + ": " + x[2] + CR_LF})
  cRet += CR_LF
  ? cRet
  cRet += s_cResult
RETURN cRet


STATIC PROC SendResponse(oServer, hSocket)
LOCAL cSend, nLen, cBuf
  cSend := MakeResponse()

//  ? cSend

  DO WHILE LEN(cSend) > 0
    IF (nLen := socket_send(hSocket, cSend)) == -1
      ? "send() error:", socket_error(), hSocket
      EXIT
    ELSEIF nLen > 0
      cSend := SUBSTR(cSend, nLen + 1)
    ENDIF
  ENDDO
  oServer:LogAccess()
RETURN


STATIC FUNC HttpDateFormat(tDate)
RETURN {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}[DOW(tDate)] + ", " + ;
       PADL(DAY(tDate), 2, "0") + " " + ;
       {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"}[MONTH(tDate)] + ;
       " " + PADL(YEAR(tDate), 4, "0") + HB_TTOC(tDate, "", "HH:MM:SS") + " GMT" // TOFIX: time zone


STATIC FUNC HttpDateUnformat(cDate, tDate)
LOCAL nMonth
  // TODO: support outdated compatibility format RFC2616
  IF LEN(cDate) == 29 .AND. RIGHT(cDate, 4) == " GMT" .AND. SUBSTR(cDate, 4, 2) == ", "
    nMonth := ASCAN({"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", ;
                     "Oct", "Nov", "Dec"}, SUBSTR(cDate, 9, 3))
    IF nMonth > 0
      tDate := HB_STOT(SUBSTR(cDate, 13, 4) + PADL(nMonth, 2, "0") + SUBSTR(cDate, 6, 2) + ;
                       STRTRAN(SUBSTR(cDate, 18, 8), ":", ""))
      RETURN ! EMPTY(tDate)
    ENDIF
  ENDIF
RETURN .F.


STATIC FUNC UErrorHandler(oErr, oServer)
  IF     oErr:genCode == EG_ZERODIV;  RETURN 0
  ELSEIF oErr:genCode == EG_LOCK;     RETURN .T.
  ELSEIF (oErr:genCode == EG_OPEN .AND. oErr:osCode == 32 .OR. ;
          oErr:genCode == EG_APPENDLOCK) .AND. oErr:canDefault
    NETERR(.T.)
    RETURN .F.
  ENDIF
  oServer:LogError(GetErrorDesc(oErr))
  BREAK(oErr)
RETURN NIL


STATIC FUNC GetErrorDesc(oErr)
LOCAL cRet, nI
  cRet := "ERRORLOG ============================================================" + HB_OSNewLine() + ;
          "Error: " + oErr:subsystem + "/" + ErrDescCode(oErr:genCode) + "(" + LTRIM(STR(oErr:genCode)) + ") " + ;
                      LTRIM(STR(oErr:subcode)) + HB_OSNewLine()
  IF !EMPTY(oErr:filename);      cRet += "File: " + oErr:filename + HB_OSNewLine()
  ENDIF
  IF !EMPTY(oErr:description);   cRet += "Description: " + oErr:description + HB_OSNewLine()
  ENDIF
  IF !EMPTY(oErr:operation);     cRet += "Operacija: " + oErr:operation + HB_OSNewLine()
  ENDIF
  IF !EMPTY(oErr:osCode);        cRet += "OS error: " + LTRIM(STR(oErr:osCode)) + HB_OSNewLine()
  ENDIF
  IF VALTYPE(oErr:args) == "A"
    cRet += "Arguments:" + HB_OSNewLine()
    AEVAL(oErr:args, {|X, Y| cRet += STR(Y, 5) + ": " + HB_CStr(X) + HB_OSNewLine()})
  ENDIF
  cRet += HB_OSNewLine()

  cRet += "Stack:" + HB_OSNewLine()
  nI := 2
  DO WHILE ! EMPTY(PROCNAME(++nI))
    cRet += "    " + PROCNAME(nI) + "(" + LTRIM(STR(PROCLINE(nI))) + ")" + HB_OSNewLine()
  ENDDO
  cRet += HB_OSNewLine()

  cRet += "Executable:  " + HB_PROGNAME() + HB_OSNewLine()
  cRet += "Versions:" + HB_OSNewLine()
  cRet += "  OS: " + OS() + HB_OSNewLine()
  cRet += "  Harbour: " + VERSION() + ", " + HB_BUILDDATE() + HB_OSNewLine()
  cRet += HB_OSNewLine()

  IF oErr:genCode != EG_MEM
    cRet += "Database areas:" + HB_OSNewLine()
    cRet += "    Current: " + LTRIM(STR(SELECT())) + "  " + ALIAS() + HB_OSNewLine()

    BEGIN SEQUENCE WITH {|o| BREAK(o)}
      IF !EMPTY(ALIAS())
        cRet += "    Filter: " + DBFILTER() + HB_OSNewLine()
        cRet += "    Relation: " + DBRELATION() + HB_OSNewLine()
        cRet += "    Index expression: " + ORDKEY(ORDSETFOCUS()) + HB_OSNewLine()
        cRet += HB_OSNewLine()
        BEGIN SEQUENCE WITH {|o| BREAK(o)}
          FOR nI := 1 to FCOUNT()
            cRet += STR(nI, 6) + " " + PADR(FIELDNAME(nI), 14) + ": " + HB_VALTOEXP(FIELDGET(nI)) + HB_OSNewLine()
          NEXT
        RECOVER
          cRet += "!!! Error reading database fields !!!" + HB_OSNewLine()
        END SEQUENCE
        cRet += HB_OSNewLine()
      ENDIF
    RECOVER
      cRet += "!!! Error accessing current workarea !!!" + HB_OSNewLine()
    END SEQUENCE

    FOR nI := 1 to 250
      BEGIN SEQUENCE WITH {|o| BREAK(o)}
        IF ! EMPTY(ALIAS(nI))
          DBSELECTAREA(nI)
          cRet += STR(nI, 6) + " " + RDDNAME() + " " + PADR(ALIAS(), 15) + ;
                  STR(RECNO()) + "/" + STR(LASTREC()) + ;
                  IIF(EMPTY(ORDSETFOCUS()), "", " Index " + ORDSETFOCUS() + "(" + LTRIM(STR(ORDNUMBER())) + ")") + HB_OSNewLine()
          DBCLOSEAREA()
        ENDIF
      RECOVER
        cRet += "!!! Error accessing workarea number: " + STR(nI, 4) + "!!!" + HB_OSNewLine()
      END SEQUENCE
    NEXT
    cRet += HB_OSNewLine()
  ENDIF
RETURN cRet


STATIC FUNC ErrDescCode(nCode)
LOCAL cI := NIL
  IF nCode > 0 .AND. nCode <= 41
    cI := {"ARG"     , "BOUND"    , "STROVERFLOW", "NUMOVERFLOW", "ZERODIV" , "NUMERR"     , "SYNTAX"  , "COMPLEXITY" , ; //  1,  2,  3,  4,  5,  6,  7,  8
           NIL       , NIL        , "MEM"        , "NOFUNC"     , "NOMETHOD", "NOVAR"      , "NOALIAS" , "NOVARMETHOD", ; //  9, 10, 11, 12, 13, 14, 15, 16
           "BADALIAS", "DUPALIAS" , NIL          , "CREATE"     , "OPEN"    , "CLOSE"      , "READ"    , "WRITE"      , ; // 17, 18, 19, 20, 21, 22, 23, 24
           "PRINT"   , NIL        , NIL          , NIL          , NIL       , "UNSUPPORTED", "LIMIT"   , "CORRUPTION" , ; // 25, 26 - 29, 30, 31, 32
           "DATATYPE", "DATAWIDTH", "NOTABLE"    , "NOORDER"    , "SHARED"  , "UNLOCKED"   , "READONLY", "APPENDLOCK" , ; // 33, 34, 35, 36, 37, 38, 39, 40
           "LOCK"    }[nCode]                                                                                            // 41
  ENDIF
RETURN IF(cI == NIL, "", "EG_" + cI)


/********************************************************************
  Public functions
********************************************************************/
PROC USetStatusCode(nStatusCode)
  s_nStatusCode := nStatusCode
RETURN


FUNC UGetHeader(cType)
LOCAL nI

  IF (nI := ASCAN(s_aHeader, {|x| UPPER(x[1]) == UPPER(cType)})) > 0
    RETURN s_aHeader[nI, 2]
  ENDIF
RETURN NIL


PROC UAddHeader(cType, cValue)
LOCAL nI

  IF (nI := ASCAN(s_aHeader, {|x| UPPER(x[1]) == UPPER(cType)})) > 0
    s_aHeader[nI, 2] := cValue
  ELSE
    AADD(s_aHeader, {cType, cValue})
  ENDIF
RETURN


PROC URedirect(cURL, nCode)
  IF nCode == NIL;  nCode := 303
  ENDIF
  USetStatusCode(nCode)
  UAddHeader("Location", cURL)
RETURN


PROC USessionDestroy()
  s_lSessionDestroy := .T.
RETURN


PROC UWrite(cString)
  s_cResult += cString
RETURN


FUNC UOsFileName(cFileName)
  IF HB_OSPathSeparator() != "/"
    RETURN STRTRAN(cFileName, "/", HB_OSPathSeparator())
  ENDIF
RETURN cFileName


FUNC UHtmlEncode(cString)
LOCAL nI, cI, cRet := ""

  FOR nI := 1 TO LEN(cString)
    cI := SUBSTR(cString, nI, 1)
    IF cI == "<"
      cRet += "&lt;"
    ELSEIF cI == ">"
      cRet += "&gt;"
    ELSEIF cI == "&"
      cRet += "&amp;"
    ELSEIF cI == '"'
      cRet += "&quot;"
    ELSE
      cRet += cI
    ENDIF
  NEXT
RETURN cRet


FUNC UUrlEncode(cString)
LOCAL nI, cI, cRet := ""

  FOR nI := 1 TO LEN(cString)
    cI := SUBSTR(cString, nI, 1)
    IF cI == " "
      cRet += "+"
    ELSEIF ASC(cI) >= 127 .OR. ASC(cI) <= 31 .OR. cI $ '=&%+'
      cRet += "%" + HB_StrToHex(cI)
    ELSE
      cRet += cI
    ENDIF
  NEXT
RETURN cRet


FUNC UUrlDecode(cString)
LOCAL nI
  cString := STRTRAN(cString, "+", " ")
  nI := 1
  DO WHILE nI <= LEN(cString)
    nI := HB_AT("%", cString, nI)
    IF nI == 0;  EXIT
    ENDIF
    IF UPPER(SUBSTR(cString, nI + 1, 1)) $ "0123456789ABCDEF" .AND. ;
       UPPER(SUBSTR(cString, nI + 2, 1)) $ "0123456789ABCDEF"
      cString := STUFF(cString, nI, 3, HB_HexToStr(SUBSTR(cString, nI + 1, 2)))
    ENDIF
    nI++
  ENDDO
RETURN cString


FUNC ULink(cText, cURL)
RETURN '<a href="' + cURL + '">' + UHtmlEncode(cText) + '</a>'


PROC UProcFiles(cFileName, lIndex)
LOCAL aDir, aF, nI, cI, tDate, tHDate

  IF lIndex == NIL;  lIndex := .F.
  ENDIF

  cFileName := STRTRAN(cFileName, "//", "/")

  // Security
  IF "/../" $ cFileName
    USetStatusCode(403)
    RETURN
  ENDIF

  IF HB_FileExists(uOSFileName(cFileName))
    IF HB_HHasKey(server, "HTTP_IF_MODIFIED_SINCE") .AND. ;
       HttpDateUnformat(server["HTTP_IF_MODIFIED_SINCE"], @tHDate) .AND. ;
       HB_FGETDATETIME(UOsFileName(cFileName), @tDate) .AND. ;
       ( tDate <= tHDate )
      USetStatusCode(304)
    ELSEIF HB_HHasKey(server, "HTTP_IF_UNMODIFIED_SINCE") .AND. ;
       HttpDateUnformat(server["HTTP_IF_UNMODIFIED_SINCE"], @tHDate) .AND. ;
       HB_FGETDATETIME(UOsFileName(cFileName), @tDate) .AND. ;
       ( tDate > tHDate )
      USetStatusCode(412)
    ELSE
      IF (nI := RAT(".", cFileName)) > 0
        SWITCH LOWER(SUBSTR(cFileName, nI + 1))
          CASE "css";                                 cI := "text/css";  EXIT
          CASE "htm";   CASE "html";                  cI := "text/html";  EXIT
          CASE "txt";   CASE "text";  CASE "asc"
          CASE "c";     CASE "h";     CASE "cpp"
          CASE "hpp";   CASE "log";                   cI := "text/plain";  EXIT
          CASE "rtf";                                 cI := "text/rtf";  EXIT
          CASE "xml";                                 cI := "text/xml";  EXIT
          CASE "bmp";                                 cI := "image/bmp";  EXIT
          CASE "gif";                                 cI := "image/gif";  EXIT
          CASE "jpg";   CASE "jpe";   CASE "jpeg";    cI := "image/jpeg";  EXIT
          CASE "png";                                 cI := "image/png";   EXIT
          CASE "tif";   CASE "tiff";                  cI := "image/tiff";  EXIT
          CASE "djv";   CASE "djvu";                  cI := "image/vnd.djvu";  EXIT
          CASE "ico";                                 cI := "image/x-icon";  EXIT
          CASE "xls";                                 cI := "application/excel";  EXIT
          CASE "doc";                                 cI := "application/msword";  EXIT
          CASE "pdf";                                 cI := "application/pdf";  EXIT
          CASE "ps";    CASE "eps";                   cI := "application/postscript";  EXIT
          CASE "ppt";                                 cI := "application/powerpoint";  EXIT
          CASE "bz2";                                 cI := "application/x-bzip2";  EXIT
          CASE "gz";                                  cI := "application/x-gzip";  EXIT
          CASE "tgz";                                 cI := "application/x-gtar";  EXIT
          CASE "js";                                  cI := "application/x-javascript";  EXIT
          CASE "tar";                                 cI := "application/x-tar";  EXIT
          CASE "tex";                                 cI := "application/x-tex";  EXIT
          CASE "zip";                                 cI := "application/zip";  EXIT
          CASE "midi";                                cI := "audio/midi";  EXIT
          CASE "mp3";                                 cI := "audio/mpeg";  EXIT
          CASE "wav";                                 cI := "audio/x-wav";  EXIT
          CASE "qt";    CASE "mov";                   cI := "video/quicktime";  EXIT
          CASE "avi";                                 cI := "video/x-msvideo";  EXIT
          OTHERWISE
            cI := "application/octet-stream"
        ENDSWITCH
      ELSE
        cI := "application/octet-stream"
      ENDIF
      UAddHeader("Content-Type", cI)

      IF HB_FGETDATETIME(UOsFileName(cFileName), @tDate)
        UAddHeader("Last-Modified", HttpDateFormat(tDate))
      ENDIF

      UWrite(HB_MEMOREAD(UOsFileName(cFileName)))
    ENDIF
  ELSEIF HB_DirExists(UOsFileName(cFileName))
    IF RIGHT(cFileName, 1) != "/"
       URedirect("http://" + server["HTTP_HOST"] + server["SCRIPT_NAME"] + "/")
       RETURN
    ENDIF
    IF (nI := ASCAN({"index.html", "index.htm"}, ;
                    {|x| IIF(HB_FileExists(UOSFileName(cFileName + X)), (cFileName += X, .T.), .F.)})) > 0
      UAddHeader("Content-Type", "text/html")
      UWrite(HB_MEMOREAD(UOsFileName(cFileName)))
      RETURN
    ENDIF
    IF ! lIndex
       USetStatusCode(403)
       RETURN
    ENDIF

    UAddHeader("Content-Type", "text/html")

    aDir := DIRECTORY(UOsFileName(cFileName), "D")
    IF HB_HHasKey(get, "s")
      IF get["s"] == "s"
        ASORT(aDir,,, {|X,Y| IF(X[5] == "D", IF(Y[5] == "D", X[1] < Y[1], .T.), ;
                                IF(Y[5] == "D", .F., X[2] < Y[2]))})
      ELSEIF get["s"] == "m"
        ASORT(aDir,,, {|X,Y| IF(X[5] == "D", IF(Y[5] == "D", X[1] < Y[1], .T.), ;
                                IF(Y[5] == "D", .F., DTOS(X[3]) + X[4] < DTOS(Y[3]) + Y[4]))})
      ELSE
        ASORT(aDir,,, {|X,Y| IF(X[5] == "D", IF(Y[5] == "D", X[1] < Y[1], .T.), ;
                                IF(Y[5] == "D", .F., X[1] < Y[1]))})
      ENDIF
    ELSE
      ASORT(aDir,,, {|X,Y| IF(X[5] == "D", IF(Y[5] == "D", X[1] < Y[1], .T.), ;
                              IF(Y[5] == "D", .F., X[1] < Y[1]))})
    ENDIF

    UWrite('<html><body><h1>Index of ' + server["SCRIPT_NAME"] + '</h1><pre>      ')
    UWrite('<a href="?s=n">Name</a>                                                  ')
    UWrite('<a href="?s=m">Modified</a>             ')
    UWrite('<a href="?s=s">Size</a>' + CR_LF + '<hr>')
    FOR EACH aF IN aDir
      IF LEFT(aF[1], 1) == "."
      ELSEIF "D" $ aF[5]
        UWrite('[DIR] <a href="' + aF[1] + '/">'+ aF[1] + '</a>' + SPACE(50 - LEN(aF[1])) + ;
               DTOC(aF[3]) + ' ' + aF[4] + CR_LF)
      ELSE
        UWrite('      <a href="' + aF[1] + '">'+ aF[1] + '</a>' + SPACE(50 - LEN(aF[1])) + ;
               DTOC(aF[3]) + ' ' + aF[4] + STR(aF[2], 12) + CR_LF)
      ENDIF
    NEXT
    UWrite("<hr></pre></body></html>")
  ELSE
    USetStatusCode(404)
  ENDIF
RETURN


PROC UProcInfo()
  UWrite('<h1>Info</h1>')

  UWrite('<h2>Platform</h2>')
  UWrite('<table border=1 cellspacing=0>')
  UWrite('<tr><td>OS</td><td>' + UHtmlEncode(OS()) + '</td></tr>')
  UWrite('<tr><td>Harbour</td><td>' + UHtmlEncode(VERSION()) + '</td></tr>')
  UWrite('<tr><td>Build date</td><td>' + UHtmlEncode(HB_BUILDDATE()) + '</td></tr>')
  UWrite('<tr><td>Compiler</td><td>' + UHtmlEncode(HB_COMPILER()) + '</td></tr>')
  UWrite('</table>')

  UWrite('<h2>Capabilities</h2>')
  UWrite('<table border=1 cellspacing=0>')
  UWrite('<tr><td>RDD</td><td>' + UHtmlEncode(join(", ", RDDLIST())) + '</td></tr>')
  UWrite('</table>')

  UWrite('<h2>Variables</h2>')

  UWrite('<h3>server</h3>')
  UWrite('<table border=1 cellspacing=0>')
  HB_HEval(server, {|k,v| UWrite('<tr><td>' + k + '</td><td>' + UHtmlEncode(HB_CStr(v)) + '</td></tr>')})
  UWrite('</table>')

  IF !EMPTY(get)
    UWrite('<h3>get</h3>')
    UWrite('<table border=1 cellspacing=0>')
    HB_HEval(get, {|k,v| UWrite('<tr><td>' + k + '</td><td>' + UHtmlEncode(HB_CStr(v)) + '</td></tr>')})
    UWrite('</table>')
  ENDIF

  IF !EMPTY(post)
    UWrite('<h3>post</h3>')
    UWrite('<table border=1 cellspacing=0>')
    HB_HEval(post, {|k,v| UWrite('<tr><td>' + k + '</td><td>' + UHtmlEncode(HB_CStr(v)) + '</td></tr>')})
    UWrite('</table>')
  ENDIF
RETURN
