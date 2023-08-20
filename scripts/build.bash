#!/bin/bash
set -euxo pipefail
cd "${BASH_SOURCE[0]%/*}"/..

env_name=nucleo_f303k8

mkdir -p build
for id in {140..149}; do
  sed -i -e "/int can_id/s/[0-9]\+/${id}/" src/main.cpp
  pio run -e $env_name
  mv ".pio/build/${env_name}/firmware.bin" build/servo_driver_firm_${id}.bin
done
