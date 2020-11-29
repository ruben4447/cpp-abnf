#include <string>

namespace abnf {
// Consume any whitespace at start of string
void consume_whitespace(std::string& text) {
    while (!text.empty() && (text[0] == ' ' || text[0] == '\t' ||
                             text[0] == '\r' || text[0] == '\n')) {
        text.erase(0, 1);
    }
}

// Consume a string
bool consume_string(std::string& input, std::string& value) {
    value = "";
    char quote = input[0];
    if (quote != '"' && quote != '\'') {
        value = "error: Expected quotation mark at position 0";
        return false;
    }
    input.erase(0, 1);

    while (input.length() > 0) {
        if (input[0] == '\\' && input.length() >= 2) {
            // Escape next character
            value += input[1];
            input.erase(0, 2);
        } else if (input[0] == quote) {
            // Close string
            input.erase(0, 1);
            return true;
        } else {
            value += input[0];
            input.erase(0, 1);
        }
    }

    // STRING HAS FAILED
    input = quote + value + input;
    value = "error: unterminated string literal";
    return false;
}
};  // namespace abnf