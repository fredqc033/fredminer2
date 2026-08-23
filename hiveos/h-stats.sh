#!/usr/bin/env bash

# HiveOS includes this file in the agent context.
# FredMiner production builds will emit machine-readable lines such as:
# FREDSTAT total_ths=258.91 gpu_ths=258.91 accepted=42 rejected=0 uptime=1234

LOG_FILE="/var/log/miner/fredminer/fredminer.log"

total_ths=0
accepted=0
rejected=0
uptime=0
gpu_csv=""

if [[ -f "$LOG_FILE" ]]; then
    line=$(grep 'FREDSTAT ' "$LOG_FILE" | tail -n 1)

    if [[ -n "$line" ]]; then
        total_ths=$(echo "$line" | sed -n 's/.*total_ths=\([^ ]*\).*/\1/p')
        gpu_csv=$(echo "$line" | sed -n 's/.*gpu_ths=\([^ ]*\).*/\1/p')
        accepted=$(echo "$line" | sed -n 's/.*accepted=\([^ ]*\).*/\1/p')
        rejected=$(echo "$line" | sed -n 's/.*rejected=\([^ ]*\).*/\1/p')
        uptime=$(echo "$line" | sed -n 's/.*uptime=\([^ ]*\).*/\1/p')
    fi
fi

[[ -z "$total_ths" ]] && total_ths=0
[[ -z "$accepted" ]] && accepted=0
[[ -z "$rejected" ]] && rejected=0
[[ -z "$uptime" ]] && uptime=0

# Hive's total variable is historically kH/s.
# 1 TH/s = 1,000,000,000 kH/s.
khs=$(awk "BEGIN { printf \"%.0f\", ($total_ths * 1000000000) }")

if [[ -n "$gpu_csv" ]]; then
    hs_json=$(echo "$gpu_csv" | awk -F',' '{
        printf "[";
        for (i=1; i<=NF; i++) {
            if (i>1) printf ",";
            printf "%s", $i;
        }
        printf "]";
    }')
else
    hs_json="[]"
fi

stats=$(cat <<EOF
{"hs":$hs_json,"hs_units":"ths","uptime":$uptime,"ver":"0.1.0","ar":[$accepted,$rejected],"algo":"pearlhash"}
EOF
)
