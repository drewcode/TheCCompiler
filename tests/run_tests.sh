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

echo "Lexer Tests Successful"

echo "2) Parser"

./parser/test_parser parser/inputs/ip1.c > parser/inputs/op1
diff parser/inputs/eop parser/inputs/op1
rm parser/inputs/op1

echo "Parser Tests Successful"

echo "Passed"
