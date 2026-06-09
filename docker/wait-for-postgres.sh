#!/bin/bash
set -e

host="$PGHOST"
port="$PGPORT"
user="$PGUSER"
db="$PGDATABASE"

echo "Waiting for PostgreSQL as $host:$port..."

until PGPASSWORD=$PGPASSWORD psql -h "$host" -p "$port" -U "$user" -d "$db" -c '\q' 2>/dev/null; do
  >&2 echo "PostgreSQL is unavailable - sleeping"
  sleep 1
done

>&2 echo "PostgreSQL is up - executing command"
exec "$@"