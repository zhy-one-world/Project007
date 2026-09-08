#!/usr/bin/env python3
"""Replace CONSOLE_INFO(...) with _RLOG_(MINFO, ...) across server sources."""

from __future__ import annotations

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SKIP_FILES = {
    ROOT / "def" / "log" / "server_log.hpp",
}
SOURCE_SUFFIXES = {".cpp", ".hpp", ".h", ".cc"}


def find_matching_paren(text: str, open_index: int) -> int:
    depth = 0
    in_string: str | None = None
    i = open_index
    while i < len(text):
        ch = text[i]
        if in_string:
            if ch == "\\" and i + 1 < len(text):
                i += 2
                continue
            if ch == in_string:
                in_string = None
            i += 1
            continue

        if ch in ("'", '"'):
            in_string = ch
            i += 1
            continue

        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth == 0:
                return i
        i += 1
    return -1


def split_top_level_args(args_text: str) -> list[str]:
    args: list[str] = []
    current: list[str] = []
    depth_paren = depth_brace = depth_bracket = 0
    in_string: str | None = None

    for ch in args_text:
        if in_string:
            current.append(ch)
            if ch == "\\":
                continue
            if ch == in_string:
                in_string = None
            continue

        if ch in ("'", '"'):
            in_string = ch
            current.append(ch)
            continue

        if ch == "(":
            depth_paren += 1
        elif ch == ")":
            depth_paren -= 1
        elif ch == "{":
            depth_brace += 1
        elif ch == "}":
            depth_brace -= 1
        elif ch == "[":
            depth_bracket += 1
        elif ch == "]":
            depth_bracket -= 1
        elif ch == "," and depth_paren == depth_brace == depth_bracket == 0:
            arg = "".join(current).strip()
            if arg:
                args.append(arg)
            current = []
            continue

        current.append(ch)

    tail = "".join(current).strip()
    if tail:
        args.append(tail)
    return args


def convert_call(args_text: str) -> str:
    args = split_top_level_args(args_text)
    if not args:
        return "_RLOG_(MINFO, \"\")"

    fmt = args[0]
    if len(args) == 1:
        return f"_RLOG_(MINFO, {fmt})"

    rest = ", ".join(args[1:])
    return f"_RLOG_(MINFO, ::faith::log_detail::format_message({fmt}, {rest}))"


def convert_content(content: str) -> tuple[str, int]:
    pattern = re.compile(r"(?<!//)\bCONSOLE_INFO\s*\(")
    replacements = 0
    pieces: list[str] = []
    last = 0

    for match in pattern.finditer(content):
        start = match.start()
        open_paren = match.end() - 1
        close_paren = find_matching_paren(content, open_paren)
        if close_paren < 0:
            continue

        args_text = content[open_paren + 1 : close_paren]
        replacement = convert_call(args_text)
        pieces.append(content[last:start])
        pieces.append(replacement)
        last = close_paren + 1
        replacements += 1

    pieces.append(content[last:])
    return "".join(pieces), replacements


def ensure_rlog_include(content: str) -> str:
    if "_RLOG_(" not in content:
        return content
    if "#include <rlog.hpp>" in content or '#include "rlog.hpp"' in content:
        return content
    if "#include <common/rlog.hpp>" in content:
        return content

    include_lines = [
        (idx, line)
        for idx, line in enumerate(content.splitlines(keepends=True))
        if line.lstrip().startswith("#include")
    ]
    if not include_lines:
        return '#include <rlog.hpp>\n' + content

    insert_at = include_lines[-1][0] + len(include_lines[-1][1])
    return content[:insert_at] + "#include <rlog.hpp>\n" + content[insert_at:]


def process_file(path: Path) -> int:
    original = path.read_text(encoding="utf-8-sig")
    converted, count = convert_content(original)
    if count == 0:
        return 0

    converted = ensure_rlog_include(converted)
    if converted != original:
        path.write_text(converted, encoding="utf-8-sig")
    return count


def main() -> int:
    total_files = 0
    total_replacements = 0

    for path in sorted(ROOT.rglob("*")):
        if path.suffix not in SOURCE_SUFFIXES:
            continue
        if path in SKIP_FILES:
            continue
        if "build" in path.parts or ".vs" in path.parts:
            continue

        count = process_file(path)
        if count:
            total_files += 1
            total_replacements += count
            print(f"{count:3d}  {path.relative_to(ROOT)}")

    print(f"\nUpdated {total_replacements} call sites in {total_files} files.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
