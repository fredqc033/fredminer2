#!/usr/bin/env bash
set -o pipefail

MINER_DIR="/hive/miners/custom/fredminer"
LOG_DIR="/var/log/miner/fredminer"
LOG_FILE="$LOG_DIR/fredminer.log"

mkdir -p "$LOG_DIR"
cd "$MINER_DIR" || exit 1

if [[ ! -x "$MINER_DIR/fredminer" ]]; then
    echo "FredMiner binary not found: $MINER_DIR/fredminer" | tee -a "$LOG_FILE"
    exit 1
fi

ALGO="${CUSTOM_ALGO:-pearlhash}"
POOL="${CUSTOM_URL:-prl-us.kryptex.network:7048}"
WALLET="${CUSTOM_TEMPLATE:-}"
PASS="${CUSTOM_PASS:-x}"
EXTRA="${CUSTOM_USER_CONFIG:-}"

echo "FredMiner HiveOS wrapper v0.2.1-dev" | tee -a "$LOG_FILE"
echo "Algorithm: $ALGO" | tee -a "$LOG_FILE"
echo "Pool: $POOL" | tee -a "$LOG_FILE"
echo "Wallet template: $WALLET" | tee -a "$LOG_FILE"

if [[ -z "$WALLET" ]]; then
    echo "ERROR: CUSTOM_TEMPLATE / wallet is empty." | tee -a "$LOG_FILE"
    exit 2
fi

# shellcheck disable=SC2086
exec ./fredminer \
    --algo "$ALGO" \
    --pool "$POOL" \
    --wallet "$WALLET" \
    --pass "$PASS" \
    $EXTRA 2>&1 | tee -a "$LOG_FILE"
