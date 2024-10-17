#[allow(dead_code)]
pub trait Logger {
  /// Log a message at the given verbosity level.
  fn log(&self, verbosity: u8, message: &str);
}

#[allow(dead_code)]
struct StdoutLogger;

impl Logger for StdoutLogger {
  fn log(&self, verbosity: u8, message: &str) {
    println!("verbosity={verbosity}: {message}");
  }
}

#[allow(dead_code)]
struct VerbosityFilter {
    max_verbosity: u8,
    inner: StdoutLogger,
}

impl Logger for VerbosityFilter {
    fn log(&self, verbosity: u8, message: &str) {
        if verbosity <= self.max_verbosity {
            self.inner.log(verbosity, message);
        }
    }
}

pub fn print_logger() {
  let logger: VerbosityFilter = VerbosityFilter { max_verbosity: 4, inner: StdoutLogger };
  logger.log(5, "FYI");
  logger.log(2, "Uhoh");
}
