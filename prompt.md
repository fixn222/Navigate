# Project: NEXUS — Local-First File Manager & P2P Sharing Platform

Act as a senior C++ systems engineer, Qt/QML architect, and software engineer. Your task is to help develop my existing C++ and Qt/QML file manager into a production-quality, local-first file management and peer-to-peer file-sharing application.

## 1. Most important instruction

I have already implemented a UI and file manager using C++ and QML.

**Do not rebuild the application from scratch.**

First, inspect the entire existing codebase and understand:

- Current project structure.
- CMake configuration and dependencies.
- C++ classes and responsibilities.
- QML components and navigation.
- Existing file browsing and filesystem operations.
- Backend-to-QML communication.
- Current application state and architecture.

Preserve existing functionality, design, and working components.

Do not replace existing implementations unless there is a clear technical reason. Explain proposed architectural changes before making significant modifications.

## 2. Project vision

NEXUS is a native Linux-first desktop application that combines:

1. A powerful local file manager.
2. A local network file-sharing system.
3. A storage analysis tool.
4. A background task manager.
5. A device discovery and management system.

The application should work without cloud services or external servers. Devices should communicate directly over the local network.

## 3. Technology requirements

Use the following technologies where appropriate:

- C++20.
- Qt 6.
- Qt Quick / QML.
- CMake.
- Qt Network.
- Qt Concurrent where useful.
- SQLite for persistent application data.
- JSON for communication protocols.
- GoogleTest or Qt Test for testing.
- Linux filesystem APIs when Qt abstractions are insufficient.

Prefer modern C++ practices:

- RAII.
- Smart pointers.
- Strong typing.
- Separation of concerns.
- Signals and slots.
- Asynchronous operations.
- Thread safety.
- Error handling.
- Dependency injection where appropriate.

Avoid unnecessary dependencies.

## 4. Core features to implement

### A. File manager

Improve the existing file manager rather than replacing it.

Implement or improve:

- Directory navigation.
- Back and forward navigation.
- Breadcrumb navigation.
- Multiple tabs.
- Split-pane browsing.
- File and folder operations.
- Copy, move, rename, delete.
- Drag and drop.
- File selection.
- Multi-selection.
- Context menus.
- File properties.
- Hidden files toggle.
- File previews.
- Bookmarks and favorites.
- Keyboard shortcuts.
- Search and filtering.
- Permission information.

Use asynchronous filesystem operations for expensive tasks so the UI never freezes.

### B. File operation engine

Create a reusable file operation system.

Requirements:

- Copy files and directories.
- Move files and directories.
- Rename files.
- Delete files.
- Create directories.
- Handle existing destination files.
- Report errors.
- Track operation progress.
- Support cancellation.
- Avoid blocking the UI thread.
- Handle large files and directories.

Design a task queue that supports multiple background operations.

### C. Local network device discovery

Implement automatic discovery of NEXUS devices on the same local network.

Requirements:

- UDP-based discovery.
- Device announcements.
- Device identification.
- Device name and IP address.
- Online/offline status.
- Periodic discovery.
- Device list updates.
- Network interface awareness.

Do not depend on a centralized server.

Design a discovery protocol with versioning and validation.

### D. Peer-to-peer file sharing

Implement direct file transfers between discovered devices.

Requirements:

- TCP-based file transfers.
- Device pairing.
- Explicit transfer acceptance.
- Sending multiple files.
- Sending directories.
- Transfer progress.
- Transfer speed.
- Estimated remaining time.
- Pause and resume where technically feasible.
- Cancellation.
- Transfer history.
- Integrity verification using SHA-256.
- Safe handling of filenames and destination paths.

Never automatically accept incoming transfers.

Never trust filenames or paths received from another device. Prevent directory traversal and unauthorized filesystem access.

### E. Device management

Create a device management system.

Features:

- Discovered devices.
- Paired devices.
- Device identification.
- Device names.
- Connection status.
- Last-seen timestamps.
- Remove paired devices.
- Revoke trust.
- Connection and transfer status.

Store trusted-device information locally using SQLite.

### F. Storage analyzer

Implement a storage analysis module.

Features:

- Directory size calculation.
- Largest files.
- Largest directories.
- File type statistics.
- Storage usage visualization.
- Duplicate file detection using hashes.
- Scan progress.
- Cancellation.

Scanning must be asynchronous and should not freeze the UI.

### G. Background task center

Build a unified task management system.

Display:

- Active operations.
- Completed operations.
- Failed operations.
- Cancelled operations.
- Transfer progress.
- File operation progress.
- Error messages.
- Operation history.

Support task cancellation and appropriate cleanup.

## 5. Architecture

Before implementing features, inspect the current architecture and propose how to extend it.

Aim for modular components with clear responsibilities.

Suggested conceptual modules:

- FileSystemService
- FileOperationManager
- DirectoryScanner
- SearchService
- StorageAnalyzer
- NetworkDiscoveryService
- DeviceManager
- TransferServer
- TransferClient
- TransferManager
- TaskManager
- DatabaseService
- ApplicationSettings

These are suggested responsibilities, not mandatory class names. Reuse existing classes where appropriate.

Keep business logic out of QML.

QML should handle presentation and user interaction. C++ should handle filesystem operations, networking, persistence, and application logic.

Expose functionality to QML using appropriate QObject properties, signals, slots, and models.

## 6. Security requirements

Treat security as a core requirement.

- Never automatically accept incoming transfers.
- Require explicit user approval.
- Validate incoming network messages.
- Validate file paths.
- Prevent directory traversal.
- Restrict writes to the selected destination.
- Verify file integrity.
- Handle interrupted connections safely.
- Avoid exposing unnecessary network services.
- Do not trust discovered devices automatically.
- Separate device discovery from device trust.
- Set reasonable limits on incoming requests and transfer sizes.
- Avoid logging sensitive file contents or credentials.

Document the security assumptions and limitations.

## 7. Development workflow

Work incrementally.

### Step 1 — Codebase audit

Inspect the existing repository.

Identify:

- Existing features.
- Missing features.
- Architecture problems.
- Bugs.
- Dependencies.
- Potential security issues.

Do not modify code yet.

Produce:

1. A concise architecture overview.
2. Existing functionality inventory.
3. A prioritized implementation roadmap.
4. Proposed architecture improvements.
5. Files likely to require modification.

### Step 2 — Implementation planning

Break the work into small, independently testable milestones.

For every milestone, specify:

- Goal.
- Files to create or modify.
- Dependencies.
- Implementation details.
- Tests.
- Acceptance criteria.

### Step 3 — Implementation

Implement one milestone at a time.

For each milestone:

1. Explain the intended changes.
2. Implement the feature.
3. Build the project.
4. Run relevant tests.
5. Fix compilation errors and regressions.
6. Summarize modified files.
7. Explain how to test the feature manually.

Do not silently skip failed tests.

Do not leave placeholder implementations while claiming a feature is complete.

### Step 4 — Quality

Maintain:

- Clean C++ code.
- Consistent naming.
- Clear class responsibilities.
- Useful comments.
- Proper error handling.
- Thread-safe operations.
- No unnecessary blocking operations.
- No memory leaks.
- No duplicated business logic.

Update documentation as the project evolves.

## 8. Important rules

- Preserve my existing QML UI and design.
- Do not rewrite working components unnecessarily.
- Do not implement everything in one massive change.
- Do not introduce unnecessary frameworks.
- Do not use cloud infrastructure.
- Do not use a centralized server for local transfers.
- Do not assume the application only runs on one Linux distribution.
- Ask before making destructive changes.
- If a requirement conflicts with the existing architecture, explain the trade-off before changing it.
- Prefer working, tested implementations over large amounts of unfinished code.

## Your first task

Start with **Step 1: Codebase Audit**.

Inspect the repository and report your findings.

Do not write or modify any code until you have completed the audit and presented the implementation roadmap.
