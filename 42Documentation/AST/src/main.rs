#[derive(Debug, Clone, PartialEq)]
enum Token {
    Number(i64),
    Plus,
    Minus,
    Multiply,
    Divide,
    LeftParen,
    RightParen,
    Identifier(String),
}

#[derive(Debug, Clone)]
enum ASTNode {
    NumberLiteral(i64),
    BinaryOperation {
        operator: Token,
        left: Box<ASTNode>,
        right: Box<ASTNode>,
    },
    UnaryOperation {
        operator: Token,
        operand: Box<ASTNode>,
    },
    Variable(String),
}

struct Parser {
    tokens: Vec<Token>,
    position: usize,
}

// Expression: Term ((PLUS | MINUS) Term)*
// Term: Factor ((MUL | DIV) Factor)*
// Factor: NUMBER | LEFT_PAREN Expression RIGHT_PAREN
impl Parser {
    fn new(tokens: Vec<Token>) -> Self {
        Self {
            tokens,
            position: 0,
        }
    }
    fn parse(&mut self) -> Result<ASTNode, String> {
        let mut node = self.parse_term()?;

        while let Some(token) = self.current_token().cloned() {
            match token {
                Token::Plus | Token::Minus => {
                    self.position += 1;
                    let rhs = self.parse_term()?;
                    node = ASTNode::BinaryOperation {
                        left: Box::new(node),
                        operator: token,
                        right: Box::new(rhs),
                    };
                }
                _ => break,
            }
        }
        Ok(node)
    }

    fn parse_term(&mut self) -> Result<ASTNode, String> {
        let mut node = self.parse_factor()?;

        while let Some(token) = self.current_token().cloned() {
            match token {
                Token::Multiply | Token::Divide => {
                    self.position += 1;
                    let rhs = self.parse_factor()?;
                    node = ASTNode::BinaryOperation {
                        left: Box::new(node),
                        operator: token,
                        right: Box::new(rhs),
                    };
                }
                _ => break,
            }
        }
        Ok(node)
    }

    fn parse_factor(&mut self) -> Result<ASTNode, String> {
        if let Some(token) = self.current_token().cloned() {
            match token {
                Token::Number(value) => {
                    self.position += 1;
                    Ok(ASTNode::NumberLiteral(value))
                }
                Token::LeftParen => {
                    self.position += 1;
                    let node = self.parse()?;
                    if let Some(Token::RightParen) = self.current_token().cloned() {
                        self.position += 1;
                        Ok(node)
                    } else {
                        Err("Expected closing parenthesis".into())
                    }
                }
                _ => Err("Unexpected token".into()),
            }
        } else {
            Err("Unexpected end of input".into())
        }
    }

    fn current_token(&self) -> Option<&Token> {
        self.tokens.get(self.position)
    }
}

fn traverse_preorder(node: &ASTNode) {
    match node {
        ASTNode::NumberLiteral(value) => {
            println!("Number: {}", value);
        }
        ASTNode::BinaryOperation {
            operator,
            left,
            right,
        } => {
            println!("Operator: {:?}", operator);
            traverse_preorder(left);
            traverse_preorder(right);
        }
        ASTNode::UnaryOperation { operator, operand } => {
            println!("Operator: {:?}", operator);
            traverse_preorder(operand);
        }
        ASTNode::Variable(name) => {
            println!("Variable: {}", name);
        }
    }
}

//   +
//  / \
// 3   *
//    / \
//   5   2
//

//   +
//  / \
// 3   *
//    / \
//   4   -
//      / \
//     2   1

fn main() {
    let tokens = vec![
        Token::Number(3),
        Token::Plus,
        Token::Number(4),
        Token::Multiply,
        Token::LeftParen,
        Token::Number(2),
        Token::Minus,
        Token::Number(1),
        Token::RightParen,
    ];

    let mut parser = Parser::new(tokens);
    match parser.parse() {
        Ok(ast) => {
            println!("{:#?}", ast);
            traverse_preorder(&ast);
        }
        Err(err) => eprintln!("Error: {}", err),
    }
}
