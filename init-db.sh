#!/bin/bash
set -e

# Create the database and enable PostGIS
psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" <<-EOSQL
    CREATE EXTENSION postgis;
EOSQL
