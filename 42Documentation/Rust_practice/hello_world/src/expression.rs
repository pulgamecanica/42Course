/// An operation to perform on two subexpressions.
#[derive(Debug)]
#[allow(dead_code)]
enum Operation {
  Add,
  Sub,
  Mul,
  Div,
}

/// An expression, in tree form.
#[derive(Debug)]
#[allow(dead_code)]
enum Expression {
  /// An operation on two subexpressions.
  Op { op: Operation, left: Box<Expression>, right: Box<Expression> },

  /// A literal value
  Value(i64),
}

#[allow(unused)]
fn eval(e: Expression) -> Result<i64, String> {
  match e {
    Expression::Value(val) => return Ok(val),
    Expression::Op { op, left, right } => {
      let left = match eval(*left) {
        Ok(v) => v,
        Err(e) => return Err(e),
      };
      let right = match eval(*right) {
        Ok(v) => v,
        Err(e) => return Err(e),
      };
      match op {
        Operation::Add => return Ok(left + right),
        Operation::Sub => return Ok(left - right),
        Operation::Mul => return Ok(left * right),
        Operation::Div => return {
          if right == 0 {
            Err(String::from("Cannot divide by zero"))
          } else {
            Ok(left / right)
          }
        },
      }
    },
  }
}

#[test]
fn test_value() {
  assert_eq!(eval(Expression::Value(19)), Ok(19));
}

#[test]
fn test_sum() {
  assert_eq!(
      eval(Expression::Op {
          op: Operation::Add,
          left: Box::new(Expression::Value(10)),
          right: Box::new(Expression::Value(20)),
      }),
      Ok(30)
  );
}

#[test]
fn test_recursion() {
  let term1 = Expression::Op {
      op: Operation::Mul,
      left: Box::new(Expression::Value(10)),
      right: Box::new(Expression::Value(9)),
  };
  let term2 = Expression::Op {
      op: Operation::Mul,
      left: Box::new(Expression::Op {
          op: Operation::Sub,
          left: Box::new(Expression::Value(3)),
          right: Box::new(Expression::Value(4)),
      }),
      right: Box::new(Expression::Value(5)),
  };
  assert_eq!(
      eval(Expression::Op {
          op: Operation::Add,
          left: Box::new(term1),
          right: Box::new(term2),
      }),
      Ok(85)
  );
}

#[test]
fn test_error() {
  assert_eq!(
      eval(Expression::Op {
          op: Operation::Div,
          left: Box::new(Expression::Value(99)),
          right: Box::new(Expression::Value(0)),
      }),
      Err(String::from("Cannot divide by zero"))
  );
}
