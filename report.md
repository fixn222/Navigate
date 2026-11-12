# NEXUS audit report

Date: 2026-11-12

## Scope and outcome

This report completes Step 1 (Codebase Audit) from `prompt.md`. No production
code was modified during this audit. The current application is called
**Navigate**; NEXUS is the proposed product direction.

## Architecture overview

The application is a small Qt 6 / Qt Quick desktop program. `main.cpp` creates
`QGuiApplication`, selects the Material style, constructs `Theme` and
`Backend`, and exposes both as QML context properties. `Main.qml` owns all
presentation and invokes the backend's Q_INVOKABLE methods.

`Backend` is currently both a navigation controller and a filesystem service.
It holds a `FileListModel` for the active directory and a flattened `TreeModel`
for the sidebar. Both models synchronously enumerate a directory with `QDir`.
The theme service watches Omarchy's colour file and publishes its accent to QML.

Builds use CMake with C++17, Qt Core/Gui/QML/Quick/QuickControls2, and Qt Test.
The repository also contains a minimal Arch package recipe and desktop entry.

## Current functionality

| Area | Present capabilities |
| --- | --- |
| Navigation | Open a path or item, navigate to parent, refresh, Home and Computer sidebar locations |
| Browsing | Folder-first file list, hidden files currently always shown, flattened expandable directory tree |
| File actions | Create folder, rename one item, move one item to trash, open via system default application, copy a path |
| Interaction | Single selection, double-click/Enter to open, keyboard shortcuts, basic context menu |
| Appearance | Dark Material UI, Omarchy accent monitoring, package icon/desktop entry |
| Tests | One Qt Test verifies directories are listed before files |

## Audit findings

### Blocking product gaps

1. There is no reusable file-operation engine: no copy or move, overwrite
   policy, queue, progress, cancellation, operation history, or asynchronous
   execution.
2. Navigation has no history, forward navigation, breadcrumbs, tabs, split
   browsing, bookmarks, search, filtering, previews, properties, multi-select,
   drag/drop, or a hidden-file preference.
3. There are no NEXUS networking, discovery, trust, transfer, task-center,
   storage-analysis, duplicate-detection, or persistent-data modules.
4. No SQLite or Qt Network dependency is configured. The project remains a
   local-only prototype.

### Architecture and quality concerns

- `Backend` couples UI navigation, filesystem mutation, desktop launching, and
  clipboard work. This will become difficult to test and unsafe to grow.
- `FileListModel::setDirectory` and `TreeModel::appendChildren` synchronously
  traverse the filesystem on the GUI thread. Large, slow, or network-backed
  directories can freeze the window. The tree probes every visible directory
  for children and can become especially expensive.
- `TreeModel` is a flattened reconstruction, not a hierarchical model. It has
  no filesystem change monitoring and can show stale entries after create,
  rename, or trash operations.
- Directory enumeration errors and unreadable paths are not represented in the
  models. The user may only see an empty list.
- A `file(GLOB ...)` is used in CMake. Although `CONFIGURE_DEPENDS` helps,
  explicit source lists make ownership and build changes clearer in a larger
  production project.
- The source style is compressed in several files, which makes error handling,
  reviews, and future ownership harder.
- Theme watching does not re-arm watched paths after Omarchy replaces a
  symlink/file, and `radius` is hard-coded to zero.
- The CMake project uses C++17; the brief requires C++20.
- The package recipe expects a source archive that the repository does not
  produce, and `bin/install` is absent despite the documented Omarchy workflow.

### Security and safety findings

- Current local actions do not verify that a requested path is inside an
  explicitly allowed root. This is acceptable for a user-operated local file
  manager but cannot be reused for remotely supplied paths.
- Rename validates a slash but has no structured conflict response, permission
  diagnostics, or race-resistant operation layer.
- Moving to trash uses Qt's safe `moveToTrash`, which is preferable to direct
  deletion, but failures only return a generic status message.
- `QDesktopServices::openUrl` is invoked on any selected local path. This is
  expected desktop behavior, but all future remote-origin files must require
  explicit user initiation and sanitised paths.
- There is no network listener today, so no current remote attack surface.
  Future discovery and transfer code must not inherit this permissive local
  path handling.

## Prioritized implementation roadmap

1. **Foundation and local-model hardening** — move to C++20; add structured
   result/error types, settings, and filesystem service interfaces; make
   listing asynchronous and cancellable; add tests for errors and hidden files.
2. **File operation engine and task center** — implement queued copy, move,
   rename, mkdir, and trash operations in worker threads with conflict policy,
   progress, cancellation, and a QML task model. This is the prerequisite for
   reliable local UI actions and transfers.
3. **Core file-manager UX** — add navigation history, breadcrumb paths,
   bookmarks, hidden-file preference, filtering/search, multi-selection,
   drag/drop, properties, tabs, and optional split panes while preserving the
   current QML shell.
4. **Persistence** — introduce a small SQLite `DatabaseService` for settings,
   bookmarks, device trust, and task/transfer history, with migration and
   temporary-database tests.
5. **Discovery and device management** — add a versioned, validated UDP
   discovery protocol using Qt Network; maintain discovered versus trusted
   devices separately; persist only explicit trust decisions.
6. **Secure transfers** — add a TCP transfer protocol with pairing and explicit
   incoming acceptance; validate message sizes and paths; constrain output to
   a selected destination; use SHA-256 and resumable part files.
7. **Storage analysis** — add cancellable background directory scanning,
   aggregate file-type statistics and largest-item models, then add duplicate
   hashes with bounded concurrency.
8. **Packaging and quality** — expand tests, add CI-friendly CTest setup,
   package installation workflow, localisation, accessibility checks, and
   manual test scenarios.

## Proposed modular architecture

Keep `Main.qml` as the application shell, but replace the monolithic backend
over time with a small `ApplicationController` that composes these services:

| Module | Responsibility |
| --- | --- |
| `FileSystemService` | Async directory listings, metadata, path validation, filesystem watches |
| `FileOperationManager` | Queued copy/move/rename/mkdir/trash operations and conflict handling |
| `TaskManager` | Unified task lifecycle, progress, cancellation, retained history |
| `NavigationController` | Per-tab history, paths, selection, breadcrumbs, split views |
| `SettingsService` | Persisted UI preferences and bookmarks |
| `DatabaseService` | SQLite connection ownership, migrations, transactional repositories |
| `NetworkDiscoveryService` | Versioned UDP announcements and validated device model |
| `DeviceManager` | Explicit pairing, trusted devices, revoked-device state |
| `TransferManager` | Explicitly accepted TCP sends/receives, hashes, pause/cancel/history |
| `StorageAnalyzer` | Cancellable scans and result models |

Each long-running module should own a worker thread or use bounded Qt
Concurrent jobs. QML should receive models and read-only state plus narrowly
scoped commands; it should not perform file, database, or network work.

## Initial milestones

| Milestone | Goal | Primary files | Tests / acceptance criteria |
| --- | --- | --- |
| 1 | Modernise local foundation | `CMakeLists.txt`, new filesystem/result types, `backend.*` | C++20 build; valid/missing/unreadable directory tests; UI stays responsive while loading |
| 2 | Operation queue | new `fileoperationmanager.*`, `taskmodel.*`, update `backend.*`/QML | Copy/move/mkdir/rename/trash integration tests; cancel and conflict tests; progress visible |
| 3 | Local browsing UX | `Main.qml`, navigation/tab models, settings | Back/forward/breadcrumb/filter/hidden files/bookmarks/manual keyboard tests |
| 4 | Persistence | new database/settings modules, CMake SQLite dependency | Fresh DB migration and CRUD tests using temporary database |
| 5 | Discovery/trust | new network/device modules, Qt Network | Datagram validation and device state tests; no device trusted automatically |
| 6 | Transfer protocol | transfer server/client/manager and task model | Invalid path/message rejection; user acceptance; SHA-256 success/failure; cancellation tests |
| 7 | Storage analysis | analyzer/scanner models and QML page | Cancellation and aggregate/duplicate detection tests on fixture tree |

## Files likely to change

- Existing: `CMakeLists.txt`, `src/main.cpp`, `src/backend.{h,cpp}`,
  `src/filelistmodel.{h,cpp}`, `src/treemodel.{h,cpp}`, `src/Main.qml`,
  `src/theme.{h,cpp}`, `src/resources.qrc`, `tests/navigate_tests.cpp`,
  `README.md`, and package files.
- New local modules: filesystem, operation manager, task model/controller,
  navigation/tab state, settings, database, and their Qt Test suites.
- New network modules: protocol types, discovery, devices, transfer client /
  server / manager, plus focused protocol and integration tests.

## Manual verification performed

- Inspected all tracked application source, QML, build scripts, package assets,
  tests, and documentation.
- Previously verified on this machine: `bin/build`, `bin/test`, and an
  offscreen QML launch. The existing test suite currently passes one test case.

## Recommended next action

Proceed with Milestone 1 only: introduce an asynchronous, tested filesystem
listing service and migrate CMake to C++20 without changing the visible design.
This establishes the safety and responsiveness needed for all later features.
