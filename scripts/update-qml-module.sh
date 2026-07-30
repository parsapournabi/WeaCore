#!/usr/bin/env bash

###############################################################################
# update-qml-module.sh
#
# Generate qmldir file recursively.
#
# Example:
#
#   ./scripts/update-qml-module.sh
#       --dir resources/qml
#       --module com.wearily.WeaQuick
#
###############################################################################

set -euo pipefail

###############################################################################
# Defaults
###############################################################################

IMPORT_ROOT="qml"
MODULE_NAME=""
MODULE_VERSION="1.0"

###############################################################################
# Help
###############################################################################

show_help()
{
cat << EOF

Usage:

    update-qml-module.sh [OPTIONS]

Description:

    Generates a qmldir file recursively.

Options:

    -d, --dir <directory>

        Import root directory.

        Default:

            qml

    -m, --module <module>

        Module name.

        Example:

            com.wearily.WeaQuick

    -v, --version <version>

        Module version.

        Default:

            1.0

    -h, --help

        Show this help.

Examples:

    ./scripts/update-qml-module.sh \\
        --module com.wearily.WeaQuick

    ./scripts/update-qml-module.sh \\
        --dir resources/qml \\
        --module com.wearily.WeaQuick

    ./scripts/update-qml-module.sh \\
        --dir resources/qml \\
        --module com.wearily.WeaQuick \\
        --version 2.3

EOF
}

###############################################################################
# Parse arguments
###############################################################################

while [[ $# -gt 0 ]]
do
    case "$1" in

        -h|--help)
            show_help
            exit 0
            ;;

        -d|--dir)
            shift
            IMPORT_ROOT="$1"
            shift
            ;;

        -m|--module)
            shift
            MODULE_NAME="$1"
            shift
            ;;

        -v|--version)
            shift
            MODULE_VERSION="$1"
            shift
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
# Validation
###############################################################################

if [[ -z "$MODULE_NAME" ]]
then
    echo "Error: --module is required."
    exit 1
fi

MODULE_PATH="${MODULE_NAME//./\/}"

TARGET_DIR="${IMPORT_ROOT}/${MODULE_PATH}"

if [[ ! -d "$TARGET_DIR" ]]
then
    echo "Module directory not found:"
    echo "    $TARGET_DIR"
    exit 1
fi

QMLDIR="${TARGET_DIR}/qmldir"

###############################################################################
# Generate
###############################################################################

echo "Generating:"
echo "    $QMLDIR"

{
echo "module ${MODULE_NAME}"
echo

find "$TARGET_DIR" \
    -type f \
    \( \
        -name "*.qml" \
        -o -name "*.js" \
    \) \
    ! -name "qmldir" \
    ! -name "plugins.qmltypes" \
    ! -name ".gitkeep" \
    ! -name ".gitignore" \
    ! -name "*.qmlc" \
    ! -name "*.jsc" \
| sort \
| while read -r FILE
do

    RELATIVE="${FILE#$TARGET_DIR/}"

    COMPONENT="$(basename "$RELATIVE")"

    COMPONENT="${COMPONENT%.*}"

    echo "${COMPONENT} ${MODULE_VERSION} ${RELATIVE}"

done

} > "$QMLDIR"

echo
echo "Done."
