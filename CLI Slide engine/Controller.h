#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Item.h"
#include <string>
#include <iostream>
#include <sstream>
#include <variant>
#include <map>
#include <memory>
#include <cctype>
#include <stdexcept>



class OptionLib{
public:
    OptionLib() = default;
    void addOption(const std::string& optionName, std::variant<int, double, std::string> option);
    std::pair<bool, std::variant<int, double, std::string>> findOption(const std::string&) const;
    void setOption(const std::string& optionName, std::variant<int, double, std::string> option);
    const std::map<std::string, std::variant<int, double, std::string>>& options() const;

private:
    std::map<std::string, std::variant<int, double, std::string>> m_optLib;
};

class CommandLib{
public:
    CommandLib();
    std::pair<bool, std::shared_ptr<class Command>> findCommand(const std::string&);
    const std::map<std::string, std::shared_ptr<class Command>>& commands() const;
    void showCommandLib(int indent = 0) const;


private:
    std::map<std::string, std::shared_ptr<class Command>> m_comLib;    
};

class Command {
public:
    Command() = default;
    virtual void execute(Project& project) = 0;
    virtual ~Command() = default;
    virtual void setLib(std::shared_ptr<class CommandLib>) {};
    virtual void showCommand(int intend = 0) const {};
    virtual void showOptions(int intend = 0) const {};
    virtual std::pair<bool, std::shared_ptr<Command>> findCommand(std::string CommandName) const {return {false, nullptr};};
    virtual std::pair<bool, std::variant<int, double, std::string>> findOption(const std::string&) const {return {false, std::variant<int, double, std::string>(0)};};
    virtual void setOption(const std::string& optionName, std::variant<int, double, std::string> value) {};
    virtual bool isEnd() const { return false; }
};

class Save : public Command {
public:
    Save();
    void execute(Project& project) override;
    std::pair<bool, std::variant<int, double, std::string>> findOption(const std::string&) const override;
    void showOptions(int intend = 0) const;
    void setOption(const std::string& optionName, std::variant<int, double, std::string> value) override;
private:
    OptionLib m_optLib;    
};

class End : public Command {
public:
    void execute(Project& project) override;
    bool isEnd() const override { return true;}

private:
    std::map<std::string, std::shared_ptr<Command>> m_comLib;
};

class AddShape : public Command {
public:
    AddShape();
    void showCommand(int intend = 0) const;
    void execute(Project& project) override {};
    std::pair<bool, std::shared_ptr<Command>> findCommand(std::string CommandName) const override;

private:
    std::map<std::string, std::shared_ptr<Command>> m_comLib;
};

class AddRect : public Command {
public:
    AddRect();
    std::pair<bool, std::variant<int, double, std::string>> findOption(const std::string&) const override; 
    void showOptions(int intend = 0) const;
    void execute(Project& project) override;
    void setOption(const std::string& optionName, std::variant<int, double, std::string> value) override;
private:
    OptionLib m_opt;      
};

class RemoveShape : public Command {
public:
    RemoveShape();
    std::pair<bool, std::variant<int, double, std::string>> findOption(const std::string&) const override;
    void showOptions(int intend = 0) const;
    void execute(Project& project) override;
    void setOption(const std::string& optionName, std::variant<int, double, std::string> value) override;

private:
    OptionLib m_opt;
};

class AddSlide : public Command {
public:
    AddSlide();
    std::pair<bool, std::variant<int, double, std::string>> findOption(const std::string&) const override;
    void showOptions(int intend = 0) const;
    void execute(Project& project) override;
    void setOption(const std::string& optionName, std::variant<int, double, std::string> value) override;

private:
    OptionLib m_opt;    
};

class RemoveSlide : public Command {
public:
    RemoveSlide();
    std::pair<bool, std::variant<int, double, std::string>> findOption(const std::string&) const override;
    void showOptions(int intend = 0) const;
    void execute(Project& project) override;
    void setOption(const std::string& optionName, std::variant<int, double, std::string> value) override;
 
private:
    OptionLib m_opt;    
};

class Help : public Command {
public:
    void setLib(std::shared_ptr<CommandLib> lib);
    void execute(Project& project) override;
private:
    std::shared_ptr<CommandLib> m_megaComLib;   
};

class Parser {
private:
    
    void tokenize(const std::string&);
    
public:
    std::vector<std::string> parse(std::istream&);
    std::shared_ptr<Command> validate();
    Parser();

private:
    std::vector<std::string> m_tokens;
    std::shared_ptr<CommandLib> m_comLib;
};

class Controller{
public:
    void run(std::istream&);
private:
    Parser m_parser;    
};



#endif //CONTROLLER_H