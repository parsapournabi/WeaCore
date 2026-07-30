#!/usr/bin/env bash

set -euo pipefail

###############################################################################
# Defaults
###############################################################################

QRC_FILE="resources.qrc"
DIRECTORIES=("qml" "shaders")

###############################################################################
# Help
###############################################################################

show_help()
{
cat << EOF
Usage:
    update-qrc.sh [OPTIONS]

Description:
    Recursively scans directories and generates a Qt Resource (.qrc) file.

Options:

    -d, --dirs <dir1> <dir2> ...
            Directories to scan.
            Default:
                qml shaders

    -q, --qrc <file>
            Target qrc file.

            Default:
                resources.qrc

    -h, --help
            Show this help.

Examples:

    ./scripts/update-qrc.sh

    ./scripts/update-qrc.sh \
        --dirs qml shaders scripts

    ./scripts/update-qrc.sh \
        --dirs qml shaders \
        --qrc resources/resources.qrc

EOF
}

###############################################################################
# Parse args
###############################################################################

while [[ $# -gt 0 ]]
do
    case "$1" in

        -h|--help)
            show_help
            exit 0
            ;;

        -q|--qrc)
            shift
            [[ $# -eq 0 ]] && {
                echo "Missing qrc file."
                exit 1
            }
            QRC_FILE="$1"
            shift
            ;;

        -d|--dirs)

            DIRECTORIES=()

            shift

            while [[ $# -gt 0 && "$1" != -* ]]
            do
                DIRECTORIES+=("$1")
                shift
            done
            ;;

        *)
            echo "Unknown argument: $1"
            echo
            show_help
            exit 1
            ;;
    esac
done

###############################################################################
# Generate qrc
###############################################################################

mkdir -p "$(dirname "$QRC_FILE")"

{
echo '<RCC>'
echo '    <qresource prefix="/">'

for dir in "${DIRECTORIES[@]}"
do

    [[ ! -d "$dir" ]] && continue

    while IFS= read -r file
    do
        file="${file#./}"

        printf '        <file>%s</file>\n' "$file"

    done < <(find "$dir" -type f | sort)

done

echo '    </qresource>'
echo '</RCC>'

} > "$QRC_FILE"

echo "Generated: $QRC_FILE"
