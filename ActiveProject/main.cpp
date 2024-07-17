#include <bits/stdc++.h>
using namespace std;

bool IsOperator(const char &ch);
bool IsDelimiter(const char &ch);

enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    INT,
    DOUBLE,
    OPERATOR,
    STRING,
    DELIMITER,
    SEMICOLON,
    COMMENT,
    ERROR
};

// Class to represent a token
class Token {
    TokenType type;
    std::string value;
public:
    Token(const TokenType &type, std::string value) : type(type), value(std::move(value))
    {}

    [[nodiscard]] TokenType getType() const
    {
        return type;
    }
    [[nodiscard]] string print() const
    {
        string output {"<" + tokenTypeToString(type) + ", \"" + value + "\">"};
        return output;
    }
    bool operator==(const Token &rhs) const
    {
        if(this->type == rhs.type && this->value == rhs.value)
            return true;
        return false;
    }

private:
    static std::string tokenTypeToString(const TokenType &t)
    {
        switch (t) {
            case TokenType::IDENTIFIER: return "IDENTIFIER";
            case TokenType::KEYWORD: return "KEYWORD";
            case TokenType::INT: return "INT";
            case TokenType::DOUBLE: return "DOUBLE";
            case TokenType::OPERATOR: return "OPERATOR";
            case TokenType::STRING: return "STRING";
            case TokenType::DELIMITER: return "DELIMITER";
            case TokenType::COMMENT: return "COMMENT";
            case TokenType::SEMICOLON: return "SEMICOLON";
            default: return "ERROR";
        }
    }
};

// Lexical analyzer class
class Lexer {
    std::string source;
    size_t position;
    std::unordered_set<std::string> Keywords;
    std::unordered_set<std::string> operators;

public:
    explicit Lexer(std::string source): source(std::move(source)), position(0)
    {
        // Initialize keywords
        Keywords = {"if", "else", "print", "get", "int", "double", "for"};
        operators = {"++", "+", "*", "-", "/", "=", "<", "<=", ">", ">=", "==", "!="};
    }

    std::vector<Token> tokenize()
    {
        std::vector<Token> tokens;

        while (position < source.length())
        {
            if(isspace(source.at(position)))
            {
                ++position;
            }
            else if(isalpha(source.at(position)))
            {
                tokens.push_back(parseIdentifierOrKeyword());
            }
            else if(isdigit(source.at(position)) && source.at(position)!='0')
            {
                tokens.push_back(parseIntOrDouble());
            }
            else if(IsOperator(source.at(position)))
            {
                tokens.push_back(parseOperator());
            }
            else if(source.at(position)== '\\')
            {
                Token temp = parseComment();
                if(temp != Token{TokenType::COMMENT, "comment"})
                    tokens.emplace_back(temp);
            }
            else if(IsDelimiter(source.at(position)))
            {
                tokens.push_back(parseDelimiter());
            }
            else if(source.at(position) == ';')
            {
                tokens.push_back(parseSemicolon());
            }
            else if(source.at(position) == '"') {
                tokens.push_back(parseString());
            }
            else
            {
                tokens.emplace_back(TokenType::ERROR, std::string(1, source[position]));
                ++position;
            }
        }

        return tokens;
    }

private:
    Token parseIdentifierOrKeyword()
    {
        const size_t start = position;
        while (position<source.length() && isalnum(source.at(position)))
        {
            ++position;
        }
        std::string value = source.substr(start, position - start);

        if (Keywords.contains(value))
        {
            return {TokenType::KEYWORD, value};
        }
        return {TokenType::IDENTIFIER, value};
    }
    Token parseIntOrDouble()
    {
        const size_t start = position;
        while (position<source.length() && isdigit(source.at(position)))
        {
            ++position;
        }
        if(position<source.length() && isalpha(source.at(position)))
        {
            const std::string value = source.substr(start, position - start);
            return {TokenType::ERROR, value};
        }
        if(position<source.length() && source.at(position) == '.')
        {
            ++position;
            if(position<source.length() && !isdigit(source.at(position)))
            {
                const std::string value = source.substr(start, position - start);
                return {TokenType::ERROR, value};
            }
            while(position<source.length() && isdigit(source.at(position)))
            {
                ++position;
            }
            if(position<source.length() && isalpha(source.at(position)))
            {
                const std::string value = source.substr(start, position - start);
                return {TokenType::ERROR, value};
            }

            const std::string value = source.substr(start, position - start);
            return {TokenType::DOUBLE, value};
        }
        std::string value = source.substr(start, position - start);
        return {TokenType::INT, value};
    }
    Token parseOperator()
    {
        const size_t start = position;
        while (position<source.length() && IsOperator(source.at(position)))
        {
            ++position;
        }
        std::string value = source.substr(start, position - start);

        if (operators.contains(value))
        {
            return {TokenType::OPERATOR, value};
        }

        return {TokenType::ERROR, value};
    }
    Token parseDelimiter()
    {
        std::string value(1, source.at(position));
        ++position;
        return {TokenType::DELIMITER, value};
    }
    Token parseSemicolon()
    {
        ++position;
        return {TokenType::SEMICOLON, ";"};
    }
    Token parseComment()
    {
        const size_t start = position;

        ++position;
        if(position<source.length() && source.at(position)=='*')
        {
            while(position<source.length())
            {
                ++position;
                if(position<source.length() && source.at(position)=='*')
                {
                    ++position;
                    if(position<source.length() && source.at(position)=='\\')
                    {
                        ++position;
                        return {TokenType::COMMENT, "comment"};
                    }
                }
            }
        }
        else
        {
            ++position;
            return {TokenType::ERROR, string{source.at(position-1)}};
        }

        std::string value = source.substr(start, position - start);
        return {TokenType::ERROR, value};
    }
    Token parseString()
    {
        const size_t start = position;

        ++position;
        if(position>=source.length())
        {
            std::string value = source.substr(start, position - start);
            return {TokenType::ERROR, value};
        }

        while(source.at(position)!='"')
        {
            ++position;
            if(position>=source.length())
            {
                std::string value = source.substr(start, position - start);
                return {TokenType::ERROR, value};
            }
        }
        ++position;

        std::string value = source.substr(start, position - start);
        return {TokenType::STRING, value};
    }
};

int main()
{
    std::ifstream in_file {"../command.txt"};
    if(!in_file) {
        system("pause");
        return 1;
    }

    std::ostringstream oss {};
    char ch {};
    while(in_file.get(ch))
        oss << ch;

    Lexer lexer(oss.str());
    std::vector<Token> tokens = lexer.tokenize();

    bool noError = true;
    for (const Token &token : tokens)
    {
        if(token.getType() == TokenType::ERROR)
            noError = false;
    }

    std::ofstream out_file {"../output.txt"};
    if(!out_file) {
        system("pause");
        return 1;
    }

    if(noError) {
        for (const Token &token : tokens)
        {
            string temp = token.print();
            out_file << temp << endl;
        }
    }
    else {
        cout << "error" << endl;
    }

    system("pause");
    return 0;
}

bool IsOperator(const char &ch)
{
    if(ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='=' || ch=='<' || ch=='>' || ch=='!')
        return true;

    return false;
}
bool IsDelimiter(const char &ch)
{
    if(ch=='(' || ch==')' || ch=='{' || ch=='}')
        return true;

    return false;
}