#!/bin/bash

# 🔧 Configure
PLUGIN_PATH="./"            # Default: current folder, change if needed
OLD_PROJECT_NAME="Articytest"
NEW_PROJECT_NAME="NewProjectName"

# If arguments are passed, override defaults
if [ $# -ge 2 ]; then
    OLD_PROJECT_NAME="$1"
    NEW_PROJECT_NAME="$2"
elif [ $# -ge 1 ]; then
    OLD_PROJECT_NAME="$NEW_PROJECT_NAME"
    NEW_PROJECT_NAME="$1"
fi

echo "🔄 Replacing: ${OLD_PROJECT_NAME} → ${NEW_PROJECT_NAME}"

# --- Check if inside a Git repository once ---
IS_GIT_REPO=$(git rev-parse --is-inside-work-tree 2>/dev/null)

# --- Create a Git commit before making changes, only if there are changes ---
if [ "$IS_GIT_REPO" = "true" ]; then
    # Check for changes in the plugin folder
    if [ -n "$(git status --porcelain "$PLUGIN_PATH")" ]; then
        echo "📦 Creating a backup commit before replacement..."
        git add "$PLUGIN_PATH"
        git commit -m "Backup before renaming project from ${OLD_PROJECT_NAME} to ${NEW_PROJECT_NAME}" --quiet
        echo "✅ Backup commit created."
    else
        echo "ℹ️ No changes to commit in $PLUGIN_PATH, skipping backup commit."
    fi
else
    echo "⚠️ Not a Git repository, skipping backup commit."
fi

# --- Replace all occurrences in .h and .cpp files ---
find "$PLUGIN_PATH" -type f \( -name "*.h" -o -name "*.cpp" \) | while read -r file; do
    if grep -q "${OLD_PROJECT_NAME}" "$file"; then
        echo "✔ Updating $file"
        sed -i "s|${OLD_PROJECT_NAME}|${NEW_PROJECT_NAME}|g" "$file"
    fi
done

echo "✅ Finished replacing all occurrences of ${OLD_PROJECT_NAME} with ${NEW_PROJECT_NAME}"

# --- Ask if user wants to commit the changes ---
if [ "$IS_GIT_REPO" = "true" ]; then
    if [ -n "$(git status --porcelain "$PLUGIN_PATH")" ]; then
        read -p "📦 Do you want to commit the changes after renaming? [y/N] " answer
        case "$answer" in
            [Yy]* )
                git add "$PLUGIN_PATH"
                git commit -m "Rename project from ${OLD_PROJECT_NAME} to ${NEW_PROJECT_NAME}"
                echo "✅ Changes committed."
                ;;
            * )
                echo "⚠️ Changes not committed."
                ;;
        esac
    else
        echo "ℹ️ No changes detected to commit."
    fi
else
    echo "⚠️ Not a Git repository, skipping commit."
fi

# --- Self-update script ---
SCRIPT_PATH="$(realpath "$0")"
sed -i "s|^OLD_PROJECT_NAME=.*|OLD_PROJECT_NAME=\"${NEW_PROJECT_NAME}\"|" "$SCRIPT_PATH"
sed -i "s|^NEW_PROJECT_NAME=.*|NEW_PROJECT_NAME=\"${NEW_PROJECT_NAME}\"|" "$SCRIPT_PATH"
