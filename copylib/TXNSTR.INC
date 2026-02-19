 /*********************************************************************/
 /*  TXNSTR.INC - Transaction Structure Definition                   */
 /*                                                                   */
 /*  Demonstrates:                                                   */
 /*    - COMPLEX data type for FX pairs                              */
 /*    - OFFSET for area-relative transaction chains                 */
 /*    - PICTURE for formatted numeric output                        */
 /*    - Varying strings                                             */
 /*    - Multiple date/time formats                                  */
 /*********************************************************************/

 DECLARE
   1 TRANSACTION_LOG,
     2 TRANSACTION_ID        CHAR(16),          /* GUID */
     2 SORT_CODE             SORT_CODE_T,
     2 ACCOUNT_NUM           ACCT_NUM_T,
     2 CUSTOMER_ID           CUST_NUM_T,
     2 TXN_TYPE              TXN_TYPE_T ORDINAL,
     2 TXN_CODE              CHAR(3),
     2 TIMESTAMP_INFO,
       3 TXN_DATE            DATE_T,
       3 TXN_TIME            TIME_T,
       3 TXN_TIMESTAMP       TIMESTAMP_T,
       3 POSTED_DATE         DATE_T,
       3 EFFECTIVE_DATE      DATE_T,
       3 VALUE_DATE          DATE_T,
     2 AMOUNT_INFO,
       3 TXN_AMOUNT          CURRENCY_T,
       3 TXN_CURRENCY        CHAR(3),           /* ISO 4217 */
       3 LOCAL_AMOUNT        CURRENCY_T,
       3 LOCAL_CURRENCY      CHAR(3),
       /* COMPLEX type for FX rate representation */
       3 FX_RATE             COMPLEX FLOAT DEC(15),
       3 FX_SPREAD           FLOAT DEC(6),
     2 BALANCE_INFO,
       3 BALANCE_BEFORE      CURRENCY_T,
       3 BALANCE_AFTER       CURRENCY_T,
       3 AVAILABLE_BEFORE    CURRENCY_T,
       3 AVAILABLE_AFTER     CURRENCY_T,
     2 TRANSFER_INFO,
       3 COUNTER_PARTY_ACCT  ACCT_NUM_T,
       3 COUNTER_PARTY_SORT  SORT_CODE_T,
       3 COUNTER_PARTY_NAME  CHAR(60) VARYING,
       3 COUNTER_PARTY_REF   CHAR(35) VARYING,
       3 ORIGINATOR_REF      CHAR(35) VARYING,
     2 CHECK_INFO,
       3 CHECK_NUMBER        FIXED BIN(31),
       3 CHECK_DATE          DATE_T,
       3 PAYEE_NAME          CHAR(60) VARYING,
       3 CHECK_IMAGE_ID      CHAR(20),
     2 CARD_INFO,
       3 CARD_NUMBER_MASKED  CHAR(16),          /* ************1234 */
       3 CARD_TYPE           CHAR(20) VARYING,  /* Debit/Credit */
       3 MERCHANT_NAME       CHAR(60) VARYING,
       3 MERCHANT_ID         CHAR(15),
       3 MERCHANT_CATEGORY   CHAR(4),           /* MCC */
       3 TERMINAL_ID         CHAR(15),
       3 AUTH_CODE           CHAR(6),
     2 DESCRIPTION           CHAR(100) VARYING,
     2 NOTES                 CHAR(255) VARYING,
     2 CHANNEL_INFO,
       3 CHANNEL_TYPE        CHAR(20) VARYING,  /* ATM/Online/Branch/Phone */
       3 CHANNEL_TERMINAL_ID CHAR(8),            /* Renamed to avoid conflict with CARD_INFO.TERMINAL_ID */
       3 USER_ID             CHAR(8),
       3 IP_ADDRESS          CHAR(45),          /* IPv6 */
       3 SESSION_ID          CHAR(32),
     2 AUTHORIZATION,
       3 AUTH_REQUIRED       BIT(1),
       3 AUTH_STATUS         CHAR(1),           /* P/A/D = Pending/Approved/Denied */
       3 AUTHORIZED_BY       CHAR(8),
       3 AUTH_TIMESTAMP      TIMESTAMP_T,
       3 AUTH_LIMIT_USED     CURRENCY_T,
     2 FEES_AND_CHARGES,
       3 BASE_FEE            CURRENCY_T,
       3 PERCENT_FEE         CURRENCY_T,
       3 LATE_FEE            CURRENCY_T,
       3 OVERDRAFT_FEE       CURRENCY_T,
       3 WIRE_FEE            CURRENCY_T,
       3 TOTAL_FEES          CURRENCY_T,
     2 STATUS_INFO,
       3 TXN_STATUS          CHAR(1),           /* P/C/R/V = Pending/Complete/Reversed/Void */
       3 REVERSED            BIT(1),
       3 REVERSAL_ID         CHAR(16),
       3 REVERSAL_TIMESTAMP  TIMESTAMP_T,
       3 REVERSAL_REASON     CHAR(100) VARYING,
     2 TRAIL_INFO,
       3 BATCH_ID            CHAR(20),
       3 TRACE_NUMBER        CHAR(15),
       3 SETTLEMENT_DATE     DATE_T,
       3 POSTED_BY           CHAR(8),
       3 POSTED_TIMESTAMP    TIMESTAMP_T,
     /* OFFSET for area-relative chain */
     2 NEXT_TXN_OFFSET       OFFSET(TXN_AREA);

 /* Area for transaction chain */
 DECLARE TXN_AREA AREA(65536);

 /* Transaction display formats with PICTURE */
 DECLARE
   1 TRANSACTION_DISPLAY,
     2 TXN_AMT_DISPLAY       PICTURE '$ZZZ,ZZZ,ZZ9.99',
     2 TXN_DATE_DISPLAY      PICTURE '99/99/9999',
     2 TXN_TIME_DISPLAY      PICTURE '99:99:99',
     2 BALANCE_DISPLAY       PICTURE '$ZZZ,ZZZ,ZZ9.99CR',
     2 FEE_DISPLAY           PICTURE '$ZZ9.99';

 /* Work areas */
 DECLARE 1 TRANSACTION_WORK LIKE TRANSACTION_LOG;
 DECLARE 1 TRANSACTION_BACKUP LIKE TRANSACTION_LOG;

 /* Buffer for I/O */
 DECLARE TRANSACTION_BUFFER CHAR(4096);
 DECLARE 1 TRANSACTION_OVERLAY DEFINED TRANSACTION_BUFFER 
   LIKE TRANSACTION_LOG;

 /* Transaction key structures */
 DECLARE
   1 TRANSACTION_KEY,
     2 TRANSACTION_ID        CHAR(16);

 DECLARE
   1 TRANSACTION_ACCT_KEY,
     2 ACCOUNT_NUM           ACCT_NUM_T,
     2 TXN_DATE              DATE_T,
     2 TRANSACTION_ID        CHAR(16);

 /* Summary structure for reporting */
 DECLARE
   1 TRANSACTION_SUMMARY,
     2 ACCOUNT_NUM           ACCT_NUM_T,
     2 SUMMARY_DATE          DATE_T,
     2 DEBIT_COUNT           FIXED BIN(31),
     2 DEBIT_TOTAL           CURRENCY_T,
     2 CREDIT_COUNT          FIXED BIN(31),
     2 CREDIT_TOTAL          CURRENCY_T,
     2 FEE_COUNT             FIXED BIN(31),
     2 FEE_TOTAL             CURRENCY_T,
     2 OPENING_BALANCE       CURRENCY_T,
     2 CLOSING_BALANCE       CURRENCY_T;
