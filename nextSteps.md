# Next Steps

## Architecture Summary

This repository contains a small C++17 chess program with a CLI game loop, a board and move-generation core, and a simple chess engine.

The current runtime entrypoint is `src/ChessGameLoop.cpp`. It creates a `ChessGame`, prompts for a mode, displays the board, accepts coordinate moves, asks the engine for moves when needed, and stores FEN snapshots in `gameHistory`.

The chess rules are split across the core files:

- `Board` owns the mutable board state, side to move, castling rights, en passant square, halfmove clock, fullmove number, FEN output, move parsing, move application, check detection, checkmate detection, stalemate detection, and legal-move filtering.
- `MoveGen` generates pseudo-legal piece moves for the side to move. `Board::generateLegalMoves()` then filters those moves by making them on a copied board and rejecting moves that leave the moving king in check.
- `Move` stores source square, target square, captured piece, promotion piece, and bit flags such as capture, promotion, en passant, and castling.
- `ChessEngine` consumes legal moves from `Board`, orders moves, evaluates positions with piece-square tables, and searches with alpha-beta and quiescence search.
- `PieceSquareTables` contains PeSTO-style material and positional tables plus tapered evaluation.

The design is usable for focused fixes. It should not be rewritten wholesale. The main risk is that board mutation is split between `makeMove()` and `updateGameState()`, so tests must verify both raw board effects and full game-state transitions.

## File-by-File Audit

| File | Current role | Implemented parts | Incomplete or TODO | Issues and missing tests |
| --- | --- | --- | --- | --- |
| `CMakeLists.txt` | Defines the C++17 `chess` executable and include paths. | Builds core source files and engine source file. | No test target. | Missing `src/engine/piece_tables.cpp`, causing linker errors when building the engine. Needs build test coverage. |
| `README.md` | Basic project description and manual build/run note. | States PvP and PvComputer exist. | Does not document tests or CMake commands. | Should eventually mention supported build and test commands once tests exist. |
| `main.cpp` | Scratch/debug driver outside the CMake target. | Creates a board, prints FEN, mutates squares, prints pseudo-legal moves. | Not maintained as a real app entrypoint. | Does not compile on Linux because of Windows-style include separators, include case mismatch, and `b.print()` call without an argument. Needs either cleanup, removal, or conversion to an example target later. |
| `src/ChessGameLoop.cpp` | Interactive CLI game loop and executable entrypoint. | Mode selection, player moves, engine moves, checkmate/stalemate stop conditions, FEN display. | Advertises `draw?` but does not handle it. No robust input validation for engine level. | Black input currently depends on board perspective through `parseMove(input, board.whiteToMove)`, which is ambiguous for standard chess notation. CLI is hard to test because logic is private and interactive. |
| `src/core/board.hpp` | Public board API and state. | Piece enum, castling rights, board state, move generation interface, parsing, checkmate/stalemate declarations. | No explicit constructor initialization for all game-state fields beyond in-class defaults and `Board()` filling squares. | Public mutable state makes tests easy but allows invalid states. Preserve API for now. |
| `src/core/board.cpp` | Board setup, printing, FEN, move application, state updates, check detection, legal move filtering, parser helpers. | Start position, FEN output, pseudo-to-legal filtering, check/checkmate/stalemate, en passant state, halfmove/fullmove updates. | Commented-out old attack detection remains. Promotion choice is intentionally queen-only. | Confirmed bugs in state reset, castling rights, capture-promotion, and castling mapping. Needs broad board and game-state tests. |
| `src/core/move.hpp` | Move flag enum and move data structure. | Basic flags and `Move` fields. | No helper methods for combined flags. | Combined flags require bitwise checks in all users; tests should include capture-promotion and other combined-flag cases. |
| `src/core/move.cpp` | Converts a `Move` to coordinate notation. | Outputs `from` + `to` and appends `q` for promotions. | Promotion piece is always printed as queen. | Needs tests for normal move strings and queen promotion strings. |
| `src/core/moveGen.hpp` | Declares move-generation functions. | Static move-generation API by piece. | None explicitly. | Includes `"Board.hpp"` with wrong case; works on case-insensitive filesystems but is non-portable. |
| `src/core/moveGen.cpp` | Generates pseudo-legal moves for pawns, knights, bishops, rooks, queens, kings, en passant, promotion, and castling. | Most normal piece movement exists. En passant, queen-only promotion, and castling are partially implemented. | Comment says sliding movement could be deduplicated later. Castling section still has TODO notes. | Confirmed bugs in black pawn captures and black queenside castling. Castling generation lacks rook-presence and king-start-square validation. Attack detection duplicates logic from `Board`. |
| `src/core/utils.hpp` | Square helper functions. | `rank`, `file`, `onBoard`, `fileChar`, `rankChar`, and `ValidRank`. | None. | Needs small tests for edge squares and algebraic helpers. |
| `src/engine/engine.hpp` | Engine public interface and private search/evaluation declarations. | Engine level enum, search API, stats API, TT entry type. | Transposition table methods are declared but not implemented. | Stubbed private methods should stay future work unless needed by current behavior. |
| `src/engine/engine.cpp` | Search, move ordering, engine levels, evaluation hooks, quiescence search. | Random move mode, alpha-beta, move ordering, tapered eval integration, basic stats. | `evaluatePieceSquares`, `evaluateKingSafety`, `evaluatePawnStructure`, `hashPosition`, `storeTTEntry`, and `probeTTEntry` are stubs. Quiescence search is marked urgent. | Confirmed bug where `getBestMove(board, timelimit)` ignores the supplied time limit. Search manually flips `whiteToMove` without full game-state updates. Needs engine smoke tests. |
| `src/engine/piece_tables.hpp` | Declares PeSTO-style tables and evaluation helpers. | Table declarations, game phase and tapered evaluation API. | None explicit. | `getPieceType()` assumes an enum layout that does not match the actual grouped-by-color `Piece` enum. Needs mapping tests for every piece. |
| `src/engine/piece_tables.cpp` | Defines piece values, tables, game phase, and tapered evaluation. | Material/positional tables and side-to-move relative tapered evaluation. | Richer evaluation remains in `engine.cpp` stubs. | Depends on the incorrect `getPieceType()` mapping. Needs evaluation sanity tests. |
| `.gitignore` | Ignores build outputs, executables, objects, VSCode, and CMake artifacts. | Appropriate ignore patterns exist. | None. | `build/` and `.vscode/` are already tracked despite ignore rules; cleanup should be separate. |
| `.vscode/setting.json` | Local VSCode CMake settings. | Configures Ninja and Windows CMake path. | Local-only project configuration. | Should not be part of engine fixes. Consider untracking in a separate cleanup. |
| `build/*` | Generated CMake and compiler artifacts. | Existing build metadata. | Not source code. | Tracked generated files are noisy and already modified. Do not edit as part of engine fixes. |

## Confirmed Bugs

| Severity | Bug | Cause | Proposed fix | Regression test |
| --- | --- | --- | --- | --- |
| Critical | CMake target fails to link. | `CMakeLists.txt` compiles `engine.cpp`, which references `PieceSquareTables`, but omits `piece_tables.cpp`. | Add `src/engine/piece_tables.cpp` to the build. Prefer creating a `chess_core` library shared by the CLI and tests. | Configure and build with CMake; verify the executable links. |
| Critical | Capture-promotion does not apply promotion. | `Board::makeMove()` checks `m.flags == CAPTURE` before promotion and only handles exact `PROMOTION`, not `CAPTURE + PROMOTION`. | Use bitwise flag checks. Move/capture first, then apply promotion when `m.flags & PROMOTION`. | White pawn captures on the last rank and becomes a queen; black pawn does the same. | 
| Critical | Castling rights are updated from the wrong board state. | `updateGameState()` calls `UpdateCastlingRights()` after `makeMove()`, but `UpdateCastlingRights()` checks `squares[m.from]`, which is empty after the move. | Update rights using `m.from`, `m.to`, and captured rook squares rather than reading moved piece from `m.from` after mutation, or capture the moved piece before mutation. | Moving either king clears both castling rights; moving/capturing each rook clears only the matching side. |
| Critical | Castling rook-side mapping is reversed. | `UpdateCastlingRights()` maps square `0` to white kingside and `7` to white queenside, but `a1` is queenside and `h1` is kingside. Same issue for black rooks. | Map `a1`/`0` to white queenside, `h1`/`7` to white kingside, `a8`/`56` to black queenside, and `h8`/`63` to black kingside. | Moving or capturing each rook clears the correct FEN castling letter. |
| Critical | Black queenside castling is generated as `e8b8`. | `MoveGen::addKingMoves()` pushes black queenside castling target `57` instead of `58`. | Generate black queenside castle from `60` to `58`; keep rook movement from `56` to `59`. | With a clear black queenside path, legal moves contain `e8c8`, not `e8b8`. |
| High | Black diagonal-right pawn capture emits the wrong move. | In black pawn `capture_right`, the non-promotion branch pushes `capture_left` and `b.squares[capture_left]`. | Push `capture_right` and `b.squares[capture_right]`. | Black pawn on `d4` with a white piece on `e3` generates `d4e3`, not `d4c3`. |
| High | Black capture-promotion has incorrect promotion metadata. | Some black capture-promotion branches test the wrong rank or set `promotion` to `EMPTY`. | Check the target capture square rank and set `promotion` to `B_QUEEN`. | Black pawn capture onto rank 1 has `PROMOTION`. `CAPTURE` and promotion piece `B_QUEEN`.|
| High | `setStartPos()` does not reset all state. | It writes starting pieces but does not clear all squares or reset all counters and side-to-move fields. | Start by filling all squares with `EMPTY`, then set pieces, `whiteToMove = true`, `enPassantSquare = -1`, `halfmoveClock = 0`, `fullMoveNumber = 1`, and all castling rights true. | Dirty board state followed by `setStartPos()` produces exact starting FEN and 20 legal moves. |
| High | Castling can be generated without verifying rook presence or king start square. | Castling generation checks rights and empty path squares, but not that the expected king and rook are on their starting squares. | Require the side's king on `e1`/`e8` and the relevant rook on `a1`, `h1`, `a8`, or `h8`. | Castling is not generated when the rook is missing even if castling rights are true. |
| High | Engine search updates only `whiteToMove` in child positions. | Search calls `makeMove()` and then manually flips `whiteToMove`, skipping en passant, clocks, and castling updates. | Use a helper or local pattern that calls `makeMove()` followed by `updateGameState()` for search child boards. | Engine child-search tests preserve castling/en-passant semantics after candidate moves. |
| Medium | `ChessEngine::getBestMove(board, timelimit)` ignores its argument. | It calls `getBestMove(board, maxDepth_, timeLimit_)` instead of using `timelimit`. | Pass `timelimit` through. | Call overload with a small time limit and verify the configured search path observes it. |
| Medium | Piece-square table type mapping is wrong. | `getPieceType()` uses `(piece - 1) / 2`, but `Piece` enum is grouped as all black pieces then all white pieces. | Replace formula with explicit mapping from each piece enum to pawn, knight, bishop, rook, queen, or king index. | Unit test every piece maps to the expected table index. |
| Medium | CLI move parsing for black is non-standard. | `handlePlayerMove()` passes `board.whiteToMove` as perspective, so black input is flipped. | For regular chess coordinate notation, parse player input with white perspective for both sides. Keep board display orientation independent. | After `e2e4`, black can input `e7e5` using standard coordinates. |
| Low | `main.cpp` scratch driver does not compile on Linux. | Backslash includes, include case mismatch, and missing `print(bool)` argument. | Either fix it as a portable scratch/example file or leave it excluded and document it as non-production. | Optional compile check if retained as an example target. |
| Low | Duplicate attack-detection logic exists. | `Board::isSquareAttacked()` and free `isSquareAttacked()` in `moveGen.cpp` implement similar logic separately. | For bug-fix pass, leave behavior unchanged unless tests expose divergence. Later, extract one shared helper. | Add attack tests against both castling and check detection paths. |

## Incomplete Features and TODOs

These are not required to fix currently broken behavior and should remain separate from the immediate bug-fix pass.

| Area | Current state | Future direction |
| --- | --- | --- |
| Promotion choice | Promotion always becomes a queen, and notation always appends `q`. | Add user/API support for choosing queen, rook, bishop, or knight only after the current queen-only behavior is covered by tests. |
| Draw offers | CLI advertises `draw?`, but no handling exists. | Add explicit draw-offer flow later, including accept/decline behavior and non-interactive testable controller logic. |
| Game termination | CLI uses `exit(0)` for quit/resign. | Return structured game outcomes from handler methods instead of terminating inside helpers. |
| Engine king safety evaluation | Stub returns `0.0f`. | Implement after rule correctness and baseline engine tests exist. |
| Engine pawn structure evaluation | Stub returns `0.0f`. | Implement isolated pawn, doubled pawn, passed pawn, and related heuristics later. |
| Engine standalone piece-square evaluation | Stub returns `0.0f`; tapered evaluation is already used elsewhere. | Remove if redundant or implement as a wrapper once engine design is clarified. |
| Transposition table | Hash, store, and probe functions are stubs. | Implement Zobrist hashing and TT use only after move/state correctness is tested. |
| Quiescence search | Marked urgent in source and uses selective capture pruning. | Stabilize with search tests before optimizing. Avoid speculative changes during rule-fix pass. |
| CLI/controller tests | CLI logic is private and interactive. | Extract a small non-interactive controller or command parser only when adding CLI tests becomes necessary. |
| Repository hygiene | `build/` and `.vscode/` are tracked despite `.gitignore`. | Remove tracked generated/local files in a separate cleanup commit, not mixed with engine behavior fixes. |

## Fix Plan

The immediate implementation should be narrow and regression-test driven.

- [x] Populate README with gameplay notation.
- [ ] Add a test target without introducing third-party dependencies.
- [x] Fix the build by including `piece_tables.cpp`.
- [ ] Add baseline tests that document current intended behavior before changing rules.
- [x] Fix board-state reset in `setStartPos()`.
- [x] Fix combined-flag handling in `Board::makeMove()`, especially capture-promotion.
- [ ] Fix castling rights updates, rook-side mapping, rook presence checks, and black queenside target.
- [ ] Verify updateGameState correctly assigns `enPassantSquare` and `halfmoveClock`.
- [ ] Fix black pawn capture generation and black capture-promotion metadata.
- [ ] Fix piece-square table type mapping.
- [ ] Fix the `getBestMove(board, timelimit)` overload.
- [ ] Update engine child-board move application to call `makeMove()` and `updateGameState()` together.
- [ ] Decide whether to parse CLI moves with standard coordinates for both colors; default recommendation is standard coordinates.
- [ ] Run the full configured test suite and document results.

Implementation notes:

- Keep the public `Board`, `Move`, and `ChessEngine` APIs stable unless a test proves an API-level bug.
- Prefer local helpers only where they remove real duplication, such as an internal `applyMoveForSearch()` pattern.
- Do not implement promotion choice, draw offers, richer evaluation, or transposition tables during the bug-fix pass.
- Do not edit tracked `build/` files as part of engine work.

## Unit Test Plan

Add a simple C++ test executable, for example `tests/chess_tests.cpp`, and register it with CTest. The tests can use small assertion helpers and manually constructed board positions. A FEN parser is not required for this pass.

### Board setup and FEN

- Starting position FEN equals `rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1`.
- Starting position has 20 legal moves.
- `setStartPos()` clears stale pieces and resets side to move, en passant square, clocks, and castling rights.
- Empty board construction initializes all squares to `EMPTY`.

### Piece movement rules

- Pawns move one square forward when empty.
- Pawns move two squares only from their starting rank and only when both path squares are empty.
- Pawns cannot move forward into occupied squares.
- Pawns capture diagonally and cannot capture forward.
- Black pawn diagonal-right regression: `d4e3` is generated correctly.
- Knights generate L-shaped moves, can jump over blockers, and cannot capture friendly pieces.
- Bishops move diagonally, stop at blockers, capture opposing pieces, and do not wrap files.
- Rooks move rank/file, stop at blockers, capture opposing pieces, and do not wrap ranks.
- Queens combine rook and bishop movement.
- Kings move one square, reject friendly captures, and avoid moving into check via legal-move filtering.

### Captures and special moves

- Normal capture replaces the target and clears the source.
- Capture resets the halfmove clock.
- En passant target is set after a double pawn push.
- En passant capture removes the passed pawn and places the capturing pawn correctly.
- Queen-only promotion works for quiet promotions.
- Capture-promotion works for white and black and promotes to queen.

### Turn order and game state

- `updateGameState()` toggles `whiteToMove`.
- Fullmove number increments after black moves.
- Halfmove clock resets after pawn moves and captures.
- Halfmove clock increments after quiet non-pawn moves.
- En passant square clears after a non-double-pawn move.

### Check, checkmate, and stalemate

- `isSquareAttacked()` detects pawn, knight, bishop, rook, queen, and king attacks.
- Blocked sliding attacks are not treated as attacks.
- `isCheck()` detects checks against each king.
- Legal move generation excludes moves that leave the moving king in check.
- Known checkmate position returns `isCheckmate() == true`.
- Known stalemate position returns `isStalemate() == true`.

### Castling

- Starting position does not generate castling because pieces block the path.
- Castling is generated for each side when path is clear, rights are present, king is not in check, and transit squares are not attacked.
- Castling is not generated when the relevant rook is missing.
- Castling is not generated when the king is not on the start square.
- Moving a king clears both rights for that color.
- Moving or capturing each rook clears only the correct side.
- White kingside target is `g1`, white queenside target is `c1`.
- Black kingside target is `g8`, black queenside target is `c8`.
- Castling move application places the rook on `f1`, `d1`, `f8`, or `d8` as appropriate.

### Parser, notation, and CLI-adjacent behavior

- `parseSquare()` accepts `a1` through `h8` and rejects invalid files/ranks.
- `parseMove()` rejects strings not exactly four characters.
- `Move::toString()` returns coordinate notation for quiet moves and appends `q` for promotion.
- If standard CLI coordinates are adopted, black player input should parse `e7e5` as `e7` to `e5`, not a side-relative move.

### Engine and evaluation

- Random engine level returns a legal move from the starting position.
- Engine returns an invalid move marker when no legal moves exist.
- One-argument `getBestMove(board, timelimit)` uses the supplied time limit.
- Search child boards update game state consistently after candidate moves.
- `PieceSquareTables::getPieceType()` maps every piece enum to the expected table type.
- Tapered evaluation returns equal or near-equal score for symmetrical starting material from the same side-to-move perspective.

## Future Implementation Plan

After the current rule bugs are fixed and covered by tests, future work can proceed in small independent passes.

### Promotion choice

Add promotion piece selection to the move parser and `Move` representation behavior already present. Keep default queen promotion for existing CLI input until the UI can request a piece explicitly.

Suggested future tests:

- `e7e8q`, `e7e8r`, `e7e8b`, and `e7e8n` parse to the expected promotion piece.
- Capture-promotion supports all promotion pieces.
- CLI rejects invalid promotion suffixes.

### Draw and game outcomes

Replace direct `exit(0)` calls with structured outcomes such as continue, quit, resign, draw offer, draw accepted, checkmate, and stalemate. This would make CLI/controller behavior testable without running an interactive process.

Suggested future tests:

- Resignation returns the correct winner.
- Quit exits the game loop cleanly.
- Draw offer can be accepted or declined.

### Engine evaluation

Implement richer evaluation only after core rules are reliable. Candidate features include king safety, pawn structure, passed pawns, mobility, and endgame-specific king activity.

Suggested future tests:

- Evaluation rewards material advantage.
- Evaluation rewards legal mobility in simple positions.
- Pawn-structure terms are isolated and deterministic.

### Transposition table

Implement Zobrist hashing with side to move, castling rights, en passant file, and piece-square keys. Then wire `storeTTEntry()` and `probeTTEntry()` into alpha-beta.

Suggested future tests:

- Different side-to-move values produce different hashes.
- Different castling rights produce different hashes.
- Make/unmake is not currently available, so hash tests should use constructed positions.
- TT probe returns exact, lower-bound, and upper-bound values according to depth and alpha/beta rules.

### Code organization

Only after behavior is well tested, consider extracting duplicated helpers:

- Shared color helpers such as `isWhitePiece()` and `isBlackPiece()`.
- Shared attack detection used by both check detection and castling validation.
- A single move-application helper for engine search child boards.

## Assumptions and Review Questions

Assumptions for the next implementation pass:

- The current public APIs should be preserved unless a concrete bug requires a change.
- The immediate test setup should use dependency-free C++ plus CTest.
- Promotion remains queen-only for now.
- Advanced engine features remain future work.
- Tracked `build/` and `.vscode/` files should not be edited as part of rule fixes.
- Standard chess coordinates should be used for both players in the CLI unless there is a deliberate product decision to keep side-relative black input.

Questions for review:

1. Should `main.cpp` remain as a scratch driver, become a supported example target, or be removed later?
2. Should CLI input use standard coordinates for both sides?
3. Should generated build artifacts and local IDE settings be removed from version control in a separate cleanup change?
4. Should a future FEN parser be added to simplify tests, or should tests continue constructing positions directly for now?

## Verification Commands

After implementation, run the following commands from the repository root. These commands are expected to work once the build and test target are added.

```sh
cmake -S . -B /tmp/chess-build -G Ninja
cmake --build /tmp/chess-build
ctest --test-dir /tmp/chess-build --output-on-failure
```

If Ninja is not installed locally, use the platform's default CMake generator instead:

```sh
cmake -S . -B /tmp/chess-build
cmake --build /tmp/chess-build
ctest --test-dir /tmp/chess-build --output-on-failure
```

Do not claim the fixes work until these commands, or an equivalent local test run, complete successfully.
