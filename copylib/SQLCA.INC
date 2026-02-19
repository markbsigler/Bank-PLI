 /*********************************************************************/
 /*  SQLCA.INC - SQL Communication Area                              */
 /*                                                                   */
 /*  Standard DB2 SQLCA structure for embedded SQL                   */
 /*  Demonstrates embedded SQL interface structures                  */
 /*********************************************************************/

 DECLARE
   1 SQLCA,
     2 SQLCAID             CHAR(8),              /* 'SQLCA   ' */
     2 SQLCABC             FIXED BIN(31),        /* Length = 136 */
     2 SQLCODE             FIXED BIN(31),        /* Return code */
     2 SQLERRM,
       3 SQLERRML          FIXED BIN(15),        /* Error msg length */
       3 SQLERRMC          CHAR(70),             /* Error message */
     2 SQLERRP             CHAR(8),              /* Diagnostic info */
     2 SQLERRD(6)          FIXED BIN(31),        /* Diagnostic data */
     2 SQLWARN,
       3 SQLWARN0          CHAR(1),              /* Master warning */
       3 SQLWARN1          CHAR(1),              /* Truncation */
       3 SQLWARN2          CHAR(1),              /* Null elimination */
       3 SQLWARN3          CHAR(1),              /* Mismatch */
       3 SQLWARN4          CHAR(1),              /* Dynamic SQL */
       3 SQLWARN5          CHAR(1),              /* SQLDA */
       3 SQLWARN6          CHAR(1),              /* Date/time */
       3 SQLWARN7          CHAR(1),              /* Reserved */
       3 SQLWARN8          CHAR(1),              /* Reserved */
       3 SQLWARN9          CHAR(1),              /* Reserved */
       3 SQLWARNA          CHAR(1),              /* Reserved */
     2 SQLSTATE            CHAR(5);              /* SQL state */

 /* Common SQLCODE values */
 DECLARE SQL_SUCCESS           FIXED BIN(31) VALUE(0)    STATIC;
 DECLARE SQL_NOT_FOUND         FIXED BIN(31) VALUE(100)  STATIC;
 DECLARE SQL_DUPLICATE_KEY     FIXED BIN(31) VALUE(-803) STATIC;
 DECLARE SQL_DEADLOCK          FIXED BIN(31) VALUE(-911) STATIC;
 DECLARE SQL_TIMEOUT           FIXED BIN(31) VALUE(-913) STATIC;
 DECLARE SQL_INVALID_CURSOR    FIXED BIN(31) VALUE(-501) STATIC;
 DECLARE SQL_CURSOR_NOT_OPEN   FIXED BIN(31) VALUE(-501) STATIC;
 DECLARE SQL_CURSOR_ALREADY_OPEN FIXED BIN(31) VALUE(-502) STATIC;

 /* SQL State Categories */
 DECLARE SQLSTATE_SUCCESS      CHAR(2) VALUE('00') STATIC;
 DECLARE SQLSTATE_WARNING      CHAR(2) VALUE('01') STATIC;
 DECLARE SQLSTATE_NOT_FOUND    CHAR(2) VALUE('02') STATIC;
 DECLARE SQLSTATE_ERROR        CHAR(1) VALUE('2') STATIC;  /* Class starts with 2 */
 DECLARE SQLSTATE_SEVERE       CHAR(1) VALUE('4') STATIC;  /* Class starts with 4 */

 /* Macro for SQL error checking */
 %CHECK_SQL: PROCEDURE;
   IF SQLCODE ¬= SQL_SUCCESS & SQLCODE ¬= SQL_NOT_FOUND THEN DO;
     PUT SKIP LIST('SQL ERROR:');
     PUT SKIP LIST('  SQLCODE: ' || TRIM(SQLCODE));
     PUT SKIP LIST('  SQLSTATE: ' || SQLSTATE);
     PUT SKIP LIST('  SQLERRMC: ' || SUBSTR(SQLERRMC, 1, SQLERRML));
     SIGNAL ERROR;
   END;
 %END CHECK_SQL;

 /* SQLDA for dynamic SQL */
 DECLARE
   1 SQLDA,
     2 SQLDAID             CHAR(8),              /* 'SQLDA   ' */
     2 SQLDABC             FIXED BIN(31),        /* Length */
     2 SQLN                FIXED BIN(15),        /* Number allocated */
     2 SQLD                FIXED BIN(15),        /* Number used */
     2 SQLVAR(1:50),
       3 SQLTYPE           FIXED BIN(15),        /* Data type */
       3 SQLLEN            FIXED BIN(15),        /* Length */
       3 SQLDATA           POINTER,              /* Data pointer */
       3 SQLIND            POINTER,              /* Null indicator */
       3 SQLNAME,
         4 SQLNAMEL        FIXED BIN(15),        /* Name length */
         4 SQLNAMEC        CHAR(30);             /* Column name */
