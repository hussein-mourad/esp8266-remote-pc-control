#!/bin/bash

declare module
declare binary
declare ldflags

# Function to check if an environment variable is set
check_env_var() {
  local var_name=$1
  if [[ -z "${!var_name}" ]]; then
    echo "Error: Environment variable $var_name is not set."
    exit 1
  fi
}

# List of required environment variables
required_vars=("BASE_URL" "ESP_USERNAME" "ESP_PASSWORD" "AP_USERNAME" "AP_PASSWORD")

# Check all required environment variables
for var in "${required_vars[@]}"; do
  check_env_var "$var"
done

module="$(head go.mod -n 1 | cut -d ' ' -f2)"
binary=powergo
ldflags="-X '${module}/internal/config.BaseURL=$(printenv BASE_URL)' \
-X '${module}/internal/config.Username=$(printenv ESP_USERNAME)' \
-X '${module}/internal/config.Password=$(printenv ESP_PASSWORD)' \
-X '${module}/internal/config.APSSID=$(printenv AP_USERNAME)' \
-X '${module}/internal/config.APPASS=$(printenv AP_PASSWORD)'
"

go build -ldflags "$ldflags" -o ./tmp/$binary
