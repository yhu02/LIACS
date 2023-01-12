#!/bin/sh

# enabling extended globbing, which is used to define file patterns
#   makes it easy to delete all files except the ones specified.
shopt -s extglob

#rm db
rm -f src/db.sqlite3

#delete old models.py
FILE=src/shared/models.py
if [ -f "$FILE" ]; then
    echo "$FILE exists! So we remove it and copy the default data."
    rm src/shared/models.py
    cp src/shared/models.default.py src/shared/models.py
else
    echo "$FILE does not exist, skip copying."
fi

#delete all in src/shared/views except for init.py, newclassifier.py, otherclassifier.py
VIEWSDIRECTORY=src/shared/views
if [ -d "$VIEWSDIRECTORY" ]; then
    echo "$VIEWSDIRECTORY exists, start deletion."
    cd src/shared/views/
    rm -f -v !("__init__.py"|"newclassifier.py"|"otherclassifier.py"|"views.py")
    cd ../../../
else
    echo "$VIEWSDIRECTORY does not exist, skip deletion."
fi

# delete shared migrations except for init.py
MIGRATIONSDIR=src/shared/migrations
if [ -d "$MIGRATIONSDIR" ]; then
    echo "$MIGRATIONSDIR does exist, start deletion."
    cd src/shared/migrations/
    rm -f -v !("__init__.py")
    cd ../../../
else
    echo "$MIGRATIONSDIR does not exist, skip deletion."
fi

# delete models migrations except for init.py
MIGRATIONSDIR=src/model/migrations
if [ -d "$MIGRATIONSDIR" ]; then
    echo "$MIGRATIONSDIR does exist, start deletion."
    cd src/model/migrations/
    rm -f -v !("__init__.py")
    cd ../../../
else
    echo "$MIGRATIONSDIR does not exist, skip deletion."
fi

# delete rules migrations except for init.py
MIGRATIONSDIR=src/rules/migrations
if [ -d "$MIGRATIONSDIR" ]; then
    echo "$MIGRATIONSDIR does exist, start deletion."
    cd src/rules/migrations/
    rm -f -v !("__init__.py")
    cd ../../../
else
    echo "$MIGRATIONSDIR does not exist, skip deletion."
fi

#delete all in src/data/ except for .gitkeep, requirement.txt
DATADIR=src/data/
if [ -d "$DATADIR" ]; then
    echo "$DATADIR exist, start deletion."
    cd src/data/
    rm -f -v !(".gitkeep"|"requirement.txt")
    cd ../../
else
    echo "$DATADIR does not exist, skip deletion."
fi

RED='\033[0;32m'
NC='\033[0m'
echo -e "${RED}Finished deleting demo files.${NC}"
