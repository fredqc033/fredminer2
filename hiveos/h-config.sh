#!/usr/bin/env bash

miner_ver() {
    echo "0.2.0-dev"
}

miner_config_echo() {
    local cfg="/hive/miners/custom/fredminer/fredminer.conf"
    [[ -f "$cfg" ]] && cat "$cfg"
}

miner_config_gen() {
    local cfg="/hive/miners/custom/fredminer/fredminer.conf"

    local algo="${CUSTOM_ALGO:-pearlhash}"
    local pool="${CUSTOM_URL:-prl-us.kryptex.network:7048}"
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
