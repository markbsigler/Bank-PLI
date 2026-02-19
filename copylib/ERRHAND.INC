 /*********************************************************************/
 /*  ERRHAND.INC - Standard Error Handlers                           */
 /*                                                                   */
 /*  Demonstrates:                                                   */
 /*    - %PROCEDURE / %END for macro-style code generation           */
 /*    - ON-conditions for all major exception types                 */
 /*    - ONCODE, ONFILE, ONKEY, ONSOURCE, ONCHAR interrogation       */
 /*    - SIGNAL, REVERT                                              */
 /*    - Nested ON-condition scoping                                 */
 /*********************************************************************/

 /* Macro to generate standard error logging */
 %DECLARE LOG_ERROR ENTRY;
 %ACTIVATE LOG_ERROR;

 %LOG_ERROR: PROCEDURE(ERROR_TYPE, ERROR_MSG);
   PUT SKIP LIST('ERROR [' || ERROR_TYPE || ']: ' || ERROR_MSG);
   PUT SKIP LIST('  ONCODE: ' || TRIM(ONCODE()));
   %IF DEBUG %THEN
     PUT SKIP LIST('  Location: ' || TRIM(ONLOC()));
   ;
 %END LOG_ERROR;

 /*-----------------------------------------------------------------*/
 /* Standard ON-condition Handlers                                  */
 /*-----------------------------------------------------------------*/

 /* Conversion Errors - ONSOURCE/ONCHAR available */
 ON CONVERSION BEGIN;
   DECLARE CONV_SOURCE CHAR(100) VARYING;
   DECLARE CONV_CHAR CHAR(1);
   DECLARE CONV_VALUE CHAR(50) VARYING;
   
   CONV_SOURCE = ONSOURCE();
   CONV_CHAR = ONCHAR();
   
   PUT SKIP LIST('CONVERSION ERROR:');
   PUT SKIP LIST('  Source value: ' || CONV_SOURCE);
   PUT SKIP LIST('  Invalid char: ' || CONV_CHAR);
   PUT SKIP LIST('  ONCODE: ' || TRIM(ONCODE()));
   
   /* Data cleansing - replace invalid char with '0' */
   ONSOURCE() = '0';
 END;

 /* Numeric Overflow */
 ON SIZE BEGIN;
   PUT SKIP LIST('SIZE CONDITION RAISED:');
   PUT SKIP LIST('  Arithmetic overflow detected');
   PUT SKIP LIST('  ONCODE: ' || TRIM(ONCODE()));
   SIGNAL ERROR;
 END;

 ON FIXEDOVERFLOW BEGIN;
   PUT SKIP LIST('FIXEDOVERFLOW:');
   PUT SKIP LIST('  Fixed-point overflow');
   PUT SKIP LIST('  ONCODE: ' || TRIM(ONCODE()));
   SIGNAL ERROR;
 END;

 ON OVERFLOW BEGIN;
   PUT SKIP LIST('OVERFLOW:');
   PUT SKIP LIST('  Generic overflow condition');
   PUT SKIP LIST('  ONCODE: ' || TRIM(ONCODE()));
   SIGNAL ERROR;
 END;

 /* Division by Zero */
 ON ZERODIVIDE BEGIN;
   PUT SKIP LIST('ZERODIVIDE:');
   PUT SKIP LIST('  Attempted division by zero');
   PUT SKIP LIST('  ONCODE: ' || TRIM(ONCODE()));
   SIGNAL ERROR;
 END;

 /* Range Violations */
 ON STRINGRANGE BEGIN;
   PUT SKIP LIST('STRINGRANGE:');
   PUT SKIP LIST('  String index out of bounds');
   PUT SKIP LIST('  ONCODE: ' || TRIM(ONCODE()));
   %IF DEBUG %THEN
     SIGNAL ERROR;
   %ELSE
     ;  /* Ignore in production */
   ;
 END;

 ON STRINGSIZE BEGIN;
   PUT SKIP LIST('STRINGSIZE:');
   PUT SKIP LIST('  String assignment truncated');
   PUT SKIP LIST('  ONCODE: ' || TRIM(ONCODE()));
   /* Continue execution - truncation allowed */
 END;

 ON SUBSCRIPTRANGE BEGIN;
   PUT SKIP LIST('SUBSCRIPTRANGE:');
   PUT SKIP LIST('  Array subscript out of bounds');
   PUT SKIP LIST('  ONCODE: ' || TRIM(ONCODE()));
   SIGNAL ERROR;
 END;

 /* File I/O Errors - ONFILE/ONKEY available */
 ON KEY(CUSTFILE) BEGIN;
   DECLARE KEY_VALUE CHAR(50) VARYING;
   DECLARE FILE_NAME CHAR(20) VARYING;
   
   KEY_VALUE = ONKEY();
   FILE_NAME = ONFILE();
   
   PUT SKIP LIST('KEY CONDITION:');
   PUT SKIP LIST('  File: ' || FILE_NAME);
   PUT SKIP LIST('  Key: ' || KEY_VALUE);
   PUT SKIP LIST('  ONCODE: ' || TRIM(ONCODE()));
   
   SELECT (ONCODE());
     WHEN (51)  /* Duplicate key */
       SIGNAL DUPLICATE_KEY;
     WHEN (52)  /* No record found */
       SIGNAL INVALID_ACCOUNT;
     OTHERWISE
       SIGNAL ERROR;
   END;
 END;

 ON KEY(ACCTFILE) BEGIN;
   DECLARE KEY_VALUE CHAR(50) VARYING;
   KEY_VALUE = ONKEY();
   
   PUT SKIP LIST('ACCTFILE KEY CONDITION:');
   PUT SKIP LIST('  Key: ' || KEY_VALUE);
   PUT SKIP LIST('  ONCODE: ' || TRIM(ONCODE()));
   
   IF ONCODE() = 52 THEN
     SIGNAL INVALID_ACCOUNT;
   ELSE
     SIGNAL ERROR;
 END;

 ON RECORD BEGIN;
   DECLARE FILE_NAME CHAR(20) VARYING;
   FILE_NAME = ONFILE();
   
   PUT SKIP LIST('RECORD CONDITION:');
   PUT SKIP LIST('  File: ' || FILE_NAME);
   PUT SKIP LIST('  Record format error');
   PUT SKIP LIST('  ONCODE: ' || TRIM(ONCODE()));
   SIGNAL ERROR;
 END;

 ON ENDFILE(CUSTFILE) BEGIN;
   PUT SKIP LIST('End of CUSTFILE reached');
   EOF_CUST = '1'B;
 END;

 ON ENDFILE(ACCTFILE) BEGIN;
   PUT SKIP LIST('End of ACCTFILE reached');
   EOF_ACCT = '1'B;
 END;

 ON ENDFILE(TXNFILE) BEGIN;
   EOF_TXN = '1'B;
 END;

 ON ENDPAGE(REPORT) BEGIN;
   PAGE_NUM = PAGE_NUM + 1;
   PUT FILE(REPORT) PAGE;
   PUT FILE(REPORT) SKIP(2) EDIT
     ('Banking System Report',
      COLUMN(50), 'Page: ', PAGE_NUM)
     (A, A, F(5));
   PUT FILE(REPORT) SKIP(2);
 END;

 /* Storage Management Errors */
 ON STORAGE BEGIN;
   PUT SKIP LIST('STORAGE:');
   PUT SKIP LIST('  Insufficient storage for allocation');
   PUT SKIP LIST('  ONCODE: ' || TRIM(ONCODE()));
   SIGNAL ERROR;
 END;

 ON AREA BEGIN;
   PUT SKIP LIST('AREA:');
   PUT SKIP LIST('  Area overflow - insufficient space');
   PUT SKIP LIST('  ONCODE: ' || TRIM(ONCODE()));
   SIGNAL ERROR;
 END;

 /* Generic Error Handler */
 ON ERROR BEGIN;
   PUT SKIP LIST('ERROR CONDITION:');
   PUT SKIP LIST('  ONCODE: ' || TRIM(ONCODE()));
   %IF DEBUG %THEN %DO;
     PUT SKIP LIST('  ONLOC: ' || TRIM(ONLOC()));
     /* Generate SNAP dump */
     PUT SKIP LIST('*** SNAP DUMP REQUESTED ***');
   %END;
   CALL CLEANUP_AND_EXIT(ONCODE());
 END;

 /* Attention/Halt */
 ON ATTENTION BEGIN;
   PUT SKIP LIST('ATTENTION:');
   PUT SKIP LIST('  User interrupt detected');
   PUT SKIP LIST('  Initiating graceful shutdown...');
   CALL CLEANUP_AND_EXIT(999);
 END;

 /*-----------------------------------------------------------------*/
 /* Custom Business Condition Handlers                              */
 /*-----------------------------------------------------------------*/

 ON CONDITION(INSUFFICIENT_FUNDS) BEGIN;
   PUT SKIP LIST('Business Rule Violation: Insufficient Funds');
   PUT SKIP LIST('  Account: ' || CURRENT_ACCT_NUM);
   PUT SKIP LIST('  Attempted Amount: ' || ATTEMPTED_AMOUNT);
   PUT SKIP LIST('  Available Balance: ' || AVAILABLE_BAL);
   RETURN_CODE = RC_INSUFF_FUNDS;
   GOTO TRANSACTION_ROLLBACK;
 END;

 ON CONDITION(OVERDRAFT_LIMIT) BEGIN;
   PUT SKIP LIST('Business Rule Violation: Overdraft Limit Exceeded');
   PUT SKIP LIST('  Account: ' || CURRENT_ACCT_NUM);
   PUT SKIP LIST('  Requested Overdraft: ' || REQUESTED_OVERDRAFT);
   PUT SKIP LIST('  Overdraft Limit: ' || ACCT_OVERDRAFT_LIMIT);
   RETURN_CODE = RC_OVERDRAFT;
   GOTO TRANSACTION_ROLLBACK;
 END;

 ON CONDITION(INVALID_ACCOUNT) BEGIN;
   PUT SKIP LIST('Business Rule Violation: Invalid Account');
   PUT SKIP LIST('  Account Number: ' || CURRENT_ACCT_NUM);
   RETURN_CODE = RC_NOT_FOUND;
 END;

 ON CONDITION(DUPLICATE_KEY) BEGIN;
   PUT SKIP LIST('Business Rule Violation: Duplicate Key');
   PUT SKIP LIST('  An account with this key already exists');
   RETURN_CODE = RC_DUPLICATE;
 END;

 ON CONDITION(CREDIT_THRESHOLD) BEGIN;
   PUT SKIP LIST('Business Rule: Credit Score Below Threshold');
   PUT SKIP LIST('  Customer: ' || CURRENT_CUST_ID);
   PUT SKIP LIST('  Credit Score: ' || CREDIT_SCORE);
   PUT SKIP LIST('  Minimum Required: ' || MIN_CREDIT_SCORE);
   RETURN_CODE = RC_INVALID_DATA;
 END;

 ON CONDITION(SECURITY_VIOLATION) BEGIN;
   PUT SKIP LIST('SECURITY VIOLATION DETECTED');
   PUT SKIP LIST('  User: ' || USERID);
   PUT SKIP LIST('  Terminal: ' || TERMID);
   PUT SKIP LIST('  Timestamp: ' || TIMESTAMP);
   CALL LOG_SECURITY_EVENT();
   RETURN_CODE = RC_SECURITY_FAIL;
   GOTO EMERGENCY_EXIT;
 END;

 /*-----------------------------------------------------------------*/
 /* Preprocessor Procedure for Standard Error Response             */
 /*-----------------------------------------------------------------*/
 %STANDARD_ERROR_RESPONSE: PROCEDURE;
   PUT SKIP LIST('Standard error response initiated');
   PUT SKIP LIST('Rolling back transaction...');
   EXEC SQL ROLLBACK WORK;
   PUT SKIP LIST('Notifying operations...');
   CALL SEND_ALERT('ERROR', ONCODE());
 %END STANDARD_ERROR_RESPONSE;

 /* Variables referenced by handlers */
 DECLARE EOF_CUST BIT(1) INIT('0'B);
 DECLARE EOF_ACCT BIT(1) INIT('0'B);
 DECLARE EOF_TXN BIT(1) INIT('0'B);
 DECLARE PAGE_NUM FIXED BIN(31) INIT(0);
 DECLARE RETURN_CODE FIXED BIN(31);
 DECLARE CURRENT_ACCT_NUM ACCT_NUM_T;
 DECLARE CURRENT_CUST_ID CUST_NUM_T;
 DECLARE ATTEMPTED_AMOUNT CURRENCY_T;
 DECLARE AVAILABLE_BAL CURRENCY_T;
 DECLARE REQUESTED_OVERDRAFT CURRENCY_T;
 DECLARE ACCT_OVERDRAFT_LIMIT CURRENCY_T;
 DECLARE CREDIT_SCORE FIXED DEC(3);
 DECLARE MIN_CREDIT_SCORE FIXED DEC(3) INIT(600);
