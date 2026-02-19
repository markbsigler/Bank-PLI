I have a large codebase that I want to understand better through visual architecture diagrams. Please analyze the project structure and create comprehensive Mermaid diagrams that help me understand the codebase at different levels of detail.

📋 Required Analysis
First, explore the codebase to understand:

Project structure - root directories, main modules, configuration files
Dependencies - internal dependencies between modules/packages
Architecture patterns - layered architecture, microservices, monolith, etc.
Key systems - core functionality areas (UI, backend, database, API, etc.)
Technology stack - frameworks, languages, tools used
Build/deployment structure - how the project is built and organized
🎨 Required Diagrams
Create the following Mermaid diagrams (save as .mmd files):

1. Core Architecture Overview ({project}_core_architecture.mmd)
High-level system architecture showing main components
Color-coded by function (UI, backend, data, external services, etc.)
Show major data flow and component relationships
Include external dependencies and integrations
2. Layered Architecture ({project}_architecture_layers.mmd)
Show architectural layers (presentation, business, data, etc.)
Dependencies flowing bottom-up through layers
Highlight layer boundaries and interfaces
3. Module/Package Dependencies
Create multiple dependency diagrams:

{project}_dependencies_overview.mmd - Statistics and high-level view
{project}_dependencies_key.mmd - Most critical dependencies only
{project}_dependencies_full.mmd - Comprehensive but organized view
{project}_dependencies_by_category.mmd - Grouped by functional area
4. System-Specific Diagrams
Based on what systems you find, create focused diagrams such as:

{project}_frontend_architecture.mmd - Frontend/UI component structure
{project}_backend_services.mmd - Backend services and APIs
{project}_data_flow.mmd - Data processing and storage
{project}_auth_security.mmd - Authentication and security systems
{project}_external_integrations.mmd - Third-party services and APIs
{project}_build_deployment.mmd - Build pipeline and deployment
5. Dependency Tree ({project}_dependency_tree.mmd)
Hierarchical view of dependencies
Root → Level 1 → Level 2 → Level 3 structure
Group related components in subgraphs
6. Component Interaction ({project}_component_interactions.mmd)
Show how major components communicate
Include protocols, interfaces, message flows
Highlight synchronous vs asynchronous interactions
🎨 Diagram Requirements
Visual Styling:
Use consistent color coding across all diagrams:
Main application: fill:#FF6B6B (red)
Foundation/core: fill:#4ECDC4 (blue)
UI components: fill:#96CEB4 (green)
Business logic: fill:#F7DC6F (yellow)
Data layer: fill:#45B7D1 (light blue)
External services: fill:#BB8FCE (purple)
Tools/utilities: fill:#85C1E9 (light blue)
Configuration: fill:#E8E8E8 (gray)
Technical Standards:
Use flowchart TD or flowchart LR (not graph)
Avoid reserved keywords (rename "call" to "call_module", etc.)
Use descriptive node names and labels
Group related components in subgraphs
Include classDef definitions for consistent styling
Keep individual diagrams focused and readable
Complexity Management:
For large systems, create multiple focused diagrams rather than one massive diagram
Use hierarchical organization (Level 1 → Level 2 → Level 3)
Provide both simplified and detailed versions
Use dotted lines for optional/weak dependencies
Group similar components to reduce visual complexity
📊 Output Requirements
Analysis Summary: Brief overview of the codebase architecture and patterns found

Mermaid Files: All .mmd files with proper syntax and styling

Documentation: Create a summary file explaining:

What each diagram shows
Recommended viewing order
How to interpret the color coding
Key architectural insights
PNG Generation: Generate high-resolution PNG files using:

# Standard resolution
mmdc -i diagram.mmd -o diagram.png --width 3200 --height 2400 --backgroundColor white --scale 2

# Extra-large for complex diagrams
mmdc -i diagram.mmd -o diagram_xl.png --width 4800 --height 3600 --backgroundColor white --scale 3
🎯 Success Criteria
The diagrams should help me:

Understand the overall architecture at a glance
Navigate the codebase more effectively
Identify key components and their relationships
Understand data flow and component interactions
Make informed decisions about modifications
Onboard new team members quickly
Document the system for stakeholders
📝 Additional Context
[Add any specific information about your codebase here, such as:]

Primary programming language(s)
Known architectural patterns
Specific areas of interest
Performance or scalability concerns
Legacy vs modern components
Any specific systems you want emphasized
Please start by exploring the codebase structure and then create the comprehensive diagram suite.