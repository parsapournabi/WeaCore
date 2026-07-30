#!/usr/bin/env bash

set -e

###############################################################################
# Help
###############################################################################

show_help() {
cat << EOF
rename-all.sh

Rename files, directories and file contents recursively.

Usage:

  rename-all.sh
      -c Module
      -t WeaQuick
      -p include src qml README.md CMakeLists.txt

Options:

  -c, --current    Current name
  -t, --target     Target name
  -p, --paths      Files or directories
  -h, --help       Show help

Examples:

  rename-all.sh \
      -c Module \
      -t WeaQuick \
      -p include src

  rename-all.sh \
      -c Module \
      -t WeaQuick \
      -p README.md CMakeLists.txt

EOF
}

###############################################################################
# Parse Arguments
###############################################################################

CURRENT=""
TARGET=""

PATHS=()

while [[ $# -gt 0 ]]
do
    case "$1" in

        -h|--help)
            show_help
            exit 0
            ;;

        -c|--current)
            CURRENT="$2"
            shift 2
            ;;

        -t|--target)
            TARGET="$2"
            shift 2
            ;;

        -p|--paths)
            shift

            while [[ $# -gt 0 && "$1" != -* ]]
            do
                PATHS+=("$1")
                shift
            done
            ;;

        *)
            echo "Unknown argument: $1"
            exit 1
            ;;
    esac
done

[[ -z "$CURRENT" ]] && {
    echo "--current is required."
    exit 1
}

[[ -z "$TARGET" ]] && {
    echo "--target is required."
    exit 1
}

[[ ${#PATHS[@]} -eq 0 ]] && {
    echo "--paths is required."
    exit 1
}

###############################################################################
# Replace Contents
###############################################################################

replace_file()
{
    local file="$1"

    if grep -Iq . "$file" 2>/dev/null
    then
        sed -i.bak "s/${CURRENT}/${TARGET}/g" "$file"
        rm -f "${file}.bak"
    fi
}

###############################################################################
# Rename File
###############################################################################

rename_file()
{
    local file="$1"

    local dir
    local name
    local new

    dir="$(dirname "$file")"
    name="$(basename "$file")"

    new="${name//$CURRENT/$TARGET}"

    [[ "$name" == "$new" ]] && return

    mv "$file" "$dir/$new"
}

###############################################################################
# Rename Directory
###############################################################################

rename_dir()
{
    local dir="$1"

    local parent
    local name
    local new

    parent="$(dirname "$dir")"
    name="$(basename "$dir")"

    new="${name//$CURRENT/$TARGET}"

    [[ "$name" == "$new" ]] && return

    mv "$dir" "$parent/$new"
}

###############################################################################
# Process
###############################################################################

for path in "${PATHS[@]}"
do

    if [[ ! -e "$path" ]]
    then
        echo "Skip: $path"
        continue
    fi

    ###########################################################################
    # File
    ###########################################################################

    if [[ -f "$path" ]]
    then
        echo "Replace: $path"
        replace_file "$path"

        echo "Rename : $path"
        rename_file "$path"

        continue
    fi

    ###########################################################################
    # Directory
    ###########################################################################

    echo "Processing: $path"

    #
    # Replace contents
    #
    while IFS= read -r -d '' file
    do
        replace_file "$file"
    done < <(
        find "$path" -type f -print0
    )

    #
    # Rename files
    #
    while IFS= read -r -d '' file
    do
        rename_file "$file"
    done < <(
        find "$path" -depth -type f -print0
    )

    #
    # Rename directories
    #
    while IFS= read -r -d '' dir
    do
        rename_dir "$dir"
    done < <(
        find "$path" -depth -type d -print0
    )

done

echo
echo "Done."
