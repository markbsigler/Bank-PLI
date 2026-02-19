//BNKCOMP  JOB (ACCT),'COMPILE BANKING SYSTEM',
//         CLASS=A,MSGCLASS=X,NOTIFY=&SYSUID,REGION=0M
//*
//*--------------------------------------------------------------------*
//* JCL to compile and link-edit the PL/I Banking Application         *
//*                                                                    *
//* This JCL demonstrates:                                             *
//*   - PL/I package compilation with %PROCESS directives             *
//*   - DB2 precompile for embedded SQL                               *
//*   - CICS command translation                                      *
//*   - SYSLIB for %INCLUDE copylib resolution                        *
//*   - Link-edit with multiple load modules                          *
//*   - Binder options for CICS programs                              *
//*--------------------------------------------------------------------*
//*
//JCLLIB   JCLLIB ORDER=(CICS.SDFHPROC,DB2.PROCLIB)
//*
//*====================================================================*
//*  SECTION 1: SIMPLE PROGRAMS (no CICS, no SQL)                     *
//*====================================================================*
//*
//*--------------------------------------------------------------------*
//* Step 1: Compile BNKPKG (Package) - No CICS or SQL                 *
//*--------------------------------------------------------------------*
//PKG      EXEC PGM=IBMZPLI,
//         PARM='/PACKAGE NOEXECOPS AGGREGATE LIMITS(NAME(31))'
//STEPLIB  DD DSN=PLI.V6R1M0.SIBMZCMP,DISP=SHR
//SYSLIB   DD DSN=BANK.PLI.COPYLIB,DISP=SHR
//SYSLIN   DD DSN=&&PKG,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//SYSIN    DD DSN=BANK.PLI.SOURCE(BNKPKG),DISP=SHR
//*
//*--------------------------------------------------------------------*
//* Step 2: Compile BNKAUDIT (Audit) - No CICS, No SQL                *
//*--------------------------------------------------------------------*
//AUDIT    EXEC PGM=IBMZPLI,
//         PARM='/NOEXECOPS AGGREGATE LIMITS(NAME(31))'
//STEPLIB  DD DSN=PLI.V6R1M0.SIBMZCMP,DISP=SHR
//SYSLIB   DD DSN=BANK.PLI.COPYLIB,DISP=SHR
//SYSLIN   DD DSN=&&AUDIT,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//SYSIN    DD DSN=BANK.PLI.SOURCE(BNKAUDIT),DISP=SHR
//*
//*--------------------------------------------------------------------*
//* Step 3: Compile BNKARCH (Archive) - No CICS, No SQL               *
//*--------------------------------------------------------------------*
//ARCH     EXEC PGM=IBMZPLI,
//         PARM='/AGGREGATE LIMITS(NAME(31))'
//STEPLIB  DD DSN=PLI.V6R1M0.SIBMZCMP,DISP=SHR
//SYSLIB   DD DSN=BANK.PLI.COPYLIB,DISP=SHR
//SYSLIN   DD DSN=&&ARCH,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//SYSIN    DD DSN=BANK.PLI.SOURCE(BNKARCH),DISP=SHR
//*
//*--------------------------------------------------------------------*
//* Step 4: Compile BNKFX (Foreign Exchange) - No CICS, No SQL        *
//*--------------------------------------------------------------------*
//FX       EXEC PGM=IBMZPLI,
//         PARM='/AGGREGATE LIMITS(NAME(31))'
//STEPLIB  DD DSN=PLI.V6R1M0.SIBMZCMP,DISP=SHR
//SYSLIB   DD DSN=BANK.PLI.COPYLIB,DISP=SHR
//SYSLIN   DD DSN=&&FX,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//SYSIN    DD DSN=BANK.PLI.SOURCE(BNKFX),DISP=SHR
//*
//*====================================================================*
//*  SECTION 2: DB2-ONLY PROGRAMS (SQL precompile, no CICS)           *
//*====================================================================*
//*
//*--------------------------------------------------------------------*
//* Step 5: DB2 Precompile BNKBATCH                                   *
//*--------------------------------------------------------------------*
//PCBATCH  EXEC PGM=DSNHPC,PARM='HOST(PLI)'
//STEPLIB  DD DSN=DB2.RUNLIB.LOAD,DISP=SHR
//DBRMLIB  DD DSN=BANK.DBRMLIB.DATA(BNKBATC),DISP=SHR
//SYSIN    DD DSN=BANK.PLI.SOURCE(BNKBATCH),DISP=SHR
//SYSCIN   DD DSN=&&BATCHPC,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//SYSUT2   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//*
//*--------------------------------------------------------------------*
//* Step 6: Compile BNKBATCH (DB2-precompiled source)                 *
//*--------------------------------------------------------------------*
//CBATCH   EXEC PGM=IBMZPLI,
//         PARM='/SQL NOEXECOPS AGGREGATE LIMITS(NAME(31))'
//STEPLIB  DD DSN=PLI.V6R1M0.SIBMZCMP,DISP=SHR
//SYSLIB   DD DSN=BANK.PLI.COPYLIB,DISP=SHR
//SYSLIN   DD DSN=&&BATCH,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//SYSIN    DD DSN=&&BATCHPC,DISP=(OLD,DELETE)
//*
//*====================================================================*
//*  SECTION 3: CICS+DB2 PROGRAMS (DB2 precompile then CICS translate)*
//*====================================================================*
//*
//*--------------------------------------------------------------------*
//* Step 7: DB2 Precompile BNKMAIN                                    *
//*--------------------------------------------------------------------*
//PCMAIN   EXEC PGM=DSNHPC,PARM='HOST(PLI)'
//STEPLIB  DD DSN=DB2.RUNLIB.LOAD,DISP=SHR
//DBRMLIB  DD DSN=BANK.DBRMLIB.DATA(BNKMAIN),DISP=SHR
//SYSIN    DD DSN=BANK.PLI.SOURCE(BNKMAIN),DISP=SHR
//SYSCIN   DD DSN=&&MAINPC,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//SYSUT2   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//*
//*--------------------------------------------------------------------*
//* Step 8: CICS Translate BNKMAIN                                    *
//*--------------------------------------------------------------------*
//TRNMAIN  EXEC PGM=DFHECP1$,PARM='NOEPILOG'
//STEPLIB  DD DSN=CICS.SDFHLOAD,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZCMP,DISP=SHR
//SYSIN    DD DSN=&&MAINPC,DISP=(OLD,DELETE)
//SYSPRINT DD SYSOUT=*
//SYSPUNCH DD DSN=&&MAINTRN,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//*
//*--------------------------------------------------------------------*
//* Step 9: Compile BNKMAIN (CICS/DB2 translated source)              *
//*--------------------------------------------------------------------*
//CMAIN    EXEC PGM=IBMZPLI,
//         PARM='/CICS SQL AGGREGATE LIMITS(NAME(31))'
//STEPLIB  DD DSN=PLI.V6R1M0.SIBMZCMP,DISP=SHR
//SYSLIB   DD DSN=BANK.PLI.COPYLIB,DISP=SHR
//SYSLIN   DD DSN=&&MAINOBJ,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//SYSIN    DD DSN=&&MAINTRN,DISP=(OLD,DELETE)
//*
//*--------------------------------------------------------------------*
//* Step 10: DB2 Precompile BNKCUST                                   *
//*--------------------------------------------------------------------*
//PCCUST   EXEC PGM=DSNHPC,PARM='HOST(PLI)'
//STEPLIB  DD DSN=DB2.RUNLIB.LOAD,DISP=SHR
//DBRMLIB  DD DSN=BANK.DBRMLIB.DATA(BNKCUST),DISP=SHR
//SYSIN    DD DSN=BANK.PLI.SOURCE(BNKCUST),DISP=SHR
//SYSCIN   DD DSN=&&CUSTPC,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//SYSUT2   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//*
//*--------------------------------------------------------------------*
//* Step 11: CICS Translate BNKCUST (after DB2 precompile)            *
//*--------------------------------------------------------------------*
//TRNCUST  EXEC PGM=DFHECP1$,PARM='NOEPILOG'
//STEPLIB  DD DSN=CICS.SDFHLOAD,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZCMP,DISP=SHR
//SYSIN    DD DSN=&&CUSTPC,DISP=(OLD,DELETE)
//SYSPRINT DD SYSOUT=*
//SYSPUNCH DD DSN=&&CUSTTRN,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//*
//*--------------------------------------------------------------------*
//* Step 12: Compile BNKCUST (CICS/DB2 translated source)             *
//*--------------------------------------------------------------------*
//CCUST    EXEC PGM=IBMZPLI,
//         PARM='/CICS SQL AGGREGATE LIMITS(NAME(31))'
//STEPLIB  DD DSN=PLI.V6R1M0.SIBMZCMP,DISP=SHR
//SYSLIB   DD DSN=BANK.PLI.COPYLIB,DISP=SHR
//SYSLIN   DD DSN=&&CUSTOBJ,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//SYSIN    DD DSN=&&CUSTTRN,DISP=(OLD,DELETE)
//*
//*--------------------------------------------------------------------*
//* Step 13: DB2 Precompile BNKACCT                                   *
//*--------------------------------------------------------------------*
//PCACCT   EXEC PGM=DSNHPC,PARM='HOST(PLI)'
//STEPLIB  DD DSN=DB2.RUNLIB.LOAD,DISP=SHR
//DBRMLIB  DD DSN=BANK.DBRMLIB.DATA(BNKACCT),DISP=SHR
//SYSIN    DD DSN=BANK.PLI.SOURCE(BNKACCT),DISP=SHR
//SYSCIN   DD DSN=&&ACCTPC,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//SYSUT2   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//*
//*--------------------------------------------------------------------*
//* Step 14: CICS Translate BNKACCT (after DB2 precompile)            *
//*--------------------------------------------------------------------*
//TRNACCT  EXEC PGM=DFHECP1$,PARM='NOEPILOG'
//STEPLIB  DD DSN=CICS.SDFHLOAD,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZCMP,DISP=SHR
//SYSIN    DD DSN=&&ACCTPC,DISP=(OLD,DELETE)
//SYSPRINT DD SYSOUT=*
//SYSPUNCH DD DSN=&&ACCTTRN,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//*
//*--------------------------------------------------------------------*
//* Step 15: Compile BNKACCT (CICS/DB2 translated source)             *
//*--------------------------------------------------------------------*
//CACCT    EXEC PGM=IBMZPLI,
//         PARM='/CICS SQL AGGREGATE LIMITS(NAME(31))'
//STEPLIB  DD DSN=PLI.V6R1M0.SIBMZCMP,DISP=SHR
//SYSLIB   DD DSN=BANK.PLI.COPYLIB,DISP=SHR
//SYSLIN   DD DSN=&&ACCTOBJ,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//SYSIN    DD DSN=&&ACCTTRN,DISP=(OLD,DELETE)
//*
//*--------------------------------------------------------------------*
//* Step 16: DB2 Precompile BNKXFER                                   *
//*--------------------------------------------------------------------*
//PCXFER   EXEC PGM=DSNHPC,PARM='HOST(PLI)'
//STEPLIB  DD DSN=DB2.RUNLIB.LOAD,DISP=SHR
//DBRMLIB  DD DSN=BANK.DBRMLIB.DATA(BNKXFER),DISP=SHR
//SYSIN    DD DSN=BANK.PLI.SOURCE(BNKXFER),DISP=SHR
//SYSCIN   DD DSN=&&XFERPC,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//SYSUT2   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//*
//*--------------------------------------------------------------------*
//* Step 17: CICS Translate BNKXFER (after DB2 precompile)            *
//*--------------------------------------------------------------------*
//TRNXFER  EXEC PGM=DFHECP1$,PARM='NOEPILOG'
//STEPLIB  DD DSN=CICS.SDFHLOAD,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZCMP,DISP=SHR
//SYSIN    DD DSN=&&XFERPC,DISP=(OLD,DELETE)
//SYSPRINT DD SYSOUT=*
//SYSPUNCH DD DSN=&&XFERTRN,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//*
//*--------------------------------------------------------------------*
//* Step 18: Compile BNKXFER (CICS/DB2 translated source)             *
//*--------------------------------------------------------------------*
//CXFER    EXEC PGM=IBMZPLI,
//         PARM='/CICS SQL AGGREGATE LIMITS(NAME(31))'
//STEPLIB  DD DSN=PLI.V6R1M0.SIBMZCMP,DISP=SHR
//SYSLIB   DD DSN=BANK.PLI.COPYLIB,DISP=SHR
//SYSLIN   DD DSN=&&XFEROBJ,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//SYSIN    DD DSN=&&XFERTRN,DISP=(OLD,DELETE)
//*
//*--------------------------------------------------------------------*
//* Step 19: DB2 Precompile BNKCREDT                                  *
//*--------------------------------------------------------------------*
//PCCREDT  EXEC PGM=DSNHPC,PARM='HOST(PLI)'
//STEPLIB  DD DSN=DB2.RUNLIB.LOAD,DISP=SHR
//DBRMLIB  DD DSN=BANK.DBRMLIB.DATA(BNKCRED),DISP=SHR
//SYSIN    DD DSN=BANK.PLI.SOURCE(BNKCREDT),DISP=SHR
//SYSCIN   DD DSN=&&CREDTPC,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//SYSUT2   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//*
//*--------------------------------------------------------------------*
//* Step 20: CICS Translate BNKCREDT (after DB2 precompile)           *
//*--------------------------------------------------------------------*
//TRNCRED  EXEC PGM=DFHECP1$,PARM='NOEPILOG'
//STEPLIB  DD DSN=CICS.SDFHLOAD,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZCMP,DISP=SHR
//SYSIN    DD DSN=&&CREDTPC,DISP=(OLD,DELETE)
//SYSPRINT DD SYSOUT=*
//SYSPUNCH DD DSN=&&CREDTTRN,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//*
//*--------------------------------------------------------------------*
//* Step 21: Compile BNKCREDT (CICS/DB2 translated source)            *
//*--------------------------------------------------------------------*
//CCREDT   EXEC PGM=IBMZPLI,
//         PARM='/CICS SQL TASK AGGREGATE LIMITS(NAME(31))'
//STEPLIB  DD DSN=PLI.V6R1M0.SIBMZCMP,DISP=SHR
//SYSLIB   DD DSN=BANK.PLI.COPYLIB,DISP=SHR
//SYSLIN   DD DSN=&&CREDT,DISP=(NEW,PASS),
//            UNIT=SYSALLDA,SPACE=(CYL,(1,1)),
//            DCB=(BLKSIZE=3200,LRECL=80,RECFM=FB)
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//SYSIN    DD DSN=&&CREDTTRN,DISP=(OLD,DELETE)
//*
//*====================================================================*
//*  SECTION 4: LINK-EDIT ALL PROGRAMS                                *
//*====================================================================*
//*
//*--------------------------------------------------------------------*
//* Step 22: Link-Edit BNKPKG (Package - shared library)              *
//*--------------------------------------------------------------------*
//LNKPKG   EXEC PGM=IEWL,
//         PARM='XREF,LIST,LET,RENT,REFR,REUS'
//SYSLIB   DD DSN=CEE.SCEELKED,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZLNK,DISP=SHR
//SYSLIN   DD DSN=&&PKG,DISP=(OLD,DELETE)
//SYSLMOD  DD DSN=BANK.LOADLIB(BNKPKG),DISP=SHR
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//*
//*--------------------------------------------------------------------*
//* Step 23: Link-Edit BNKBATCH (Batch program with DB2)              *
//*--------------------------------------------------------------------*
//LNKBATCH EXEC PGM=IEWL,
//         PARM='XREF,LIST,LET,RENT'
//SYSLIB   DD DSN=CEE.SCEELKED,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZLNK,DISP=SHR
//         DD DSN=DB2.RUNLIB.LOAD,DISP=SHR
//         DD DSN=BANK.LOADLIB,DISP=SHR
//SYSLIN   DD DSN=&&BATCH,DISP=(OLD,DELETE)
//         DD *
  INCLUDE SYSLIB(BNKPKG)
  INCLUDE SYSLIB(DSNELI)
/*
//SYSLMOD  DD DSN=BANK.LOADLIB(BNKBATCH),DISP=SHR
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//*
//*--------------------------------------------------------------------*
//* Step 24: Link-Edit BNKAUDIT (Audit program)                       *
//*--------------------------------------------------------------------*
//LNKAUDIT EXEC PGM=IEWL,
//         PARM='XREF,LIST,LET,RENT'
//SYSLIB   DD DSN=CEE.SCEELKED,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZLNK,DISP=SHR
//         DD DSN=BANK.LOADLIB,DISP=SHR
//SYSLIN   DD DSN=&&AUDIT,DISP=(OLD,DELETE)
//         DD *
  INCLUDE SYSLIB(BNKPKG)
/*
//SYSLMOD  DD DSN=BANK.LOADLIB(BNKAUDIT),DISP=SHR
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//*
//*--------------------------------------------------------------------*
//* Step 25: Link-Edit BNKARCH (Archive program)                      *
//*--------------------------------------------------------------------*
//LNKARCH  EXEC PGM=IEWL,
//         PARM='XREF,LIST,LET,RENT'
//SYSLIB   DD DSN=CEE.SCEELKED,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZLNK,DISP=SHR
//         DD DSN=BANK.LOADLIB,DISP=SHR
//SYSLIN   DD DSN=&&ARCH,DISP=(OLD,DELETE)
//         DD *
  INCLUDE SYSLIB(BNKPKG)
/*
//SYSLMOD  DD DSN=BANK.LOADLIB(BNKARCH),DISP=SHR
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//*
//*--------------------------------------------------------------------*
//* Step 26: Link-Edit BNKFX (Foreign Exchange program)               *
//*--------------------------------------------------------------------*
//LNKFX    EXEC PGM=IEWL,
//         PARM='XREF,LIST,LET,RENT'
//SYSLIB   DD DSN=CEE.SCEELKED,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZLNK,DISP=SHR
//         DD DSN=BANK.LOADLIB,DISP=SHR
//SYSLIN   DD DSN=&&FX,DISP=(OLD,DELETE)
//         DD *
  INCLUDE SYSLIB(BNKPKG)
/*
//SYSLMOD  DD DSN=BANK.LOADLIB(BNKFX),DISP=SHR
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//*
//*--------------------------------------------------------------------*
//* Step 27: Link-Edit BNKCREDT (Credit scoring - CICS/DB2/Tasking)   *
//*--------------------------------------------------------------------*
//LNKCREDT EXEC PGM=IEWL,
//         PARM='XREF,LIST,LET,RENT,REFR,REUS,DYNAM=DLL,AMODE=31,RMODE=ANY'
//SYSLIB   DD DSN=CEE.SCEELKED,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZLNK,DISP=SHR
//         DD DSN=CICS.SDFHLOAD,DISP=SHR
//         DD DSN=DB2.RUNLIB.LOAD,DISP=SHR
//         DD DSN=BANK.LOADLIB,DISP=SHR
//SYSLIN   DD DSN=&&CREDT,DISP=(OLD,DELETE)
//         DD *
  INCLUDE SYSLIB(BNKPKG)
  INCLUDE SYSLIB(DFHELII)
  INCLUDE SYSLIB(DSNELI)
/*
//SYSLMOD  DD DSN=BANK.CICSLOAD(BNKCREDT),DISP=SHR
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//*
//*--------------------------------------------------------------------*
//* Step 28: Link-Edit BNKMAIN (CICS Main dispatcher)                 *
//*--------------------------------------------------------------------*
//LNKMAIN  EXEC PGM=IEWL,
//         PARM='XREF,LIST,LET,RENT,REFR,REUS,DYNAM=DLL,AMODE=31,RMODE=ANY'
//SYSLIB   DD DSN=CEE.SCEELKED,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZLNK,DISP=SHR
//         DD DSN=CICS.SDFHLOAD,DISP=SHR
//         DD DSN=DB2.RUNLIB.LOAD,DISP=SHR
//         DD DSN=BANK.LOADLIB,DISP=SHR
//SYSLIN   DD DSN=&&MAINOBJ,DISP=(OLD,DELETE)
//         DD *
  INCLUDE SYSLIB(BNKPKG)
  INCLUDE SYSLIB(DFHELII)
  INCLUDE SYSLIB(DSNELI)
  ENTRY BNKMAIN
/*
//SYSLMOD  DD DSN=BANK.CICSLOAD(BNKMAIN),DISP=SHR
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//*
//*--------------------------------------------------------------------*
//* Step 29: Link-Edit BNKCUST (CICS Customer program)                *
//*--------------------------------------------------------------------*
//LNKCUST  EXEC PGM=IEWL,
//         PARM='XREF,LIST,LET,RENT,REFR,REUS,DYNAM=DLL,AMODE=31,RMODE=ANY'
//SYSLIB   DD DSN=CEE.SCEELKED,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZLNK,DISP=SHR
//         DD DSN=CICS.SDFHLOAD,DISP=SHR
//         DD DSN=DB2.RUNLIB.LOAD,DISP=SHR
//         DD DSN=BANK.LOADLIB,DISP=SHR
//SYSLIN   DD DSN=&&CUSTOBJ,DISP=(OLD,DELETE)
//         DD *
  INCLUDE SYSLIB(BNKPKG)
  INCLUDE SYSLIB(DFHELII)
  INCLUDE SYSLIB(DSNELI)
/*
//SYSLMOD  DD DSN=BANK.CICSLOAD(BNKCUST),DISP=SHR
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//*
//*--------------------------------------------------------------------*
//* Step 30: Link-Edit BNKACCT (CICS Account program)                 *
//*--------------------------------------------------------------------*
//LNKACCT  EXEC PGM=IEWL,
//         PARM='XREF,LIST,LET,RENT,REFR,REUS,DYNAM=DLL,AMODE=31,RMODE=ANY'
//SYSLIB   DD DSN=CEE.SCEELKED,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZLNK,DISP=SHR
//         DD DSN=CICS.SDFHLOAD,DISP=SHR
//         DD DSN=DB2.RUNLIB.LOAD,DISP=SHR
//         DD DSN=BANK.LOADLIB,DISP=SHR
//SYSLIN   DD DSN=&&ACCTOBJ,DISP=(OLD,DELETE)
//         DD *
  INCLUDE SYSLIB(BNKPKG)
  INCLUDE SYSLIB(DFHELII)
  INCLUDE SYSLIB(DSNELI)
/*
//SYSLMOD  DD DSN=BANK.CICSLOAD(BNKACCT),DISP=SHR
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//*
//*--------------------------------------------------------------------*
//* Step 31: Link-Edit BNKXFER (CICS Fund Transfer program)           *
//*--------------------------------------------------------------------*
//LNKXFER  EXEC PGM=IEWL,
//         PARM='XREF,LIST,LET,RENT,REFR,REUS,DYNAM=DLL,AMODE=31,RMODE=ANY'
//SYSLIB   DD DSN=CEE.SCEELKED,DISP=SHR
//         DD DSN=PLI.V6R1M0.SIBMZLNK,DISP=SHR
//         DD DSN=CICS.SDFHLOAD,DISP=SHR
//         DD DSN=DB2.RUNLIB.LOAD,DISP=SHR
//         DD DSN=BANK.LOADLIB,DISP=SHR
//SYSLIN   DD DSN=&&XFEROBJ,DISP=(OLD,DELETE)
//         DD *
  INCLUDE SYSLIB(BNKPKG)
  INCLUDE SYSLIB(DFHELII)
  INCLUDE SYSLIB(DSNELI)
/*
//SYSLMOD  DD DSN=BANK.CICSLOAD(BNKXFER),DISP=SHR
//SYSPRINT DD SYSOUT=*
//SYSUT1   DD UNIT=SYSALLDA,SPACE=(CYL,(5,2))
//*
//*====================================================================*
//*  SECTION 5: DB2 BIND                                              *
//*====================================================================*
//*
//*--------------------------------------------------------------------*
//* Step 32: DB2 BIND for all DB2 programs                            *
//*--------------------------------------------------------------------*
//BINDALL  EXEC PGM=IKJEFT01
//STEPLIB  DD DSN=DB2.RUNLIB.LOAD,DISP=SHR
//DBRMLIB  DD DSN=BANK.DBRMLIB.DATA,DISP=SHR
//SYSTSPRT DD SYSOUT=*
//SYSTSIN  DD *
  DSN SYSTEM(DB2P)
  BIND PACKAGE(BANKPKG) -
       MEMBER(BNKBATC) -
       ACT(REP) ISO(CS) CURRENTDATA(NO) -
       RELEASE(COMMIT) DEGREE(ANY)
  BIND PACKAGE(BANKPKG) -
       MEMBER(BNKMAIN) -
       ACT(REP) ISO(CS) CURRENTDATA(NO) -
       RELEASE(COMMIT) DEGREE(ANY)
  BIND PACKAGE(BANKPKG) -
       MEMBER(BNKCUST) -
       ACT(REP) ISO(CS) CURRENTDATA(NO) -
       RELEASE(COMMIT) DEGREE(ANY)
  BIND PACKAGE(BANKPKG) -
       MEMBER(BNKACCT) -
       ACT(REP) ISO(CS) CURRENTDATA(NO) -
       RELEASE(COMMIT) DEGREE(ANY)
  BIND PACKAGE(BANKPKG) -
       MEMBER(BNKXFER) -
       ACT(REP) ISO(CS) CURRENTDATA(NO) -
       RELEASE(COMMIT) DEGREE(ANY)
  BIND PACKAGE(BANKPKG) -
       MEMBER(BNKCRED) -
       ACT(REP) ISO(CS) CURRENTDATA(NO) -
       RELEASE(COMMIT) DEGREE(ANY)
  BIND PLAN(BANKPLAN) -
       PKLIST(BANKPKG.*) -
       ACT(REP) ISO(CS) VALIDATE(BIND)
  END
/*
//*
