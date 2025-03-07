#include "logger.hpp"

int main() {
    Logger logger("./log", "main", true, 3);
    logger.debug("This is a debug message.");
    logger.info("This is an info message.");
    logger.warning("This is a warning message.");
    logger.error("This is an error message.");
    logger.fatal("This is a fatal message.");
    return 0;
}
