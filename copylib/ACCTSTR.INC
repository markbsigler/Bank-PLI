 /*********************************************************************/
 /*  ACCTSTR.INC - Account Structure Definition                      */
 /*                                                                   */
 /*  Demonstrates:                                                   */
 /*    - UNION for overlaid type-specific structures                 */
 /*    - REFER for self-defining arrays                              */
 /*    - BASED storage with POINTER                                  */
 /*    - HANDLE for typed pointers                                   */
 /*    - OFFSET for area-relative addressing                         */
 /*    - Complex nested structures                                   */
 /*********************************************************************/

 DECLARE
   1 ACCOUNT_MASTER,
     2 ACCOUNT_NUM           ACCT_NUM_T,
     2 CUSTOMER_ID           CUST_NUM_T,
     2 SORT_CODE             SORT_CODE_T,
     2 ACCOUNT_TYPE          ACCT_TYPE_T ORDINAL,
     2 ACCOUNT_STATUS,
       3 ACTIVE              BIT(1),
       3 FROZEN              BIT(1),
       3 DORMANT             BIT(1),
       3 CLOSED              BIT(1),
       3 RESTRICTED          BIT(1),
       3 RESERVED_FLAGS      BIT(3),
     2 BALANCES,
       3 CURRENT_BALANCE     CURRENCY_T,
       3 AVAILABLE_BALANCE   CURRENCY_T,
       3 PENDING_DEBITS      CURRENCY_T,
       3 PENDING_CREDITS     CURRENCY_T,
       3 HELD_AMOUNT         CURRENCY_T,
       3 RESERVED_AMOUNT     CURRENCY_T,
     2 DATES,
       3 OPENED_DATE         DATE_T,
       3 LAST_STATEMENT_DT   DATE_T,
       3 NEXT_STATEMENT_DT   DATE_T,
       3 LAST_INTEREST_DT    DATE_T,
       3 MATURITY_DATE       DATE_T,
       3 CLOSED_DATE         DATE_T,
     2 RATES_AND_LIMITS,
       3 INTEREST_RATE       RATE_T,
       3 PENALTY_RATE        RATE_T,
       3 OVERDRAFT_LIMIT     CURRENCY_T,
       3 DAILY_DEBIT_LIMIT   CURRENCY_T,
       3 DAILY_CREDIT_LIMIT  CURRENCY_T,
       3 WIRE_TRANSFER_LIMIT CURRENCY_T,

     /* UNION - Type-specific data overlaid in same storage */
     2 TYPE_SPECIFIC_DATA    UNION,
       
       /* Checking Account Specifics */
       3 CHECKING,
         4 LAST_CHECK_NUM    FIXED BIN(31),
         4 CHECK_REORDER_AT  FIXED BIN(31),
         4 CHECKS_CLEARED    FIXED BIN(31),
         4 CHECKS_PENDING    FIXED BIN(31),
         4 DEBIT_CARD_NUM    CHAR(16),
         4 DEBIT_CARD_EXP    CHAR(6),           /* YYYYMM */
         4 ATM_DAILY_LIMIT   CURRENCY_T,
         4 MONTHLY_FEE       CURRENCY_T,
         4 MIN_BALANCE_REQ   CURRENCY_T,

       /* Savings Account Specifics */
       3 SAVINGS,
         4 INTEREST_TIER     FIXED BIN(15),     /* Tier 1-5 */
         4 COMPOUND_FREQ     FIXED BIN(15),     /* Times per year */
         4 WITHDRAWALS_MTD   FIXED BIN(15),
         4 WITHDRAWAL_LIMIT  FIXED BIN(15),     /* Per month */
         4 EXCESS_WD_FEE     CURRENCY_T,
         4 QUARTERLY_BONUS   RATE_T,
         4 TARGET_BALANCE    CURRENCY_T,
         4 AUTO_TRANSFER     BIT(1),

       /* ISA (Tax-Free Savings) Specifics */
       3 ISA,
         4 TAX_YEAR          CHAR(9),           /* 2025/2026 */
         4 ANNUAL_LIMIT      CURRENCY_T,
         4 CONTRIBUTIONS_YTD CURRENCY_T,
         4 WITHDRAWALS_YTD   CURRENCY_T,
         4 TRANSFERRED_IN    CURRENCY_T,
         4 PREVIOUS_YEAR_BAL CURRENCY_T,
         4 LOCK_IN_RATE      RATE_T,
         4 TERM_MONTHS       FIXED BIN(31),

       /* Mortgage Account Specifics */
       3 MORTGAGE,
         4 PRINCIPAL_AMOUNT  CURRENCY_T,
         4 ORIGINAL_AMOUNT   CURRENCY_T,
         4 TERM_MONTHS       FIXED BIN(31),
         4 MONTHS_REMAINING  FIXED BIN(31),
         4 MONTHLY_PAYMENT   CURRENCY_T,
         4 ESCROW_BALANCE    CURRENCY_T,
         4 PROPERTY_VALUE    CURRENCY_T,
         4 LTV_RATIO         FLOAT DEC(6),      /* Loan-to-Value */
         4 PROPERTY_ADDRESS  CHAR(100) VARYING,
         4 PAYMENT_COUNT     FIXED BIN(31),
         4 LATE_PAYMENTS     FIXED BIN(31),
         /* REFER - self-defining array */
         4 SCHEDULE_ENTRIES  FIXED BIN(31),
         4 AMORTIZATION(MORTGAGE.SCHEDULE_ENTRIES REFER),
           5 PAYMENT_NUM     FIXED BIN(31),
           5 PAYMENT_DATE    DATE_T,
           5 PAYMENT_AMT     CURRENCY_T,
           5 PRINCIPAL_PAID  CURRENCY_T,
           5 INTEREST_PAID   CURRENCY_T,
           5 BALANCE_REMAIN  CURRENCY_T,

       /* Loan Account Specifics */
       3 LOAN,
         4 LOAN_TYPE         CHAR(20) VARYING,  /* Auto, Personal, etc. */
         4 PRINCIPAL_AMOUNT  CURRENCY_T,
         4 ORIGINAL_AMOUNT   CURRENCY_T,
         4 TERM_MONTHS       FIXED BIN(31),
         4 MONTHS_REMAINING  FIXED BIN(31),
         4 MONTHLY_PAYMENT   CURRENCY_T,
         4 ORIGINATION_FEE   CURRENCY_T,
         4 PAYOFF_AMOUNT     CURRENCY_T,
         4 PREPAYMENT_PENALTY CURRENCY_T,
         4 COLLATERAL_DESC   CHAR(100) VARYING,
         4 COLLATERAL_VALUE  CURRENCY_T,

     /* Transaction history with REFER */
     2 HISTORY_COUNT         FIXED BIN(31),
     2 TRANSACTION_HISTORY(ACCOUNT_MASTER.HISTORY_COUNT REFER),
       3 TXN_DATE            DATE_T,
       3 TXN_TIME            TIME_T,
       3 TXN_TYPE            TXN_TYPE_T ORDINAL,
       3 TXN_AMOUNT          CURRENCY_T,
       3 TXN_BALANCE         CURRENCY_T,
       3 TXN_REFERENCE       CHAR(20) VARYING,
       3 TXN_DESCRIPTION     CHAR(80) VARYING,

     /* Linked list of joint account holders - BASED with POINTER */
     2 JOINT_HOLDERS,
       3 HOLDER_COUNT        FIXED BIN(15),
       3 PRIMARY_HOLDER      CUST_NUM_T,
       3 HOLDER_LIST_PTR     POINTER,           /* Points to first node */

     2 AUDIT_INFO,
       3 CREATED_BY          CHAR(8),
       3 CREATED_DATE        TIMESTAMP_T,
       3 UPDATED_BY          CHAR(8),
       3 UPDATED_DATE        TIMESTAMP_T,
       3 UPDATE_COUNT        FIXED BIN(31);

 /* BASED structure for joint holder linked list */
 DECLARE
   1 JOINT_HOLDER_NODE BASED(HOLDER_PTR),
     2 CUSTOMER_ID           CUST_NUM_T,
     2 HOLDER_TYPE           CHAR(1),           /* P/J/S = Primary/Joint/Secondary */
     2 AUTHORITY_LEVEL       CHAR(1),           /* F/P = Full/Partial */
     2 ADDED_DATE            DATE_T,
     2 NEXT_HOLDER_PTR       POINTER;

 DECLARE HOLDER_PTR POINTER;

 /* HANDLE - typed pointer to account structure */
 DECLARE ACCT_HANDLE TYPE(HANDLE ACCOUNT_MASTER);
 DECLARE CURRENT_ACCT HANDLE ACCOUNT_MASTER;

 /* Work areas */
 DECLARE 1 ACCOUNT_WORK_AREA LIKE ACCOUNT_MASTER;
 DECLARE 1 ACCOUNT_BACKUP LIKE ACCOUNT_MASTER;

 /* Buffer for I/O with DEFINED overlay */
 DECLARE ACCOUNT_BUFFER CHAR(8192);
 DECLARE 1 ACCOUNT_OVERLAY DEFINED ACCOUNT_BUFFER LIKE ACCOUNT_MASTER;

 /* Account key structures */
 DECLARE
   1 ACCOUNT_PRIMARY_KEY,
     2 SORT_CODE             SORT_CODE_T,
     2 ACCOUNT_NUM           ACCT_NUM_T;

 DECLARE
   1 ACCOUNT_CUSTOMER_KEY,
     2 CUSTOMER_ID           CUST_NUM_T,
     2 ACCOUNT_NUM           ACCT_NUM_T;

 /* Array demonstration - account registry */
 DECLARE ACCOUNT_REGISTRY(10000) TYPE HANDLE ACCOUNT_MASTER;

 /* OFFSET for area-relative addressing */
 DECLARE ACCOUNT_AREA AREA(32768);
 DECLARE ACCOUNT_OFFSET OFFSET(ACCOUNT_AREA);
