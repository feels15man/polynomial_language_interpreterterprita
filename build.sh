
/opt/homebrew/Cellar/bison/3.8.2/bin/bison -y -d gram.y
if [ $? -ne 0 ]; then
    echo "Bison compile error"
    exit 1;
fi

flex scanner.l
if [ $? -ne 0 ]; then
    echo "flex compile error"
    exit 1;
fi

gcc y.tab.c lex.yy.c -o main -ll $1
if [ $? -ne 0 ]; then
    echo "gcc compile error"
    exit 1;
fi

echo "Built successesfully"