 /*********************************************************************/
 /*  PICFMT.INC - Advanced PICTURE Format Definitions                */
 /*                                                                   */
 /*  Demonstrates:                                                   */
 /*    - PICTURE with drift characters ($, Z, *, +, -, CR, DB)       */
 /*    - PICTURE with insertion characters (/, ., ,, B)              */
 /*    - PICTURE with suppression and replacement                    */
 /*    - PICTURE dB (decibel) specification                          */
 /*    - Complex PICTURE for formatted output                        */
 /*********************************************************************/

 /*-----------------------------------------------------------------*/
 /* Currency and Financial Formats - Drift Characters              */
 /*-----------------------------------------------------------------*/

 /* Floating dollar sign - $ drifts to first significant digit */
 DECLARE CURRENCY_DRIFT_1       PIC '$$$,$$9.99';
 DECLARE CURRENCY_DRIFT_2       PIC '$$$$$$9.99CR';  /* With credit indicator */
 DECLARE CURRENCY_DRIFT_3       PIC '$$$,$$$,$$9.99-'; /* With sign */

 /* Asterisk fill (check protection) - prevents alteration */
 DECLARE CHECK_PROTECTION_1     PIC '***,**9.99';
 DECLARE CHECK_PROTECTION_2     PIC '*,***,**9.99';
 DECLARE CHECK_PROTECTION_3     PIC '****,***,**9.99DB'; /* With debit indicator */

 /* Zero suppression with drift */
 DECLARE ZERO_SUPPRESS_1        PIC 'ZZZ,ZZ9.99';    /* Replace leading zeros */
 DECLARE ZERO_SUPPRESS_2        PIC 'ZZZZ,ZZZ,ZZ9'; /* Integer with suppression */
 DECLARE ZERO_SUPPRESS_3        PIC 'ZZ,ZZ9.99-';    /* With sign */

 /* Plus/minus drift */
 DECLARE SIGNED_AMOUNT_1        PIC '+++,++9.99';    /* Drift + or - */
 DECLARE SIGNED_AMOUNT_2        PIC '---,--9.99';    /* Drift - only */

 /*-----------------------------------------------------------------*/
 /* Statement and Report Formats                                    */
 /*-----------------------------------------------------------------*/

 DECLARE 1 BANK_STATEMENT_LINE,
   2 TRANSACTION_DATE       PIC '99/99/9999',       /* MM/DD/YYYY */
   2 CHECK_NUMBER           PIC '****9999',          /* Protected */
   2 DESCRIPTION            CHAR(40) VARYING,
   2 DEBIT_AMOUNT           PIC '$$$,$$9.99DB',     /* Debit with DB */
   2 CREDIT_AMOUNT          PIC '$$$,$$9.99CR',     /* Credit with CR */
   2 BALANCE                PIC '$$,$$$,$$9.99-';   /* Balance with sign */

 DECLARE 1 ACCOUNT_SUMMARY,
   2 BEGINNING_BALANCE      PIC '$$,$$$,$$9.99CR',
   2 TOTAL_DEPOSITS         PIC '$$,$$$,$$9.99',
   2 TOTAL_WITHDRAWALS      PIC '$$,$$$,$$9.99DB',
   2 SERVICE_CHARGES        PIC '$$$,$$9.99DB',
   2 INTEREST_EARNED        PIC '$$$,$$9.99CR',
   2 ENDING_BALANCE         PIC '$$,$$$,$$9.99-';

 /*-----------------------------------------------------------------*/
 /* Date and Time Formats                                           */
 /*-----------------------------------------------------------------*/

 DECLARE DATE_US_FORMAT         PIC '99/99/9999';     /* MM/DD/YYYY */
 DECLARE DATE_ISO_FORMAT        PIC '9999-99-99';     /* YYYY-MM-DD */
 DECLARE DATE_EUROPEAN          PIC '99.99.9999';     /* DD.MM.YYYY */
 DECLARE TIME_STANDARD          PIC '99:99:99';       /* HH:MM:SS */
 DECLARE TIME_WITH_AMPM         PIC '99:99B99';       /* HH:MM AM */
 DECLARE TIMESTAMP_COMPACT      PIC '99999999999999'; /* YYYYMMDDHHMMSS */

 /*-----------------------------------------------------------------*/
 /* Percentage and Rate Formats                                     */
 /*-----------------------------------------------------------------*/

 DECLARE INTEREST_RATE          PIC '.999V99%';       /* 0.04950% */
 DECLARE PERCENTAGE_WHOLE       PIC 'ZZ9%';           /* 95% */
 DECLARE PERCENTAGE_DECIMAL     PIC 'ZZ9.99%';        /* 12.50% */
 DECLARE BASIS_POINTS           PIC 'ZZZ9.99';        /* 125.50 bps */

 /*-----------------------------------------------------------------*/
 /* Account and ID Formats                                          */
 /*-----------------------------------------------------------------*/

 DECLARE SSN_FORMAT             PIC '999-99-9999';    /* Social Security */
 DECLARE EIN_FORMAT             PIC '99-9999999';     /* Employer ID */
 DECLARE PHONE_US_FORMAT        PIC '(999)B999-9999'; /* (555) 123-4567 */
 DECLARE PHONE_INTL_FORMAT      PIC '+99B999B999B9999'; /* +1 555 123 4567 */
 DECLARE ROUTING_NUMBER         PIC '999999999';      /* ABA routing */
 DECLARE ACCOUNT_NUMBER_MASK    PIC 'XXXX-XXXX-XXXX-9999'; /* Masked display */

 /*-----------------------------------------------------------------*/
 /* Numeric Display - Various Scales                               */
 /*-----------------------------------------------------------------*/

 DECLARE SMALL_AMOUNT           PIC '$Z9.99';         /* $5.00 */
 DECLARE MEDIUM_AMOUNT          PIC '$ZZZ,ZZ9.99';    /* $1,234.56 */
 DECLARE LARGE_AMOUNT           PIC '$$,$$$,$$9.99';  /* $1,000,000.00 */
 DECLARE VERY_LARGE_AMOUNT      PIC '$$$,$$$$,$$$$,$$9.99'; /* Billions */

 /* Without dollar sign */
 DECLARE NUMERIC_COMMA_1        PIC 'ZZZ,ZZ9';        /* 1,234 */
 DECLARE NUMERIC_COMMA_2        PIC 'ZZZ,ZZZ,ZZ9';    /* 1,234,567 */
 DECLARE NUMERIC_COMMA_3        PIC 'Z,ZZZ,ZZZ,ZZ9.99'; /* With decimals */

 /*-----------------------------------------------------------------*/
 /* Scientific and Technical Formats                                */
 /*-----------------------------------------------------------------*/

 /* Decibel format - for voice biometric quality (fraud detection) */
 DECLARE VOICE_QUALITY_DB       PIC 'S999V99DB';      /* Signed decibel */
 DECLARE SIGNAL_STRENGTH        PIC 'ZZ9.99DB';       /* Positive only */
 DECLARE NOISE_LEVEL            PIC '-ZZ9.99DB';      /* Negative indicator */

 /* Engineering notation approximation */
 DECLARE SCIENTIFIC_1           PIC 'S9.9999999';     /* Mantissa */
 DECLARE EXPONENT_PART          PIC 'S99';            /* Exponent */

 /*-----------------------------------------------------------------*/
 /* Credit Card and Payment Formats                                 */
 /*-----------------------------------------------------------------*/

 DECLARE 1 CREDIT_CARD_DISPLAY,
   2 CARD_NUMBER            PIC 'XXXX-XXXX-XXXX-9999', /* Masked */
   2 EXPIRY_DATE            PIC '99/99',               /* MM/YY */
   2 CVV_CODE               PIC 'XXX';                 /* Hidden */

 DECLARE 1 PAYMENT_DETAILS,
   2 INVOICE_NUMBER         PIC '99999999',
   2 INVOICE_DATE           PIC '99/99/9999',
   2 DUE_DATE               PIC '99/99/9999',
   2 INVOICE_AMOUNT         PIC '$$,$$$,$$9.99',
   2 AMOUNT_PAID            PIC '$$,$$$,$$9.99CR',
   2 BALANCE_DUE            PIC '$$,$$$,$$9.99DB';

 /*-----------------------------------------------------------------*/
 /* Special Formats for Reporting                                   */
 /*-----------------------------------------------------------------*/

 /* Blank insertion for readability */
 DECLARE FORMATTED_ACCOUNT      PIC '9999BB9999BB9999'; /* Groups of 4 */
 DECLARE FORMATTED_ID           PIC '99BBB99BBB9999';   /* Groups of 2,2,4 */

 /* Slash insertion for dates */
 DECLARE DATE_SLASHES           PIC '99/99/99';         /* Short date */
 DECLARE LONG_DATE              PIC '9999/99/99';       /* ISO-ish */

 /* Comma insertion for thousands */
 DECLARE FORMATTED_THOUSANDS    PIC '9,999,999,999';    /* No decimals */
 DECLARE FORMATTED_MONEY        PIC '9,999,999,999.99'; /* With decimals */

 /*-----------------------------------------------------------------*/
 /* Suppression and Replacement Examples                            */
 /*-----------------------------------------------------------------*/

 /* Zero suppression with replacement */
 DECLARE ZERO_TO_BLANK          PIC 'ZZZZZ';           /* Blanks for zeros */
 DECLARE ZERO_TO_ASTERISK       PIC '*****';           /* Asterisks */

 /* Sign handling variations */
 DECLARE TRAILING_SIGN          PIC '999999-';         /* Trailing minus */
 DECLARE LEADING_SIGN           PIC '-999999';         /* Leading minus */
 DECLARE CREDIT_DEBIT           PIC '999999CR';        /* CR for negative */
 DECLARE DEBIT_ONLY             PIC '999999DB';        /* DB for negative */
 DECLARE PARENTHESES            PIC '(999999)';        /* Accounting format */

 /*-----------------------------------------------------------------*/
 /* Complex Combined Formats                                        */
 /*-----------------------------------------------------------------*/

 /* Full featured statement line format */
 DECLARE COMPLEX_STATEMENT      PIC '$$$,$$$,$$9.99CR';  /* CR or DB but not both */

 /* International currency - PL/I PICTURE supports only $ as drift.
    Use CHAR concatenation at runtime to prefix €, £, ¥ symbols. */
 DECLARE EURO_AMOUNT            PIC '$ZZZ,ZZ9.99';     /* Replace $ with € at display */
 DECLARE POUND_AMOUNT           PIC '$ZZZ,ZZ9.99';     /* Replace $ with £ at display */
 DECLARE YEN_AMOUNT             PIC '$ZZZ,ZZ9';         /* Replace $ with ¥ at display */

 /*-----------------------------------------------------------------*/
 /* Fixed vs. Varying Point                                        */
 /*-----------------------------------------------------------------*/

 /* V indicates assumed decimal point (not printed) */
 DECLARE IMPLIED_DECIMAL_1      PIC '99999V99';        /* 5 digits, 2 decimal */
 DECLARE IMPLIED_DECIMAL_2      PIC 'ZZZV99';          /* Suppress, assumed dec */

 /* Explicit decimal point */
 DECLARE EXPLICIT_DECIMAL       PIC 'ZZZ,ZZ9.99';      /* Printed decimal */

 /*-----------------------------------------------------------------*/
 /* Usage Examples in Structure                                     */
 /*-----------------------------------------------------------------*/

 DECLARE 1 MONTHLY_STATEMENT,
   2 STATEMENT_DATE         PIC '99/99/9999',
   2 ACCOUNT_ID             PIC '9999-9999-9999',
   2 CUSTOMER_NAME          CHAR(40) VARYING,
   2 OPENING_BALANCE        PIC '$$,$$$,$$9.99CR',
   2 DEPOSITS_COUNT         PIC 'ZZ9',
   2 DEPOSITS_TOTAL         PIC '$$,$$$,$$9.99',
   2 WITHDRAWALS_COUNT      PIC 'ZZ9',
   2 WITHDRAWALS_TOTAL      PIC '$$,$$$,$$9.99DB',
   2 CHECKS_CLEARED         PIC 'ZZ9',
   2 SERVICE_CHARGE         PIC '$ZZ9.99DB',
   2 INTEREST_RATE          PIC 'Z.999%',
   2 INTEREST_AMOUNT        PIC '$ZZZ.99CR',
   2 CLOSING_BALANCE        PIC '$$,$$$,$$9.99-',
   2 AVAILABLE_BALANCE      PIC '$$,$$$,$$9.99',
   2 OVERDRAFT_LIMIT        PIC '$$,$$$,$$9.99',
   2 AVERAGE_BALANCE        PIC '$$,$$$,$$9.99';

 DECLARE 1 VOICE_BIOMETRIC_SCORE,
   2 SAMPLE_QUALITY_DB      PIC 'S99.99DB',     /* Signal quality */
   2 NOISE_FLOOR_DB         PIC 'S99.99DB',     /* Background noise */
   2 SNR_RATIO_DB           PIC '+99.99DB',     /* Signal-to-noise */
   2 MATCH_CONFIDENCE       PIC 'ZZ9.99%',      /* Match percentage */
   2 FRAUD_RISK_SCORE       PIC 'ZZ9.99',       /* Risk score 0-100 */
   2 ACCEPT_THRESHOLD       PIC '99.99%';       /* Threshold for accept */
