#!/usr/bin/env bash
set -euo pipefail

VERSION="${1:-0.1.0}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$ROOT/build/fredminer"
DIST="$ROOT/dist"
STAGE="$(mktemp -d)"
DEST="$STAGE/fredminer"

trap 'rm -rf "$STAGE"' EXIT

if [[ ! -x "$BIN" ]]; then
    echo "ERROR: $BIN not found."
    echo "Build first:"
    echo "  cmake -S . -B build -DCMAKE_BUILD_TYPE=Release"
    echo "  cmake --build build -j\$(nproc)"
    exit 1
fi

mkdir -p "$DEST" "$DIST"

cp "$BIN" "$DEST/fredminer"
cp "$ROOT/hiveos/h-manifest.conf" "$DEST/"
cp "$ROOT/hiveos/h-config.sh" "$DEST/"
cp "$ROOT/hiveos/h-run.sh" "$DEST/"
cp "$ROOT/hiveos/h-stats.sh" "$DEST/"

chmod +x "$DEST/fredminer" "$DEST"/h-*.sh

# Stamp package version.
sed -i "s/CUSTOM_VERSION=\"[^\"]*\"/CUSTOM_VERSION=\"$VERSION\"/" "$DEST/h-manifest.conf"

TARBALL="$DIST/fredminer-$VERSION.tar.gz"
tar -czf "$TARBALL" -C "$STAGE" fredminer

echo "Created: $TARBALL"
echo
tar -tzf "$TARBALL"
echo
sha256sum "$TARBALL"
