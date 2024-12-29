#!/bin/bash
declare module
declare binary
declare ldflags

module="$(head go.mod -n 1 | cut -d ' ' -f2)"
binary=powergo
ldflags="-X '${module}/internal/config.BaseURL=$(printenv BASE_URL)' \
-X '${module}/internal/config.Username=$(printenv ESP_USERNAME)' \
-X '${module}/internal/config.Password=$(printenv ESP_PASSWORD)' \
-X '${module}/internal/config.APSSID=$(printenv AP_USERNAME)' \
-X '${module}/internal/config.APPASS=$(printenv AP_PASSWORD)'
"

go build -ldflags "$ldflags" -o ./tmp/$binary
