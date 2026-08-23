#!/usr/bin/env bash

miner_ver() {
    echo "0.2.1-dev"
}

miner_config_echo() {
    echo "FredMiner uses HiveOS Flight Sheet variables directly."
}

miner_config_gen() {
    # Nothing to generate.
    # HiveOS provides CUSTOM_ALGO, CUSTOM_URL, CUSTOM_TEMPLATE,
    # CUSTOM_PASS and CUSTOM_USER_CONFIG in the environment.
    return 0
}
