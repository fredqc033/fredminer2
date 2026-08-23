#!/usr/bin/env bash

# HiveOS sources this file and provides CUSTOM_* variables.

miner_ver() {
    echo "0.1.0"
}

miner_config_echo() {
    local cfg="/hive/miners/custom/fredminer/fredminer.conf"
    [[ -f "$cfg" ]] && cat "$cfg"
}

miner_config_gen() {
    local cfg="/hive/miners/custom/fredminer/fredminer.conf"

    local algo="${CUSTOM_ALGO:-pearlhash}"
    local pool="${CUSTOM_URL:-}"
    local wallet="${CUSTOM_TEMPLATE:-}"
    local pass="${CUSTOM_PASS:-x}"
    local extra="${CUSTOM_USER_CONFIG:-}"

    cat > "$cfg" <<EOF
ALGO='${algo}'
POOL='${pool}'
WALLET='${wallet}'
PASS='${pass}'
EXTRA='${extra}'
EOF
}
