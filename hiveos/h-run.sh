#!/usr/bin/env bash
set -o pipefail

MINER_DIR="/hive/miners/custom/fredminer"
CONF="$MINER_DIR/fredminer.conf"
LOG_DIR="/var/log/miner/fredminer"
LOG_FILE="$LOG_DIR/fredminer.log"

mkdir -p "$LOG_DIR"

if [[ ! -x "$MINER_DIR/fredminer" ]]; then
    echo "FredMiner binary not found: $MINER_DIR/fredminer" | tee -a "$LOG_FILE"
    exit 1
fi

if [[ ! -f "$CONF" ]]; then
    echo "FredMiner config not found: $CONF" | tee -a "$LOG_FILE"
    exit 1
fi

# shellcheck disable=SC1090
source "$CONF"

cd "$MINER_DIR" || exit 1

# shellcheck disable=SC2086
exec ./fredminer \
    --algo "${ALGO:-pearlhash}" \
    --pool "$POOL" \
    --wallet "$WALLET" \
    --pass "${PASS:-x}" \
    $EXTRA 2>&1 | tee -a "$LOG_FILE"
