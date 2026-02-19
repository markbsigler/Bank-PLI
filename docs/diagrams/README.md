# Bank-PLI Architecture Diagrams

Comprehensive Mermaid architecture diagrams for the Bank-PLI enterprise PL/I banking application.

## Diagram Inventory

| # | File | Type | Purpose |
| --- | ------ | ------ | --------- |
| 1 | `bankpli_core_architecture.mmd` | Flowchart | High-level system architecture — all programs, entry points, data stores, build infrastructure |
| 2 | `bankpli_architecture_layers.mmd` | Flowchart | 8-layer architectural view from Presentation to Physical Persistence |
| 3 | `bankpli_dependencies_overview.mmd` | Flowchart | Project statistics and dependency fan-out summary |
| 4 | `bankpli_dependencies_key.mmd` | Flowchart | Critical dependencies only — dispatch, DB2, VSAM, EXTERNAL |
| 5 | `bankpli_dependencies_full.mmd` | Flowchart | Complete %INCLUDE matrix — all 9 programs × 7 copylib members |
| 6 | `bankpli_dependencies_by_category.mmd` | Flowchart | Dependencies grouped by functional area (Structures, System, Cross-Cutting, Foundation) |
| 7 | `bankpli_data_flow.mmd` | Flowchart | End-to-end data flow — inputs, processing, persistence, reports |
| 8 | `bankpli_backend_services.mmd` | Flowchart | CICS backend services — dispatch table, COMMAREA, DB2, VSAM, multitasking |
| 9 | `bankpli_build_deployment.mmd` | Flowchart | 32-step build pipeline — precompile, translate, compile, link, BIND |
| 10 | `bankpli_batch_processing.mmd` | Flowchart | RUNBATCH.JCL execution flow — 4-step batch pipeline |
| 11 | `bankpli_dependency_tree.mmd` | Flowchart | Hierarchical tree — Foundation → Executables → Runtime → Deploy |
| 12 | `bankpli_component_interactions.mmd` | Sequence | Runtime interactions — CICS LINK, COMMAREA, DB2 SQL, VSAM, multitasking |

## Recommended Viewing Order

### For New Team Members

1. **Core Architecture** → Get the big picture
2. **Architecture Layers** → Understand the stack
3. **Dependency Tree** → See the hierarchy
4. **Dependencies Key** → Focus on critical paths

### For Developers

1. **Dependencies Full** → See every %INCLUDE
2. **Data Flow** → Trace data through the system
3. **Component Interactions** → Understand runtime behavior
4. **Backend Services** → Dive into CICS services

### For Operations / Build Engineers

1. **Build & Deployment** → Understand the 32-step compile pipeline
2. **Batch Processing** → Understand RUNBATCH.JCL flow
3. **Dependencies by Category** → Know what breaks if a copylib changes

## Color Coding

All diagrams use a consistent color scheme:

| Color | Hex | Meaning |
| ------- | ----- | --------- |
| 🔴 Red | `#FF6B6B` | Main application / dispatcher (BNKMAIN) |
| 🟡 Yellow | `#F7DC6F` | CICS online programs |
| 🟢 Green | `#96CEB4` | Batch programs |
| 🔵 Teal | `#4ECDC4` | Foundation package (BNKPKG) |
| 🔵 Light Blue | `#85C1E9` | Data structures / tools |
| 🟣 Purple | `#BB8FCE` | External services / system definitions |
| ⚪ Gray | `#E8E8E8` | Configuration / cross-cutting / reports |
| 🔵 Blue | `#45B7D1` | Data stores (DB2, VSAM, files) |

## Key Architectural Insights

### Program Classification

- **5 CICS Online**: BNKMAIN, BNKCUST, BNKACCT, BNKXFER, BNKCREDT
- **4 Batch**: BNKBATCH, BNKAUDIT, BNKARCH, BNKFX
- **1 Root Package**: BNKPKG (included by all 9 programs)

### Dependency Hotspots

- **BNKPKG** → 9 dependents (universal)
- **ERRHAND** → 8 dependents (all except BNKAUDIT)
- **CUSTSTR** → 7 dependents
- **TXNSTR** → 7 dependents
- **ACCTSTR** → 6 dependents
- **SQLCA** → 6 dependents (all DB2 programs)
- **CICSDEF** → 5 dependents (all CICS programs)
- **PICFMT** → 1 dependent (BNKFX only)

### Communication Protocols

- **EXEC CICS LINK** — BNKMAIN dispatches to BNKCUST, BNKACCT, BNKXFER, BNKBATCH
- **CICS COMMAREA** — Data exchange for all LINK calls
- **EXTERNAL variables** — Cross-task sharing (BNKCREDT → BNKMAIN)
- **DB2 SQL** — 6 programs access DB2 (3 tables)
- **VSAM I/O** — BNKBATCH writes, BNKCREDT reads KSDS
- **ENTRY BNKENTRY** — COBOL interoperability entry point in BNKMAIN

## Rendering Diagrams

### Using Mermaid CLI (mmdc)

```bash
# Install
npm install -g @mermaid-js/mermaid-cli

# Render single diagram
mmdc -i bankpli_core_architecture.mmd -o bankpli_core_architecture.png \
     --width 3200 --height 2400 --backgroundColor white --scale 2

# Render all diagrams
for f in *.mmd; do
  mmdc -i "$f" -o "${f%.mmd}.png" \
       --width 3200 --height 2400 --backgroundColor white --scale 2
done
```

### Using VS Code

Install the **Markdown Preview Mermaid Support** extension to preview `.mmd` files directly.

### Using GitHub

Mermaid diagrams render natively in GitHub Markdown. Wrap the content in:

````markdown
```mermaid
<diagram content>
```
````

## Codebase Statistics

- **Total files**: 20 (10 PL/I + 7 copylib + 2 JCL + 1 README)
- **Total LOC**: ~7,300
- **PL/I constructs**: 150+ demonstrated
- **Technology stack**: Enterprise PL/I V6.1, z/OS, DB2, VSAM, CICS TS, Language Environment
