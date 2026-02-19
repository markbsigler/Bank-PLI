//BNKBATCH JOB (ACCT),'RUN BANKING BATCH',
//         CLASS=B,MSGCLASS=X,NOTIFY=&SYSUID,REGION=256M
//*
//*--------------------------------------------------------------------*
//* JCL to execute the PL/I Banking Batch Processing Application      *
//*                                                                    *
//* This JCL demonstrates:                                             *
//*   - PARM passing to PL/I main program                             *
//*   - VSAM KSDS file allocation for indexed files                   *
//*   - Sequential file allocation                                    *
//*   - Regional(1) file allocation for direct access                 *
//*   - SYSPRINT for report output                                    *
//*   - DB2 connection in batch environment                           *
//*--------------------------------------------------------------------*
//*
//JCLLIB   JCLLIB ORDER=(DB2.PROCLIB)
//*
//*--------------------------------------------------------------------*
//* Step 1: Run BNKBATCH - Generate test data                         *
//*--------------------------------------------------------------------*
//GENERATE EXEC PGM=BNKBATCH,
//         PARM='CUSTOMERS=1000 ACCOUNTS=5000 TRANSACTIONS=10000'
//STEPLIB  DD DSN=BANK.LOADLIB,DISP=SHR
//         DD DSN=CEE.SCEERUN,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZRUN,DISP=SHR
//         DD DSN=DB2.RUNLIB.LOAD,DISP=SHR
//*
//*--------------------------------------------------------------------*
//* VSAM KSDS Customer Master File (Keyed Sequential)                 *
//*--------------------------------------------------------------------*
//CUSTFILE DD DSN=BANK.CUSTOMER.MASTER,DISP=SHR,
//            AMP=('BUFNI=10,BUFND=20')
//*
//* Alternate allocation if file does not exist:
//* //CUSTFILE DD DSN=BANK.CUSTOMER.MASTER,DISP=(NEW,CATLG,DELETE),
//* //            UNIT=SYSDA,
//* //            VOL=SER=USER01,
//* //            SPACE=(CYL,(10,5)),
//* //            DCB=(DSORG=PS,RECFM=FB,LRECL=500,BLKSIZE=27500),
//* //            AMP=('KILOBYTES=50')
//*
//*--------------------------------------------------------------------*
//* VSAM KSDS Account Master File (Keyed Sequential)                  *
//*--------------------------------------------------------------------*
//ACCTFILE DD DSN=BANK.ACCOUNT.MASTER,DISP=SHR,
//            AMP=('BUFNI=15,BUFND=30')
//*
//* Create using IDCAMS:
//* //DEFACCT  EXEC PGM=IDCAMS
//* //SYSPRINT DD SYSOUT=*
//* //SYSIN    DD *
//*   DELETE BANK.ACCOUNT.MASTER CLUSTER PURGE
//*   DEFINE CLUSTER (NAME(BANK.ACCOUNT.MASTER) -
//*          CYLINDERS(20 5) -
//*          RECORDSIZE(800 800) -
//*          KEYS(10 0) -
//*          INDEXED -
//*          SHAREOPTIONS(2 3) -
//*          LOG(NONE)) -
//*          DATA (NAME(BANK.ACCOUNT.MASTER.DATA) -
//*                CISZ(4096)) -
//*          INDEX (NAME(BANK.ACCOUNT.MASTER.INDEX) -
//*                 CISZ(2048))
//* /*
//*
//*--------------------------------------------------------------------*
//* Transaction Sequential File (for batch processing)                *
//*--------------------------------------------------------------------*
//SEQFILE  DD DSN=BANK.TRANSACTION.SEQ,DISP=(NEW,CATLG,DELETE),
//            UNIT=SYSDA,
//            SPACE=(CYL,(5,2),RLSE),
//            DCB=(DSORG=PS,RECFM=FB,LRECL=200,BLKSIZE=27800)
//*
//*--------------------------------------------------------------------*
//* Regional(1) Direct Access File                                    *
//*--------------------------------------------------------------------*
//REGFILE  DD DSN=BANK.REGIONAL.DATA,DISP=SHR
//*
//* Alternate allocation if file does not exist:
//* //REGFILE  DD DSN=BANK.REGIONAL.DATA,DISP=(NEW,CATLG,DELETE),
//* //            UNIT=SYSDA,
//* //            SPACE=(TRK,(100,10)),
//* //            DCB=(DSORG=DA,RECFM=F,LRECL=100,BLKSIZE=100,
//* //                 KEYLEN=0,RKP=0,NCP=5)
//*
//*--------------------------------------------------------------------*
//* Report Output File (SYSPRINT)                                     *
//*--------------------------------------------------------------------*
//REPORT   DD SYSOUT=*,
//            DCB=(RECFM=FBA,LRECL=133,BLKSIZE=1330)
//*
//*--------------------------------------------------------------------*
//* Additional output files for BNKAUDIT if needed                    *
//*--------------------------------------------------------------------*
//AUDIT    DD DSN=BANK.AUDIT.LOG,DISP=(MOD,KEEP),
//            UNIT=SYSDA,
//            SPACE=(CYL,(2,1),RLSE),
//            DCB=(RECFM=FBA,LRECL=133,BLKSIZE=13300)
//*
//DETAIL   DD DSN=BANK.DETAIL.REPORT,DISP=(NEW,CATLG,DELETE),
//            UNIT=SYSDA,
//            SPACE=(CYL,(3,1),RLSE),
//            DCB=(RECFM=FBA,LRECL=133,BLKSIZE=13300)
//*
//SUMMARY  DD DSN=BANK.SUMMARY.REPORT,DISP=(NEW,CATLG,DELETE),
//            UNIT=SYSDA,
//            SPACE=(CYL,(1,1),RLSE),
//            DCB=(RECFM=FBA,LRECL=81,BLKSIZE=8100)
//*
//*--------------------------------------------------------------------*
//* DB2 Plan allocation                                               *
//*--------------------------------------------------------------------*
//DSNPLAN  DD DSN=DB2.RUNLIB.LOAD,DISP=SHR
//*
//*--------------------------------------------------------------------*
//* System files                                                      *
//*--------------------------------------------------------------------*
//SYSOUT   DD SYSOUT=*
//SYSUDUMP DD SYSOUT=*
//CEEDUMP  DD SYSOUT=*
//*
//*--------------------------------------------------------------------*
//* Step 2 (Optional): Run BNKAUDIT - Audit log analysis              *
//*--------------------------------------------------------------------*
//AUDIT    EXEC PGM=BNKAUDIT,COND=(0,NE)
//STEPLIB  DD DSN=BANK.LOADLIB,DISP=SHR
//         DD DSN=CEE.SCEERUN,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZRUN,DISP=SHR
//*
//AUDITLOG DD DSN=BANK.AUDIT.LOG,DISP=SHR
//*
//DETAILRP DD SYSOUT=*,
//            DCB=(RECFM=FBA,LRECL=133)
//*
//SUMMARYR DD SYSOUT=*,
//            DCB=(RECFM=FBA,LRECL=81)
//*
//SYSOUT   DD SYSOUT=*
//SYSUDUMP DD SYSOUT=*
//CEEDUMP  DD SYSOUT=*
//*
//*--------------------------------------------------------------------*
//* Step 3 (Optional): Run credit scoring batch                       *
//*--------------------------------------------------------------------*
//SCORING  EXEC PGM=BNKCREDT,COND=(0,NE)
//STEPLIB  DD DSN=BANK.LOADLIB,DISP=SHR
//         DD DSN=CEE.SCEERUN,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZRUN,DISP=SHR
//*
//CUSTFILE DD DSN=BANK.CUSTOMER.MASTER,DISP=SHR,
//            AMP=('BUFNI=10,BUFND=20')
//*
//SCORERPT DD SYSOUT=*,
//            DCB=(RECFM=FBA,LRECL=133)
//*
//SYSOUT   DD SYSOUT=*
//SYSUDUMP DD SYSOUT=*
//CEEDUMP  DD SYSOUT=*
//*
//*--------------------------------------------------------------------*
//* Alternative: Run under DB2 TSO environment                        *
//*--------------------------------------------------------------------*
//*RUNDB2   EXEC PGM=IKJEFT01,DYNAMNBR=30
//*STEPLIB  DD DSN=DB2.RUNLIB.LOAD,DISP=SHR
//*         DD DSN=CEE.SCEERUN,DISP=SHR
//*         DD DSN=PLI.V6R1M0.SIBMZRUN,DISP=SHR
//*         DD DSN=BANK.LOADLIB,DISP=SHR
//*SYSTSPRT DD SYSOUT=*
//*SYSTSIN  DD *
//*  DSN SYSTEM(DB2P)
//*  RUN PROGRAM(BNKBATCH) PLAN(BANKPLAN) -
//*      PARM('CUSTOMERS=1000 ACCOUNTS=5000')
//*  END
//*/*
//*CUSTFILE DD DSN=BANK.CUSTOMER.MASTER,DISP=SHR
//*ACCTFILE DD DSN=BANK.ACCOUNT.MASTER,DISP=SHR
//*SEQFILE  DD DSN=BANK.TRANSACTION.SEQ,DISP=(NEW,CATLG,DELETE),
//*            UNIT=SYSDA,SPACE=(CYL,(5,2)),
//*            DCB=(RECFM=FB,LRECL=200,BLKSIZE=27800)
//*REGFILE  DD DSN=BANK.REGIONAL.DATA,DISP=SHR
//*REPORT   DD SYSOUT=*
//*SYSOUT   DD SYSOUT=*
//*SYSUDUMP DD SYSOUT=*
//*
//*--------------------------------------------------------------------*
//* Post-processing: Verify data generation results                   *
//*--------------------------------------------------------------------*
//VERIFY   EXEC PGM=IDCAMS,COND=(0,NE)
//SYSPRINT DD SYSOUT=*
//SYSIN    DD *
  LISTCAT ENTRIES(BANK.CUSTOMER.MASTER) ALL
  LISTCAT ENTRIES(BANK.ACCOUNT.MASTER) ALL
  PRINT INFILE(CUSTDD) COUNT(10)
  PRINT INFILE(ACCTDD) COUNT(10)
/*
//CUSTDD   DD DSN=BANK.CUSTOMER.MASTER,DISP=SHR
//ACCTDD   DD DSN=BANK.ACCOUNT.MASTER,DISP=SHR
//*
//*--------------------------------------------------------------------*
//* Environment setup notes:                                          *
//*                                                                   *
//* 1. Create VSAM files using IDCAMS before first run                *
//* 2. Ensure BANK.LOADLIB contains compiled programs                 *
//* 3. DB2 subsystem must be active for SQL programs                  *
//* 4. Adjust REGION size based on actual data volumes                *
//* 5. Review SYSOUT for execution results and statistics             *
//*--------------------------------------------------------------------*
