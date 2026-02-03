#include "Controller.h"
#include <string>
#include <stdexcept>
#include <cctype>

Project project;

std::pair<bool, std::variant<int, double, std::string>> OptionLib::findOption(const std::string& opt) const{
    auto it = m_optLib.find(opt);
    if (it == m_optLib.end()){
        return {false, std::variant<int, double, std::string>("")};
    }
    return {true, it->second};
}

void OptionLib::setOption(const std::string& optionName, std::variant<int,double,std::string> option) {
    auto it = m_optLib.find(optionName);
    if (it == m_optLib.end()) {
        throw std::runtime_error("Option not found: " + optionName);
    }
    it->second = option;
}

void OptionLib::addOption(const std::string& optionName, std::variant<int, double, std::string> option){
    m_optLib.insert({optionName, option});
}

const std::map<std::string, std::variant<int, double, std::string>>& OptionLib::options() const{
    return m_optLib;
}

AddShape::AddShape(){
    std::shared_ptr<Command> add_rect = std::make_shared<AddRect>();
    m_comLib.insert({"-add_rect", add_rect});
}

std::pair<bool, std::shared_ptr<Command>> AddShape::findCommand(std::string CommandName) const{
    auto it = m_comLib.find(CommandName);
    if (it == m_comLib.end()){
        return {false, nullptr};
    } 
    return {true, it->second};
}

void AddShape::showCommand(int indent) const {
    for (auto& elem : m_comLib) {
        std::cout << std::string(indent, ' ') << "Command: " << elem.first << std::endl;
        elem.second->showCommand(indent + 4);
        elem.second->showOptions(indent + 4);
    }
}

AddRect::AddRect(){
    double llx = 0.0;
    m_opt.addOption("-llx", llx);
    double lly = 0.0;
    m_opt.addOption("-lly", lly);
    double urx = 0.0;
    m_opt.addOption("-urx", urx);
    double ury = 0.0;
    m_opt.addOption("-ury", ury);
    int id = 1;
    m_opt.addOption("-slide_id", id);

}

std::pair<bool, std::variant<int, double, std::string>> AddRect::findOption(const std::string& optionName) const{
    return m_opt.findOption(optionName);       

}

void End::execute(Project& project){

}

void AddRect::showOptions(int indent) const{
    for(auto& elem : m_opt.options()){
        std::cout << std::string(indent, ' ') << "Option: " << elem.first << std::endl;
    }    
}

RemoveShape::RemoveShape(){
    int slideID = 1;
    m_opt.addOption("-slide_id", slideID);
    int shapeID = 1;
    m_opt.addOption("-shape_id", shapeID);
}

std::pair<bool, std::variant<int, double, std::string>> RemoveShape::findOption(const std::string& optionName) const{
    return m_opt.findOption(optionName);
}

void RemoveShape::showOptions(int indent) const{
    for(auto& elem : m_opt.options()){
        std::cout << std::string(indent, ' ') << "Option: " << elem.first << std::endl;
    }     
}

CommandLib::CommandLib(){
    std::shared_ptr<Command> add_shape = std::make_shared<AddShape>();
    m_comLib.insert({"-add_shape", add_shape});
    std::shared_ptr<Command> remove_shape = std::make_shared<RemoveShape>();
    m_comLib.insert({"-remove_shape", remove_shape});
    std::shared_ptr<Command> add_slide = std::make_shared<AddSlide>();
    m_comLib.insert({"-add_slide", add_slide});
    std::shared_ptr<Command> remove_slide = std::make_shared<RemoveSlide>();
    m_comLib.insert({"-remove_slide", remove_slide});
    std::shared_ptr<Command> help = std::make_shared<Help>();
    m_comLib.insert({"-help", help});
    std::shared_ptr<Command> end = std::make_shared<End>();
    m_comLib.insert({"-end", end});    
    std::shared_ptr<Command> save = std::make_shared<Save>();
    m_comLib.insert({"-save", save});

}

Save::Save(){
    std::string filename = "default.txt";
    m_optLib.addOption("-filename", filename);
    std::string format = "csv";
    m_optLib.addOption("-format", format); 
}

void Save::showOptions(int indent) const{
    for(auto& elem : m_optLib.options()){
        std::cout << std::string(indent, ' ') << "Option: " << elem.first << std::endl;
    }      
}

std::pair<bool, std::variant<int, double, std::string>> Save::findOption(const std::string& optionName) const{
    return m_optLib.findOption(optionName);
}

void Save::execute(Project& project){
    if(std::get<std::string>(findOption("-format").second) == "csv"){
        project.serializeCSV(std::get<std::string>(findOption("-filename").second));
    }
}

void Save::setOption(const std::string& optName, std::variant<int,double,std::string> value) {
    m_optLib.setOption(optName, value);
}

const std::map<std::string, std::shared_ptr<Command>>& CommandLib::commands() const{
    return m_comLib;
}

std::pair<bool, std::shared_ptr<class Command>> CommandLib::findCommand(const std::string& command){
    auto it = m_comLib.find(command);
    if (it == m_comLib.end()){
        return {false, nullptr};
    }
    return {true, it->second};
}

void CommandLib::showCommandLib(int indent) const{
    for(auto& elem : m_comLib){
        std::cout << std::string(indent, ' ') << "Command: " << elem.first << std::endl;
        elem.second->showCommand(indent + 4);
        elem.second->showOptions(indent + 6);
    }
}

AddSlide::AddSlide(){
    int ID = 1;
    m_opt.addOption("-id", ID);
}

void AddSlide::showOptions(int indent) const{
    for(auto& elem : m_opt.options()){
        std::cout << std::string(indent, ' ') << "Option: " << elem.first << std::endl;
    }      
}


std::pair<bool, std::variant<int, double, std::string>> AddSlide::findOption(const std::string& optionName) const{
    return m_opt.findOption(optionName);
}

RemoveSlide::RemoveSlide(){
    int ID = 1;
    m_opt.addOption("-id", ID);
}

std::pair<bool, std::variant<int, double, std::string>> RemoveSlide::findOption(const std::string& optionName) const{
    return m_opt.findOption(optionName);
}

void RemoveSlide::showOptions(int indent) const{
    for(auto& elem : m_opt.options()){
        std::cout << std::string(indent, ' ') << "Option: " << elem.first << std::endl;
    }      
}

void Help::setLib(std::shared_ptr<class CommandLib> lib){
    m_megaComLib = lib;
}

void AddRect::execute(Project& project){
    std::shared_ptr<Shape> r = std::make_shared<Rectangle>(
        std::pair<double,double>{std::get<double>(findOption("-llx").second), std::get<double>(findOption("-lly").second)},
        std::pair<double,double>{std::get<double>(findOption("-urx").second), std::get<double>(findOption("-ury").second)});
    project.get_slide(std::get<int>(findOption("-slide_id").second)).add_shape(r);
}

void AddRect::setOption(const std::string& optName, std::variant<int,double,std::string> value) {
    m_opt.setOption(optName, value);
} 

void RemoveShape::execute(Project& project){
    project.get_slide(std::get<int>(findOption("-slide_id").second)).remove_shape(std::get<int>(findOption("-shape_id").second));
}

void RemoveShape::setOption(const std::string& optName, std::variant<int,double,std::string> value) {
    m_opt.setOption(optName, value);
}

void AddSlide::execute(Project& project){
    auto opt = findOption("-id");
    if (opt.first) {
        int id = std::get<int>(opt.second);
        project.add_slide(id);
    }
}

void AddSlide::setOption(const std::string& optName, std::variant<int,double,std::string> value) {
    m_opt.setOption(optName, value);
}

void RemoveSlide::execute(Project& project){
    auto opt = findOption("-id");
    if (opt.first) {
        int id = std::get<int>(opt.second);
        project.remove_slide(id);
    }
}

void RemoveSlide::setOption(const std::string& optName, std::variant<int,double,std::string> value) {
    m_opt.setOption(optName, value);
}

void Help::execute(Project& project){
    m_megaComLib->showCommandLib();
}

Parser::Parser()
    :m_tokens{}
    ,m_comLib{std::make_shared<CommandLib>()}
{
    auto res = m_comLib->findCommand("-help");
    if(res.first) {
        res.second->setLib(m_comLib);
    }    
}

void Parser::tokenize(const std::string& commandLine){
    m_tokens.clear();
    std::string tempWord;        
    for(int i = 0; i < commandLine.size(); ++i){
        if(!std::isspace(commandLine[i])){
            tempWord += commandLine[i];
        }
        else{
            m_tokens.push_back(tempWord);
            tempWord = "";
        }
    }

    if(!tempWord.empty()){
        m_tokens.push_back(tempWord);
    }
}

bool possible_stoi(const std::string& text){
    for(int i = 0; i < text.size(); ++i){
        if(text[i] < '0' || text[i] > '9'){
            return false;
        }
    }
    return true;
}

bool possible_stod(const std::string& text) {
    if (text.empty()) return false;
    int dots = 0;
    size_t start = 0;
    if (text[0] == '+' || text[0] == '-') {
        start = 1;
    }
    for (size_t i = start; i < text.size(); ++i) {
        if (text[i] == '.') {
            ++dots;
            if (dots > 1) {
                return false;
            }
        } 
        else if (text[i] < '0' || text[i] > '9') {
            return false;
        }
    }
    return true;
}

std::shared_ptr<Command> Parser::validate() {
    if (m_tokens.empty()) {
        return nullptr;
    }
    auto mainRes = m_comLib->findCommand(m_tokens[0]);
    if (mainRes.first && m_tokens[0] == "-end") {
        return mainRes.second;
    }
    if (!mainRes.first) {
        throw std::runtime_error("invalid command: " + m_tokens[0]);
    }
    auto mainCmd = mainRes.second;
    if (m_tokens.size() >= 2) {
        auto subRes = mainCmd->findCommand(m_tokens[1]);
        if (subRes.first) {
            auto target = subRes.second;
            for (size_t i = 2; i + 1 < m_tokens.size(); i += 2) {
                const std::string opt = m_tokens[i];
                const std::string val = m_tokens[i+1];
                auto optCheck = target->findOption(opt);
                if (!optCheck.first) {
                    throw std::runtime_error("invalid option: " + opt);
                }
                if (opt == "-slide_id" || opt == "-shape_id" ||  opt == "-id") {
                    if (!possible_stoi(val)) {
                        throw std::runtime_error("invalid integer value: " + val);
                    }
                    int v = std::stoi(val);
                    target->setOption(opt, v);
                } 
                else {
                    if (!possible_stod(val)) {
                        throw std::runtime_error("invalid numeric value: " + val);
                    }
                    double v = std::stod(val);
                    target->setOption(opt, v);
                }
            }
            return target;
        } 
        else {
            auto target = mainCmd;
            for (size_t i = 1; i + 1 < m_tokens.size(); i += 2) {
                const std::string opt = m_tokens[i];
                const std::string val = m_tokens[i+1];
                auto optCheck = target->findOption(opt);
                if (!optCheck.first) {
                    throw std::runtime_error("invalid option: " + opt);
                }
                if (opt == "-slide_id" || opt == "-shape_id" || opt == "-id") {
                    if (!possible_stoi(val)) {
                        throw std::runtime_error("invalid integer value: " + val);
                    }
                    int v = std::stoi(val);
                    target->setOption(opt, v);
                } 
                else if (opt == "-filename" || opt == "-format") {
                    target->setOption(opt, val);
                } 
                else {
                    if (!possible_stod(val)) {
                        throw std::runtime_error("invalid numeric value: " + val);
                    }
                    double v = std::stod(val);
                    target->setOption(opt, v);
                }
            }
            return target;
        }
    }
    return mainCmd;
}

std::vector<std::string> Parser::parse(std::istream& in) {
    std::string command_line;
    if (!std::getline(in, command_line)) {
        return {};
    }    
    tokenize(command_line);
    validate();
    return m_tokens;
}

void Controller::run(std::istream& in){
while (true) {
        try {
            m_parser.parse(in);
            auto command = m_parser.validate();
            if (command == nullptr) {
                return;
            }
            command->execute(project);
            if (command->isEnd()) {
                return;
            }
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
}