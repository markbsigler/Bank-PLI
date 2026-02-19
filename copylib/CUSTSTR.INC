 /*********************************************************************/
 /*  CUSTSTR.INC - Customer Structure Definition                     */
 /*                                                                   */
 /*  Demonstrates:                                                   */
 /*    - Nested STRUCTURE with level numbers                         */
 /*    - CHAR VARYING / NONVARYING                                   */
 /*    - PICTURE attribute for formatted fields                      */
 /*    - BIT flags                                                   */
 /*    - FIXED DEC and FIXED BIN types                               */
 /*    - LIKE attribute for derived structures                       */
 /*    - DEFINED overlay for raw I/O mapping                         */
 /*    - DIMENSION attribute on repeating elements                   */
 /*********************************************************************/

 DECLARE
   1 CUSTOMER_MASTER,
     2 CUSTOMER_ID           CUST_NUM_T,
     2 CUSTOMER_STATUS       BIT(8),
       3 ACTIVE              BIT(1),
       3 PREMIUM_CUSTOMER    BIT(1),
       3 FLAGGED_FOR_REVIEW  BIT(1),
       3 ONLINE_ACCESS       BIT(1),
       3 PAPER_STATEMENTS    BIT(1),
       3 MARKETING_CONSENT   BIT(1),
       3 RESERVED            BIT(2),
     2 NAME_INFO,
       3 TITLE               CHAR(10) VARYING,    /* Mr., Mrs., Dr., etc. */
       3 FIRST_NAME          CHAR(30) VARYING,
       3 MIDDLE_INITIAL      CHAR(1),
       3 LAST_NAME           CHAR(40) VARYING,
       3 SUFFIX              CHAR(10) VARYING,    /* Jr., III, etc. */
       3 FULL_NAME           CHAR(100) VARYING,   /* Computed/stored */
     2 CONTACT_INFO,
       3 EMAIL               CHAR(80) VARYING,
       3 PHONE_PRIMARY       PICTURE '(999)999-9999',
       3 PHONE_SECONDARY     PICTURE '(999)999-9999',
       3 PHONE_MOBILE        PICTURE '(999)999-9999',
       3 PREFERRED_CONTACT   CHAR(1),             /* E/P/M */
     2 ADDRESS_INFO,
       3 ADDRESS_LINES(3)    CHAR(60) VARYING,    /* DIMENSION demo */
       3 CITY                CHAR(40) VARYING,
       3 STATE_PROVINCE      CHAR(20) VARYING,
       3 POSTAL_CODE         CHAR(10),
       3 COUNTRY_CODE        CHAR(3),             /* ISO 3166-1 */
     2 DEMOGRAPHIC_INFO,
       3 DATE_OF_BIRTH       DATE_T,
       3 NATIONAL_ID         CHAR(20) VARYING,    /* SSN, NI number, etc. */
       3 TAX_ID              CHAR(20) VARYING,
       3 CITIZENSHIP         CHAR(3),
       3 RESIDENCY_STATUS    CHAR(1),
       3 LANGUAGE_PREF       CHAR(3),             /* ISO 639-2 */
     2 FINANCIAL_INFO,
       3 CREDIT_SCORE        FIXED DEC(3),        /* 0-999 */
       3 SCORE_DATE          DATE_T,
       3 NEXT_REVIEW_DATE    DATE_T,
       3 ANNUAL_INCOME       CURRENCY_T,
       3 NET_WORTH_ESTIMATE  CURRENCY_T,
       3 EMPLOYMENT_STATUS   CHAR(1),             /* E/S/R/U/O */
       3 EMPLOYER_NAME       CHAR(60) VARYING,
       3 YEARS_AT_EMPLOYER   FIXED BIN(15),
     2 BANKING_INFO,
       3 RELATIONSHIP_START  DATE_T,
       3 BRANCH_CODE         SORT_CODE_T,
       3 RELATIONSHIP_MGR    CHAR(8),             /* Employee ID */
       3 ACCOUNT_COUNT       FIXED BIN(15),
       3 TOTAL_DEPOSITS      CURRENCY_T,
       3 LAST_CONTACT_DATE   DATE_T,
       3 LAST_TRANSACTION    DATE_T,
     2 SECURITY_INFO,
       3 SECURITY_QUESTION   CHAR(100) VARYING,
       3 SECURITY_ANSWER     CHAR(60) VARYING,    /* Hashed */
       3 PIN_HASH            CHAR(64),            /* SHA-256 */
       3 FAILED_LOGIN_COUNT  FIXED BIN(15),
       3 LAST_LOGIN          TIMESTAMP_T,
       3 PASSWORD_CHANGE_DT  DATE_T,
     2 AUDIT_INFO,
       3 CREATED_BY          CHAR(8),
       3 CREATED_DATE        TIMESTAMP_T,
       3 UPDATED_BY          CHAR(8),
       3 UPDATED_DATE        TIMESTAMP_T,
       3 UPDATE_COUNT        FIXED BIN(31);

 /* LIKE attribute - derived structure with same layout */
 DECLARE 1 CUSTOMER_WORK_AREA LIKE CUSTOMER_MASTER;

 /* Second LIKE example with different name */
 DECLARE 1 CUSTOMER_BACKUP LIKE CUSTOMER_MASTER;

 /* DEFINED overlay - map structure to character buffer for I/O */
 DECLARE CUSTOMER_BUFFER CHAR(2048);
 DECLARE 1 CUSTOMER_OVERLAY DEFINED CUSTOMER_BUFFER LIKE CUSTOMER_MASTER;

 /* Alternate DEFINED approach - specific field overlay */
 DECLARE CUST_ID_OVERLAY CUST_NUM_T 
   DEFINED CUSTOMER_BUFFER POSITION(1);

 /* Demonstration of PICTURE with various formats */
 DECLARE
   1 CUSTOMER_DISPLAY,
     2 CUST_ID_DISPLAY     PICTURE '9999999999',
     2 CREDIT_SCORE_DSP    PICTURE 'ZZZ',
     2 BALANCE_DISPLAY     PICTURE 'ZZZ,ZZZ,ZZ9.99',
     2 SSN_DISPLAY         PICTURE '999-99-9999',
     2 DATE_DISPLAY        PICTURE '99/99/9999';

 /* Array of customer pointers for batch processing */
 DECLARE CUST_PTR_ARRAY(1000) POINTER;

 /* Customer key structure for indexed access */
 DECLARE
   1 CUSTOMER_KEY,
     2 BRANCH_CODE         SORT_CODE_T,
     2 CUSTOMER_ID         CUST_NUM_T;

 /* Alternate key by National ID */
 DECLARE
   1 CUSTOMER_ALT_KEY,
     2 COUNTRY_CODE        CHAR(3),
     2 NATIONAL_ID         CHAR(20);
