# AGENTS.md — AI Agent Instructions

**Project**: Bank-PLI
**Purpose**: Enterprise PL/I banking application demonstrating 99%+ language constructs
**Primary Language**: Enterprise PL/I for z/OS V6.1
**Runtime**: z/OS with DB2, VSAM, CICS TS, Language Environment
**Last Updated**: February 19, 2026

---

## Project Overview

Bank-PLI is a comprehensive PL/I banking system that showcases nearly every construct in the Enterprise PL/I language through a realistic banking application. It serves as both a working reference implementation and a learning resource.

**Key characteristics:**

- 10 PL/I source programs (5 CICS online + 4 batch + 1 root package)
- 7 copylib include members (3 data structures + 2 system definitions + 2 cross-cutting)
- 2 JCL jobs (COMPILE.JCL with 32 build steps, RUNBATCH.JCL with 4 execution steps)
- 150+ PL/I language constructs demonstrated
- ~7,300 lines of code

---

## Repository Structure

```text
Bank-PLI/
├── *.PLI                  # PL/I source programs (root level)
│   ├── BNKPKG.PLI         # Root PACKAGE — types, constants, shared declarations
│   ├── BNKMAIN.PLI        # CICS dispatcher — routes by transaction type
│   ├── BNKCUST.PLI        # Customer CRUD via DB2
│   ├── BNKACCT.PLI        # Account management
│   ├── BNKXFER.PLI        # Fund transfer (2-phase DB2 update)
│   ├── BNKCREDT.PLI       # Credit scoring (multitasking, VSAM)
│   ├── BNKBATCH.PLI       # Batch test data generator (DB2 + VSAM + SEQ + PRINT)
│   ├── BNKAUDIT.PLI       # Audit report generator (3 PRINT files)
│   ├── BNKARCH.PLI        # Archive/compliance (REGIONAL(3), TRANSIENT, BACKWARDS)
│   └── BNKFX.PLI          # Foreign exchange rates (PICTURE formats, DEFINE ALIAS)
├── copylib/               # Shared include members (%INCLUDE targets)
│   ├── CUSTSTR.INC        # CUSTOMER_T structure (7 dependents)
│   ├── ACCTSTR.INC        # ACCOUNT_T structure (6 dependents)
│   ├── TXNSTR.INC         # TRANSACTION_T + TXN_TYPE_T ordinal (7 dependents)
│   ├── SQLCA.INC          # DB2 SQLCA declaration (6 dependents)
│   ├── CICSDEF.INC        # CICS DFHCOMMAREA + EIB (5 dependents)
│   ├── ERRHAND.INC        # ON-condition handlers + preprocessor guards (8 dependents)
│   └── PICFMT.INC         # PICTURE format strings (1 dependent — BNKFX)
├── jcl/                   # z/OS Job Control Language
│   ├── COMPILE.JCL        # 32-step build: precompile → translate → compile → link → BIND
│   └── RUNBATCH.JCL       # 4-step batch execution: BNKBATCH → BNKAUDIT → BNKCREDT → IDCAMS
├── docs/
│   └── diagrams/          # Mermaid architecture diagrams (.mmd + .png)
│       ├── README.md      # Diagram inventory, viewing order, color legend
│       └── *.mmd / *.png  # 12 diagram pairs
├── README.md              # Project documentation with architecture, construct coverage
├── DIAGRAM.md             # Prompt used to generate the architecture diagrams
└── LICENSE
```

---

## Program Classification

### CICS Online Programs

| Program | Role | DB2 | CICS | Includes |
| --- | --- | --- | --- | --- |
| BNKMAIN | Transaction dispatcher | CONNECT only | LINK, RECEIVE/SEND MAP, HANDLE ABEND | All 7 |
| BNKCUST | Customer CRUD | CUSTOMER_TABLE | LINK target, COMMAREA | 5 |
| BNKACCT | Account management | Prepared stmts | LINK target, COMMAREA | All 7 |
| BNKXFER | Fund transfer | ACCOUNT_TABLE ×2 | SYNCPOINT, ENQ/DEQ | All 7 |
| BNKCREDT | Credit scoring | %PROCESS SQL | START (multitask), VSAM READ | 6 |

### Batch Programs

| Program | Role | I/O | Includes |
| --- | --- | --- | --- |
| BNKBATCH | Test data generator | DB2 INSERT, VSAM WRITE, SEQ WRITE, PRINT | 6 |
| BNKAUDIT | Audit reporter | 3 PRINT files (AUDITLOG, DETAILRP, SUMMARYR) | 4 |
| BNKARCH | Archive/compliance | REGIONAL(3), TRANSIENT, BACKWARDS READ | 3 |
| BNKFX | Currency conversion | In-memory rate table, PICTURE formatting | 4 |

---

## Coding Conventions

### PL/I Source Standards

- **File extension**: `.PLI` (uppercase) for programs, `.INC` (uppercase) for copylib
- **Naming**: Programs use `BNK` prefix + 2–5 char function code (e.g., `BNKCUST`, `BNKXFER`)
- **Include guard**: ERRHAND.INC uses `%IF ^DECLARED(ERRHAND_INCLUDED)` preprocessor guard
- **Package structure**: All programs reference `BNKPKG` root package via `%INCLUDE`
- **SQL embedding**: Programs with DB2 use `%PROCESS SQL;` directive
- **CICS embedding**: CICS programs use `%PROCESS CICS;` directive
- **Indentation**: 3 spaces
- **Comments**: `/* ... */` block comments; section headers use `/*═══...═══*/` box style
- **Statement termination**: Every statement ends with `;`

### Include Dependency Rules

- **BNKPKG.PLI** is always the first `%INCLUDE` — it provides all shared types and constants
- **ERRHAND.INC** uses a preprocessor guard — safe to include multiple times
- CICS programs include `CICSDEF.INC`; batch programs do not
- DB2 programs include `SQLCA.INC`
- Data structure includes (`CUSTSTR`, `ACCTSTR`, `TXNSTR`) are included as needed per program

### JCL Standards

- **Job names**: `BANKCOMP` (compile), `BANKRUN` (batch execution)
- **Step naming**: Program name used as step name (e.g., `//BNKMAIN EXEC ...`)
- **Libraries**: `BANK.PLI.SOURCE` (source), `BANK.PLI.COPYLIB` (includes), `BANK.LOADLIB` (batch output), `BANK.CICSLOAD` (CICS output)
- **DB2 BIND**: All 6 DB2 programs bound into `BANKPLAN`

---

## Architecture Decisions

### Dispatch Pattern

BNKMAIN uses a table-driven dispatch pattern:

```text
TXN_TYPE    → Program
'CUST'      → BNKCUST  (EXEC CICS LINK)
'ACCT'      → BNKACCT  (EXEC CICS LINK)
'XFER'      → BNKXFER  (EXEC CICS LINK)
'MENU'      → internal handling
'BTCH'      → BNKBATCH (EXEC CICS LINK)
```

### Inter-Program Communication

- **CICS COMMAREA**: Primary data exchange for all LINK calls
- **EXTERNAL variables**: `credit_count` shared between BNKCREDT and BNKMAIN across tasks
- **ENTRY BNKENTRY**: Alternate entry point in BNKMAIN for COBOL interoperability

### Data Access Patterns

- **DB2**: CUSTOMER_TABLE (BNKCUST), ACCOUNT_TABLE (BNKXFER), TRANSACTION_TABLE (BNKBATCH)
- **VSAM KSDS**: BANK.VSAM.CUSTFILE — written by BNKBATCH, read by BNKCREDT
- **Sequential**: BANK.BATCH.SEQFILE — RECSIZE(2048)
- **REGIONAL(3)**: Keyed file access in BNKARCH
- **PRINT**: SYSPRINT (BNKBATCH), AUDITLOG/DETAILRP/SUMMARYR (BNKAUDIT)

---

## Build Pipeline

COMPILE.JCL executes in 5 phases:

1. **Foundation**: Compile + link BNKPKG (base package, no DB2/CICS)
2. **Batch DB2**: DB2 precompile → PL/I compile → link BNKBATCH → BANK.LOADLIB
3. **Batch Pure**: Compile + link BNKAUDIT, BNKARCH, BNKFX → BANK.LOADLIB
4. **CICS Programs**: DB2 precompile → CICS translate → PL/I compile → link → BANK.CICSLOAD
5. **DB2 BIND**: BIND PACKAGE for 6 programs → BIND PLAN BANKPLAN

---

## Editing Guidelines

### When Modifying PL/I Source

- Ensure every `%INCLUDE` has a matching SYSLIB DD in COMPILE.JCL
- If adding DB2 SQL, add `%PROCESS SQL;` and a DB2 precompile step
- If adding CICS commands, add `%PROCESS CICS;` and a CICS translate step
- Test that ERRHAND.INC guard works correctly if included transitively
- Keep BNKPKG as the single source of shared type definitions

### When Modifying JCL

- Match DD names to DCL ... FILE(...) names in PL/I source
- LRECL must match RECSIZE in PL/I ENVIRONMENT attributes
- CICS load modules go to BANK.CICSLOAD; batch to BANK.LOADLIB
- All DB2 programs must appear in the BIND PLAN step

### When Adding New Programs

1. Create `BNK<name>.PLI` in the root directory
2. Start with `%INCLUDE BNKPKG;` and `%INCLUDE ERRHAND;`
3. Add appropriate copylib includes for data structures needed
4. Add compile + link steps in COMPILE.JCL (in the correct phase)
5. If DB2: add precompile step and BIND PACKAGE entry
6. If CICS: add translate step and target BANK.CICSLOAD
7. Update README.md architecture diagram and construct coverage
8. Update RUNBATCH.JCL if the program runs in batch

---

## Architecture Diagrams

12 Mermaid diagrams in `docs/diagrams/` cover the system at multiple levels of detail. See [docs/diagrams/README.md](docs/diagrams/README.md) for the full inventory and viewing order.

Diagrams were generated using the `codebase-diagrams` skill with consistent color coding:

| Color | Meaning |
| --- | --- |
| Red (#FF6B6B) | Main dispatcher (BNKMAIN) |
| Yellow (#F7DC6F) | CICS online programs |
| Green (#96CEB4) | Batch programs |
| Teal (#4ECDC4) | Foundation package (BNKPKG) |
| Blue (#45B7D1) | Data stores |
| Purple (#BB8FCE) | External services / system definitions |
| Gray (#E8E8E8) | Configuration / cross-cutting |

To regenerate PNGs after editing `.mmd` files:

```bash
cd docs/diagrams
for f in *.mmd; do
  mmdc -i "$f" -o "${f%.mmd}.png" --width 3200 --height 2400 --backgroundColor white --scale 2
done
```
