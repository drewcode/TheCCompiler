set -e

cd tests

echo "Running Tests"
echo "1) Lexer"

./lexer/test_lexer lexer/inputs/ip1.c > lexer/inputs/op1
diff lexer/inputs/eop1 lexer/inputs/op1
rm lexer/inputs/op1

./lexer/test_lexer lexer/inputs/ip2.c > lexer/inputs/op2
diff lexer/inputs/eop2 lexer/inputs/op2
rm lexer/inputs/op2

echo "Passed"
